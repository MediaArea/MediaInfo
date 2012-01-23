object Form1: TForm1
  Left = 192
  Top = 114
  Caption = 'Form1'
  ClientHeight = 505
  ClientWidth = 680
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 16
    Width = 649
    Height = 481
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object MainMenu1: TMainMenu
    Left = 648
    Top = 16
    object File1: TMenuItem
      Caption = 'File'
      object Testiftherearetoooldfiles1: TMenuItem
        Caption = 'Test if there are too old files'
        OnClick = Testiftherearetoooldfiles1Click
      end
      object Updateresourcefiles1: TMenuItem
        Caption = 'Update resource files'
        OnClick = Updateresourcefiles1Click
      end
    end
  end
end
