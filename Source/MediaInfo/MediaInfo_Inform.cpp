// File__Base_Inform - Base for other files
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
//
// Inform part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/Utils.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "MediaInfo/MediaInfo_Config.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
String MediaInfo_Internal::Inform()
{
    #ifndef MEDIAINFO_MINIMIZESIZE
        if (MediaInfoLib::Config.Details_Get())
            return Details;
    #endif //MEDIAINFO_MINIMIZESIZE

    if (!(
        MediaInfoLib::Config.Inform_Get(_T("General")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Video")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Audio")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Text")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Chapters")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Image")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Menu")).empty()
     ))
    {
        Ztring Retour;
        Retour+=MediaInfoLib::Config.Inform_Get(_T("File_Begin"));
        Retour+=MediaInfoLib::Config.Inform_Get(_T("General_Begin"));
        Retour+=Inform(Stream_General);
        Retour+=MediaInfoLib::Config.Inform_Get(_T("General_End"));
        if (Count_Get(Stream_Video))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Video_Begin"));
        for (size_t I1=0; I1<Count_Get(Stream_Video); I1++)
        {
            Retour+=Inform(Stream_Video, I1);
            if (I1!=Count_Get(Stream_Video)-1)
                Retour+=MediaInfoLib::Config.Inform_Get(_T("Video_Middle"));
        }
        if (Count_Get(Stream_Video))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Video_End"));
        if (Count_Get(Stream_Audio))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Audio_Begin"));
        for (size_t I1=0; I1<Count_Get(Stream_Audio); I1++)
        {
            Retour+=Inform(Stream_Audio, I1);
            if (I1!=Count_Get(Stream_Audio)-1)
                Retour+=MediaInfoLib::Config.Inform_Get(_T("Audio_Middle"));
        }
        if (Count_Get(Stream_Audio))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Audio_End"));
        if (Count_Get(Stream_Text))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Text_Begin"));
        for (size_t I1=0; I1<Count_Get(Stream_Text); I1++)
        {
            Retour+=Inform(Stream_Text, I1);
            if (I1!=Count_Get(Stream_Text)-1)
                Retour+=MediaInfoLib::Config.Inform_Get(_T("Text_Middle"));
        }
        if (Count_Get(Stream_Text))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Text_End"));
        if (Count_Get(Stream_Chapters))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Chapters_Begin"));
        for (size_t I1=0; I1<Count_Get(Stream_Chapters); I1++)
        {
            Retour+=Inform(Stream_Chapters, I1);
            if (I1!=Count_Get(Stream_Chapters)-1)
                Retour+=MediaInfoLib::Config.Inform_Get(_T("Chapters_Middle"));
        }
        if (Count_Get(Stream_Chapters))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Chapters_End"));
        if (Count_Get(Stream_Image))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Image_Begin"));
        for (size_t I1=0; I1<Count_Get(Stream_Image); I1++)
        {
            Retour+=Inform(Stream_Image, I1);
            if (I1!=Count_Get(Stream_Image)-1)
                Retour+=MediaInfoLib::Config.Inform_Get(_T("Image_Middle"));
        }
        if (Count_Get(Stream_Image))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Image_End"));
        if (Count_Get(Stream_Menu))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Menu_Begin"));
        for (size_t I1=0; I1<Count_Get(Stream_Menu); I1++)
        {
            Retour+=Inform(Stream_Menu, I1);
            if (I1!=Count_Get(Stream_Menu)-1)
                Retour+=MediaInfoLib::Config.Inform_Get(_T("Menu_Middle"));
        }
        if (Count_Get(Stream_Menu))
            Retour+=MediaInfoLib::Config.Inform_Get(_T("Menu_End"));
        Retour+=MediaInfoLib::Config.Inform_Get(_T("File_End"));

        Retour.FindAndReplace(_T("\\r\\n"), _T("\\n"), 0, Ztring_Recursive);
        Retour.FindAndReplace(_T("\\n"), MediaInfoLib::Config.LineSeparator_Get(), 0, Ztring_Recursive);
        //Retour.FindAndReplace(_T("\\n"), MediaInfoLib::Config.LineSeparator_Get(), 0, Ztring_Recursive);
        //Retour.FindAndReplace(_T("\\r"), MediaInfoLib::Config.LineSeparator_Get(), 0, Ztring_Recursive);
        return Retour;
    }

    //Informations
    Ztring Retour;
    bool HTML=false;
    if (MediaInfoLib::Config.Inform_Get()==_T("HTML"))
        HTML=true;

    if (HTML) Retour+=_T("<html>\n\n<head>\n<META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body>\n");

    for (size_t StreamKind=(size_t)Stream_General; StreamKind<Stream_Max; StreamKind++)
    {
        //Pour chaque type de flux
        for (size_t StreamPos=0; StreamPos<(size_t)Count_Get((stream_t)StreamKind); StreamPos++)
        {
            //Pour chaque stream
            if (HTML) Retour+=_T("<table width=\"100%\" border=\"0\" cellpadding=\"1\" cellspacing=\"2\" style=\"border:1px solid Navy\">\n<tr>\n    <td width=\"150\">");
            Ztring A=Get((stream_t)StreamKind, StreamPos, _T("StreamKind/String"));
            Ztring B=Get((stream_t)StreamKind, StreamPos, _T("StreamKindPos"));
            if (!B.empty())
            {
                A+=MediaInfoLib::Config.Language_Get(_T("  Config_Text_NumberTag"));
                A+=B;
            }
            Retour+=A;
            if (HTML) Retour+=_T("</td>\n  </tr>");
            Retour+=MediaInfoLib::Config.LineSeparator_Get();
            Retour+=Inform((stream_t)StreamKind, StreamPos);
            if (HTML) Retour+=_T("</table>\n<br />");
            Retour+=MediaInfoLib::Config.LineSeparator_Get();
        }
    }

    return Retour;
}

