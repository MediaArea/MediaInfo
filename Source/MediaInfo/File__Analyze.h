// File__Analysze - Base for analyze files
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
#ifndef MediaInfo_File__AnalyzeH
#define MediaInfo_File__AnalyzeH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/File__Analyse_Automatic.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/int128u.h"
#include "ZenLib/ZtringListList.h"
#include <vector>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#ifdef MEDIAINFO_MINIMIZESIZE
    #include "MediaInfo/File__Analyze_MinimizeSize.h"
#else
//***************************************************************************
// Class File__Base
//***************************************************************************

class File__Analyze : public File__Base
{
public :
    //***************************************************************************
    // Constructor/Destructor
    //***************************************************************************

    File__Analyze();
    virtual ~File__Analyze();

    //***************************************************************************
    // Open
    //***************************************************************************

    void    Open_Buffer_Init        (                    int64u File_Size, int64u File_Offset=0);
    void    Open_Buffer_Init        (File__Analyze* Sub);
    void    Open_Buffer_Init        (File__Analyze* Sub, int64u File_Size, int64u File_Offset=0);
    void    Open_Buffer_Continue    (                    const int8u* Buffer, size_t Buffer_Size);
    void    Open_Buffer_Continue    (File__Analyze* Sub, const int8u* Buffer, size_t Buffer_Size);
    void    Open_Buffer_Finalize    (bool NoBufferModification=false);
    void    Open_Buffer_Finalize    (File__Analyze* Sub);

    //***************************************************************************
    // In/Out (for parsers)
    //***************************************************************************

    //In

    //Out
    size_t Frame_Count_InThisBlock;


protected :
    //***************************************************************************
    // Buffer
    //***************************************************************************

    //Buffer
    virtual void Read_Buffer_Init ()          {}; //Temp, should be in File__Base caller
    virtual void Read_Buffer_Continue ()      {}; //Temp, should be in File__Base caller
    virtual void Read_Buffer_Continue_Once () {}; //Temp, should be in File__Base caller
    virtual void Read_Buffer_Unsynched ()       ; //Temp, should be in File__Base caller
    virtual void Read_Buffer_Finalize ()      {}; //Temp, should be in File__Base caller
    bool Buffer_Parse();

    //***************************************************************************
    // BitStream init
    //***************************************************************************

    void BS_Begin();
    void BS_Begin_LE(); //Little Endian version
    void BS_End();
    void BS_End_LE(); //Little Endian version

    //***************************************************************************
    // File Header
    //***************************************************************************

    //File Header - Management
    bool FileHeader_Manage ();

    //File Header - Begin
    virtual bool FileHeader_Begin ()                                            {return true;};

    //File Header - Parse
    virtual void FileHeader_Parse ()                                            {Element_DoNotShow();};

    //***************************************************************************
    // Header
    //***************************************************************************

    //Header - Management
    bool Header_Manage ();

    //Header - Begin
    virtual bool Header_Begin ()                                                {return true;};

    //Header - Parse
    virtual void Header_Parse ()                                                {};

    //Header - Info
    void Header_Fill_Code (int64u Code, const Ztring &Name);
    void Header_Fill_Size (int64u Size);

    //***************************************************************************
    // Data
    //***************************************************************************

    //Header - Management
    bool Data_Manage ();

    //Data - Parse
    virtual void Data_Parse ()                                                  {};

    //Data - Info
    void Data_Info (const Ztring &Parameter);
    inline void Data_Info_From_Milliseconds (int64u Parameter)                  {Data_Info(Ztring().Duration_From_Milliseconds(Parameter));}

    //Data - Get info
    size_t Data_Remain ()                                                       {return (size_t)(Element_Size-(Element_Offset+BS->Offset_Get()));};
    size_t Data_BS_Remain ()                                                    {return (size_t)BS->Remain();};

    //Data - Detect EOF
    virtual void Detect_EOF ()                                                  {};
    bool EOF_AlreadyDetected;

    //Data - Helpers
    void Data_Finnished(const char* Message)                                    {Data_GoTo(File_Size, Message);};
    void Data_GoTo     (int64u GoTo, const char* Message);

    //***************************************************************************
    // Elements
    //***************************************************************************

    //Elements - Begin
    void Element_Begin ();
    void Element_Begin (const Ztring &Name, int64u Size=(int64u)-1);
    inline void Element_Begin (int64u Size) {Element_Begin(Ztring(), Size);}
    inline void Element_Begin (const char *Name, int64u Size=(int64u)-1) {Element_Begin(Ztring().From_UTF8(Name), Size);}

    //Elements - Name
    void Element_Name (const Ztring &Name);
    inline void Element_Name (const char*   Name) {Element_Name(Ztring().From_UTF8(Name));}

