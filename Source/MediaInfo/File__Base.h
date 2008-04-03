// File__Base - Base for other files
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
// Give common methods for all file types
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo__BaseH
#define MediaInfo__BaseH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Config.h"
#include <ZenLib/ZtringListList.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

class MediaInfo_Config_MediaInfo;

//***************************************************************************
// Class File__Base
//***************************************************************************

class File__Base
{
public :
    //Constructor/Destructor
    File__Base();
    virtual ~File__Base();

    //Save
    int     Save ();

    //Get information
    Ztring  Inform ();
    Ztring  Inform (stream_t StreamKind, size_t StreamNumber=0); //All about only a specific stream
    const Ztring &Get (stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t KindOfInfo=Info_Text);
    const Ztring &Get (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, info_t KindOfInfo=Info_Text, info_t KindOfSearch=Info_Name);

    //Set information
    int           Set (stream_t StreamKind, size_t StreamNumber, size_t Parameter, const Ztring &ToSet, const Ztring &OldValue=_T(""));
    int           Set (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, const Ztring &ToSet, const Ztring &OldValue=_T(""));

    //Output buffer
    virtual size_t Output_Buffer_Get (const String &) {return 0;};
    virtual size_t Output_Buffer_Get (size_t) {return 0;};

    //Options
    size_t Count_Get (stream_t StreamKind, size_t Pos=Error) const;
    void   Language_Set (); //Update language for an existing File__Base

    //Infos
           ZtringListList Info_Capacities ();
    static ZtringListList Info_Capacities_Parameters ();

protected :
    //Read
    virtual void Read_File            (); //To overload
    virtual void Read_Buffer_Init     (); //To overload
    virtual void Read_Buffer_Unsynched(); //To overload
    virtual void Read_Buffer_Continue (); //To overload
    virtual void Read_Buffer_Finalize (); //To overload

    //Write
    virtual int Write       (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, const Ztring &ToSet, const Ztring &OldValue); //Write the value in memory
    virtual int WriteToDisk (); //Write modified tags on disk

    //How to
    virtual void HowTo (stream_t StreamType);

    //Arrays
    std::vector<ZtringListList> General;
    std::vector<ZtringListList> Video;
    std::vector<ZtringListList> Audio;
    std::vector<ZtringListList> Text;
    std::vector<ZtringListList> Chapters;
    std::vector<ZtringListList> Image;
    std::vector<ZtringListList> Menu;
    std::vector<ZtringListList>* Stream[Stream_Max]; //pointer to others listed streams

    //Details
    Ztring Details;

    //Demux
    void Demux (const int8u* Buffer, size_t Buffer_Size, const Ztring& StreamName, bool Final=true);

    //Debug
    bool   Synched;
    size_t Trusted;

protected :
    std::vector<bool> Optimized[Stream_Max]; //If true, Arrays are not fully filled
    stream_t StreamKind_Last;
    size_t   StreamPos_Last;

public :
    void   Details_Clear();
    void   Details_Add(const char* Parameter);

    //File
    Ztring File_Name;
    int64u File_Size;
    int64u File_Offset;
    int64u File_GoTo; //How many byte to skip?
    int64u File_MaximumOffset;

    //Divers
    void Clear();

public :  //A virer
    void Traiter(Ztring &C); //enleve les $if...
    friend class File__Analyze;
    MediaInfo_Config_MediaInfo* Config;
};

} //NameSpace

#endif
