//---------------------------------------------------------------------------

#ifndef Main_UtilsH
#define Main_UtilsH
//---------------------------------------------------------------------------



#include <ZenLib/ZtringListList.h>

void Codecs_Enumerate(ZenLib::ZtringListList &Video, ZenLib::ZtringListList &Audio);
void Debug_Header_Create(const ZenLib::Ztring &FileName, void* Handle=NULL);

#endif
