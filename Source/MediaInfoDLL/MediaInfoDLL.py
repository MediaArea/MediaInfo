## MediaInfoDLL - All info about media files, for DLL
#  Copyright (C) 2002-2007 Jerome Martinez, Zen@MediaArea.net
# 
#  This library is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  any later version.
# 
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library. If not, see <http://www.gnu.org/licenses/>.
# 
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 
#  Public DLL interface implementation
#  Wrapper for MediaInfo Library
#  Please see MediaInfo.h for help
# 
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#
#  Converted to python module by Petr Kaderabek
# Modifications by Jerome Martinez
#


from ctypes import *
#print windll.MediaInfo   
NULL = c_void_p(0)

### types [C 		Python]:
# size_t		c_uint
# unsigned char*	c_char_p
# enum			c_int       # recommended in ctypes documentation
# const wchar_t*	c_wchar_p,
### these functions need strings in unicode format 



#MEDIAINFO_EXP void*             __stdcall MediaInfo_New (); /*you must ALWAYS call MediaInfo_Delete(Handle) in order to free memory*/
#/** @brief A 'new' MediaInfo interface (with a quick init of useful options : "**VERSION**;**APP_NAME**;**APP_VERSION**", but without debug information, use it only if you know what you do), return a Handle, don't forget to delete it after using it*/
MediaInfo_New = windll.MediaInfo.MediaInfo_New
MediaInfo_New.argtypes = []  
MediaInfo_New.restype  = c_void_p

#MEDIAINFO_EXP void*             __stdcall MediaInfo_New_Quick (const wchar_t* File, const wchar_t* Options); /*you must ALWAYS call MediaInfo_Delete(Handle) in order to free memory*/
MediaInfo_New_Quick = windll.MediaInfo.MediaInfo_New_Quick
MediaInfo_New_Quick.argtypes = [c_wchar_p, c_wchar_p]
MediaInfo_New_Quick.restype  = c_void_p

#/** @brief Delete a MediaInfo interface*/
#MEDIAINFO_EXP void              __stdcall MediaInfo_Delete (void* Handle);
MediaInfo_Delete = windll.MediaInfo.MediaInfo_Delete
MediaInfo_Delete.argtypes = [c_void_p]

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Open (with a filename)*/
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_Open (void* Handle, const wchar_t* File);
MediaInfo_Open = windll.MediaInfo.MediaInfo_Open
MediaInfo_Open.argtype = [c_void_p, c_wchar_p]
MediaInfo_Open.restype = c_uint   # size_t bereme jako unsigned int (ma to taky 4 bajty a asi je to unsigned)

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Open (with a buffer) */
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_Open_Buffer (void* Handle, const unsigned char* Begin, size_t Begin_Size, const unsigned char* End, size_t End_Size); /*return Handle*/
MediaInfo_Open_Buffer = windll.MediaInfo.MediaInfo_Open_Buffer
MediaInfo_Open_Buffer.argtype = [c_void_p, c_char_p, c_uint, c_char_p, c_uint]  
MediaInfo_Open_Buffer.restype = c_uint   

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Save */
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_Save (void* Handle);
MediaInfo_Save = windll.MediaInfo.MediaInfo_Save
MediaInfo_Save.argtype = [c_void_p]
MediaInfo_Save.restype = c_uint   

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Close */
#MEDIAINFO_EXP void              __stdcall MediaInfo_Close (void* Handle);
MediaInfo_Close = windll.MediaInfo.MediaInfo_Close
MediaInfo_Close.argtype = [c_void_p]

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Inform */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_Inform (void* Handle, size_t Reserved); /*Default : Reserved=0*/
MediaInfo_Inform = windll.MediaInfo.MediaInfo_Inform
MediaInfo_Inform.argtype = [c_void_p, c_uint]
MediaInfo_Inform.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Get */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_GetI (void* Handle, MediaInfo_stream_C StreamKind, size_t StreamNumber, size_t Parameter, MediaInfo_info_C InfoKind); /*Default : InfoKind=Info_Text*/
MediaInfo_GetI = windll.MediaInfo.MediaInfo_GetI
MediaInfo_GetI.argtype = [c_void_p, c_int, c_uint, c_uint, c_int]
MediaInfo_GetI.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Get */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_Get (void* Handle, MediaInfo_stream_C StreamKind, size_t StreamNumber, const wchar_t* Parameter, MediaInfo_info_C InfoKind, MediaInfo_info_C SearchKind); /*Default : InfoKind=Info_Text, SearchKind=Info_Name*/
MediaInfo_Get = windll.MediaInfo.MediaInfo_Get
MediaInfo_Get.argtype = [c_void_p, c_int, c_uint, c_wchar_p, c_int, c_int]
MediaInfo_Get.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Set */
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_SetI (void* Handle, const wchar_t* ToSet, MediaInfo_stream_C StreamKind, size_t StreamNumber, size_t Parameter, const wchar_t* OldParameter);
MediaInfo_SetI = windll.MediaInfo.MediaInfo_SetI
MediaInfo_SetI.argtype = [c_void_p, c_wchar_p, c_int, c_uint, c_uint, c_wchar_p]
MediaInfo_SetI.restype = c_uint

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Set */
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_Set (void* Handle, const wchar_t* ToSet, MediaInfo_stream_C StreamKind, size_t StreamNumber, const wchar_t* Parameter, const wchar_t* OldParameter);
MediaInfo_Set = windll.MediaInfo.MediaInfo_Set
MediaInfo_Set.argtype = [c_void_p, c_wchar_p, c_int, c_uint, c_wchar_p, c_wchar_p]
MediaInfo_Set.restype = c_uint

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Option */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_Option (void* Handle, const wchar_t* Option, const wchar_t* Value);
MediaInfo_Option = windll.MediaInfo.MediaInfo_Option
MediaInfo_Option.argtype = [c_void_p, c_wchar_p, c_wchar_p]
MediaInfo_Option.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoLib::MediaInfo::State_Get */
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_State_Get (void* Handle);
MediaInfo_State_Get = windll.MediaInfo.MediaInfo_State_Get
MediaInfo_State_Get.argtype = [c_void_p]
MediaInfo_State_Get.restype = c_uint

