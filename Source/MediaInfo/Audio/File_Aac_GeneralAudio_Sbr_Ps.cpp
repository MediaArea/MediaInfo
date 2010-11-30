// File_Aac - Info for AAC (Raw) files
// Copyright (C) 2008-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
#if defined(MEDIAINFO_AAC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Aac.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
void File_Aac::ps_data(size_t End)
{
    //Parsing
    Element_Begin("ps_data");
    bool enable_ps_header;
    Get_SB(enable_ps_header,                                    "enable_ps_header");
    if (enable_ps_header)
    {
        //Init
        delete ps; ps=new ps_handler();

        Get_SB(ps->enable_iid,                                  "enable_iid");
        if (ps->enable_iid)
        {
            Get_S1 (3, ps->iid_mode,                            "iid_mode");
        }
        Get_SB(ps->enable_icc,                                  "enable_icc");
        if (ps->enable_icc)
        {
            Get_S1 (3, ps->icc_mode,                            "icc_mode");
        }
        Get_SB(ps->enable_ext,                                  "enable_ext");
    }

    if (ps==NULL)
    {
        if (Data_BS_Remain()>End)
            Skip_BS(Data_BS_Remain()-End,                       "(Waiting for header)");
        Element_End();
        return;
    }

    //PS not yet parsed
    if (Data_BS_Remain()>End)
        Skip_BS(Data_BS_Remain()-End,                           "Data");
    Element_End();
}

} //NameSpace

#endif //MEDIAINFO_AAC_YES

