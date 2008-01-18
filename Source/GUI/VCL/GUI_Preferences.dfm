object PreferencesF: TPreferencesF
  Left = 286
  Top = 180
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Preferences'
  ClientHeight = 184
  ClientWidth = 572
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object OK: TTntButton
    Left = 448
    Top = 156
    Width = 122
    Height = 27
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKClick
  end
  object Tree: TTntTreeView
    Left = 0
    Top = -1
    Width = 150
    Height = 150
    Indent = 19
    ParentShowHint = False
    ReadOnly = True
    ShowHint = False
    TabOrder = 1
    OnChange = TreeChange
    Items.NodeData = {
      0102000000230000000000000000000000FFFFFFFFFFFFFFFF00000000010000
      000553006500740075007000290000000000000000000000FFFFFFFFFFFFFFFF
      00000000000000000841006400760061006E006300650064002B000000000000
      0000000000FFFFFFFFFFFFFFFF00000000040000000943007500730074006F00
      6D0069007A006500230000000000000000000000FFFFFFFFFFFFFFFF00000000
      0000000005530068006500650074002F0000000000000000000000FFFFFFFFFF
      FFFFFF00000000000000000B5400720065006500200026002000540065007800
      74002F0000000000000000000000FFFFFFFFFFFFFFFF00000000000000000B43
      007500730074006F006D00200074006500780074002900000000000000000000
      00FFFFFFFFFFFFFFFF0000000000000000084C0061006E006700750061006700
      6500}
    Items.Utf8Data = {
      02000000210000000000000000000000FFFFFFFFFFFFFFFF0000000001000000
      08EFBBBF5365747570240000000000000000000000FFFFFFFFFFFFFFFF000000
      00000000000BEFBBBF416476616E636564250000000000000000000000FFFFFF
      FFFFFFFFFF00000000040000000CEFBBBF437573746F6D697A65210000000000
      000000000000FFFFFFFFFFFFFFFF000000000000000008EFBBBF536865657427
      0000000000000000000000FFFFFFFFFFFFFFFF00000000000000000EEFBBBF54
      72656520262054657874270000000000000000000000FFFFFFFFFFFFFFFF0000
      0000000000000EEFBBBF437573746F6D20746578742400000000000000000000
      00FFFFFFFFFFFFFFFF00000000000000000BEFBBBF4C616E6775616765}
  end
  object Cancel: TTntButton
    Left = 324
    Top = 156
    Width = 121
    Height = 27
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object Page: TTntPageControl
    Left = 149
    Top = 0
    Width = 425
    Height = 157
    ActivePage = Setup
    MultiLine = True
    TabOrder = 3
    object Setup: TTntTabSheet
      Caption = 'General'
      OnShow = Setup_GeneralShow
      object Langue_C: TTntLabel
        Left = 0
        Top = 9
        Width = 54
        Height = 14
        Caption = 'Language :'
      end
      object Output_C: TTntLabel
        Left = 0
        Top = 34
        Width = 38
        Height = 14
        Caption = 'Output :'
      end
      object General_Language_Sel: TTntComboBox
        Left = 103
        Top = 0
        Width = 113
        Height = 22
        Style = csDropDownList
        ItemHeight = 14
        TabOrder = 0
        OnChange = General_Language_SelChange
      end
      object General_Output_Sel: TTntComboBox
        Left = 103
        Top = 26
        Width = 113
        Height = 22
        Style = csDropDownList
        ItemHeight = 14
        TabOrder = 1
        OnChange = General_Output_SelChange
        Items.Strings = (
          'Simple'
          'Sheet'
          'Tree'
          'Text'
          'HTML'
          'Custom')
      end
      object CB_CheckUpdate: TTntCheckBox
        Left = 0
        Top = 52
        Width = 406
        Height = 18
        Caption = 'Check for newest versions (require Internet connection)'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = CB_CheckUpdateClick
      end
      object General_Language_More: TTntButton
        Left = 216
        Top = 0
        Width = 45
        Height = 23
        Caption = 'More...'
        TabOrder = 3
        OnClick = General_Language_MoreClick
      end
      object General_Output_More: TTntButton
        Left = 216
        Top = 26
        Width = 45
        Height = 22
        Caption = 'More...'
        TabOrder = 4
        OnClick = General_Output_MoreClick
      end
      object CB_InfoTip: TTntCheckBox
        Left = 0
        Top = 84
        Width = 406
        Height = 18
        Caption = 
          'Shell InfoTip (in explorer, move the mouse on file, info will be' +
          ' displayed)'
        TabOrder = 5
        OnClick = CB_InfoTipClick
      end
      object CB_InscrireShell: TTntCheckBox
        Left = 0
        Top = 68
        Width = 406
        Height = 18
        Caption = 
          'Shell extension (in explorer, righ click, there will be a "Media' +
          ' Info" option...)'
        Checked = True
        State = cbChecked
        TabOrder = 6
        OnClick = CB_InscrireShellClick
      end
    end
    object Setup_Advanced: TTntTabSheet
      Caption = 'Advanced'
      ImageIndex = 1
      OnShow = Setup_AdvancedShow
      object CB_ShowToolBar: TTntCheckBox
        Left = 0
        Top = 26
        Width = 268
        Height = 18
        Caption = 'Show toolbar'
        TabOrder = 0
        OnClick = CB_ShowToolBarClick
      end
      object Advanced_CloseAllAuto: TTntCheckBox
        Left = 0
        Top = 59
        Width = 268
        Height = 18
        Caption = 'Close all before open'
        TabOrder = 1
        OnClick = Advanced_CloseAllAutoClick
      end
      object CB_ShowMenu: TTntCheckBox
        Left = 0
        Top = 42
        Width = 268
        Height = 18
        Caption = 'Show menu'
        TabOrder = 2
        OnClick = CB_ShowMenuClick
      end
    end
    object Customize_Language: TTntTabSheet
      Caption = 'Language'
      ImageIndex = 3
      OnShow = Customize_LanguageShow
      object Language_Caption: TTntLabel
        Left = 0
        Top = 0
        Width = 154
        Height = 14
        Caption = 'Choose your desired language :'
      end
      object Language_New: TTntButton
        Left = 162
        Top = 43
        Width = 80
        Height = 23
        Caption = 'New...'
        TabOrder = 3
        OnClick = Language_NewClick
      end
      object Language_Delete: TTntButton
        Left = 0
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Delete'
        TabOrder = 1
        OnClick = Language_DeleteClick
      end
      object Language_Edit: TTntButton
        Left = 81
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Edit...'
        TabOrder = 2
        OnClick = Language_EditClick
      end
      object Language_Sel: TTntComboBox
        Left = 0
        Top = 17
        Width = 242
        Height = 22
        Style = csDropDownList
        ItemHeight = 14
        TabOrder = 0
        OnChange = Language_SelChange
      end
    end
    object Customize_Sheet: TTntTabSheet
      Caption = 'Sheet'
      ImageIndex = 4
      OnShow = Customize_SheetShow
      object Sheet_Caption: TTntLabel
        Left = 0
        Top = 0
        Width = 175
        Height = 14
        Caption = 'Choose your desired custom sheet :'
      end
      object Sheet_Sel: TTntComboBox
        Left = 0
        Top = 17
        Width = 242
        Height = 22
        Style = csDropDownList
        ItemHeight = 14
        TabOrder = 0
        OnChange = Sheet_SelChange
      end
      object Sheet_Delete: TTntButton
        Left = 0
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Delete'
        TabOrder = 1
        OnClick = Sheet_DeleteClick
      end
      object Sheet_Edit: TTntButton
        Left = 81
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Edit...'
        TabOrder = 2
        OnClick = Sheet_EditClick
      end
      object Sheet_New: TTntButton
        Left = 162
        Top = 43
        Width = 80
        Height = 23
        Caption = 'New...'
        TabOrder = 3
        OnClick = Sheet_NewClick
      end
    end
    object Customize_TreeText: TTntTabSheet
      Caption = 'Tree && Text'
      ImageIndex = 6
      object Label13: TTntLabel
        Left = 362
        Top = 78
        Width = 43
        Height = 14
        Caption = 'Not yet...'
      end
    end
    object Customize_Custom: TTntTabSheet
      Caption = 'Custom Text'
      ImageIndex = 6
      OnShow = Customize_CustomShow
      object Custom_Caption: TTntLabel
        Left = 0
        Top = 0
        Width = 166
        Height = 14
        Caption = 'Choose your desired custom text :'
      end
      object Custom_New: TTntButton
        Left = 162
        Top = 43
        Width = 80
        Height = 23
        Caption = 'New...'
        TabOrder = 0
        OnClick = Custom_NewClick
      end
      object Custom_Edit: TTntButton
        Left = 81
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Edit...'
        TabOrder = 1
        OnClick = Custom_EditClick
      end
      object Custom_Delete: TTntButton
        Left = 0
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Delete'
        TabOrder = 2
        OnClick = Custom_DeleteClick
      end
      object Custom_Sel: TTntComboBox
        Left = 0
        Top = 17
        Width = 242
        Height = 22
        Style = csDropDownList
        ItemHeight = 14
        TabOrder = 3
        OnChange = Custom_SelChange
      end
    end
    object Customize: TTntTabSheet
      Caption = 'Customize'
      ImageIndex = 7
    end
  end
end