#/** @brief Wrapper for MediaInfoLib::MediaInfo::Count_Get */
#MEDIAINFO_EXP size_t            __stdcall MediaInfo_Count_Get (void* Handle, MediaInfo_stream_C StreamKind, size_t StreamNumber); /*Default : StreamNumber=-1*/
MediaInfo_Count_Get = windll.MediaInfo.MediaInfo_Count_Get
MediaInfo_Count_Get.argtype = [c_void_p, c_int, c_uint]
MediaInfo_Count_Get.restype = c_uint





#/** @brief A 'new' MediaInfoList interface, return a Handle, don't forget to delete it after using it*/
#MEDIAINFO_EXP void*             __stdcall MediaInfoList_New (); /*you must ALWAYS call MediaInfoList_Delete(Handle) in order to free memory*/
MediaInfoList_New = windll.MediaInfo.MediaInfoList_New
MediaInfoList_New.argtype = []
MediaInfoList_New.restype = c_void_p

#/** @brief A 'new' MediaInfoList interface (with a quick init of useful options : "**VERSION**;**APP_NAME**;**APP_VERSION**", but without debug information, use it only if you know what you do), return a Handle, don't forget to delete it after using it*/
#MEDIAINFO_EXP void*             __stdcall MediaInfoList_New_Quick (const wchar_t* Files, const wchar_t* Config); /*you must ALWAYS call MediaInfoList_Delete(Handle) in order to free memory*/
MediaInfoList_New_Quick = windll.MediaInfo.MediaInfoList_New_Quick
MediaInfoList_New_Quick.argtype = [c_wchar_p, c_wchar_p]
MediaInfoList_New_Quick.restype = c_void_p

