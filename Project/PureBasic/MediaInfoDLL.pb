; MediaInfoDLL - All info about media files
; This software is provided 'as-is', without any express or implied
; warranty.  In no event will the authors be held liable for any damages
; arising from the use of this software.
;
; Permission is granted to anyone to use this software for any purpose,
; including commercial applications, and to alter it and redistribute it
; freely, subject to the following restrictions:
;
; 1. The origin of this software must not be misrepresented; you must not
;    claim that you wrote the original software. If you use this software
;    in a product, an acknowledgment in the product documentation would be
;    appreciated but is not required.
; 2. Altered source versions must be plainly marked as such, and must not be
;    misrepresented as being the original software.
; 3. This notice may not be removed or altered from any source distribution.
;
;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



;***************************************************************************
; Constants
;***************************************************************************

Enumeration
    #Stream_General
    #Stream_Video
    #Stream_Audio
    #Stream_Text
    #Stream_Chapters
    #Stream_Image
    #Stream_Menu
    #Stream_Max
EndEnumeration

Enumeration
    #Info_Name
    #Info_Text
    #Info_Measure
    #Info_Options
    #Info_NameText
    #Info_MeasureText
    #Info_Info
    #Info_HowTo
    #Info_Max
EndEnumeration

Enumeration
    #Option_ShowInInform
    #Option_Reserved
    #Option_ShowInSupported
    #Option_TypeOfValue
    #Option_Max
EndEnumeration

;***************************************************************************
; Prototyping MediaInfo calls
;***************************************************************************

Prototype protoMediaInfo_New()
Prototype protoMediaInfo_Delete(Handle)
Prototype protoMediaInfoA_Open(Handle, File.s)
Prototype protoMediaInfoA_Count_Get(Handle, Streamkind.l, StreamNumber.l)
Prototype protoMediaInfoA_Get(Handle, StreamKind.l, StreamNumber.l, Parameter.s, InfoKind.l, SearchKind.l)
Prototype protoMediaInfoA_GetI(Handle, StreamKind.l, StreamNumber.l, Parameter.l, InfoKind.l)
Prototype protoMediaInfoA_Inform(Handle, Reserved.l)
Prototype protoMediaInfoA_Option(handle, Option.s, Value.s)
Prototype protoMediaInfoA_close()

MI_ptr = OpenLibrary(#PB_Any, "MediaInfo.dll")
If MI_ptr = #False
  MessageRequester ("Warning", "MediaInfo.dll not open :("+ #CRLF$ + "Is it in the same folder as this app?")
  End
EndIf

Define MediaInfo_New.protoMediaInfo_New                     = GetFunction(MI_ptr, "MediaInfo_New")
Define MediaInfo_Delete.protoMediaInfo_Delete               = GetFunction(MI_ptr, "MediaInfo_Delete")
Define MediainfoA_Open.protoMediaInfoA_Open                 = GetFunction(MI_ptr, "MediaInfoA_Open")
Define MediainfoA_Count_Get.protoMediaInfoA_Count_Get       = GetFunction(MI_ptr, "MediaInfoA_Count_Get")
Define MediaInfoA_Get.protoMediaInfoA_Get                   = GetFunction(MI_ptr, "MediaInfoA_Get")
Define MediaInfoA_GetI.protoMediaInfoA_GetI                 = GetFunction(MI_ptr, "MediaInfoA_GetI")
Define MediaInfoA_Inform.protoMediaInfoA_Inform             = GetFunction(MI_ptr, "MediaInfoA_Inform")  
Define MediaInfoA_Option.protoMediaInfoA_Option             = GetFunction(MI_ptr, "MediaInfoA_Option")
Define MediainfoA_close.protoMediaInfoA_Close               = GetFunction(MI_ptr, "MediaInfoA_Close")

; IDE Options = PureBasic 4.20 (Windows - x86)
; CursorPosition = 85
; FirstLine = 44
; Folding = -