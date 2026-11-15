/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
// Generates the Windows icons of MediaInfo and the MSIX/UWP image assets.
//
// Build (C++11, only dependency: zlib, the same one as MediaInfoLib):
//   Windows, from a Visual Studio "x64 Native Tools" prompt, in this directory, zlib next to MediaInfo:
//     cl /nologo /O2 /EHsc /fp:precise /I..\..\..\zlib MediaInfo_Windows_Icons.cpp
//        ..\..\..\zlib\adler32.c ..\..\..\zlib\compress.c ..\..\..\zlib\crc32.c
//        ..\..\..\zlib\deflate.c ..\..\..\zlib\trees.c ..\..\..\zlib\zutil.c
//   Linux/macOS:
//     c++ -O2 -std=c++11 -ffp-contract=off MediaInfo_Windows_Icons.cpp -lz
// Usage:
//   MediaInfo_Windows_Icons [path to Source/Resource/Image]
//   (default: ../Resource/Image, Source/Resource/Image or the current directory, the first one found)
//
// Outputs (in Source/Resource/Image):
// - MediaInfo.ico          GUI (VCL, Qt), CLI, NSIS installer/uninstaller
// - MediaInfo_TinyOnly.ico 16x16 only (wxWidgets GUI)
// - MSIX_Assets/*.png      sparse package (VCL GUI) and Qt MSIX package
// - Assets/*.png           UWP GUI, regenerated in place (same names and sizes)
//
// Design:
// - The logo geometry comes from MediaInfo.svg (45x45 units).
// - Windows can not switch .ico content with the theme, and the logo is mostly black,
//   so the .ico icons put the logo on a white backing following the logo silhouette
//   (RGB bars + outer arc) with a thin white rim around the outer crescent:
//   pixel-identical to the logo on light backgrounds, a white "sticker" on dark ones
//   (dark taskbar, Explorer/Firefox dark themes, dark title bars...).
// - The logo is inverted (white disc and crescents, black "?", transparent background)
//   wherever Windows draws it on a known dark or colored background: dark theme assets
//   ("unplated"), Windows 10 plated icons and tiles (accent color or dark tiles, the
//   default assets of the theme-aware tiles), UWP lock screen. This is what Microsoft
//   recommends for theme-aware assets and what the macOS/iOS dark icons do. Light theme
//   assets ("lightunplated", "colorful_theme-light") use the bare logo. Only the assets
//   drawn on an unknown background (.ico, UWP splash screen and Store logo) use the backing.
// - Small sizes favor readability: 16, 20 and 24 px are hand-drawn pixel art, 30 to 48 px
//   are laid out on the pixel grid with quantized anti-aliasing; the disc and the "?" are
//   enlarged so the "?" stays visible.
// - ICO contains 16 20 24 30 32 36 40 48 60 64 72 80 96 (32-bit BMP) and 256 (PNG)
//   i.e. every size used by Windows 10/11 at 100-400% scaling, Windows 7 compatible.
//
// Computations are done with explicit float32/float64 steps (no fused multiply-add),
// so the output files are byte-identical on any platform with the same zlib output.
//---------------------------------------------------------------------------

#include <zlib.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <sys/stat.h>
#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
#else
    #include <dirent.h>
#endif

using namespace std;

//---------------------------------------------------------------------------
// File system helpers (C++11 has no std::filesystem)
static bool IsDir(const string& Path)
{
    struct stat St;
    return stat(Path.c_str(), &St) == 0 && (St.st_mode & S_IFDIR);
}

static bool IsFile(const string& Path)
{
    struct stat St;
    return stat(Path.c_str(), &St) == 0 && (St.st_mode & S_IFREG);
}

static void MakeDir(const string& Path)
{
    if (IsDir(Path))
        return;
    #ifdef _WIN32
        _mkdir(Path.c_str());
    #else
        mkdir(Path.c_str(), 0755);
    #endif
}

static vector<string> ListDir(const string& Path)
{
    vector<string> Names;
    #ifdef _WIN32
        _finddata_t Data;
        intptr_t Handle = _findfirst((Path + "/*").c_str(), &Data);
        if (Handle != -1)
        {
            do
                if (!(Data.attrib & _A_SUBDIR))
                    Names.push_back(Data.name);
            while (_findnext(Handle, &Data) == 0);
            _findclose(Handle);
        }
    #else
        if (DIR* D = opendir(Path.c_str()))
        {
            while (dirent* E = readdir(D))
                if (IsFile(Path + "/" + E->d_name))
                    Names.push_back(E->d_name);
            closedir(D);
        }
    #endif
    sort(Names.begin(), Names.end());
    return Names;
}

static bool EndsWith(const string& S, const char* End)
{
    size_t n = strlen(End);
    return S.size() >= n && S.compare(S.size() - n, n, End) == 0;
}

static bool StartsWith(const string& S, const char* Begin)
{
    return S.compare(0, strlen(Begin), Begin) == 0;
}

//---------------------------------------------------------------------------
// Basic types
struct Pt { double x, y; };
typedef vector<Pt> Poly;
typedef vector<Poly> Polys;
struct Color { uint8_t r, g, b; };

static const Color RED   = {0xFF, 0x03, 0x02};
static const Color GREEN = {0x01, 0xD3, 0x18};
static const Color BLUE  = {0x12, 0x0F, 0xFE};
static const Color BLACK = {0x00, 0x00, 0x00};
static const Color WHITE = {0xFF, 0xFF, 0xFF};

static const double PI = 3.141592653589793;

struct Image
{
    int W, H;
    vector<uint8_t> Px; // RGBA, top-down
    Image() : W(0), H(0) {}
    Image(int W_, int H_) : W(W_), H(H_), Px((size_t)W_ * H_ * 4, 0) {}
    uint8_t* At(int x, int y) { return &Px[((size_t)y * W + x) * 4]; }
    const uint8_t* At(int x, int y) const { return &Px[((size_t)y * W + x) * 4]; }
};

static int Clamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }
static int RoundHalfEven(double v)      { return (int)nearbyint(v); } // like Python round()