#/** @brief Delete a MediaInfoList interface*/
#MEDIAINFO_EXP void              __stdcall MediaInfoList_Delete (void* Handle);
MediaInfoList_Delete = windll.MediaInfo.MediaInfoList_Delete
MediaInfoList_Delete.argtype = [c_void_p]

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Open (with a filename)*/
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_Open (void* Handle, const wchar_t* Files, const MediaInfo_fileoptions_C Options); /*Default : Options=MediaInfo_FileOption_Nothing*/
MediaInfoList_Open = windll.MediaInfo.MediaInfoList_Open
MediaInfoList_Open.argtype = [c_void_p, c_wchar_p, c_int]
MediaInfoList_Open.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Open (with a buffer) */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_Open_Buffer (void* Handle, const unsigned char* Begin, size_t Begin_Size, const unsigned char* End, size_t End_Size); /*return Handle*/
MediaInfoList_Open_Buffer = windll.MediaInfo.MediaInfoList_Open_Buffer
MediaInfoList_Open_Buffer.argtype = [c_void_p, c_char_p, c_uint, c_char_p, c_uint]
MediaInfoList_Open_Buffer.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Save */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_Save (void* Handle, size_t FilePos);
MediaInfoList_Save = windll.MediaInfo.MediaInfoList_Save
MediaInfoList_Save.argtype = [c_void_p, c_uint]
MediaInfoList_Save.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Close */
#MEDIAINFO_EXP void              __stdcall MediaInfoList_Close (void* Handle, size_t FilePos);
MediaInfoList_Close = windll.MediaInfo.MediaInfoList_Close
MediaInfoList_Close.argtype = [c_void_p, c_uint]

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Inform */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_Inform (void* Handle, size_t FilePos, size_t Reserved); /*Default : Reserved=0*/
MediaInfoList_Inform = windll.MediaInfo.MediaInfoList_Inform
MediaInfoList_Inform.argtype = [c_void_p, c_uint, c_uint]
MediaInfoList_Inform.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Get */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_GetI (void* Handle, size_t FilePos, MediaInfo_stream_C StreamKind, size_t StreamNumber, size_t Parameter, MediaInfo_info_C InfoKind); /*Default : InfoKind=Info_Text*/
MediaInfoList_GetI = windll.MediaInfo.MediaInfoList_GetI
MediaInfoList_GetI.argtype = [c_void_p, c_uint, c_int, c_uint, c_uint, c_int]
MediaInfoList_GetI.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Get */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_Get (void* Handle, size_t FilePos, MediaInfo_stream_C StreamKind, size_t StreamNumber, const wchar_t* Parameter, MediaInfo_info_C InfoKind, MediaInfo_info_C SearchKind); /*Default : InfoKind=Info_Text, SearchKind=Info_Name*/
MediaInfoList_Get = windll.MediaInfo.MediaInfoList_Get
MediaInfoList_Get.argtype = [c_void_p, c_uint, c_int, c_uint, c_wchar_p, c_int, c_int]
MediaInfoList_Get.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Set */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_SetI (void* Handle, const wchar_t* ToSet, size_t FilePos, MediaInfo_stream_C StreamKind, size_t StreamNumber, size_t Parameter, const wchar_t* OldParameter);
MediaInfoList_SetI = windll.MediaInfo.MediaInfoList_SetI
MediaInfoList_SetI.argtype = [c_void_p, c_wchar_p, c_uint, c_int, c_uint, c_uint, c_wchar_p]
MediaInfoList_SetI.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Set */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_Set (void* Handle, const wchar_t* ToSet, size_t FilePos, MediaInfo_stream_C StreamKind, size_t StreamNumber, const wchar_t* Parameter, const wchar_t* OldParameter);
MediaInfoList_Set = windll.MediaInfo.MediaInfoList_Set
MediaInfoList_Set.argtype = [c_void_p, c_wchar_p, c_uint, c_int, c_uint, c_wchar_p, c_wchar_p]
MediaInfoList_Set.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Option */
#MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_Option (void* Handle, const wchar_t* Option, const wchar_t* Value);
MediaInfoList_Option = windll.MediaInfo.MediaInfoList_Option
MediaInfoList_Option.argtype = [c_void_p, c_wchar_p, c_wchar_p]
MediaInfoList_Option.restype = c_wchar_p

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::State_Get */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_State_Get (void* Handle);
MediaInfoList_State_Get = windll.MediaInfo.MediaInfoList_State_Get
MediaInfoList_State_Get.argtype = [c_void_p]
MediaInfoList_State_Get.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Count_Get */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_Count_Get (void* Handle, size_t FilePos, MediaInfo_stream_C StreamKind, size_t StreamNumber); /*Default : StreamNumber=-1*/
MediaInfoList_Count_Get = windll.MediaInfo.MediaInfoList_Count_Get
MediaInfoList_Count_Get.argtype = [c_void_p, c_uint, c_int, c_uint]
MediaInfoList_Count_Get.restype = c_uint

#/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Count_Get */
#MEDIAINFO_EXP size_t            __stdcall MediaInfoList_Count_Get_Files (void* Handle);
MediaInfoList_Count_Get_Files = windll.MediaInfo.MediaInfoList_Count_Get_Files
MediaInfoList_Count_Get_Files.argtype = [c_void_p]
MediaInfoList_Count_Get_Files.restype = c_uint



class Stream:
	General, Video, Audio, Text, Chapters, Image, Menu, Max = range(8)

class Info:
	Name, Text, Measure, Options, Name_Text, Measure_Text, Info, HowTo, Max = range(9)

class InfoOption:
	ShowInInform, Reserved, ShowInSupported, TypeOfValue, Max = range(5)

class FileOptions:
	Nothing, Recursive, CloseAll, xxNULLxx_3, Max = range(5)



