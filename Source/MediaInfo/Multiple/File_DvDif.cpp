// File_DvDif - Info for DV-DIF files
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_DVDIF_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_DvDif.h"
#if defined(MEDIAINFO_EIA608_YES)
    #include "MediaInfo/Text/File_Eia608.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char*  Dv_sct[]=
{
    "Header",
    "Subcode",
    "VAUX",
    "Audio",
    "Video",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Dv_Ssyb_Pc0(int8u Pc0)
{
    switch (Pc0)
    {
        case 0x13 : return "Timecode";
        case 0x14 : return "Binary group";
        case 0x50 :
        case 0x60 : return "Source";
        case 0x51 :
        case 0x61 : return "Source control";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char*  Dv_Disp[]=
{
    "4/3",
    "",
    "16/9",
    "Letterbox",
    "",
    "",
    "",
    "16/9",
};

//---------------------------------------------------------------------------
const int32u  Dv_Audio_SamplingRate[]=
{
    48000,
    44100,
    32000,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
const char*  Dv_StereoMode[]=
{
    "Multi-Stero",
    "Lumped",
};

//---------------------------------------------------------------------------
const int32u  Dv_Audio_Resolution[]=
{
    16,
    12,
    0,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
const int8u  Dv_ChannelsPerBlock[]=
{
    1,
    2,
    0,
    0,
};

//---------------------------------------------------------------------------
const char*  Dv_Pair[]=
{
    "One pair of channels",
    "Independant channels",
};

//---------------------------------------------------------------------------
const char*  Dv_CopyGenerationManagementSystem[]=
{
    "Unrestricted",
    "Not used",
    "One generation only",
    "No copy",
};

//---------------------------------------------------------------------------
const char*  Dv_InputType[]=
{
    "Analog",
    "Digital",
    "Reserved",
    "No information",
};

//---------------------------------------------------------------------------
const char*  Dv_CompressionTimes[]=
{
    "Once",
    "Twice",
    "Three+",
    "No information",
};

//---------------------------------------------------------------------------
const char*  Dv_Emphasis[]=
{
    "Enphasis off",
    "Enphasis on",
    "Reserved",
    "Reserved",
};

//---------------------------------------------------------------------------
const char*  Dv_consumer_camera_1_ae_mode[]=
{
    "full automatic",
    "gain priority mode",
    "shutter priority mode",
    "iris priority mode",
    "manual",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "", //no info
};

//---------------------------------------------------------------------------
const char*  Dv_consumer_camera_1_wb_mode[]=
{
    "automatic",
    "hold",
    "one push",
    "pre-set",
    "",
    "",
    "",
    "", //no info
};

//---------------------------------------------------------------------------
const char* Dv_consumer_camera_1_white_balance(int8u white_balance)
{
    switch (white_balance)
    {
        case 0x00 : return "candle";
        case 0x01 : return "incandescent lamp";
        case 0x02 : return "low color temperature; florescent lamp";
        case 0x03 : return "high color temperature; florescent lamp";
        case 0x04 : return "sunlight";
        case 0x05 : return "cloudy weather";
        case 0x1F : return ""; //No info
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char*  Dv_consumer_camera_1_fcm[]=
{
    "auto focus",
    "manual focus",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_DvDif::File_DvDif()
:File__Analyze()
{
    //In
    Frame_Count_Valid=14;
    AuxToAnalyze=0x00; //No Aux to analyze
    IgnoreAudio=false;

    //Temp
    FrameCount=0;
    FrameSize_Theory=0;
    Duration=0;
    TimeCode_First=(int64u)-1;
    SCT_Old=4; //Video
    DBN_Olds[1]=1; //SubCode
    DBN_Olds[2]=2; //Vaux
    DBN_Olds[3]=8; //Audio
    DBN_Olds[4]=134; //Video
    consumer_camera_1_Parsed=false;
    consumer_camera_2_Parsed=false;
    DSF_IsValid=false;
    APT=0xFF; //Impossible
    TF1=false; //Valid by default, for direct analyze
    TF2=false; //Valid by default, for direct analyze
    TF3=false; //Valid by default, for direct analyze

    #ifdef MEDIAINFO_DVDIF_ANALYZE_YES
    Speed_FrameCount=0;
    Speed_FrameCount_Video_STA_Errors=0;
    Speed_FrameCount_Audio_Errors.resize(8);
    Speed_FrameCount_Timecode_Incoherency=0;
    Speed_FrameCount_Contains_NULL=0;
    Speed_Contains_NULL=0;
    System_IsValid=false;
    Frame_AtLeast1DIF=false;
    QU=(int8u)-1;
    CH_IsPresent.resize(8);
    #endif //MEDIAINFO_DVDIF_ANALYZE_YES
}

//---------------------------------------------------------------------------
File_DvDif::~File_DvDif()
{
    #if defined(MEDIAINFO_EIA608_YES)
        for (size_t Pos=0; Pos<CC_Parsers.size(); Pos++)
            delete CC_Parsers[Pos]; //CC_Parsers[Pos]=NULL;
    #endif
}

//***************************************************************************
// Format
//***************************************************************************

#ifdef MEDIAINFO_DVDIF_ANALYZE_YES
//---------------------------------------------------------------------------
void File_DvDif::Read_Buffer_Continue()
{
    //Errors stats
    while (Buffer_Offset+80<=Buffer_Size)
    {
        if ((Buffer[Buffer_Offset]&0xE0)==0x00  //SCT=0 (Header)
         && !(Buffer[Buffer_Offset  ]==0x00
           && Buffer[Buffer_Offset+1]==0x00
           && Buffer[Buffer_Offset+2]==0x00))
        {
            Frame_AtLeast1DIF=true;

            if (!DSF_IsValid)
            {
                DSF=(Buffer[Buffer_Offset+3]&0x80)?true:false;
                DSF_IsValid=true;
                Dseq_Old=DSF?11:9;
            }
        }

        //Quick search depends of SCT
        switch(Buffer[Buffer_Offset]&0xE0)
        {
            case 0x20 : //SCT=1 (Subcode)
                {
                    Frame_AtLeast1DIF=true;

                    for (size_t Pos=3*8; Pos<40; Pos+=2*8)
                    {
                        int8u PackType=Buffer[Buffer_Offset+3+Pos+3];
                        //timecode
                        if (PackType==0x13) //Pack type=0x13 (timecode)
                        {
                            bool  DropFrame                 =( Buffer[Buffer_Offset+3+Pos+3+1]&0x40)?true:false;
                            int8u Frames                    =((Buffer[Buffer_Offset+3+Pos+3+1]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3+1]&0x0F)   )   ;
                            int8u Seconds                   =((Buffer[Buffer_Offset+3+Pos+3+2]&0x70)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3+2]&0x0F))      ;
                            int8u Minutes                   =((Buffer[Buffer_Offset+3+Pos+3+3]&0x70)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3+3]&0x0F)   )   ;
                            int8u Hours                     =((Buffer[Buffer_Offset+3+Pos+3+4]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3+4]&0x0F)   )   ;

                            if (Frames ==0x00
                             && Seconds==0x00
                             && Minutes==0x00
                             && Hours  ==0x00
                             && Buffer[Buffer_Offset+3+Pos+3+1]==0x00
                             && Buffer[Buffer_Offset+3+Pos+3+2]==0x00
                             && Buffer[Buffer_Offset+3+Pos+3+3]==0x00
                             && Buffer[Buffer_Offset+3+Pos+3+4]==0x00
                             )
                            {
                                Frames =45;
                                Seconds=85;
                                Minutes=85;
                                Hours  =45;
                            }
                            if (Frames !=45
                             && Seconds!=85
                             && Minutes!=85
                             && Hours  !=45) //If not disabled
                            {
                                if (Speed_TimeCode_Current.IsValid
                                 && (Speed_TimeCode_Current.DropFrame !=DropFrame
                                  || Speed_TimeCode_Current.Frames    !=Frames
                                  || Speed_TimeCode_Current.Seconds   !=Seconds
                                  || Speed_TimeCode_Current.Minutes   !=Minutes
                                  || Speed_TimeCode_Current.Hours     !=Hours))
                                {
                                    Speed_TimeCode_Current.MultipleValues=true; //There are 2+ different values
                                }
                                else if (!Speed_TimeCode_Current.IsValid && !Speed_TimeCode_Current.MultipleValues)
                                {
                                    Speed_TimeCode_Current.DropFrame=DropFrame;
                                    Speed_TimeCode_Current.Frames   =Frames;
                                    Speed_TimeCode_Current.Seconds  =Seconds;
                                    Speed_TimeCode_Current.Minutes  =Minutes;
                                    Speed_TimeCode_Current.Hours    =Hours;
                                    Speed_TimeCode_Current.IsValid  =true;
                                }
                            }
                        }

                        //video_recdate
                        if (PackType==0x62) //Pack type=0x62 (video_rectime)
                        {
                            int8u Days                      =((Buffer[Buffer_Offset+3+Pos+2]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+2]&0x0F)   )   ;
                            int8u Months                    =((Buffer[Buffer_Offset+3+Pos+3]&0x10)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3]&0x0F)   )   ;
                            int8u Years                     =((Buffer[Buffer_Offset+3+Pos+4]&0xF0)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+4]&0x0F)   )   ;
                            if (Months<=12
                             && Days  <=31)
                            {
                                if (Speed_RecDate_Current.IsValid
                                 && Speed_RecDate_Current.Days      !=Days
                                 && Speed_RecDate_Current.Months     !=Months
                                 && Speed_RecDate_Current.Years     !=Years)
                                {
                                    Speed_RecDate_Current.MultipleValues=true; //There are 2+ different values
                                }
                                else if (!Speed_RecTime_Current.MultipleValues)
                                {
                                    Speed_RecDate_Current.Days     =Days;
                                    Speed_RecDate_Current.Months   =Months;
                                    Speed_RecDate_Current.Years    =Years;
                                    Speed_RecDate_Current.IsValid  =true;
                                }
                            }
                        }

                        //video_rectime
                        if (PackType==0x63) //Pack type=0x63 (video_rectime)
                        {
                            int8u Frames                    =((Buffer[Buffer_Offset+3+Pos+1]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+1]&0x0F)   )   ;
                            int8u Seconds                   =((Buffer[Buffer_Offset+3+Pos+2]&0x70)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+2]&0x0F))      ;
                            int8u Minutes                   =((Buffer[Buffer_Offset+3+Pos+3]&0x70)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3]&0x0F)   )   ;
                            int8u Hours                     =((Buffer[Buffer_Offset+3+Pos+4]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+4]&0x0F)   )   ;
                            if (Seconds<61
                             && Minutes<60
                             && Hours  <24) //If not disabled
                            {
                                if (Speed_RecTime_Current.IsValid
                                 && Speed_RecTime_Current.Frames    !=Frames
                                 && Speed_RecTime_Current.Seconds   !=Seconds
                                 && Speed_RecTime_Current.Minutes   !=Minutes
                                 && Speed_RecTime_Current.Hours     !=Hours)
                                {
                                    Speed_RecTime_Current.MultipleValues=true; //There are 2+ different values
                                }
                                else if (!Speed_RecTime_Current.MultipleValues)
                                {
                                    Speed_RecTime_Current.Frames   =Frames;
                                    Speed_RecTime_Current.Seconds  =Seconds;
                                    Speed_RecTime_Current.Minutes  =Minutes;
                                    Speed_RecTime_Current.Hours    =Hours;
                                    Speed_RecTime_Current.IsValid  =true;
                                }
                            }
                        }
                    }
                }
                break;

            case 0x40 : //SCT=2 (VAUX)
                {
                    Frame_AtLeast1DIF=true;

                    for (size_t Pos=0; Pos<15*5; Pos+=5)
                    {
                        int8u PackType=Buffer[Buffer_Offset+3+Pos];
                        //video_source
                        if (PackType==0x60 && !System_IsValid) //Pack type=0x60 (video_source)
                        {
                            System=(Buffer[Buffer_Offset+3+Pos+3]&0x20)==0x20?true:false;
                            System_IsValid=true;
                        }

                        //video_recdate
                        if (PackType==0x62) //Pack type=0x62 (video_rectime)
                        {
                            int8u Days                      =((Buffer[Buffer_Offset+3+Pos+2]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+2]&0x0F)   )   ;
                            int8u Months                    =((Buffer[Buffer_Offset+3+Pos+3]&0x10)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3]&0x0F)   )   ;
                            int8u Years                     =((Buffer[Buffer_Offset+3+Pos+4]&0xF0)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+4]&0x0F)   )   ;
                            if (Months<=12
                             && Days  <=31)
                            {
                                if (Speed_RecDate_Current.IsValid
                                 && Speed_RecDate_Current.Days      !=Days
                                 && Speed_RecDate_Current.Months     !=Months
                                 && Speed_RecDate_Current.Years     !=Years)
                                {
                                    Speed_RecDate_Current.MultipleValues=true; //There are 2+ different values
                                }
                                else if (!Speed_RecTime_Current.MultipleValues)
                                {
                                    Speed_RecDate_Current.Days     =Days;
                                    Speed_RecDate_Current.Months   =Months;
                                    Speed_RecDate_Current.Years    =Years;
                                    Speed_RecDate_Current.IsValid  =true;
                                }
                            }
                        }

                        //video_rectime
                        if (PackType==0x63) //Pack type=0x63 (video_rectime)
                        {
                            int8u Frames                    =((Buffer[Buffer_Offset+3+Pos+1]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+1]&0x0F)   )   ;
                            int8u Seconds                   =((Buffer[Buffer_Offset+3+Pos+2]&0x70)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+2]&0x0F))      ;
                            int8u Minutes                   =((Buffer[Buffer_Offset+3+Pos+3]&0x70)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+3]&0x0F)   )   ;
                            int8u Hours                     =((Buffer[Buffer_Offset+3+Pos+4]&0x30)>>4)*10
                                                           + ((Buffer[Buffer_Offset+3+Pos+4]&0x0F)   )   ;
                            if (Seconds!=85
                             && Minutes!=85
                             && Hours  !=45) //If not disabled
                            {
                                if (Speed_RecTime_Current.IsValid
                                 && Speed_RecTime_Current.Frames    !=Frames
                                 && Speed_RecTime_Current.Seconds   !=Seconds
                                 && Speed_RecTime_Current.Minutes   !=Minutes
                                 && Speed_RecTime_Current.Hours     !=Hours)
                                {
                                    Speed_RecTime_Current.MultipleValues=true; //There are 2+ different values
                                }
                                else if (!Speed_RecTime_Current.MultipleValues)
                                {
                                    Speed_RecTime_Current.Frames   =Frames;
                                    Speed_RecTime_Current.Seconds  =Seconds;
                                    Speed_RecTime_Current.Minutes  =Minutes;
                                    Speed_RecTime_Current.Hours    =Hours;
                                    Speed_RecTime_Current.IsValid  =true;
                                }
                            }
                        }
                    }
                }
                break;

            case 0x60 : //SCT=3 (Audio)
                {
                    //audio_source
                    if (Buffer[Buffer_Offset+3+0]==0x50) //audio_source
                    {
                              QU_FSC    =(Buffer[Buffer_Offset+1  ]&0x08)?true:false; //FSC
                              QU_System =(Buffer[Buffer_Offset+3+3]&0x20)?true:false; //50/60

                        int8u AUDIO_MODE=Buffer[Buffer_Offset+3+2]&0x0F;
                              QU        =Buffer[Buffer_Offset+3+4]&0x07;

                        size_t Channel=(QU_FSC?2:0)+((Buffer[Buffer_Offset+1]>>4)>=(QU_System?6:5)?1:0); //If Dseq>=5 or 6
                        if (audio_source_IsPresent.empty())
                            audio_source_IsPresent.resize(8);
                        audio_source_IsPresent[Channel]=true;

                        if (AUDIO_MODE==0x0F)
                        {
                            if (Audio_Invalids.empty())
                            {
                                Audio_Invalids.resize(8);
                                for (size_t Pos=0; Pos<8; Pos++)
                                    Audio_Invalids[Pos].resize(16);
                            }
                            Audio_Invalids[Channel][Buffer[Buffer_Offset+1]>>4]+=9;
                        }
                    }

                    //audio_source_control
                    if (Buffer[Buffer_Offset+3+0]==0x51) //audio_source_control
                    {
                        REC_ST =(Buffer[Buffer_Offset+3+2]&0x80)?true:false;
                        REC_END=(Buffer[Buffer_Offset+3+2]&0x40)?true:false;
                        REC_IsValid=true;
                    }

                    //Audio errors
                    if (Buffer[Buffer_Offset+8]==0x80)
                    {
                        if (QU==0 && Buffer[Buffer_Offset+ 9]==0x00  //16-bit 0x8000
                         || QU==1 && Buffer[Buffer_Offset+10]==0x00  //12-bit 0x800
                         || QU==(int8u)-1 && (Buffer[Buffer_Offset+ 9]==0x00 && Buffer[Buffer_Offset+10]==0x80 && Buffer[Buffer_Offset+11]==0x00
                                           || Buffer[Buffer_Offset+ 9]==0x80 && Buffer[Buffer_Offset+10]==0x00)) //In case of QU is not already detected
                        {
                            if (Audio_Errors.empty())
                                Audio_Errors.resize(16);
                            Audio_Errors[Buffer[Buffer_Offset+1]>>4]++;
                        }
                    }
                }
                break;

            case 0x80 : //SCT=4 (Video)
                {
                    if (Buffer[Buffer_Offset+3]&0xF0) //STA present
                    {
                        if (Video_STA_Errors.empty())
                            Video_STA_Errors.resize(16);
                        Video_STA_Errors[Buffer[Buffer_Offset+3]>>4]++;
                    }
                }
                break;
        }

        //Errors stats
        if (Frame_AtLeast1DIF &&
            (Buffer[Buffer_Offset  ]&0xE0)==0x00  //SCT=0 (Header)
         && (Buffer[Buffer_Offset+1]&0xF0)==0x00  //Dseq=0
         && (Buffer[Buffer_Offset+1]&0x08)==0x00) //FSC=0
            Errors_Stats_Update();

        //Coherency test
        if (Buffer[Buffer_Offset  ]==0x00
         && Buffer[Buffer_Offset+1]==0x00
         && Buffer[Buffer_Offset+2]==0x00)
           Speed_Contains_NULL++;

        Buffer_Offset+=80;
    }

    if (!IsAccepted)
        File__Analyze::Buffer_Offset=0;
}