//---------------------------------------------------------------------------
// Rasterizer: union of polygons (pixel units) -> coverage, supersampled ss x ss
static vector<float> Coverage(const Polys& Ps, int S, int ss)
{
    const int N = S * ss;
    vector<uint8_t> Acc((size_t)N * N, 0);
    vector<int32_t> Delta;
    for (size_t k = 0; k < Ps.size(); k++)
    {
        const Poly& P = Ps[k];
        Delta.assign((size_t)N * (N + 1), 0);
        bool Any = false;
        size_t n = P.size();
        for (size_t i = 0; i < n; i++)
        {
            double x0 = P[i].x, y0 = P[i].y, x1 = P[(i + 1) % n].x, y1 = P[(i + 1) % n].y;
            if (y0 == y1)
                continue;
            int r0 = Clamp((int)ceil(min(y0, y1) * ss - 0.5), 0, N);
            int r1 = Clamp((int)ceil(max(y0, y1) * ss - 0.5), 0, N);
            int Sign = y1 > y0 ? 1 : -1;
            for (int r = r0; r < r1; r++)
            {
                double yc = (r + 0.5) / ss;
                double t = (yc - y0) / (y1 - y0);
                double dx = t * (x1 - x0);
                double xc = x0 + dx;
                int Col = Clamp((int)ceil(xc * ss - 0.5), 0, N);
                Delta[(size_t)r * (N + 1) + Col] += Sign;
                Any = true;
            }
        }
        if (!Any)
            continue;
        for (int r = 0; r < N; r++)
        {
            int32_t Winding = 0;
            const int32_t* D = &Delta[(size_t)r * (N + 1)];
            uint8_t* A = &Acc[(size_t)r * N];
            for (int c = 0; c < N; c++)
            {
                Winding += D[c];
                if (Winding)
                    A[c] = 1;
            }
        }
    }
    vector<float> Cov((size_t)S * S);
    const double Area = (double)ss * ss;
    for (int y = 0; y < S; y++)
        for (int x = 0; x < S; x++)
        {
            int Count = 0;
            for (int j = 0; j < ss; j++)
            {
                const uint8_t* A = &Acc[(size_t)(y * ss + j) * N + (size_t)x * ss];
                for (int i = 0; i < ss; i++)
                    Count += A[i];
            }
            Cov[(size_t)y * S + x] = (float)(Count / Area);
        }
    return Cov;
}

//---------------------------------------------------------------------------
struct Quant { double Lo, Hi, Step; };

class Canvas
{
public:
    int S, ss;
    bool HasQuant;
    Quant Q;
    vector<float> R, G, B, A; // premultiplied

    Canvas(int S_, const Quant* Q_) : S(S_), ss(max(8, min(64, 4096 / S_))), HasQuant(Q_ != NULL),
        R((size_t)S_ * S_, 0), G((size_t)S_ * S_, 0), B((size_t)S_ * S_, 0), A((size_t)S_ * S_, 0)
    {
        Quant Zero = {0, 0, 0};
        Q = Q_ ? *Q_ : Zero;
    }

    // Quantized anti-aliasing: coverage < Lo -> 0, > Hi -> 1, else rounded to Step
    vector<float> Cov(const Polys& Ps) const
    {
        vector<float> C = Coverage(Ps, S, ss);
        if (HasQuant)
        {
            const float Lo = (float)Q.Lo, Hi = (float)Q.Hi, Step = (float)Q.Step, Max = (float)(1 - Q.Step);
            for (size_t i = 0; i < C.size(); i++)
            {
                float c = C[i];
                if (c < Lo)
                    c = 0;
                else if (c > Hi)
                    c = 1;
                else
                {
                    float v = nearbyint(c / Step);
                    v = v * Step;
                    v = max(v, Step);
                    c = min(v, Max);
                }
                C[i] = c;
            }
        }
        return C;
    }

    void Paint(const vector<float>& Cv, Color Col)
    {
        const float Cr = Col.r, Cg = Col.g, Cb = Col.b;
        for (size_t i = 0; i < Cv.size(); i++)
        {
            // source-over, premultiplied (separate statements: no fused multiply-add)
            float c = Cv[i];
            float Inv = 1 - c;
            float p, t;
            p = Cr * c; t = R[i] * Inv; R[i] = p + t;
            p = Cg * c; t = G[i] * Inv; G[i] = p + t;
            p = Cb * c; t = B[i] * Inv; B[i] = p + t;
            t = A[i] * Inv; A[i] = c + t;
        }
    }

    void Fill(const Polys& Ps, Color Col) { Paint(Cov(Ps), Col); }
    void Fill(const Poly& P, Color Col)   { Fill(Polys(1, P), Col); }

    // Pixel art ('#' = painted pixel), top-left at (x0, y0)
    void Pixels(const vector<string>& Pattern, int x0, int y0, Color Col)
    {
        vector<float> Cv((size_t)S * S, 0);
        for (size_t j = 0; j < Pattern.size(); j++)
            for (size_t i = 0; i < Pattern[j].size(); i++)
                if (Pattern[j][i] == '#')
                    Cv[(size_t)(y0 + (int)j) * S + x0 + i] = 1;
        Paint(Cv, Col);
    }

    Image Rgba() const
    {
        Image Out(S, S);
        for (size_t i = 0; i < A.size(); i++)
        {
            uint8_t* P = &Out.Px[i * 4];
            float a = A[i];
            if (a > 0)
            {
                float m = max(a, 1e-6f);
                float v;
                v = nearbyint(R[i] / m); P[0] = (uint8_t)min(max(v, 0.0f), 255.0f);
                v = nearbyint(G[i] / m); P[1] = (uint8_t)min(max(v, 0.0f), 255.0f);
                v = nearbyint(B[i] / m); P[2] = (uint8_t)min(max(v, 0.0f), 255.0f);
            }
            float Al = nearbyint(a * 255);
            P[3] = (uint8_t)min(max(Al, 0.0f), 255.0f);
            if (!P[3])
                P[0] = P[1] = P[2] = 0;
        }
        return Out;
    }
};

//---------------------------------------------------------------------------
// Shapes
static Poly Arc(double cx, double cy, double rx, double ry, double a0, double a1, int n = -1)
{
    if (n < 0)
        n = max(16, (int)(fabs(a1 - a0) * max(rx, ry) * 4));
    Poly P(n + 1);
    double Step = (a1 - a0) / n; // same points as numpy.linspace(a0, a1, n + 1)
    for (int i = 0; i <= n; i++)
    {
        double t = i == n ? a1 : (double)i * Step + a0;
        double c = rx * cos(t), s = ry * sin(t);
        P[i].x = cx + c;
        P[i].y = cy + s;
    }
    return P;
}