class MediaInfo:
	Handle = c_void_p(0)
	def __init__(self):                
		self.Handle=MediaInfo_New()
	def __del__(self):
		MediaInfo_Delete(self.Handle)
	def Open(self, File):
		return MediaInfo_Open (self.Handle, File);
	def Open_Buffer(self, Begin, Begin_Size, End=NULL, End_Size=0):
		return MediaInfo_Open_Buffer(self.Handle, Begin, Begin_Size, End, End_Size)
	def Save(self):
		return MediaInfo_Save(self.Handle)
	def Close(self):
		return MediaInfo_Close(self.Handle)
    #//General information
	def Inform(self):
		return MediaInfo_Inform(self.Handle, 0)
	def Get(self, StreamKind, StreamNumber, Parameter, InfoKind=Info.Text, SearchKind=Info.Name):
		return MediaInfo_Get(self.Handle, StreamKind, StreamNumber, Parameter, InfoKind, SearchKind)
	def GetI(self, StreamKind, StreamNumber, Parameter, InfoKind=Info.Text):
		return MediaInfo_GetI(self.Handle, StreamKind, StreamNumber, Parameter, InfoKind)
	def Set(self, ToSet, StreamKind, StreamNumber, Parameter, OldParameter=u""):
		return MediaInfo_Set(self.Handle, ToSet, StreamKind, StreamNumber, Parameter, OldParameter)
	def SetI(self, ToSet, StreamKind, StreamNumber, Parameter, OldValue):
		return MediaInfo_SetI(self.Handle, ToSet, StreamKind, StreamNumber, Parameter, OldValue)
    #//Options
	def Option(self, Option, Value=u""):
		return MediaInfo_Option(self.Handle, Option, Value)
	def Option_Static(self, Option, Value=u""):
		return MediaInfo_Option(NULL, Option, Value)
	def State_Get(self):
		return MediaInfo_State_Get(self, Handle)
	def Count_Get(self, StreamKind, StreamNumber=-1):
		return MediaInfo_Count_Get(self.Handle, StreamKind, StreamNumber)


class MediaInfoList:
	Handle = c_void_p(0)
	def __init__(self):              
		self.Handle=MediaInfoList_New()
	def __del__(self):
		MediaInfoList_Delete(self.Handle)
	def Open(self, Files, Options=FileOptions.Nothing):
		return MediaInfoList_Open(self.Handle, Files, Options)
	def Open_Buffer(self, Begin, Begin_Size, End=NULL, End_Size=0):
		return MediaInfoList_Open_Buffer (self.Handle, Begin, Begin_Size, End, End_Size)
	def Save(self, FilePos):
		return MediaInfoList_Save(self.Handle, FilePos)
	def Close(self, FilePos):
		MediaInfoList_Close (self.Handle, FilePos)
	def Inform(self, FilePos, Reserved=0):
		return MediaInfoList_Inform (self.Handle, FilePos, Reserved)
	def GetI(self, FilePos, StreamKind, StreamNumber, Parameter, InfoKind=Info.Text):
		return MediaInfoList_GetI (self.Handle, FilePos, StreamKind, StreamNumber, Parameter, InfoKind)
	def Get(self, FilePos, StreamKind, StreamNumber, Parameter, InfoKind=Info.Text, SearchKind=Info.Name):
		return MediaInfoList_Get (self.Handle, FilePos, StreamKind, StreamNumber, (Parameter), InfoKind, SearchKind)
	def SetI(self, ToSet, FilePos, StreamKind, StreamNumber, Parameter, OldParameter=u""):
		return MediaInfoList_SetI (self, Handle, ToSet, FilePos, StreamKind, StreamNumber, Parameter, OldParameter)
	def Set(self, ToSet, FilePos, StreamKind, StreamNumber, Parameter, OldParameter=u""):
		return MediaInfoList_Set (self.Handle, ToSet, FilePos, StreamKind, StreamNumber, Parameter, OldParameter)
	def Option(self, Option, Value=u""):
		return MediaInfoList_Option (self.Handle, Option, Value)
	def Option_Static(self, Option, Value=u""):
		return MediaInfoList_Option(NULL, Option, Value)
	def State_Get(self):
		return MediaInfoList_State_Get (self.Handle)
	def Count_Get(self, FilePos, StreamKind, StreamNumber):
		return MediaInfoList_Count_Get (self.Handle, FilePos, StreamKind, StreamNumber=-1)
	def Count_Get_Files(self):
		return MediaInfoList_Count_Get_Files (self.Handle)



