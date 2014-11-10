object Form1: TForm1
  Left = 192
  Top = 114
  Caption = 'Form1'
  ClientHeight = 525
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
    TabOrder = 0
  end
  object MainMenu1: TMainMenu
    Left = 648
    Top = 16
    object File1: TMenuItem
      Caption = 'File'
      object CreateotherlanguagefilesfromAllcsv1: TMenuItem
        Caption = 'Create other language files from All.csv'
        OnClick = CreateotherlanguagefilesfromAllcsv1Click
      end
      object UpdateAllcsvfromotherlanguagefiles1: TMenuItem
        Caption = 'Update All.csv from other language files'
        OnClick = UpdateAllcsvfromotherlanguagefiles1Click
      end
    end
  end
end