static Poly Ellipse(double cx, double cy, double rx, double ry)
{
    Poly P = Arc(cx, cy, rx, ry, 0, 2 * PI);
    P.pop_back();
    return P;
}

// Corners: radius factor of top-left, top-right, bottom-right, bottom-left corners
static Poly RRect(double x0, double y0, double x1, double y1, double r, const double* Corners = NULL)
{
    static const double All[4] = {1, 1, 1, 1};
    if (!Corners)
        Corners = All;
    const double Def[4][5] = {{1, 1, PI, x0, y0}, {-1, 1, 1.5 * PI, x1, y0}, {-1, -1, 0, x1, y1}, {1, -1, 0.5 * PI, x0, y1}};
    Poly P;
    for (int k = 0; k < 4; k++)
    {
        double sx = Def[k][0], sy = Def[k][1], a0 = Def[k][2], px = Def[k][3], py = Def[k][4];
        double rc = max(0.0, min(min(r * Corners[k], (x1 - x0) / 2), (y1 - y0) / 2));
        if (rc > 0)
        {
            Poly Q = Arc(px + sx * rc, py + sy * rc, rc, rc, a0, a0 + PI / 2, 16);
            P.insert(P.end(), Q.begin(), Q.end());
        }
        else
        {
            Pt Corner = {px, py};
            P.push_back(Corner);
        }
    }
    return P;
}

struct EllipseDef { double cx, cy, rx, ry; };

// Right half of an ellipse, top to bottom
static Poly HalfEllipse(double cx, double cy, double rx, double ry)
{
    return Arc(cx, cy, rx, ry, -PI / 2, PI / 2);
}

static Poly Crescent(const EllipseDef& Outer, const EllipseDef& Inner)
{
    Poly P = HalfEllipse(Outer.cx, Outer.cy, Outer.rx, Outer.ry);
    Poly I = HalfEllipse(Inner.cx, Inner.cy, Inner.rx, Inner.ry);
    P.insert(P.end(), I.rbegin(), I.rend());
    return P;
}

// Absolute M/L/C/Z SVG path -> polygons
static Polys SvgPath(const char* D, double Scale)
{
    Polys Out;
    Poly Cur;
    Pt Pos = {0, 0};
    char Cmd = 0;
    const char* p = D;
    for (;;)
    {
        while (*p == ' ')
            p++;
        if (!*p)
            break;
        if (strchr("MLCZz", *p))
        {
            Cmd = *p++;
            if (Cmd == 'Z' || Cmd == 'z')
            {
                for (size_t i = 0; i < Cur.size(); i++)
                {
                    Cur[i].x = Cur[i].x * Scale;
                    Cur[i].y = Cur[i].y * Scale;
                }
                Out.push_back(Cur);
                Cur.clear();
            }
            continue;
        }
        int Count = Cmd == 'C' ? 6 : 2;
        double V[6];
        for (int i = 0; i < Count; i++)
        {
            while (*p == ' ')
                p++;
            char* End;
            V[i] = strtod(p, &End);
            p = End;
        }
        if (Cmd == 'M')
            Cur.clear();
        if (Cmd == 'M' || Cmd == 'L')
        {
            Pt Q = {V[0], V[1]};
            Cur.push_back(Q);
        }
        else if (Cmd == 'C')
        {
            const double Step = 1.0 / 24; // same t as numpy.linspace(0, 1, 25)[1:]
            for (int i = 1; i <= 24; i++)
            {
                double t = i == 24 ? 1.0 : (double)i * Step;
                double u = 1 - t;
                double c0 = pow(u, 3.0), c1 = 3 * (u * u), c2 = 3 * u, c3 = pow(t, 3.0);
                double c1t = c1 * t, c2t = c2 * t;
                double c2tt = c2t * t;
                double ax = c0 * Pos.x, ay = c0 * Pos.y;
                double bx = c1t * V[0], by = c1t * V[1];
                double cx = c2tt * V[2], cy = c2tt * V[3];
                double dx = c3 * V[4], dy = c3 * V[5];
                Pt Q;
                Q.x = ax + bx; Q.x = Q.x + cx; Q.x = Q.x + dx;
                Q.y = ay + by; Q.y = Q.y + cy; Q.y = Q.y + dy;
                Cur.push_back(Q);
            }
        }
        Pos = Cur.back();
    }
    return Out;
}

// "?" of MediaInfo_Monochrome.svg (1190 units == 45 logo units)
static const Polys& QMark()
{
    static const Polys Q = SvgPath(
        "M 489.0141 486.99872 C 509.0141 486.99872 524.98798 492.01841 535.98798 503.01841 C 547.98798 513.01841 554.02305 528.0132 554.02305 546.0132 "
        "C 554.02305 555.0132 551.97691 564.0246 547.97691 571.0246 C 543.97691 579.0246 537.97813 587.00246 527.97813 595.00246 L 518.00458 603.01231 "
        "C 512.00458 608.01231 507.98211 612.99848 504.98211 617.99848 C 501.98211 621.99848 501.00303 627.01817 501.00303 634.01817 L 466.01808 634.01817 "
        "C 467.01808 624.01817 468.97625 614.9914 473.97625 606.9914 C 477.97625 598.9914 484.99318 591.97723 492.99318 584.97723 "
        "C 501.99318 577.97723 509.01011 571.01198 512.01011 566.01198 C 516.01011 560.01198 518.00458 553.99506 518.00458 546.99506 "
        "C 518.00458 538.99506 515.97658 531.99628 510.97658 526.99628 C 505.97658 521.99628 498.99595 518.98643 489.99595 518.98643 "
        "C 480.99595 518.98643 473.98179 521.97813 467.98179 527.97813 C 461.98179 532.97813 459.00824 540.99229 458.00824 549.99229 "
        "L 421.98976 549.99229 C 423.98976 529.99229 431.02207 515.00026 443.02207 504.00026 C 455.02207 493.00026 470.0141 486.99872 489.0141 486.99872 Z "
        "M 466.01808 657.01418 L 501.98488 657.01418 L 501.98488 694.99636 L 466.01808 694.99636 L 466.01808 657.01418 Z", 45.0 / 1190);
    return Q;
}