void File_DvDif::Errors_Stats_Update()
{
    bool Errors_AreDetected=false;
    bool Infos_AreDetected=false;
    Ztring Errors_Stats_Line;
    if (Speed_FrameCount) //We must have at least one complete frame
    {
        //Framerate computing
        float64 FrameRate=29.970;
        if (System_IsValid)
            FrameRate=System?25.000:29.970;
        else if (DSF_IsValid)
            FrameRate=DSF?25.000:29.970;
        else
            FrameRate=29.970;
        if (FrameRate==29.970 && Speed_TimeCode_Current.IsValid && !Speed_TimeCode_Current.DropFrame)
            FrameRate=30.000;

        //Frame number
        Ztring Frame_Number_Padded=Ztring::ToZtring(Speed_FrameCount);
        if (Frame_Number_Padded.size()<8)
            Frame_Number_Padded.insert(0, 8-Frame_Number_Padded.size(), _T(' '));
        Errors_Stats_Line+=Frame_Number_Padded;
        Errors_Stats_Line+=_T('\t');

        //Time Offset
        float64 Time_Offset=(Speed_FrameCount-1)*1000/FrameRate;
        Errors_Stats_Line+=Ztring().Duration_From_Milliseconds((int64u)Time_Offset);
        Errors_Stats_Line+=_T('\t');

        //TimeCode
        if (Speed_TimeCode_Current.IsValid)
        {
            Speed_TimeCodeZ_Last=Speed_TimeCodeZ_Current;
            Speed_TimeCodeZ_Current.clear();
            Speed_TimeCodeZ_Current.reserve(12);
            Speed_TimeCodeZ_Current.reserve(11);
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Hours  /10;
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Hours  %10;
            Speed_TimeCodeZ_Current+=_T(':');
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Minutes/10;
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Minutes%10;
            Speed_TimeCodeZ_Current+=_T(':');
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Seconds/10;
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Seconds%10;
            Speed_TimeCodeZ_Current+=(!DSF && Speed_TimeCode_Current.DropFrame)?_T(';'):_T(':');
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Frames /10;
            Speed_TimeCodeZ_Current+=_T('0')+Speed_TimeCode_Current.Frames %10;
            Errors_Stats_Line+=Speed_TimeCodeZ_Current;
            if (Speed_TimeCodeZ.empty())
            {
                Speed_TimeCodeZ.resize(1);
                Speed_TimeCodeZ[0].First.FramePos=Speed_FrameCount;
                Speed_TimeCodeZ[0].First.TimeCode=Speed_TimeCodeZ_Current;
            }
        }
        else
            Errors_Stats_Line+=_T("XX:XX:XX:XX");
        Errors_Stats_Line+=_T('\t');

        //Order coherency
        if (Speed_TimeCode_Current.IsValid && Speed_TimeCode_Last.IsValid
         && Speed_TimeCode_Current.Frames ==Speed_TimeCode_Last.Frames
         && Speed_TimeCode_Current.Seconds==Speed_TimeCode_Last.Seconds
         && Speed_TimeCode_Current.Minutes==Speed_TimeCode_Last.Minutes
         && Speed_TimeCode_Current.Hours  ==Speed_TimeCode_Last.Hours)
        {
            Errors_Stats_Line+=_T('R');
            if (Speed_TimeCode_Current.Hours
             || Speed_TimeCode_Current.Seconds
             || Speed_TimeCode_Current.Minutes
             || Speed_TimeCode_Current.Hours)
                Errors_AreDetected=true;
        }
        else if (Speed_TimeCode_Current.IsValid && Speed_TimeCode_Current_Theory.IsValid
              && (   Speed_TimeCode_Current.Frames !=Speed_TimeCode_Current_Theory.Frames
                  || Speed_TimeCode_Current.Seconds!=Speed_TimeCode_Current_Theory.Seconds
                  || Speed_TimeCode_Current.Minutes!=Speed_TimeCode_Current_Theory.Minutes
                  || Speed_TimeCode_Current.Hours  !=Speed_TimeCode_Current_Theory.Hours))
        {
            size_t Speed_TimeCodeZ_Pos=Speed_TimeCodeZ.size();
            Speed_TimeCodeZ.resize(Speed_TimeCodeZ_Pos+1);
            Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].First.FramePos=Speed_FrameCount-1;
            Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].First.TimeCode=Speed_TimeCodeZ_Current;
            Speed_TimeCodeZ[Speed_TimeCodeZ_Pos-1].Last.FramePos=Speed_FrameCount-1;
            Speed_TimeCodeZ[Speed_TimeCodeZ_Pos-1].Last.TimeCode=Speed_TimeCodeZ_Last;

            Errors_Stats_Line+=_T('N');
            Speed_TimeCode_Current_Theory=Speed_TimeCode_Current;
            Errors_AreDetected=true;
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');

        //RecDate/RecTime
        if (Speed_RecDate_Current.IsValid)
        {
            Speed_RecDateZ_Last=Speed_RecDateZ_Current;
            Speed_RecDateZ_Current.clear();
            Speed_RecDateZ_Current.reserve(10);
            Speed_RecDateZ_Current+=Speed_RecDate_Current.Years<75?_T("20"):_T("19");
            Speed_RecDateZ_Current+=_T('0')+Speed_RecDate_Current.Years  /10;
            Speed_RecDateZ_Current+=_T('0')+Speed_RecDate_Current.Years  %10;
            Speed_RecDateZ_Current+=_T('-');
            Speed_RecDateZ_Current+=_T('0')+Speed_RecDate_Current.Months /10;
            Speed_RecDateZ_Current+=_T('0')+Speed_RecDate_Current.Months %10;
            Speed_RecDateZ_Current+=_T('-');
            Speed_RecDateZ_Current+=_T('0')+Speed_RecDate_Current.Days   /10;
            Speed_RecDateZ_Current+=_T('0')+Speed_RecDate_Current.Days   %10;
            Errors_Stats_Line+=Speed_RecDateZ_Current;
            if (Speed_RecZ.empty())
            {
                Speed_RecZ.resize(1);
                Speed_RecZ[0].First.FramePos=Speed_FrameCount;
                Speed_RecZ[0].First.Date=Speed_RecDateZ_Current;
            }
        }
        else
            Errors_Stats_Line+=_T("XXXX-XX-XX");
        Errors_Stats_Line+=_T(" ");
        if (Speed_RecTime_Current.IsValid)
        {
            Speed_RecTimeZ_Last=Speed_RecTimeZ_Current;
            Speed_RecTimeZ_Current.clear();
            Speed_RecTimeZ_Current.reserve(12);
            Speed_RecTimeZ_Current+=_T('0')+Speed_RecTime_Current.Hours  /10;
            Speed_RecTimeZ_Current+=_T('0')+Speed_RecTime_Current.Hours  %10;
            Speed_RecTimeZ_Current+=_T(':');
            Speed_RecTimeZ_Current+=_T('0')+Speed_RecTime_Current.Minutes/10;
            Speed_RecTimeZ_Current+=_T('0')+Speed_RecTime_Current.Minutes%10;
            Speed_RecTimeZ_Current+=_T(':');
            Speed_RecTimeZ_Current+=_T('0')+Speed_RecTime_Current.Seconds/10;
            Speed_RecTimeZ_Current+=_T('0')+Speed_RecTime_Current.Seconds%10;
            if (Speed_RecTime_Current.Frames!=45)
            {
                int32u Milliseconds=Speed_RecTime_Current.Frames*(System?40:33);
                Speed_RecTimeZ_Current+=_T('.');
                Speed_RecTimeZ_Current+=_T('0')+(Char)(Milliseconds/100);
                Speed_RecTimeZ_Current+=_T('0')+(Char)((Milliseconds%100)/10);
                Speed_RecTimeZ_Current+=_T('0')+(Char)(Milliseconds%10);
            }
            else
                Speed_RecTimeZ_Current+=_T("    ");
            Errors_Stats_Line+=Speed_RecTimeZ_Current;
            if (Speed_RecZ.empty() || Speed_RecZ[0].First.FramePos==Speed_FrameCount) //Empty or the same frame as RecDate
            {
                if (Speed_RecZ.empty())
                    Speed_RecZ.resize(1);
                Speed_RecZ[0].First.FramePos=Speed_FrameCount;
                Speed_RecZ[0].First.Time=Speed_RecTimeZ_Current;
            }
        }
        else
            Errors_Stats_Line+=_T("XX:XX:XX.XXX");
        Errors_Stats_Line+=_T('\t');

        //RecDate/RecTime coherency, Rec start/end
        if (/*(!REC_IsValid || !REC_ST || !REC_END) &&*/
            Speed_RecTime_Current.IsValid && Speed_RecTime_Current_Theory.IsValid
         && !(   Speed_RecTime_Current.Seconds==Speed_RecTime_Current_Theory.Seconds
              && Speed_RecTime_Current.Minutes==Speed_RecTime_Current_Theory.Minutes
              && Speed_RecTime_Current.Hours  ==Speed_RecTime_Current_Theory.Hours)
         && !(   Speed_RecTime_Current.Seconds==Speed_RecTime_Current_Theory2.Seconds
              && Speed_RecTime_Current.Minutes==Speed_RecTime_Current_Theory2.Minutes
              && Speed_RecTime_Current.Hours  ==Speed_RecTime_Current_Theory2.Hours))
        {
            size_t Speed_RecZ_Pos=Speed_RecZ.size();
            Speed_RecZ.resize(Speed_RecZ_Pos+1);
            Speed_RecZ[Speed_RecZ_Pos].First.FramePos=Speed_FrameCount-1;
            Speed_RecZ[Speed_RecZ_Pos].First.Date=Speed_RecDateZ_Current;
            Speed_RecZ[Speed_RecZ_Pos].First.Time=Speed_RecTimeZ_Current;
            Speed_RecZ[Speed_RecZ_Pos-1].Last.FramePos=Speed_FrameCount-1;
            Speed_RecZ[Speed_RecZ_Pos-1].Last.Date=Speed_RecDateZ_Last;
            Speed_RecZ[Speed_RecZ_Pos-1].Last.Time=Speed_RecTimeZ_Last;

            Errors_Stats_Line+=_T('N');
            if (!REC_IsValid || REC_ST)
                Errors_AreDetected=true; //If there is a start, this is not an error
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');

        if (REC_IsValid && !REC_ST)
        {
            Errors_Stats_Line+=_T('S');
            Infos_AreDetected=true;
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');

        if (REC_IsValid && !REC_END)
        {
            Errors_Stats_Line+=_T('E');
            Infos_AreDetected=true;
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');

        //Channels
        for (size_t Channel=0; Channel<8; Channel++)
        {
            if (!audio_source_IsPresent.empty() && audio_source_IsPresent[Channel])
            {
                if (Channel<4 && (!Audio_Invalids.empty() || !Audio_Errors.empty()))
                {
                    size_t Audio_Errors_Count=0;
                    size_t Pos_Begin=(Channel%2)*(QU_System?6:5);
                    size_t Pos_End=(Channel%2+1)*(QU_System?6:5);
                    if (Channel>=2 && !QU_FSC
                     || Channel< 2 &&  QU_FSC)
                        Pos_End=Pos_Begin; //Not here
                    if (!Audio_Invalids.empty())
                        for (size_t Pos=Pos_Begin; Pos<Pos_End; Pos++)
                            if (Audio_Invalids[Channel][Pos])
                                Audio_Errors_Count+=Audio_Invalids[Channel][Pos];
                    if (!Audio_Errors.empty())
                        for (size_t Pos=Pos_Begin; Pos<Pos_End; Pos++)
                            if (Audio_Errors[Pos])
                                Audio_Errors_Count+=Audio_Errors[Pos];
                    if (Audio_Errors_Count>=(size_t)((QU_System?6:5)*9))
                        audio_source_IsPresent[Channel]=false;
                    else
                        CH_IsPresent[Channel]=true;
                }
            }
        }

        //Error 1: Video errors
        Ztring Errors_Stats_Line_Details;
        if (!Video_STA_Errors.empty())
        {
            Errors_Stats_Line+=_T('1');

            size_t Video_STA_Errors_Count=0;
            Ztring Video_STA_Errors_Details;
            for (size_t Pos=0; Pos<16; Pos++)
            {
                if (Video_STA_Errors[Pos])
                {
                    Video_STA_Errors_Count+=Video_STA_Errors[Pos];
                    Ztring Video_STA_Errors_Count_Padded=Ztring::ToZtring(Video_STA_Errors[Pos]);
                    if (Video_STA_Errors_Count_Padded.size()<8)
                        Video_STA_Errors_Count_Padded.insert(0, 8-Video_STA_Errors_Count_Padded.size(), _T(' '));
                    Video_STA_Errors_Details+=Video_STA_Errors_Count_Padded;
                    Video_STA_Errors_Details+=_T(" \"");
                    Video_STA_Errors_Details+=Ztring::ToZtring(Pos, 16);
                    Video_STA_Errors_Details+=_T("\" errors, ");
                    if (Video_STA_Errors_Total.empty())
                        Video_STA_Errors_Total.resize(16);
                    Video_STA_Errors_Total[Pos]+=Video_STA_Errors[Pos];
                }
            }
            if (Video_STA_Errors_Details.size()>2)
            {
                Ztring Video_STA_Errors_Count_Padded=Ztring::ToZtring(Video_STA_Errors_Count);
                if (Video_STA_Errors_Count_Padded.size()<8)
                    Video_STA_Errors_Count_Padded.insert(0, 8-Video_STA_Errors_Count_Padded.size(), _T(' '));
                Errors_Stats_Line_Details+=Video_STA_Errors_Count_Padded+_T(" STA errors");
                Video_STA_Errors_Details.resize(Video_STA_Errors_Details.size()-2);
                Errors_Stats_Line_Details+=_T(" (")+Video_STA_Errors_Details+_T(")");
                Speed_FrameCount_Video_STA_Errors++;
                Errors_AreDetected=true;
            }
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 2: Audio errors
        if (QU!=(int8u)-1 && (!Audio_Invalids.empty() || !Audio_Errors.empty()))
        {
            if (Audio_Errors.empty())
                Audio_Errors.resize(16);
            bool ErrorsAreAlreadyDetected=false;
            for (size_t Channel=0; Channel<4; Channel++)
            {
                size_t Audio_Errors_Count=0;
                Ztring Audio_Errors_Details;
                size_t Pos_Begin=(Channel%2)*(QU_System?6:5);
                size_t Pos_End=(Channel%2+1)*(QU_System?6:5);
                if (Channel>=2 && !QU_FSC
                 || Channel< 2 &&  QU_FSC
                 || !CH_IsPresent[Channel])
                    Pos_End=Pos_Begin; //Not here
                for (size_t Pos=Pos_Begin; Pos<Pos_End; Pos++)
                {
                    if (Audio_Errors[Pos])
                    {
                        Audio_Errors_Count+=Audio_Errors[Pos];
                        Ztring Audio_Errors_Count_Padded=Ztring::ToZtring(Audio_Errors[Pos]);
                        if (Audio_Errors_Count_Padded.size()<2)
                            Audio_Errors_Count_Padded.insert(0, 2-Audio_Errors_Count_Padded.size(), _T(' '));
                        Audio_Errors_Details+=Audio_Errors_Count_Padded;
                        Audio_Errors_Details+=_T(" Dseq=");
                        Audio_Errors_Details+=Ztring::ToZtring(Pos, 16);
                        Audio_Errors_Details+=_T(", ");
                        if (Audio_Errors_Total.empty())
                        {
                            Audio_Errors_Total.resize(8);
                            for (size_t Pos=0; Pos<8; Pos++)
                                Audio_Errors_Total[Pos].resize(16);
                        }
                        Audio_Errors_Total[Channel][Pos]+=Audio_Errors[Pos];
                    }
                }
                if (Audio_Errors_Count)
                {
                    if (!ErrorsAreAlreadyDetected)
                        Errors_Stats_Line+=_T('2');

                    Ztring Audio_Errors_Count_Padded=Ztring::ToZtring(Audio_Errors_Count);
                    if (Audio_Errors_Count_Padded.size()<2)
                        Audio_Errors_Count_Padded.insert(0, 2-Audio_Errors_Count_Padded.size(), _T(' '));
                    if (ErrorsAreAlreadyDetected)
                        Errors_Stats_Line_Details+=_T(", ");
                    if (Audio_Errors_Count<(size_t)((QU_System?6:5)*9))
                    {
                        Errors_Stats_Line_Details+=_T("CH")+Ztring::ToZtring(Channel+1)+_T(": ")+Audio_Errors_Count_Padded+_T(" audio errors");
                        Audio_Errors_Details.resize(Audio_Errors_Details.size()-2);
                        Errors_Stats_Line_Details+=_T(" (")+Audio_Errors_Details+_T(")");
                    }
                    else
                        Errors_Stats_Line_Details+=_T("CH")+Ztring::ToZtring(Channel+1)+_T(": no valid DIF");

                    Speed_FrameCount_Audio_Errors[Channel]++;
                    ErrorsAreAlreadyDetected=true;
                    Errors_AreDetected=true;
                }
            }
            if (!ErrorsAreAlreadyDetected)
                Errors_Stats_Line+=_T(' ');
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 3: Timecode incoherency
        if (Speed_TimeCode_Current.MultipleValues)
        {
            Errors_Stats_Line+=_T('3');
            Errors_Stats_Line_Details+=_T("(Timecode incoherency, first detected value is used)");
            Speed_FrameCount_Timecode_Incoherency++;
            Errors_AreDetected=true;
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 4: DIF order incoherency
        if (Speed_Contains_NULL)
        {
            Errors_Stats_Line+=_T('4');
            Errors_Stats_Line_Details+=_T("\t(DIF incoherency, ")+Ztring::ToZtring(Speed_Contains_NULL)+_T(" NULL DIFs)");
            Speed_FrameCount_Contains_NULL++;
            Errors_AreDetected=true;
        }
        else
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 5:
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 6:
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 7:
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 8:
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 9:
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Error 0:
            Errors_Stats_Line+=_T(' ');
        Errors_Stats_Line+=_T('\t');
        Errors_Stats_Line_Details+=_T('\t');

        //Filling the main text if needed
        if (Speed_FrameCount==1
         || IsFinished
         ||                       MediaInfoLib::Config.Verbosity_Get()>=(float32)1.0
         || Infos_AreDetected  && MediaInfoLib::Config.Verbosity_Get()>=(float32)0.5
         || Errors_AreDetected && MediaInfoLib::Config.Verbosity_Get()>=(float32)0.2)
        {
            Errors_Stats+=Errors_Stats_Line;
            Errors_Stats+=Errors_Stats_Line_Details;
            Errors_Stats+=_T("&");
        }
    }

    //Speed_TimeCode_Current
    if (!Speed_TimeCode_Current_Theory.IsValid)
        Speed_TimeCode_Current_Theory=Speed_TimeCode_Current;
    if (Speed_TimeCode_Current_Theory.IsValid)
    {
        int8u Frames_Max;
        if (System_IsValid)
            Frames_Max=System?25:30;
        else if (DSF_IsValid)
            Frames_Max=DSF?25:30;
        else
            Frames_Max=30;

        Speed_TimeCode_Current_Theory.Frames++;
        if (Speed_TimeCode_Current_Theory.Frames>=Frames_Max)
        {
            Speed_TimeCode_Current_Theory.Seconds++;
            Speed_TimeCode_Current_Theory.Frames=0;
            if (Speed_TimeCode_Current_Theory.Seconds>=60)
            {
                Speed_TimeCode_Current_Theory.Seconds=0;
                Speed_TimeCode_Current_Theory.Minutes++;

                if (Speed_TimeCode_Current_Theory.DropFrame && Speed_TimeCode_Current_Theory.Minutes%10)
                    Speed_TimeCode_Current_Theory.Frames=2; //frames 0 and 1 are dropped for every minutes except 00 10 20 30 40 50

                if (Speed_TimeCode_Current_Theory.Seconds>=60)
                {
                    Speed_TimeCode_Current_Theory.Minutes=0;
                    Speed_TimeCode_Current_Theory.Hours++;
                    if (Speed_TimeCode_Current_Theory.Hours>=24)
                    {
                        Speed_TimeCode_Current_Theory.Hours=0;
                    }
                }
            }
        }
    }

    //Speed_RecTime_Current_Theory
    Speed_RecTime_Current_Theory=Speed_RecTime_Current;
    Speed_RecTime_Current_Theory2=Speed_RecTime_Current; //Don't change it
    if (Speed_RecTime_Current_Theory.IsValid)
    {
        Speed_RecTime_Current_Theory.Seconds++;
        if (Speed_RecTime_Current_Theory.Seconds>=60)
        {
            Speed_RecTime_Current_Theory.Seconds=0;
            Speed_RecTime_Current_Theory.Minutes++;
            if (Speed_RecTime_Current_Theory.Seconds>=60)
            {
                Speed_RecTime_Current_Theory.Minutes=0;
                Speed_RecTime_Current_Theory.Hours++;
                if (Speed_RecTime_Current_Theory.Hours>=24)
                {
                    Speed_RecTime_Current_Theory.Hours=0;
                }
            }
        }
    }

    Speed_TimeCode_Last=Speed_TimeCode_Current;
    Speed_TimeCode_Current.Clear();
    Speed_RecDate_Current.IsValid=false;
    Speed_RecTime_Current.IsValid=false;
    Speed_FrameCount++;
    REC_IsValid=false;
    Speed_Contains_NULL=0;
    Frame_AtLeast1DIF=true;
    if (Buffer_Offset+2>=Buffer_Size
     || Buffer[Buffer_Offset  ]==0x00
     && Buffer[Buffer_Offset+1]==0x00
     && Buffer[Buffer_Offset+2]==0x00)
        Frame_AtLeast1DIF=false;
    Video_STA_Errors.clear();
    Audio_Errors.clear();
    Audio_Invalids.clear();
}

void File_DvDif::Errors_Stats_Update_Finnish()
{
    //Preparing next frame
    Speed_FrameCount--;
    Ztring Errors_Stats_End;
    Ztring Errors_Stats_End_Lines;

    //Frames
    if (Speed_FrameCount)
        Errors_Stats_End_Lines+=_T("Frame Count: ")+Ztring::ToZtring(Speed_FrameCount)+_T('&');

    //One block
    if (!Errors_Stats_End_Lines.empty())
    {
        Errors_Stats_End+=Errors_Stats_End_Lines;
        Errors_Stats_End_Lines.clear();
        Errors_Stats_End+=_T('&');
    }

    //Error 1: Video errors (STA)
    if (Speed_FrameCount_Video_STA_Errors)
        Errors_Stats_End_Lines+=_T("Frame Count with video STA errors: ")+Ztring::ToZtring(Speed_FrameCount_Video_STA_Errors)+_T('&');
    if (!Video_STA_Errors_Total.empty())
    {
        Ztring Errors_Details;
        size_t Errors_Count=0;
        for (size_t Pos=0; Pos<16; Pos++)
        {
            if (Video_STA_Errors_Total[Pos])
            {
                Errors_Count+=Video_STA_Errors_Total[Pos];
                Ztring Errors_Count_Padded=Ztring::ToZtring(Video_STA_Errors_Total[Pos]);
                if (Errors_Count_Padded.size()<8)
                    Errors_Count_Padded.insert(0, 8-Errors_Count_Padded.size(), _T(' '));
                Errors_Details+=Errors_Count_Padded;
                Errors_Details+=_T(" \"");
                Errors_Details+=Ztring::ToZtring(Pos, 16);
                Errors_Details+=_T("\" errors, ");
            }
        }
        if (Errors_Details.size()>2)
        {
            Errors_Stats_End_Lines+=_T("Total video STA errors: ");
            Ztring Errors_Count_Padded=Ztring::ToZtring(Errors_Count);
            if (Errors_Count_Padded.size()<8)
                Errors_Count_Padded.insert(0, 8-Errors_Count_Padded.size(), _T(' '));
            Errors_Stats_End_Lines+=_T(" ")+Errors_Count_Padded+_T(" video STA errors");
            Errors_Details.resize(Errors_Details.size()-2);
            Errors_Stats_End_Lines+=_T(" (")+Errors_Details+_T(")")+_T('&');
        }
    }

    //Error 2: Audio errors
    if (!Audio_Errors_Total.empty())
    {
        for (size_t Channel=0; Channel<8; Channel++)
        {
            if (Speed_FrameCount_Audio_Errors[Channel])
                Errors_Stats_End_Lines+=_T("Frame Count with CH")+Ztring::ToZtring(Channel+1)+_T(" audio errors: ")+Ztring::ToZtring(Speed_FrameCount_Audio_Errors[Channel])+_T('&');

            Ztring Errors_Details;
            size_t Errors_Count=0;
            for (size_t Pos=0; Pos<16; Pos++)
            {
                if (Audio_Errors_Total[Channel][Pos])
                {
                    Errors_Count+=Audio_Errors_Total[Channel][Pos];
                    Ztring Errors_Count_Padded=Ztring::ToZtring(Audio_Errors_Total[Channel][Pos]);
                    if (Errors_Count_Padded.size()<8)
                        Errors_Count_Padded.insert(0, 8-Errors_Count_Padded.size(), _T(' '));
                    Errors_Details+=Errors_Count_Padded;
                    Errors_Details+=_T(" Dseq=");
                    Errors_Details+=Ztring::ToZtring(Pos, 16);
                    Errors_Details+=_T(", ");
                }
            }
            if (Errors_Details.size()>2)
            {
                Errors_Stats_End_Lines+=_T("Total audio errors for CH")+Ztring::ToZtring(Channel+1)+_T(": ");
                Ztring Errors_Count_Padded=Ztring::ToZtring(Errors_Count);
                if (Errors_Count_Padded.size()<8)
                    Errors_Count_Padded.insert(0, 8-Errors_Count_Padded.size(), _T(' '));
                Errors_Stats_End_Lines+=_T(" ")+Errors_Count_Padded+_T(" audio errors");
                Errors_Details.resize(Errors_Details.size()-2);
                Errors_Stats_End_Lines+=_T(" (")+Errors_Details+_T(")")+_T('&');
            }
        }
    }

    //Error 3: Timecode incoherency
    if (Speed_FrameCount_Timecode_Incoherency)
        Errors_Stats_End_Lines+=_T("Frame Count with Timecode incoherency errors: ")+Ztring::ToZtring(Speed_FrameCount_Timecode_Incoherency)+_T('&');

    //Error 4: Contains NULL DIFs
    if (Speed_FrameCount_Contains_NULL)
        Errors_Stats_End_Lines+=_T("Frame Count with NULL DIF errors: ")+Ztring::ToZtring(Speed_FrameCount_Contains_NULL)+_T('&');

    //One block
    if (!Errors_Stats_End_Lines.empty())
    {
        Errors_Stats_End+=Errors_Stats_End_Lines;
        Errors_Stats_End_Lines.clear();
        Errors_Stats_End+=_T('&');
    }

    //RecDate/RecTime
    if (!Speed_RecDateZ_Current.empty() && !Speed_RecTimeZ_Current.empty()) //Date and Time must be both available
    {
        size_t Speed_RecZ_Pos=Speed_RecZ.size();
        if (Speed_RecZ_Pos)
        {
            Speed_RecZ_Pos--;
            Speed_RecZ[Speed_RecZ_Pos].Last.FramePos=Speed_FrameCount;
            Speed_RecZ[Speed_RecZ_Pos].Last.Date=Speed_RecDateZ_Current;
            Speed_RecZ[Speed_RecZ_Pos].Last.Time=Speed_RecTimeZ_Current;
            if (Speed_RecZ[Speed_RecZ_Pos].Last.FramePos-(Speed_RecZ_Pos?Speed_RecZ[Speed_RecZ_Pos-1].Last.FramePos:0)==1)
            {
                //Only one frame, the "Last" part is not filled
                Speed_RecZ[Speed_RecZ_Pos].Last.Date=Speed_RecZ[Speed_RecZ_Pos].First.Date;
                Speed_RecZ[Speed_RecZ_Pos].Last.Time=Speed_RecZ[Speed_RecZ_Pos].First.Time;
            }

            for (size_t Pos=0; Pos<Speed_RecZ.size(); Pos++)
            {
                Errors_Stats_End_Lines+=_T("Date Covered: ");
                Errors_Stats_End_Lines+=Speed_RecZ[Pos].First.Date.empty()?Ztring(_T("XXXX-XX-XX")):Speed_RecZ[Pos].First.Date;
                Errors_Stats_End_Lines+=_T(' ');
                Errors_Stats_End_Lines+=Speed_RecZ[Pos].First.Time.empty()?Ztring(_T("XX:XX:XX:XX")):Speed_RecZ[Pos].First.Time;

                Errors_Stats_End_Lines+=_T(" - ");

                Errors_Stats_End_Lines+=Speed_RecZ[Pos].Last.Date.empty()?Ztring(_T("XXXX-XX-XX")):Speed_RecZ[Pos].Last.Date;
                Errors_Stats_End_Lines+=_T(' ');
                Errors_Stats_End_Lines+=Speed_RecZ[Pos].Last.Time.empty()?Ztring(_T("XX:XX:XX:XX")):Speed_RecZ[Pos].Last.Time;

                Ztring Start_Padded=Ztring::ToZtring((Pos?Speed_RecZ[Pos-1].Last.FramePos+1:1));
                if (Start_Padded.size()<8)
                    Start_Padded.insert(0, 8-Start_Padded.size(), _T(' '));
                Errors_Stats_End_Lines+=_T(" (start at ")+Start_Padded;

                Ztring Frames_Padded=Ztring::ToZtring(Speed_RecZ[Pos].Last.FramePos-(Pos?Speed_RecZ[Pos-1].Last.FramePos:0));
                if (Frames_Padded.size()<8)
                    Frames_Padded.insert(0, 8-Frames_Padded.size(), _T(' '));
                Errors_Stats_End_Lines+=_T(", ")+Frames_Padded+_T(" frames)");

                Errors_Stats_End_Lines+=_T('&');
            }
        }
    }

    //One block
    if (!Errors_Stats_End_Lines.empty())
    {
        Errors_Stats_End+=Errors_Stats_End_Lines;
        Errors_Stats_End_Lines.clear();
        Errors_Stats_End+=_T('&');
    }

    //TimeCode
    if (!Speed_TimeCodeZ_Current.empty())
    {
        size_t Speed_TimeCodeZ_Pos=Speed_TimeCodeZ.size();
        if (Speed_TimeCodeZ_Pos)
        {
            Speed_TimeCodeZ_Pos--;
            Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].Last.FramePos=Speed_FrameCount;
            Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].Last.TimeCode=Speed_TimeCodeZ_Current;
            if (Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].Last.FramePos-(Speed_TimeCodeZ_Pos?Speed_TimeCodeZ[Speed_TimeCodeZ_Pos-1].Last.FramePos:0)==1)
            {
                //Only one frame, the "Last" part is not filled
                Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].Last.TimeCode=Speed_TimeCodeZ[Speed_TimeCodeZ_Pos].First.TimeCode;
            }

            for (size_t Pos=0; Pos<Speed_TimeCodeZ.size(); Pos++)
            {
                Errors_Stats_End_Lines+=_T("Timecode Covered: ");
                Errors_Stats_End_Lines+=Speed_TimeCodeZ[Pos].First.TimeCode;

                Errors_Stats_End_Lines+=_T(" - ");

                Errors_Stats_End_Lines+=Speed_TimeCodeZ[Pos].Last.TimeCode;

                Ztring Start_Padded=Ztring::ToZtring((Pos?Speed_TimeCodeZ[Pos-1].Last.FramePos+1:1));
                if (Start_Padded.size()<8)
                    Start_Padded.insert(0, 8-Start_Padded.size(), _T(' '));
                Errors_Stats_End_Lines+=_T(" (start at ")+Start_Padded;

                Ztring Frames_Padded=Ztring::ToZtring(Speed_TimeCodeZ[Pos].Last.FramePos-(Pos?Speed_TimeCodeZ[Pos-1].Last.FramePos:0));
                if (Frames_Padded.size()<8)
                    Frames_Padded.insert(0, 8-Frames_Padded.size(), _T(' '));
                Errors_Stats_End_Lines+=_T(", ")+Frames_Padded+_T(" frames)");

                Errors_Stats_End_Lines+=_T('&');
            }
        }
    }

    //One block
    if (!Errors_Stats_End_Lines.empty())
    {
        Errors_Stats_End+=Errors_Stats_End_Lines;
        Errors_Stats_End_Lines.clear();
        Errors_Stats_End+=_T('&');
    }

    //
    if (Errors_Stats_End.size()>2)
        Errors_Stats_End.resize(Errors_Stats_End.size()-2); //Removing last carriage returns

    //Filling
    if (Count_Get(Stream_Video)==0)
        Stream_Prepare(Stream_Video);
    Fill(Stream_Video, 0, "Errors_Stats_Begin", "Frame # \tTime        \tTimeCode   \tN\tRecorded date/time     \tN\tS\tE\t1\t2\t3\t4\t5\t6\t7\t8\t9\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\t0");
    Fill(Stream_Video, 0, "Errors_Stats", Errors_Stats);
    Fill(Stream_Video, 0, "Errors_Stats_End", Errors_Stats_End);
    Fill(Stream_Video, 0, "FrameCount_Speed", Speed_FrameCount);
}
#endif //MEDIAINFO_DVDIF_ANALYZE_YES

//---------------------------------------------------------------------------
void File_DvDif::Read_Buffer_Finalize()
{
    if (!IsFilled && FrameCount>=1)
        Header_Fill();

    if (!Recorded_Date_Date.empty())
    {
        if (Recorded_Date_Time.size()>4)
        {
            Recorded_Date_Time.resize(Recorded_Date_Time.size()-4); //Keep out milliseconds
            Recorded_Date_Date+=_T(" ");
            Recorded_Date_Date+=Recorded_Date_Time;
        }
        Fill(Stream_General, 0, General_Recorded_Date, Recorded_Date_Date);
    }
    if (!File_Name.empty() && Duration)
        Fill(Stream_General, 0, General_Duration, Duration);

    //Delay
    if (TimeCode_First!=(int64u)-1)
    {
        Fill(Stream_Video, 0, Video_Delay, TimeCode_First);
        Fill(Stream_Audio, 0, Audio_Delay, TimeCode_First);
    }

    #if defined(MEDIAINFO_EIA608_YES)
        for (size_t Pos=0; Pos<CC_Parsers.size(); Pos++)
            if (CC_Parsers[Pos] && CC_Parsers[Pos]->IsAccepted)
            {
                Open_Buffer_Finalize(CC_Parsers[Pos]);
                Merge(*CC_Parsers[Pos]);
                Fill(Stream_Text, StreamPos_Last, Text_ID, Pos);
            }
    #endif

    #ifdef MEDIAINFO_DVDIF_ANALYZE_YES
    //Errors stats
    if (!IsFinished)
        Finish();
    Errors_Stats_Update();
    Errors_Stats_Update_Finnish();
    #endif //MEDIAINFO_DVDIF_ANALYZE_YES
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header_Parse()
#ifndef MEDIAINFO_MINIMIZESIZE
{
    if (AuxToAnalyze!=0x00)
    {
        Header_Fill_Code(AuxToAnalyze, Ztring::ToZtring(AuxToAnalyze, 16));
        Header_Fill_Size(4);
        return;
    }

    //Unsynch problems
    if (Element_Size<80)
    {
        Element_WaitForMoreData();
        return;
    }
    if (Buffer[Buffer_Offset  ]==0x00
     && Buffer[Buffer_Offset+1]==0x00
     && Buffer[Buffer_Offset+2]==0x00)
    {
        Header_Fill_Code((int64u)-1);
        Header_Fill_Size(80);
        return;
    }

    //Parsing
    BS_Begin();
    //0
    Get_S1 (3, SCT,                                             "SCT - Section Type"); Param_Info(Dv_sct[SCT]);
    Skip_SB(                                                    "Res - Reserved");
    Skip_S1(4,                                                  "Arb - Arbitrary bits");
    //1
    Get_S1 (4, Dseq,                                            "Dseq - DIF sequence number"); //0-9 for 525/60; 0-11 for 625/50
    Get_S1 (1, FSC,                                             "FSC - Channel number");
    Skip_S1(3,                                                  "Res - Reserved");
    BS_End();
    //2
    Get_B1 (DBN,                                                "DBN - DIF block number"); //Video: 0-134, Audio: 0-8

    Header_Fill_Code(SCT, Dv_sct[SCT]);
    Header_Fill_Size(80);
}
#else //MEDIAINFO_MINIMIZESIZE
{
    if (AuxToAnalyze!=0x00)
    {
        Header_Fill_Code(AuxToAnalyze);
        Header_Fill_Size(4);
        return;
    }

    //Unsynch problems
    if (Element_Size<80)
    {
        Element_WaitForMoreData();
        return;
    }
    if (Buffer[Buffer_Offset  ]==0x00
     && Buffer[Buffer_Offset+1]==0x00
     && Buffer[Buffer_Offset+2]==0x00)
    {
        Header_Fill_Code((int64u)-1);
        Header_Fill_Size(80);
        return;
    }
    
    //Parsing
    SCT =(Buffer[Buffer_Offset  ]&0xE0)>>5;
    Dseq=(Buffer[Buffer_Offset+1]&0xF0)>>4;
    FSC =(Buffer[Buffer_Offset+1]&0x08)>>3;
    DBN = Buffer[Buffer_Offset+2];
    Element_Offset+=3;

    Header_Fill_Code(SCT);
    Header_Fill_Size(80);
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File_DvDif::Data_Parse()
{
    if (AuxToAnalyze!=0x00)
    {
        Element();
        return;
    }

    if (Element_Code==(int64u)-1)
    {
        Skip_XX(Element_Size,                                   "Junk");
        return;
    }

    //Integrity
    if (!IsAccepted)
    {
        //DIF Sequence Numbers
        if (DSF_IsValid)
        {
            if (Dseq_Old!=Dseq)
            {
                if (Dseq==0
                 && !(!DSF && Dseq_Old==9)
                 && !( DSF && Dseq_Old==11))
                    Trusted_IsNot("Wrong order");
                Dseq_Old=Dseq;
            }
        }

        //DIF Block Numbers
        int8u Number=DBN_Olds[(size_t)Element_Code]+1;
        switch (SCT)
        {
            case 0 : //Header
                        if (SCT_Old!=4
                         || DBN!=0)
                            Trusted_IsNot("Wrong order");
                        break;
            case 1 : //Subcode
                        if (!((DBN==0 && SCT_Old==0) || (DBN!=0 && SCT_Old==1))
                         || Number!=DBN && !(Number==2 && DBN==0))
                            Trusted_IsNot("Wrong order");
                        break;
            case 2 : //VAUX
                        if (!((DBN==0 && SCT_Old==1) || (DBN!=0 && SCT_Old==2))
                         || Number!=DBN && !(Number==3 && DBN==0))
                            Trusted_IsNot("Wrong order");
                        break;
            case 3 : //Audio
                        if (!((DBN==0 && SCT_Old==2) || (DBN!=0 && SCT_Old==4))
                         || Number!=DBN && !(Number==9 && DBN==0))
                            Trusted_IsNot("Wrong order");
                        break;
            case 4 : //Video
                        if (!(SCT_Old==3 || SCT_Old==4)
                         || Number!=DBN && !(Number==135 && DBN==0))
                            Trusted_IsNot("Wrong order");
                        break;
            default: ;
        }
        SCT_Old=SCT;
        DBN_Olds[SCT]=DBN;
    }

    Element_Info(DBN);

    switch (SCT)
    {
        case 0 : Header(); break;
        case 1 : Subcode(); break;
        case 2 : VAUX(); break;
        case 3 : Audio(); break;
        #ifndef MEDIAINFO_MINIMIZESIZE
        case 4 : Video(); break;
        default: Skip_XX(Element_Size,                          "Unknown");
        #endif //MEDIAINFO_MINIMIZESIZE
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header()
#ifndef MEDIAINFO_MINIMIZESIZE
{
    BS_Begin();
    //3
    Get_SB (   DSF,                                             "DSF - DIF Sequence Flag"); //0=NTSC, 1=PAL
    Skip_SB(                                                    "Zero");
    Skip_S1(6,                                                  "Reserved");

    //4
    Skip_S1(5,                                                  "Reserved");
    Get_S1 (3, APT,                                             "APT"); //Track application ID, 0=4:2:0, 1=not 4:2:0

    //5
    Get_SB (   TF1,                                             "TF1 - Audio data is not valid");
    Skip_S1(4,                                                  "Reserved");
    Skip_S1(3,                                                  "AP1 - Audio application ID");

    //6
    Get_SB (  TF2,                                              "TF2 - Video data is not valid");
    Skip_S1(4,                                                  "Reserved");
    Skip_S1(3,                                                  "AP2 - Video application ID");

    //7
    Get_SB (  TF3,                                              "TF3 - Subcode is not valid");
    Skip_S1(4,                                                  "Reserved");
    Skip_S1(3,                                                  "AP3 - Subcode application ID");

    //8-79
    BS_End();
    Skip_XX(72,                                                 "Reserved"); //Should be filled with 0xFF

    FILLING_BEGIN();
        DSF_IsValid=true;
        Dseq_Old=DSF?11:9;
        FrameSize_Theory=(DSF?12:10)*150*80; //12 DIF sequences for PAL, 10 for NTSC

        if (TF1 && TF2)
        {
            //This is not logic, the header says no audio and no video! We do not trust the header, resetting all
            TF1=false;
            TF2=false;
            TF3=false;
        }

        FrameCount++;
        if (Count_Get(Stream_General)==0)
            Stream_Prepare(Stream_General);
        if (!IsAccepted && FrameCount>=10)
            Accept("DV DIF");
        if (!IsFilled && FrameCount>=Frame_Count_Valid)
            Header_Fill();
    FILLING_END();
}
#else //MEDIAINFO_MINIMIZESIZE
{
    if (Element_Size<77)
    {
        Trusted_IsNot("Size is wrong");
        return;
    }

    DSF=(Buffer[Buffer_Offset  ]&0x80)?true:false;
    APT=(Buffer[Buffer_Offset+1]&0x07);
    TF1=(Buffer[Buffer_Offset+2]&0x80)?true:false;
    TF2=(Buffer[Buffer_Offset+3]&0x80)?true:false;
    TF3=(Buffer[Buffer_Offset+4]&0x80)?true:false;

    FILLING_BEGIN();
        DSF_IsValid=true;
        Dseq_Old=DSF?11:9;
        FrameSize_Theory=(DSF?12:10)*150*80; //12 DIF sequences for PAL, 10 for NTSC

        if (TF1 && TF2)
        {
            //This is not logic, the header says no audio and no video! We do not trust the header, resetting all
            TF1=false;
            TF2=false;
            TF3=false;
        }

        FrameCount++;
        if (Count_Get(Stream_General)==0)
            Stream_Prepare(Stream_General);
        if (!IsAccepted && FrameCount>=10)
            Accept("DV DIF");
        if (!IsFilled && FrameCount>=Frame_Count_Valid)
            Header_Fill();
    FILLING_END();
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File_DvDif::Header_Fill()
{
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "Digital Video");

    IsFilled=true;
    #ifndef MEDIAINFO_DVDIF_ANALYZE_YES
        Finish("DV DIF");
    #endif //MEDIAINFO_DVDIF_ANALYZE_YES
}

//---------------------------------------------------------------------------
void File_DvDif::Subcode()
#ifndef MEDIAINFO_MINIMIZESIZE
{
    //Present?
    if (TF3)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    //Parsing
    for (int8u syb_num=0; syb_num<6; syb_num++)
        Subcode_Ssyb(syb_num);
    Skip_XX(29,                                                 "Unused");
}
#else //MEDIAINFO_MINIMIZESIZE
{
    if (TF3)
        return;

    for (int8u syb_num=0; syb_num<6; syb_num++)
        Subcode_Ssyb(syb_num);
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File_DvDif::Subcode_Ssyb(int8u syb_num)
{
    Element_Begin("ssyb");

    //Parsing
    BS_Begin();
    //ID0-ID1
    Skip_SB(                                                    "FR - Identification of half of channel"); //1=first half, 0=second
    if (syb_num==0)
        Skip_S1( 3,                                             "AP3 - Subcode application ID");
    else if (DBN==1 && syb_num==5)
        Skip_S1(3,                                              "APT - track application ID");
    else
        Skip_S1(3,                                              "Res - Reserved");
    Skip_S1(8,                                                  "Arb - Arbitrary bits");
    Skip_S1(4,                                                  "Syb - SSYSB number");
    BS_End();
    //FFh
    Skip_B1(                                                    "0xFF");
    //PC0-PC4
    Element();

    Element_End();
}

//---------------------------------------------------------------------------
void File_DvDif::VAUX()
{
    //Present?
    if (TF2)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    //Parsing
    for (int8u i=0; i<15; i++)
        Element();
    Skip_XX(2,                                                  "Unused");
}

//---------------------------------------------------------------------------
void File_DvDif::Audio()
{
    //Present?
    if (TF1)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    Element_Name("Audio");

    Element(); //First 5 bytes
    Skip_XX(Element_Size-Element_Offset,                        "Unknown");
}

//---------------------------------------------------------------------------
void File_DvDif::Video()
{
    //Present?
    if (TF2)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    Element_Name("Video");

    //Parsing
    BS_Begin();
    Skip_S1(4,                                                  "STA");
    Skip_S1(4,                                                  "QNO");
    BS_End();
    Skip_XX(Element_Size-Element_Offset,                        "Unknown");
}

//---------------------------------------------------------------------------
void File_DvDif::Element()
{
    Element_Begin();
    int8u PackType;
    if (AuxToAnalyze==0x00)
        Get_B1 (PackType,                                       "Pack Type");
    else
        PackType=AuxToAnalyze; //Forced by parser

    switch(PackType)
    {
        case 0x13 : timecode(); break;
        case 0x50 : audio_source(); break;
        case 0x51 : audio_sourcecontrol(); break;
        case 0x52 : audio_recdate(); break;
        case 0x53 : audio_rectime(); break;
        case 0x60 : video_source(); break;
        case 0x61 : video_sourcecontrol(); break;
        case 0x62 : video_recdate(); break;
        case 0x63 : video_rectime(); break;
        case 0x65 : closed_captions(); break;
        case 0x70 : consumer_camera_1(); break;
        case 0x71 : consumer_camera_2(); break;
        case 0xFF : Element_Name(Ztring().From_Number(PackType, 16));
                    Skip_B4(                                    "Unused"); break;
        default   : Element_Name(Ztring().From_Number(PackType, 16));
                    Skip_B4(                                    "Unknown");
    }
    Element_End();
}

//---------------------------------------------------------------------------
void File_DvDif::timecode()
{
    Element_Name("timecode");

    if (Buffer[Buffer_Offset+(size_t)Element_Offset  ]==0x00
     && Buffer[Buffer_Offset+(size_t)Element_Offset+1]==0x00
     && Buffer[Buffer_Offset+(size_t)Element_Offset+2]==0x00
     && Buffer[Buffer_Offset+(size_t)Element_Offset+3]==0x00
    )
    {
        Skip_XX(4,                                              "All zero");
        return;
    }

    //PArsing
    int8u Temp;
    int64u MilliSeconds=0;
    int8u  Frames=0;
    bool   DropFrame=false;
    BS_Begin();
    Skip_SB(                                                    "CF - Color fame");
    if (!DSF_IsValid)
        Skip_SB(                                                "Arbitrary bit or DP");
    else if (DSF)    //625/50
        Skip_SB(                                                "Arbitrary bit");
    else        //525/60
        Get_SB (DropFrame,                                      "DP - Drop frame"); //525/60
    Get_S1 (2, Temp,                                            "Frames (Tens)");
    Frames+=Temp*10;
    Get_S1 (4, Temp,                                            "Frames (Units)");
    Frames+=Temp;

    if (!DSF_IsValid)
        Skip_SB(                                                "BGF0 or PC");
    else if (DSF)    //625/50
        Skip_SB(                                                "BGF0 - Binary group flag");
    else        //525/60
        Skip_SB(                                                "PC - Biphase mark polarity correction"); //0=even; 1=odd
    Get_S1 (3, Temp,                                            "Seconds (Tens)");
    MilliSeconds+=Temp*10*1000;
    Get_S1 (4, Temp,                                            "Seconds (Units)");
    MilliSeconds+=Temp*1000;

    if (!DSF_IsValid)
        Skip_SB(                                                "BGF2 or BGF0");
    else if (DSF)    //625/50
        Skip_SB(                                                "BGF2 - Binary group flag");
    else        //525/60
        Skip_SB(                                                "BGF0 - Binary group flag");
    Get_S1 (3, Temp,                                            "Minutes (Tens)");
    MilliSeconds+=Temp*10*60*1000;
    Get_S1 (4, Temp,                                            "Minutes (Units)");
    MilliSeconds+=Temp*60*1000;

    if (!DSF_IsValid)
        Skip_SB(                                                "PC or BGF1");
    else if (DSF)    //625/50
        Skip_SB(                                                "PC - Biphase mark polarity correction"); //0=even; 1=odd
    else        //525/60
        Skip_SB(                                                "BGF1 - Binary group flag");
    Skip_SB(                                                    "BGF2 - Binary group flag");
    Get_S1 (2, Temp,                                            "Hours (Tens)");
    MilliSeconds+=Temp*10*60*60*1000;
    Get_S1 (4, Temp,                                            "Hours (Units)");
    MilliSeconds+=Temp*60*60*1000;
    Element_Info(Ztring().Duration_From_Milliseconds(MilliSeconds+((DSF_IsValid && Frames!=45)?((int64u)(Frames/(DSF?25.000:29.970)*1000)):0)));
    BS_End();

    if (TimeCode_First==(int64u)-1 && MilliSeconds!=167185000) //if all bits are set to 1, this is not a valid timestamp
    {
        TimeCode_First=MilliSeconds;
        if (DSF_IsValid && Frames!=45) //all bits are set to 1
            TimeCode_First+=(int64u)(Frames/(DSF?25.000:29.970)*1000);
    }
}

//---------------------------------------------------------------------------
void File_DvDif::audio_source()
{
    if (TF1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_source");

    int8u stype, SamplingRate, Resolution;
    BS_Begin();
    //PC1
    Skip_SB(                                                    "LF - Locked mode");
    Skip_SB(                                                    "Reserved");
    Skip_S1(6,                                                  "AF - Samples in this frame");

    //PC2
    Info_S1(1, StereoMode,                                      "SM - Stereo mode"); Param_Info(Dv_StereoMode[StereoMode]);
    Info_S1(2, ChannelsPerBlock,                                "CHN - Channels per block"); Param_Info(Dv_ChannelsPerBlock[ChannelsPerBlock]);
    Info_S1(1, Pair,                                            "PA - Pair"); Param_Info(Dv_Pair[Pair]);
    Skip_S1(4,                                                  "AM - Audio mode");

    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "ML - Multi-language");
    Skip_SB(                                                    "50/60");
    Get_S1 (5, stype,                                           "STYPE - audio blocks per video frame"); Param_Info(stype==0?"2 channels":(stype==2?"4 channels":"Unknown")); //0=25 Mbps, 2=50 Mbps

    Skip_SB(                                                    "EF - Emphasis off");
    Skip_SB(                                                    "TC - Time constant of emphasis");
    Get_S1 (3, SamplingRate,                                    "SMP - Sampling rate"); Param_Info(Dv_Audio_SamplingRate[SamplingRate]);
    Get_S1 (3, Resolution,                                      "QU - Resolution"); Param_Info(Dv_Audio_Resolution[Resolution]);
    BS_End();

    FILLING_BEGIN();
        if (!IgnoreAudio && (FrameCount==1 || AuxToAnalyze)) //Only the first time
        {
            Stream_Prepare(Stream_Audio);
            Fill(Stream_Audio, 0, Audio_ID, 0);
            Fill(Stream_Audio, 0, Audio_Format, "PCM");
            Fill(Stream_Audio, 0, Audio_Codec, "PCM");
            Fill(Stream_Audio, 0, Audio_Channel_s_, 2);
            Fill(Stream_Audio, 0, Audio_SamplingRate, Dv_Audio_SamplingRate[SamplingRate]);
            Fill(Stream_Audio, 0, Audio_Resolution, Dv_Audio_Resolution[Resolution]);
            Fill(Stream_Audio, 0, Audio_BitRate, 2*Dv_Audio_SamplingRate[SamplingRate]*Dv_Audio_Resolution[Resolution]);

            if (stype==2 || (Resolution==1 && SamplingRate==2)) //stype=? or (Resolution=12 bits and SamplingRate=32 KHz)
            {
                Stream_Prepare(Stream_Audio);
                Fill(Stream_Audio, 1, Audio_ID, 1);
                Fill(Stream_Audio, 1, Audio_Format, "PCM");
                Fill(Stream_Audio, 1, Audio_Codec, "PCM");
                Fill(Stream_Audio, 1, Audio_Channel_s_, 2);
                Fill(Stream_Audio, 1, Audio_SamplingRate, Dv_Audio_SamplingRate[SamplingRate]);
                Fill(Stream_Audio, 1, Audio_Resolution, Dv_Audio_Resolution[Resolution]);
                Fill(Stream_Audio, 1, Audio_BitRate, 2*Dv_Audio_SamplingRate[SamplingRate]*Dv_Audio_Resolution[Resolution]);
            }
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_sourcecontrol()
{
    if (TF1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_control");

    BS_Begin();

    //PC1
    Info_S1(2, CopyGenerationManagementSystem,                  "CGMS - Copy generation management system"); Param_Info(Dv_CopyGenerationManagementSystem[CopyGenerationManagementSystem]);
    Info_S1(2, InputType,                                       "ISR - Input type"); Param_Info(Dv_InputType[InputType]);
    Info_S1(2, CompressionTimes,                                "CMP - Compression times"); Param_Info(Dv_CompressionTimes[CompressionTimes]);
    Info_S1(2, Emphasis,                                        "EFC - Emphasis"); Param_Info(Dv_Emphasis[Emphasis]);

    //PC2
    Skip_SB(                                                    "REC S Non-recording start point");
    Skip_SB(                                                    "REC E - Non-recording end point");
    Skip_SB(                                                    "FADE S - Recording mode"); //1=Original
    Skip_SB(                                                    "FADE E - Unknown");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");

    //PC3
    Skip_SB(                                                    "DRF - Direction"); //1=Forward
    Skip_S1(7,                                                  "SPD - Speed");

    //PC4
    Skip_SB(                                                    "Reserved");
    Skip_S1(7,                                                  "GEN - Category");

    BS_End();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_recdate()
{
    if (TF1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_recdate");

    recdate();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_rectime()
{
    if (TF1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_rectime");

    rectime();
}

//---------------------------------------------------------------------------
void File_DvDif::video_source()
{
    if (TF2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_source");

    int8u stype;
    bool system;
    BS_Begin();
    //PC1
    Skip_S1(4,                                                  "TVCH (tens of units, 0–9)");
    Skip_S1(4,                                                  "TVCH (units, 0–9)");

    //PC2
    Skip_SB(                                                    "B/W - Black and White"); //0=Black and White, 1=Color
    Skip_SB(                                                    "EN - Color Frames is not valid");
    Skip_S1(2,                                                  "CLF - Color frames id");
    Skip_S1(4,                                                  "TVCH (hundreds of units, 0–9)");

    //PC3
    Skip_S1(2,                                                  "SRC");
    Get_SB (   system,                                          "50/60 - System");
    Get_S1 (4, stype,                                           "STYPE - Signal type of video signal"); //0=not 4:2:2, 4=4:2:2

    //PC4
    BS_End();
    Skip_B1(                                                    "TUN/VISC");

    FILLING_BEGIN();
        if ((FrameCount==1 || AuxToAnalyze) && Count_Get(Stream_Video)==0) //Only the first time
        {
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, 0, Video_Format, "Digital Video");
            Fill(Stream_Video, 0, Video_Codec, "DV");
            Fill(Stream_Video, 0, Video_Standard, DSF?"PAL":"NTSC");
            Fill(Stream_Video, 0, Video_Width, 720);
            Fill(Stream_Video, 0, Video_Height, DSF?576:480);
            Fill(Stream_Video, 0, Video_FrameRate, system?25.000:29.970);
            Fill(Stream_Video, 0, Video_FrameRate_Mode, "CFR");

            if (DSF==false && stype==4) //NTSC and 4:2:2
                Fill(Stream_Video, 0, Video_Colorimetry, "4:2:2");       //NTSC 50 Mbps
            else if (DSF==false) //NTSC and not 4:2:2 (--> 4:1:1)
                Fill(Stream_Video, 0, Video_Colorimetry, "4:1:1");       //NTSC 25 Mbps
            else if (stype==4) //PAL and 4:2:2
                Fill(Stream_Video, 0, Video_Colorimetry, "4:2:2");       //PAL  50 Mbps
            else if (APT==0) //PAL and 4:2:0
                Fill(Stream_Video, 0, Video_Colorimetry, "4:2:0");       //PAL  25 Mbps 4:2:0
            else if (APT==1) //PAL and not 4:2:0 (--> 4:1:1)
                Fill(Stream_Video, 0, Video_Colorimetry, "4:1:1");       //PAL  25 Mbps 4:1:1

            if (FrameSize_Theory)
                Duration=(int64u)(File_Size*1000/(FrameSize_Theory*(DSF?25.000:29.970)));
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::video_sourcecontrol()
{
    if (TF2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_control");

    BS_Begin();
    //PC1
    Info_S1(2, CopyGenerationManagementSystem,                  "CGMS - Copy generation management system"); Param_Info(Dv_CopyGenerationManagementSystem[CopyGenerationManagementSystem]);
    Skip_S1(2,                                                  "ISR");
    Skip_S1(2,                                                  "CMP");
    Skip_S2(2,                                                  "SS");

    //PC2
    Skip_SB(                                                    "REC S");
    Skip_SB(                                                    "Reserved");
    Skip_S1(2,                                                  "REC M");
    Skip_SB(                                                    "Reserved");
    Get_S1 (3, aspect,                                          "DISP - Aspect ratio"); Param_Info(Dv_Disp[aspect]);

    //PC3
    Skip_SB(                                                    "FF - Frame/Field"); //1=Frame, 0=Field
    Skip_SB(                                                    "FS - First/second field"); //0=Field 2, 1=Field 1, if FF=0 x is output twice, if FF=1, Field x fisrst, other second
    Skip_SB(                                                    "FC - Frame Change"); //0=Same picture as before
    Get_SB (   Interlaced,                                      "IL - Interlaced"); //1=Interlaced
    Skip_SB(                                                    "SF");
    Skip_SB(                                                    "SC");
    Skip_S1(2,                                                  "BCS");

    //PC4
    Skip_SB(                                                    "Reserved");
    Skip_S1(7,                                                  "GEN - Category");

    BS_End();

    FILLING_BEGIN();
        if ((FrameCount==1 || AuxToAnalyze) && Retrieve(Stream_Video, 0, Video_ScanType).empty()) //Only the first time
        {
            Fill(Stream_Video, 0, Video_ScanType, Interlaced?"Interlaced":"Progressive");
            Fill(Stream_Video, 0, Video_Interlacement, Interlaced?"Interlaced":"PFF");
            switch (aspect)
            {
                case 0 : Fill(Stream_Video, 0, Video_DisplayAspectRatio, 4.0/3.0); break;
                case 2 :
                case 7 : Fill(Stream_Video, 0, Video_DisplayAspectRatio, 16.0/9.0); break;
                default: ;
            }
        }

        if (!IsAccepted && AuxToAnalyze)
            Header_Fill();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::video_recdate()
{
    if (TF2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_recdate");

    Ztring Date=recdate();
    if (Recorded_Date_Date.empty())
        Recorded_Date_Date=Date;
}

//---------------------------------------------------------------------------
void File_DvDif::video_rectime()
{
    if (TF2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_rectime");

    Ztring Date=rectime();
    if (Recorded_Date_Time.empty())
        Recorded_Date_Time=Date;
}

//---------------------------------------------------------------------------
void File_DvDif::closed_captions()
{
    Element_Name("closed_captions");

    #if defined(MEDIAINFO_EIA608_YES)
        if (CC_Parsers.empty())
        {
            CC_Parsers.resize(2);
            for (size_t Pos=0; Pos<2; Pos++)
                CC_Parsers[Pos]=new File_Eia608();
            Frame_Count_Valid*=10; //More frames
        }
        if (Dseq==0) //CC are duplicated for each DIF sequence!
        {
            for (size_t Pos=0; Pos<2; Pos++)
            {
                Open_Buffer_Init(CC_Parsers[Pos]);
                Open_Buffer_Continue(CC_Parsers[Pos], Buffer+Buffer_Offset+(size_t)Element_Offset, 2);
                Element_Offset+=2;
            }
        }

    #else
        Skip_XX(4,                                              "Captions");
    #endif
}

//---------------------------------------------------------------------------
void File_DvDif::consumer_camera_1()
{
    Element_Name("consumer_camera_1");

    //Parsing
    BS_Begin();
    Mark_1_NoTrustError();
    Mark_1_NoTrustError();
    Skip_S1(6,                                                  "iris");
    Info_S1(4, ae_mode,                                         "ae mode"); Param_Info(Dv_consumer_camera_1_ae_mode[ae_mode]);
    Skip_S1(4,                                                  "agc(Automatic Gain Control)");
    Info_S1(3, wb_mode,                                         "wb mode (white balance mode)"); Param_Info(Dv_consumer_camera_1_wb_mode[wb_mode]);
    Info_S1(5, white_balance,                                   "white balance"); Param_Info(Dv_consumer_camera_1_white_balance(white_balance));
    Info_S1(1, fcm,                                             "fcm (Focus mode)"); Param_Info(Dv_consumer_camera_1_fcm[fcm]);
    Skip_S1(7,                                                  "focus (focal point)");
    BS_End();

    if (!consumer_camera_1_Parsed)
    {
        if (ae_mode!=0x0F) Fill(Stream_Video, 0, Video_Encoded_Library_Settings, _T("ae mode=")+Ztring(Dv_consumer_camera_1_ae_mode[ae_mode]));
        if (wb_mode!=0x08) Fill(Stream_Video, 0, Video_Encoded_Library_Settings, _T("wb mode=")+Ztring(Dv_consumer_camera_1_wb_mode[wb_mode]));
        if (wb_mode!=0x1F) Fill(Stream_Video, 0, Video_Encoded_Library_Settings, _T("white balance=")+Ztring(Dv_consumer_camera_1_white_balance(white_balance)));
                           Fill(Stream_Video, 0, Video_Encoded_Library_Settings, _T("fcm=")+Ztring(Dv_consumer_camera_1_fcm[fcm]));
        consumer_camera_1_Parsed=true;
    }
}

//---------------------------------------------------------------------------
void File_DvDif::consumer_camera_2()
{
    Element_Name("consumer_camera_2");

    //Parsing
    BS_Begin();
    Mark_1_NoTrustError();
    Mark_1_NoTrustError();
    Skip_S1(1,                                                  "vpd");
    Skip_S1(5,                                                  "vertical panning speed");
    Skip_S1(1,                                                  "is");
    Skip_S1(1,                                                  "hpd");
    Skip_S1(6,                                                  "horizontal panning speed");
    Skip_S1(8,                                                  "focal length");
    Skip_S1(1,                                                  "zen");
    Info_S1(3, zoom_U,                                          "units of e-zoom");
    Info_S1(4, zoom_D,                                          "1/10 of e-zoom"); if (zoom_D!=0xF) Param_Info(_T("zoom=")+Ztring().From_Number(zoom_U+((float32)zoom_U)/10, 2));
    BS_End();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
Ztring File_DvDif::recdate()
{
    BS_Begin();

    int8u Temp;
    int16u Year=0;
    int8u  Month=0, Day=0;
    Skip_S1(8,                                                  "Time zone specific"); //ds, tm, tens of time zone, units of time zone, 0xFF for Unknwon
    Skip_SB(                                                    "1");
    Skip_SB(                                                    "1");
    Get_S1 (2, Temp,                                            "Days (Tens)");
    Day+=Temp*10;
    Get_S1 (4, Temp,                                            "Days (Units)");
    Day+=Temp;
    Skip_SB(                                                    "1");
    Skip_SB(                                                    "1");
    Skip_SB(                                                    "1");
    Get_S1 (1, Temp,                                            "Month (Tens)");
    Month+=Temp*10;
    Get_S1 (4, Temp,                                            "Month (Units)");
    Month+=Temp;
    Get_S1 (4, Temp,                                            "Year (Tens)");
    Year+=Temp*10;
    Get_S1 (4, Temp,                                            "Year (Units)");
    Year+=Temp;
    Year+=Year<25?2000:1900;
    Element_Info(Ztring::ToZtring(Year)+_T("-")+Ztring::ToZtring(Month)+_T("-")+Ztring::ToZtring(Day));

    BS_End();

    if (Month>12 || Day>31)
        return Ztring(); //If all bits are set to 1, this is invalid
    Ztring MonthString;
    if (Month<10)
        MonthString=_T("0");
    MonthString+=Ztring::ToZtring(Month);
    Ztring DayString;
    if (Day<10)
        DayString=_T("0");
    DayString+=Ztring::ToZtring(Day);
    return Ztring::ToZtring(Year)+_T("-")+MonthString+_T("-")+DayString;
}

//---------------------------------------------------------------------------
Ztring File_DvDif::rectime()
{
    if (!DSF_IsValid)
    {
        Trusted_IsNot("Not in right order");
        return Ztring();
    }

    BS_Begin();

    if (Buffer[Buffer_Offset+(size_t)Element_Offset  ]==0x00
     && Buffer[Buffer_Offset+(size_t)Element_Offset+1]==0x00
     && Buffer[Buffer_Offset+(size_t)Element_Offset+2]==0x00
     && Buffer[Buffer_Offset+(size_t)Element_Offset+3]==0x00
    )
    {
        Skip_XX(4,                                              "All zero");
        return Ztring();
    }

    int8u Temp;
    int64u Time=0;
    int8u Frames=0;
    Skip_SB(                                                    "Unknown");
    Skip_SB(                                                    "1");
    Get_S1 (2, Temp,                                            "Frames (Tens)");
    Frames+=Temp*10;
    Get_S1 (4, Temp,                                            "Frames (Units)");
    Frames+=Temp;
    if (Temp!=0xF && DSF_IsValid)
        Time+=(int64u)(Frames/(DSF?25.000:29.970));
    Skip_SB(                                                    "1");
    Get_S1 (3, Temp,                                            "Seconds (Tens)");
    Time+=Temp*10*1000;
    Get_S1 (4, Temp,                                            "Seconds (Units)");
    Time+=Temp*1000;
    Skip_SB(                                                    "1");
    Get_S1 (3, Temp,                                            "Minutes (Tens)");
    Time+=Temp*10*60*1000;
    Get_S1 (4, Temp,                                            "Minutes (Units)");
    Time+=Temp*60*1000;
    Skip_SB(                                                    "1");
    Skip_SB(                                                    "1");
    Get_S1 (2, Temp,                                            "Hours (Tens)");
    Time+=Temp*10*60*60*1000;
    Get_S1 (4, Temp,                                            "Hours (Units)");
    Time+=Temp*60*60*1000;
    Element_Info(Ztring().Duration_From_Milliseconds(Time));

    BS_End();

    if (Time!=167185000)
        return Ztring().Duration_From_Milliseconds(Time);
    else
        return Ztring(); //If all bits are set to 1, this is invalid
}

} //NameSpace

#endif //MEDIAINFO_DV_YES

