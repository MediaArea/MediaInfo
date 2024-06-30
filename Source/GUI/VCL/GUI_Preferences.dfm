object PreferencesF: TPreferencesF
  Left = 286
  Top = 180
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Preferences'
  ClientHeight = 187
  ClientWidth = 667
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Position = poDesigned
  OnShow = FormShow
  TextHeight = 14
  object OK: TButton
    Left = 544
    Top = 159
    Width = 122
    Height = 27
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKClick
  end
  object Tree: TTreeView
    Left = 0
    Top = 0
    Width = 176
    Height = 158
    Indent = 19
    ParentShowHint = False
    ReadOnly = True
    ShowHint = False
    TabOrder = 1
    OnChange = TreeChange
    Items.NodeData = {
      0302000000280000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000
      00010000000105530065007400750070002E0000000000000000000000FFFFFF
      FFFFFFFFFFFFFFFFFF0000000000000000010841006400760061006E00630065
      006400300000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF0000000005
      000000010943007500730074006F006D0069007A006500280000000000000000
      000000FFFFFFFFFFFFFFFFFFFFFFFF0000000000000000010553006800650065
      007400340000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF0000000000
      000000010B540072006500650020002600200054006500780074003400000000
      00000000000000FFFFFFFFFFFFFFFFFFFFFFFF0000000000000000010B430075
      00730074006F006D0020007400650078007400280000000000000000000000FF
      FFFFFFFFFFFFFFFFFFFFFF00000000000000000105470072006100700068002E
      0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000000001
      084C0061006E0067007500610067006500}
  end
  object Cancel: TButton
    Left = 417
    Top = 159
    Width = 121
    Height = 27
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object Page: TPageControl
    Left = 176
    Top = 0
    Width = 490
    Height = 158
    ActivePage = Setup
    MultiLine = True
    TabHeight = 22
    TabOrder = 3
    object Setup: TTabSheet
      Caption = 'General'
      OnShow = Setup_GeneralShow
      object Langue_C: TLabel
        Left = 0
        Top = 9
        Width = 54
        Height = 14
        Caption = 'Language :'
      end
      object Output_C: TLabel
        Left = 0
        Top = 34
        Width = 38
        Height = 14
        Caption = 'Output :'
      end
      object General_Language_Sel: TComboBox
        Left = 124
        Top = 0
        Width = 113
        Height = 22
        Style = csDropDownList
        TabOrder = 0
        OnChange = General_Language_SelChange
      end
      object General_Output_Sel: TComboBox
        Left = 124
        Top = 26
        Width = 113
        Height = 22
        Style = csDropDownList
        DropDownCount = 10
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
      object CB_CheckUpdate: TCheckBox
        Left = 0
        Top = 52
        Width = 506
        Height = 18
        Caption = 'Check for newest versions (require Internet connection)'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = CB_CheckUpdateClick
      end
      object General_Language_More: TButton
        Left = 237
        Top = 0
        Width = 57
        Height = 23
        Caption = 'More...'
        TabOrder = 6
        OnClick = General_Language_MoreClick
      end
      object General_Output_More: TButton
        Left = 237
        Top = 26
        Width = 57
        Height = 22
        Caption = 'More...'
        TabOrder = 7
        OnClick = General_Output_MoreClick
      end
      object CB_InfoTip: TCheckBox
        Left = 0
        Top = 100
        Width = 506
        Height = 18
        Caption = 
          'Shell InfoTip (in explorer, move the mouse on file, info will be' +
          ' displayed)'
        TabOrder = 5
        OnClick = CB_InfoTipClick
      end
      object CB_InscrireShell: TCheckBox
        Left = 0
        Top = 68
        Width = 506
        Height = 18
        Caption = 
          'Shell extension (in explorer, righ click, there will be a "Media' +
          ' Info" option...)'
        Checked = True
        State = cbChecked
        TabOrder = 3
        OnClick = CB_InscrireShellClick
      end
      object CB_InscrireShell_Folder: TCheckBox
        Left = 16
        Top = 84
        Width = 369
        Height = 17
        Caption = 'For folders too'
        TabOrder = 4
        OnClick = CB_InscrireShell_FolderClick
      end
    end
    object Setup_Advanced: TTabSheet
      Caption = 'Advanced'
      ImageIndex = 1
      OnShow = Setup_AdvancedShow
      object Advanced_DisplayCaptions_Caption: TLabel
        Left = 0
        Top = 105
        Width = 141
        Height = 14
        Caption = 'Handling of 608/708 streams:'
      end
      object CB_ShowToolBar: TCheckBox
        Left = 0
        Top = 0
        Width = 268
        Height = 18
        Caption = 'Show toolbar'
        TabOrder = 0
        OnClick = CB_ShowToolBarClick
      end
      object Advanced_CloseAllAuto: TCheckBox
        Left = 0
        Top = 33
        Width = 268
        Height = 18
        Caption = 'Close all before open'
        TabOrder = 5
        OnClick = Advanced_CloseAllAutoClick
      end
      object CB_ShowMenu: TCheckBox
        Left = 0
        Top = 16
        Width = 268
        Height = 18
        Caption = 'Show menu'
        TabOrder = 2
        OnClick = CB_ShowMenuClick
      end
      object Advanced_InformVersion: TCheckBox
        Left = 0
        Top = 50
        Width = 268
        Height = 18
        Caption = 'Add version to text output'
        TabOrder = 3
        OnClick = Advanced_InformVersionClick
      end
      object Advanced_InformTimestamp: TCheckBox
        Left = 0
        Top = 67
        Width = 268
        Height = 18
        Caption = 'Add creation date to text output'
        TabOrder = 6
        OnClick = Advanced_InformTimestampClick
      end
      object Advanced_EnableFfmpeg: TCheckBox
        Left = 0
        Top = 84
        Width = 268
        Height = 18
        Caption = 'Enable FFmpeg plugin'
        TabOrder = 4
        OnClick = Advanced_EnableFfmpegClick
      end
      object Advanced_DisplayCaptions_Sel: TComboBox
        Left = 196
        Top = 102
        Width = 285
        Height = 22
        Style = csDropDownList
        DropDownCount = 10
        TabOrder = 1
        OnChange = Advanced_DisplayCaptions_SelChange
        Items.Strings = (
          'When content is detected'
          'When content or a command is detected'
          'Even when no content or command is detected')
      end
    end
    object Customize_Language: TTabSheet
      Caption = 'Language'
      ImageIndex = 3
      OnShow = Customize_LanguageShow
      object Language_Caption: TLabel
        Left = 0
        Top = 0
        Width = 154
        Height = 14
        Caption = 'Choose your desired language :'
      end
      object Language_New: TButton
        Left = 162
        Top = 43
        Width = 80
        Height = 23
        Caption = 'New...'
        TabOrder = 3
        OnClick = Language_NewClick
      end
      object Language_Delete: TButton
        Left = 0
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Delete'
        TabOrder = 1
        OnClick = Language_DeleteClick
      end
      object Language_Edit: TButton
        Left = 81
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Edit...'
        TabOrder = 2
        OnClick = Language_EditClick
      end
      object Language_Sel: TComboBox
        Left = 0
        Top = 17
        Width = 242
        Height = 22
        Style = csDropDownList
        TabOrder = 0
        OnChange = Language_SelChange
      end
    end
    object Customize_Sheet: TTabSheet
      Caption = 'Sheet'
      ImageIndex = 4
      OnShow = Customize_SheetShow
      object Sheet_Caption: TLabel
        Left = 0
        Top = 0
        Width = 175
        Height = 14
        Caption = 'Choose your desired custom sheet :'
      end
      object Sheet_Sel: TComboBox
        Left = 0
        Top = 17
        Width = 242
        Height = 22
        Style = csDropDownList
        TabOrder = 0
        OnChange = Sheet_SelChange
      end
      object Sheet_Delete: TButton
        Left = 0
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Delete'
        TabOrder = 1
        OnClick = Sheet_DeleteClick
      end
      object Sheet_Edit: TButton
        Left = 81
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Edit...'
        TabOrder = 2
        OnClick = Sheet_EditClick
      end
      object Sheet_New: TButton
        Left = 162
        Top = 43
        Width = 80
        Height = 23
        Caption = 'New...'
        TabOrder = 3
        OnClick = Sheet_NewClick
      end
    end
    object Customize_TreeText: TTabSheet
      Caption = 'Tree && Text'
      ImageIndex = 6
      object Tree_NotYet: TLabel
        Left = 362
        Top = 78
        Width = 43
        Height = 14
        Caption = 'Not yet...'
      end
    end
    object Customize_Graph: TTabSheet
      Caption = 'Graph'
      ImageIndex = 8
      OnShow = Customize_GraphShow
      object Graph_Adm_ShowTrackUIDs: TCheckBox
        Left = 0
        Top = 0
        Width = 268
        Height = 18
        Caption = 'ADM: Show TrackUIDs'
        TabOrder = 0
        OnClick = Graph_Adm_ShowTrackUIDsClick
      end
      object Graph_Adm_ShowChannelFormats: TCheckBox
        Left = 0
        Top = 16
        Width = 268
        Height = 18
        Caption = 'ADM: Show ChannelFormats'
        TabOrder = 1
        OnClick = Graph_Adm_ShowChannelFormatsClick
      end
    end
    object Customize_Custom: TTabSheet
      Caption = 'Custom Text'
      ImageIndex = 6
      OnShow = Customize_CustomShow
      object Custom_Caption: TLabel
        Left = 0
        Top = 0
        Width = 166
        Height = 14
        Caption = 'Choose your desired custom text :'
      end
      object Custom_New: TButton
        Left = 162
        Top = 43
        Width = 80
        Height = 23
        Caption = 'New...'
        TabOrder = 0
        OnClick = Custom_NewClick
      end
      object Custom_Edit: TButton
        Left = 81
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Edit...'
        TabOrder = 1
        OnClick = Custom_EditClick
      end
      object Custom_Delete: TButton
        Left = 0
        Top = 43
        Width = 81
        Height = 23
        Caption = 'Delete'
        TabOrder = 2
        OnClick = Custom_DeleteClick
      end
      object Custom_Sel: TComboBox
        Left = 0
        Top = 17
        Width = 242
        Height = 22
        Style = csDropDownList
        TabOrder = 3
        OnChange = Custom_SelChange
      end
    end
    object Customize: TTabSheet
      Caption = 'Customize'
      ImageIndex = 7
    end
  end
end
