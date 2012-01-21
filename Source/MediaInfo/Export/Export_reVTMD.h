// Export_reVTMD - reVTMD export
// Copyright (C) 2012-2012 U.S. National Archives
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the NARA OPEN SOURCE AGREEMENT VERSION 1.3
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Export_reVTMDH
#define Export_reVTMDH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Internal.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
/// @brief Export_reVTMD
//***************************************************************************

class Export_reVTMD
{
public :
    //Constructeur/Destructeur
    Export_reVTMD ();
    ~Export_reVTMD ();

    //Input
    Ztring Transform(MediaInfo_Internal &MI);
};

} //NameSpace

#endif //Export_reVTMDH
