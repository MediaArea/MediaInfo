// File_Id3v2 - Info for ID3v2 tagged files
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
// Information about ID3v2 tagged files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_Id3v2H
#define MediaInfo_File_Id3v2H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

///***************************************************************************
// Class File_Id3v2
//***************************************************************************

class File_Id3v2 : public File__Analyze
{
protected :
    //Format
    void Read_Buffer_Finalize ();

public :
    File_Id3v2();

private :
    //Buffer
    void FileHeader_Parse ();
    void Header_Parse();
    void Data_Parse();

    Ztring Element_Value;
    ZtringList Element_Values;

protected :
    int64u Id3v2_Size;

private :
    int8u  Id3v2_Version;
    bool   Unsynchronisation_Global;
    bool   Unsynchronisation_Frame;

    //Elements
    void Fill_Name();

    //From elsewhere
    void Id3v2();

    #ifdef TEXT
        #undef TEXT
    #endif
    void ____();
    void T___();
    void T__X();
    void W___();
    void W__X();
    void AENC()   {Skip_XX(Element_Size, "Data");};
    void APIC();
    void ASPI()   {Skip_XX(Element_Size, "Data");};
    void COMM();
    void COMR()   {Skip_XX(Element_Size, "Data");};
    void ENCR()   {Skip_XX(Element_Size, "Data");};
    void EQU2()   {Skip_XX(Element_Size, "Data");};
    void EQUA()   {Skip_XX(Element_Size, "Data");};
    void ETCO()   {Skip_XX(Element_Size, "Data");};
    void GEOB()   {Skip_XX(Element_Size, "Data");};
    void GRID()   {Skip_XX(Element_Size, "Data");};
    void IPLS()   {Skip_XX(Element_Size, "Data");};
    void LINK()   {Skip_XX(Element_Size, "Data");};
    void MCDI()   {T___();}
    void MLLT()   {Skip_XX(Element_Size, "Data");};
    void OWNE()   {Skip_XX(Element_Size, "Data");};
    void PCNT()   {Skip_XX(Element_Size, "Data");};
    void POPM()   {Skip_XX(Element_Size, "Data");};
    void POSS()   {Skip_XX(Element_Size, "Data");};
    void PRIV();
    void RBUF()   {Skip_XX(Element_Size, "Data");};
    void RGAD();
    void RVA2()   {Skip_XX(Element_Size, "Data");};
    void RVRB()   {Skip_XX(Element_Size, "Data");};
    void SEEK()   {Skip_XX(Element_Size, "Data");};
    void SIGN()   {Skip_XX(Element_Size, "Data");};
    void SYLT();
    void SYTC()   {Skip_XX(Element_Size, "Data");};
    void TALB()   {T___();}
    void TBPM()   {T___();}
    void TCMP()   {Skip_XX(Element_Size, "Data");};
    void TCOM()   {T___();}
    void TCON()   {T___();}
    void TCOP()   {T___();}
    void TDAT()   {T___();}
    void TDEN()   {T___();}
    void TDLY()   {T___();}
    void TDOR()   {T___();}
    void TDRC()   {T___();}
    void TDRL()   {T___();}
    void TDTG()   {T___();}
    void TENC()   {T___();}
    void TEXT()   {T___();}
    void TFLT()   {T___();}
    void TIME()   {T___();}
    void TIPL()   {T___();}
    void TIT1()   {T___();}
    void TIT2()   {T___();}
    void TIT3()   {T___();}
    void TKEY()   {T___();}
    void TLAN()   {T___();}
    void TLEN()   {T___();}
    void TMCL()   {T___();}
    void TMED()   {T___();}
    void TMOO()   {T___();}
    void TOAL()   {T___();}
    void TOFN()   {T___();}
    void TOLY()   {T___();}
    void TOPE()   {T___();}
    void TORY()   {T___();}
    void TOWN()   {T___();}
    void TPE1()   {T___();}
    void TPE2()   {T___();}
    void TPE3()   {T___();}
    void TPE4()   {T___();}
    void TPOS()   {T___();}
    void TPRO()   {T___();}
    void TPUB()   {T___();}
    void TRCK()   {T___();}
    void TRDA()   {T___();}
    void TRSN()   {T___();}
    void TRSO()   {T___();}
    void TSIZ()   {T___();}
    void TSO2()   {T___();}
    void TSOA()   {T___();}
    void TSOC()   {T___();}
    void TSOP()   {T___();}
    void TSOT()   {T___();}
    void TSRC()   {T___();}
    void TSSE()   {T___();}
    void TSST()   {T___();}
    void TXXX();
    void TYER()   {T___();}
    void UFID()   {Skip_XX(Element_Size, "Data");};
    void USER()   {Skip_XX(Element_Size, "Data");};
    void USLT();
    void WCOM()   {W___();}
    void WCOP()   {W___();}
    void WOAF()   {W___();}
    void WOAR()   {W___();}
    void WOAS()   {W___();}
    void WORS()   {W___();}
    void WPAY()   {W___();}
    void WPUB()   {W___();}
    void WXXX();
    void BUF()    {Skip_XX(Element_Size, "Data");};
    void CNT()    {Skip_XX(Element_Size, "Data");};
    void COM()    {COMM();};
    void CRA()    {Skip_XX(Element_Size, "Data");};
    void CRM()    {Skip_XX(Element_Size, "Data");};
    void EQU()    {Skip_XX(Element_Size, "Data");};
    void ETC()    {Skip_XX(Element_Size, "Data");};
    void GEO()    {Skip_XX(Element_Size, "Data");};
    void IPL()    {Skip_XX(Element_Size, "Data");};
    void LNK()    {Skip_XX(Element_Size, "Data");};
    void MCI()    {Skip_XX(Element_Size, "Data");};
    void MLL()    {Skip_XX(Element_Size, "Data");};
    void PIC_()   {APIC();};
    void POP()    {Skip_XX(Element_Size, "Data");};
    void REV()    {Skip_XX(Element_Size, "Data");};
    void RVA()    {Skip_XX(Element_Size, "Data");};
    void SLT()    {Skip_XX(Element_Size, "Data");};
    void STC()    {Skip_XX(Element_Size, "Data");};
    void TAL()    {T___();}
    void TBP()    {T___();}
    void TCM()    {T___();}
    void TCO()    {T___();}
    void TCP()    {Skip_XX(Element_Size, "Data");};
    void TCR()    {T___();}
    void TDA()    {T___();}
    void TDY()    {T___();}
    void TEN()    {T___();}
    void TFT()    {T___();}
    void TIM()    {T___();}
    void TKE()    {T___();}
    void TLA()    {T___();}
    void TLE()    {T___();}
    void TMT()    {T___();}
    void TOA()    {T___();}
    void TOF()    {T___();}
    void TOL()    {T___();}
    void TOR()    {T___();}
    void TOT()    {T___();}
    void TP1()    {T___();}
    void TP2()    {T___();}
    void TP3()    {T___();}
    void TP4()    {T___();}
    void TPA()    {T___();}
    void TPB()    {T___();}
    void TRC()    {T___();}
    void TRD()    {T___();}
    void TRK()    {T___();}
    void TSI()    {T___();}
    void TSS()    {T___();}
    void TT1()    {T___();}
    void TT2()    {T___();}
    void TT3()    {T___();}
    void TXT()    {T___();}
    void TXX()    {TXXX();};
    void TYE()    {T___();}
    void UFI()    {Skip_XX(Element_Size, "Data");};
    void ULT()    {Skip_XX(Element_Size, "Data");};
    void WAF()    {W___();}
    void WAR()    {W___();};
    void WAS()    {W___();};
    void WCM()    {W___();};
    void WCP()    {W___();};
    void WPB()    {W___();};
    void WXX()    {WXXX();};
    void XRVA()   {RVA2();}

