object GUI_Main_: TGUI_Main_
  Left = 293
  Top = 192
  Caption = 'GUI_Main'
  ClientHeight = 226
  ClientWidth = 339
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu1: TMainMenu
    Left = 304
    Top = 160
    object Menu_File: TMenuItem
      Caption = 'File'
      object Menu_File_Open: TMenuItem
        Caption = 'Open'
        object Menu_File_Open_File: TMenuItem
          Caption = 'File'
          OnClick = Menu_File_Open_FileClick
        end
        object Menu_File_Open_File_Directory: TMenuItem
          Caption = 'Directory'
        end
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Menu_File_Exit: TMenuItem
        Caption = 'Exit'
        OnClick = Menu_File_ExitClick
      end
    end
    object Menu_View: TMenuItem
      Caption = 'View'
      object Menu_View_Easy: TMenuItem
        AutoCheck = True
        Caption = 'Easy'
        RadioItem = True
        OnClick = Menu_View_EasyClick
      end
      object Menu_View_Sheet: TMenuItem
        AutoCheck = True
        Caption = 'Sheet'
        Enabled = False
        RadioItem = True
        OnClick = Menu_View_SheetClick
      end
      object Menu_View_Tree: TMenuItem
        AutoCheck = True
        Caption = 'Tree'
        Enabled = False
        RadioItem = True
        OnClick = Menu_View_TreeClick
      end
      object Menu_View_HTML: TMenuItem
        AutoCheck = True
        Caption = 'HTML'
        RadioItem = True
        OnClick = Menu_View_HTMLClick
      end
      object Menu_View_Text: TMenuItem
        AutoCheck = True
        Caption = 'Text'
        Checked = True
        RadioItem = True
        OnClick = Menu_View_TextClick
      end
    end
    object Menu_Debug_Debug: TMenuItem
      Caption = 'Debug'
      object Menu_Debug_Complete: TMenuItem
        AutoCheck = True
        Caption = 'Complete'
      end
      object Menu_Debug_Details: TMenuItem
        AutoCheck = True
        Caption = 'Details'
        OnClick = Menu_Debug_DetailsClick
      end
      object Menu_Debug_Demux: TMenuItem
        Caption = 'Demux'
        object Menu_Debug_Demux_No: TMenuItem
          AutoCheck = True
          Caption = 'No'
          RadioItem = True
        end
        object Menu_Debug_Demux_Element: TMenuItem
          AutoCheck = True
          Caption = 'Element'
          RadioItem = True
        end
        object Menu_Debug_Demux_All: TMenuItem
          AutoCheck = True
          Caption = 'All'
          RadioItem = True
        end
      end
    end
    object Menu_Help: TMenuItem
      Caption = 'Help'
      object Menu_Help_About: TMenuItem
        Caption = 'About'
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 304
    Top = 128
  end
end
