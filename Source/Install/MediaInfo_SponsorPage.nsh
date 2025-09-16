!include 'NSDialogs.nsh'
!include 'StrFunc.nsh'

${Using:StrFunc} StrTrimNewLines
${Using:StrFunc} StrTok

!define INSTALLER_BIN_URL "https://MediaArea.net/mediainfo_check/installer.bin"
!define INSTALLER_BIN_FILE "$PLUGINSDIR\mediainfo_installer.bin"
!define SPONSOR_IMG_FILE "$PLUGINSDIR\mediainfo_sponsor_img.bin"

Var ShowSponsor
Var SponsorImageUrl
Var SponsorLinkUrl

Function DownloadAndParseSponsorData
  ; Check donator status
  ReadRegDWORD $0 HKCU "Software\MediaArea.net\MediaInfo" "Donated"
  ReadRegDWORD $1 HKCU "Software\MediaArea\MediaInfo" "Donated"
  ${If} $0 == "1"
  ${OrIf} $1 == "1"
    StrCpy $ShowSponsor "0"
    Goto end
  ${EndIf}

  ; Download installer.bin
  inetc::get /SILENT "${INSTALLER_BIN_URL}" "${INSTALLER_BIN_FILE}" /END
  Pop $0
  StrCmp $0 "OK" 0 fail

  ClearErrors
  FileOpen $1 "${INSTALLER_BIN_FILE}" r
  IfErrors fail

  read_loop:
  FileRead $1 $2
  StrCmp $2 "" end_loop ; EOF

  ${StrTrimNewLines} $2 $2
  StrCmp $2 "" read_loop ; Skip empty lines

  ${StrTok} $3 $2 ";" "0" "0"

  StrCmp $3 "Sponsor" 0 read_loop ; Ignore others parameters

  ; 2nd field is language, ignore for now

  ${StrTok} $SponsorImageUrl $2 ";" "2" "0"

  ${StrTok} $SponsorLinkUrl $2 ";" "3" "0"
  end_loop:

  FileClose $1

  ; If both lines are present, download image
  StrCmp $SponsorImageUrl "" fail
  StrCmp $SponsorLinkUrl "" fail
  inetc::get /SILENT "$SponsorImageUrl" "${SPONSOR_IMG_FILE}" /END
  Pop $0
  StrCmp $0 "OK" 0 fail

  ; Set flag for banner display
  StrCpy $ShowSponsor "1"
  Goto end

  fail:
  StrCpy $ShowSponsor "0"
  end:
FunctionEnd

