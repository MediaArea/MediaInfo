// MediaInfo - All info about media files
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "MediaInfo/File__Base.h"
#include "MediaInfo/Multiple/File_MpegTs.h" //TODO
#include "MediaInfo/File__MultipleParsing.h"
#include <ZenLib/FileName.h>
#include <ZenLib/File.h>
#include <ZenLib/InfoMap.h>
#include <vector>
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

const size_t Buffer_NormalSize=/*188*7;//*/32768;

//---------------------------------------------------------------------------
extern MediaInfo_Config         Config;
       ZtringListList           MediaInfo_Temp;
//       vector<ZtringListList>   MediaInfo_Capacities;
       String         MediaInfo_Capacities_Final;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/destructor
//***************************************************************************

//---------------------------------------------------------------------------
MediaInfo::MediaInfo()
{
    Thread=NULL;
    BlockMethod=BlockMethod_Local;
    Info=NULL;
    Buffer=NULL;
    Buffer_Size=0;
    Buffer_Size_Max=0;
    BufferConst=NULL;
    File_Handle=NULL;
    File_Size=0;
    File_Offset=0;
    File_AlreadyBuffered=false;
    MultipleParsing_IsDetected=false;

    Config.Init(); //Initialize Configuration
}

//---------------------------------------------------------------------------
MediaInfo::~MediaInfo()
{
    Close();

    delete Info; //Info=NULL;
    delete[] Buffer; //Buffer=NULL;
    delete (File*)File_Handle; //File_Handle=NULL;
}

//***************************************************************************
// Files
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Open(const String &File_Name_)
{
    //Test existence of the file
    File_Name=File_Name_;
    if (!File::Exists(File_Name))
        return 0;

    //Get the Extension
    Ztring Extension=FileName::Extension_Get(File_Name);
    Extension.MakeLowerCase();

    //Search the theorical format from extension
    InfoMap &FormatList=Config.Format_Get();
    InfoMap::iterator Format=FormatList.begin();
    while (Format!=FormatList.end())
    {
        const Ztring &Extensions=FormatList.Get(Format->first, InfoFormat_Extensions);
        if (Extensions.find(Extension)!=Error)
        {
            if(Extension.size()==Extensions.size())
                break; //Only one extenion in the list
            if(Extensions.find(Extension+_T(" "))!=Error
            || Extensions.find(_T(" ")+Extension)!=Error)
                break;
        }
        Format++;
    }
    if (Format!=FormatList.end())
    {
        const Ztring &Parser=Format->second(InfoFormat_Parser);
        SelectFromExtension(Parser);
    }

    //Test the theorical format
    if (Format_Test()>0)
         return 1;

    //Extension is not the good one, parse with all formats
    InternalMethod=1;
    size_t ToReturn=ListFormats();

    Format_Test_FillBuffer_Close();
    return ToReturn;
}

//---------------------------------------------------------------------------
int MediaInfo::Format_Test()
{
    //Integrity
    if (Info==NULL)
        return 0;

    //Test the format with filename
    if (Info->Open_File(File_Name)>0)
        return 1;

    //Test the format with buffer
    //-Test is already test with failure
    if (File_AlreadyBuffered && File_Size==0)
        return 0; //Already tested, ad a big problem on it

    //-Initating the format
    if (Format_Test_FillBuffer_Init()<0)
        return 0;
    Info->Open_Buffer_Init(File_Size);

    //-Test the format with buffer
    do
    {
        if (Format_Test_FillBuffer_Continue()<0)
            break; //Error during reading
        else if (Info)
            Info->Open_Buffer_Continue(Buffer, Buffer_Size);
    }
    while (Info && Info->File_Offset<File_Size);

    //-Close
    Format_Test_FillBuffer_Close();

    //Finalize
    if (Info && Info->Count_Get(Stream_General)>0)
    {
        Info->Open_Buffer_Finalize();
        Info->Finalize();
        return 1;
    }

    //Cleanup if needed
    delete Info; Info=NULL;
    return 0;
}

