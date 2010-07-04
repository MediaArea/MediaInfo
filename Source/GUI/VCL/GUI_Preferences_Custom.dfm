object Preferences_CustomF: TPreferences_CustomF
  Left = 277
  Top = 147
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu]
  Caption = 'Output'
  ClientHeight = 502
  ClientWidth = 676
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 14
  object Liste: TComboBox
    Left = 9
    Top = 9
    Width = 216
    Height = 22
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 0
    Text = 'General'
    OnChange = ListeChange
    Items.Strings = (
      'General'
      'Video'
      'Audio'
      'Text'
      'Chapters'
      'Image'
      'Begin'
      'End'
      'Page_Begin'
      'Page_Middle'
      'Page_End'
      'General_Begin'
      'General_End'
      'Video_Begin'
      'Video_Middle'
      'Video_End'
      'Audio_Begin'
      'Audio_Middle'
      'Audio_End'
      'Text_Begin'
      'Text_Middle'
      'Text_End'
      'Chapters_Begin'
      'Chapters_Middle'
      'Chapters_End'
      'Image_Begin'
      'Image_Middle'
      'Image_End')
  end
  object Memo: TMemo
    Left = 0
    Top = 34
    Width = 716
    Height = 458
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'System'
    Font.Pitch = fpFixed
    Font.Style = []
    Lines.Strings = (
      'Memo')
    ParentFont = False
    TabOrder = 1
  end
  object OK: TButton
    Left = 638
    Top = 0
    Width = 80
    Height = 27
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = OKClick
  end
  object Infos: TComboBox
    Left = 0
    Top = 500
    Width = 260
    Height = 22
    Style = csDropDownList
    TabOrder = 3
    OnChange = InfosChange
  end
  object Repeat: TButton
    Left = 258
    Top = 500
    Width = 53
    Height = 21
    Caption = 'Repeat'
    TabOrder = 4
    OnClick = InfosChange
  end
  object Cancel: TButton
    Left = 560
    Top = 0
    Width = 81
    Height = 27
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
  object Help: TButton
    Left = 638
    Top = 500
    Width = 80
    Height = 27
    Cancel = True
    Caption = 'Help'
    TabOrder = 6
    OnClick = HelpClick
  end
end
