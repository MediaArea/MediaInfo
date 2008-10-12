// File_Jpeg - Info for JPEG files
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
//
// Information about JPEG files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_JpegH
#define MediaInfo_File_JpegH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Jpeg
//***************************************************************************

class File_Jpeg : public File__Analyze
{
private :
    //Buffer
    void FileHeader_Parse ();
    void Header_Parse();
    void Data_Parse();

    //Elements
    void TEM () {};
    void SOF_();
    void S0F0() {SOF_();};
    void S0F1() {SOF_();};
    void S0F2() {SOF_();};
    void S0F3() {SOF_();};
    void DHT () {Skip_XX(Element_Size, "Data");};
    void S0F5() {SOF_();};
    void S0F6() {SOF_();};
    void S0F7() {SOF_();};
    void JPG () {Skip_XX(Element_Size, "Data");};
    void S0F9() {SOF_();};
    void S0FA() {SOF_();};
    void S0FB() {SOF_();};
    void DAC () {Skip_XX(Element_Size, "Data");};
    void S0FD() {SOF_();};
    void S0FE() {SOF_();};
    void S0FF() {SOF_();};
    void RST0() {};
    void RST1() {};
    void RST2() {};
    void RST3() {};
    void RST4() {};
    void RST5() {};
    void RST6() {};
    void RST7() {};
    void SOI () {};
    void EOI () {};
    void SOS ();
    void DQT () {Skip_XX(Element_Size, "Data");};
    void DNL () {Skip_XX(Element_Size, "Data");};
    void DRI () {Skip_XX(Element_Size, "Data");};
    void DHP () {Skip_XX(Element_Size, "Data");};
    void EXP () {Skip_XX(Element_Size, "Data");};
    void APP0();
    void APP0_AVI1();
    void APP0_JFIF();
    void APP0_JFXX();
    void APP0_JFXX_JPEG();
    void APP0_JFXX_1B();
    void APP0_JFXX_3B();
    void APP1();
    void APP1_EXIF();
    void APP2() {Skip_XX(Element_Size, "Data");};
    void APP3() {Skip_XX(Element_Size, "Data");};
    void APP4() {Skip_XX(Element_Size, "Data");};
    void APP5() {Skip_XX(Element_Size, "Data");};
    void APP6() {Skip_XX(Element_Size, "Data");};
    void APP7() {Skip_XX(Element_Size, "Data");};
    void APP8() {Skip_XX(Element_Size, "Data");};
    void APP9() {Skip_XX(Element_Size, "Data");};
    void APPA() {Skip_XX(Element_Size, "Data");};
    void APPB() {Skip_XX(Element_Size, "Data");};
    void APPC() {Skip_XX(Element_Size, "Data");};
    void APPD() {Skip_XX(Element_Size, "Data");};
    void APPE() {Skip_XX(Element_Size, "Data");};
    void APPF() {Skip_XX(Element_Size, "Data");};
    void JPG0() {Skip_XX(Element_Size, "Data");};
    void JPG1() {Skip_XX(Element_Size, "Data");};
    void JPG2() {Skip_XX(Element_Size, "Data");};
    void JPG3() {Skip_XX(Element_Size, "Data");};
    void JPG4() {Skip_XX(Element_Size, "Data");};
    void JPG5() {Skip_XX(Element_Size, "Data");};
    void JPG6() {Skip_XX(Element_Size, "Data");};
    void JPG7() {Skip_XX(Element_Size, "Data");};
    void JPG8() {Skip_XX(Element_Size, "Data");};
    void JPG9() {Skip_XX(Element_Size, "Data");};
    void JPGA() {Skip_XX(Element_Size, "Data");};
    void JPGB() {Skip_XX(Element_Size, "Data");};
    void JPGC() {Skip_XX(Element_Size, "Data");};
    void JPGD() {Skip_XX(Element_Size, "Data");};
    void COM () {Skip_XX(Element_Size, "Data");};
};

} //NameSpace

#endif