//---------------------------------------------------------------------------
int MediaInfo::Format_Test_Buffer()
{
    //Integrity
    if (Info==NULL)
        return 0;

    if (Info->Count_Get(Stream_General)==0)
        Info->Open_Buffer_Init(NULL, File_Size, File_Offset);

    //-Test the format with buffer
    Info->Open_Buffer_Continue(NULL, BufferConst?BufferConst:Buffer, Buffer_Size);
    //We must wait fo more data
    if (Info->Count_Get(Stream_General)>0)
        return 1;
    else
        return 0;
}

//---------------------------------------------------------------------------
int MediaInfo::Format_Test_FillBuffer_Init()
{
    //Integrity
    if (Info==NULL)
        return -1;

    //Is there a file to open?
    if (File_Name.empty())
        return 1; //Buffer is handled elsewhere
    if (File_Handle)
    {
        File_AlreadyBuffered=true;
        return 1; //Already opened
    }

    //Init
    Buffer_Size_Max=Buffer_NormalSize;
    File_Size=0;
    File_Offset=0;
    Buffer=NULL;

    //Opening the file
    File_Handle=new File;
    ((File*)File_Handle)->Open(File_Name);
    if (!((File*)File_Handle)->Opened_Get())
    {
        File_AlreadyBuffered=true; //We don't succeed to open it, so File_Size is 0
        return -1;
    }


    //FileSize
    File_Size=((File*)File_Handle)->Size_Get();

    //Buffer
    delete[] Buffer; Buffer=new int8u[Buffer_Size_Max];

    return 1;
}

//---------------------------------------------------------------------------
int MediaInfo::Format_Test_FillBuffer_Continue()
{
    //Integrity
    if (Info==NULL)
        return -1;

    //Is there a file to open?
    if (File_Name.empty())
    {
        if (File_Offset==0)
            return 1; //Buffer is handled elsewhere
        else
            return -1; //Not possible to have more
    }

    //Seek (if needed)
    if (Info->File_GoTo!=(int64u)-1 && Info->File_GoTo<File_Size)
    {
        if (((File*)File_Handle)->GoTo(Info->File_GoTo))
        {
            File_Offset=Info->File_GoTo;
            Info->Open_Buffer_Init(File_Size, File_Offset);
        }
        else
            //File is not seekable
            return -1;
    }

    //Buffering
    if (!File_AlreadyBuffered)
    {
        Buffer_Size=((File*)File_Handle)->Read(Buffer, Buffer_Size_Max);
        if (Buffer_Size!=0)
        {
            //Read is OK
            if (Buffer_Size==0)
                return -1;
            File_Offset+=Buffer_Size;
        }
        else
            //Problem while reading
            return -1;
    }
    else
        File_AlreadyBuffered=false;

    return 1;
}