// Pixel "?" (bold) for the small discs
static const map<string, vector<string> > QMarkPixels = {
    {"3x5", {"##.",
             "..#",
             ".#.",
             "...",
             ".#."}},
    {"4x6", {".##.",
             "#..#",
             "..#.",
             ".#..",
             "....",
             ".#.."}},
    {"5x7", {".###.",
             "##.##",
             "...##",
             "..##.",
             "..##.",
             ".....",
             "..##."}},
    {"6x8", {".####.",
             "##..##",
             "....##",
             "...##.",
             "..##..",
             "..##..",
             "......",
             "..##.."}},
    {"7x9", {".#####.",
             "##...##",
             ".....##",
             "....##.",
             "...##..",
             "...##..",
             ".......",
             "...##..",
             "...##.."}},
};

//---------------------------------------------------------------------------
// Logo geometry (pixel units, logo in a S x S box)
//
// Master (MediaInfo.svg, 45x45 units): RGB bars 3 x 2.555 wide; disc centre (18.48, 22.72)
// radii (6.42, 6.70); inner crescent between half ellipses centred x=20.69 (rx 10.58, ry 13.26)
// and x=21.59 (rx 13.74, ry 13.29); outer crescent between half ellipses centred x=18.77
// (rx 21.23 and 26.25, ry 22.5). "rim" is the width of the white rim right of the outer
// crescent, the logo is horizontally squeezed to make room for it.
//
// Small sizes are laid out on the pixel grid, in whole pixels from left to right:
// width of each RGB bar, gap, disc, gap, inner crescent, gap, outer crescent, white rim.
// The disc is enlarged to hold a readable bold pixel "?" (or a bigger vector "?"),
// the inner crescent wraps it, and the anti-aliasing is quantized (faint isolated
// pixels removed, clean steps kept).
struct SmallDef { int Bar, G1, Disc, G2, Ti, G3, To, Rim; const char* Q; bool HasQuant; Quant Qt; };
static const map<int, SmallDef> Small = {
    //     bar g1 disc g2  ic g3  oc rim   "?"   AA quantization
    {16, {1, 1,  7, 1,  1, 1,  2, 0, "3x5", true,  {0.30, 0.70, 0.50}}},
    {20, {1, 1,  8, 2,  2, 1,  2, 1, "4x6", true,  {0.30, 0.70, 0.50}}},
    {24, {1, 2,  9, 2,  2, 2,  3, 1, "5x7", true,  {0.25, 0.75, 0.50}}},
    {30, {2, 2, 11, 2,  3, 2,  3, 1, "5x7", true,  {0.20, 0.80, 0.25}}},
    {32, {2, 3, 12, 2,  3, 2,  3, 1, "6x8", true,  {0.20, 0.80, 0.25}}},
    {36, {2, 3, 13, 3,  3, 3,  4, 1, "7x9", true,  {0.20, 0.80, 0.25}}},
    {40, {2, 4, 15, 3,  4, 3,  4, 1, "v",   false, {0, 0, 0}}},
    {48, {3, 4, 16, 4,  4, 4,  5, 2, "v",   false, {0, 0, 0}}},
};
static const map<int, double> QMarkScale = {{40, 1.25}, {48, 1.25}}; // vector "?" size factor (1 = MediaInfo.svg proportions)

struct Geometry
{
    int S;
    double Rim, Bar, BarR;
    bool HasQuant;
    Quant Qt;
    EllipseDef Disc;
    string Q;
    double QScale;
    double OcCx, OcRxOut, OcRxIn;
    EllipseDef IcOut, IcIn;
};

static EllipseDef MakeEllipse(double cx, double cy, double rx, double ry)
{
    EllipseDef E = {cx, cy, rx, ry};
    return E;
}

static Geometry SmallGeometry(int S, bool Backing)
{
    const SmallDef& D = Small.at(S);
    int Bar = D.Bar, G1 = D.G1, d = D.Disc, G2 = D.G2, Ti = D.Ti, G3 = D.G3, To = D.To, Rim = D.Rim;
    if (!Backing) // bare logo: the rim pixels go to the outer crescent and the gap before it
    {
        To += Rim / 2;
        G3 += Rim - Rim / 2;
        Rim = 0;
    }
    int x = 3 * Bar + G1;
    double r = d / 2.0;
    double dcx = x + r, dcy = (S - d) % 2 == 0 ? S / 2.0 : S / 2.0 + 0.5;
    int IcIn = x + d + G2, IcOut = IcIn + Ti, OcIn = IcOut + G3, OcOut = OcIn + To;
    double cx = dcx + 0.12 * d, ry = 1.6 * r; // crescent tips a bit right of the disc centre, as in the master
    double OcCx = 18.77 * OcOut / 45;
    Geometry G;
    G.S = S; G.Rim = Rim; G.Bar = Bar; G.BarR = 1.2 * S / 45; G.HasQuant = D.HasQuant; G.Qt = D.Qt;
    G.Disc = MakeEllipse(dcx, dcy, r, r); G.Q = D.Q;
    map<int, double>::const_iterator It = QMarkScale.find(S);
    G.QScale = It == QMarkScale.end() ? 1.0 : It->second;
    G.OcCx = OcCx; G.OcRxOut = OcOut - OcCx; G.OcRxIn = OcIn - OcCx;
    G.IcOut = MakeEllipse(cx, dcy, IcOut - cx, ry);
    G.IcIn = MakeEllipse(cx, dcy, IcIn - cx, ry);
    return G;
}

struct Hints { int Rim, Bar, To, Gap, Ti, D, Dx; double Dcy; string Q; };

// Other sizes below 128 px: master proportions rounded to whole pixels
static Hints AutoHints(int S, bool Rim)
{
    Hints H;
    H.Rim = Rim ? max(1, RoundHalfEven(S * 1.5 / 45)) : 0;
    double k = (S - H.Rim) / 45.0;
    H.Bar = max(1, RoundHalfEven(2.555 * k));
    H.To = max(1, RoundHalfEven(5.02 * k));
    H.Gap = max(1, RoundHalfEven(4.67 * k));
    H.Ti = max(1, RoundHalfEven(4.06 * k));
    H.D = max(2, RoundHalfEven(12.84 * k));
    H.Dx = max(RoundHalfEven(12.06 * k), 3 * H.Bar + 1);
    H.D = min(H.D, S - H.Rim - H.To - H.Gap - H.Ti - 1 - H.Dx);
    H.Q = H.D >= 13 ? "v" : H.D >= 10 ? "4x6" : H.D >= 7 ? "3x5" : "";
    H.Dcy = RoundHalfEven(22.72 * S / 45 - H.D / 2.0) + H.D / 2.0;
    return H;
}

