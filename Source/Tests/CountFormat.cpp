/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "GUI/Common/GUI_Main_Easy_Box_Core.h"
#include "GUI/Common/GUI_Main_Easy_Core.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace ZenLib;
using namespace MediaInfoNameSpace;

namespace
{
size_t Checks=0, Failures=0;
Ztring Utf8(const std::string& Text) { return Ztring().From_UTF8(Text); }

void Equal(const char* Label, const Ztring& Actual, const Ztring& Expected)
{
    ++Checks;
    if (Actual==Expected) return;
    ++Failures;
    std::cerr << Label << ": expected [" << Expected.To_UTF8()
              << "], got [" << Actual.To_UTF8() << "]\n";
}

void Check(const char* Label, bool Condition)
{
    Equal(Label, Condition?__T("true"):__T("false"), __T("true"));
}

Ztring Catalog(const std::string& Directory, const char* Locale)
{
    std::ifstream Input((Directory+"/"+Locale+".csv").c_str(), std::ios::binary);
    Check("catalog exists", Input.good());
    return Utf8(std::string(std::istreambuf_iterator<char>(Input), std::istreambuf_iterator<char>()));
}

void Load(const Ztring& Catalog) { MediaInfo::Option_Static(__T("Language"), Catalog); }

std::string Unsigned(unsigned Value)
{
    std::string Bytes(1, char(Value&0xFF));
    while (Value>>=8) Bytes.insert(Bytes.begin(), char(Value&0xFF));
    return Bytes;
}

std::string Element(unsigned Id, const std::string& Data)
{
    unsigned Size=unsigned(Data.size());
    return Unsigned(Id)+(Size<127?std::string(1, char(0x80|Size)):
        std::string(1, char(0x40|(Size>>8)))+char(Size&0xFF))+Data;
}

std::string Integer(unsigned Id, unsigned Value) { return Element(Id, Unsigned(Value)); }

// Six distinct mono audio tracks and ten subtitle tracks, each with one block.
// The fixture is generated locally; no external media or tools are required.
void WriteMatroska(const char* Path)
{
    std::string Header=Integer(0x4286, 1)+Integer(0x42F7, 1)+Integer(0x42F2, 4)+Integer(0x42F3, 8)+
        Element(0x4282, "matroska")+Integer(0x4287, 4)+Integer(0x4285, 2);
    std::string Info=Element(0x4D80, "MediaInfo test")+Element(0x5741, "MediaInfo test");
    std::string Tracks, Blocks;
    for (unsigned Track=1; Track<=16; ++Track)
    {
        bool Audio=Track<=6;
        std::string Entry=Integer(0xD7, Track)+Integer(0x73C5, Track)+Integer(0x83, Audio?2:17)+
            Element(0x86, Audio?"A_PCM/INT/LIT":"S_TEXT/UTF8");
        if (Audio)
            Entry+=Element(0xE1, Element(0xB5, std::string("\x40\xE7\x70\0\0\0\0\0", 8))+
                Integer(0x9F, 1)+Integer(0x6264, 16));
        Tracks+=Element(0xAE, Entry);
        Blocks+=Element(0xA3, std::string(1, char(0x80|Track))+std::string("\0\0\x80", 3)+
            (Audio?std::string(2, '\0'):std::string("test")));
    }
    std::string File=Element(0x1A45DFA3, Header)+Element(0x18538067,
        Element(0x1549A966, Info)+Element(0x1654AE6B, Tracks)+Element(0x1F43B675, Integer(0xE7, 0)+Blocks));
    std::ofstream Output(Path, std::ios::binary);
    Output.write(File.data(), File.size());
    Check("fixture written", Output.good());
}
}

