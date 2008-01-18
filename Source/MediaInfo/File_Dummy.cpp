// File_Dummy - Fill with Name of tags
// Copyright (C) 2005-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_DUMMY_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File_Dummy.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dummy::Read_File()
{
    File_Name=_T("D:\\Example\\"); File_Name+=KindOfDummy;
         if (KindOfDummy==_T("Album"))
    {
        Fill_Dummy_General();
        Fill_Dummy_Audio();
    }
    else if (KindOfDummy==_T("Comic"))
    {
        Fill_Dummy_General();
        Fill_Dummy_Video();
    }
    else if (KindOfDummy==_T("Movie"))
    {
        Fill_Dummy_General();
        Fill_Dummy_Video();
        Fill_Dummy_Video();
        Fill_Dummy_Audio();
        Fill_Dummy_Audio();
        Fill_Dummy_Audio();
        Fill_Dummy_Audio();
        Fill_Dummy_Text();
        Fill_Dummy_Text();
        Fill_Dummy_Text();
        Fill_Dummy_Text();
        Fill_Dummy_Chapters();
        Fill_Dummy_Chapters();
    }
    else
    {
        File_Name=_T("D:\\WhatIsIt.mkv");
        size_t Pos;
        Pos=Stream_Prepare(Stream_General);
        General[Pos](_T("Domain"))=_T("Starwars saga");
        General[Pos](_T("Movie"))=_T("Starwars 4");
        General[Pos](_T("Movie/More"))=_T("A new hope");
        General[Pos](_T("Director"))=_T("Georges Lucas");
        General[Pos](_T("Released_Date"))=_T("1977");
        General[Pos](_T("FileSize"))=_T("734000000");
        General[Pos](_T("Format"))=_T("Matroska");
        General[Pos](_T("Format/Url"))=_T("http://mediainfo.sourceforge.net");
        Pos=Stream_Prepare(Stream_Video);
        Video[Pos](_T("Codec"))=_T("XVID");
        Video[Pos](_T("Codec/Url"))=_T("http://mediainfo.sourceforge.net");
        Video[Pos](_T("BitRate"))=_T("800000");
        Video[Pos](_T("Width"))=_T("720");
        Video[Pos](_T("Height"))=_T("320");
        Video[Pos](_T("FrameRate"))=_T("24.976");
        Video[Pos](_T("Resolution"))=_T("24");
        Video[Pos](_T("Language"))=_T("en");
        Pos=Stream_Prepare(Stream_Audio);
        Audio[Pos](_T("Codec"))=_T("AC3");
        Audio[Pos](_T("Codec/Url"))=_T("http://mediainfo.sourceforge.net");
        Audio[Pos](_T("BitRate"))=_T("384000");
        Audio[Pos](_T("BitRate_Mode"))=_T("CBR");
        Audio[Pos](_T("Channel(s)"))=_T("6");
        Audio[Pos](_T("SamplingRate"))=_T("48000");
        Audio[Pos](_T("Language"))=_T("en");
        Pos=Stream_Prepare(Stream_Text);
        Text[Pos](_T("Codec"))=_T("SSA");
        Text[Pos](_T("Codec/Url"))=_T("http://mediainfo.sourceforge.net");
        Text[Pos](_T("Language"))=_T("en");
        Text[Pos](_T("Language_More"))=_T("Forced");
        Pos=Stream_Prepare(Stream_Chapters);
        Chapters[Pos](_T("Total"))=_T("16");
        Chapters[Pos](_T("Language"))=_T("en");
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dummy::Fill_Dummy_General()
{
    size_t Pos=Stream_Prepare(Stream_General);
    General[Pos](_T("Format"))=_T("Format");
    General[Pos](_T("Format/String"))=_T("Format, longer version");
    General[Pos](_T("Format/More"))=_T("More information about this format");
    General[Pos](_T("Format/Url"))=_T("http://mediainfo.sourceforge.net");
    General[Pos](_T("Format/Extensions"))=_T("fmt fmt fmt");
    General[Pos](_T("FileSize"))=_T("1000000");
    General[Pos](_T("PlayTime"))=_T("10000");
    General[Pos](_T("Domain"))=_T("Domain");
    General[Pos](_T("Domain/Position_Total"))=_T("Domain/Position_Total");
    General[Pos](_T("Collection"))=_T("Collection");
    General[Pos](_T("Collection/Position_Total"))=_T("Collection/Position_Total");
    General[Pos](_T("Season"))=_T("Season");
    General[Pos](_T("Season/Position_Total"))=_T("Season/Position_Total");
         if (KindOfDummy==_T("Album"))
    {
        General[Pos](_T("Album"))=_T("Album name");
        General[Pos](_T("Album/More"))=_T("More information about the album");
        General[Pos](_T("Album/Position_Total"))=_T("Album/Position_Total");
        General[Pos](_T("Part"))=_T("Part");
        General[Pos](_T("Part/Position_Total"))=_T("Part/Position_Total");
        General[Pos](_T("Part/Position"))=_T("Part/Position");
        General[Pos](_T("Track"))=_T("Track");
        General[Pos](_T("Track/Position"))=_T("Track/Position");
        General[Pos](_T("Track/More"))=_T("More information about the track");
    }
    else if (KindOfDummy==_T("Comic"))
    {
        General[Pos](_T("Comic"))=_T("Comic name");
        General[Pos](_T("Comic/More"))=_T("More information about the comic");
    }
    else //if (KindOfDummy==_T("Movie"))
    {
        General[Pos](_T("Movie"))=_T("Movie name");
        General[Pos](_T("Movie/More"))=_T("More information about the movie");
    }
    General[Pos](_T("Performer"))=_T("Performer");
    General[Pos](_T("Artist"))=_T("Artist");
    General[Pos](_T("Performer/Sort"))=_T("Performer/Sort");
    General[Pos](_T("Performer/Url"))=_T("Performer/Url");
    General[Pos](_T("Original/Performer"))=_T("Original/Performer");
    General[Pos](_T("Accompaniment"))=_T("Accompaniment");
    General[Pos](_T("Musician_Instrument"))=_T("Musician_Instrument");
    General[Pos](_T("Composer"))=_T("Composer");
    General[Pos](_T("Composer/Nationality"))=_T("Composer/Nationality");
    General[Pos](_T("Arranger"))=_T("Arranger");
    General[Pos](_T("Lyricist"))=_T("Lyricist");
    General[Pos](_T("Original/Lyricist"))=_T("Original/Lyricist");
    General[Pos](_T("Conductor"))=_T("Conductor");
    General[Pos](_T("Actor"))=_T("Actor");
    General[Pos](_T("Actor_Character"))=_T("Actor_Character");
    General[Pos](_T("WrittenBy"))=_T("WrittenBy");
    General[Pos](_T("ScreenplayBy"))=_T("ScreenplayBy");
    General[Pos](_T("Director"))=_T("Director");
    General[Pos](_T("AssistantDirector"))=_T("AssistantDirector");
    General[Pos](_T("DirectorOfPhotography"))=_T("DirectorOfPhotography");
    General[Pos](_T("ArtDirector"))=_T("ArtDirector");
    General[Pos](_T("EditedBy"))=_T("EditedBy");
    General[Pos](_T("Producer"))=_T("Producer");
    General[Pos](_T("CoProducer"))=_T("CoProducer");
    General[Pos](_T("ExecutiveProducer"))=_T("ExecutiveProducer");
    General[Pos](_T("ProductionDesigner"))=_T("ProductionDesigner");
    General[Pos](_T("CostumeDesigner"))=_T("CostumeDesigner");
    General[Pos](_T("Choregrapher"))=_T("Choregrapher");
    General[Pos](_T("SoundEngineer"))=_T("SoundEngineer");
    General[Pos](_T("MasteredBy"))=_T("MasteredBy");
    General[Pos](_T("RemixedBy"))=_T("RemixedBy");
    General[Pos](_T("ProductionStudio"))=_T("ProductionStudio");
    General[Pos](_T("Label"))=_T("Label");
    General[Pos](_T("Publisher"))=_T("Publisher");
    General[Pos](_T("Publisher/URL"))=_T("Publisher/URL");
    General[Pos](_T("DistributedBy"))=_T("DistributedBy");
    General[Pos](_T("EncodedBy"))=_T("EncodedBy");
    General[Pos](_T("ThanksTo"))=_T("ThanksTo");
    General[Pos](_T("Technician"))=_T("Technician");
    General[Pos](_T("CommissionedBy"))=_T("CommissionedBy");
    General[Pos](_T("Encoded_Original/DistributedBy"))=_T("Encoded_Original/DistributedBy");
    General[Pos](_T("Broadcaster"))=_T("Broadcaster");
    General[Pos](_T("Broadcaster/Owner"))=_T("Broadcaster/Owner");
    General[Pos](_T("Broadcaster/URL"))=_T("Broadcaster/URL");
    General[Pos](_T("ContentType"))=_T("ContentType");
    General[Pos](_T("Subject"))=_T("Subject");
    General[Pos](_T("Synopsys"))=_T("Synopsys");
    General[Pos](_T("Summary"))=_T("Summary");
    General[Pos](_T("Description"))=_T("Description");
    General[Pos](_T("Keywords"))=_T("Keywords");
    General[Pos](_T("Period"))=_T("Period");
    General[Pos](_T("LawRating"))=_T("LawRating");
    General[Pos](_T("IRCA"))=_T("IRCA");
    General[Pos](_T("Language"))=_T("Language");
    General[Pos](_T("Medium"))=_T("Medium");
    General[Pos](_T("Product"))=_T("Product");
    General[Pos](_T("Country"))=_T("Country");
    General[Pos](_T("Written_Date"))=_T("Written_Date");
    General[Pos](_T("Recorded_Date"))=_T("Recorded_Date");
    General[Pos](_T("Released_Date"))=_T("Released_Date");
    General[Pos](_T("Mastered_Date"))=_T("Mastered_Date");
    General[Pos](_T("Encoded_Date"))=_T("Encoded_Date");
    General[Pos](_T("Tagged_Date"))=_T("Tagged_Date");
    General[Pos](_T("Original/Released_Date"))=_T("Original/Released_Date");
    General[Pos](_T("Original/Recorded_Date"))=_T("Original/Recorded_Date");
    General[Pos](_T("Written_Location"))=_T("Written_Location");
    General[Pos](_T("Recorded_Location"))=_T("Recorded_Location");
    General[Pos](_T("Archival_Location"))=_T("Archival_Location");
    General[Pos](_T("Genre"))=_T("Genre");
    General[Pos](_T("Mood"))=_T("Mood");
    General[Pos](_T("Comment"))=_T("Comment");
    General[Pos](_T("Rating "))=_T("Rating ");
    General[Pos](_T("Encoded_Application"))=_T("Encoded_Application");
    General[Pos](_T("Encoded_Library"))=_T("Encoded_Library");
    General[Pos](_T("Encoded_Library_Settings"))=_T("Encoded_Library_Settings");
    General[Pos](_T("Encoded_Original"))=_T("Encoded_Original");
    General[Pos](_T("Encoded_Original/Url"))=_T("Encoded_Original/Url");
    General[Pos](_T("Copyright"))=_T("Copyright");
    General[Pos](_T("Producer_Copyright"))=_T("Producer_Copyright");
    General[Pos](_T("TermsOfUse"))=_T("TermsOfUse");
    General[Pos](_T("Copyright/Url"))=_T("Copyright/Url");
    General[Pos](_T("ISRC"))=_T("ISRC");
    General[Pos](_T("MSDI"))=_T("MSDI");
    General[Pos](_T("ISBN"))=_T("ISBN");
    General[Pos](_T("BarCode"))=_T("BarCode");
    General[Pos](_T("LCCN"))=_T("LCCN");
    General[Pos](_T("CatalogNumber"))=_T("CatalogNumber");
    General[Pos](_T("LabelCode"))=_T("LabelCode");
    General[Pos](_T("Cover"))=_T("Y");
    General[Pos](_T("Cover_Datas"))=_T("Cover_Datas");
    General[Pos](_T("Summary"))=_T("Summary");
    General[Pos](_T("BPM"))=_T("100");
}

//---------------------------------------------------------------------------
void File_Dummy::Fill_Dummy_Video()
{
    size_t Pos=Stream_Prepare(Stream_Video);
    Video[Pos](_T("ID"))=_T("ID");
    Video[Pos](_T("UniqueID"))=_T("UniqueID");
    Video[Pos](_T("Title"))=_T("Title");
    Video[Pos](_T("Codec"))=_T("Codec"); Video[Pos](_T("Codec"))+=Ztring::ToZtring(Pos+1);
    Video[Pos](_T("Codec/Info"))=_T("Codec/Info");
    Video[Pos](_T("Codec/Url"))=_T("http://--Codec/Url--");
    Video[Pos](_T("BitRate"))=_T("10000");
    Video[Pos](_T("BitRate_Mode"))=_T("BitRate_Mode");
    Video[Pos](_T("Encoded_Library"))=_T("Encoded_Library");
    Video[Pos](_T("Encoded_Library_Settings"))=_T("Encoded_Library_Settings");
         if (KindOfDummy==_T("Album"))
    {
        Video[Pos](_T("Width"))=_T("2000");
        Video[Pos](_T("Height"))=_T("3000");
    }
    else //if (KindOfDummy==_T("Movie"))
    {
        Video[Pos](_T("DisplayAspectRatio"))=_T("2");
        Video[Pos](_T("FrameRate"))=_T("24.976");
        Video[Pos](_T("FrameCount"))=_T("FrameCount");
        Video[Pos](_T("Resolution"))=_T("24");
        Video[Pos](_T("Bits-(Pixel*Frame)"))=_T("Bits/(Pixel*Frame)");
        Video[Pos](_T("Delay"))=_T("100");
        Video[Pos](_T("PlayTime"))=_T("990000");
    }
    Video[Pos](_T("Language"))=_T("eng");
    Video[Pos](_T("Language_More"))=_T("Language_More");
}

//---------------------------------------------------------------------------
void File_Dummy::Fill_Dummy_Audio()
{
    size_t Pos=Stream_Prepare(Stream_Audio);
    Audio[Pos](_T("ID"))=_T("ID");
    Audio[Pos](_T("UniqueID"))=_T("UniqueID");
    Audio[Pos](_T("Title"))=_T("Title");
    Audio[Pos](_T("Codec"))=_T("Codec"); Audio[Pos](_T("Codec"))+=Ztring::ToZtring(Pos+1);
    Audio[Pos](_T("Codec/Info"))=_T("Codec/Info");
    Audio[Pos](_T("Codec/Url"))=_T("http://--Codec/Url--");
    Audio[Pos](_T("BitRate"))=_T("1000");
    Audio[Pos](_T("BitRate_Mode"))=_T("BitRate_Mode");
    Audio[Pos](_T("Encoded_Library"))=_T("Encoded_Library");
    Audio[Pos](_T("Encoded_Library_Settings"))=_T("Encoded_Library_Settings");
    Audio[Pos](_T("Channel(s)"))=Ztring::ToZtring(Pos+1);
    Audio[Pos](_T("ChannelPositions"))=_T("ChannelPositions");
    Audio[Pos](_T("SamplingRate"))=_T("48000");
    Audio[Pos](_T("SamplingCount"))=_T("SamplingCount");
    Audio[Pos](_T("Resolution"))=_T("Resolution");
    Audio[Pos](_T("Delay"))=_T("10");
    Audio[Pos](_T("PlayTime"))=_T("100000");
    Audio[Pos](_T("Language"))=_T("fre");
    Audio[Pos](_T("Language_More"))=_T("Language_More");
    Audio[Pos](_T("Summary"))=_T("Summary");
}

//---------------------------------------------------------------------------
void File_Dummy::Fill_Dummy_Text()
{
    size_t Pos=Stream_Prepare(Stream_Text);
    Text[Pos](_T("ID"))=_T("ID");
    Text[Pos](_T("UniqueID"))=_T("UniqueID");
    Text[Pos](_T("Title"))=_T("Title");
    Text[Pos](_T("Codec"))=_T("Codec"); Text[Pos](_T("Codec"))+=Ztring::ToZtring(Pos+1);
    Text[Pos](_T("Codec/Url"))=_T("http://--Codec/Url--");
    Text[Pos](_T("Delay"))=_T("100");
    Text[Pos](_T("PlayTime"))=_T("100");
    Text[Pos](_T("Language"))=_T("de");
    Text[Pos](_T("Language_More"))=_T("Language_More");
    Text[Pos](_T("Summary"))=_T("Summary");
}

//---------------------------------------------------------------------------
void File_Dummy::Fill_Dummy_Chapters()
{
    size_t Pos=Stream_Prepare(Stream_Chapters);
    Chapters[Pos](_T("ID"))=_T("ID");
    Chapters[Pos](_T("UniqueID"))=_T("UniqueID");
    Chapters[Pos](_T("Title"))=_T("Title");
    Chapters[Pos](_T("Codec"))=_T("Codec"); Chapters[Pos](_T("Codec"))+=Ztring::ToZtring(Pos+1);
    Chapters[Pos](_T("Codec/Url"))=_T("http://--Codec/Url--");
    Chapters[Pos](_T("Total"))=_T("Total");
    Chapters[Pos](_T("Language"))=_T("de");
    Chapters[Pos](_T("Summary"))=_T("Summary");
}

} //NameSpace

#endif
