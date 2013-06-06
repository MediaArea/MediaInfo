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
;
; HowToUse - Example for MediaInfoLib and PureBasic 4.20
; Note : this one uses a read-only EditorGadget
;
;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

;***************************************************************************
; Defines
;***************************************************************************

EnableExplicit
Define MI_ptr.l
Define handle.l
Define info_ptr.l
Define mediafile.s = "Example.ogg"

Define win_main.l
Define ggl_main.l
Define display_gagdet.l

;***************************************************************************
; Configuration
;***************************************************************************

IncludeFile "MediaInfoDLL.pb"

;***************************************************************************
; Creating the handle and opening the file
;***************************************************************************

Handle = MediaInfo_New()

If Handle = #False
  MessageRequester ("Warning", "MediaInfo_New() didn't return a handle")
  End
EndIf

MediaInfoA_Open(Handle, mediafile)

;***************************************************************************
; Create a GUI to send Information to
;***************************************************************************

win_main = OpenWindow(#PB_Any,10,10,790,590,"MediaInfo-Example",#PB_Window_SystemMenu |#PB_Window_ScreenCentered)
If win_main = #False
  MessageRequester ("Warning", "Can't open window")
  End
EndIf

ggl_main = CreateGadgetList(WindowID(win_main))
If ggl_main = #False
  MessageRequester ("Warning", "Can't create GadgetList")
  End
EndIf

LoadFont(1,"FixedSys",12)

display_gagdet = EditorGadget(#PB_Any,5,5,780,580,#PB_Editor_ReadOnly)
If display_gagdet = #False
  MessageRequester ("Warning", "Can't create EditorGadget")
  End
EndIf

LoadFont(1,"FixedSys",12)
SetGadgetFont(display_gagdet, FontID(1))

;***************************************************************************
; Information about MediaInfo
;***************************************************************************
AddGadgetItem(display_gagdet, -1, ";***************************************************************************")
AddGadgetItem(display_gagdet, -1, "; Information about MediaInfo")
AddGadgetItem(display_gagdet, -1, ";***************************************************************************"+#CRLF$)

info_ptr = MediaInfoA_Option(Handle, "Info_Version", "")
AddGadgetItem(display_gagdet, -1, "Ex01:Info_Version:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Option(Handle, "Info_Parameters", "")
AddGadgetItem(display_gagdet, -1, "Ex02:Info_Parameters:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Option(Handle, "Info_Capacities", "")
AddGadgetItem(display_gagdet, -1, "Ex03:Info_Capacities:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Option(Handle, "Info_Codecs", "")
AddGadgetItem(display_gagdet, -1, "Ex04:Info_Codecs:   "+#CRLF$+ PeekS(info_ptr)+#CRLF$)

;***************************************************************************
; Examples of how To use the library
;***************************************************************************

AddGadgetItem(display_gagdet, -1, ";***************************************************************************")
AddGadgetItem(display_gagdet, -1, "; Examples of how To use the library")
AddGadgetItem(display_gagdet, -1, ";***************************************************************************"+#CRLF$)

MediaInfoA_Option(Handle, "Complete", "")
info_ptr = MediaInfoA_Inform(Handle, 0)
AddGadgetItem(display_gagdet, -1, "Ex05:Inform With Complete=false:   "+ PeekS(info_ptr)+#CRLF$)

MediaInfoA_Option(Handle, "Complete", "1")
info_ptr = MediaInfoA_Inform(Handle, 0)
AddGadgetItem(display_gagdet, -1, "Ex06: Inform With Complete=true:   "+ PeekS(info_ptr)+#CRLF$)

MediaInfoA_Option(Handle, "Inform", "General;Example : FileSize=%FileSize%")
info_ptr =  MediaInfoA_Inform(Handle, 0)
AddGadgetItem(display_gagdet, -1, "Ex07: Custom Inform-string:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Get(Handle, #Stream_General, 0, "FileSize", #Info_Text, #Info_Name)
AddGadgetItem(display_gagdet, -1, "Ex08: Get with Stream=General and Parameter=FileSize:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_GetI(Handle, #Stream_General, 0, 46, #Info_Text)
AddGadgetItem(display_gagdet, -1, "Ex09: Get with Stream=General and Parameter=46:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Count_Get(Handle, #Stream_Audio, -1)
AddGadgetItem(display_gagdet, -1, "Ex10: Count_Get with StreamKind=Stream_Audio:   "+ Str(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Get(Handle, #Stream_General, 0, "AudioCount", #Info_Text, #Info_Name)
AddGadgetItem(display_gagdet, -1, "Ex11: Get with Stream=General and Parameter=AudioCount:   "+ PeekS(info_ptr)+#CRLF$)

info_ptr = MediaInfoA_Get(Handle, #Stream_Audio, 0, "StreamCount", #Info_Text, #Info_Name)
AddGadgetItem(display_gagdet, -1, "Ex12: Get with Stream=Audio and Parameter=StreamCount:   "+ PeekS(info_ptr)+#CRLF$)

;***************************************************************************
; main loop
;***************************************************************************

Repeat
Until WaitWindowEvent(1) = #PB_Event_CloseWindow

;***************************************************************************
; Clean up
;***************************************************************************

MediaInfoA_Close()
MediaInfo_Delete(Handle)
CloseLibrary(MI_ptr)
CloseWindow(win_main)
End

; IDE Options = PureBasic 4.20 (Windows - x86)
; CursorPosition = 21
; Folding = -