int main(int argc, char** argv)
{
    if (argc!=2)
    {
        std::cerr << "Usage: gui_count_format <language catalog directory>\n";
        return 2;
    }
    Equal("formatter available", MediaInfo::Option_Static(__T("Language_Format")), __T("1"));
    Ztring Czech=Catalog(argv[1], "cs"), English=Catalog(argv[1], "en"), Polish=Catalog(argv[1], "pl");
    Load(Czech);
    const unsigned Counts[]={0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 14, 21, 22, 23, 24, 25, 101, 102};
    const char* Audio[]={"zvukový stream", "zvukové streamy", "zvukových streamů"};
    const char* Text[]={"textový stream", "textové streamy", "textových streamů"};
    const char* Files[]={"soubor", "soubory", "souborů"};
    for (size_t Pos=0; Pos<sizeof(Counts)/sizeof(*Counts); ++Pos)
    {
        unsigned Count=Counts[Pos];
        unsigned Form=Count==1?0:Count>=2 && Count<=4?1:2;
        Ztring Number=Ztring::ToZtring(Count)+__T(" ");
        Equal("Czech audio summary", Core::FormatCount(__T("StreamSummary"), Count, Stream_Audio, __T("PCM")), Number+Utf8(Audio[Form])+__T(": PCM"));
        Equal("Czech text summary", Core::FormatCount(__T("StreamSummary"), Count, Stream_Text, __T("UTF-8")), Number+Utf8(Text[Form])+__T(": UTF-8"));
        Equal("Czech Sheet summary", Core::FormatCount(__T("StreamSummaryMore"), Count, Stream_Audio), Number+Utf8(Audio[Form])+Utf8(", viz níže"));
        Equal("Czech file title", Core::FormatCount(__T("FileCount"), Count), Number+Utf8(Files[Form]));
    }
    Equal("video kind", Core::FormatCount(__T("StreamSummary"), 6, Stream_Video, __T("AVC")), Utf8("6 streamů videa: AVC"));
    Equal("image kind", Core::FormatCount(__T("StreamSummary"), 6, Stream_Image, __T("JPEG")), Utf8("6 obrazových streamů: JPEG"));
    Equal("missing Czech other family uses English", Core::FormatCount(__T("StreamSummary"), 6, Stream_Other, __T("Time code")), __T("6 other streams: Time code"));
    Equal("missing Czech menu family uses English", Core::FormatCount(__T("StreamSummary"), 6, Stream_Menu, __T("Chapters")), __T("6 menu streams: Chapters"));
    Ztring Literal=__T("PCM; \"quoted\"\r\n{count} {{formats}} \\ tail");
    Equal("literal metadata", Core::FormatCount(__T("StreamSummary"), 6, Stream_Audio, Literal), Utf8("6 zvukových streamů: ")+Literal);
    Equal("empty metadata", Core::FormatCount(__T("StreamSummary"), 6, Stream_Audio), Utf8("6 zvukových streamů: "));
    Load(Polish);
    Equal("switch Czech to Polish", Core::FormatCount(__T("StreamSummary"), 22, Stream_Audio, __T("PCM")), Utf8("22 strumienie audio: PCM"));
    Load(English);
    Equal("switch to English singular", Core::FormatCount(__T("FileCount"), 1), __T("1 file"));
    Equal("switch to English plural", Core::FormatCount(__T("FileCount"), 22), __T("22 files"));
    Load(Czech);
    Equal("switch back to Czech", Core::FormatCount(__T("FileCount"), 22), Utf8("22 souborů"));
    Load(__T("  Language_ISO639;cs\n audio stream1; zvukový stream"));
    Equal("incomplete catalog fallback", Core::FormatCount(__T("StreamSummary"), 22, Stream_Audio, __T("PCM")), __T("22 audio streams: PCM"));
    Load(__T("  Language_ISO639;cs\nStreamSummary.Audio.other;{unknown}"));
    Equal("malformed pattern fallback", Core::FormatCount(__T("StreamSummary"), 22, Stream_Audio, __T("PCM")), __T("22 audio streams: PCM"));
    Load(__T("  Language_ISO639;cs\nStreamSummary.Audio.other;{formats}: {count} {{stop}}"));
    Equal("translator word order and braces", Core::FormatCount(__T("StreamSummary"), 22, Stream_Audio, __T("PCM")), __T("PCM: 22 {stop}"));
    const char* Fixture="count-format-test.mkv";
    WriteMatroska(Fixture);
    {
        Core C;
        Check("open multi-stream fixture", C.MI->Open(Utf8(Fixture))==1);
        Check("six distinct audio streams", C.MI->Count_Get(0, Stream_Audio)==6);
        Check("ten distinct text streams", C.MI->Count_Get(0, Stream_Text)==10);
        Equal("mono audio is not six channels", C.MI->Get(0, Stream_Audio, 0, __T("Channel(s)")), __T("1"));
        GUI_Main_Easy_Core Easy(&C);
        Easy.File_Pos=0;
        GUI_Main_Easy_Box_Core Box(&C, &Easy, Stream_General, 0);
        Load(Czech);
        Ztring Summary=Box.Text_Get();
        Check("shared Easy Czech audio", Summary.find(Utf8("6 zvukových streamů: "))!=Ztring::npos);
        Check("shared Easy Czech text", Summary.find(Utf8("10 textových streamů: "))!=Ztring::npos);
        Load(English);
        Summary=Box.Text_Get();
        Check("shared Easy language switch", Summary.find(__T("6 audio streams: "))!=Ztring::npos);
        Check("shared Easy updated text", Summary.find(__T("10 text streams: "))!=Ztring::npos);
    }
    Check("fixture removed", std::remove(Fixture)==0);
    std::cout << (Failures?"FAIL: ":"PASS: ") << Checks << " GUI count checks, " << Failures << " failures\n";
    return Failures?1:0;
}
