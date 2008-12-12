// File_Mpeg_Descriptors - Info for MPEG files
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGPS_YES) || defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg_Descriptors.h"
#ifdef MEDIAINFO_MPEG4_YES
    #include "MediaInfo/Multiple/File_Mpeg4_Descriptors.h"
#endif
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
//Extern
extern const char* Avc_profile_idc(int8u profile_idc);

//---------------------------------------------------------------------------
const char* Mpeg_Descriptors_audio_type(int8u ID)
{
    switch (ID)
    {
        case 0x00 : return "Undefined";
        case 0x01 : return "Clean effects";
        case 0x02 : return "Hearing impaired";
        case 0x03 : return "Visual impaired commentary";
        default   : return "Reserved";
    }
}

const char* Mpeg_Descriptors_alignment_type(int8u alignment_type)
{
    switch (alignment_type)
    {
        case 0x01 : return "Slice or video access unit (Video), or sync word (Audio)";
        case 0x02 : return "Video access unit";
        case 0x03 : return "GOP, or SEQ";
        case 0x04 : return "SEQ";
        default   : return "Reserved";
    }
}

const char* Mpeg_Descriptors_teletext_type(int8u teletext_type)
{
    switch (teletext_type)
    {
        case 0x01 : return "initial Teletext page";
        case 0x02 : return "Teletext subtitle page";
        case 0x03 : return "additional information page";
        case 0x04 : return "programme schedule page";
        case 0x05 : return "Teletext subtitle page for hearing impaired people";
        default   : return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_1(int8u content_nibble_level_1)
{
    switch (content_nibble_level_1)
    {
        case 0x00 : return "undefined";
        case 0x01 : return "movie/drama";
        case 0x02 : return "news/current affairs";
        case 0x03 : return "show/game show";
        case 0x04 : return "sports";
        case 0x05 : return "children's/youth programmes";
        case 0x06 : return "music/ballet/dance";
        case 0x07 : return "arts/culture (without music)";
        case 0x08 : return "social/political issues/economics";
        case 0x09 : return "education/science/factual topics";
        case 0x0A : return "leisure hobbies";
        case 0x0B : return "Special characteristics:";
        default   :
            if (content_nibble_level_1==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_01(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "movie/drama";
        case 0x01 : return "detective/thriller";
        case 0x02 : return "adventure/western/war";
        case 0x03 : return "science fiction/fantasy/horror";
        case 0x04 : return "comedy";
        case 0x05 : return "soap/melodrama/folkloric";
        case 0x06 : return "romance";
        case 0x07 : return "serious/classical/religious/historical movie/drama";
        case 0x08 : return "adult movie/drama";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_02(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "news/current affairs";
        case 0x01 : return "news/weather report";
        case 0x02 : return "news magazine";
        case 0x03 : return "documentary";
        case 0x04 : return "discussion/interview/debate";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_03(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "show/game show";
        case 0x01 : return "game show/quiz/contest";
        case 0x02 : return "variety show";
        case 0x03 : return "talk show";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_04(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "sports";
        case 0x01 : return "special events";
        case 0x02 : return "sports magazines";
        case 0x03 : return "football/soccer";
        case 0x04 : return "tennis/squash";
        case 0x05 : return "team sports (excluding football)";
        case 0x06 : return "athletics";
        case 0x07 : return "motor sport";
        case 0x08 : return "water sport";
        case 0x09 : return "winter sports";
        case 0x0A : return "equestrian";
        case 0x0B : return "martial sports";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_05(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "children's/youth programmes";
        case 0x01 : return "pre-school children's programmes";
        case 0x02 : return "entertainment programmes for 6 to 14";
        case 0x03 : return "entertainment programmes for 10 to 16";
        case 0x04 : return "informational/educational/school programmes";
        case 0x05 : return "cartoons/puppets";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_06(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "music/ballet/dance";
        case 0x01 : return "rock/pop";
        case 0x02 : return "serious music/classical music";
        case 0x03 : return "folk/traditional music";
        case 0x04 : return "jazz";
        case 0x05 : return "musical/opera";
        case 0x06 : return "ballet";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_07(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "arts/culture (without music)";
        case 0x01 : return "performing arts";
        case 0x02 : return "fine arts";
        case 0x03 : return "religion";
        case 0x04 : return "popular culture/traditional arts";
        case 0x05 : return "literature";
        case 0x06 : return "film/cinema";
        case 0x07 : return "experimental film/video";
        case 0x08 : return "broadcasting/press";
        case 0x09 : return "new media";
        case 0x0A : return "arts/culture magazines";
        case 0x0B : return "fashion";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_08(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "social/political issues/economics";
        case 0x01 : return "magazines/reports/documentary";
        case 0x02 : return "economics/social advisory";
        case 0x03 : return "remarkable people";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_09(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "education/science/factual topics";
        case 0x01 : return "nature/animals/environment";
        case 0x02 : return "technology/natural sciences";
        case 0x03 : return "medicine/physiology/psychology";
        case 0x04 : return "foreign countries/expeditions";
        case 0x05 : return "social/spiritual sciences";
        case 0x06 : return "further education";
        case 0x07 : return "languages";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_0A(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "leisure hobbies";
        case 0x01 : return "tourism/travel";
        case 0x02 : return "handicraft";
        case 0x03 : return "motoring";
        case 0x04 : return "fitness and health";
        case 0x05 : return "cooking";
        case 0x06 : return "advertisement/shopping";
        case 0x07 : return "gardening";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2_0B(int8u content_nibble_level_2)
{
    switch (content_nibble_level_2)
    {
        case 0x00 : return "original language";
        case 0x01 : return "black and white";
        case 0x02 : return "unpublished";
        case 0x03 : return "live broadcast";
        default   :
            if (content_nibble_level_2==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_content_nibble_level_2(int8u content_nibble_level_1, int8u content_nibble_level_2)
{
    switch (content_nibble_level_1)
    {
        case 0x00 : return "undefined";
        case 0x01 : return Mpeg_Descriptors_content_nibble_level_2_01(content_nibble_level_2);
        case 0x02 : return Mpeg_Descriptors_content_nibble_level_2_02(content_nibble_level_2);
        case 0x03 : return Mpeg_Descriptors_content_nibble_level_2_03(content_nibble_level_2);
        case 0x04 : return Mpeg_Descriptors_content_nibble_level_2_04(content_nibble_level_2);
        case 0x05 : return Mpeg_Descriptors_content_nibble_level_2_05(content_nibble_level_2);
        case 0x06 : return Mpeg_Descriptors_content_nibble_level_2_06(content_nibble_level_2);
        case 0x07 : return Mpeg_Descriptors_content_nibble_level_2_07(content_nibble_level_2);
        case 0x08 : return Mpeg_Descriptors_content_nibble_level_2_08(content_nibble_level_2);
        case 0x09 : return Mpeg_Descriptors_content_nibble_level_2_09(content_nibble_level_2);
        case 0x0A : return Mpeg_Descriptors_content_nibble_level_2_0A(content_nibble_level_2);
        case 0x0B : return Mpeg_Descriptors_content_nibble_level_2_0B(content_nibble_level_2);
        default   :
            if (content_nibble_level_1==0x0F)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_linkage_type(int8u linkage_type)
{
    switch (linkage_type)
    {
        case 0x00 : return "reserved for future use";
        case 0x01 : return "information service";
        case 0x02 : return "Electronic Programme Guide (EPG) service";
        case 0x03 : return "CA replacement service";
        case 0x04 : return "transport stream containing complete Network/Bouquet SI";
        case 0x05 : return "service replacement service";
        case 0x06 : return "data broadcast service";
        case 0xFF : return "reserved for future use";
        default   :
            if (linkage_type>=0x80)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_dvb_service_type(int8u service_type)
{
    switch (service_type)
    {
        case 0x00 : return "reserved for future use";
        case 0x01 : return "Digital television";
        case 0x02 : return "Digital radio";
        case 0x03 : return "Teletext";
        case 0x04 : return "NVOD reference";
        case 0x05 : return "NVOD time-shifted";
        case 0x06 : return "Mosaic";
        case 0x07 : return "PAL coded signal";
        case 0x08 : return "SECAM coded signal";
        case 0x09 : return "D/D2-MAC";
        case 0x0A : return "FM Radio";
        case 0x0B : return "NTSC coded signal";
        case 0x0C : return "Data";
        case 0xFF : return "reserved for future use";
        default   :
            if (service_type>=0x80)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_stream_content(int8u stream_content)
{
    switch (stream_content)
    {
        case 0x01 : return "MPEG-2 Video";
        case 0x02 : return "MPEG-1 Audio L2";
        case 0x03 : return "Subtitle";
        case 0x04 : return "AC3";
        case 0x05 : return "AVC";
        case 0x06 : return "HE-AAC";
        case 0x07 : return "DTS";
        default   :
            if (stream_content>=0x0C)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O1(int8u component_type)
{
    switch (component_type)
    {
        case 0x01 : return "4:3 aspect ratio, 25 Hz";
        case 0x02 : return "16:9 aspect ratio with pan vectors, 25 Hz";
        case 0x03 : return "16:9 aspect ratio without pan vectors, 25 Hz";
        case 0x04 : return ">16:9 aspect ratio, 25 Hz";
        case 0x05 : return "4:3 aspect ratio, 30 Hz";
        case 0x06 : return "16:9 aspect ratio with pan vectors, 30 Hz";
        case 0x07 : return "16:9 aspect ratio without pan vectors, 30 Hz";
        case 0x08 : return ">16:9 aspect ratio, 30 Hz";
        case 0x09 : return "4:3 aspect ratio, 25 Hz (high definition)";
        case 0x0A : return "16:9 aspect ratio with pan vectors, 25 Hz (high definition)";
        case 0x0B : return "16:9 aspect ratio without pan vectors, 25 Hz (high definition)";
        case 0x0C : return ">16:9 aspect ratio, 25 Hz (high definition)";
        case 0x0D : return "4:3 aspect ratio, 30 Hz (high definition)";
        case 0x0E : return "16:9 aspect ratio with pan vectors, 30 Hz (high definition)";
        case 0x0F : return "16:9 aspect ratio without pan vectors, 30 Hz (high definition)";
        case 0x10 : return ">16:9 aspect ratio, 30 Hz (high definition)";
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O2(int8u component_type)
{
    switch (component_type)
    {
        case 0x01 : return "single mono channel";
        case 0x02 : return "dual mono channel";
        case 0x03 : return "stereo (2 channel)";
        case 0x04 : return "multi-lingual, multi-channel";
        case 0x05 : return "surround sound";
        case 0x40 : return "description for the visually impaired";
        case 0x41 : return "for the hard of hearing";
        case 0x42 : return "receiver-mixed supplementary audio";
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O3(int8u component_type)
{
    switch (component_type)
    {
        case 0x01 : return "EBU Teletext subtitles";
        case 0x02 : return "associated EBU Teletext";
        case 0x03 : return "VBI data";
        case 0x10 : return "DVB subtitles (normal) with no monitor aspect ratio criticality";
        case 0x11 : return "DVB subtitles (normal) for display on 4:3 aspect ratio monitor";
        case 0x12 : return "DVB subtitles (normal) for display on 16:9 aspect ratio monitor";
        case 0x13 : return "DVB subtitles (normal) for display on 2.21:1 aspect ratio monitor";
        case 0x20 : return "DVB subtitles (for the hard of hearing) with no monitor aspect ratio criticality";
        case 0x21 : return "DVB subtitles (for the hard of hearing) for display on 4:3 aspect ratio monitor";
        case 0x22 : return "DVB subtitles (for the hard of hearing) for display on 16:9 aspect ratio monitor";
        case 0x23 : return "DVB subtitles (for the hard of hearing) for display on 2.21:1 aspect ratio monitor";
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O4(int8u)
{
    return "Defined by AC3";
}

const char* Mpeg_Descriptors_component_type_O5(int8u component_type)
{
    switch (component_type)
    {
        case 0x01 : return "4:3 aspect ratio, 25 Hz";
        case 0x03 : return "16:9 aspect ratio, 25 Hz";
        case 0x04 : return ">16:9 aspect ratio, 25 Hz";
        case 0x05 : return "4:3 aspect ratio, 30 Hz";
        case 0x07 : return "16:9 aspect ratio, 30 Hz";
        case 0x08 : return ">16:9 aspect ratio, 30 Hz";
        case 0x0B : return "16:9 aspect ratio, 25 Hz (high definition)";
        case 0x0C : return ">16:9 aspect ratio, 25 Hz (high definition)";
        case 0x0F : return "16:9 aspect ratio, 30 Hz (high definition)";
        case 0x10 : return ">16:9 aspect ratio, 30 Hz (high definition)";
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O6(int8u component_type)
{
    switch (component_type)
    {
        case 0x01 : return "single mono channel";
        case 0x03 : return "stereo";
        case 0x05 : return "surround sound";
        case 0x40 : return "description for the visually impaired";
        case 0x41 : return "for the hard of hearing";
        case 0x42 : return "receiver-mixed supplementary audio";
        case 0x43 : return "astereo (v2)";
        case 0x44 : return "description for the visually impaired (v2)";
        case 0x45 : return "for the hard of hearing (v2)";
        case 0x46 : return "receiver-mixed supplementary audio (v2)";
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O7(int8u)
{
    return "Defined by DTS";
}

const char* Mpeg_Descriptors_codepage_1(int8u codepage)
{
    switch (codepage)
    {
        case 0x01 : return "ISO/IEC 8859-5 (Cyrillic)";
        case 0x02 : return "ISO/IEC 8859-6 (Arabic)";
        case 0x03 : return "ISO/IEC 8859-7 (Greek)";
        case 0x04 : return "ISO/IEC 8859-8 (Hebrew)";
        case 0x05 : return "ISO/IEC 8859-9 (Latin)";
        case 0x06 : return "ISO/IEC 8859-10 (Latin)";
        case 0x07 : return "ISO/IEC 8859-11 (Thai)";
        case 0x08 : return "ISO/IEC 8859-12 (Indian)";
        case 0x09 : return "ISO/IEC 8859-13 (Latin)";
        case 0x0A : return "ISO/IEC 8859-14 (Celtic)";
        case 0x0B : return "ISO/IEC 8859-15 (Latin)";
        case 0x11 : return "ISO/IEC 10646-1 (Basic Multilingual Plane)";
        case 0x12 : return "KSC5601-1987 (Korean)";
        case 0x13 : return "GB-2312-1980 (Simplified Chinese)";
        case 0x14 : return "Big5 (Traditional Chinese)";
        case 0x15 : return "UTF-8 (Basic Multilingual Plane)";
        default   : return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type(int8u stream_content, int8u component_type)
{
    switch (stream_content)
    {
        case 0x01 : return Mpeg_Descriptors_component_type_O1(component_type);
        case 0x02 : return Mpeg_Descriptors_component_type_O2(component_type);
        case 0x03 : return Mpeg_Descriptors_component_type_O3(component_type);
        case 0x04 : return Mpeg_Descriptors_component_type_O4(component_type);
        case 0x05 : return Mpeg_Descriptors_component_type_O5(component_type);
        case 0x06 : return Mpeg_Descriptors_component_type_O6(component_type);
        case 0x07 : return Mpeg_Descriptors_component_type_O7(component_type);
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_format_identifier(int32u format_identifier)
{
    switch (format_identifier)
    {
        case Mpeg_Descriptors::AC_3 : return "AC3";
        case Mpeg_Descriptors::BSSD : return "PCM (AES3)";
        case Mpeg_Descriptors::CUEI : return "SCTE 35 2003 - Digital Program Insertion Cueing Message for Cable";
        case Mpeg_Descriptors::DTS1 : return "DTS (512)";
        case Mpeg_Descriptors::DTS2 : return "DTS (1024)";
        case Mpeg_Descriptors::DTS3 : return "DTS (2048)";
        case Mpeg_Descriptors::GA94 : return "ATSC A/53 - Terrestrial";
        case Mpeg_Descriptors::HDMV : return "Blu-ray";
        case Mpeg_Descriptors::S14A : return "ATSC - Satellite";
        case Mpeg_Descriptors::SCTE : return "SCTE 54 2003 - Digital Video Service Multiplex and Transport System for Cable Television";
        case Mpeg_Descriptors::VC_1 : return "VC1";
        default :                     return "";
    }
}

const char* Mpeg_Descriptors_stream_Format(int8u descriptor_tag, int32u format_identifier)
{
    switch (descriptor_tag)
    {
        case 0x02 : return "MPEG Video";
        case 0x03 : return "MPEG Audio";
        case 0x1B : return "MPEG-4 Visual";
        case 0x1C : return "AAC";
        case 0x28 : return "AVC";
        case 0x2B : return "AAC";
        case 0x2D : return "Text";
        default :
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (descriptor_tag)
                        {
                            case 0x81 : return "AC-3";
                            default   : return "";
                        }
                case Mpeg_Descriptors::AC_3 : return "AC-3";
                case Mpeg_Descriptors::DTS1 : return "DTS";
                case Mpeg_Descriptors::DTS2 : return "DTS";
                case Mpeg_Descriptors::DTS3 : return "DTS";
                case Mpeg_Descriptors::VC_1 : return "VC-1";
                case Mpeg_Descriptors::drac : return "Dirac";
                default                     :
                        switch (descriptor_tag)
                        {
                            case 0x56 : return "Teletext";
                            case 0x59 : return "DVB Subtitles";
                            case 0x6A : return "AC-3";
                            case 0x7A : return "E-AC-3";
                            case 0x7B : return "DTS";
                            case 0x7C : return "AAC";
                            case 0x81 : return "AC-3";
                            default   : return "";
                        }
            }
    }
}

const char* Mpeg_Descriptors_stream_Codec(int8u descriptor_tag, int32u format_identifier)
{
    switch (descriptor_tag)
    {
        case 0x02 : return "MPEG-V";
        case 0x03 : return "MPEG-A";
        case 0x1B : return "MPEG-4V";
        case 0x1C : return "AAC";
        case 0x28 : return "AVC";
        case 0x2B : return "AAC";
        case 0x2D : return "Text";
        default :
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (descriptor_tag)
                        {
                            case 0x81 : return "AC3";
                            default   : return "";
                        }
                case Mpeg_Descriptors::AC_3 : return "AC3";
                case Mpeg_Descriptors::DTS1 : return "DTS";
                case Mpeg_Descriptors::DTS2 : return "DTS";
                case Mpeg_Descriptors::DTS3 : return "DTS";
                case Mpeg_Descriptors::VC_1 : return "VC-1";
                case Mpeg_Descriptors::drac : return "Dirac";
                default                     :
                        switch (descriptor_tag)
                        {
                            case 0x56 : return "Teletext";
                            case 0x59 : return "DVB Subtitles";
                            case 0x6A : return "AC3";
                            case 0x7A : return "AC3+";
                            case 0x7B : return "DTS";
                            case 0x7C : return "AAC";
                            case 0x81 : return "AC3";
                            default   : return "";
                        }
            }
    }
}

stream_t Mpeg_Descriptors_stream_Kind(int8u descriptor_tag, int32u format_identifier)
{
    switch (descriptor_tag)
    {
        case 0x02 : return Stream_Video;
        case 0x03 : return Stream_Audio;
        case 0x1B : return Stream_Video;
        case 0x1C : return Stream_Audio;
        case 0x28 : return Stream_Video;
        case 0x2B : return Stream_Audio;
        case 0x2D : return Stream_Text;
        default :
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (descriptor_tag)
                        {
                            case 0x81 : return Stream_Audio;
                            default   : return Stream_Max;
                        }
                case Mpeg_Descriptors::AC_3 : return Stream_Audio;
                case Mpeg_Descriptors::DTS1 : return Stream_Audio;
                case Mpeg_Descriptors::DTS2 : return Stream_Audio;
                case Mpeg_Descriptors::DTS3 : return Stream_Audio;
                case Mpeg_Descriptors::VC_1 : return Stream_Video;
                case Mpeg_Descriptors::drac : return Stream_Video;
                default                     :
                        switch (descriptor_tag)
                        {
                            case 0x56 : return Stream_Text;
                            case 0x59 : return Stream_Text;
                            case 0x6A : return Stream_Audio;
                            case 0x7A : return Stream_Audio;
                            case 0x7B : return Stream_Audio;
                            case 0x7C : return Stream_Audio;
                            case 0x81 : return Stream_Audio;
                            default   : return Stream_Max;
                        }
            }
    }
}

//---------------------------------------------------------------------------
extern const float32 Mpegv_frame_rate[]; //In Video/File_Mpegv.cpp
extern const char*  Mpegv_Colorimetry_format[]; //In Video/File_Mpegv.cpp
extern const char*  Mpegv_profile_and_level_indication_profile[]; //In Video/File_Mpegv.cpp
extern const char*  Mpegv_profile_and_level_indication_level[]; //In Video/File_Mpegv.cpp

//---------------------------------------------------------------------------
extern const char*  Mpega_Version[]; //In Audio/File_Mpega.cpp
extern const char*  Mpega_Layer[]; //In Audio/File_Mpega.cpp

//---------------------------------------------------------------------------
extern const int32u AC3_SamplingRate[]; //In Audio/File_Ac3.cpp
extern const int16u AC3_BitRate[]; //In Audio/File_Ac3.cpp
extern const char*  AC3_ChannelPositions[]; //In Audio/File_Ac3.cpp
extern const int8u  AC3_Channels[]; //In Audio/File_Ac3.cpp
extern const char*  AC3_Mode[]; //In Audio/File_Ac3.cpp
extern const char*  AC3_Surround[]; //In Audio/File_Ac3.cpp

const char* Mpeg_Descriptors_AC3_Channels[]=
{
    "1",
    "2",
    "2",
    "2",
    "3+",
    "6+",
    "",
    "",
};

const char* Mpeg_Descriptors_AC3_Priority[]=
{
    "",
    "Primary Audio",
    "Other Audio",
    "",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Read_Buffer_Init()
{
    //In
    format_identifier=0x00000000;
    StreamKind=Stream_Max;
    table_id=0x00;

    //Out
    registration_format_identifier=0x00000000;
    descriptor_tag=0x00;
    CA_PID=0x0000;
    ES_ID=0x0000;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Header_Parse()
{
    if (Element_Offset+2>Element_Size)
        return; //Not enough data
    int8u descriptor_tag, descriptor_length;
    Get_B1 (descriptor_tag,                                     "descriptor_tag");
    Get_B1 (descriptor_length,                                  "descriptor_length");

    //Size
    if (Element_Size<Element_Offset+descriptor_length)
    {
        Element_WaitForMoreData();
        return;
    }
    //Element[Element_Level-1].IsComplete=true;

    //Filling
    Header_Fill_Code(descriptor_tag, Ztring().From_Number(descriptor_tag, 16));
    Header_Fill_Size(2+descriptor_length);
}


//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Data_Parse()
{
    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case 0x##_NAME : Element_Name(_DETAIL); Descriptor_##_NAME(); break;

    //Parsing
         if (table_id> 0x00 && table_id<0x40)
    {
        switch (Element_Code)
        {
            ELEMENT_CASE(00, "Reserved");
            ELEMENT_CASE(01, "Reserved");
            ELEMENT_CASE(02, "video_stream");
            ELEMENT_CASE(03, "audio_stream");
            ELEMENT_CASE(04, "hierarchy");
            ELEMENT_CASE(05, "registration");
            ELEMENT_CASE(06, "data_stream_alignment");
            ELEMENT_CASE(07, "target_background_grid");
            ELEMENT_CASE(08, "Video_window");
            ELEMENT_CASE(09, "CA");
            ELEMENT_CASE(0A, "ISO_639_language");
            ELEMENT_CASE(0B, "System_clock");
            ELEMENT_CASE(0C, "Multiplex_buffer_utilization");
            ELEMENT_CASE(0D, "Copyright");
            ELEMENT_CASE(0E, "Maximum_bitrate");
            ELEMENT_CASE(0F, "Private_data_indicator");
            ELEMENT_CASE(10, "Smoothing_buffer");
            ELEMENT_CASE(11, "STD");
            ELEMENT_CASE(12, "IBP");
            ELEMENT_CASE(13, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(14, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(15, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(16, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(17, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(18, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(19, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(1A, "Defined in ISO/IEC 13818-6");
            ELEMENT_CASE(1B, "MPEG-4_video");
            ELEMENT_CASE(1C, "MPEG-4_audio");
            ELEMENT_CASE(1D, "IOD");
            ELEMENT_CASE(1E, "SL");
            ELEMENT_CASE(1F, "FMC");
            ELEMENT_CASE(20, "External_ES_ID");
            ELEMENT_CASE(21, "MuxCode");
            ELEMENT_CASE(22, "FmxBufferSize");
            ELEMENT_CASE(23, "MultiplexBuffer");
            ELEMENT_CASE(24, "Content_labeling_descriptor");
            ELEMENT_CASE(25, "Metadata_pointer_descriptor");
            ELEMENT_CASE(26, "Metadata_descriptor");
            ELEMENT_CASE(27, "Metadata_STD_descriptor");
            ELEMENT_CASE(28, "AVC video descriptor");
            ELEMENT_CASE(29, "IPMP_descriptor (defined in ISO/IEC 13818-11, MPEG-2 IPMP)");
            ELEMENT_CASE(2A, "AVC timing and HRD descriptor");
            ELEMENT_CASE(2B, "MPEG-2 AAC audio descriptor");
            ELEMENT_CASE(2C, "FlexMux_Timing_descriptor");
            ELEMENT_CASE(2D, "MPEG-4_text_descriptor");
            ELEMENT_CASE(2E, "MPEG-4_audio_extension_descriptor");
            ELEMENT_CASE(2F, "Auxiliary_video_data_descriptor");

            //Following is in private sections, in case there is not network type detected
            ELEMENT_CASE(40, "DVB - network_name_descriptor");
            ELEMENT_CASE(41, "DVB - service_list_descriptor");
            ELEMENT_CASE(42, "DVB - stuffing_descriptor");
            ELEMENT_CASE(43, "DVB - satellite_delivery_system_descriptor");
            ELEMENT_CASE(44, "DVB - cable_delivery_system_descriptor");
            ELEMENT_CASE(45, "DVB - VBI_data_descriptor");
            ELEMENT_CASE(46, "DVB - VBI_teletext_descriptor");
            ELEMENT_CASE(47, "DVB - bouquet_name_descriptor");
            ELEMENT_CASE(48, "DVB - service_descriptor");
            ELEMENT_CASE(49, "DVB - country_availability_descriptor");
            ELEMENT_CASE(4A, "DVB - linkage_descriptor");
            ELEMENT_CASE(4B, "DVB - NVOD_reference_descriptor");
            ELEMENT_CASE(4C, "DVB - time_shifted_service_descriptor");
            ELEMENT_CASE(4D, "DVB - short_event_descriptor");
            ELEMENT_CASE(4E, "DVB - extended_event_descriptor");
            ELEMENT_CASE(4F, "DVB - time_shifted_event_descriptor");
            ELEMENT_CASE(50, "DVB - component_descriptor");
            ELEMENT_CASE(51, "DVB - mosaic_descriptor");
            ELEMENT_CASE(52, "DVB - stream_identifier_descriptor");
            ELEMENT_CASE(53, "DVB - CA_identifier_descriptor");
            ELEMENT_CASE(54, "DVB - content_descriptor");
            ELEMENT_CASE(55, "DVB - parental_rating_descriptor");
            ELEMENT_CASE(56, "DVB - teletext_descriptor");
            ELEMENT_CASE(57, "DVB - telephone_descriptor");
            ELEMENT_CASE(58, "DVB - local_time_offset_descriptor");
            ELEMENT_CASE(59, "DVB - subtitling_descriptor");
            ELEMENT_CASE(5A, "DVB - terrestrial_delivery_system_descriptor");
            ELEMENT_CASE(5B, "DVB - multilingual_network_name_descriptor");
            ELEMENT_CASE(5C, "DVB - multilingual_bouquet_name_descriptor");
            ELEMENT_CASE(5D, "DVB - multilingual_service_name_descriptor");
            ELEMENT_CASE(5E, "DVB - multilingual_component_descriptor");
            ELEMENT_CASE(5F, "DVB - private_data_specifier_descriptor");
            ELEMENT_CASE(60, "DVB - service_move_descriptor");
            ELEMENT_CASE(61, "DVB - short_smoothing_buffer_descriptor");
            ELEMENT_CASE(62, "DVB - frequency_list_descriptor");
            ELEMENT_CASE(63, "DVB - partial_transport_stream_descriptor");
            ELEMENT_CASE(64, "DVB - data_broadcast_descriptor");
            ELEMENT_CASE(65, "DVB - scrambling_descriptor");
            ELEMENT_CASE(66, "DVB - data_broadcast_id_descriptor");
            ELEMENT_CASE(67, "DVB - transport_stream_descriptor");
            ELEMENT_CASE(68, "DVB - DSNG_descriptor");
            ELEMENT_CASE(69, "DVB - PDC_descriptor");
            ELEMENT_CASE(6A, "DVB - AC-3_descriptor");
            ELEMENT_CASE(6B, "DVB - ancillary_data_descriptor");
            ELEMENT_CASE(6C, "DVB - cell_list_descriptor");
            ELEMENT_CASE(6D, "DVB - cell_frequency_link_descriptor");
            ELEMENT_CASE(6E, "DVB - announcement_support_descriptor");
            ELEMENT_CASE(6F, "DVB - application_signalling_descriptor");
            ELEMENT_CASE(70, "DVB - adaptation_field_data_descriptor");
            ELEMENT_CASE(71, "DVB - service_identifier_descriptor");
            ELEMENT_CASE(72, "DVB - service_availability_descriptor");
            ELEMENT_CASE(73, "DVB - default_authority_descriptor");
            ELEMENT_CASE(74, "DVB - related_content_descriptor");
            ELEMENT_CASE(75, "DVB - TVA_id_descriptor");
            ELEMENT_CASE(76, "DVB - content_identifier_descriptor");
            ELEMENT_CASE(77, "DVB - time_slice_fec_identifier_descriptor");
            ELEMENT_CASE(78, "DVB - ECM_repetition_rate_descriptor");
            ELEMENT_CASE(79, "DVB - S2_satellite_delivery_system_descriptor");
            ELEMENT_CASE(7A, "DVB - enhanced_AC-3_descriptor");
            ELEMENT_CASE(7B, "DVB - DTS descriptor");
            ELEMENT_CASE(7C, "DVB - AAC descriptor");
            ELEMENT_CASE(7D, "DVB - reserved for future use");
            ELEMENT_CASE(7E, "DVB - reserved for future use");
            ELEMENT_CASE(7F, "DVB - extension descriptor");
            ELEMENT_CASE(80, "ATSC - stuffing");
            ELEMENT_CASE(81, "ATSC - AC-3 audio");
            ELEMENT_CASE(86, "ATSC - caption service");
            ELEMENT_CASE(87, "ATSC - content advisory");
            ELEMENT_CASE(A0, "ATSC - extended channel name");
            ELEMENT_CASE(A1, "ATSC - service location");
            ELEMENT_CASE(A2, "ATSC - time-shifted service");
            ELEMENT_CASE(A3, "ATSC - component name");
            ELEMENT_CASE(A8, "ATSC - DCC Departing Request");
            ELEMENT_CASE(A9, "ATSC - DCC Arriving Request");
            ELEMENT_CASE(AA, "ATSC - Redistribution Control");
            ELEMENT_CASE(AB, "ATSC - DCC Location Code");
            default: if (Element_Code>=0x40)
                        Element_Info("user private");
                     else
                        Element_Info("unknown");
                     Skip_XX(Element_Size,                          "Data");
                     break;
        }
    }
    else if (table_id>=0x40 && table_id<0x80)
    {
        switch (Element_Code)
        {
            ELEMENT_CASE(40, "DVB - network_name_descriptor");
            ELEMENT_CASE(41, "DVB - service_list_descriptor");
            ELEMENT_CASE(42, "DVB - stuffing_descriptor");
            ELEMENT_CASE(43, "DVB - satellite_delivery_system_descriptor");
            ELEMENT_CASE(44, "DVB - cable_delivery_system_descriptor");
            ELEMENT_CASE(45, "DVB - VBI_data_descriptor");
            ELEMENT_CASE(46, "DVB - VBI_teletext_descriptor");
            ELEMENT_CASE(47, "DVB - bouquet_name_descriptor");
            ELEMENT_CASE(48, "DVB - service_descriptor");
            ELEMENT_CASE(49, "DVB - country_availability_descriptor");
            ELEMENT_CASE(4A, "DVB - linkage_descriptor");
            ELEMENT_CASE(4B, "DVB - NVOD_reference_descriptor");
            ELEMENT_CASE(4C, "DVB - time_shifted_service_descriptor");
            ELEMENT_CASE(4D, "DVB - short_event_descriptor");
            ELEMENT_CASE(4E, "DVB - extended_event_descriptor");
            ELEMENT_CASE(4F, "DVB - time_shifted_event_descriptor");
            ELEMENT_CASE(50, "DVB - component_descriptor");
            ELEMENT_CASE(51, "DVB - mosaic_descriptor");
            ELEMENT_CASE(52, "DVB - stream_identifier_descriptor");
            ELEMENT_CASE(53, "DVB - CA_identifier_descriptor");
            ELEMENT_CASE(54, "DVB - content_descriptor");
            ELEMENT_CASE(55, "DVB - parental_rating_descriptor");
            ELEMENT_CASE(56, "DVB - teletext_descriptor");
            ELEMENT_CASE(57, "DVB - telephone_descriptor");
            ELEMENT_CASE(58, "DVB - local_time_offset_descriptor");
            ELEMENT_CASE(59, "DVB - subtitling_descriptor");
            ELEMENT_CASE(5A, "DVB - terrestrial_delivery_system_descriptor");
            ELEMENT_CASE(5B, "DVB - multilingual_network_name_descriptor");
            ELEMENT_CASE(5C, "DVB - multilingual_bouquet_name_descriptor");
            ELEMENT_CASE(5D, "DVB - multilingual_service_name_descriptor");
            ELEMENT_CASE(5E, "DVB - multilingual_component_descriptor");
            ELEMENT_CASE(5F, "DVB - private_data_specifier_descriptor");
            ELEMENT_CASE(60, "DVB - service_move_descriptor");
            ELEMENT_CASE(61, "DVB - short_smoothing_buffer_descriptor");
            ELEMENT_CASE(62, "DVB - frequency_list_descriptor");
            ELEMENT_CASE(63, "DVB - partial_transport_stream_descriptor");
            ELEMENT_CASE(64, "DVB - data_broadcast_descriptor");
            ELEMENT_CASE(65, "DVB - scrambling_descriptor");
            ELEMENT_CASE(66, "DVB - data_broadcast_id_descriptor");
            ELEMENT_CASE(67, "DVB - transport_stream_descriptor");
            ELEMENT_CASE(68, "DVB - DSNG_descriptor");
            ELEMENT_CASE(69, "DVB - PDC_descriptor");
            ELEMENT_CASE(6A, "DVB - AC-3_descriptor");
            ELEMENT_CASE(6B, "DVB - ancillary_data_descriptor");
            ELEMENT_CASE(6C, "DVB - cell_list_descriptor");
            ELEMENT_CASE(6D, "DVB - cell_frequency_link_descriptor");
            ELEMENT_CASE(6E, "DVB - announcement_support_descriptor");
            ELEMENT_CASE(6F, "DVB - application_signalling_descriptor");
            ELEMENT_CASE(70, "DVB - adaptation_field_data_descriptor");
            ELEMENT_CASE(71, "DVB - service_identifier_descriptor");
            ELEMENT_CASE(72, "DVB - service_availability_descriptor");
            ELEMENT_CASE(73, "DVB - default_authority_descriptor");
            ELEMENT_CASE(74, "DVB - related_content_descriptor");
            ELEMENT_CASE(75, "DVB - TVA_id_descriptor");
            ELEMENT_CASE(76, "DVB - content_identifier_descriptor");
            ELEMENT_CASE(77, "DVB - time_slice_fec_identifier_descriptor");
            ELEMENT_CASE(78, "DVB - ECM_repetition_rate_descriptor");
            ELEMENT_CASE(79, "DVB - S2_satellite_delivery_system_descriptor");
            ELEMENT_CASE(7A, "DVB - enhanced_AC-3_descriptor");
            ELEMENT_CASE(7B, "DVB - DTS descriptor");
            ELEMENT_CASE(7C, "DVB - AAC descriptor");
            ELEMENT_CASE(7D, "DVB - reserved for future use");
            ELEMENT_CASE(7E, "DVB - reserved for future use");
            ELEMENT_CASE(7F, "DVB - extension descriptor");
            default: if (Element_Code>=0x40)
                        Element_Info("user private");
                     else
                        Element_Info("unknown");
                     Skip_XX(Element_Size,                          "Data");
                     break;
        }
    }
    else if ((table_id>=0xC0 && table_id<0xE0))
    {
        switch (Element_Code)
        {
            ELEMENT_CASE(80, "ATSC - stuffing");
            ELEMENT_CASE(81, "ATSC - AC-3 audio");
            ELEMENT_CASE(86, "ATSC - caption service");
            ELEMENT_CASE(87, "ATSC - content advisory");
            ELEMENT_CASE(A0, "ATSC - extended channel name");
            ELEMENT_CASE(A1, "ATSC - service location");
            ELEMENT_CASE(A2, "ATSC - time-shifted service");
            ELEMENT_CASE(A3, "ATSC - component name");
            ELEMENT_CASE(A8, "ATSC - DCC Departing Request");
            ELEMENT_CASE(A9, "ATSC - DCC Arriving Request");
            ELEMENT_CASE(AA, "ATSC - Redistribution Control");
            ELEMENT_CASE(AB, "ATSC - DCC Location Code");
            default: if (Element_Code>=0x40)
                        Element_Info("user private");
                     else
                        Element_Info("unknown");
                     Skip_XX(Element_Size,                          "Data");
                     break;
        }
    }
    else
    {
        switch (Element_Code)
        {
            default: if (Element_Code>=0x40)
                        Element_Info("user private");
                     else
                        Element_Info("unknown");
                     Skip_XX(Element_Size,                          "Data");
                     break;
        }
    }

    //Info about format
    if (Mpeg_Descriptors_stream_Kind((int8u)Element_Code, format_identifier)!=Stream_Max)
        descriptor_tag=(int8u)Element_Code;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_02()
{
    //Parsing
    int8u frame_rate_code;
    bool multiple_frame_rate_flag, MPEG_1_only_flag;
    int8u profile_and_level_indication_profile=4, profile_and_level_indication_level=10, chroma_format=1;
    bool frame_rate_extension_flag=false;
    BS_Begin();
    Get_SB (   multiple_frame_rate_flag,                        "multiple_frame_rate_flag");
    Get_S1 (4, frame_rate_code,                                 "frame_rate_code"); Param_Info(Mpegv_frame_rate[frame_rate_code]);
    Get_SB (   MPEG_1_only_flag,                                "MPEG_1_only_flag");
    Skip_SB(                                                    "constrained_parameter_flag");
    Skip_SB(                                                    "still_picture_flag");
    if (MPEG_1_only_flag==0)
    {
        Skip_SB(                                                "profile_and_level_indication_escape");
        Get_S1 (3, profile_and_level_indication_profile,        "profile_and_level_indication_profile"); Param_Info(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile]);
        Get_S1 (4, profile_and_level_indication_level,          "profile_and_level_indication_level"); Param_Info(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]);
        Get_S1 (2, chroma_format,                               "chroma_format"); Param_Info(Mpegv_Colorimetry_format[chroma_format]);
        Get_SB (   frame_rate_extension_flag,                   "frame_rate_extension_flag");
        Skip_S1(5,                                              "reserved");
    }
    BS_End();

    //Filling
    FILLING_BEGIN();
        if (!multiple_frame_rate_flag && !frame_rate_extension_flag)
            Infos["FrameRate"]=Ztring::ToZtring(Mpegv_frame_rate[frame_rate_code]);
        Infos["Colorimetry"]=Mpegv_Colorimetry_format[chroma_format];
        Infos["Codec_Profile"]=Ztring().From_Local(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile])+_T("@")+Ztring().From_Local(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_03()
{
    //Parsing
    int8u ID, layer;
    bool variable_rate_audio_indicator;
    BS_Begin();
    Skip_SB(                                                    "free_format_flag");
    Get_S1 (1, ID,                                              "ID"); Param_Info(Mpega_Version[2+ID]); //Mpega_Version is with MPEG2.5 hack
    Get_S1 (2, layer,                                           "layer");  Param_Info(Mpega_Layer[layer]);
    Get_SB (   variable_rate_audio_indicator,                   "variable_rate_audio_indicator");
    Skip_S1(3,                                                  "reserved");
    BS_End();

    //Filling
    Infos["BitRate_Mode"]=variable_rate_audio_indicator?_T("VBR"):_T("CBR");
    Infos["Codec"]=Ztring(Mpega_Version[ID])+Ztring(Mpega_Layer[layer]);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_05()
{
    //Parsing
    Get_B4 (registration_format_identifier,                     "format_identifier"); Element_Info(Mpeg_Descriptors_format_identifier(registration_format_identifier)); Param_Info(Mpeg_Descriptors_format_identifier(registration_format_identifier));

    //Filling
    switch (format_identifier)
    {
        case Mpeg_Descriptors::AC_3 : StreamKind=Stream_Audio; Infos["Codec"]=_T("AC3"); break;
        case Mpeg_Descriptors::BSSD : StreamKind=Stream_Audio; Infos["Codec"]=_T("PCM"); break;
        case Mpeg_Descriptors::DTS1 :
        case Mpeg_Descriptors::DTS2 :
        case Mpeg_Descriptors::DTS3 : StreamKind=Stream_Audio; Infos["Codec"]=_T("DTS"); break;
        case Mpeg_Descriptors::VC_1 : StreamKind=Stream_Video; Infos["Codec"]=_T("VC-1"); break;
        case Mpeg_Descriptors::drac : StreamKind=Stream_Video; Infos["Codec"]=_T("Dirac"); break;
        default : ;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_06()
{
    //Parsing
    Info_B1(alignment_type,                                     "alignment_type"); Param_Info(Mpeg_Descriptors_alignment_type(alignment_type));
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_07()
{
    //Parsing
    BS_Begin();
    Skip_S1(14,                                                 "horizontal_size");
    Skip_S1(14,                                                 "vertical_size");
    Skip_S1( 4,                                                 "aspect_ratio_information"); //Same as ISO/IEC 13818-2
    BS_End();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_08()
{
    //Parsing
    BS_Begin();
    Skip_S1(14,                                                 "horizontal_offset");
    Skip_S1(14,                                                 "vertical_offset");
    Skip_S1( 4,                                                 "window_priority");
    BS_End();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_09()
{
    //Parsing
    Skip_B2(                                                    "CA_system_ID");
    BS_Begin();
    Skip_S1( 3,                                                 "reserved");
    Get_S2 (13, CA_PID,                                         "CA_PID");
    BS_End();
    if (Element_Size-Element_Offset>0)
        Skip_XX(Element_Size-Element_Offset,                    "private_data_byte");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_0A()
{
    //Parsing
    Ztring ISO_639_language_code;
    int8u audio_type;
    Get_Local(3, ISO_639_language_code,                         "ISO_639_language_code");
    Get_B1 (audio_type,                                         "audio_type"); Param_Info(Mpeg_Descriptors_audio_type(audio_type));

    //Filling
    Infos["Language"]=ISO_639_language_code;
    if (audio_type)
        Infos["Language_More"]=Mpeg_Descriptors_audio_type(audio_type);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_0E()
{
    //Parsing
    int32u maximum_bitrate;
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Get_S3 (22, maximum_bitrate,                                "maximum_bitrate"); Param_Info(maximum_bitrate*400, " bps");
    BS_End();

    //Filling
    Infos["BitRate_Maximum"]=Ztring::ToZtring(maximum_bitrate*400);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_0F()
{
    //Parsing
    Skip_B4(                                                    "private_data_indicator");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_10()
{
    //Parsing
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Info_S3(22, sb_leak_rate,                                   "sb_leak_rate"); Param_Info(sb_leak_rate*400, " bps");
    Skip_S1( 2,                                                 "reserved");
    Info_S3(22, sb_size,                                        "sb_size"); Param_Info(sb_size, " bytes");
    BS_End();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_11()
{
    //Parsing
    BS_Begin();
    Skip_S1( 7,                                                 "reserved");
    Skip_SB(                                                    "leak_valid_flag");
    BS_End();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_1D()
{
    //Parsing
    Skip_B1(                                                    "Scope_of_IOD_label");
    Skip_B1(                                                    "IOD_label");
    #ifdef MEDIAINFO_MPEG4_YES
        File_Mpeg4_Descriptors MI;
        MI.Parser_DoNotFreeIt=true;
        Open_Buffer_Init(&MI, File_Offset+Buffer_Offset+Element_Size, File_Offset+Buffer_Offset+Element_Offset);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Finalize(&MI);

        //Parser from Descriptor
        if (MI.Parser)
            ES_Elements[MI.ES_ID].Parser=MI.Parser;
    #else
        Skip_XX(Element_Size-Element_Offset,                    "MPEG-4 Descriptor");
    #endif
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_1F()
{
    //Parsing
    while (Element_Offset<Element_Size)
    {
        Get_B2 (ES_ID,                                          "ES_ID");
        if (Element_Offset!=Element_Size)
            Skip_B1(                                            "FlexMuxChannel");
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_28()
{
    //Parsing
    int8u profile_idc, level_idc;
    Get_B1 (profile_idc,                                        "profile_idc"); Param_Info(Avc_profile_idc(profile_idc));
    BS_Begin();
    Element_Begin("constraints");
        Skip_SB(                                                "constraint_set0_flag");
        Skip_SB(                                                "constraint_set1_flag");
        Skip_SB(                                                "constraint_set2_flag");
        Skip_SB(                                                "constraint_set3_flag");
        Skip_SB(                                                "reserved_zero_4bits");
        Skip_SB(                                                "reserved_zero_4bits");
        Skip_SB(                                                "reserved_zero_4bits");
        Skip_SB(                                                "reserved_zero_4bits");
    Element_End();
    BS_End();
    Get_B1 (level_idc,                                          "level_idc");
    BS_Begin();
    Skip_SB(                                                    "AVC_still_present");
    Skip_SB(                                                    "AVC_24_hour_picture_flag");
    Skip_S1(6,                                                  "reserved");
    BS_End();

    //Filling
    if (StreamKind!=Stream_General)
        Infos["Format_Profile"]=Ztring().From_Local(Avc_profile_idc(profile_idc))+_T("@")+Ztring().From_Number(((float)level_idc)/10, 1);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_2F()
{
    //Parsing
    int8u aux_video_params_length;
    Skip_B1(                                                    "aux_video_type"); //ISO/IEC 23002-3
    Get_B1 (aux_video_params_length,                            "aux_video_params_length");
    Skip_XX(aux_video_params_length,                            "aux_video_params");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_48()
{
    //Parsing
    Ztring service_provider_name, service_name;
    int8u service_type, service_provider_name_length, service_name_length;
    Get_B1 (service_type,                                       "service_type"); Param_Info(Mpeg_Descriptors_dvb_service_type(service_type));
    Get_B1 (service_provider_name_length,                       "service_provider_name_length");
    Get_Local (service_provider_name_length, service_provider_name, "service_provider_name");
    Get_B1 (service_name_length,                                "service_name_length");
    Get_Local(service_name_length, service_name,                "service_name");

    //Filling
    FILLING_BEGIN();
        Program.Infos["ServiceType"]=Mpeg_Descriptors_dvb_service_type(service_type);
        Program.Infos["ServiceProvider"]=service_provider_name;
        Program.Infos["ServiceName"]=service_name;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_4A()
{
    //Parsing
    int8u linkage_type;
    Skip_B2(                                                    "transport_stream_id");
    Skip_B2(                                                    "original_network_id");
    Skip_B2(                                                    "service_id");
    Get_B1 (linkage_type,                                       "linkage_type"); Param_Info(Mpeg_Descriptors_linkage_type(linkage_type));
    if (Element_Size>7)
        Skip_XX(Element_Size-7,                                 "private_data");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_4D()
{
    //Parsing
    Ztring event_name;
    int8u event_name_length, text_length;
    Skip_Local(3,                                               "ISO_639_language_code");
    Get_B1 (event_name_length,                                  "event_name_length");
    Get_DVB_Text(event_name_length, event_name,                 "event_name"); Element_Info(event_name);
    Get_B1 (text_length,                                        "text_length");
    Skip_DVB_Text(text_length,                                  "text");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_50()
{
    //Parsing
    Ztring ISO_639_language_code;
    BS_Begin();
    Skip_S1(4,                                                  "reserved_future_use");
    Info_S1(4, stream_content,                                  "stream_content"); Param_Info(Mpeg_Descriptors_stream_content(stream_content)); Element_Info(Mpeg_Descriptors_stream_content(stream_content));
    BS_End();
    Info_B1(component_type,                                     "component_type"); Param_Info(Mpeg_Descriptors_component_type(stream_content, component_type)); Element_Info(Mpeg_Descriptors_component_type(stream_content, component_type));
    Info_B1(component_tag,                                      "component_tag");
    Get_Local(3, ISO_639_language_code,                         "ISO_639_language_code");
    Skip_DVB_Text(Element_Size-Element_Offset,                  "text");

    //Filling
    if (StreamKind!=Stream_General)
        Infos["Language"]=ISO_639_language_code;
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_52()
{
    //Parsing
    Skip_B1(                                                    "component_tag");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_54()
{
    //Parsing
    while (Element_Offset<Element_Size)
    {
        BS_Begin();
        Info_S1(4, content_nibble_level_1,                      "content_nibble_level_1"); Param_Info(Mpeg_Descriptors_content_nibble_level_1(content_nibble_level_1)); Element_Info(Mpeg_Descriptors_content_nibble_level_1(content_nibble_level_1));
        Info_S1(4, content_nibble_level_2,                      "content_nibble_level_2"); Param_Info(Mpeg_Descriptors_content_nibble_level_2(content_nibble_level_1, content_nibble_level_2)); if (content_nibble_level_1==0xB || content_nibble_level_2!=0) Element_Info(Mpeg_Descriptors_content_nibble_level_2(content_nibble_level_1, content_nibble_level_2));
        Skip_S1(4,                                              "user_nibble");
        Skip_S1(4,                                              "user_nibble");
        BS_End();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_55()
{
    //Parsing
    while (Element_Offset<Element_Size)
    {
        Skip_Local(3,                                           "country_code");
        Info_B1(rating,                                         "rating"); Param_Info(rating+3, " years old"); Element_Info(rating+3, " years old");
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_56()
{
    //Parsing
    Ztring ISO_639_language_code;
    while (Element_Offset<Element_Size)
    {
        Get_Local(3, ISO_639_language_code,                         "ISO_639_language_code");
        BS_Begin();
        Info_S1(5, teletext_type,                                   "teletext_type"); Param_Info(Mpeg_Descriptors_teletext_type(teletext_type));
        Skip_S1(3,                                                  "teletext_magazine_number");
        Skip_S1(4,                                                  "teletext_page_number_1");
        Skip_S1(4,                                                  "teletext_page_number_2");
        BS_End();

        //Filling
        Infos["Language"]=ISO_639_language_code;
        //TODO: this stream is teletext. Be careful, multiple stream in a PID
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_59()
{
    //Parsing
    Ztring ISO_639_language_code;
    while (Element_Offset<Element_Size)
    {
        Get_Local(3, ISO_639_language_code,                         "ISO_639_language_code");
        Info_B1(subtitling_type,                                    "subtitling_type"); Param_Info(Mpeg_Descriptors_component_type_O3(subtitling_type));
        Skip_B2(                                                    "composition_page_id");
        Skip_B2(                                                    "ancillary_page_id");

        //Filling
        Infos["Language"]=ISO_639_language_code;
        //TODO: this stream is subtitle. Be careful, multiple stream in a PID
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_5F()
{
    //Parsing
    Info_B4(private_data_specifier,                             "private_data_specifier"); Param_Info(Ztring().From_CC4(private_data_specifier));
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_63()
{
    //Parsing
    int32u peak_rate;
    BS_Begin();
    Skip_S1( 2,                                                 "DVB_reserved_future_use");
    Get_S3 (22, peak_rate,                                      "peak_rate");
    Skip_S1( 2,                                                 "DVB_reserved_future_use");
    Skip_S3(22,                                                 "minimum_overall_smoothing_rate");
    Skip_S1( 2,                                                 "DVB_reserved_future_use");
    Skip_S2(14,                                                 "maximum_overall_smoothing_buffer");
    BS_End();

    //Filling
    Infos["OverallBitRate_Maximum"]=Ztring::ToZtring(peak_rate*400);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_66()
{
    //Parsing
    Ztring ISO_639_language_code;
    int8u selector_length, text_length;
    Skip_B2(                                                    "data_broadcast_id");
    Skip_B1(                                                    "component_tag");
    Get_B1 (selector_length,                                    "selector_length");
    Skip_XX(selector_length,                                    "selector_bytes");
    Get_Local(3, ISO_639_language_code,                         "ISO_639_language_code");
    Get_B1 (text_length,                                        "text_length");
    Skip_Local(text_length,                                     "text_chars");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_6A()
{
    //Parsing
    BS_Begin();
    bool component_type_flag, bsid_flag, mainid_flag, asvc_flag;
    Get_SB (   component_type_flag,                             "component_type_flag");
    Get_SB (   bsid_flag,                                       "bsid_flag");
    Get_SB (   mainid_flag,                                     "mainid_flag");
    Get_SB (   asvc_flag,                                       "asvc_flag");
    Skip_SB(                                                    "reserved_flag");
    Skip_SB(                                                    "reserved_flag");
    Skip_SB(                                                    "reserved_flag");
    Skip_SB(                                                    "reserved_flag");
    BS_End();
    if (component_type_flag)
    {
        int8u service_type, number_of_channels;
        BS_Begin();
        Skip_SB(                                                "enhanced AC-3");
        Skip_SB(                                                "full_service");
        Get_S1 (3, service_type,                                "service_type"); Param_Info(AC3_Mode[service_type]);
        Get_S1 (3, number_of_channels,                          "number_of_channels"); Param_Info(Mpeg_Descriptors_AC3_Channels[number_of_channels], " channels");
        BS_End();
        Infos["Channel(s)"]=Ztring().From_Local(Mpeg_Descriptors_AC3_Channels[number_of_channels]);
    }
    if (bsid_flag)
    {
        BS_Begin();
        Skip_S1(3,                                              "zero");
        Skip_S1(5,                                              "bsid");
        BS_End();
    }
    if (mainid_flag)
    {
        Skip_B1(                                                "mainid");
    }
    if (asvc_flag)
    {
        Skip_B1(                                                "asvc");
    }
    //BS_End_CANBEMORE();

    //Filling
    StreamKind=Stream_Audio;
    Infos["Codec"]=_T("AC3");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_7A()
{
    //Parsing
    bool component_type_flag, bsid_flag, mainid_flag, asvc_flag, mixinfoexists, substream1_flag, substream2_flag, substream3_flag, enhanced_ac3=0;
    BS_Begin();
    Get_SB (  component_type_flag,                             "component_type_flag");
    Get_SB (  bsid_flag,                                       "bsid_flag");
    Get_SB (  mainid_flag,                                     "mainid_flag");
    Get_SB (  asvc_flag,                                       "asvc_flag");
    Get_SB (  mixinfoexists,                                   "mixinfoexists");
    Get_SB (  substream1_flag,                                 "substream1_flag");
    Get_SB (  substream2_flag,                                 "substream2_flag");
    Get_SB (  substream3_flag,                                 "substream3_flag");
    BS_End();
    if (component_type_flag)
    {
        int8u service_type, number_of_channels;
        BS_Begin();
        Get_SB (  enhanced_ac3,                                "enhanced AC-3");
        Skip_SB(                                               "full_service");
        Get_S1 (3, service_type,                               "service_type"); Param_Info(AC3_Mode[service_type]);
        Get_S1 (3, number_of_channels,                         "number_of_channels"); Param_Info(Mpeg_Descriptors_AC3_Channels[number_of_channels], " channels");
        Infos["Channel(s)"]=Ztring().From_Local(Mpeg_Descriptors_AC3_Channels[number_of_channels]);
        BS_End();
    }
    if (bsid_flag)
    {
        BS_Begin();
        Skip_S1(3,                                              "zero");
        Skip_S1(5,                                              "bsid");
        BS_End();
    }
    if (mainid_flag)
    {
        Skip_B1(                                                "mainid");
    }
    if (asvc_flag)
    {
        Skip_B1(                                                "asvc");
    }
    if (substream1_flag)
    {
        Skip_B1(                                                "substream1");
    }
    if (substream2_flag)
    {
        Skip_B1(                                                "substream2");
    }
    if (substream3_flag)
    {
        Skip_B1(                                                "substream3");
    }
    //BS_End_CANBEMORE();

    //Filling
    StreamKind=Stream_Audio;
    Infos["Codec"]=enhanced_ac3?_T("E-AC-3"):_T("AC-3");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_7B()
{
    //Parsing
    BS_Begin();
    Skip_S1(6,                                                  "bit_rate_code");
    Skip_S2(7,                                                  "nblks");
    Skip_S2(14,                                                 "fsize");
    Skip_S1(6,                                                  "surround_mode");
    Skip_SB(                                                    "lfe_flag");
    Skip_S1(2,                                                  "extended_surround_flag");
    BS_End();
    //BS_End_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_7C()
{
    //Parsing
    bool AAC_type_flag;
    Skip_B1(                                                    "Profile_and_level");
    BS_Begin();
    Get_SB (   AAC_type_flag,                                   "AAC_type_flag");
    Skip_SB(                                                    "reserved");
    Skip_SB(                                                    "reserved");
    Skip_SB(                                                    "reserved");
    Skip_SB(                                                    "reserved");
    Skip_SB(                                                    "reserved");
    Skip_SB(                                                    "reserved");
    Skip_SB(                                                    "reserved");
    BS_End();
    if (AAC_type_flag)
    {
        Skip_B1(                                                "AAC_type");
    }
    //BS_End_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_81()
{
    //if (format_identifier!=Mpeg_Descriptors::GA94);
    //    return;

    //Parsing
    Ztring Text, Language1, Language2;
    int8u sample_rate_code, bit_rate_code, surround_mode, bsmod, num_channels, langcod, textlen, text_code;
    bool language_flag, language_flag_2;
    BS_Begin();
    Get_S1 (3, sample_rate_code,                                "sample_rate_code"); if (sample_rate_code<4) {Param_Info(AC3_SamplingRate[sample_rate_code], " Hz");}
    Skip_S1(5,                                                  "bsid");
    Get_S1 (6, bit_rate_code,                                   "bit_rate_code"); Param_Info(AC3_BitRate[bit_rate_code]*1000, " Kbps");
    Get_S1 (2, surround_mode,                                   "surround_mode"); Param_Info(AC3_Surround[surround_mode]);
    Get_S1 (3, bsmod,                                           "bsmod");
    Get_S1 (4, num_channels,                                    "num_channels"); if (num_channels<8) {Param_Info(AC3_Channels[num_channels], " channels");}
    Skip_SB(                                                    "full_svc");
    BS_End();

    //Filling
    if (sample_rate_code<4)
        Infos["SamplingRate"]=Ztring::ToZtring(AC3_SamplingRate[sample_rate_code]);
    Infos["BitRate"]=Ztring::ToZtring(AC3_BitRate[bit_rate_code]*1000);
    if (num_channels<8)
        Infos["Channel(s)"]=Ztring::ToZtring(AC3_Channels[num_channels]);

    //Parsing
    if (Element_Offset==Element_Size) return;
        Get_B1 (langcod,                                        "langcod");

    //Parsing
    if (Element_Offset==Element_Size) return;
    if (num_channels==0)
        Skip_B1(                                                "langcod2");

    //Parsing
    if (Element_Offset==Element_Size) return;
    if (bsmod<2)
    {
        BS_Begin();
        Skip_S1(3,                                              "mainid");
        Info_BS(2, priority,                                    "priority"); Param_Info(Mpeg_Descriptors_AC3_Priority[priority]);
        Skip_S1(3,                                              "reserved");
        BS_End();
    }
    else
        Skip_B1(                                                "asvcflags");

    //Parsing
    if (Element_Offset==Element_Size) return;
    BS_Begin();
    Get_S1 (7, textlen,                                         "textlen");
    Get_S1 (1, text_code,                                       "text_code"); if (text_code) Param_Info("Unicode");
    BS_End();
    if (textlen)
        Get_Local(textlen, Text,                                "text");

    //Parsing
    if (Element_Offset==Element_Size) return;
    BS_Begin();
    Get_SB (   language_flag,                                   "language_flag");
    Get_SB (   language_flag_2,                                 "language_flag_2");
    Skip_S4(6,                                                  "all is 1");
    BS_End();

    //Parsing
    if (Element_Offset==Element_Size) return;
    if (language_flag)
        Get_Local(3, Language1,                                 "language1");

    //Parsing
    if (Element_Offset==Element_Size) return;
    if (language_flag_2)
        Get_Local(3, Language2,                                 "language2");
    //BS_End_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_86()
{
    //Parsing
    Ztring Text, Language1, Language2;
    int8u number_of_services;
    BS_Begin();
    Mark_1();
    Mark_1();
    Mark_1();
    Get_S1 (5, number_of_services,                              "number_of_services");
    BS_End();
    for (int8u Pos=0; Pos<number_of_services; Pos++)
    {
        bool digital_cc;
        Skip_Local(3,                                           "language");
        BS_Begin();
        Get_SB (digital_cc,                                     "digital_cc");
        Skip_SB(                                                "reserved");
        if (digital_cc) //line21
        {
            Skip_S1(5,                                          "reserved");
            Skip_SB(                                            "line21_field");
        }
        else
            Skip_S1(6,                                          "caption_service_number");
        Skip_SB(                                                "easy_reader");
        Skip_SB(                                                "wide_aspect_ratio");
            Skip_S1(5,                                          "reserved");
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        BS_End();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_A0()
{
    //Parsing
    Ztring Value;
    ATSC_multiple_string_structure(Value);

    //Filling
    if (!Value.empty())
        Program.Infos["Title"]=Value;
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_A1()
{
    //Parsing
    int8u number_elements;
    BS_Begin();
    Skip_S1( 3,                                                 "reserved");
    Skip_S2(13,                                                 "PCR_PID");
    BS_End();
    Get_B1 (    number_elements,                                "number_elements");
    for (int8u Pos=0; Pos<number_elements; Pos++)
    {
        Element_Begin();
        int16u elementary_PID;
        Skip_B1(                                                "stream_type");
        BS_Begin();
        Skip_S1( 3,                                             "reserved");
        Get_S2 (13, elementary_PID,                             "elementary_PID");
        BS_End();
        Skip_Local(3,                                           "ISO_639_language_code");
        Element_End(Ztring().From_CC2(elementary_PID), 6);
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_A3()
{
    //Parsing
    Ztring Value;
    ATSC_multiple_string_structure(Value);

    //Filling
    if (!Value.empty())
        Infos["Name"]=Value;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::ATSC_multiple_string_structure(Ztring &Value)
{
    //Parsing
    Ztring string;
    int8u number_strings, number_segments, compression_type, mode, number_bytes;
    Get_B1(number_strings,                                      "number_strings");
    for (int8u Pos=0; Pos<number_strings; Pos++)
    {
        Element_Begin("String");
        Skip_Local(3,                                           "ISO_639_language_code");
        Get_B1(number_segments,                                 "number_segments");
        for (int8u Pos=0; Pos<number_segments; Pos++)
        {
            Element_Begin("Segment");
            Get_B1 (compression_type,                           "compression_type");
            Get_B1 (mode,                                       "mode");
            Get_B1 (number_bytes,                               "number_bytes");
            if (compression_type==0)
                Get_Local(number_bytes, string,                 "string_bytes");
            else
                Skip_XX(number_bytes,                           "compressed_string_bytes");
            Element_End();
        }
        Element_End();
    }

    //Filling
    Value=string;
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Get_DVB_Text(int64u Size, Ztring &Value, const char* Info)
{
    if (Size<1)
    {
        Get_Local(Size, Value,                                  Info);
        return;
    }

    //Testing if there is a codepage
    int8u CodePage1;
    Peek_B1(CodePage1);
    if (CodePage1<0x20)
    {
        Skip_B1(                                                "CodePage"); Param_Info(Mpeg_Descriptors_codepage_1(CodePage1));
        if (CodePage1!=0x10)
        {
            Get_Local(Size-1, Value,                            Info);
        }
        else
        {
            if (Size<3)
            {
                Value.clear();
                return;
            }
            Skip_B2(                                            "CodePage2");
            Get_Local(Size-3, Value,                            Info);
        }

    }
    else
        Get_Local(Size, Value,                                  Info);
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGTS_YES