static Geometry MakeGeometry(int S, bool Rim)
{
    if (Small.count(S))
        return SmallGeometry(S, Rim);
    double u = S / 45.0;
    Geometry G;
    G.S = S; G.HasQuant = false;
    Quant Zero = {0, 0, 0};
    G.Qt = Zero;
    if (S >= 128) // exact master geometry
    {
        double r = Rim ? 1.5 * u : 0.0;
        double k = (S - r) / 45;
        G.Rim = r; G.Bar = 2.555 * k; G.BarR = 1.2 * u; G.QScale = 1.0;
        G.Disc = MakeEllipse(18.48 * k, 22.72 * u, 6.42 * k, 6.70 * u); G.Q = "v";
        G.OcCx = 18.77 * k; G.OcRxOut = 26.25 * k; G.OcRxIn = 21.23 * k;
        G.IcOut = MakeEllipse(21.59 * k, S / 2.0, 13.74 * k, 13.29 * u);
        G.IcIn = MakeEllipse(20.69 * k, S / 2.0, 10.58 * k, 13.26 * u);
        return G;
    }
    Hints H = AutoHints(S, Rim);
    int R = S - H.Rim;
    double k = R / 45.0;
    int IcR = R - H.To - H.Gap;
    int d = H.D;
    G.Rim = H.Rim; G.Bar = H.Bar; G.BarR = 1.2 * u; G.QScale = S < 64 ? 1.15 : 1.0;
    G.Disc = MakeEllipse(H.Dx + d / 2.0, H.Dcy, d / 2.0, d / 2.0); G.Q = H.Q;
    G.OcCx = 18.77 * k; G.OcRxOut = R - 18.77 * k; G.OcRxIn = (R - H.To) - 18.77 * k;
    G.IcOut = MakeEllipse(21.59 * k, S / 2.0, IcR - 21.59 * k, 13.29 * u);
    G.IcIn = MakeEllipse(20.69 * k, S / 2.0, (IcR - H.Ti) - 20.69 * k, 13.26 * u);
    return G;
}

//---------------------------------------------------------------------------
// Styles: "sticker" (logo on white backing), "bare" (logo alone), "inverted" (logo for dark backgrounds)
static void DrawLogo(Canvas& C, const Geometry& G, const string& Style)
{
    const int S = G.S;
    const double Bar = G.Bar;
    const Color Ink = Style == "inverted" ? WHITE : BLACK, Hole = Style == "inverted" ? BLACK : WHITE;
    if (Style == "sticker") // white "D": from the middle of the RGB bars to the outer arc + rim
    {
        Pt Top = {Bar * 1.5, 0}, Bottom = {Bar * 1.5, (double)S};
        Poly P(1, Top);
        Poly E = HalfEllipse(G.OcCx, S / 2.0, G.OcRxOut + G.Rim, S / 2.0);
        P.insert(P.end(), E.begin(), E.end());
        P.push_back(Bottom);
        C.Fill(P, WHITE);
    }
    static const double Left[4] = {1, 0, 0, 1};
    C.Fill(RRect(0, 0, 3 * Bar, S, G.BarR), BLUE);
    C.Fill(RRect(0, 0, 2 * Bar, S, G.BarR, Left), GREEN);
    C.Fill(RRect(0, 0, Bar, S, G.BarR, Left), RED);
    const EllipseDef& D = G.Disc;
    C.Fill(Ellipse(D.cx, D.cy, D.rx, D.ry), Ink);
    if (G.Q == "v")
    {
        double kx = G.QScale * D.rx / 6.42, ky = G.QScale * D.ry / 6.70;
        Polys Q = QMark();
        for (size_t i = 0; i < Q.size(); i++)
            for (size_t j = 0; j < Q[i].size(); j++)
            {
                double x = (Q[i][j].x - 18.48) * kx, y = (Q[i][j].y - 22.72) * ky;
                Q[i][j].x = x + D.cx;
                Q[i][j].y = y + D.cy;
            }
        C.Fill(Q, Hole);
    }
    else if (QMarkPixels.count(G.Q))
    {
        const vector<string>& Pat = QMarkPixels.at(G.Q);
        C.Pixels(Pat, (int)floor(D.cx - Pat[0].size() / 2.0 + 0.5), (int)floor(D.cy - Pat.size() / 2.0 + 0.5), Hole);
    }
    C.Fill(Crescent(G.IcOut, G.IcIn), Ink);
    C.Fill(Crescent(MakeEllipse(G.OcCx, S / 2.0, G.OcRxOut, S / 2.0), MakeEllipse(G.OcCx, S / 2.0, G.OcRxIn, S / 2.0)), Ink);
}