//---------------------------------------------------------------------------
int MediaInfo::Format_Test_FillBuffer_Close()
{
    //Close
    delete (File*)File_Handle; File_Handle=NULL;
    Buffer_Clear();

    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open (const int8u* Begin_, size_t Begin_Size_, const int8u*, size_t, int64u FileSize_)
{
    Buffer_Size_Max=Begin_Size_;
    delete[] Buffer; Buffer=new int8u[Buffer_Size_Max];
    memcpy(Buffer, Begin_, Begin_Size_);
    Buffer_Size=Begin_Size_;
    File_Name.clear();
    File_Size=FileSize_;

    InternalMethod=1;
    size_t ToReturn=ListFormats();

    Buffer_Clear();
    return ToReturn;
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Init (int64u File_Size_, int64u File_Offset_)
{
    if (Info==NULL)
        Info=new File__MultipleParsing;
    Info->Open_Buffer_Init(File_Size_, File_Offset_);

    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Continue (const int8u* ToAdd, size_t ToAdd_Size)
{
    if (Info==NULL)
        Info=new File__MultipleParsing;

    Info->Open_Buffer_Continue(Info, ToAdd, ToAdd_Size);

    if (!MultipleParsing_IsDetected && Info->Count_Get(Stream_General)>0)
    {
        //Found
        File__Base* Info_ToDelete=Info;
        Info=((File__MultipleParsing*)Info)->Parser_Get();
        delete Info_ToDelete; //Info_ToDelete=NULL;
        MultipleParsing_IsDetected=true;
    }

    //The parser wanted seek but the buffer is not seekable
    if (Info->File_GoTo!=(int64u)-1 && Config.File_IsSeekable_Get()==0)
    {
        Info->Open_Buffer_Finalize(true);
        Info->Finalize();
        Info->File_GoTo=(int64u)-1;
        return 0;
    }

    return 1;
}

//---------------------------------------------------------------------------
int64u MediaInfo::Open_Buffer_Continue_GoTo_Get ()
{
    if (Info!=NULL)
        return Info->File_GoTo;
    else
        return 0;
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Finalize ()
{
    if (Info!=NULL)
    {
        Info->Open_Buffer_Finalize();
        Info->Finalize();
    }
    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo::Save()
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
void MediaInfo::Close()
{
    delete Info; Info=NULL;
    Buffer_Clear();
}

//***************************************************************************
// Get File info
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfo::Inform(size_t)
{
    //Info case
    if (Info)
        return Info->Inform();

    if (!Info)
        return Config.EmptyString_Get();

    return Info->Inform();
}

//---------------------------------------------------------------------------
String MediaInfo::Get(stream_t KindOfStream, size_t StreamNumber, size_t Parameter, info_t KindOfInfo)
{
    if (!Info)
        return Config.EmptyString_Get();

    return Info->Get(KindOfStream, StreamNumber, Parameter, KindOfInfo).c_str();
}

//---------------------------------------------------------------------------
String MediaInfo::Get(stream_t KindOfStream, size_t StreamNumber, const String &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    if (!Info)
        return Config.EmptyString_Get();

    return Info->Get(KindOfStream, StreamNumber, Parameter, KindOfInfo, KindOfSearch).c_str();
}

//***************************************************************************
// Set File info
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Set(const String &ToSet, stream_t StreamKind, size_t StreamNumber, size_t Parameter, const String &OldValue)
{
    if (!Info)
        return 0;

    return Info->Set(StreamKind, StreamNumber, Parameter, ToSet, OldValue);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Set(const String &ToSet, stream_t StreamKind, size_t StreamNumber, const String &Parameter, const String &OldValue)
{
    if (!Info)
        return 0;

    return Info->Set(StreamKind, StreamNumber, Parameter, ToSet, OldValue);
}

//***************************************************************************
// Output buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Output_Buffer_Get (const String &Value)
{
    if (!Info)
        return 0;

    return Info->Output_Buffer_Get(Value);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Output_Buffer_Get (size_t Pos)
{
    if (!Info)
        return 0;

    return Info->Output_Buffer_Get(Pos);
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfo::Option (const String &Option, const String &Value)
{
         if (Option.empty())
        return _T("");
    else if (Option==_T("Language_Update"))
    {
        if (!Info || Info->Get(Stream_General, 0, _T("CompleteName"))==_T(""))
            return _T("");

        ZtringListList Language=Value.c_str();
        Config.Language_Set(Language);

        return _T("");
    }
    else if (Option==_T("Create_Dummy"))
    {
        CreateDummy (Value);
        if (Info && Info->Open_File(_T(""))>0)
            return _T("Y");
        else
        {
            delete Info; Info=NULL;
            return _T("");
        }
    }
    else if (Option==_T("Info_Capacities"))
    {
        return _T("Option desactivated for this version, will come back soon!");
        /*
        //Header
        Ztring Header;
        Header+=MediaInfo::Option_Static(_T("Info_Version"))+_T("\n");
        Header+=_T("What's supported?\n");
        Header+=_T("Known Format :\n");
        Header+=_T("Matroska (mkv/mka/mks),\n");
        Header+=_T("Ogg (ogg/ogm),\n");
        Header+=_T("Riff (avi/wav/divx/gvi),\n");
        Header+=_T("Mpeg 1&2 container (mpeg/mpg/vob),\n");
        Header+=_T("Mpeg 4 container (mp4),\n");
        Header+=_T("Mpeg video specific (mpgv/mpv/m1v/m2v),\n");
        Header+=_T("Mpeg audio specific (mp2/mp3),\n");
        Header+=_T("Musepack (mpc),\n");
        Header+=_T("Windows Media (asf/wma/wmv),\n");
        Header+=_T("Quicktime (qt/mov),\n");
        Header+=_T("Real (rm/rmvb/ra)\n");
        Header+=_T("flic (fli/flc)\n");
        Header+=_T("DVD-Video (ifo)\n");
        Header+=_T("AC3 (ac3),\n");
        Header+=_T("DTS (dts),\n");
        Header+=_T("AAC (aac),\n");
        Header+=_T("Monkey Audio (ape/mac),\n");
        Header+=_T("Flac (flac),\n");
        Header+=_T("CDXA, like Video-CD (dat),\n");
        Header+=_T("Apple/SGI (aiff/aifc) ,\n");
        Header+=_T("Sun/NeXT (au),\n");
        Header+=_T("Amiga IFF/SVX8/SV16 (iff),\n");
        Header+=_T("Ensoniq PARIS (paf),\n");
        Header+=_T("Sound Designer 2 (sd2),\n");
        Header+=_T("Berkeley/IRCAM/CARL (irca),\n");
        Header+=_T("SoundFoundry WAVE 64 (w64),\n");
        Header+=_T("Matlab (mat),\n");
        Header+=_T("Portable Voice format (pvf),\n");
        Header+=_T("FastTracker2 Extanded (xi),\n");
        Header+=_T("Midi Sample dump Format (sds),\n");
        Header+=_T("Audio Video Research (avr),\n");
        Header+=_T("JPEG (jpeg/jpg/jpe\n");
        Header+=_T("Portable Network Graphic (png)\n");
        Header+=_T("Graphic Interchange Format (gif)\n");
        Header+=_T("Tagged Image Format File (tiff)\n");
        Header+=_T("Bitmap (bmp)\n");
        Header+=_T("Icon (ico)\n");
        Header+=_T("7-Zip (7z)\n");
        Header+=_T("Rar (rar)\n");
        Header+=_T("Zip (zip)\n");
        Header+=_T("Ace (ace)\n");
        Header+=_T("R=Read\n");
        Header+=_T("W=Read and Write\n");
        Header+=_T("N=Not yet supported\n");
        Header+=_T("\n");
        Header+=_T("                      m o r m m m m m w q r f d a d a a f c s f m a a i p s i w m x s a j p g t b i 7 r z a\n");
        Header+=_T("                      k g i p p p p p m t m l v c t a p l d w l x i u f a d r 6 a i d v p n i i m c z a i c\n");
        Header+=_T("                        g f e e e e c       i d 3 s c e a x f v f f   f f 2 c 4 t   s r e g f f p o   r p e\n");
        Header+=_T("                          f g g g g         c v         c a       f         a           g     f           \n");
        Header+=_T("                              4 v a                                                                       \n");

        //Filling
        InternalMethod=3;
        MediaInfo_Capacities.clear();
        ListFormats();

        //Parameters list
        ZtringListList Parameters=File__Base::Info_Capacities_Parameters();

        //Filling
        MediaInfo_Temp.clear();
        size_t MediaInfo_Temp_Pos=0;
        for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
        {
            MediaInfo_Temp(MediaInfo_Temp_Pos, 0)=Config.Info_Get((stream_t)StreamKind)(_T("StreamKind"), Info_Measure);
            MediaInfo_Temp_Pos++;
            for (size_t Pos=0; Pos<Parameters(StreamKind).size(); Pos++)
            {
                MediaInfo_Temp(MediaInfo_Temp_Pos, 0)=Parameters(StreamKind, Pos);
                MediaInfo_Temp(MediaInfo_Temp_Pos, 0).resize(19, _T(' '));
                MediaInfo_Temp(MediaInfo_Temp_Pos, 0)+=_T(" :");
                for (size_t Format=0; Format<MediaInfo_Capacities.size(); Format++)
                {
                    MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)=MediaInfo_Capacities[Format](StreamKind, Pos);
                    MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1).resize(1, _T(' '));
                }
                MediaInfo_Temp_Pos++;
            }
            MediaInfo_Temp_Pos++;
        }

        //Merging
        MediaInfo_Temp.Separator_Set(0, Config.LineSeparator_Get());
        MediaInfo_Temp.Separator_Set(1, _T(" "));
        MediaInfo_Temp.Quote_Set(_T(""));
        Header.FindAndReplace(_T("\n"), Config.LineSeparator_Get(), 0, Ztring_Recursive);
        MediaInfo_Capacities_Final=Header+MediaInfo_Temp.Read();
        return MediaInfo_Capacities_Final;

        //Filling
        MediaInfo_Temp.clear();
        size_t MediaInfo_Temp_Pos=0;
        for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
        {
            size_t Config_Pos=Config.Info_Get((stream_t)StreamKind).Find(_T("StreamKind"));
            if (Config_Pos!=Error)
                MediaInfo_Temp(MediaInfo_Temp_Pos, 0)=Ztring(_T("  <td width=\"100%\" bgcolor=\"#EEEEEE\">"))+Config.Info_Get((stream_t)StreamKind)[Config_Pos][Info_Measure]+Ztring(_T("</td>"));
            MediaInfo_Temp_Pos++;
            for (size_t Pos=0; Pos<Parameters(StreamKind).size(); Pos++)
            {
                MediaInfo_Temp(MediaInfo_Temp_Pos, 0)=Ztring(_T("  <td width=\"10%\" bgcolor=\"#EEEEEE\">"))+Parameters(StreamKind, Pos)+Ztring(_T("</td>"));
                //MediaInfo_Temp(MediaInfo_Temp_Pos, 0).resize(19, _T(' '));
                //MediaInfo_Temp(MediaInfo_Temp_Pos, 0)+=_T(" :");
                for (size_t Format=0; Format<MediaInfo_Capacities.size(); Format++)
                {
                    MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)=MediaInfo_Capacities[Format](StreamKind, Pos);
                    if (MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1).empty())
                        MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)=_T("  <td width=\"10%\" bgcolor=\"#EEEEEE\">&nbsp;</td>");
                    else if (MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)[0]==_T('R'))
                        MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)=Ztring(_T("  <td width=\"10%\" bgcolor=\"#00AA00\"><div align=\"center\">"))+MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)+Ztring(_T("</div></td>"));
                    else if (MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)[0]==_T('N'))
                        MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)=Ztring(_T("  <td width=\"10%\" bgcolor=\"#FF0000\"><div align=\"center\">"))+MediaInfo_Temp(MediaInfo_Temp_Pos, Format+1)+Ztring(_T("</div></td>"));
                }
                MediaInfo_Temp_Pos++;
            }
            MediaInfo_Temp(MediaInfo_Temp_Pos, 0)=_T("  <td width=\"10%\" bgcolor=\"#EEEEEE\">&nbsp;</td>");
            MediaInfo_Temp_Pos++;
        }
        MediaInfo_Temp.resize(MediaInfo_Temp.size()-1);

        //Merging
        MediaInfo_Temp.Separator_Set(0, _T("\r\n </tr>\r\n <tr>\r\n"));
        MediaInfo_Temp.Separator_Set(1, _T("\r\n"));
        MediaInfo_Temp.Quote_Set(_T(""));
        Header.FindAndReplace(_T("\n"), Config.LineSeparator_Get(), 0, Ztring_Recursive);
        MediaInfo_Capacities_Final+=_T("<html>\r\n<head>\r\n</head>\r\n<body>\r\n");
        MediaInfo_Capacities_Final+=_T("<table width=\"100%\" BORDER CELLPADDING=0 CELLSPACING=0 >\r\n <tr>\r\n");
        MediaInfo_Capacities_Final+=_T(" <tr>\r\n  <td>&nbsp;</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">mkv</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">ogg</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">riff</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">mpeg</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">mpeg4</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">mpegv</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">mpega</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">mpc</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">wm</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">qt</td>\r\n");
        MediaInfo_Capacities_Final+=_T("  <td width=\"10%\">rm</td>\r\n");
        MediaInfo_Capacities_Final+=_T(" </tr>\r\n");
        MediaInfo_Capacities_Final+=MediaInfo_Temp.Read();
        MediaInfo_Capacities_Final+=_T("\r\n </tr>\r\n</table>\r\n</body>\r\n");
        return MediaInfo_Capacities_Final;
        */
    }
    else
        return Option_Static(Option, Value);
}

//---------------------------------------------------------------------------
String MediaInfo::Option_Static (const String &Option, const String &Value)
{
    Config.Init(); //Initialize Configuration

         if (Option==_T("Info_Capacities"))
    {
        return _T("Option desactivated for this version, will come back soon!");
        MediaInfo MI;
        return MI.Option(Option);
    }
    else if (Option==_T("Info_Version"))
    {
        Ztring ToReturn=Config.Info_Version_Get();
        #if defined(MEDIAINFO_VIDEO_NO) || defined(MEDIAINFO_AUDIO_NO) || defined(MEDIAINFO_RIFF_NO) || defined(MEDIAINFO_OGG_NO) || defined(MEDIAINFO_MPEGPS_NO) || defined(MEDIAINFO_MPEGA_NO) || defined(MEDIAINFO_WM_NO) || defined(MEDIAINFO_QT_NO) || defined(MEDIAINFO_RM_NO) || defined(MEDIAINFO_DVDV_NO) || defined(MEDIAINFO_AAC_NO) || defined(MEDIAINFO_MK_NO) || defined(MEDIAINFO_APE_NO) || defined(MEDIAINFO_FLAC_NO) || defined(MEDIAINFO_SNDFILE_NO) || defined(MEDIAINFO_FLV_NO) || defined(MEDIAINFO_SWF_NO)
            ToReturn+=_T(" modified");
        #endif
        return ToReturn;
    }
    else
        return Config.Option(Option, Value);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Count_Get (stream_t StreamKind, size_t StreamNumber)
{
    if (!Info)
        return 0;

    return Info->Count_Get(StreamKind, StreamNumber);
}

//---------------------------------------------------------------------------
size_t MediaInfo::State_Get ()
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
void MediaInfo::Buffer_Clear()
{
    Buffer_Size_Max=0;
    delete[] Buffer; Buffer=NULL;
    Buffer_Size=0;
    File_Size=0;
}

//---------------------------------------------------------------------------
int MediaInfo::ApplyMethod()
{
    switch (InternalMethod)
    {
        case 1 : //Open file
            return Format_Test();
        case 2 : //Open buffer
            return Format_Test_Buffer();
        case 3 : //Supported formats
            if (Info)
                MediaInfo_Temp=Info->Info_Capacities();
            //MediaInfo_Capacities.resize(MediaInfo_Capacities.size()+1, MediaInfo_Temp);
            delete Info; Info=NULL;
            return 0; //We want to continue the format listing
    }
    return 0;
}

} //NameSpace