    //Temp
    Ztring Year, Month, Day, Hour, Minute;
    stream_t StreamKind;

    //Helpers
    void Normalize_Date (Ztring& Date);
public :
    inline static size_t SynchSafeInt (const int8u* B) {return ((B[0]<<21)
                                                              | (B[1]<<14)
                                                              | (B[2]<< 7)
                                                              | (B[3]<< 0));}
};

//***************************************************************************
// Const
//***************************************************************************

namespace Id3
{
    const int32u AENC=0x41454E47;
    const int32u APIC=0x41504943;
    const int32u ASPI=0x41535049;
    const int32u COMM=0x434F4D4D;
    const int32u COMR=0x434F4D52;
    const int32u ENCR=0x454E4352;
    const int32u EQU2=0x45515532;
    const int32u EQUA=0x45515541;
    const int32u ETCO=0x4554434F;
    const int32u GEOB=0x47454F42;
    const int32u GRID=0x47524944;
    const int32u IPLS=0x49504C53;
    const int32u LINK=0x4C494E4B;
    const int32u MCDI=0x4D434449;
    const int32u MLLT=0x4D4C4C54;
    const int32u OWNE=0x4F574E45;
    const int32u PCNT=0x50434E58;
    const int32u POPM=0x504F504D;
    const int32u POSS=0x504F5353;
    const int32u PRIV=0x50524956;
    const int32u RBUF=0x52425546;
    const int32u RGAD=0x52474144;
    const int32u RVA2=0x52564132;
    const int32u RVRB=0x52565242;
    const int32u SEEK=0x5345454B;
    const int32u SIGN=0x5349474E;
    const int32u SYLT=0x53594C54;
    const int32u SYTC=0x53595443;
    const int32u TALB=0x54414C42;
    const int32u TBPM=0x5442504D;
    const int32u TCMP=0x54434D50;
    const int32u TCOM=0x54434F4D;
    const int32u TCON=0x54434F4E;
    const int32u TCOP=0x54434F50;
    const int32u TDAT=0x54444154;
    const int32u TDEN=0x5444454E;
    const int32u TDLY=0x54444C59;
    const int32u TDOR=0x54444F52;
    const int32u TDRC=0x54445243;
    const int32u TDRL=0x5444524C;
    const int32u TDTG=0x54445447;
    const int32u TENC=0x54454E43;
    const int32u TEXT=0x54455854;
    const int32u TFLT=0x54464C54;
    const int32u TIME=0x54494D45;
    const int32u TIPL=0x5449504C;
    const int32u TIT1=0x54495431;
    const int32u TIT2=0x54495432;
    const int32u TIT3=0x54495433;
    const int32u TKEY=0x544B4559;
    const int32u TLAN=0x544C414E;
    const int32u TLEN=0x544C454E;
    const int32u TMCL=0x544D434C;
    const int32u TMED=0x544D4544;
    const int32u TMOO=0x544D4F4F;
    const int32u TOAL=0x544F414C;
    const int32u TOFN=0x544F464E;
    const int32u TOLY=0x544F4C59;
    const int32u TOPE=0x544F5045;
    const int32u TORY=0x544F5259;
    const int32u TOWN=0x544F574E;
    const int32u TPE1=0x54504531;
    const int32u TPE2=0x54504532;
    const int32u TPE3=0x54504533;
    const int32u TPE4=0x54504534;
    const int32u TPOS=0x54504F53;
    const int32u TPRO=0x5450524F;
    const int32u TPUB=0x54505542;
    const int32u TRCK=0x5452434B;
    const int32u TRDA=0x54524441;
    const int32u TRSN=0x5452534E;
    const int32u TRSO=0x5452534F;
    const int32u TSIZ=0x5453495A;
    const int32u TSO2=0x54534F32;
    const int32u TSOA=0x54534F41;
    const int32u TSOC=0x54534F43;
    const int32u TSOP=0x54534F50;
    const int32u TSOT=0x54534F54;
    const int32u TSRC=0x54535243;
    const int32u TSSE=0x54535345;
    const int32u TSST=0x54535354;
    const int32u TXXX=0x54585858;
    const int32u TYER=0x54594552;
    const int32u UFID=0x55464944;
    const int32u USER=0x55534552;
    const int32u USLT=0x55534C54;
    const int32u WCOM=0x57434F4D;
    const int32u WCOP=0x57434F50;
    const int32u WOAF=0x574F4146;
    const int32u WOAR=0x574F4152;
    const int32u WOAS=0x574F4153;
    const int32u WORS=0x574F5253;
    const int32u WPAY=0x57504159;
    const int32u WPUB=0x57505542;
    const int32u WXXX=0x57585858;
    const int32u XRVA=0x58525641;
    const int32u BUF=0x425546;
    const int32u CNT=0x434E56;
    const int32u COM=0x434F4D;
    const int32u CRA=0x435241;
    const int32u CRM=0x43524D;
    const int32u EQU=0x455155;
    const int32u ETC=0x455443;
    const int32u GEO=0x47454F;
    const int32u IPL=0x49504C;
    const int32u LNK=0x4C4E4B;
    const int32u MCI=0x4D4349;
    const int32u MLL=0x4D4C4C;
    const int32u PIC_=0x504943; //PIC is used by shared libs in GCC
    const int32u POP=0x504F50;
    const int32u REV=0x524556;
    const int32u RVA=0x525641;
    const int32u SLT=0x534C54;
    const int32u STC=0x535443;
    const int32u TAL=0x54414C;
    const int32u TBP=0x544250;
    const int32u TCM=0x54434D;
    const int32u TCO=0x54434F;
    const int32u TCP=0x544350;
    const int32u TCR=0x544352;
    const int32u TDA=0x544441;
    const int32u TDY=0x544459;
    const int32u TEN=0x54454E;
    const int32u TFT=0x544654;
    const int32u TIM=0x54494D;
    const int32u TKE=0x544B45;
    const int32u TLA=0x544C41;
    const int32u TLE=0x544C45;
    const int32u TMT=0x544D54;
    const int32u TOA=0x544F41;
    const int32u TOF=0x544F46;
    const int32u TOL=0x544F4C;
    const int32u TOR=0x544F52;
    const int32u TOT=0x544F54;
    const int32u TP1=0x545031;
    const int32u TP2=0x545032;
    const int32u TP3=0x545033;
    const int32u TP4=0x545034;
    const int32u TPA=0x545041;
    const int32u TPB=0x545042;
    const int32u TRC=0x545243;
    const int32u TRD=0x545244;
    const int32u TRK=0x54524B;
    const int32u TSI=0x545349;
    const int32u TSS=0x545353;
    const int32u TT1=0x545431;
    const int32u TT2=0x545432;
    const int32u TT3=0x545433;
    const int32u TXT=0x545854;
    const int32u TXX=0x545858;
    const int32u TYE=0x545945;
    const int32u UFI=0x554649;
    const int32u ULT=0x554C54;
    const int32u WAF=0x574146;
    const int32u WAR=0x574152;
    const int32u WAS=0x574153;
    const int32u WCM=0x57434D;
    const int32u WCP=0x574350;
    const int32u WPB=0x575042;
    const int32u WXX=0x575858;
}

} //NameSpace

#endif