//---------------------------------------------------------------------------
// Hand-drawn pixel art for the smallest sizes (16 px: context menus, title bars, lists;
// 20 and 24 px: same at 125 and 150 %). One map gives the three styles:
//   R G B  RGB bars                               K  logo ink (inverted: white)
//   w      white backing (sticker only)           q  "?" (inverted: black)
//   v      white backing edge, 50 % alpha         .  transparent
//   1 2 3  ink 25/50/75 % over the backing (sticker: gray, others: ink with that alpha)
//   a b c  ink 25/50/75 % over transparent
static const map<int, vector<string> > PixelArtMaps = {
    {16, {"RGBwwwwwwwv.....",
          "RGBwwwwwwwwK....",
          "RGBwwwwwwwwwKb..",
          "RGBwwwwwwwKwwKa.",
          "RGBwwwwwwwwKw2K.",
          "RGBww1KKK1wKwwKb",
          "RGBw1KqqKK1wKwKK",
          "RGBwKKKKqKKwKwKK",
          "RGBwKKKqKKKwKwKK",
          "RGBwKKKKKKKwKwKK",
          "RGBw1KKqKK1wKwKb",
          "RGBww1KKK1wKw2K.",
          "RGBwwwwwwwwKwKa.",
          "RGBwwwwwwwKwKb..",
          "RGBwwwwwwwwK....",
          "RGBwwwwwwwv....."}},
    {20, {"RGBwwwwwwwwv........",
          "RGBwwwwwwwww1K......",
          "RGBwwwwwwwwwwwKw....",
          "RGBwwwwwwww2wwwKw...",
          "RGBwwwwwwwwwKww2Kw..",
          "RGBwwwwwwwwwwKwwKKv.",
          "RGBww1KKKK1wwKKwKKw.",
          "RGBw1KKqqKK1wwKKwKKv",
          "RGBwKKqKKqKKwwKKwKKw",
          "RGBwKKKKqKKKwwKKwKKw",
          "RGBwKKKqKKKKwwKKwKKw",
          "RGBwKKKKKKKKwwKKwKKw",
          "RGBw1KKqKKK1wwKKwKKv",
          "RGBww1KKKK1wwKKwKKw.",
          "RGBwwwwwwwwwwKwwKKv.",
          "RGBwwwwwwwwwKww2Kw..",
          "RGBwwwwwwww2wwwKw...",
          "RGBwwwwwwwwwwwKw....",
          "RGBwwwwwwwww1K......",
          "RGBwwwwwwwwv........"}},
    {24, {"RGBwwwwwwwww1v..........",
          "RGBwwwwwwwwwww1Kv.......",
          "RGBwwwwwwwwwwwwwKKv.....",
          "RGBwwwwwwwwwwwwwwKKv....",
          "RGBwwwwwwwwwwwwww1KKw...",
          "RGBwwwwwwwww1wwwwwKKKv..",
          "RGBwwwwwwwwww2KwwwwKK2v.",
          "RGBwwwwwwwwwww2KwwwKKKw.",
          "RGBwwww1KKK1wwwKKww2KK2.",
          "RGBwww3KqqqK3wwKKwwwKKKv",
          "RGBww2KqqKqqK2wwKKwwKKKw",
          "RGBwwKKKKKqqKKwwKKwwKKKw",
          "RGBwwKKKKqqKKKwwKKwwKKKw",
          "RGBwwKKKKqqKKKwwKKwwKKKw",
          "RGBww2KKKKKKK2wwKKwwKKKv",
          "RGBwww3KKqqK3wwKKww2KK2.",
          "RGBwwww1KKK1wwwKKwwKKKw.",
          "RGBwwwwwwwwwww2KwwwKK2v.",
          "RGBwwwwwwwwww2KwwwKKKv..",
          "RGBwwwwwwwww1wwww1KKw...",
          "RGBwwwwwwwwwwwwwwKKv....",
          "RGBwwwwwwwwwwwwwKKv.....",
          "RGBwwwwwwwwwww1Kv.......",
          "RGBwwwwwwwww1v.........."}},
};

static void SetPixel(uint8_t* P, Color C, int Alpha)
{
    P[0] = C.r;
    P[1] = C.g;
    P[2] = C.b;
    P[3] = (uint8_t)Alpha;
}

static Image PixelArt(int S, const string& Style)
{
    Image Out(S, S);
    const bool Sticker = Style == "sticker";
    const Color Ink = Style == "inverted" ? WHITE : BLACK, Hole = Style == "inverted" ? BLACK : WHITE;
    const vector<string>& Rows = PixelArtMaps.at(S);
    for (int y = 0; y < S; y++)
    {
        if ((int)Rows[y].size() != S)
        {
            fprintf(stderr, "Pixel art %d: row %d has a wrong length\n", S, y);
            exit(1);
        }
        for (int x = 0; x < S; x++)
        {
            char ch = Rows[y][x];
            uint8_t* P = Out.At(x, y);
            const char* Lvl123 = ch ? strchr("123", ch) : NULL;
            const char* LvlAbc = ch ? strchr("abc", ch) : NULL;
            if (ch == 'R')
                SetPixel(P, RED, 255);
            else if (ch == 'G')
                SetPixel(P, GREEN, 255);
            else if (ch == 'B')
                SetPixel(P, BLUE, 255);
            else if (ch == 'K')
                SetPixel(P, Ink, 255);
            else if (ch == 'q')
                SetPixel(P, Hole, 255);
            else if (ch == 'w' && Sticker)
                SetPixel(P, WHITE, 255);
            else if (ch == 'v' && Sticker)
                SetPixel(P, WHITE, 128);
            else if (Lvl123 && Sticker)
            {
                uint8_t g = (uint8_t)RoundHalfEven(255 * (1 - (ch - '0') / 4.0));
                Color Gray = {g, g, g};
                SetPixel(P, Gray, 255);
            }
            else if (Lvl123 || LvlAbc)
            {
                int Level = Lvl123 ? (int)(Lvl123 - "123") + 1 : (int)(LvlAbc - "abc") + 1;
                SetPixel(P, Ink, RoundHalfEven(255 * Level / 4.0));
            }
        }
    }
    return Out;
}

//---------------------------------------------------------------------------
// Renderers
static Image Logo(int S, const string& Style)
{
    static map<pair<int, string>, Image> Cache;
    pair<int, string> Key(S, Style);
    map<pair<int, string>, Image>::const_iterator It = Cache.find(Key);
    if (It != Cache.end())
        return It->second;
    Image Out;
    if (PixelArtMaps.count(S))
        Out = PixelArt(S, Style);
    else
    {
        Geometry G = MakeGeometry(S, Style == "sticker");
        Canvas C(S, G.HasQuant ? &G.Qt : NULL);
        DrawLogo(C, G, Style);
        Out = C.Rgba();
    }
    Cache[Key] = Out;
    return Out;
}

// Logo of size L centred on a transparent W x H canvas (tiles, plated icons)
static Image Placed(int W, int H, int L, const string& Style)
{
    Image Out(W, H);
    Image Lg = Logo(L, Style);
    int x = (W - L) / 2, y = (H - L) / 2;
    for (int j = 0; j < L; j++)
        memcpy(Out.At(x, y + j), Lg.At(0, j), (size_t)L * 4);
    return Out;
}

