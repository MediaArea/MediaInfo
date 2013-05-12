;;  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 ;
 ;  Use of this source code is governed by a BSD-style license that can
 ;  be found in the License.html file in the root of the source tree.
 ;;

;***************************************************************************
; Constants
;***************************************************************************

Enumeration
    #Stream_General
    #Stream_Video
    #Stream_Audio
    #Stream_Text
    #Stream_Other
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