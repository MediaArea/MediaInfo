object PreferencesF: TPreferencesF
  Left = 286
  Top = 180
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Preferences'
  ClientHeight = 256
  ClientWidth = 850
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Position = poOwnerFormCenter
  OnShow = FormShow
  DesignSize = (
    850
    256)
  TextHeight = 14
  object OK: TButton
    Left = 720
    Top = 223
    Width = 122
    Height = 27
    Anchors = [akRight, akBottom]
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
    Height = 215
    Anchors = [akLeft, akTop, akRight, akBottom]
    Indent = 19
    ParentShowHint = False
    ReadOnly = True
    ShowHint = False
    TabOrder = 1
    OnChange = TreeChange
    Items.NodeData = {
      070200000009540054007200650065004E006F00640065002900000000000000
      00000000FFFFFFFFFFFFFFFFFFFFFFFF00000000000100000001055300650074
      007500700000002F0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF00
      0000000000000000010841006400760061006E00630065006400000031000000
      0000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000005000000010943
      007500730074006F006D0069007A0065000000290000000000000000000000FF
      FFFFFFFFFFFFFFFFFFFFFF000000000000000000010553006800650065007400
      0000350000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000010B540072006500650020002600200054006500780074000000350000
      000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000000000010B
      43007500730074006F006D002000740065007800740000002900000000000000
      00000000FFFFFFFFFFFFFFFFFFFFFFFF00000000000000000001054700720061
      007000680000002F0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF00
      000000000000000001084C0061006E0067007500610067006500}
  end
  object Cancel: TButton
    Left = 593
    Top = 223
    Width = 121
    Height = 27
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object Page: TPageControl
    Left = 176
    Top = 0
    Width = 666
    Height = 217
    ActivePage = Setup_Advanced
    Anchors = [akLeft, akTop, akRight, akBottom]
    MultiLine = True
    TabHeight = 22
    TabOrder = 3
    object Setup: TTabSheet
      Caption = 'General'
      OnShow = Setup_GeneralShow
      object Langue_C: TLabel
        Left = 2
        Top = 7
        Width = 54
        Height = 14
        Caption = 'Language :'
      end
      object Output_C: TLabel
        Left = 2
        Top = 33
        Width = 38
        Height = 14
        Caption = 'Output :'
      end
      object General_Language_Sel: TComboBox
        Left = 164
        Top = 4
        Width = 182
        Height = 22
        Style = csDropDownList
        TabOrder = 0
        OnChange = General_Language_SelChange
      end
      object General_Output_Sel: TComboBox
        Left = 164
        Top = 30
        Width = 182
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
        Left = 2
        Top = 56
        Width = 655
        Height = 18
        Caption = 'Check for newest versions (require Internet connection)'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = CB_CheckUpdateClick
      end
      object General_Language_More: TButton
        Left = 352
        Top = 4
        Width = 105
        Height = 23
        Caption = 'More...'
        TabOrder = 6
        OnClick = General_Language_MoreClick
      end
      object General_Output_More: TButton
        Left = 352
        Top = 30
        Width = 105
        Height = 22
        Caption = 'More...'
        TabOrder = 7
        OnClick = General_Output_MoreClick
      end
      object CB_InfoTip: TCheckBox
        Left = 2
        Top = 135
        Width = 655
        Height = 18
        Caption = 
          'Shell InfoTip (in explorer, move the mouse on file, info will be' +
          ' displayed)'
        TabOrder = 5
        OnClick = CB_InfoTipClick
      end
      object CB_InscrireShell: TCheckBox
        Left = 2
        Top = 72
        Width = 655
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
        Left = 18
        Top = 88
        Width = 639
        Height = 17
        Caption = 'For folders too'
        TabOrder = 4
        OnClick = CB_InscrireShell_FolderClick
      end
      object CB_InscrireShell_SeparateInstance: TCheckBox
        Left = 18
        Top = 104
        Width = 639
        Height = 17
        Caption = 'Open each item in a separate instance'
        TabOrder = 8
        Visible = False
      end
      object CB_InscrireShell_RetainLegacy: TCheckBox
        Left = 18
        Top = 120
        Width = 639
        Height = 17
        Caption = 'Retain legacy shell extension'
        TabOrder = 9
        Visible = False
        OnClick = CB_InscrireShell_RetainLegacyClick
      end
    end
    object Setup_Advanced: TTabSheet
      Caption = 'Advanced'
      ImageIndex = 1
      OnShow = Setup_AdvancedShow
      object Advanced_DisplayCaptions_Caption: TLabel
        Left = 2
        Top = 158
        Width = 141
        Height = 14
        Caption = 'Handling of 608/708 streams:'
      end
      object CB_ShowToolBar: TCheckBox
        Left = 2
        Top = 1
        Width = 658
        Height = 18
        Caption = 'Show toolbar'
        TabOrder = 0
        OnClick = CB_ShowToolBarClick
      end
      object Advanced_CloseAllAuto: TCheckBox
        Left = 2
        Top = 65
        Width = 658
        Height = 18
        Caption = 'Close all before open'
        TabOrder = 6
        OnClick = Advanced_CloseAllAutoClick
      end
      object CB_ShowMenu: TCheckBox
        Left = 2
        Top = 17
        Width = 658
        Height = 18
        Caption = 'Show menu'
        TabOrder = 2
        OnClick = CB_ShowMenuClick
      end
      object Advanced_InformVersion: TCheckBox
        Left = 2
        Top = 81
        Width = 658
        Height = 18
        Caption = 'Add version to text output'
        TabOrder = 4
        OnClick = Advanced_InformVersionClick
      end
      object Advanced_InformTimestamp: TCheckBox
        Left = 2
        Top = 97
        Width = 658
        Height = 18
        Caption = 'Add creation date to text output'
        TabOrder = 7
        OnClick = Advanced_InformTimestampClick
      end
      object Advanced_EnableFfmpeg: TCheckBox
        Left = 2
        Top = 113
        Width = 658
        Height = 18
        Caption = 'Enable FFmpeg plugin'
        TabOrder = 5
        OnClick = Advanced_EnableFfmpegClick
      end
      object Advanced_DisplayCaptions_Sel: TComboBox
        Left = 303
        Top = 154
        Width = 352
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
      object Advanced_LegacyStreamDisplay: TCheckBox
        Left = 2
        Top = 129
        Width = 658
        Height = 18
        Caption = 'LegacyStreamDisplay'
        TabOrder = 8
        OnClick = Advanced_LegacyStreamDisplayClick
      end
      object CB_RememberWindowPosition: TCheckBox
        Left = 2
        Top = 33
        Width = 279
        Height = 17
        Caption = 'Remember window position'
        TabOrder = 9
        OnClick = CB_RememberWindowPositionClickClick
      end
      object CB_RememberWindowDimensions: TCheckBox
        Left = 2
        Top = 49
        Width = 279
        Height = 17
        Caption = 'Remember window dimensions'
        TabOrder = 3
        OnClick = CB_RememberWindowDimensionsClickClick
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
        Left = 2
        Top = 3
        Width = 658
        Height = 18
        Caption = 'ADM: Show TrackUIDs'
        TabOrder = 0
        OnClick = Graph_Adm_ShowTrackUIDsClick
      end
      object Graph_Adm_ShowChannelFormats: TCheckBox
        Left = 2
        Top = 19
        Width = 658
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
