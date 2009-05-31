// File_DvDif - Info for DVD objects (IFO) files
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
//
// Information about DV-DIF (Digital Video Digital Interface Format)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_DvDifH
#define MediaInfo_File_DvDifH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_DvDif
//***************************************************************************

class File_DvDif : public File__Analyze
{
public :
    //In
    size_t Frame_Count_Valid;
    int8u  AuxToAnalyze; //Only Aux must be parsed
    bool   IgnoreAudio;

protected :
    //Buffer - Global
    #ifdef MEDIAINFO_DVDIF_ANALYZE_YES
    void Read_Buffer_Continue();
    #endif //MEDIAINFO_DVDIF_ANALYZE_YES
    void Read_Buffer_Finalize ();

public :
    File_DvDif();
    ~File_DvDif();

private :
    //Buffer
    void Header_Parse();
    void Data_Parse();

    //Elements - Main
    void Header();
    void Header_Fill();
    void Subcode();
    void Subcode_Ssyb(int8u syb_num);
    void VAUX();
    void Audio();
    void Video();

    //Elements - Sub
    void Element();
    void timecode();
    void audio_source();
    void audio_sourcecontrol();
    void audio_recdate();
    void audio_rectime();
    void video_source();
    void video_sourcecontrol();
    void video_recdate();
    void video_rectime();
    void closed_captions();
    void consumer_camera_1();
    void consumer_camera_2();

    //Helpers
    Ztring recdate();
    Ztring rectime();

    //Temp
    #if defined(MEDIAINFO_EIA608_YES)
        std::vector<File__Analyze*> CC_Parsers;
    #endif
    Ztring Recorded_Date_Date;
    Ztring Recorded_Date_Time;
    size_t FrameCount;
    size_t FrameSize_Theory; //The size of a frame
    int64u Duration;
    int64u TimeCode_First;
    int8u  SCT;
    int8u  SCT_Old;
    int8u  Dseq;
    int8u  Dseq_Old;
    int8u  DBN;
    int8u  DBN_Olds[8];
    int8u  FSC;
    bool   consumer_camera_1_Parsed;
    bool   consumer_camera_2_Parsed;
    bool   DSF;
    bool   DSF_IsValid;
    int8u  APT;
    bool   TF1;
    bool   TF2;
    bool   TF3;
    int8u  aspect;
    bool   Interlaced;

    #ifdef MEDIAINFO_DVDIF_ANALYZE_YES
    void Errors_Stats_Update();
    void Errors_Stats_Update_Finnish();
    Ztring Errors_Stats;
    Ztring Date;
    Ztring Time;
    int64u Speed_FrameCount;                            //Global    - Total
    int64u Speed_FrameCount_Video_STA_Errors;           //Global    - Error 1
    std::vector<int64u> Speed_FrameCount_Audio_Errors;  //Global    - Error 2
    int64u Speed_FrameCount_Timecode_Incoherency;       //Global    - Error 3
    int64u Speed_FrameCount_Contains_NULL;              //Global    - Error 4
    int64u Speed_Contains_NULL;                         //Per Frame - Error 4
    int8u  QU;
    bool   QU_FSC; //Validity is with QU
    bool   QU_System; //Validity is with QU
    bool   REC_ST;
    bool   REC_END;
    bool   REC_IsValid;
    bool   System;
    bool   System_IsValid;
    bool   Frame_AtLeast1DIF;
    struct dvdate
    {
        int8u  Days;
        int8u  Months;
        int8u  Years;
        bool   MultipleValues;
        bool   IsValid;

        dvdate() {Clear();}

        void Clear()
        {
            MultipleValues=false;
            IsValid=false;
        }
    };
    struct dvtime
    {
        int8u  Frames;
        int8u  Seconds;
        int8u  Minutes;
        int8u  Hours;
        bool   DropFrame;
        bool   MultipleValues;
        bool   IsValid;

        dvtime() {Clear();}

        void Clear()
        {
            MultipleValues=false;
            IsValid=false;
        }
    };
    dvtime Speed_TimeCode_Last;
    dvtime Speed_TimeCode_Current;
    dvtime Speed_TimeCode_Current_Theory;
    Ztring Speed_TimeCodeZ_First;
    Ztring Speed_TimeCodeZ_Last;
    Ztring Speed_TimeCodeZ_Current;
    dvtime Speed_RecTime_Current;
    dvtime Speed_RecTime_Current_Theory;
    dvtime Speed_RecTime_Current_Theory2;
    Ztring Speed_RecTimeZ_First;
    Ztring Speed_RecTimeZ_Last;
    Ztring Speed_RecTimeZ_Current;
    dvdate Speed_RecDate_Current;
    Ztring Speed_RecDateZ_First;
    Ztring Speed_RecDateZ_Last;
    Ztring Speed_RecDateZ_Current;
    std::vector<size_t> Video_STA_Errors; //Per STA type
    std::vector<size_t> Video_STA_Errors_Total; //Per STA type
    std::vector<size_t> Audio_Errors; //Per Dseq
    std::vector<size_t> audio_source_IsPresent;
    std::vector<bool>   CH_IsPresent;
    std::vector<std::vector<size_t> > Audio_Errors_Total; //Per Channel and Dseq
    std::vector<std::vector<size_t> > Audio_Invalids; //Per Channel and Dseq
    std::vector<std::vector<size_t> > Audio_Invalids_Total; //Per Channel and Dseq
    struct recZ_Single
    {
        int64u FramePos;
        Ztring Date;
        Ztring Time;

        recZ_Single()
        {
            FramePos=(int64u)-1;
        }
    };
    struct recZ
    {
        recZ_Single First;
        recZ_Single Last;
    };
    std::vector<recZ> Speed_RecZ;
    struct timeCodeZ_Single
    {
        int64u FramePos;
        Ztring TimeCode;

        timeCodeZ_Single()
        {
            FramePos=(int64u)-1;
        }
    };
    struct timeCodeZ
    {
        timeCodeZ_Single First;
        timeCodeZ_Single Last;
    };
    std::vector<timeCodeZ> Speed_TimeCodeZ;
    #endif //MEDIAINFO_DVDIF_ANALYZE_YES
};

} //NameSpace

#endif