//---------------------------------------------------------------------------
String MediaInfo_Internal::Inform (stream_t StreamKind, size_t StreamPos)
{
    //Integrity
    if (StreamKind>=Stream_Max || StreamPos>=Stream[StreamKind].size())
        return _T("");

    if (MediaInfoLib::Config.Inform_Get(_T("General")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Video")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Audio")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Text")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Chapters")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Image")).empty()
     && MediaInfoLib::Config.Inform_Get(_T("Menu")).empty())
    {
        Ztring Retour;
        bool HTML=false;
        if (MediaInfoLib::Config.Inform_Get()==_T("HTML"))
            HTML=true;
        size_t Size=Count_Get(StreamKind, StreamPos);
        for (size_t Champ_Pos=0; Champ_Pos<Size; Champ_Pos++)
        {
            //Pour chaque champ
            //Ztring A=Get((stream_t)4, 2, 0, Info_Measure_Text); // TODO Bug sinon? voir Ztring
            Ztring A=Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Measure_Text); // TODO Bug sinon? voir Ztring
            if ((MediaInfoLib::Config.Complete_Get() || Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]==_T('Y')) && Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text)!=_T(""))
            {
                Ztring Nom=Get((stream_t)StreamKind, 0, Champ_Pos, Info_Name_Text);
                if (Nom==_T(""))
                    Nom=Get((stream_t)StreamKind, 0, Champ_Pos, Info_Name); //Texte n'existe pas
                if (!HTML)
                {
                     int8u Size=MediaInfoLib::Config.Language_Get(_T("  Config_Text_ColumnSize")).To_int8u();
                     if (Size==0)
                        Size=32; //Default
                     Nom.resize(Size, ' ');
                }
                Ztring Valeur=Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text);
                if (HTML)
                {
                    Retour+=_T("  <tr>\n    <td><i>");
                    Retour+=Nom;
                    Retour+=_T(" :</i></td>\n    <td colspan=\"3\">");
                    Retour+=Valeur;
                    Retour+=_T("</td>\n  </tr>");
                }
                else
                    Retour+=Nom + MediaInfoLib::Config.Language_Get(_T("  Config_Text_Separator")) + Valeur;
                Retour+=MediaInfoLib::Config.LineSeparator_Get();
            }
        }

        return Retour;
    }

    Ztring Retour=MediaInfoLib::Config.Inform_Get(Get(StreamKind, 0, _T("StreamKind"), Info_Text));
    ZtringList Info;

    if (StreamKind>=Stream_Max)
        return _T("");
    Info=Stream[StreamKind][StreamPos];

    //Special characters
    Retour.FindAndReplace(_T("\\\\"), _T("|SC1|"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\["), _T("|SC2|"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\]"), _T("|SC3|"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\,"), _T("|SC4|"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\;"), _T("|SC5|"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\("), _T("|SC6|"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\)"), _T("|SC7|"), 0, Ztring_Recursive);

    //Gestion $xx$
    size_t PosX=0;
    while (Retour.find(_T("$"), PosX)!=(size_t)-1)
    {
        PosX=Retour.find(_T("$"), PosX);
        if (Retour.size()>PosX+2 && !(Retour(PosX+1)==_T('i') && Retour(PosX+2)==_T('f') && Retour(PosX+3)==_T('('))) //To keep out "%" without any signification, or "$if(..."
        {
            Ztring ARemplacer=Ztring(_T("$")+Retour.SubString(_T("$"), _T("$"), PosX))+_T("$");
            Ztring RemplacerPar=MediaInfoLib::Config.Language_Get(Retour.SubString(_T("$"), _T("$"), PosX)); //TODO : case sensitive
            Retour.FindAndReplace(ARemplacer, RemplacerPar);
        }
        else
            PosX++;
    }

    //Gestion $if()
    size_t Position=Retour.find(_T("$if("));
    while (Position!=Error && Position>0)
    {
        ZtringList Elements;
        size_t Elements_Index;
        Elements.Separator_Set(0, _T(","));
        Elements.Write(Retour.SubString(_T("$if("), _T(")"), Position));
        Elements(0)=Elements(0).SubString(_T("%"), _T("%"));

        //Test if there is something to replace
        size_t Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Elements(0));
        if (Pos!=std::string::npos)
        {
            if (Info(Pos).size()>0)
                Elements_Index=1;
            else
                Elements_Index=2;
        }
        else
            Elements_Index=2;

        //Replace
        while (Elements(Elements_Index).SubString(_T("%"), _T("%")).size()>0)
        {
            Ztring ToReplace=Elements(Elements_Index).SubString(_T("%"), _T("%"));
            Ztring ReplacedBy=Info(MediaInfoLib::Config.Info_Get(StreamKind).Find(ToReplace));
            ToReplace=Ztring(_T("%"))+ToReplace+Ztring(_T("%"));
            Elements(Elements_Index).FindAndReplace(ToReplace, ReplacedBy);
        }

        Ztring ToReplace=Ztring(_T("$if("))+Retour.SubString(_T("$if("), _T(")"), Position)+_T(")");
        Retour.FindAndReplace(ToReplace, Elements(Elements_Index));
        Position=Retour.find(_T("$if("));
    }

    //Gestion []
    while (Retour.SubString(_T("["), _T("]"))!=_T(""))
    {
        Ztring Crochets=Retour.SubString(_T("["), _T("]"));
        Ztring ValueToFind=Crochets.SubString(_T("%"), _T("%"));
        size_t ValueToFind_Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(ValueToFind);
        Ztring ARemplacer=Ztring(_T("[")+Crochets+_T("]"));
        if (ValueToFind_Pos!=Error)
        {
            Ztring RemplacerPar=Info(ValueToFind_Pos);
            if (RemplacerPar==_T(""))
                Retour.FindAndReplace(ARemplacer, _T(""));
            else
            {
                //Formate l'interieur
                Ztring ATraiter=Crochets;
                Ztring Crochets_ARemplacer=Ztring(_T("%")+ATraiter.SubString(_T("%"), _T("%")))+_T("%");
                Ztring Crochets_RemplacerPar=Info(MediaInfoLib::Config.Info_Get(StreamKind).Find(ATraiter.SubString(_T("%"), _T("%"))));
                ATraiter.FindAndReplace(Crochets_ARemplacer, Crochets_RemplacerPar);
                Retour.FindAndReplace(ARemplacer, ATraiter);
            }
        }
        else
            Retour.FindAndReplace(ARemplacer, _T(""));
    }

    //Gestion %xxx%
    PosX=0;
    while (Retour.find(_T("%"), PosX)!=(size_t)-1)
    {
        PosX=Retour.find(_T("%"), PosX);
        if (Retour.size()>PosX+2 && Retour[PosX+1]>=_T('A') && Retour[PosX+1]<=_T('Z')) //To keep out "%" without any signification
        {
            Ztring ARemplacer=Ztring(_T("%")+Retour.SubString(_T("%"), _T("%"), PosX))+_T("%");
            Ztring RemplacerPar=Get(StreamKind, StreamPos, Retour.SubString(_T("%"), _T("%"), PosX));
            RemplacerPar.FindAndReplace(_T("),"), _T("|SC9|"), 0, Ztring_Recursive);
            RemplacerPar.FindAndReplace(_T(")"), _T("|SC8|"), 0, Ztring_Recursive);
            Retour.FindAndReplace(ARemplacer, RemplacerPar);
        }
        else
            PosX++;
    }

    //Special characters
    Retour.FindAndReplace(_T("|SC1|"), _T("\\"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC2|"), _T("["), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC3|"), _T("]"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC4|"), _T(","), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC5|"), _T(";"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC6|"), _T("("), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC7|"), _T(")"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC8|"), _T(")"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("|SC9|"), _T("),"), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\\r\\n"), MediaInfoLib::Config.LineSeparator_Get(), 0, Ztring_Recursive);
    Retour.FindAndReplace(_T("\r\n"), MediaInfoLib::Config.LineSeparator_Get(), 0, Ztring_Recursive);

    //Retour=_T("<table width=\"100%\" border=\"0\" cellpadding=\"1\" cellspacing=\"2\" style=\"border:1px solid Navy\">\n<tr>\n    <td width=\"150\">Video #0</td>\n  </tr>\r\n  <tr>\n    <td><i>Codec :</i></td>\n    <td colspan=\"3\">WMV1</td>\n  </tr>\r\n  <tr>\n    <td><i>Codec/Info :</i></td>\n    <td colspan=\"3\">Windows Media Video 7</td>\n  </tr>\r\n  <tr>\n    <td><i>Width :</i></td>\n    <td colspan=\"3\">200 pixels</td>\n  </tr>\r\n  <tr>\n    <td><i>Height :</i></td>\n    <td colspan=\"3\">150 pixels</td>\n  </tr>\r\n  <tr>\n    <td><i>Aspect ratio :</i></td>\n    <td colspan=\"3\">4/3</td>\n  </tr>\r\n  <tr>\n    <td><i>Resolution :</i></td>\n    <td colspan=\"3\">24 bits</td>\n  </tr>\r\n</table>\n");
    return Retour;
}

//---------------------------------------------------------------------------
void MediaInfo_Internal::Traiter(Ztring &C)
{
    //$if(%a%,zezeze%a%,rrere)
    size_t Position=C.find(_T("$if("));
    while (Position>0)
    {
        //Recuperation de la chaine entiere
        Ztring Total;
        Ztring ARemplacer;
        ZtringList Total1;
        Total1.Separator_Set(0, _T("),"));
        Total=C.SubString(_T("$if("), _T(")"), Position);
        ARemplacer=Ztring(_T("$if(")+Total+_T(")"));
        Total1.Write(Total);
        if (Total1(0)==_T("")) //mettre champ2
            C.FindAndReplace(ARemplacer, Total1(2), Position);
        else
            C.FindAndReplace(ARemplacer, Total1(1), Position);
        Position=C.find(_T("$if("), Position);
    }

    //reformatage
    C.FindAndReplace(_T("|SC8|"), _T(")"), 0, Ztring_Recursive);
    C.FindAndReplace(_T("|SC9|"), _T("),"), 0, Ztring_Recursive);
    //C.FindAndReplace(_T("\\r\\n"), _T("\n"), 0, Ztring_Recursive);
}

} //NameSpace

