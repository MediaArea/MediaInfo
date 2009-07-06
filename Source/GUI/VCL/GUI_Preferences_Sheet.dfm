object Preferences_SheetF: TPreferences_SheetF
  Left = 230
  Top = 142
  Caption = 'Preference_SheetF'
  ClientHeight = 341
  ClientWidth = 686
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
  object Column_Text0: TLabel
    Left = 362
    Top = 103
    Width = 6
    Height = 14
    Caption = '1'
    Visible = False
  end
  object Label6: TLabel
    Left = 9
    Top = 9
    Width = 93
    Height = 14
    Caption = 'Number of colums :'
  end
  object Column_Parameter0: TComboBox
    Left = 138
    Top = 95
    Width = 156
    Height = 22
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 0
    Visible = False
    OnChange = Column_Parameter0Change
  end
  object Column_Kind0: TComboBox
    Left = 0
    Top = 95
    Width = 87
    Height = 22
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 1
    Visible = False
    OnChange = Column_Kind0Change
  end
  object ColumnsCount: TComboBox
    Left = 112
    Top = 0
    Width = 44
    Height = 22
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 2
    OnChange = ColumnsCountChange
  end
  object OK: TButton
    Left = 655
    Top = 0
    Width = 81
    Height = 27
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = OKClick
  end
  object Cancel: TButton
    Left = 575
    Top = 0
    Width = 81
    Height = 27
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object Sheet_Show: TStringGrid
    Left = 0
    Top = 26
    Width = 733
    Height = 61
    FixedCols = 0
    RowCount = 2
    GridLineWidth = 0
    ScrollBars = ssVertical
    TabOrder = 5
  end
  object Column_Size0: TComboBox
    Left = 302
    Top = 95
    Width = 52
    Height = 22
    ItemHeight = 0
    TabOrder = 6
    Visible = False
  end
  object Column_Pos0: TComboBox
    Left = 86
    Top = 95
    Width = 53
    Height = 22
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 7
    Visible = False
    OnChange = Column_Pos0Change
  end
end