// Largest integer <= v with the parity of ref (keeps the logo exactly centred)
static int SameParity(double v, int Ref)
{
    int i = (int)floor(v);
    return ((Ref - i) % 2 + 2) % 2 == 0 ? i : i - 1;
}

//---------------------------------------------------------------------------
// Writers
static void Put32BE(string& s, uint32_t v) { for (int i = 3; i >= 0; i--) s += (char)((v >> (i * 8)) & 0xFF); }
static void Put32LE(string& s, uint32_t v) { for (int i = 0; i < 4; i++) s += (char)((v >> (i * 8)) & 0xFF); }
static void Put16LE(string& s, uint16_t v) { s += (char)(v & 0xFF); s += (char)(v >> 8); }

static void PngChunk(string& Out, const char* Type, const string& Data)
{
    Put32BE(Out, (uint32_t)Data.size());
    string TD = string(Type, 4) + Data;
    Out += TD;
    Put32BE(Out, (uint32_t)crc32(0, (const Bytef*)TD.data(), (uInt)TD.size()));
}

// RGBA PNG, adaptive filter per row (minimum sum of absolute differences), zlib level 9
static string PngBytes(const Image& Im)
{
    const int W = Im.W, H = Im.H, Bpl = W * 4;
    string Raw;
    Raw.reserve((size_t)H * (Bpl + 1));
    vector<int> Cand[5];
    for (int y = 0; y < H; y++)
    {
        const uint8_t* X = &Im.Px[(size_t)y * Bpl];
        const uint8_t* Up = y ? &Im.Px[(size_t)(y - 1) * Bpl] : NULL;
        for (int f = 0; f < 5; f++)
            Cand[f].assign(Bpl, 0);
        for (int i = 0; i < Bpl; i++)
        {
            int x = X[i];
            int a = i >= 4 ? X[i - 4] : 0;
            int b = Up ? Up[i] : 0;
            int c = Up && i >= 4 ? Up[i - 4] : 0;
            int p = a + b - c;
            int pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);
            int Paeth = pa <= pb && pa <= pc ? a : (pb <= pc ? b : c);
            Cand[0][i] = x & 0xFF;
            Cand[1][i] = (x - a) & 0xFF;
            Cand[2][i] = (x - b) & 0xFF;
            Cand[3][i] = (x - (a + b) / 2) & 0xFF;
            Cand[4][i] = (x - Paeth) & 0xFF;
        }
        int Best = 0;
        long long BestScore = -1;
        for (int f = 0; f < 5; f++)
        {
            long long Score = 0;
            for (int i = 0; i < Bpl; i++)
                Score += abs((int)(int8_t)(uint8_t)Cand[f][i]);
            if (BestScore < 0 || Score < BestScore)
            {
                BestScore = Score;
                Best = f;
            }
        }
        Raw += (char)Best;
        for (int i = 0; i < Bpl; i++)
            Raw += (char)Cand[Best][i];
    }
    uLongf ZLen = compressBound((uLong)Raw.size());
    string Z(ZLen, '\0');
    if (compress2((Bytef*)&Z[0], &ZLen, (const Bytef*)Raw.data(), (uLong)Raw.size(), 9) != Z_OK)
    {
        fprintf(stderr, "zlib error\n");
        exit(1);
    }
    Z.resize(ZLen);
    string Ihdr;
    Put32BE(Ihdr, W);
    Put32BE(Ihdr, H);
    Ihdr += string("\x08\x06\x00\x00\x00", 5);
    string Out("\x89PNG\r\n\x1a\n", 8);
    PngChunk(Out, "IHDR", Ihdr);
    PngChunk(Out, "IDAT", Z);
    PngChunk(Out, "IEND", string());
    return Out;
}

// 32-bit BGRA DIB + AND mask, as stored in .ico/RT_ICON
static string BmpEntry(const Image& Im)
{
    const int W = Im.W, H = Im.H, Stride = (W + 31) / 32 * 4;
    string Xor, Mask;
    for (int y = H - 1; y >= 0; y--)
    {
        string Row((size_t)Stride, '\0');
        for (int x = 0; x < W; x++)
        {
            const uint8_t* P = Im.At(x, y);
            Xor += (char)P[2];
            Xor += (char)P[1];
            Xor += (char)P[0];
            Xor += (char)P[3];
            if (P[3] == 0)
                Row[x / 8] |= (char)(0x80 >> (x % 8));
        }
        Mask += Row;
    }
    string Out;
    Put32LE(Out, 40);
    Put32LE(Out, W);
    Put32LE(Out, H * 2);
    Put16LE(Out, 1);
    Put16LE(Out, 32);
    Put32LE(Out, 0);
    Put32LE(Out, (uint32_t)(Xor.size() + Mask.size()));
    Put32LE(Out, 0);
    Put32LE(Out, 0);
    Put32LE(Out, 0);
    Put32LE(Out, 0);
    return Out + Xor + Mask;
}

// Images largest first. 256 px stored as PNG, smaller ones as 32-bit BMP (Windows 7+)
static string IcoBytes(const vector<Image>& Images)
{
    vector<string> Blobs;
    for (size_t i = 0; i < Images.size(); i++)
        Blobs.push_back(Images[i].W >= 256 ? PngBytes(Images[i]) : BmpEntry(Images[i]));
    string Out;
    Put16LE(Out, 0);
    Put16LE(Out, 1);
    Put16LE(Out, (uint16_t)Images.size());
    uint32_t Off = 6 + 16 * (uint32_t)Images.size();
    for (size_t i = 0; i < Images.size(); i++)
    {
        char s = (char)(uint8_t)(Images[i].W % 256);
        Out += s;
        Out += s;
        Out += (char)0;
        Out += (char)0;
        Put16LE(Out, 1);
        Put16LE(Out, 32);
        Put32LE(Out, (uint32_t)Blobs[i].size());
        Put32LE(Out, Off);
        Off += (uint32_t)Blobs[i].size();
    }
    for (size_t i = 0; i < Blobs.size(); i++)
        Out += Blobs[i];
    return Out;
}

static void Write(const string& Path, const string& Data)
{
    ofstream F(Path.c_str(), ios::binary);
    F.write(Data.data(), (streamsize)Data.size());
    if (!F)
    {
        fprintf(stderr, "Can not write %s\n", Path.c_str());
        exit(1);
    }
}

