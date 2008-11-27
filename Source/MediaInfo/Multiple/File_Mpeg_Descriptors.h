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
//
// Information about MPEG files, Descriptors
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Mpeg_DescriptorsH
#define MediaInfo_Mpeg_DescriptorsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpeg_Descriptors
//***************************************************************************

class File_Mpeg_Descriptors : public File__Analyze
{
public :
    //In
    int32u   format_identifier; //Must be filled by the caller
    stream_t StreamKind;
    int8u    table_id;
    bool     From_ATSC;

    //Out
    int32u   registration_format_identifier;
    std::map<std::string, ZenLib::Ztring> Infos;
    int8u    descriptor_tag;
    int16u   CA_PID;
    int16u   ES_ID;
    struct es_element
    {
        File__Analyze* Parser;

        es_element()
        {
            Parser=NULL;
        }

        ~es_element()
        {
            delete Parser; //Parser=NULL;
        }
    };
    std::map<int16u, es_element> ES_Elements; //Key is ES_ID

    //Out - About Programs
    struct program
    {
        int16u                                      pid;
        int32u                                      format_identifier;
        std::map<std::string, ZenLib::Ztring>       Infos;

        program()
        {
            pid=0;
            format_identifier=0x00000000;
        }

        ~program()
        {
        }
    };
    program Program;


protected :
    //Formats
    void Read_Buffer_Init ();

private :
    //Buffer
    void Header_Parse();
    void Data_Parse();