Function ShowSponsorOnFinish
  StrCmp $ShowSponsor "1" 0 end

  ; Create page
  nsDialogs::Create 1018
  Pop $0
  ${If} $0 == error
    Goto end
  ${EndIf}

  ; Variables, all prefixed for avoiding conflicts with others plugins
  Var /GLOBAL SS_BitmapCtl
  Var /GLOBAL SS_GpToken
  Var /GLOBAL SS_GpStartupInputStruct
  Var /GLOBAL SS_GpImage
  Var /GLOBAL SS_GpGraphic
  Var /GLOBAL SS_BitmapHandle
  Var /GLOBAL SS_CtlDC
  Var /GLOBAL SS_CmpDC

  Var /GLOBAL SS_ImgW
  Var /GLOBAL SS_ImgH
  Var /GLOBAL SS_CtlW
  Var /GLOBAL SS_CtlH
  Var /GLOBAL SS_BmpH
  Var /GLOBAL SS_BmpW

  ; Create bitmap control
  ${NSD_CreateBitmap} 0 0 100% 100% ""
  Pop $SS_BitmapCtl
  ${If} $SS_BitmapCtl == error
    Goto end
  ${EndIf}
  ${NSD_OnClick} $SS_BitmapCtl SponsorImageClicked

  System::Call 'USER32::LoadCursor(i0, i32649) i.r9'
  ${If} $9 <> 0
    System::Call 'USER32::SetClassLong(p$SS_BitmapCtl, i-12, ir9) i.r9'
  ${EndIf}

  ; Initialize GDI+
  System::Alloc 16
  Pop $SS_GpToken

  System::Call '*(i1, p0, b0, b0) p.r9' ; GdiplusStartupInput struct
                                        ; Use 'p' instead of the 'k' type
                                        ; for DebugEventCallback to avoid
                                        ; being flagged by antivirus programs

  StrCpy $SS_GpStartupInputStruct $9

  System::Call 'gdiplus::GdiplusStartup(p$SS_GpToken, p$SS_GpStartupInputStruct, i0) i.r9'
  ${If} $9 <> 0   
    Goto finish
  ${EndIf}

  ; Load sponsor image
  System::Call 'gdiplus::GdipLoadImageFromFile(w"${SPONSOR_IMG_FILE}", *p.r0) i.r9'
  ${If} $9 <> 0
    Goto finish
  ${EndIf}
  StrCpy $SS_GpImage $0

  ; Get image dimensions
  System::Call 'gdiplus::GdipGetImageWidth(p$SS_GpImage, *i.r0) i.r9'
  ${If} $9 <> 0
  ${OrIf} $0 == 0
    Goto finish
  ${EndIf}
  StrCpy $SS_ImgW $0

  System::Call 'gdiplus::GdipGetImageHeight(p$SS_GpImage, *i.r0) i.r9'
  ${If} $9 <> 0
  ${OrIf} $0 == 0
    Goto finish
  ${EndIf}
  StrCpy $SS_ImgH $0

  ; Get control dimensions
  System::Call '*(i, i, i, i) i.r0' ; RECT
  System::Call 'USER32::GetClientRect(p$SS_BitmapCtl, ir0) b.r9'
  ${If} $9 == 0
    Goto finish
  ${EndIf}

  System::Call '*$0(i.r1, i.r2, i.r3, i.r4)'
  IntOp $SS_CtlW $3 - $1
  IntOp $SS_CtlH $4 - $2
  System::Free $0

  ; Calculate new width/height to fit and preserve aspect ratio
  IntOp $0 $SS_CtlW * 1000
  IntOp $0 $0 / $SS_ImgW
  IntOp $1 $SS_CtlH * 1000
  IntOp $1 $1 / $SS_ImgH

  ${If} $0 < $1
    StrCpy $2 $0
  ${Else}
    StrCpy $2 $1
  ${EndIf}

  IntOp $SS_BmpW $SS_ImgW * $2
  IntOp $SS_BmpW $SS_BmpW / 1000
  IntOp $SS_BmpH $SS_ImgH * $2
  IntOp $SS_BmpH $SS_BmpH / 1000

  ; Create compatible DC and bitmap
  System::Call 'USER32::GetDC(p$SS_BitmapCtl) p.r9'
  StrCpy $SS_CtlDC $9

  System::Call 'GDI32::CreateCompatibleDC(p$SS_CtlDC) p.r9'
  StrCpy $SS_CmpDC $9

  System::Call 'GDI32::CreateCompatibleBitmap(p$SS_CtlDC, i$SS_BmpW, i$SS_BmpH) p.r9'
  StrCpy $SS_BitmapHandle $9

  System::Call 'GDI32::SelectObject(p$SS_CmpDC, p$SS_BitmapHandle)'

  ; Create Graphics from memory DC
  System::Call 'gdiplus::GdipCreateFromHDC(p$SS_CmpDC, *p.r0) i.r9'
  ${If} $9 <> 0
    Goto finish
  ${EndIf}
  StrCpy $SS_GpGraphic $0

  ; Draw scaled image
  System::Call 'gdiplus::GdipDrawImageRectI(p$SS_GpGraphic, p$SS_GpImage, i0, i0, i$SS_BmpW, i$SS_BmpH) i.r9'
  ${If} $9 <> 0
    Goto finish
  ${EndIf}

  ; Commit
  System::Call 'gdiplus::GdipFlush(p$SS_GpGraphic, i1)'

  ; Set bitmap to control
  SendMessage $SS_BitmapCtl ${STM_SETIMAGE} ${IMAGE_BITMAP} $SS_BitmapHandle

  finish:
  ; Cleanup
  ${If} $SS_GpGraphic <> 0
    System::Call 'gdiplus::GdipDeleteGraphics(p$SS_GpGraphic)'
  ${EndIf}

  ${If} $SS_GpImage <> 0
    System::Call 'gdiplus::GdipDisposeImage(p$SS_GpImage)'
  ${EndIf}

  ${If} $SS_CmpDC <> 0
    System::Call 'GDI32::DeleteDC(p$SS_CmpDC)'
  ${EndIf}

  ${If} $SS_CtlDC <> 0 
    System::Call 'USER32::ReleaseDC(p$SS_BitmapCtl, p$SS_CtlDC)'
  ${EndIf}

  ; Show the page (mandatory after nsDialogs::Create)
  nsDialogs::Show

  ; Shutdown GDI+
  System::Call 'gdiplus::GdiplusShutdown(p$SS_GpToken)'
  System::Free $SS_GpStartupInputStruct
  System::Free $SS_GpToken
  end:
FunctionEnd

Function SponsorImageClicked
  ExecShell "open" "$SponsorLinkUrl"
FunctionEnd