    //Elements - Info
    void Element_Info (const Ztring &Parameter);
    inline void Element_Info (const char*   Parameter) {Element_Info(Ztring().From_UTF8(Parameter));}
    inline void Element_Info (const char*   Parameter, const char*   Measure)      {Element_Info(Ztring().From_UTF8(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int8s         Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int8u         Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int16s        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int16u        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int32s        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int32u        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int64s        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int64u        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (int128u       Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    #ifdef NEED_SIZET
    inline void Element_Info (size_t        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    #endif //NEED_SIZET
    inline void Element_Info (float32       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    inline void Element_Info (float64       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    inline void Element_Info_From_Milliseconds (int64u Parameter)                  {Element_Info(Ztring().Duration_From_Milliseconds(Parameter));}

    //Elements - End
    void Element_End ();
    void Element_End (const Ztring &Name, int64u Size=(int64u)-1);
    inline void Element_End (int64u Size) {Element_End(Ztring(), Size);}
    inline void Element_End (const char *Name, int64u Size=(int64u)-1) {Element_End(Ztring().From_UTF8(Name), Size);}

    //Elements - Preparation of element from external app
    void Element_Prepare (int64u Size);

protected :
    //Element - Common
    void   Element_End_Common_Flush();
    void   Element_End_Common_Flush_Details();
    Ztring Element_End_Common_Flush_Build();
public :

    //***************************************************************************
    // Param
    //***************************************************************************

    //TODO: put this in Ztring()
    Ztring ToZtring(const char* Value, size_t Value_Size=Unlimited, bool Utf8=true)
    {
        if (Utf8)
            return Ztring().From_UTF8(Value, Value_Size);
        else
            return Ztring().From_Local(Value, Value_Size);
    }
    #define VALUE(Value) \
        Ztring::ToZtring(Value, 16).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")")

    //Param - Main
    void Param      (const Ztring &Parameter, const Ztring& Value);
    inline void Param      (const char*   Parameter, const Ztring& Value) {Param(Ztring().From_Local(Parameter), Value);};
    inline void Param      (const char*   Parameter, const std::string& Value) {Param(Parameter, Ztring().From_Local(Value.c_str()));}
    inline void Param      (const char*   Parameter, const char*   Value, size_t Value_Size=Unlimited, bool Utf8=true) {Param(Parameter, ToZtring(Value, Value_Size, Utf8));}
    inline void Param      (const char*   Parameter, const int8u*  Value, size_t Value_Size=Unlimited, bool Utf8=true) {Param(Parameter, (const char*)Value, Value_Size, Utf8);}
    inline void Param      (const char*   Parameter, bool   Value) {if (Value) Param(Parameter, "Yes"); else Param(Parameter, "No");}
    inline void Param      (const char*   Parameter, int8u  Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int8s  Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int16u Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int16s Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int32u Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int32s Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int64u Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int64s Value) {Param(Parameter, VALUE(Value));}
    inline void Param      (const char*   Parameter, int128u Value){Param(Parameter, VALUE(Value));}
    inline void Param_UUID (const char*   Parameter, int128u Value){Param(Parameter, Ztring().From_UUID(Value));}
    #ifdef NEED_SIZET
    inline void Param      (const char*   Parameter, size_t Value, intu Radix=16) {Param(Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    #endif //NEED_SIZET
    inline void Param      (const char*   Parameter, float32 Value, intu AfterComma=3) {Param(Parameter, Ztring::ToZtring(Value, AfterComma));}
    inline void Param      (const char*   Parameter, float64 Value, intu AfterComma=3) {Param(Parameter, Ztring::ToZtring(Value, AfterComma));}
    inline void Param      (const char*   Parameter, float80 Value, intu AfterComma=3) {Param(Parameter, Ztring::ToZtring(Value, AfterComma));}
    inline void Param      (const int32u  Parameter, const Ztring& Value) {Param(Ztring().From_CC4(Parameter), Value);};
    inline void Param      (const int16u  Parameter, const Ztring& Value) {Param(Ztring().From_CC2(Parameter), Value);};

    //Param - Info
    void Param_Info (const Ztring &Parameter);
    inline void Param_Info (const char*   Parameter) {Param_Info(Ztring().From_UTF8(Parameter));}
    inline void Param_Info (const char*   Parameter, const char*   Measure)      {Param_Info(Ztring().From_UTF8(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int64u        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int64s        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int32u        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int32s        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int16u        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int16s        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int8u         Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (int8s         Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (float32       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (float64       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    inline void Param_Info (float80       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    #ifdef NEED_SIZET
    inline void Param_Info (size_t        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    #endif //NEED_SIZET
    inline void Param_Info_From_Milliseconds (int64u Parameter)                  {Param_Info(Ztring().Duration_From_Milliseconds(Parameter));}

    //***************************************************************************
    // Information
    //***************************************************************************

    void Info (const Ztring& Value, size_t Element_Level_Minus=0);

    //***************************************************************************
    // Big Endian (Integer, Float, Fixed-Point)
    //***************************************************************************

    void Get_B1   (int8u   &Info, const char* Name);
    void Get_B2   (int16u  &Info, const char* Name);
    void Get_B3   (int32u  &Info, const char* Name);
    void Get_B4   (int32u  &Info, const char* Name);
    void Get_B5   (int64u  &Info, const char* Name);
    void Get_B6   (int64u  &Info, const char* Name);
    void Get_B7   (int64u  &Info, const char* Name);
    void Get_B8   (int64u  &Info, const char* Name);
    void Get_B16  (int128u &Info, const char* Name);
    void Get_BF4  (float32 &Info, const char* Name);
    void Get_BF8  (float64 &Info, const char* Name);
    void Get_BF10 (float80 &Info, const char* Name);
    void Get_BFP4 (size_t Bits, float32 &Info, const char* Name);
    void Peek_B1  (int8u   &Info);
    void Peek_B2  (int16u  &Info);
    void Peek_B3  (int32u  &Info);
    void Peek_B4  (int32u  &Info);
    void Peek_B5  (int64u  &Info);
    void Peek_B6  (int64u  &Info);
    void Peek_B7  (int64u  &Info);
    void Peek_B8  (int64u  &Info);
    void Peek_B16 (int128u &Info);
    void Peek_BF4 (float32 &Info);
    void Peek_BF8 (float64 &Info);
    void Peek_BF10(float64 &Info);
    void Peek_BFP4(size_t Bits, float64 &Info);
    void Skip_B1  (               const char* Name);
    void Skip_B2  (               const char* Name);
    void Skip_B3  (               const char* Name);
    void Skip_B4  (               const char* Name);
    void Skip_B5  (               const char* Name);
    void Skip_B6  (               const char* Name);
    void Skip_B7  (               const char* Name);
    void Skip_B8  (               const char* Name);
    void Skip_B16 (               const char* Name);
    void Skip_BF4 (               const char* Name);
    void Skip_BF8 (               const char* Name);
    void Skip_BF10(               const char* Name);
    void Skip_BFP4(size_t Bits,                const char* Name);
    #define Info_B1(_INFO, _NAME)   int8u   _INFO; Get_B1  (_INFO, _NAME)
    #define Info_B2(_INFO, _NAME)   int16u  _INFO; Get_B2  (_INFO, _NAME)
    #define Info_B3(_INFO, _NAME)   int32u  _INFO; Get_B3  (_INFO, _NAME)
    #define Info_B4(_INFO, _NAME)   int32u  _INFO; Get_B4  (_INFO, _NAME)
    #define Info_B5(_INFO, _NAME)   int64u  _INFO; Get_B5  (_INFO, _NAME)
    #define Info_B6(_INFO, _NAME)   int64u  _INFO; Get_B6  (_INFO, _NAME)
    #define Info_B7(_INFO, _NAME)   int64u  _INFO; Get_B7  (_INFO, _NAME)
    #define Info_B8(_INFO, _NAME)   int64u  _INFO; Get_B8  (_INFO, _NAME)
    #define Info_B16(_INFO, _NAME)  int128u _INFO; Get_B16 (_INFO, _NAME)
    #define Info_BF4(_INFO, _NAME)  float32 _INFO; Get_BF4 (_INFO, _NAME)
    #define Info_BF8(_INFO, _NAME)  float64 _INFO; Get_BF8 (_INFO, _NAME)
    #define Info_BF10(_INFO, _NAME) float80 _INFO; Get_BF10(_INFO, _NAME)
    #define Info_BFP4(_BITS, _INFO, _NAME) float32 _INFO; Get_BFP4(_BITS, _INFO, _NAME)

    //***************************************************************************
    // Little Endian
    //***************************************************************************

    void Get_L1  (int8u   &Info, const char* Name);
    void Get_L2  (int16u  &Info, const char* Name);
    void Get_L3  (int32u  &Info, const char* Name);
    void Get_L4  (int32u  &Info, const char* Name);
    void Get_L5  (int64u  &Info, const char* Name);
    void Get_L6  (int64u  &Info, const char* Name);
    void Get_L7  (int64u  &Info, const char* Name);
    void Get_L8  (int64u  &Info, const char* Name);
    void Get_L16 (int128u &Info, const char* Name);
    void Get_LF4 (float32 &Info, const char* Name);
    void Get_LF8 (float64 &Info, const char* Name);
    void Peek_L1 (int8u   &Info);
    void Peek_L2 (int16u  &Info);
    void Peek_L3 (int32u  &Info);
    void Peek_L4 (int32u  &Info);
    void Peek_L5 (int64u  &Info);
    void Peek_L6 (int64u  &Info);
    void Peek_L7 (int64u  &Info);
    void Peek_L8 (int64u  &Info);
    void Peek_L16(int128u &Info);
    void Peek_LF4(float32 &Info);
    void Peek_LF8(float64 &Info);
    void Skip_L1 (               const char* Name);
    void Skip_L2 (               const char* Name);
    void Skip_L3 (               const char* Name);
    void Skip_L4 (               const char* Name);
    void Skip_L5 (               const char* Name);
    void Skip_L6 (               const char* Name);
    void Skip_L7 (               const char* Name);
    void Skip_L8 (               const char* Name);
    void Skip_LF4(               const char* Name);
    void Skip_LF8(               const char* Name);
    void Skip_L16(               const char* Name);
    #define Info_L1(_INFO, _NAME)  int8u   _INFO; Get_L1 (_INFO, _NAME)
    #define Info_L2(_INFO, _NAME)  int16u  _INFO; Get_L2 (_INFO, _NAME)
    #define Info_L3(_INFO, _NAME)  int32u  _INFO; Get_L3 (_INFO, _NAME)
    #define Info_L4(_INFO, _NAME)  int32u  _INFO; Get_L4 (_INFO, _NAME)
    #define Info_L5(_INFO, _NAME)  int64u  _INFO; Get_L5 (_INFO, _NAME)
    #define Info_L6(_INFO, _NAME)  int64u  _INFO; Get_L6 (_INFO, _NAME)
    #define Info_L7(_INFO, _NAME)  int64u  _INFO; Get_L7 (_INFO, _NAME)
    #define Info_L8(_INFO, _NAME)  int64u  _INFO; Get_L8 (_INFO, _NAME)
    #define Info_L16(_INFO, _NAME) int128u _INFO; Get_L16(_INFO, _NAME)
    #define Info_LF4(_INFO, _NAME) float32 _INFO; Get_LF4(_INFO, _NAME)
    #define Info_LF8(_INFO, _NAME) float64 _INFO; Get_LF8(_INFO, _NAME)

    //***************************************************************************
    // UUID
    //***************************************************************************

    void Get_UUID (int128u &Info, const char* Name);
    void Peek_UUID(int128u &Info);
    void Skip_UUID(               const char* Name);
    #define Info_UUID(_INFO, _NAME) int128u _INFO; Get_UUID(_INFO, _NAME)

    //***************************************************************************
    // EBML
    //***************************************************************************

    void Get_EB (int64u &Info, const char* Name);
    void Get_ES (int64s &Info, const char* Name);
    void Skip_EB(              const char* Name);
    void Skip_ES(              const char* Name);
    #define Info_EB(_INFO, _NAME) int64u _INFO; Get_EB(_INFO, _NAME)
    #define Info_ES(_INFO, _NAME) int64s _INFO; Get_ES(_INFO, _NAME)

    //***************************************************************************
    // Variable Size Value
    //***************************************************************************

    void Get_VS (int64u &Info, const char* Name);
    void Skip_VS(              const char* Name);
    #define Info_VS(_INFO, _NAME) int64u _INFO; Get_VS(_INFO, _NAME)

    //***************************************************************************
    // Exp-Golomb
    //***************************************************************************

    void Get_UE (int32u &Info, const char* Name);
    void Get_SE (int32s &Info, const char* Name);
    void Skip_UE(              const char* Name);
    void Skip_SE(              const char* Name);
    #define Info_UE(_INFO, _NAME) int32u _INFO; Get_UE(_INFO, _NAME)
    #define Info_SE(_INFO, _NAME) int32s _INFO; Get_SE(_INFO, _NAME)

    //***************************************************************************
    // Interleaved Exp-Golomb
    //***************************************************************************

    void Get_UI (int32u &Info, const char* Name);
    void Get_SI (int32s &Info, const char* Name);
    void Skip_UI(              const char* Name);
    void Skip_SI(              const char* Name);
    #define Info_UI(_INFO, _NAME) int32u _INFO; Get_UI(_INFO, _NAME)
    #define Info_SI(_INFO, _NAME) int32s _INFO; Get_SI(_INFO, _NAME)

    //***************************************************************************
    // Variable Length Code
    //***************************************************************************

    void Get_VL (int32u Call(int8u Size, int32u ToCall), int32u &Info, const char* Name);
    void Skip_VL(int32u Call(int8u Size, int32u ToCall),               const char* Name);
    #define Info_VL(_CALL, _INFO, _NAME) int32u _INFO; Get_VL(_CALL, _INFO, _NAME)

    //***************************************************************************
    // Characters
    //***************************************************************************

    void Get_C1 (int8u  &Info, const char* Name);
    void Get_C2 (int16u &Info, const char* Name);
    void Get_C3 (int32u &Info, const char* Name);
    void Get_C4 (int32u &Info, const char* Name);
    void Get_C5 (int64u &Info, const char* Name);
    void Get_C6 (int64u &Info, const char* Name);
    void Get_C7 (int64u &Info, const char* Name);
    void Get_C8 (int64u &Info, const char* Name);
    void Skip_C1(              const char* Name);
    void Skip_C2(              const char* Name);
    void Skip_C3(              const char* Name);
    void Skip_C4(              const char* Name);
    void Skip_C5(              const char* Name);
    void Skip_C6(              const char* Name);
    void Skip_C7(              const char* Name);
    void Skip_C8(              const char* Name);
    #define Info_C1(_INFO, _NAME) int8u  _INFO; Get_C1(_INFO, _NAME)
    #define Info_C2(_INFO, _NAME) int16u _INFO; Get_C2(_INFO, _NAME)
    #define Info_C3(_INFO, _NAME) int32u _INFO; Get_C3(_INFO, _NAME)
    #define Info_C4(_INFO, _NAME) int32u _INFO; Get_C4(_INFO, _NAME)
    #define Info_C5(_INFO, _NAME) int64u _INFO; Get_C5(_INFO, _NAME)
    #define Info_C6(_INFO, _NAME) int64u _INFO; Get_C6(_INFO, _NAME)
    #define Info_C7(_INFO, _NAME) int64u _INFO; Get_C7(_INFO, _NAME)
    #define Info_C8(_INFO, _NAME) int64u _INFO; Get_C8(_INFO, _NAME)

    //***************************************************************************
    // Text
    //***************************************************************************

    void Get_Local  (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_String (int64u Bytes, std::string &Info, const char* Name);
    void Get_UTF8   (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_UTF16  (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_UTF16B (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_UTF16L (int64u Bytes, Ztring      &Info, const char* Name);
    void Peek_Local (int64u Bytes, Ztring      &Info);
    void Peek_String(int64u Bytes, std::string &Info);
    void Skip_Local (int64u Bytes,                    const char* Name);
    void Skip_String(int64u Bytes,                    const char* Name);
    void Skip_UTF8  (int64u Bytes,                    const char* Name);
    void Skip_UTF16B(int64u Bytes,                    const char* Name);
    void Skip_UTF16L(int64u Bytes,                    const char* Name);
    #define Info_Local(_BYTES, _INFO, _NAME)  Ztring _INFO; Get_Local (_BYTES, _INFO, _NAME)
    #define Info_UTF8(_BYTES, _INFO, _NAME)   Ztring _INFO; Get_UTF8  (_BYTES, _INFO, _NAME)
    #define Info_UTF16B(_BYTES, _INFO, _NAME) Ztring _INFO; Get_UTF16B(_BYTES, _INFO, _NAME)
    #define Info_UTF16L(_BYTES, _INFO, _NAME) Ztring _INFO; Get_UTF16L(_BYTES, _INFO, _NAME)

    //***************************************************************************
    // PAscal strings
    //***************************************************************************

    void Get_PA (std::string &Info, const char* Name);
    void Peek_PA(std::string &Info);
    void Skip_PA(                   const char* Name);
    #define Info_PA(_INFO, _NAME) Ztring _INFO; Get_PA (_INFO, _NAME)

    //***************************************************************************
    // Unknown
    //***************************************************************************

    void Skip_XX(int64u Bytes, const char* Name);

    //***************************************************************************
    // Flags
    //***************************************************************************

    void Get_Flags (int64u Flags, size_t Order, bool  &Info, const char* Name);
    void Get_Flags (int64u ValueToPut,          int8u &Info, const char* Name);
    void Skip_Flags(int64u Flags, size_t Order,              const char* Name);
    void Skip_Flags(int64u ValueToPut,                       const char* Name);

    //***************************************************************************
    // BitStream
    //***************************************************************************

    void Get_BS (size_t Bits, int32u  &Info, const char* Name);
    void Get_SB (             bool    &Info, const char* Name);
    void Get_S1 (size_t Bits, int8u   &Info, const char* Name);
    void Get_S2 (size_t Bits, int16u  &Info, const char* Name);
    void Get_S3 (size_t Bits, int32u  &Info, const char* Name);
    void Get_S4 (size_t Bits, int32u  &Info, const char* Name);
    void Get_S5 (size_t Bits, int64u  &Info, const char* Name);
    void Get_S6 (size_t Bits, int64u  &Info, const char* Name);
    void Get_S7 (size_t Bits, int64u  &Info, const char* Name);
    void Get_S8 (size_t Bits, int64u  &Info, const char* Name);
    void Peek_BS(size_t Bits, int32u  &Info);
    void Peek_SB(              bool    &Info);
    void Peek_S1(size_t Bits, int8u   &Info);
    void Peek_S2(size_t Bits, int16u  &Info);
    void Peek_S3(size_t Bits, int32u  &Info);
    void Peek_S4(size_t Bits, int32u  &Info);
    void Peek_S5(size_t Bits, int64u  &Info);
    void Peek_S6(size_t Bits, int64u  &Info);
    void Peek_S7(size_t Bits, int64u  &Info);
    void Peek_S8(size_t Bits, int64u  &Info);
    void Skip_BS(size_t Bits,                const char* Name);
    void Skip_SB(                            const char* Name);
    void Skip_S1(size_t Bits,                const char* Name);
    void Skip_S2(size_t Bits,                const char* Name);
    void Skip_S3(size_t Bits,                const char* Name);
    void Skip_S4(size_t Bits,                const char* Name);
    void Skip_S5(size_t Bits,                const char* Name);
    void Skip_S6(size_t Bits,                const char* Name);
    void Skip_S7(size_t Bits,                const char* Name);
    void Skip_S8(size_t Bits,                const char* Name);
    void Mark_0 ();
    void Mark_1 ();
    #define Info_BS(_BITS, _INFO, _NAME) int32u  _INFO; Get_BS(_BITS, _INFO, _NAME)
    #define Info_SB(_INFO, _NAME)        bool    _INFO; Get_SB(       _INFO, _NAME)
    #define Info_S1(_BITS, _INFO, _NAME) int8u   _INFO; Get_S1(_BITS, _INFO, _NAME)
    #define Info_S2(_BITS, _INFO, _NAME) int16u  _INFO; Get_S2(_BITS, _INFO, _NAME)
    #define Info_S3(_BITS, _INFO, _NAME) int32u  _INFO; Get_S4(_BITS, _INFO, _NAME)
    #define Info_S4(_BITS, _INFO, _NAME) int32u  _INFO; Get_S4(_BITS, _INFO, _NAME)
    #define Info_S5(_BITS, _INFO, _NAME) int64u  _INFO; Get_S5(_BITS, _INFO, _NAME)
    #define Info_S6(_BITS, _INFO, _NAME) int64u  _INFO; Get_S6(_BITS, _INFO, _NAME)
    #define Info_S7(_BITS, _INFO, _NAME) int64u  _INFO; Get_S7(_BITS, _INFO, _NAME)
    #define Info_S8(_BITS, _INFO, _NAME) int64u  _INFO; Get_S8(_BITS, _INFO, _NAME)

    #define TEST_SB_GET(_CODE, _NAME) \
        { \
            Peek_SB(_CODE); \
            if (!_CODE) \
                Skip_SB(                                        _NAME); \
            else \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TEST_SB_SKIP(_NAME) \
        { \
            bool Temp; \
            Peek_SB(Temp); \
            if (!Temp) \
                Skip_SB(                                        _NAME); \
            else \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_GET(_CODE, _NAME) \
        { \
            Peek_SB(_CODE); \
            if (_CODE) \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_SKIP(_NAME) \
        { \
            bool Temp; \
            Peek_SB(Temp); \
            if (Temp) \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_ELSE(_NAME) \
                Element_End(); \
            } \
            else \
            { \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_END() \
            } \
        } \

    #define TEST_SB_END() \
                Element_End(); \
            } \
        } \

    //***************************************************************************
    // Next code planning
    //***************************************************************************

    void NextCode_Add(int64u Code);
    void NextCode_Clear();
    void NextCode_Test();

    //***************************************************************************
    // Element trusting
    //***************************************************************************

    void Trusted_IsNot (const char* Reason);

    //***************************************************************************
    // Stream filling
    //***************************************************************************

    //Before filling the stream, the stream must be prepared
    size_t Stream_Prepare   (stream_t KindOfStream);
    void   General_Fill     (); //Special : pre-fill General with some important information

    //Fill with datas (with parameter as a size_t)
    void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, const Ztring  &Value, bool Replace=false);
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, const std::string &Value, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value.c_str(), Value.size())); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value.c_str(), Value.size()), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, const char*    Value, size_t Value_Size=Unlimited, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value, Value_Size), Replace); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, const wchar_t* Value, size_t Value_Size=Unlimited, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring().From_Unicode(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int8u          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int8s          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int16u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int16s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int32u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int32s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int64u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, int64s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, float32        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, float64        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, float80        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    #ifdef NEED_SIZET
    inline void Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, size_t         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    #endif //NEED_SIZET
    //Fill with datas
    void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const Ztring  &Value, bool Replace=false);
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const std::string &Value, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value.c_str(), Value.size())); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value.c_str(), Value.size()), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const char*    Value, size_t Value_Size=Unlimited, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value, Value_Size), Replace); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const wchar_t* Value, size_t Value_Size=Unlimited, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring().From_Unicode(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int8u          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int8s          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int16u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int16s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int32u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int32s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int64u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int64s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float32        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float64        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float80        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    #ifdef NEED_SIZET
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, size_t         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    #endif //NEED_SIZET
    ZtringListList Fill_Temp;
    void Fill_Flush ();

    const Ztring &Retrieve (stream_t StreamKind, size_t StreamPos, size_t Parameter, info_t KindOfInfo=Info_Text);
    const Ztring &Retrieve (stream_t StreamKind, size_t StreamPos, const char* Parameter, info_t KindOfInfo=Info_Text);

    void Clear (stream_t StreamKind, size_t StreamPos, size_t Parameter);
    void Clear (stream_t StreamKind, size_t StreamPos, const char* Parameter);
    void Clear (stream_t StreamKind, size_t StreamPos);
    void Clear (stream_t StreamKind);
    inline void Clear () {File__Base::Clear();}

    //***************************************************************************
    // Filling
    //***************************************************************************

    //Actions
    void Finnished();
    int64u Element_Code_Get (size_t Level);
    int64u Element_TotalSize_Get (size_t LevelLess=0);
    bool Element_IsComplete_Get ();
    void Element_ThisIsAList ();
    void Element_WaitForMoreData ();
    void Element_DoNotTrust (const char* Reason);
    void Element_DoNotShow ();
    void Element_Show ();
    bool Element_Show_Get ();
    void Element_Show_Add (const Ztring &ToShow);

    //Status
    bool Element_IsOK ();
    bool Element_IsNotFinnished ();
    bool Element_IsWaitingForMoreData ();

    //Begin
    #define FILLING_BEGIN() if (Element_IsOK()) {

    //End
    #define FILLING_END() }

    //***************************************************************************
    // Merging
    //***************************************************************************

    //Utils
public :
    size_t Merge(File__Base &ToAdd, bool Erase=true); //Merge 2 File_Base
    size_t Merge(File__Base &ToAdd, stream_t StreamKind, size_t StreamPos_From, size_t StreamPos_To, bool Erase=true); //Merge 2 streams

    //***************************************************************************
    // Finalize
    //***************************************************************************

    //End
    void Finalize();

protected :

    void Finalize__All      (stream_t StreamKind);
    void Finalize__All      (stream_t StreamKind, size_t Pos);
    void Finalize_General   (size_t Pos);
    void Finalize_Video     (size_t Pos);
    void Finalize_Audio     (size_t Pos);
    void Finalize_Audio_BitRate (size_t Pos, audio Parameter);
    void Finalize_Text      (size_t Pos);
    void Finalize_Chapters  (size_t Pos);
    void Finalize_Image     (size_t Pos);
    void Finalize_Menu      (size_t Pos);
    void Finalize_Tags      ();
    void Finalize_Final     ();
    void Finalize_Final_All (stream_t StreamKind);
    void Finalize_Final_All (stream_t StreamKind, size_t Pos, Ztring &Codec_List, Ztring &Language_List, Ztring &Format_List, Ztring &Format_WithHint_List);

    //Utils - Finalize
    void Duration_Duration123   (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void FileSize_FileSize123   (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void Kilo_Kilo123           (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void Value_Value123         (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void YesNo_YesNo            (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void CodecID_Fill           (const Ztring &Value, stream_t StreamKind, size_t StreamPos, infocodecid_format_t Format);

    //***************************************************************************
    //
    //***************************************************************************

protected :
    //Save for speed improvement
    float Config_Details;
    bool  IsSub;

    //Configuration
    bool DataMustAlwaysBeComplete;  //Data must always be complete, else wait for more data
    bool MustUseAlternativeParser;  //Must use the second parser (example: for Data part)

    //Synchro
    bool MustParseTheHeaderFile;    //There is an header part, must parse it
    bool Synched;                   //Data is synched
    size_t Trusted;
    size_t Trusted_Multiplier;

    //Elements
    size_t Element_Level;           //Current level
    bool   Element_WantNextLevel;   //Want to go to the next leavel instead of the same level

    //Element
    int64u Element_Code;            //Code filled in the file, copy of Element[Element_Level].Code
    int64u Element_Offset;          //Position in the Element (without header)
    int64u Element_Size;            //Size of the Element (without header)

private :
    //***************************************************************************
    // Buffer
    //***************************************************************************

    void Buffer_Clear(); //Clear the buffer
    void Open_Buffer_Continue_Loop();
protected :
    //Buffer
    const int8u* Buffer;
public : //TO CHANGE
    size_t Buffer_Size;
protected :
    int8u* Buffer_Temp;
    size_t Buffer_Temp_Size;
    size_t Buffer_Temp_Size_Max;
    size_t Buffer_Offset; //Temporary usage in this parser
    size_t Buffer_Offset_Temp; //Temporary usage in this parser
    size_t Buffer_MinimumSize;
    size_t Buffer_MaximumSize;
    bool   Buffer_Init_Done;
    friend class File__Tags_Helper;
private :

    //***************************************************************************
    // Elements
    //***************************************************************************

    //Data
    size_t Data_Level;              //Current level for Data ("Top level")

    //Element
    BitStream* BS;                  //For conversion from bytes to bitstream
public : //TO CHANGE
    int64u Header_Size;             //Size of the header of the current element
private :

    //Elements
    size_t Element_Level_Base;      //From other parsers

    struct element_details
    {
        struct to_show
        {
            int64u Pos;             //Position of the element in the file
            int64u Size;            //Size of the element (including header and sub-elements)
            int64u Header_Size;     //Size of the header of the element
            Ztring Name;            //Name planned for this element
            Ztring Info;            //More info about the element
            Ztring Details;         //The main text
            bool   NoShow;          //Don't show this element
        };

        int64u  Code;               //Code filled in the file
        int64u  Next;               //
        bool    WaitForMoreData;    //This element is not complete, we need more data
        bool    UnTrusted;          //This element has a problem
        bool    IsComplete;         //This element is fully buffered, no need of more
        bool    InLoop;             //This element is in a parsing loop
        to_show ToShow;
    };
    std::vector<element_details> Element;

    //NextCode
    std::map<int64u, bool> NextCode;

    //BookMarks
    size_t              BookMark_Element_Level;
    int64u              BookMark_GoTo;
    std::vector<int64u> BookMark_Code;
    std::vector<int64u> BookMark_Next;

public :
    void BookMark_Set(size_t Element_Level_ToGet=(size_t)-1);
    void BookMark_Get();
    virtual bool BookMark_Needed()                                              {return false;};

    //Temp
    bool NewFinnishMethod;
    bool IsFinnished;
    bool ShouldContinueParsing;
};
#endif //MEDIAINFO_MINIMIZESIZE

} //NameSpace



/*
#define BS_END() \
    { \
        BS.Byte_Align(); \
        int32u BS_Value=0x00; \
        while(BS.Remain()>0 && BS_Value==0x00) \
            BS_Value=BS.Get(8); \
        if (BS_Value!=0x00) \
            INTEGRITY_SIZE(BS.Offset_Get()-1, BS.Offset_Get()) \
        else \
            INTEGRITY_SIZE(BS.Offset_Get(), BS.Offset_Get()) \
    } \

#define BS_END_FF() \
    { \
        BS.Byte_Align(); \
        int32u BS_Value=0xFF; \
        while(BS.Remain()>0 && BS_Value==0x00) \
            BS_Value=BS.Get(8); \
        if (BS_Value!=0xFF) \
            INTEGRITY_SIZE(BS.Offset_Get()-1, BS.Offset_Get()) \
        else \
            INTEGRITY_SIZE(BS.Offset_Get(), BS.Offset_Get()) \
    } \

#define BS_END_CANBEMORE() \
    { \
    } \
*/

#define ATOM_BEGIN \
    if (Level!=Element_Level) \
    { \
        Level++; \
        switch (Element_Code_Get(Level)) \
        { \

#define ATOM(_ATOM) \
            case Elements::_ATOM : \
                    if (Level==Element_Level) \
                    { \
                        if (Element_IsComplete_Get()) \
                            _ATOM(); \
                        else \
                        { \
                            Element_WaitForMoreData(); \
                            return; \
                        } \
                    } \
                    break; \

#define ATOM_DEFAULT(_ATOM) \
            default : \
                    if (Level==Element_Level) \
                    { \
                        if (Element_IsComplete_Get()) \
                            _ATOM(); \
                        else \
                        { \
                            Element_WaitForMoreData(); \
                            return; \
                        } \
                    } \
                    break; \

#define ATOM_END \
            default : \
            Skip_XX(Element_TotalSize_Get(), "Unknown"); \
        } \
    } \
    break; \

#define LIST(_ATOM) \
    case Elements::_ATOM : \
            if (Level==Element_Level) \
            { \
                _ATOM(); \
                Element_ThisIsAList(); \
            } \

#define LIST_DEFAULT(_ATOM) \
            default : \
            if (Level==Element_Level) \
            { \
                _ATOM(); \
                Element_ThisIsAList(); \
            } \

#define ATOM_END_DEFAULT \
        } \
    } \
    break; \

#define ATOM_DEFAULT_ALONE(_ATOM) \
    if (Level!=Element_Level) \
    { \
        Level++; \
        if (Level==Element_Level) \
        { \
            if (Element_IsComplete_Get()) \
                _ATOM(); \
            else \
            { \
                Element_WaitForMoreData(); \
                return; \
            } \
        } \
    } \
    break; \

#define LIST_SKIP(_ATOM) \
    case Elements::_ATOM : \
            if (Level==Element_Level) \
            { \
                _ATOM(); \
                Element_ThisIsAList(); \
            } \
            if (Element_TotalSize_Get()>Element_Offset) \
                Skip_XX(Element_TotalSize_Get()-Element_Offset, "Unknown"); \
            break; \


#define DATA_BEGIN \
    size_t Level=0; \
    ATOM_BEGIN \

#define DATA_END \
        default : ; \
            Skip_XX(Element_TotalSize_Get(), "Unknown"); \
    }} \
     \

#endif