    //Elements
    void Descriptors();
    void Descriptor();
    void Descriptor_00() {Skip_XX(Element_Size, "Data");};
    void Descriptor_01() {Skip_XX(Element_Size, "Data");};
    void Descriptor_02();
    void Descriptor_03();
    void Descriptor_04() {Skip_XX(Element_Size, "Data");};
    void Descriptor_05();
    void Descriptor_06();
    void Descriptor_07();
    void Descriptor_08();
    void Descriptor_09();
    void Descriptor_0A();
    void Descriptor_0B() {Skip_XX(Element_Size, "Data");};
    void Descriptor_0C() {Skip_XX(Element_Size, "Data");};
    void Descriptor_0D() {Skip_XX(Element_Size, "Data");};
    void Descriptor_0E();
    void Descriptor_0F();
    void Descriptor_10();
    void Descriptor_11();
    void Descriptor_12() {Skip_XX(Element_Size, "Data");};
    void Descriptor_13() {Skip_XX(Element_Size, "Data");};
    void Descriptor_14() {Skip_XX(Element_Size, "Data");};
    void Descriptor_15() {Skip_XX(Element_Size, "Data");};
    void Descriptor_16() {Skip_XX(Element_Size, "Data");};
    void Descriptor_17() {Skip_XX(Element_Size, "Data");};
    void Descriptor_18() {Skip_XX(Element_Size, "Data");};
    void Descriptor_19() {Skip_XX(Element_Size, "Data");};
    void Descriptor_1A() {Skip_XX(Element_Size, "Data");};
    void Descriptor_1B() {Skip_XX(Element_Size, "Data");};
    void Descriptor_1C() {Skip_XX(Element_Size, "Data");};
    void Descriptor_1D();
    void Descriptor_1E() {Skip_XX(Element_Size, "Data");};
    void Descriptor_1F();
    void Descriptor_20() {Skip_XX(Element_Size, "Data");};
    void Descriptor_21() {Skip_XX(Element_Size, "Data");};
    void Descriptor_22() {Skip_XX(Element_Size, "Data");};
    void Descriptor_23() {Skip_XX(Element_Size, "Data");};
    void Descriptor_24() {Skip_XX(Element_Size, "Data");};
    void Descriptor_25() {Skip_XX(Element_Size, "Data");};
    void Descriptor_26() {Skip_XX(Element_Size, "Data");};
    void Descriptor_27() {Skip_XX(Element_Size, "Data");};
    void Descriptor_28();
    void Descriptor_29() {Skip_XX(Element_Size, "Data");};
    void Descriptor_2A() {Skip_XX(Element_Size, "Data");};
    void Descriptor_2B() {Skip_XX(Element_Size, "Data");};
    void Descriptor_2C() {Skip_XX(Element_Size, "Data");};
    void Descriptor_2D() {Skip_XX(Element_Size, "Data");};
    void Descriptor_2E() {Skip_XX(Element_Size, "Data");};
    void Descriptor_2F();
    void Descriptor_40() {Skip_XX(Element_Size, "Data");};
    void Descriptor_41() {Skip_XX(Element_Size, "Data");};
    void Descriptor_42() {Skip_XX(Element_Size, "Data");};
    void Descriptor_43() {Skip_XX(Element_Size, "Data");};
    void Descriptor_44() {Skip_XX(Element_Size, "Data");};
    void Descriptor_45() {Skip_XX(Element_Size, "Data");};
    void Descriptor_46() {Skip_XX(Element_Size, "Data");};
    void Descriptor_47() {Skip_XX(Element_Size, "Data");};
    void Descriptor_48();
    void Descriptor_49() {Skip_XX(Element_Size, "Data");};
    void Descriptor_4A();
    void Descriptor_4B() {Skip_XX(Element_Size, "Data");};
    void Descriptor_4C() {Skip_XX(Element_Size, "Data");};
    void Descriptor_4D();
    void Descriptor_4E() {Skip_XX(Element_Size, "Data");};
    void Descriptor_4F() {Skip_XX(Element_Size, "Data");};
    void Descriptor_50();
    void Descriptor_51() {Skip_XX(Element_Size, "Data");};
    void Descriptor_52();
    void Descriptor_53() {Skip_XX(Element_Size, "Data");};
    void Descriptor_54();
    void Descriptor_55();
    void Descriptor_56();
    void Descriptor_57() {Skip_XX(Element_Size, "Data");};
    void Descriptor_58() {Skip_XX(Element_Size, "Data");};
    void Descriptor_59();
    void Descriptor_5A() {Skip_XX(Element_Size, "Data");};
    void Descriptor_5B() {Skip_XX(Element_Size, "Data");};
    void Descriptor_5C() {Skip_XX(Element_Size, "Data");};
    void Descriptor_5D() {Skip_XX(Element_Size, "Data");};
    void Descriptor_5E() {Skip_XX(Element_Size, "Data");};
    void Descriptor_5F();
    void Descriptor_60() {Skip_XX(Element_Size, "Data");};
    void Descriptor_61() {Skip_XX(Element_Size, "Data");};
    void Descriptor_62() {Skip_XX(Element_Size, "Data");};
    void Descriptor_63();
    void Descriptor_64() {Skip_XX(Element_Size, "Data");};
    void Descriptor_65() {Skip_XX(Element_Size, "Data");};
    void Descriptor_66();
    void Descriptor_67() {Skip_XX(Element_Size, "Data");};
    void Descriptor_68() {Skip_XX(Element_Size, "Data");};
    void Descriptor_69() {Skip_XX(Element_Size, "Data");};
    void Descriptor_6A();
    void Descriptor_6B() {Skip_XX(Element_Size, "Data");};
    void Descriptor_6C() {Skip_XX(Element_Size, "Data");};
    void Descriptor_6D() {Skip_XX(Element_Size, "Data");};
    void Descriptor_6E() {Skip_XX(Element_Size, "Data");};
    void Descriptor_6F() {Skip_XX(Element_Size, "Data");};
    void Descriptor_70() {Skip_XX(Element_Size, "Data");};
    void Descriptor_71() {Skip_XX(Element_Size, "Data");};
    void Descriptor_72() {Skip_XX(Element_Size, "Data");};
    void Descriptor_73() {Skip_XX(Element_Size, "Data");};
    void Descriptor_74() {Skip_XX(Element_Size, "Data");};
    void Descriptor_75() {Skip_XX(Element_Size, "Data");};
    void Descriptor_76() {Skip_XX(Element_Size, "Data");};
    void Descriptor_77() {Skip_XX(Element_Size, "Data");};
    void Descriptor_78() {Skip_XX(Element_Size, "Data");};
    void Descriptor_79() {Skip_XX(Element_Size, "Data");};
    void Descriptor_7A();
    void Descriptor_7B();
    void Descriptor_7C();
    void Descriptor_7D() {Skip_XX(Element_Size, "Data");};
    void Descriptor_7E() {Skip_XX(Element_Size, "Data");};
    void Descriptor_7F() {Skip_XX(Element_Size, "Data");};
    void Descriptor_80() {Skip_XX(Element_Size, "Data");};
    void Descriptor_81();
    void Descriptor_86();
    void Descriptor_87() {Skip_XX(Element_Size, "Data");};
    void Descriptor_A0();
    void Descriptor_A1();
    void Descriptor_A2() {Skip_XX(Element_Size, "Data");};
    void Descriptor_A3();
    void Descriptor_A8() {Skip_XX(Element_Size, "Data");};
    void Descriptor_A9() {Skip_XX(Element_Size, "Data");};
    void Descriptor_AA() {Skip_XX(Element_Size, "Data");};
    void Descriptor_AB() {Skip_XX(Element_Size, "Data");};

    //Helpers
    void ATSC_multiple_string_structure(Ztring &Value);
    void Get_DVB_Text(int64u Size, Ztring &Value, const char* Info);
    void Skip_DVB_Text(int64u Size, const char* Info) {Ztring Temp; Get_DVB_Text(Size, Temp, Info);};
};

//***************************************************************************
// Const
//***************************************************************************

namespace Mpeg_Descriptors
{
    const int32u AC_3=0x41432D33; //Exactly AC-3
    const int32u BSSD=0x42535344; //PCM
    const int32u CUEI=0x43554549; //SCTE
    const int32u DTS1=0x44545331; //DTS
    const int32u DTS2=0x44545332; //DTS
    const int32u DTS3=0x44545333; //DTS
    const int32u GA94=0x47413934; //ATSC - Terrestrial
    const int32u HDMV=0x48444D56; //BluRay
    const int32u S14A=0x53313441; //ATSC - Satellite
    const int32u SCTE=0x53435445; //SCTE
    const int32u VC_1=0x56432D31; //Exactly VC-1
    const int32u drac=0x64726163; //Dirac

    const int32u DVB =0x00000001; //Forced value, does not exist is stream
}

} //NameSpace

#endif