//---------------------------------------------------------------------------
static const int IcoSizes[] = {256, 96, 80, 72, 64, 60, 48, 40, 36, 32, 30, 24, 20, 16};
static const int TargetSizes[] = {16, 20, 24, 30, 32, 36, 40, 48, 60, 64, 72, 80, 96, 256};
static const int Scales[] = {100, 200, 400};

// Theme aware assets (sparse package of the VCL GUI, Qt MSIX package)
static size_t MsixAssets(const string& Folder)
{
    MakeDir(Folder);
    vector<string> Old = ListDir(Folder);
    for (size_t i = 0; i < Old.size(); i++)
        if (EndsWith(Old[i], ".png"))
            remove((Folder + "/" + Old[i]).c_str());
    map<string, Image> Files;
    for (size_t i = 0; i < sizeof(TargetSizes) / sizeof(*TargetSizes); i++)
    {
        int t = TargetSizes[i];
        int m = RoundHalfEven(t / 8.0);
        string T = to_string(t);
        Files["Square44x44Logo.targetsize-" + T + ".png"] = Placed(t, t, t - 2 * m, "inverted");        // plated on the accent color (Windows 10)
        Files["Square44x44Logo.altform-unplated_targetsize-" + T + ".png"] = Logo(t, "inverted");       // dark theme
        Files["Square44x44Logo.altform-lightunplated_targetsize-" + T + ".png"] = Logo(t, "bare");      // light theme
    }
    for (size_t i = 0; i < sizeof(Scales) / sizeof(*Scales); i++)
    {
        int s = Scales[i];
        // default: accent color or dark tiles; colorful_theme-light: light theme-aware tiles (Windows 10)
        const char* Variants[2][2] = {{"", "inverted"}, {"_altform-colorful_theme-light", "bare"}};
        for (int v = 0; v < 2; v++)
        {
            string Suffix = "scale-" + to_string(s) + Variants[v][0] + ".png", Style = Variants[v][1];
            int n = 44 * s / 100;
            Files["Square44x44Logo." + Suffix] = Placed(n, n, n - 2 * RoundHalfEven(n / 8.0), Style);
            n = 150 * s / 100;
            Files["Square150x150Logo." + Suffix] = Placed(n, n, SameParity(n / 3.0, n), Style);
            int w = 310 * s / 100, h = 150 * s / 100;
            Files["Wide310x150Logo." + Suffix] = Placed(w, h, SameParity(h / 3.0, h), Style);
            n = 310 * s / 100;
            Files["LargeTile." + Suffix] = Placed(n, n, SameParity(n / 3.0, n), Style);
            n = 71 * s / 100;
            Files["SmallTile." + Suffix] = Placed(n, n, SameParity(n / 2.0, n), Style);
        }
    }
    for (map<string, Image>::const_iterator F = Files.begin(); F != Files.end(); ++F)
        Write(Folder + "/" + F->first, PngBytes(F->second));
    return Files.size();
}

// UWP GUI assets: regenerated in place (names are listed in the UWP project), no theme variants
static size_t UwpAssets(const string& Folder)
{
    vector<string> Names = ListDir(Folder);
    size_t Count = 0;
    for (size_t i = 0; i < Names.size(); i++)
    {
        const string& f = Names[i];
        if (!EndsWith(f, ".png") || StartsWith(f, "logo_")) // logo_*: legacy Qt WinRT assets, kept as is
            continue;
        unsigned char Hdr[24] = {0};
        {
            ifstream In((Folder + "/" + f).c_str(), ios::binary);
            In.read((char*)Hdr, 24);
        }
        int w = (Hdr[16] << 24) | (Hdr[17] << 16) | (Hdr[18] << 8) | Hdr[19];
        int h = (Hdr[20] << 24) | (Hdr[21] << 16) | (Hdr[22] << 8) | Hdr[23];
        Image Im;
        if (f.find("unplated") != string::npos || StartsWith(f, "LockScreenLogo")) // dark taskbar/Start, lock screen
            Im = Logo(w, "inverted");
        else if (StartsWith(f, "StoreLogo")) // unknown background
            Im = Logo(w, "sticker");
        else if (StartsWith(f, "SplashScreen")) // unknown background
            Im = Placed(w, h, SameParity(h / 3.0, h), "sticker");
        else if (StartsWith(f, "Square44x44Logo")) // plated on the accent color
            Im = Placed(w, h, w - 2 * RoundHalfEven(w / 8.0), "inverted");
        else if (StartsWith(f, "SmallTile")) // tiles: accent color
            Im = Placed(w, h, SameParity(h / 2.0, h), "inverted");
        else // Square150x150Logo, Wide310x150Logo, LargeTile
            Im = Placed(w, h, SameParity(h / 3.0, h), "inverted");
        Write(Folder + "/" + f, PngBytes(Im));
        Count++;
    }
    return Count;
}

static bool IsImageDir(const string& Path)
{
    return IsFile(Path + "/MediaInfo.svg") && IsDir(Path + "/Assets");
}

int main(int argc, char* argv[])
{
    string Here;
    if (argc > 1)
        Here = argv[1];
    else
    {
        const char* Candidates[] = {"../Resource/Image", "Source/Resource/Image", "."};
        for (size_t i = 0; i < 3 && Here.empty(); i++)
            if (IsImageDir(Candidates[i]))
                Here = Candidates[i];
    }
    if (Here.empty() || !IsImageDir(Here))
    {
        fprintf(stderr, "Usage: MediaInfo_Windows_Icons [path to Source/Resource/Image]\n");
        return 1;
    }
    vector<Image> Ico;
    for (size_t i = 0; i < sizeof(IcoSizes) / sizeof(*IcoSizes); i++)
        Ico.push_back(Logo(IcoSizes[i], "sticker"));
    Write(Here + "/MediaInfo.ico", IcoBytes(Ico));
    Write(Here + "/MediaInfo_TinyOnly.ico", IcoBytes(vector<Image>(1, Logo(16, "sticker"))));
    size_t n = MsixAssets(Here + "/MSIX_Assets");
    size_t m = UwpAssets(Here + "/Assets");
    printf("%s: 2 icons, %u MSIX assets, %u UWP assets\n", Here.c_str(), (unsigned)n, (unsigned)m);
    return 0;
}
