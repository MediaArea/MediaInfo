/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef Main_UtilsH
#define Main_UtilsH
//---------------------------------------------------------------------------



#include "ZenLib/ZtringListList.h"

void Codecs_Enumerate(ZenLib::ZtringListList &Video, ZenLib::ZtringListList &Audio);
void Debug_Header_Create(const ZenLib::Ztring &FileName, void* Handle=NULL);

#endif
