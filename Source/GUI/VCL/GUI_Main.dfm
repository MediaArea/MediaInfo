object MainF: TMainF
  Left = 223
  Top = 273
  Caption = 'MainF'
  ClientHeight = 631
  ClientWidth = 793
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Page: TTntPageControl
    Left = 40
    Top = 0
    Width = 690
    Height = 385
    ActivePage = Page_Text
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    TabHeight = 20
    TabOrder = 0
    TabStop = False
    object Page_Easy: TTntTabSheet
      Caption = 'Easy'
      object Page_Easy_Note: TTntLabel
        Left = 0
        Top = 318
        Width = 657
        Height = 15
        Alignment = taRightJustify
        AutoSize = False
        Caption = 
          'Note : this is a basic view. For more information about this fil' +
          'e, you must select a different view (Sheet, Tree...)'
      end
      object Page_Easy_A2: TTntGroupBox
        Left = 340
        Top = 200
        Width = 340
        Height = 56
        Caption = 'Second audio stream'
        TabOrder = 4
        object Page_Easy_A2_Codec: TTntLabel
          Left = 9
          Top = 17
          Width = 36
          Height = 15
          Caption = 'Codec'
        end
        object Page_Easy_A2_Web: TTntButton
          Left = 4
          Top = 32
          Width = 314
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
      object Page_Easy_File: TTntComboBox
        Left = 0
        Top = 0
        Width = 657
        Height = 23
        Style = csDropDownList
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 1
        OnChange = Page_Easy_FileChange
      end
      object Page_Easy_V1: TTntGroupBox
        Left = 0
        Top = 144
        Width = 680
        Height = 56
        Caption = 'First video stream'
        TabOrder = 2
        object Page_Easy_V1_Codec: TTntLabel
          Left = 9
          Top = 17
          Width = 36
          Height = 15
          Caption = 'Codec'
        end
        object Page_Easy_V1_Web: TTntButton
          Left = 4
          Top = 32
          Width = 322
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
      object Page_Easy_A1: TTntGroupBox
        Left = 0
        Top = 200
        Width = 340
        Height = 56
        Caption = 'First audio stream'
        TabOrder = 3
        object Page_Easy_A1_Codec: TTntLabel
          Left = 9
          Top = 17
          Width = 36
          Height = 15
          Caption = 'Codec'
        end
        object Page_Easy_A1_Web: TTntButton
          Left = 4
          Top = 32
          Width = 322
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
      object Page_Easy_T2: TTntGroupBox
        Left = 228
        Top = 256
        Width = 225
        Height = 56
        Caption = 'Second text stream'
        TabOrder = 5
        object Page_Easy_T2_Codec: TTntLabel
          Left = 9
          Top = 16
          Width = 36
          Height = 15
          Caption = 'Codec'
        end
        object Page_Easy_T2_Web: TTntButton
          Left = 4
          Top = 32
          Width = 215
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
      object Page_Easy_T1: TTntGroupBox
        Left = 0
        Top = 256
        Width = 228
        Height = 56
        Caption = 'First text stream'
        TabOrder = 6
        object Page_Easy_T1_Codec: TTntLabel
          Left = 9
          Top = 17
          Width = 36
          Height = 15
          Caption = 'Codec'
        end
        object Page_Easy_T1_Web: TTntButton
          Left = 4
          Top = 32
          Width = 215
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
      object Page_Easy_FileSelect: TTntButton
        Left = 658
        Top = 0
        Width = 21
        Height = 23
        Caption = '...'
        TabOrder = 7
        OnClick = M_File_Open_FileClick
      end
      object Page_Easy_G1: TTntGroupBox
        Left = 0
        Top = 24
        Width = 680
        Height = 121
        Caption = 'Container and general information'
        TabOrder = 0
        object Page_Easy_G1_Codec: TTntLabel
          Left = 5
          Top = 16
          Width = 39
          Height = 15
          AutoSize = False
          Caption = 'Format'
        end
        object Page_Easy_General_List_V: TTntLabel
          Left = 5
          Top = 32
          Width = 31
          Height = 15
          AutoSize = False
          Caption = 'Video'
        end
        object Page_Easy_General_List_A: TTntLabel
          Left = 5
          Top = 48
          Width = 31
          Height = 15
          AutoSize = False
          Caption = 'Audio'
        end
        object Page_Easy_General_List_T: TTntLabel
          Left = 5
          Top = 64
          Width = 22
          Height = 15
          AutoSize = False
          Caption = 'Text'
        end
        object Page_Easy_General_List_C: TTntLabel
          Left = 5
          Top = 80
          Width = 51
          Height = 15
          AutoSize = False
          Caption = 'Chapters'
        end
        object Page_Easy_General_Tag1: TTntLabel
          Left = 330
          Top = 16
          Width = 28
          Height = 15
          AutoSize = False
          Caption = 'Tag1'
        end
        object Page_Easy_General_Tag2: TTntLabel
          Left = 330
          Top = 32
          Width = 28
          Height = 15
          AutoSize = False
          Caption = 'Tag2'
        end
        object Page_Easy_General_Tag3: TTntLabel
          Left = 330
          Top = 48
          Width = 28
          Height = 15
          AutoSize = False
          Caption = 'Tag3'
        end
        object Page_Easy_General_Tag4: TTntLabel
          Left = 330
          Top = 64
          Width = 28
          Height = 15
          AutoSize = False
          Caption = 'Tag4'
        end
        object Page_Easy_General_Tag5: TTntLabel
          Left = 330
          Top = 80
          Width = 28
          Height = 15
          AutoSize = False
          Caption = 'Tag5'
        end
        object Page_Easy_G1_Web: TTntButton
          Left = 4
          Top = 96
          Width = 323
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
      object Page_Easy_DifferentView: TTntButton
        Left = 656
        Top = 312
        Width = 27
        Height = 25
        Caption = '-->'
        PopupMenu = ToolBar_View_Menu
        TabOrder = 8
        OnClick = Page_Easy_DifferentViewClick
      end
      object Page_Easy_T3: TTntGroupBox
        Left = 456
        Top = 256
        Width = 225
        Height = 56
        Caption = 'Third text stream'
        TabOrder = 9
        object Page_Easy_T3_Codec: TTntLabel
          Left = 9
          Top = 16
          Width = 36
          Height = 15
          Caption = 'Codec'
        end
        object Page_Easy_T3_Web: TTntButton
          Left = 4
          Top = 32
          Width = 215
          Height = 21
          Hint = 'Go to the web site of a player for this file'
          Caption = 'Go to the web site of a player for this file'
          TabOrder = 0
          Visible = False
          OnClick = Page_Easy_WebClick
        end
      end
    end
    object Page_Sheet: TTntTabSheet
      Caption = 'Sheet'
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = False
      object Page_Sheet_A_Web: TTntButton
        Left = 566
        Top = 173
        Width = 38
        Height = 21
        Hint = 'Go to the Web site for this codec'
        Caption = 'Web'
        Enabled = False
        TabOrder = 0
        OnClick = Page_Sheet_WebClick
      end
      object Page_Sheet_V_Web: TTntButton
        Left = 566
        Top = 152
        Width = 38
        Height = 20
        Hint = 'Go to the Web site for this codec'
        Caption = 'Web'
        Enabled = False
        TabOrder = 1
        OnClick = Page_Sheet_WebClick
      end
      object Page_Sheet_G_Web: TTntButton
        Left = 566
        Top = 130
        Width = 38
        Height = 20
        Hint = 'Go to the web site of a player for this file'
        Caption = 'Web'
        Enabled = False
        TabOrder = 2
        OnClick = Page_Sheet_WebClick
      end
      object Page_Sheet_T: TTntComboBox
        Left = 0
        Top = 198
        Width = 561
        Height = 23
        Enabled = False
        ItemHeight = 15
        TabOrder = 3
        OnChange = Page_Sheet_Change
        OnEnter = Page_Sheet_Change
      end
      object Page_Sheet_T_Web: TTntButton
        Left = 566
        Top = 199
        Width = 38
        Height = 20
        Hint = 'Go to the Web site for this codec'
        Caption = 'Web'
        Enabled = False
        TabOrder = 4
        OnClick = Page_Sheet_WebClick
      end
      object Page_Sheet_C: TTntComboBox
        Left = 0
        Top = 224
        Width = 561
        Height = 23
        Enabled = False
        ItemHeight = 15
        TabOrder = 5
        OnChange = Page_Sheet_Change
        OnEnter = Page_Sheet_Change
      end
      object Page_Sheet_C_Web: TTntButton
        Left = 566
        Top = 225
        Width = 38
        Height = 20
        Hint = 'Go to the Web site for this codec'
        Caption = 'Web'
        Enabled = False
        TabOrder = 6
        OnClick = Page_Sheet_WebClick
      end
      object Page_Sheet_Text: TTntMemo
        Left = 0
        Top = 248
        Width = 604
        Height = 105
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'System'
        Font.Pitch = fpFixed
        Font.Style = []
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 7
      end
      object Page_Sheet_G: TTntComboBox
        Left = 0
        Top = 127
        Width = 561
        Height = 23
        ItemHeight = 15
        TabOrder = 8
        OnChange = Page_Sheet_Change
        OnEnter = Page_Sheet_Change
      end
      object Page_Sheet_V: TTntComboBox
        Left = 0
        Top = 149
        Width = 561
        Height = 23
        Enabled = False
        ItemHeight = 15
        TabOrder = 9
        OnChange = Page_Sheet_Change
        OnEnter = Page_Sheet_Change
      end
      object Page_Sheet_A: TTntComboBox
        Left = 0
        Top = 173
        Width = 561
        Height = 23
        Enabled = False
        ItemHeight = 15
        TabOrder = 10
        OnChange = Page_Sheet_Change
        OnEnter = Page_Sheet_Change
      end
      object Page_Sheet_Sheet: TTntStringGrid
        Left = 0
        Top = 0
        Width = 604
        Height = 121
        DefaultRowHeight = 16
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
        TabOrder = 11
        OnSelectCell = Page_Sheet_SheetSelectCell
      end
    end
    object Page_Tree: TTntTabSheet
      Hint = 'Tree form'
      Caption = 'Tree'
      ImageIndex = 1
      ParentShowHint = False
      ShowHint = False
      object Page_Tree_Tree: TTntTreeView
        Left = 0
        Top = 2
        Width = 628
        Height = 378
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = []
        Indent = 19
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = False
        TabOrder = 0
      end
    end
    object Page_Text: TTntTabSheet
      Hint = 'Text form'
      Caption = 'Text'
      ParentShowHint = False
      ShowHint = False
      object Page_Text_Text: TTntMemo
        Left = 3
        Top = 0
        Width = 637
        Height = 352
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'System'
        Font.Pitch = fpFixed
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ScrollBars = ssBoth
        ShowHint = True
        TabOrder = 0
      end
    end
    object Page_HTML: TTntTabSheet
      Caption = 'HTML'
      ImageIndex = 6
      object Page_HTML_HTML: TCppWebBrowser
        Left = 4
        Top = 0
        Width = 673
        Height = 385
        TabOrder = 0
        ControlData = {
          4C0000008E450000CA2700000000000000000000000000000000000000000000
          000000004C000000000000000000000001000000E0D057007335CF11AE690800
          2B2E126208000000000000004C0000000114020000000000C000000000000046
          8000000000000000000000000000000000000000000000000000000000000000
          00000000000000000100000000000000000000000000000000000000}
      end
    end
    object Page_Custom: TTntTabSheet
      Caption = 'Custom text'
      ImageIndex = 4
      ParentShowHint = False
      ShowHint = False
      object Page_Custom_Text: TTntMemo
        Left = 2
        Top = 2
        Width = 326
        Height = 370
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'System'
        Font.Pitch = fpFixed
        Font.Style = []
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 0
      end
      object Page_Custom_HTML: TCppWebBrowser
        Left = 336
        Top = 0
        Width = 300
        Height = 369
        TabOrder = 1
        ControlData = {
          4C000000021F0000232600000000000000000000000000000000000000000000
          000000004C000000000000000000000001000000E0D057007335CF11AE690800
          2B2E126208000000000000004C0000000114020000000000C000000000000046
          8000000000000000000000000000000000000000000000000000000000000000
          00000000000000000100000000000000000000000000000000000000}
      end
    end
    object Page_System: TTntTabSheet
      Caption = 'Your system'
      ImageIndex = 3
      object Page_System_Buttons: TTntGroupBox
        Left = 9
        Top = 0
        Width = 285
        Height = 30
        TabOrder = 0
        object Page_System_Buttons_Video: TTntRadioButton
          Left = 3
          Top = 9
          Width = 70
          Height = 18
          Caption = 'Video'
          Checked = True
          TabOrder = 0
          TabStop = True
          OnClick = Page_System_Buttons_VideoClick
        end
        object Page_System_Buttons_Audio: TTntRadioButton
          Left = 95
          Top = 9
          Width = 70
          Height = 18
          Caption = 'Audio'
          TabOrder = 1
          OnClick = Page_System_Buttons_AudioClick
        end
        object Page_System_Buttons_Text: TTntRadioButton
          Left = 190
          Top = 9
          Width = 78
          Height = 18
          Caption = 'Subtitle'
          TabOrder = 2
          OnClick = Page_System_Buttons_TextClick
        end
      end
      object Page_System_Sheet: TTntListView
        Left = 9
        Top = 30
        Width = 603
        Height = 329
        Columns = <
          item
            Caption = 'Id'
            Width = 48
          end
          item
            Caption = 'Name'
            Width = 162
          end
          item
            Caption = 'More info'
            Width = 215
          end
          item
            Caption = 'Web'
            Width = 54
          end
          item
            Caption = 'Supported?'
            Width = 75
          end>
        RowSelect = True
        SortType = stText
        TabOrder = 1
        ViewStyle = vsReport
        OnColumnClick = Page_System_SheetColumnClick
        OnCompare = Page_System_SheetCompare
      end
    end
  end
  object ToolBar: TTntToolBar
    Left = 0
    Top = 0
    Width = 42
    Height = 631
    Align = alLeft
    ButtonHeight = 39
    ButtonWidth = 39
    Caption = 'ToolBar'
    Color = clNone
    DisabledImages = ToolBar_Image_Disabled
    EdgeBorders = [ebRight]
    EdgeInner = esNone
    EdgeOuter = esNone
    Images = ToolBar_Image
    ParentColor = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    Transparent = True
    ExplicitHeight = 611
    object Tool_File: TTntToolButton
      Left = 0
      Top = 0
      Hint = 'Select a multimedia file to study'
      Caption = 'File'
      ImageIndex = 0
      MenuItem = M_File_Open_File
      ParentShowHint = False
      Wrap = True
      ShowHint = True
    end
    object Tool_Folder: TTntToolButton
      Left = 0
      Top = 39
      Hint = 'Select a folder to study'
      Caption = 'Folder'
      ImageIndex = 1
      MenuItem = M_File_Open_Folder
      Wrap = True
    end
    object Tool_Export: TTntToolButton
      Left = 0
      Top = 78
      Caption = 'Export...'
      ImageIndex = 2
      MenuItem = M_File_Export
      Wrap = True
    end
    object Tool_Options: TTntToolButton
      Left = 0
      Top = 117
      Hint = 'Change your preferences...'
      Caption = 'Preferences...'
      ImageIndex = 3
      MenuItem = M_Options_Preferences
      Wrap = True
    end
    object Tool_About: TTntToolButton
      Left = 0
      Top = 156
      Hint = 'About MediaInfo'
      Caption = 'About'
      ImageIndex = 4
      MenuItem = M_Help_About
      Wrap = True
    end
    object Tool_View: TTntToolButton
      Left = 0
      Top = 195
      Caption = 'Tool_View'
      DropdownMenu = ToolBar_View_Menu
      ImageIndex = 5
    end
  end
  object MainMenu: TTntMainMenu
    Images = Menu_Image
    Left = 740
    Top = 76
    object M_File: TTntMenuItem
      Caption = 'File'
      ImageIndex = 0
      object M_File_Open: TTntMenuItem
        Caption = 'Open'
        ImageIndex = 0
        object M_File_Open_File: TTntMenuItem
          Caption = 'File'
          Hint = 'Select a multimedia file to study'
          ImageIndex = 0
          ShortCut = 32847
          OnClick = M_File_Open_FileClick
        end
        object M_File_Open_Folder: TTntMenuItem
          Caption = 'Folder'
          Hint = 'Select a folder to study'
          ImageIndex = 1
          ShortCut = 49231
          OnClick = M_File_Open_FolderClick
        end
      end
      object M_File_Close: TTntMenuItem
        Caption = 'Close'
        object M_File_Close_File: TTntMenuItem
          Caption = 'File'
          ShortCut = 32835
          OnClick = M_File_Close_FileClick
        end
        object M_File_Close_All: TTntMenuItem
          Caption = 'All'
          ShortCut = 49219
          OnClick = M_File_Close_AllClick
        end
      end
      object M_File_Z4: TTntMenuItem
        Caption = '-'
      end
      object M_File_Export: TTntMenuItem
        Caption = 'Export...'
        ImageIndex = 2
        ShortCut = 32837
        OnClick = M_File_ExportClick
      end
      object M_File_Z5: TTntMenuItem
        Caption = '-'
      end
      object M_File_Exit: TTntMenuItem
        Caption = 'Exit'
        ShortCut = 32856
        OnClick = M_File_ExitClick
      end
    end
    object M_View: TTntMenuItem
      Caption = 'View'
      ImageIndex = 5
      object M_View_Easy: TTntMenuItem
        AutoCheck = True
        Caption = 'Easy'
        RadioItem = True
        OnClick = M_View_EasyClick
      end
      object M_View_Sheet: TTntMenuItem
        AutoCheck = True
        Caption = 'Sheet'
        Hint = 'Sheet form'
        RadioItem = True
        OnClick = M_View_SheetClick
      end
      object M_View_Tree: TTntMenuItem
        AutoCheck = True
        Caption = 'Tree'
        Hint = 'Tree form'
        RadioItem = True
        OnClick = M_View_TreeClick
      end
      object M_View_Text: TTntMenuItem
        AutoCheck = True
        Caption = 'Text'
        Hint = 'Text form'
        RadioItem = True
        OnClick = M_View_TextClick
      end
      object M_View_HTML: TTntMenuItem
        AutoCheck = True
        Caption = 'HTML'
        RadioItem = True
        OnClick = M_View_HTMLClick
      end
      object M_View_Custom: TTntMenuItem
        AutoCheck = True
        Caption = 'Custom'
        Hint = 'Custom form'
        RadioItem = True
        OnClick = M_View_CustomClick
      end
      object N1: TTntMenuItem
        Caption = '-'
      end
      object M_View_System: TTntMenuItem
        AutoCheck = True
        Caption = 'Your system'
        Hint = 'Give info about you system'
        ImageIndex = 6
        RadioItem = True
        OnClick = M_View_SystemClick
      end
    end
    object M_Options: TTntMenuItem
      Caption = 'Options'
      ImageIndex = 3
      object M_Options_ShowToolBar: TTntMenuItem
        AutoCheck = True
        Caption = 'Show toolbar'
        Checked = True
        Hint = 'Show toolbar with buttons'
        OnClick = M_Options_ShowToolBarClick
      end
      object M_Options_ShowMenu: TTntMenuItem
        AutoCheck = True
        Caption = 'Show menu'
        Checked = True
        Hint = 'Show Menu'
        OnClick = M_Options_ShowMenuClick
      end
      object N5: TTntMenuItem
        Caption = '-'
      end
      object M_Options_CloseAllAuto: TTntMenuItem
        AutoCheck = True
        Caption = 'Close all before Open'
      end
      object M_Options_Z1: TTntMenuItem
        Caption = '-'
      end
      object M_Options_Preferences: TTntMenuItem
        Caption = 'Preferences...'
        Hint = 'Change your preferences...'
        ImageIndex = 3
        ShortCut = 32848
        OnClick = M_Options_PreferencesClick
      end
    end
    object M_Debug: TTntMenuItem
      Caption = 'Debug'
      ImageIndex = 8
      object M_Debug_Header: TTntMenuItem
        Caption = 'Create a header file'
        Hint = 'Create a 10K to send me'
        OnClick = M_Debug_HeaderClick
      end
      object M_Debug_Advanced: TTntMenuItem
        Caption = 'Advanced mode'
        Hint = 'Give more information about file'
        OnClick = M_Debug_AdvancedClick
      end
      object N6: TTntMenuItem
        Caption = '-'
      end
      object M_Debug_Avanced_More: TTntMenuItem
        Caption = 'Advanced mode -->'
        Visible = False
        object M_Debug_Dummy: TTntMenuItem
          Caption = 'Fill with a realistic movie'
          OnClick = M_Debug_DummyClick
        end
        object M_Debug_Dummy_Movie: TTntMenuItem
          Caption = 'Fill with a Dummy movie'
          OnClick = M_Debug_Dummy_MovieClick
        end
      end
      object N7: TTntMenuItem
        Caption = '-'
      end
      object M_Debug_Details0: TTntMenuItem
        AutoCheck = True
        Caption = 'Details - 0'
        Checked = True
        RadioItem = True
        OnClick = M_View_TextClick
      end
      object M_Debug_Details50: TTntMenuItem
        AutoCheck = True
        Caption = 'Details - 5'
        RadioItem = True
        OnClick = M_View_TextClick
      end
      object M_Debug_Details90: TTntMenuItem
        AutoCheck = True
        Caption = 'Details - 9'
        RadioItem = True
        OnClick = M_View_TextClick
      end
      object M_Debug_Details100: TTntMenuItem
        AutoCheck = True
        Caption = 'Details - 10'
        RadioItem = True
        OnClick = M_View_TextClick
      end
    end
    object M_Help: TTntMenuItem
      Caption = 'Help'
      ImageIndex = 4
      object M_Help_About: TTntMenuItem
        Caption = 'About'
        Hint = 'About MediaInfo'
        ImageIndex = 4
        ShortCut = 32833
        OnClick = M_Help_AboutClick
      end
      object N2: TTntMenuItem
        Caption = '-'
      end
      object M_Help_SupportedFormats: TTntMenuItem
        Caption = 'Known formats'
        OnClick = M_Help_SupportedFormatsClick
      end
      object M_Help_SupportedCodecs: TTntMenuItem
        Caption = 'Known codecs'
        OnClick = M_Help_SupportedCodecsClick
      end
      object M_Help_SupportedParameters: TTntMenuItem
        Caption = 'Known parameters'
        OnClick = M_Help_SupportedParametersClick
      end
    end
    object M_Language: TTntMenuItem
      Caption = 'Language'
      ImageIndex = 7
      object ToDo1: TTntMenuItem
        Caption = 'ToDo...'
      end
    end
  end
  object Menu_Image: TImageList
    Left = 744
    Top = 136
    Bitmap = {
      494C010109000E00040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000004000000001002000000000000040
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E7E7E7FFCACACAFFC6C6C6FFDEDEDEFFFBFBFBFF0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000BFBFBFFF878787FFE5E5E5FFDADADAFFE7E7E7FFBBBBBBFF444444FF9F9F
      9FFFFBFBFBFF0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008383
      83FF9D9D9DFFA7A7A7FFA0A09FFF838485FF8F8F8FFFAFAFAFFF757575FFDEDE
      DEFF616161FFF3F3F3FF00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000009B9B9BFF7A7A
      7AFF7C7C7CFFC3C5C5FFCBCBCBFFCFD1D1FFCBD3D6FFCCCECEFF808180FFBEBD
      BDFFB2B2B2FF646464FFFDFDFDFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C6FF727272FF6A6B
      6BFFD7DADAFFD9DAD9FFE6E7E6FFCED0D1FFE0E3E3FFDDE0E1FFCDCFCEFF5F60
      5FFFC5C5C5FFACACACFFABABABFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000969696FFB4B4B4FFCFD0
      CFFFD1D6D9FFD5D8D7FF7E7D7DFFC4C7C7FFD9DDDCFFD7DBDAFFD3D8DAFFBEC2
      C2FF626361FF838383FF4B4B4BFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007B7B7BFF6D6C6BFFBFC0
      BEFFC4CACAFFCDD2D1FFFFFFFFFF232424FF8F9291FFD2D8D7FFCFD1D0FFC9CE
      CFFFB7BBBAFFC5C5C5FF666666FFEEEEEEFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DDDDDDFFBABABAFF8F8B8BFFC8CB
      CBFFC3C7C8FFD3D8D7FFE2E4E3FFFFFFFFFF181715FF8B8D8DFFC9D0CDFFC6CB
      CAFFB2B3B3FF878686FF6F6F6FFFDEDEDEFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000D1D1D1FFD4D4D4FF8D8C8AFFAEB0
      AFFFCFD2D1FF9B9C9BFF626463FF8D8DFFFF3C3CC3FF727272FFBABCBBFFA3A8
      A8FFA7ACAAFF767677FF686868FFE4E4E4FF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F7F7F7FFC3C3C3FF7D7B7AFFCED0
      D0FFE2E4E2FFE3E3E2FF898A89FF8A8B7FFF6464FFFFB2B4AFFFDEE2E1FFC3CA
      C9FFA4A8A9FFA5A5A5FF5C5C5CFFF8F8F8FF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000848484FF636363FFB2B5
      B4FFE9EAEBFFECECEAFFE8E9E9FFC5C6C6FFECEDEAFFA1A2C6FFE7EAE9FFCED3
      D1FFB3B8B7FFF7F7F7FF5B5B5BFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000747474FFFFFFFFFF8182
      80FF979898FFF0F1F0FFF3F3F2FFEFF1F0FFF1F2F2FFE4E4F5FFF8FAF8FFC7CE
      CDFF717272FF9C9C9CFFE4E4E4FF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000868686FFDBDB
      DBFF7E807FFFF2F2F2FFF9F9F9FFF7F7F7FFF9F9F9FFF8F9F9FFCCCFCEFF5F60
      60FFFCFCFCFF7D7D7DFF00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000A1A1
      A1FFFFFFFFFF606061FF838382FFC3C4C3FFB5B3B2FF656766FFBBBABBFFFBFB
      FBFF777777FF0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000878787FFE8E8E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAEAEAEFFE0E0
      E0FF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F9F9F9FFDDDDDDFFE5E5E5FF00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000ECEC
      EC00E0E2E000EDEDED00FCFCFC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000F4F4F5FFFDFDFDFF00000000FDFDFDFFF8F8F8FF000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F8F8F800C8CFC80098A9
      98008C9F8C0092A69300B3BEB400E9E9E9000000000000000000000000000000
      00000000000000000000000000000000000000000000E0E0E1FFFAFAFAFF0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000083A2BAFF2296F7FF43749DFFF9F9F9FFD0ADB1FFD8BABDFF8C6C6FFF9A96
      97FFE1E0E0FFFDFDFDFF00000000000000000000000000000000000000000000
      0000FCFCFCFFD6D0CFFF988A7CFF8B7966FF8C7D6DFFB9AEABFFEFECEBFF0000
      00000000000000000000000000000000000000000000B8C2B80092A59400F2F3
      F200D7DCD800F4F5F400BBC4BB007B8C7D00EAEAEA0000000000000000000000
      0000000000000000000000000000000000004657C9FF98ADFEFF818EE0FF6062
      8EFFA1A1A4FFE6E6E8FFFEFEFEFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00001F96F0FF1F98F2FF1C9FF5FFC08080FFFCE8E5FFFEF4F3FFFEF4F4FFFEF4
      F3FFF6E8E8FFC09A9DFFACABABFF00000000000000000000000000000000D2CB
      CAFF987658FFCB927BFFCB907FFFC58E81FFCB9288FFCC9380FFAB8564FF998A
      7FFFF9F8F8FF00000000000000000000000000000000B6C1B700899D8A00CCCE
      CC0071857300C2C2C000A2ADA2007C8B7900C8CBC7000000000000000000FEFE
      FE00ECECEC00CFCFCF00ECECEC00000000003B52D5FF7D97FFFF849DFFFF8499
      F7FF454EC5FF3944C4FF494DABFF757485FFFEFEFEFF00000000000000000000
      00000000000000000000000000000000000000000000F9F8F7FF95786CFF9A93
      91FF19A4F1FF14ACF1FF1BA3F5FFF4B3ACFFFCE6E4FFFCE9E7FFFCE5E2FFF6E0
      DCFFFCE4E2FFFCE7E5FF8C7E80FF000000000000000000000000B7ACA6FFD0A0
      84FFC59477FFC7987FFFBA7E6AFF819F8FFF8D766EFFB27871FFC7928BFFCB90
      7EFF816954FFFAF9F9FF000000000000000000000000EDEDEC0088AE8A00BDC0
      BD009CB59D00A8A9A8007193710086908C00738076009096980081878F006A7A
      89006D899A008DB2C50078858E00FDFDFD004157D5FF6483FFFF6B88FFFF4B5A
      D4FFC38085FFFBAF89FFCF847EFF5245B2FF1E2FB5FF616383FFBCBCBCFFF3F3
      F4FF0000000000000000000000000000000000000000D8B095FFE79D77FFD95E
      3BFF11B2F3FF17ABF8FF15ABF3FFF5B6AFFFFCE5E3FFFCEAE8FFFAD3CFFFFAD3
      CFFFFAD3CFFFFBDFDCFF877376FF0000000000000000E5E1E1FFD5A888FFC79D
      7BFFC8A17EFFCBA480FF83C9B2FF80C3B0FF79B49FFF6FA190FF95625AFF9B66
      61FFC68877FF968677FF0000000000000000000000000000000091A5950090A5
      93007E8F8600748B7D004F6A5C006B8C7D006281820060866E00B4D8E800C7EC
      F300CFF0F400CFF3F70070889400F3F3F300475CD5FF4E71FFFF5374FCFF363D
      C1FFFCB792FFFEEADAFFEEC8C1FFFBAD8AFFFAA07BFFF69571FFA76484FF2D34
      A7FF0000000000000000000000000000000000000000C2A995FFE79D77FFD95E
      3BFF13CCF7FF0FC1F6FF11B2F0FFF5BAB4FFFBE4E1FFFCE5E2FFF9CAC4FFF8C1
      BBFFF8C1BBFFFBE4E2FF886D70FF0000000000000000A17F5FFFCBA47FFFCCA8
      83FFBB9872FFC8A782FFA1C5A5FF75D2BBFF6EC7AFFF72B19BFF6D9E89FF5C8B
      6FFFB27566FFC19277FFF2F0F0FF00000000EFEFEF00858B93006B7F900095C3
      9C0072888D00A4C8C00090B0A200BEDAD400B7D0D6004E754E00C1D8DC00CDE6
      EA00D3E6EA00A2CACF006D838F00F3F3F3004D61D5FF3961FFFF2E44D6FFECA4
      87FFFCCDB7FFFDDFCEFFFAE6D9FFFEE4CFFFFDC0A3FFFCBB9BFFFAA783FFE683
      62FFFAF9F9FF00000000000000000000000000000000C3A895FFE79D77FFD95E
      3BFFAA9899FFF0EFEFFFC3CED2FFF8C0B9FFFBE3E0FFFCDFDCFFFCDFDCFFF9D0
      CBFFFBDAD7FFFCDFDCFF916F72FF0000000000000000EBCDAFFFD3B996FF8B86
      62FF45715AFFD9BA8CFFD6BB8BFFD7BA89FF98BD9DFF68C9B1FF858A70FF8991
      75FFC0856EFFC48573FFA4978AFF00000000A6ABB30090BACB00EAFFFF00C4D9
      DA00AAC0AC008DA69D00B7CDD400BDCED300658670006A8D7300DFEEF500E4F3
      F600CEE1E4009FC7CD007B8D9300F5F5F5005466D5FF2752FEFF6858B2FFFDC3
      A0FFFFF3E9FFE3DAEAFFC77456FFF2C3ABFFEBAA8EFFFDCDAFFFFED7B7FFFAC9
      A5FFFCFBFBFF00000000000000000000000000000000D9BAA9FFE79D77FFC671
      58FFDCDBDAFFE2E1E0FFE7E6E6FFE3BCB7FFFBE2DFFFFBDAD6FFD5A5A7FFB473
      79FFE0BCBDFFFADBD7FF9D7578FF00000000D3CEC9FFE8CFB2FFD0BD9AFF83B1
      9DFF8F987AFFD2C18DFFDBC690FFD9C28FFFDABB8BFFC3AC86FF6FBDA3FFBD8C
      6BFFC29174FFBE816EFF907A63FF00000000E1E1E2007A99A4009BC4C800D5E8
      EE00E4EDEE00A8C4AC0082A5870088A18F00B2C3B600F3FCFD00F5FDFF00F9FD
      FF00BAD2D500C3E1E50088969C00F4F4F4005A6BD5FF1C36D8FFFABC96FFFEF5
      EFFFFCE2D5FFFACDBAFFF8D8C6FFF5E2D1FF78BBFAFF199DF6FF4094E2FFAB9B
      96FF0000000000000000000000000000000000000000F7F5F4FFE7E6E5FF9E78
      6AFFC8C7C6FFDEBAB6FFC1C0BEFFD4CCCBFFFBE2DFFFE7CFCFFFE2C5C2FFEBC3
      C1FFF7D0CCFFFBD4CFFFB08082FF00000000C8C4C2FFDDC7A8FFCBBDA0FF92C1
      ADFF80B09FFFB6A485FFDBCE9CFFDDCB96FFDAC390FFD4BB8CFFD2AA80FFC49D
      76FFC19374FFBE866FFF957A60FF0000000000000000AFB5BB0081B3BD0079A5
      AC00C3D9DB0000000000FAFBFC000000000000000000E1DFE000F4F2F200F5FB
      F900C8DFE200F2FFFF00979FA500F9F9F9006071D5FFAC82A0FFFDC7A9FFFDE5
      DBFFE5E0F1FFD3927AFFD08A73FFEBC4AFFF12D3FAFF01BDFFFF1A9DF9FF2E80
      CCFFE2E3E4FF000000000000000000000000DEDEDEFFE9E9E9FFE4E3E3FF8280
      80FFC9BCBCFFF8CFCAFFCECDCCFFDDC4C1FFFBE2DFFFFAE4E2FFF7DDD9FFF7D4
      D0FFF1CFCCFFEFD1CFFFBE898BFF00000000DED9D7FFC3D9CBFFA8DFD0FFA1D0
      C1FF96C2B1FF7BAD9BFFE4D6BBFFE6D7ADFFDFCC99FFD7C08FFFC8A47AFF647D
      61FFA67A5FFFBF8B70FF977F69FF0000000000000000000000008198A4009AC4
      CA0087ADB400B6CED300FBFFFF00FFFDFC00B5BFC10087A6AD00A1B0B400A6B0
      B300B4BCBF00F0EFED00A3A6AC0000000000575BBDFFFEC9A2FFFEFCF9FFF7F3
      F6FFFEF7F1FFFDDED0FFFCD0BCFFFEE5D5FF40CFEAFF10D9FFFF01BDFFFF1A9C
      F9FF2F80C9FFE3E4E5FF0000000000000000CCCBC9FF989694FFCFCDCCFF9693
      91FFE5B7B6FFFBD7D3FFF5D5D1FFFBDAD6FFFBE0DCFFF2B1ABFFF4BCB6FFF4C4
      BFFFF3CCC8FFF3D4D1FFCB9091FFFDFDFDFF00000000D0FFFCFFBBE8DCFFB1DE
      D0FFA6D1C2FF95C4B4FFD4C3B8FFDCD4B4FFE0D0A2FFDCC89AFF959473FF6496
      7BFF8E7E63FFA67362FFB9AFA7FF000000000000000000000000C7CACE00B6D2
      D900C4DADF00BCD5DA00BDCFD300B6C0C200A8C4CB00B5D3D800B9D5DA00BED6
      DA00C2D2D300E2E8E60092969E00FDFDFD001EA2D6FFFEE8D7FFF0C7BBFFEDCF
      D2FFFDEAE0FFFEF5EEFFFFF2E7FFFFEEE1FFFEE8D7FF3CC9E4FF0FD8FFFF00BD
      FFFF819FB7FF696DA3FFDFDFE1FF00000000B9B9B8FFDDDCDBFFF1F1F0FFCFCD
      CCFFBCB9B8FFA39C9AFFFCE1DDFFFCE1DEFFFBDEDBFFF2DCD9FFA8CDA6FFDDBC
      BDFFD2A3A6FFCD9294FFD69A9AFFFBFBFBFF0000000087A088FFCAF1E5FFBFE9
      DEFFB5DED2FFA9D3C6FF98C6B7FF95B2A1FFE5D1A8FFE8C59CFF6CC4B2FF68B1
      99FFA8644EFF658C6CFF00000000000000000000000000000000000000009AA3
      AA0000000000DAE3E500BBC4C600CFE1E400E3EFF100EDF5F600F4FCFC00FAFF
      FF00EEF3F600BBC3CC00A4A8AD000000000041A5B9FFC8A9A0FF40E0FCFFFEFB
      F9FF7CCCDAFFE6D4CCFFFCD8C8FFFCD0BCFFFEE7D7FFF0F2F2FF3DC2DEFF96C8
      CFFF4666F5FF224FFFFF2034B3FFF8F8F8FF0000000000000000EAE9E9FFCBC9
      C8FFEAD2D0FFFCE7E4FFFCE8E6FFFDEAE8FFFBDEDAFFF9E2E0FFFAE7E5FFFAE7
      E4FFF9E4E1FFF5DDDBFFD99A99FFF8F8F8FF0000000000000000CBFFFFFFD9DE
      CEFFD2E9DBFFB4E1D6FFA8D6C8FFAFC7B7FFE6DBB7FFDCBD97FF88D7C5FF80C4
      AEFF6EBDA5FFC1B1A9FF0000000000000000000000000000000000000000E6E7
      E800C6D1DB00FAF7F5000000000000000000FCFDFD00E3E6EC00BFC6CF00999F
      A800C0C4C900E0E2E50000000000000000009FD2E0FF5DDFF2FF4E94A6FF2DD9
      F9FFAFA4A1FF4DD8F0FFFEF3EBFF39D5F0FFB09086FF00000000000000009598
      C9FF728DFFFF234EFFFF2246EBFFFEFEFEFF0000000000000000000000000000
      0000F7E5E4FFFDEFEEFFFDF1F0FFFEF2F1FFFADDD9FFEEA9A4FFF3B6B0FFF4BF
      B9FFF4C6C2FFF4CECAFFDF9B98FFF3F3F3FF0000000000000000E7E1DDFFFBFA
      E4FFF6EEDDFFF6EAD1FFF4E5CCFFF1E1C6FFEADDC0FFBFD9C2FF9CDDCAFF94F0
      DCFFA68C7FFF0000000000000000000000000000000000000000000000000000
      0000ADB2B900DAE0E500E2E5E800ADB6BE00B5BDC500C4CACF00F9FAFB000000
      0000000000000000000000000000000000000000000000000000FBFDFDFFEBF4
      F6FF6EDEF3FF46C9E1FF55B3C5FFDDE2E3FF0000000000000000000000000000
      00007885DFFF7187F2FFF0F1F5FF000000000000000000000000000000000000
      0000FBF3F2FFFEF8F8FFFEFAFAFFFEFCFBFFFCEEEDFFF8C5BFFFF6BDB7FFF3B4
      AEFFEDAAA4FFE6A09CFFEEA9A3FFF4F3F3FF0000000000000000000000000000
      0000AD9473FFFFFFE8FFFFF8E7FFF4EDD7FFF5E8D2FFFDDFC9FF99CBB8FFE0D8
      D5FF000000000000000000000000000000000000000000000000000000000000
      0000FEFEFE00C5CCD300C1C5CB00F4F5F6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000DEC3C5FFE8D0D1FFEDD4D5FFF2D5D4FFF5D2CFFFEBBBB9FFD19899FFB78E
      91FFD0BCBDFFF0EBEBFFFEFDFDFF000000000000000000000000000000000000
      00000000000000000000E8E3DFFFDCD7D4FFDCD8D6FF00000000000000000000
      0000000000000000000000000000000000000000000000000000FBFAFA00E2E0
      E000F9F9F9000000000000000000000000000000000000000000000000000000
      0000FEFEFE00CFD1D800D9DADD00000000000000000000000000FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00000000000000
      0000576D840054667900577EA600000000000000000000000000000000000000
      00000000000000000000FAFAFAFFF9F9F9FFFAFAFAFFFCFCFCFF000000000000
      00000000000000000000000000000000000000000000A4A5A500A3A4A500A0A3
      A4009DA1A2009A9F9F00969D9D00939C9C00909999008C9696008A9797008793
      930087939300C5CFCF00FFFFFF00FFFFFF000000000000000000CAA29800D79F
      8F00A3786C00A8958F00D6D3D200EFEEEE00F1F1F100FEFEFE0000000000DFE2
      E8006081B8000D7AF200326AB900DFE0E20000000000F5F5F500DEDEDE00D5D5
      D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500576D
      8400576D84004E749C0054667900577EA600000000000000000000000000FEFE
      FEFFEDEDEDFFBFC0BFFF709173FF6EA071FF6A9A6DFF869988FFD5D5D5FFF6F6
      F6FF000000000000000000000000000000009C9FA0008F8F8F008F8F8F008F8F
      8F008F8F8F008F8F8F008F8F8F00434343008F8F8F0047616100003333004747
      47008F8F8F0046515100FFFFFF00FFFFFF0000000000F4EEEC00EABAAB00FFEB
      DC00FEEBDB00EDCEC000BB928800A4A09E009A9896008F8F8F0097A3BB001C78
      E200118FFE002AA3FF004CBBFC00AEB2BE00F5F5F500C7C7C700818181006A6A
      6A006A6A6A006A6A6A006A6A6A006A6A6A006A6A6A006A6A6A00576D8400576D
      8400577EA60098B1CA006B839E00576D84000000000000000000FBFBFBFFCDCD
      CDFF6FB775FF66D16CFF6FE073FF77E77AFF75E478FF68DC6CFF7BD182FF6692
      6AFFE7E7E7FF000000000000000000000000BBC0C200B4BABB00AAB9BC00C0D2
      D600C0D2D600C0D2D60060B5B700004C4C00263F3F0026595900006666002626
      2600263F3F000099990089999900FFFFFF0000000000DAB6AD00FDEEE600FEE4
      D500E5C0AE00B9B6B400D4BAAE00F6AF8900F7AF8800DFB5A300858E9F0078A6
      CD0046BAFE0067CDFA0082A9CF00EFF1F400DEDEDE001C82B5001A80B300177D
      B000177DB0009D868000826A6400826A6400826A6400826A64009D8680005876
      970096AFC90091A5B900576D84008181810000000000FCFCFCFFC5C7C5FF78C9
      7EFF5DD465FF66DC6CFF6DDF71FF70E375FF6EE071FF6ADE6FFF63D86BFF54CC
      5FFF68AC6DFFE8E8E8FF0000000000000000BBC0C200C0C0C00069696900D8DD
      DF00D8DDDF009C9E9F00307C7C00197F7F00004C4C00007373007FE5E5000033
      33000066660019A5A50048545400C6D3D30000000000D4A59800FFF4EC00FDD8
      C700C6B0A700CFCECD00E5B69500FEDBAA00FED5A300FBB88C00D8B4A5008EA9
      BB0073A3D000BAC0CE0000000000000000002187BA0066CCFF001F85B80099FF
      FF00826A6400BA9C9400DAB9AF00DCBAB100DCBAB100C39F9600826A64009D86
      80008F9FB000576D8400006699006B6B6B0000000000E7E7E7FF7AC581FF52CB
      5BFF59D262FF5FD666FF64DD6AFFFFFEFEFFFEFFFEFF60CD66FF5BD464FF57D0
      5FFF4BC555FF67956AFFF8F8F8FF00000000BBC0C200C9C9C9003F3F3F00F0FB
      FF00F0FBFF0000333300004C4C004C99990059A5A5003FACAC007FE5E5003F92
      920059A5A50059A5A500004C4C0000333300F7F1F000EEC1B400FFF7F100FBC2
      AC00CBB8B100DCDBDA00E4C2A200FFEDC100FFE4B500FDD09F00F4B19100C2B7
      B700B18B8200FDFDFD000000000000000000248ABD0066CCFF00268CBF009D86
      8000CBABA100EABFAA00F4D9C900F4D9C900F4D9C900EFCAB600D3AA9F009D86
      80009D86800099FFFF00006699006A6A6A00FDFDFDFF7A977DFF46C153FF4ECA
      5AFF52CE5CFF56D160FF5CD463FFF5FAF6FFFEFDFEFFFCFEFEFF55C45FFF4ECD
      5AFF4DC957FF6AC773FFDEDEDEFF00000000BBC0C200CCCCCC0045454500F0FB
      FF00F0FBFF003C71720078B0B2007BCBCC0059A5A5004F868600686C6C003F72
      720066B2B20033999900007F8C0002414100E6CFC900FAE9E300FEF3ED00FBBA
      A200D8B4A700E8E8E700DAC8BB00FDEABE00FFE7B800FED2A100F3AF8D00C3A4
      9C00D3C8C600000000000000000000000000278DC00066CCFF002C92C5009D86
      8000E1AC9500EFC3AB00F1CCB600F1CEB900F1CEB900F0C7B000E1AC9500826A
      64004DB3E60099FFFF00076DA0006A6A6A00F6F6F6FF73BF7AFF4BC857FF49C8
      55FF4CCA58FF4ECC59FF51CE5AFF50CE5CFFF6FBF6FFFDFEFDFFFEFFFEFF4EC0
      57FF4BC857FF4AC155FF849C87FFFCFCFCFFBBC0C200CED2D20041414100787D
      7F00787D7F00003333003063630066B2B2001966660078B0B200D1D9D9004C7F
      7F0000666600007F7F004B575700C6D5D500DDBCB200FEF8F400FEEEE600FDDE
      D200EEB9A500E7E2E100F4F3F300DAC1B000F2C09700F6B38B00D6B6A900A479
      6D00F9F8F800000000000000000000000000298FC20066CCFF003298CB009D86
      8000E8B8A000F3D2C000F1CEB900E8B8A000E8B8A000ECB89A00E8AC8B00826A
      640056BCEF0099FFFF00076DA0006A6A6A00F1F1F1FF71C67AFF4ACB57FFEEF5
      EEFFFEFEFEFFFEFEFEFFFEFEFEFFFEFDFEFFFDFEFEFFFDFEFDFFFEFFFDFFFDFD
      FEFF46BF52FF49C856FF6EA373FFFEFEFEFFBBC0C200CED2D200A5A5A500A9A9
      A900A5A5A5009F9F9F00546E6E0068B9B90059C5C50078B0B200D1D9D9004C7F
      7F0000BFBF00008C8C008E9E9E00FFFFFF00DBBCAE00FEF3EC00FEEEE600FDDA
      CC00FBC2AD00E7AC9700E1DAD700E4E3E300CFCDCC00C3BFBD00D6CDC700AF92
      8A00000000000000000000000000000000002C92C5006ED4FF003399CC009D86
      8000E8C1B000F3D2C000F4D9C900F3D2C000F3D2C000EEC0A600E5A68500826A
      64005FC5F80099FFFF00076DA0006B6B6B00F1F1F1FF67C46EFF49C553FFFEFE
      FEFFFEFEFEFFFEFEFFFFFEFEFFFFFEFEFFFFFFFFFEFFFFFEFFFFFDFEFCFFFEFE
      FEFF74C77BFF48C756FF71AA76FFFEFEFEFFBBC0C200DBE7E700C6D2D200D1D9
      D900D6DADA00D6DADA00689F9F003399990048878900689F9F00B5B9B9004C7F
      7F0059727200008C8C00CDD7D700FFFFFF00DDC2B000FDE9DD00FCD7C700FBCD
      BA00FCD0BF00FBC7B400F5BFAA00DEC0B400DCCFC800F7E7DD00F7D9CD00C8BA
      B700000000000000000000000000000000002E94C7007AE0FF002C92C5009D86
      8000D4B6AB00F4D9C900F6E0D400F6E0D400F6E0D400F6DED100E1BBAE009D86
      800080E6FF00000000000C72A5008A8A8A00F7F7F7FF76C77EFF49C756FF71CA
      7BFF82CE88FF82CE87FF82CF88FF82CF87FFA3D0A5FFFEFDFEFFFEFEFDFFD8E9
      D9FF55CB60FF48C655FF6F9A73FF00000000BBC0C200D6E7E70069696900D8DD
      DF00D8DDDF00D8DDDF00C8CCCC00C0D2D600D8DDDF006CA1A2006BA6A6006699
      9900A3A9AA0097A2A200FFFFFF00FFFFFF00DFC9B500FBCAB200FCDBC900FDE9
      DD00FEF2EA00FEECE400FDDED200FCD3C200FCD0BF00FEF3EB00E9BAAC00E2DE
      DD00000000000000000000000000000000003096C90085EBFF0080E6FF002C92
      C5009D868000CBABA100E8CCC100E8CCC100E8CCC100DCBAB100826A64001F85
      B8001C82B5001C82B500177DB000DEDEDE00FDFDFDFF6BAA71FF49C656FF84DA
      8CFF84DA8CFF83DB8EFF85DC8CFF9FD6A3FFFDFEFDFFFEFEFEFFDDEEDEFF86DB
      8CFF84D98CFF46BE52FFC7CAC7FF00000000BBC0C200DBE7E7003F3F3F00B6BE
      C1006E707100F0FBFF00CBD3D300C0D2D600C0C0C000C0C0C000A3A3A3004854
      560094A1A3009AA2A300FFFFFF00FFFFFF00E8DED100FACEB000FBC7B200FBC1
      AA00FDECE300FEF6F100FFFAF700FFFAF700FFF9F400FFF7F100D2A59800F0EE
      ED00000000000000000000000000000000003298CB0091F7FF008EF4FF008EF4
      FF008EF4FF009D8680009D8680009D8680009D8680009D868000000000000000
      0000157BAE008A8A8A00DDDDDD00FEFEFE0000000000DDDEDDFF5DC369FF96E0
      9DFF96DF9CFF96E09CFF96DE9CFFFDFEFDFFFEFFFEFFE2EFE3FF96DF9DFF95DF
      9DFF95E09DFF76C17EFFF8F9F8FF00000000BBC0C200D6E7E70045454500F0FB
      FF00E4ECEF00AAAFB100CBD3D300C0D2D600C0C0C000C0C0C000A3A3A3004854
      560094A1A3009CA3A300FFFFFF00FFFFFF0000000000E1CAAD00F89E7500FBC6
      B100FDE8DB00FEF5EF00FFFAF700FFFCF900FFFAF700FFF9F500C69A8F00F5F3
      F300000000000000000000000000000000003399CC000000000099FFFF0099FF
      FF0099FFFF0099FFFF0000000000248ABD002187BA001E84B7001C82B5001A80
      B300177DB000DEDEDE00F6F6F6000000000000000000FEFEFEFF6E9C72FFA2E1
      A9FFA6E6ACFFA4E4ACFFA4E5ABFFCBE6CCFFD5EAD5FFA6E4ABFFA4E4ABFFA5E4
      ABFFABDBB2FFDADCDBFF0000000000000000BBC0C200DBE7E7003F3F3F00787D
      7F00787D7F007A7F8100CCD0D000C0D2D600C0C0C000C0C0C000A3A3A3004A56
      580094A1A3009FA3A400FFFFFF00FFFFFF0000000000FEFDFD00E5C2A100FBC2
      A900FDE3D500FEF4EE00FFFAF800FFFDFB00FFFCFA00FFFBF800C49A8E00F3F2
      F20000000000000000000000000000000000000000003399CC00000000000000
      00000000000000000000298FC200C7C7C700F5F5F500FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE0000000000000000000000000000000000FBFBFBFF6F9D
      72FFB7E5BBFFB3E7B9FFB5EAB9FFB3E8B7FFB4E7B7FFB4EAB8FFB4E7B9FFB6E2
      BAFFCDD1CDFF000000000000000000000000BBC0C200DBE7E700B3B7B700AFB3
      B300B3B7B700AFB3B300C7CFCF00A8BDC100C0D2D600C0D2D600C0D2D6006C7E
      810094A1A3009FA2A200FFFFFF00FFFFFF000000000000000000FDFCFB00E7CD
      B400FCE4D500FEF6F000FEFBFA00FFFDFD00FFFDFC00FFFCFA00CEA49800EDEC
      EB000000000000000000000000000000000000000000000000003399CC003298
      CB003096C9002E94C700DEDEDE00F5F5F5000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FEFE
      FEFFDDE0DDFF7AB77DFFC1E4C5FFC7EACAFFC9E9CBFFACD9B1FF729D75FFF8F9
      F8FF000000000000000000000000000000009A9E9E00BBC0C200BBC0C200BBC0
      C200BBC0C200BBC0C200BBC0C200BBC0C200BBC0C200BBC0C200BBC0C200BBC0
      C200A5A8A900B1B3B300FFFFFF00FFFFFF00000000000000000000000000FDFD
      FC00EDE6D900E9DBCA00E5D2C200E0C9B900DFC3B500DBBAAC00CCA39700F8F7
      F600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FEFEFEFFFCFDFCFFF8F8F8FFF8F8F8FFFDFDFDFFFEFEFEFF0000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00424D3E000000000000003E000000
      2800000040000000400000000100010000000000000200000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FC1F000000000000F007000000000000
      E003000000000000C00100000000000080010000000000008001000000000000
      8000000000000000000000000000000000000000000000000000000000000000
      80010000000000008001000000000000C003000000000000E007000000000000
      F00F000000000000FC7F000000000000E1FFFFFFF93FFFFF80FF9FFFF003F01F
      807F01FFF001E0078061007F8001C0038000000F80018003C000000F80018001
      000000078001800100000007800100010000000F800100018580000700010001
      C001000300008001C000000100008003E8010000C000C003E3030060F000C007
      F01FC0F1F000F00FF0FFFFFFF001FC7FC7F1C031FC3F8000C0208000E00F0000
      80000000C0070000800000008003000080030000800100000003000000010000
      00070000000000000007000000000000000F000000000000000F000400010000
      000F000000010000000F003080010000800F420180030000800FBC03C0070000
      C00FC0FFE00F0000E00FFFFFF81F000000000000000000000000000000000000
      000000000000}
  end
  object ToolBar_Image: TImageList
    Height = 32
    Width = 32
    Left = 744
    Top = 160
    Bitmap = {
      494C010106000900040020002000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      00000000000036000000280000008000000060000000010020000000000000C0
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FBFBFB00EAEAEA00E0DFE000DEDDDE00E1E1E100EDED
      ED00FCFCFC000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000EBEBEB00C3C3C20099A199006F8771006181630061846400668569008091
      8200B8B8B800E4E3E400FAFAFA00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F2F3F400E0E0
      E100EDEDEE00FAFAFA0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000E9E9
      E9007E8E7F005C7D5E007B8F7B0098A79800A2AEA2009EAB9F008CA08C006487
      67005C83600071867300C0C0C000F6F5F5000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D2D3DD007074A1006367
      99006B6C860093939900BDBDBD00DEDEDF00F3F3F400FCFCFC00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000ABB1
      AB005E8E6300A8AEA9000000000000000000000000000000000000000000F8F8
      F800BCC4BC00729074005B805D00B5B7B500F9F9F90000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FCFCFD004657C9008CA3FD0098AD
      FE0096A8F900818EE0006870B40060628E0074748800A1A1A400C9C9CA00E6E6
      E800F7F7F700FEFEFE0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008D9E
      8D0062906700C1C2C10000000000F9F9F900D8D8D800E5E4E500FEFEFE000000
      000000000000E7E6E6009FA8A000597B5A00CCCDCB0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F9F9FC00374FD500819AFF008AA2
      FF008EA5FF0092A8FF0096ABFF0098ADFD007B88E3005055B8004D4F97005B5C
      830081818E00AFAFB000D3D3D400EEEEEF00FEFEFE0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000097A4
      960061906600B2B5B20000000000AFB3AF006283640073887300CFCFCF000000
      0000B4B9B400718A7100BFBFBE009AA199008E9A8E0000000000000000000000
      00000000000000000000000000000000000000000000FEFEFE00F7F7F700F5F5
      F500F7F7F700000000000000000000000000F9FAFC003B52D5007591FF007D97
      FF00819AFF00849DFF0088A0FF008499F7005A65D200454EC500303DC7003944
      C400464CBD00494DAB004F518D0075748500D6D6D700FEFEFE00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000AEB3
      AE00689B6D00929C93000000000087A1890075987800999F990068866900C8C7
      C700A8B1A9006FA177006376610088938600B8B9B800F5F4F500000000000000
      0000FEFEFE00FBFBFB00F4F4F400E9E9E900DADADA00C5C5C500A6A6A9009393
      9B00B8B8BA00EAEAEB000000000000000000F9FAFC003D54D5006987FF00708D
      FF007490FF007792FF007B95FE006477E5004650C900313CC7007A5EA9006953
      AC00313AC5001C31CE002A39C7003237AB006F6F8100B0B0B100D4D4D600EDED
      EE00FAFAFA000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000D1D2
      D10075A47C0074957400E9E8E800B7BFB70084B68A00BDBCBD00C7C8C700758F
      7500949993007AA77F00A0A6A000A9AFAB006E876C009BA09B00D0D1D400DEDE
      DF00CCCCCC00AEAEB0008E8F9500727681005865770052677B0059768E00658A
      A60058697C00C4C4C500FDFDFD0000000000F9FAFC004157D5005E7FFF006483
      FF006886FF006B88FF006B87FA004B5AD4002E3AC400C3808500FBB38D00FBAF
      8900F39F7D00CF847E00926496005245B2002231C7001E2FB500394190006163
      830091919900BCBCBC00DDDDDF00F3F3F4000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FAFA
      F90081A083008AC29200A8A9A700F1F1F10095C09A0088AF8C00D9D6D900C4C8
      C7006A8468007EA580007D958000D0CFD40077807F006C936700535D60005963
      750052657900526E890065879F0080A6BB0099C2D100ACD1DC00B7D8DE00BEE0
      E600779FB5008F919900FBFBFB0000000000F9FAFC00435AD5005476FF00597A
      FF005C7CFF005F7FFE00526BE9003541C6007056A600FAB18C00FCB48F00FCB5
      9200FCAF8B00FBAB8600FBA88300F9A27D00EB8F7300BC7380007B549D003D39
      BB00192DC7002232A800454989007F7F8C00E1E1E20000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000B6BBB700A5DEAA0082A08500D7D6D800B4B8B800ACE4AD007F978000A1A0
      A8006A72760087B0860083AD85003D4A5D004E627E00628769005F846000708D
      9E00ABD0DD00B9DCE200BFE0E600C2E0E400C1DDE100BDDADE00BFDCE100B9D7
      DB008BB6C1007B828F00F9F9F90000000000F9FAFC00475CD5004A6EFF004E71
      FF005173FF005374FC003C4ED500363DC100DD948400FCB79200FDDAC400FEEA
      DA00FEE2CF00EEC8C100FABEA100FBAD8A00FBA37E00FAA07B00F99D7800F695
      7100DF806D00A76484006548A5002D34A700BCBCBD0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FDFDFD00FCFC
      FC00E1E1E300849F8400BFFDC500626E6A0087878F0056696C00ABDFAB005E7C
      690038486300607E720091C48A0073938A00B4D1E6007C978C00598455007D94
      8800BCD6DE00BFDDE000BCD8DE00BCD8DE00BCDADF00C1DBE000C8E1E6009DBE
      C4008CB7C1007D859200F9F9F90000000000F9FAFC004A5FD5004066FF004369
      FF00466BFF00415FEE002B39C400926A9D00FCB89200FDCAAC00FEEEE100FFEC
      DE00FAE8DB00E5D7E100FEE6D200FEE2CC00FCC5A900FBB19000FAA68300FA9D
      7900FA987400F9967200F8936F00AB606600B7B7B80000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FAFAFA00D4D4D5009294
      9A00787B87004C5A6700A4D8A9009ECEA30040576400708DAB008CA7A3009FCE
      9F0081AA8400657C6D0079967D009FBAB300CFE8F30093A9A500588258006785
      6F00B5C8D100C7DDE300C6DEE300C7DEE300C7DFE300D0E6E900BED6DC0088AF
      B50091BAC2007C849200F9F9F90000000000F9FAFC004D61D500375FFF003961
      FF003C62FD002E44D6004A48BB00ECA48700FCBC9700FCCDB700FCD1BE00FDDF
      CE00E6D9E400FAE6D900FFE7D400FEE4CF00FDCAAF00FDC0A300FCBFA000FCBB
      9B00FBB29000FAA78300F99F7B00E683620099929100FAF9F900000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000E8E8E8006A7989005F85
      A10086ABC10099B8CF007F989200B9E4B700A7C1A900879B9C00C2DBE600A9C1
      C300A3BDB600C1D8DF00C4DBE400CBE3EA00CFE4EE008EA19E005A855B006586
      6B00BBCCD300D0E6E900CFE3E700CFE3E800D0E3E800DBECEF009BBDC30088AF
      B60096BDC7007F859100F8F8F80000000000F9FAFC005064D5002F58FF003059
      FF002F52F2002937C500B47E9300FCBD9600FDD4BB00FEF2E800FEEDE000F5D7
      CE00E7C7C900F5BEA500FCD0B900FDDCC600FCC4A900FDC1A400FDBFA100FDBD
      9E00FDBC9B00FDC5A400FED2AF00FCC6A200967B7200F3F3F200000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000B9BCC0006790AE00C5DF
      E600D7EFF100D1EBED00C4DAE4008CA39900D2E4D000A6AFA7007E8D9000BED2
      DC00D2E7EF00D1E9ED00D1E8EC00D0E4EB00C1CEDA00738B7C00568657006F8B
      7900D4E1EA00D6E9EC00D6E8EB00D7E9EC00DFEDF100CEE1E4008AB0B8009DBE
      C400A3C5CC0082868E00F7F7F70000000000F9FAFC005466D5002752FF002752
      FE00223BD6006858B200F6B28E00FDC3A000FEF0E600FFF3E900FEF1E600E3DA
      EA00FBEADD00C7745600E3AD9400F2C3AB00D1937A00EBAA8E00FABDA000FDCD
      AF00FED2B400FED7B700FFD4B200FAC9A500AB979000FCFBFB00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000B7BABF00719CB100A7C6
      CA00C7DDE200D3E7EA00D1E7EB00C7DCE20097ACA100BCD2BC009EBD9F007F95
      880099A6AA00B1BCC400B9C4CA00A4B1B40070897900558156005D806000BBC9
      CA00E4F2F800DFECEE00DFECEF00E1EEF200EBF4F600B0CBD00099BAC100AECA
      CE00ACCCD40083868F00F8F8F80000000000F9FAFC005769D500204CFF001E47
      F500333FC600CF918E00FDC09A00FCCAB200FCD5C500FDE3D500F0E5E800EFE4
      E800F2D5C500C97B5F00E6B9A300C5947F0031363E006B99C40081B1E600759D
      CC007CA3CA00DBBAAA00FDCAA900C9937C00E8E6E50000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EBEBEC006F90A5008CB7
      BB008CB1B800BCD6DB00E0EEEF00DFEFF400D7E8EE00ABBCB9009ABB9E008CBF
      90007DA98000779B7C0068896C00638064007A977E009EB1A600D5DFE300EEF6
      FC00E7EFF200E8F1F300E8F2F400F1F7F800E1EBED00A4C2C700AEC8CC00BCD4
      D600B9D4DD0083878F00F8F8F80000000000F9FAFC005A6BD5001A47FE001C36
      D8008B6DA900FABC9600FDCBAB00FEF5EF00FEF3EB00FCE2D500E2C8D500FACD
      BA00F3C1AB00F8D8C600FEEADA00F5E2D100719FC90078BBFA002DAAEE00199D
      F600268EF6004094E200D1AC9C00AB9B9600FCFCFC0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000949DA80094BD
      C7008BB3B50081A7B000A6C5C900E2EDEF00EEF6F800EBF6FD00D7E0E700B7C4
      BF00A8BEAC009EBAA400A7BDAF00D7E0E200F2F8FC00F9FFFF00F3FBFE00EEF6
      F600EEF6F700EFF5F700F1F6F800F8FCFD00C8DBDD00AFCBCF00BCD2D600CADE
      DF00C5DEE40084878F00F8F8F80000000000F9FAFC005D6ED5001440F700474D
      C400E3A18C00FEC49E00FEEADC00FFF8F200FFF6F000EBE4F000F3EAEA00D9A1
      8D00FBE6D900F5CBB700FCCDB700FBCBB30089B8E9002EAAEE0001BCFD0001BB
      FE00199EF900268EF6003684D1007C848A00E3E3E40000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F4F4F4007492
      A6009AC2C50089AFB6007EA8AF0091B5BB00D3E3E400F7FDFE00F5FCFE00F6FC
      FF00F5F8FE00F7F9FF00FCFFFF00F8FEFF00F4FBFC00F3F7FB00F3F7F700F5F8
      FC00F8FCFD00FBFFFF0000000000F3F7F700BCD3D600C5D8DC00CADCDE00D8E8
      E900D2E4EC0085889000F8F8F80000000000F9FAFC006071D500203BDA00AC82
      A000FCC49D00FDC7A900FCD7C800FDE5DB00FAF1ED00E5E0F100F9EBE100D392
      7A00D4968000D08A7300F0D1BF00EBC4AF0073A0C40012D3FA0001BDFF0001BD
      FF0002BBFE001A9DF900268EF6002E80CC007E868B00E2E3E400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000ACB2
      BB008AB3BF0091B6B9008AB0B70083ABB30083AAB000B9D2D500F8FDFC00FEFF
      FF00F5FCFD00F7FCFD00F9FCFE00FCFEFE00FCFFFF00EDECEC00D1CECE00D1CC
      CC00E4E1E100F5F3F40000000000DEEBEC00CCDDE000D7E6E900D8E6E900E8F2
      F300DEEDF4008F919800FDFDFD0000000000FAFAFC00616DCF006964C000F0B0
      9000FEC8A300FEF2E900FEF8F300FDE8DF00E5CCD700F7D1C500FBD6C600DEA2
      8D00F4DBCD00FAE8DB00DCA48E00E6BAA5007DBED0001BECFE0011D9FF0001BD
      FF0001BDFF0002BBFE001A9DF900268EF6002F80CB0080878D00E3E4E5000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000778B9E009BC3C8008CB1B70093B5BB0095B7BC008DB3B800A9C4CA00EBF1
      F30000000000000000000000000000000000EFECEA00B9BABB008EA8AE0095A6
      A700A3A7A900B6B4B500C4C2C200B8BFC300CCD6DB00DCE7E900E6EDEE00F8FC
      FC00E8EFF5009C9FA4000000000000000000F9FAFB00575BBD00C9949A00FEC9
      A200FEDEC800FEFCF900FFFBF800F7F3F600E9E5F400FEF7F100FEF0E800FDDE
      D000FCD2BF00FCD0BC00FDD7C400FEE5D500E2E0D90040CFEA001BECFE0010D9
      FF0001BDFF0001BDFF0002BBFE001A9CF900268EF6002F80C90081888D00E3E4
      E500FEFEFE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C9CACD0088AEBE00A7C8CA009CBBC400A4C1C700A9C5C900A5C2C700ABC5
      CA00D7E4E8000000000000000000EDE9E900B7B7B80095B3BA0098BCC6009CC2
      C7009EBEC4009EB8BE009BABAE00949D9F009DA1A300AFB1B000C6C8C800EDEC
      EB00E9EFF5009D9FA4000000000000000000F6F7F9004C5EB800E8A78B00FECB
      A700FCD5C600FDE6DD00FEF6F300E3E1F700FCF7F400FFF7F200FFF6EF00FFF4
      EC00FFF3E900FEF0E500FEE6D800FCD4C000FCCCB600DEC2B5003EC6DF001BEC
      FE0010D8FF0001BDFF0001BDFF0002BBFE001B9CF900278EF500427EB3007D7E
      7F00C6C6C600FCFCFC0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008493A300C2E0E600B1CACE00ADC7CC00B6CED200BCD1D500BCD2
      D600B9D1D600D2E2E600E6E4E300BABBBC00A3BDC300ADCBCF00B1CAD000B4CB
      D100B7CFD300B9D1D800BCD5DA00BED6DA00B8CCD100AFBCBE00A9ADAE00B4B4
      B100BDC2C4008E919B000000000000000000BCD9E4001EA2D600B8828000FEE8
      D700FEFBF900F0C7BB00E8B6B000EDCFD200FCD9CB00FDEAE000FEF5EE00FEF5
      EE00FFF3EB00FFF2E700FFF0E400FFEEE100FEECDD00FEE8D700DFD8CF003CC9
      E4001BECFE000FD8FF0000BDFF0000BDFF0015B4ED00819FB7009493A700696D
      A30070707700DFDFE10000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E5E5E6008FABBB00E2F1F100C4D6DA00C3D5DB00C7DADD00CCDF
      E200D1DFE300B4C1C2009EA9AC00B5CBD000BFD6DB00C2D5DA00C4D7DB00C7D8
      DE00C9DCDE00CBDEE000CEDEE100D1E0E300D5E7E900DBEAEC00D8E8EB00E0EA
      EB00CBD5D800868C9500000000000000000090C9D7003FA1B600BAA09900FDF6
      F300FEFAF9006ABECA00A1C2CC00FEFBF900EACDC500E6B3A300FCD5C600FCD3
      C200FDDACB00FEEADE00FEF0E500FFEFE200FFECDE00FFEADB00F5D3C000A09A
      9B003DC1DE001BECFE000FD7FF0014B7EE00A0B8C100777CBF002648E700123F
      FB001F35B70084848D00E7E7E900000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000979FA900D8EAF300EBF3F200DDE8EA00DBE8EA00DBE8
      EB00BCC5C700A3AAAC00C3D6DA00CFE2E600D0E0E200D3E2E400D6E4E700D8E6
      E900DCE8EA00DEEAEB00E1EBED00E3ECEE00E7EEEF00ECF3F400F9FBFD000000
      0000BDCAD200B2B5BB00000000000000000098CEDB0041A5B900C0BBB900C8A9
      A000D8D5D30040E0FC00CAF1FA00FEFBF90078C4D2007CCCDA00FEF8F300E6D4
      CC00C8ACA100FCD8C800FCD0BD00FCD0BC00FDDAC800FEE7D700B9918300F0F2
      F200E1EBEE003DC2DE001DEAFB0096C8CF008A8ECC004666F500355DFF00224F
      FF00103FFD002034B30092929800F8F8F8000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F7F7F70087A1B60000000000EBF1F300EDF4F500D4D7
      D800BEC3C400DEE9EB00E7F1F200E2ECED00E4EDEE00E7EFF100EAF1F200EDF3
      F400EFF4F500F3F7F70000000000000000000000000000000000DFE7EB00A9B6
      C1009AA2AA00FDFDFD000000000000000000D5EAF00047D7EF006C878F00779F
      AB0033D7F40052B5C700BAA19B00BFC5C3003FDCF900B0E7F300FEF9F40094CF
      D90057D1E500FEF2EA00E9D2C700A5A59F00FADACA00EAAF9900C5BDBA00FEFE
      FE0000000000DCE6EA0051B2C900BABBCF00657BEB005E7DFF00486CFF002651
      FF00093AFF000737FB00666A9300F5F5F6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000B2B7C000D5E7ED0000000000D7DCDC00CED2
      D000F3F7F700F9FEFF00F7F8F900FBFDFC00FCFCFC00FDFFFE00000000000000
      00000000000000000000F1F6F900C6D1D80097A4AF00959DA600BCC1C700EEEE
      EF0000000000000000000000000000000000FEFEFE009FD2E0005FE1F6005DDF
      F2004CE0FB004E94A6007898A2002DD9F90042B8D000AFA4A1009ABABB004DD8
      F00099D8E400FEF3EB00B1D6DA0039D5F000FAE9DC00B0908600F8F7F7000000
      00000000000000000000CBCFD2009598C900869EFD00728DFF004A6EFF00234E
      FF00224EFF002246EB00A8A9B700FEFEFE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008696A70000000000DEDEDD00F9F9
      F90000000000000000000000000000000000000000000000000000000000D6E0
      E700A5B1BE00919DA900A5ABB300D4D5D800FAFAFA0000000000000000000000
      0000000000000000000000000000000000000000000000000000EEF6F800E6F2
      F500C9E3EA0068D6EA0060E1F60051D9F50046AFC4006A909A0033D4F3003CBC
      D400B1A9A700B9A099005BCDDF007EC0CA00CEA39600EBE8E700000000000000
      00000000000000000000FDFDFD00B5B5C2007D89DD00839CFE004E71FF004A6E
      FF00476AF9006C71A900F4F4F700000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000CCCDD000B8C9D400000000000000
      0000000000000000000000000000EDF4F700BBC8D00095A2AE0096A0AB00BEC3
      CA00EEEEEE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FBFDFD00F6FBFC00EBF4F60082CCDD006EDEF30063CFE90046C9
      E1005D929E0055B3C50052C8DE00DDE2E300FDFDFD0000000000000000000000
      000000000000000000000000000000000000D2D3E4007885DF007D96FE007187
      F200757AB200F0F1F50000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000009BA4AF00BDCEDA00F7FD
      FF00D8E1E800A5B1BC008F9CA900A4AAB200D8DADE00FBFBFB00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FDFEFE00FBFDFD00A8D7
      E3007CC6D30090CAD700E7EDEF00000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D5D6E8009092C000BFC1
      D800FAFBFC000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CCD1D700B1BA
      C400BDC4CA00EFEFF00000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FAFA
      FA00FAFAFA000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FCFCFC00FAFAFA00FAFAFA00F9F9F900F9F9F900FAFAFA00FAFAFA00FCFC
      FC00FEFEFE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000EDEBEA00BBB9B900C9C8C800EAE9E900FAFAFA00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F9F9FA00C7C9CE007E82
      9B0092939A00D9D9DA00FEFEFE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FEFEFE00F9F9F900F1F1
      F100E7E7E700DBDBDB00D4D4D400D0D0D000D0D0D000D3D3D300D8D8D800E4E4
      E400EEEEEE00F8F8F800FDFDFD00000000000000000000000000000000000000
      0000000000000000000000000000000000000808080000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000080808000808080000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F5F1F000C0725E00CE6B5100A45742008B6B62009C979600C5C4C400E9E7
      E700F9F9F9000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FDFDFD00DDDFE3007680A5001B61D6000678
      F7001B5ED1006C708500E0E0E200000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F3F3F300F0F0
      F000FEFEFE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FEFEFE00F9F9F900EDEDED00DADADA00BFC0
      BF0092A092007091730069976C006EA071006F9F72006A9A6D006B906F008699
      8800B5B7B500D5D5D500E9E9E900F6F6F600FEFEFE0000000000000000000000
      0000000000000000000000000000000000007777770086868600868686008686
      8600868686008686860086868600868686008686860086868600868686008686
      8600868686008686860086868600868686008686860086868600868686000000
      0000000000000000000000000000868686008686860086868600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C6A49B00EEB9AA00FEF1E900F7DAD000EAA89600D4725900A75742008C67
      5E0098918F00BFBFBE00DFDEDE00DFDEDE00DCDCDC00E9E9E900F9F9F9000000
      000000000000FEFEFE00ECEDF0009699AE002C5EC3000578F7000786FE000E8D
      FF001995FE002C63C4009F9FA300FBFBFB000000000000000000FBFBFB00E6E6
      E600F0F0EF00FBFBFB0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D5D7DA00616E7D005662
      6F00C0C0C200FBFBFB0000000000000000000000000000000000000000000000
      00000000000000000000FDFDFD00F2F2F200DCDCDC00B2B6B3006B906F0070B2
      75007DCB820086D58B007FDA820078D97A0076D878007DD8810085D78A0081CE
      860075BB7A00649369009FA8A000D5D5D500EDEDED00FBFBFB00000000000000
      00000000000000000000000000000000000086868600F0FBFF00999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000009999990099999900999999000066
      6600006666000066660000000000999999009999990099999900000000000000
      000000000000000000000000000000000000000000000000000000000000F9F7
      F600CC735D00FCEDE500FFEBDB00FFEAD900FFEBDC00FEEEE300F8DED400EDB2
      A200D67B64009C615100837F7E0089888700828180007877770097979700D6D6
      D700F9F9FA00B9BCC7004462B000096FEF000584FE000C8BFF001693FF00239E
      FF0033ABFF003FA7F6006C718D00F2F2F30000000000F8F9F9005A9AB7000C8F
      C6002682A9004E8299008897A000AEB3B600D6D6D600EDEDED00FBFBFB000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000A1A6AC00355271003D6794003D66
      9200304962008C8C900000000000000000000000000000000000000000000000
      000000000000FBFBFB00EDEDED00CDCDCD00779379006FB775007FCF860066D1
      6C0068D96C006FE0730075E4780077E77A0077E77B0075E4780070E2740068DC
      6C0063D36A007BD1820078C27E0066926A00BCBEBD00E7E7E700F9F9F9000000
      00000000000000000000000000000000000086868600F0FBFF00C0C0C00090A9
      AD0090A9AD0090A9AD0090A9AD0090A9AD0090A9AD0090A9AD0090A9AD0090A9
      AD0090A9AD000099990000999900000000000000000099999900999999000066
      6600006666000066660000000000999999009999990000000000009999000099
      990000000000000000000000000000000000000000000000000000000000DAC2
      BC00EAA69500FEF1E600FFEDDF00FFEBDC00FFE9D900FFE8D600FAE2D000D5C6
      B900BAB7B500C3C0BF00C8C6C500CBC4C100CCC6C300C2BFBE009B9997007474
      73008B8FA0001E6AE0000D88FC001492FF001391FF001F9BFF002EA7FF0040B6
      FF0055C7FF0068D2FC006A749900F5F5F60000000000E3E8EA000099D7000196
      D00001ACE600009DD9000492CB001489BA002F86A9004E88A100859BA400B7B9
      BB00E5E5E500F7F7F70000000000000000000000000000000000000000000000
      0000000000000000000000000000BCBEC20037537400406C95004F7DA8005B85
      AF0051769C00344C6600DEDEDE00000000000000000000000000000000000000
      0000FDFDFD00ECECEC00C2C4C2006295670080CA85005FCB670060D5660068DD
      6D006EE0720072E2750074E5760075E6770075E5770073E3760072E475006EE2
      72006ADD6F0062D769005ACE62007DCC820063A36900ABB1AB00E4E4E400FBFB
      FB000000000000000000000000000000000086868600F0FBFF00C0C0C000C0C0
      C00099999900F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FB
      FF00F0FBFF000099990000999900000000000066660000000000000000000066
      6600006666000066660000000000000000000000000000666600009999000099
      9900000000000000000000000000000000000000000000000000FEFEFE00BF7B
      6A00F9E3DB00FFF0E500FFEFE200FEEBDC00FEE9DA00EFD6C600ADA6A100B4B2
      B000D4D3D200C7B5AE00E8A68900F7A37E00F7A37E00F0AB8F00DFC2B600B7B5
      B3006B7092006185A8007291AD002895F00033ABFF003BB1FF004FC2FF0066D5
      FF007AE2FC00639EDD00BDC1CD00FEFEFE0000000000D5DCDF000099D5000192
      CE0001CBFE0001D7FF0001CEFF0001C1F80000AEE70000A6DE00009DD7000096
      D200138ABA003783A4007391A0009EA9AF00CACBCD00E6E6E600FAFAFA000000
      00000000000000000000A1A6AD003754750046729E00517DA9006F94B9009BB6
      D00096B2CD0043638400C2C3C40000000000000000000000000000000000FCFC
      FC00EFEFEF00C5C7C500659B670078C97E0053C95C005DD4650062D96A0066DC
      6C0069DE6E006DDF71006EE1730070E375006CDA6D006EE071006CE172006ADE
      6F0067DC6D0063D86B005DD4670054CC5F0069CA740068AC6D00ABB1AC00E8E8
      E800FBFBFB0000000000000000000000000086868600F0FBFF00C0C0C000C0C0
      C0008686860099999900C0C0C000C0C0C000C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C0000099990000999900000000000066660000666600006666000080
      800099FFFF0066CCCC0000000000006666000066660000666600009999000099
      9900080808000000000000000000000000000000000000000000F2EBE900DD84
      6D00FEF5EF00FFF2E700FFF0E500FCC8B200F7A38500B09C9300B4B2B000D1D0
      CF00C5BCB800EFA48200FBB78B00FCBE9100FCBB8E00FBB18700F9A57D00ECB9
      A400BBB8B700908E8D00C7C6C50080ADD2004BBEFE005FCFFF0077E2FE006FBE
      EC007589B900DDE2EA00000000000000000000000000C9D4D700009BD9000196
      D10001B6ED0001D8FF0001D6FF0001D6FF0001D7FF0001D2FF0001CAFF0001C1
      F70001B1E90000A3DC000194CF00098CC1002387AE003C86A500698FA10095A4
      AB00CDCDCE00AEB2B60038567500406C95004F7CA900638AB3009FB9D200C3D3
      E5008FA9C3003D577400F1F1F100000000000000000000000000FEFEFE00F7F7
      F700D6D6D6006495690074C87E004FC5590057D060005CD464005FD7660062D9
      690065DB6B0068DD6D0065D76A00A0D4A200D2E9D300AFD7B10066D06B0065DB
      6D0063DA6A005FD867005CD4660057D1610050C95C0066C6700063A56A00C0C3
      C100F1F1F100FDFDFD00000000000000000086868600F0FBFF00C0C0C000C0C0
      C0008686860000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF000000
      000000000000009999000099990066CCCC000000000000666600006666000080
      800066CCCC0099FFFF000000000000666600006666000066660066CCCC000099
      9900000000000000000008080800000000000000000000000000D4B4AD00EFBB
      AE00FFF5ED00FFF3EA00FEF1E700FCD0BC00DDAB9800B1AFAD00BFBDBC00DBDA
      D900CAA69700FCBC8E00FED3A100FED7A500FED4A200FDC99900FCB98C00FAA6
      7E00E9BDAC00ACAAA900B2B1B00093AFBF0060CFFD0077D2F4006284BD00C2C8
      D800FCFDFD0000000000000000000000000000000000BAD4DD0000A1DE0008A5
      E10001A5DD0001D3FF0001D8FF0001D6FF0001D7FF0001D6FF0001D3FF0001D1
      FF0001CFFF0001CFFF0001C3F50001B4E100019CCA00008EBD000086B800007D
      AB006E5245004F454A0043709D004F7DA9006A8FB6009CB5D000C5D6E6009EB4
      CC0051688100C6CACE0000000000000000000000000000000000FBFBFB00E7E7
      E700829984007AC581004BC2560052CB5B0055D05F0059D262005BD363005FD6
      660061D8670064DD6A0089CD8B00FFFEFE00FEFEFE00FEFFFE00D0E7D20060CD
      66005CD666005BD4640058D3610057D05F0053CC5C004BC5550074C77C006795
      6A00DCDCDC00F8F8F800000000000000000086868600F0FBFF00CCCCCC00C0C0
      C0007777770000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF00000000000066
      6600006666000000000066CCCC0066CCCC0066CCCC0000000000006666000080
      800099FFFF0066CCCC0000808000000000000000000099FFFF0066CCCC0066CC
      CC000000000000666600006666000000000000000000FEFEFE00C07D6B00FAE8
      E200FFF5EE00FFF4EC00FDE4D700FBBBA100C7A69900C1C0BF00C8C7C600DCDB
      DA00D3A48D00FDD1A000FFE0AF00FFE1B100FFDEAD00FED8A600FDCB9B00FBB6
      8A00F8A48000D3C6C100898F9A006CB7E200659DE3008E577300CCCACB000000
      00000000000000000000000000000000000000000000AECCD60002A5E10022BA
      F100019DD60025D3FF002AE1FF0022DEFF000ED9FF0001D2FF0001CAFB0001C5
      F60002AFDA0030849100586662006B6D670070706A00676660004A5B5A004655
      5400B9928500A0776A00555D6D005E87B10096B3CE00C3D5E600A2B8CF004662
      7D00CDD3D700FCFCFC0000000000000000000000000000000000F4F4F400C7C8
      C70069AF6F0055C2600049C8580050CC5B0052CE5C0055D05E0058D161005BD3
      63005DD564005DD76500A2D5A400FEFEFD00FEFEFE00FDFDFE00FDFEFE00CFE7
      D2005BC7630056D1600055D05E0053CF5D0050CD5C004BC9570049C0540076C0
      7B00A0AAA100EEEEEE00000000000000000086868600F0FBFF00CCCCCC00CCCC
      CC008686860000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF00000000000066
      660000666600006666000000000066CCCC0066CCCC0099FFFF0066CCCC0099FF
      FF0066CCCC0099FFFF0066CCCC0099FFFF0066CCCC0066CCCC0099FFFF000000
      00000066660000666600006666000000000000000000F8F5F400D87D6500FEF8
      F400FFF7F000FFF6EF00FCCEBB00F99F7E00C59B8C00CCCAC900D1D0CF00E0DF
      DE00CFA69100FEDAAA00FFE9BB00FFE9BC00FFE5B600FFDFAE00FED6A400FCC3
      9400FAAA8000E9BCAA00A7A4A500C4B0BC00E0B4B100A47B7100F8F8F8000000
      000000000000000000000000000000000000000000009FC3CF0004A7E30043D6
      FF000196D10034CAF30058EAFF0051E5FF0045DBFD0037CDF1001EB6DB002F83
      93008F675600BD968B00DBBFBA00DEBFBB00DFBDB800D9B8B500CCABA800AC82
      760094665400BF9F96009E76660086808300C5D7E800A6BAD0004F678100BACA
      D2000000000000000000000000000000000000000000FDFDFD00E9E9E9007A97
      7D0077C87F0046C153004BC859004ECA5A004FCC5A0052CE5C0054CF5E0056D1
      600059D261005CD4630069C56D00F5FAF600FDFEFD00FEFDFE00FCFEFD00FCFE
      FE00CFE6D00055C45F004FCF5C004ECD5A004ECB59004DC9570048C553006AC7
      7300659A6A00DEDEDE00FBFBFB000000000086868600F0FBFF00CCCCCC00CCCC
      CC008686860008080800F0FBFF00F0FBFF00F0FBFF00F0FBFF00000000000066
      660000666600006666000066660066CCCC0099FFFF0066CCCC0066CCCC000000
      000000000000000000000000000066CCCC0099FFFF0099FFFF0066CCCC000000
      00000066660000666600006666000808080000000000E6D5D100E8A08D00FFF9
      F500FFF8F200FFF7F100FDDED000FBBEA600CAAA9D00D3D2D100DAD9D800E3E3
      E200C7AFA300FDD8A900FFF2C700FFF1C500FFEABD00FFE3B300FEDBA900FDCB
      9A00FBB18600F0AF9500B8B6B400E5D3C700CF776000D0CCCB00000000000000
      0000000000000000000000000000000000000000000094BCC90008A9E5004EDD
      FF0008A6DD001DB5E5006AEEFF005BE3FE0048CEEE0034BFE1004E797E009E7B
      6D00DCC5C400DCB2A900E7B6A300EEC5B500F0CBBA00EBBFAD00DBA79600D2AD
      AA00C2A09C0092635300C1A29A00A780700093949D00496582009BB9C800FCFC
      FD000000000000000000000000000000000000000000FAFAFA00DBDBDB0069A8
      6F0057C2610049C755004BCA58004BC957004CCA57004ECC5A0050CE5B0052D0
      5C0054D15D0056D15E0056D05F0079C67C00F7FAF600FEFEFD00FDFEFE00FDFD
      FD00FEFFFE00CDE5CF0050C25A004DCB58004CC958004BC9570049C8550047BF
      520073BC7B00B9BDBA00F6F6F600FEFEFE0086868600F0FBFF00CCCCCC00CCCC
      CC008686860000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF0000666600F0FB
      FF00F0FBFF00F0FBFF00F0FBFF0099FFFF0066CCCC000000000000666600D6E7
      E700D6E7E700CCCCCC0099999900006666000000000066CCCC0066CCCC0000CC
      CC000099CC0000999900009999000000000000000000D5AEA300F2C8BD00FFFA
      F600FFF9F400FEECE400FCCAB700FBBBA200D9AD9D00D2D1D000E2E2E100E7E7
      E600CFCBC900EABD9800FEF5CB00FFF4CA00FFECBF00FFE4B500FFDCAA00FDCD
      9C00FBB28700F0AB8F00BDBCBA00D3B0A600A3736600F7F6F600000000000000
      0000000000000000000000000000000000000000000085BBCD000CABE60058E4
      FF002EC3F100019AD30076E7FF0063D7F00043B9D70049849000AB807100E2CB
      C900E3A89200F6C7B100FEE2D000FFE6D800FFE8DA00FFE5D700FCDAC600F0B9
      9F00D5A79C00D2B4B30096675500D5BEB700855847001A678500549FBD00F5F5
      F5000000000000000000000000000000000000000000F6F6F600B2B8B20073BF
      7A0048BF52004BC857004BCB570049C855004BC956004CCA58004CCB58004ECC
      59004FCD5A0051CE5A0051CF5B0050CE5C0076C47D00F6FBF600FEFEFE00FDFE
      FD00FDFEFD00FEFFFE00CEE5CD004EC0570049C856004BC857004BC958004AC1
      550076C67F00849C8700EEEEEE00FCFCFC0086868600F0FBFF00CCCCCC00CCCC
      CC007777770008080800F0FBFF00F0FBFF00F0FBFF00F0FBFF00006666000066
      6600006666000066660099FFFF0066CCCC00006666000000000000666600F0FB
      FF00D6E7E700CCCCCC009999990000666600000000000066660000CCCC000099
      99000000000008080800000000000000000000000000C0837300FAE8E300FFFA
      F700FFF9F600FDEDE500FBC4AF00F9A18200F29B7C00C3B8B300EBEAEA00EEEE
      ED00EAE9E900C6AFA400F9D2A600FEEBBE00FFE8BA00FFE1B100FED9A700FDC7
      9700FBAD8300E7B19B00B9B7B500C3715D00BDB5B40000000000000000000000
      000000000000000000000000000000000000000000007BC0DA0010AFE7005DE6
      FF0046D5FC000197D20061D1ED0063CBE30045A1BA0070605600E1CBC600E1AA
      9500F6C5AB00FBD6C100FDDDCB00FEE1CF00FEE1D100FEDFCE00FCDAC600FAD3
      BE00EAAD9300D7ADA500B9968E0092634F007177720022A2C4001C94C500CED3
      D5000000000000000000000000000000000000000000F2F2F200899E8B0074C6
      7C0047C353004CCA58004ACA550049BC52005CBE61005ABE61005ABE62005ABF
      62005BC063005CC064005EC164005FC166005EC1660087C28D00FCFDFD00FDFE
      FD00FEFEFE00FEFEFE00FEFEFE00CDE5CF004EBE57004AC956004BCA58004AC5
      56006AC673006C977000E9E9E900FEFEFE0086868600F0FBFF00D6E7E700CCCC
      CC00868686000000000000000000000000000000000000000000000000000000
      0000C0C0C000000000000000000099FFFF0066CCCC000066660000666600F0FB
      FF00D6E7E700CCCCCC0099999900006666000066660000CCCC00009999000000
      000000000000000000000000000000000000FDFDFC00D0816A00FDF6F300FFF9
      F500FFF9F500FFF9F500FEF8F300FCCDBC00FBBDA600CDAB9E00E7E6E600F5F5
      F500F7F6F600E2E1E000C8AA9A00F7C39800FED7A600FED6A500FDCB9B00FBB7
      8B00F6A37E00D1BFB800B0A39F00A9604D00EAE8E70000000000000000000000
      000000000000000000000000000000000000FDFDFD006FBAD60017B4EA0064EA
      FF005DE5FF000BA6DA002EAAD20061BED50054808900B1928500E4BAAD00EEAD
      8E00F8CAAF00FACDB500FBD5BE00FCD7C200FCD8C300FAD5C100FBD2BB00FACD
      B500F5C2A600E39D8000D5B9B9009266560020BEE90032DDFF000394CC0074A9
      BF000000000000000000000000000000000000000000F1F1F1007496770071C6
      7A0048C654004ACB57006DC17700EEF5EE00FEFEFE00FEFEFE00FEFEFF00FEFE
      FE00FEFEFE00FEFEFE00FFFDFE00FEFDFE00FEFEFE00FDFEFE00FDFEFD00FDFE
      FD00FEFEFE00FEFFFD00FDFDFE00FDFDFE00CCE5CF0046BF520048CA560049C8
      560055C060006EA37300E5E5E500FEFEFE0086868600F0FBFF00CCCCCC00CCCC
      CC00868686007777770086868600868686007777770086868600868686007777
      770086868600000000000066660066CCCC0099FFFF0066CCCC0000666600F0FB
      FF00D6E7E700CCCCCC00999999000066660000CCCC000099990000CCCC000000
      000000000000000000000000000000000000FAF8F700DC8F7400FEF9F500FFF7
      F200FEF6F100FCCEBC00FCD8CA00FDDCCF00FBBFA900F6BDA700C6B6B000F9F8
      F800FDFDFD00F9F9F800E2E1E100C6B3AB00DCA68B00F2AC8600F6A88100EAA3
      8500CFB5AB00C4C3C200B78375009F847D00FBFAFA0000000000000000000000
      000000000000000000000000000000000000FBFBFB0066B6D3001CB9EE0069EE
      FF006AEEFF001FB4E1001196C40058B3CC0062757500CDB5AD00DF9F8600F2B4
      9100F5C2A600F6C6AB00F7CBB200FACDB700FACFB700FACEB600F8CBB200F7C6
      AB00F5BFA100EDA78300D9B2AB00AF8173002EA6C30034DFFF0016B1E1001D98
      CB00FDFDFD0000000000000000000000000000000000EFEFEF007096740067C3
      700047C5540046C45100D0E7D100FEFEFF00FEFEFE00FEFEFE00FEFEFF00FEFE
      FF00FEFFFF00FEFFFF00FEFEFF00FEFDFE00FEFDFD00FFFEFE00FEFDFE00FEFE
      FD00FEFFFE00FEFFFD00FEFEFD00FDFDFE00FDFEFD0073C67A0048C9570048C7
      560048BC540073AA7700E4E4E400FDFDFD0086868600F0FBFF00D6E7E700CCCC
      CC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00C0C0C000C0C0
      C000CCCCCC000066660066CCCC00D6E7E70066CCCC000080800000666600F0FB
      FF00D6E7E700CCCCCC00999999000066660000CCCC0000CCCC000099990000CC
      CC0000000000000000000000000000000000F1EAE800E9A88C00FEF6F100FEF4
      EE00FEF5EF00FDDDD000FBC7B300FAB49B00F9A58700F9A18200ECA28800C8B5
      AE00F5F5F500F6F6F600F0EFEF00E6E5E400D7D5D400C8C3C100C5BFBD00CAC7
      C500C3C2C000BEBAB600D4806A00BFBAB9000000000000000000000000000000
      000000000000000000000000000000000000F7F7F7005BB3D40026C1F2006FF2
      FF006FF1FF0047CFF0000187BA0043A3C10077787200D7C2BB00E69D7E00F7CB
      B200FCD8C300FCD9C600FBD3BE00F7CAAE00F7C4A700F5C4A600F6C0A200F5BE
      9D00F4B89700EFAA8600DBA69600C09C9200449AAC0033DFFF002ACEFA000396
      CE00A5BDC70000000000000000000000000000000000F1F1F1007197750067C4
      6E0048C7550049C55300D1E7D200FEFEFE00FEFEFE00FEFEFE00FEFEFF00FEFE
      FF00FEFFFF00FEFEFF00FEFEFF00FEFEFF00FEFEFE00FFFFFE00FFFEFF00FFFE
      FF00FEFEFE00FDFEFC00FDFEFD00FEFEFE00FEFEFE0074C77B004ACB560048C7
      560048BC530071AA7600E6E6E600FEFEFE0086868600F0FBFF00EAEAEA00D6E7
      E700D6E7E700D6E7E700CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00CCCCCC000066660066CCCC0066CCCC00006666000066660000666600D6E7
      E700D6E7E700CCCCCC009999990000666600000000000066660000CCCC000099
      990000000000000000000000000000000000EBE1DC00EFBBA100FEF2EA00FEF1
      E900FEF2EB00FEF4ED00FEF5EF00FEF6F100FEF3ED00FCCDBC00FAB29800EFA8
      8E00C6ADA300D4D2D100E1E0DF00DAD9D800D1D0CF00CCCBCA00BFBDBC00BFBA
      B500DCCFC500FAEBE200B3655100E4E2E2000000000000000000000000000000
      000000000000000000000000000000000000F4F5F50054BBE2002EC4F30075F5
      FF0072F3FF0061E1F7000187BA000E88B40077777300D8C2BD00EAAD9200FBD6
      C000FBD9C300FDD9C700FDDCC900FCDAC500F4BE9E00F3BA9B00F4B89700F2B7
      9300F2B28E00EEA68000DCA59300C1A0970077A7AD0074F2FF0057E6FF000EA8
      DA00549CB800F5F5F500000000000000000000000000F3F3F30078987B0072C7
      7A0047C6540049CB560070C17700EFF7F000FDFDFD00FEFDFD00FFFDFE00FEFE
      FE00FEFFFE00FEFEFD00FEFEFE00FFFEFE00FFFEFC00FEFEFD00FEFEFE00FEFE
      FE00FEFEFE00FDFEFD00FDFDFD00FDFEFD00D0E6D20049C1520049C9560048C8
      550055C060006FA37400E9E9E900FEFEFE0086868600F0FBFF00D6E7E700D6E7
      E70096969600D6E7E700D6E7E700D6E7E700EAEAEA00D6E7E700D6E7E700EAEA
      EA00D6E7E70000666600006666000066660090A9AD0090A9AD0000666600CCCC
      CC0099999900999999009999990000666600CCCCCC0099999900006666000066
      660000000000000000000000000000000000E5D9D200F3CBB200FEECE100FEED
      E200FDE1D400FABDA500FBCFBC00FDE1D500FEF0E900FEF5EF00FDDFD300FCC4
      B000FABFA900E2B3A100C6ABA000BBABA400B6AAA400BFB2AB00E2C9BD00FDEF
      E400FFEFE300F5D2C7009D6E6200F6F4F4000000000000000000000000000000
      000000000000000000000000000000000000ECEEEF004BBAE40037CCF5007BFA
      FF0079F7FF006FEAFC0035B5D5000182B2003E5F6700D2BAB300E8B19A00FCD9
      C400FDDCCA00FCDDCA00FEDCCB00FEDFCE00FBD5C000F4BB9C00F2B49000F1B1
      8E00EFAC8500E99A7100DDB1A600B28D810094C0C400A5FCFF00A1FCFF006EDC
      F500138FC500BEC7CC00000000000000000000000000F7F7F70090A3910076C7
      7E0047C3530049C756004ECB5A0071CA7B0080CC870082CE880081CE880082CE
      870082CE870082CF880082CF870082CF870082CD8700A3D0A500FDFEFD00FEFD
      FE00FEFEFE00FEFEFD00FDFDFD00D8E9D90077CD7F0055CB60004AC9560048C6
      55006AC774006F9A7300F0F0F0000000000086868600F0FBFF00D6E7E700D6E7
      E7008686860099999900C0C0C000C0C0C000C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000CCCCCC0090A9AD00F0FBFF00F0FBFF00F0FBFF0000666600F0FB
      FF00D6E7E700D6E7E700CCCCCC0000666600CCCCCC0099999900000000000000
      000000000000000000000000000000000000E2D3C800F5D4BD00FDE6D700FDE7
      DA00FDE7DA00FCD8C700FBC9B500FABAA200FAAE9400FAAE9300FAB8A000FBC2
      AD00FBC2AD00FCC6B200FCD0BE00FCDCCE00FDEEE600FEF4EC00FFF3EA00FFF2
      E800FFF2E700ECB1A1009F8A8500FCFBFB000000000000000000000000000000
      000000000000000000000000000000000000E7E9EA0046B9E30041D3F80080FD
      FF007EFBFF0073F0FE0059D0E40032ABC7003A707C00BDA09500E5BBAD00F8D2
      BF00FEDFCE00FDE1CE00FDDFCF00FFE2D000FEE1D000FDDCCA00FBD6C000FBD6
      C000F6C6AA00E38F6A00DEC5C3009E726100ABE3EB00B4FFFF00AFFDFF00A9FC
      FF000296CD0073A4B900000000000000000000000000FBFBFB00BDC2BD0074C0
      7A0045BF500049C8560068D272007DD785007DD885007DD885007ED886007ED8
      86007ED886007FD986007FD984007ED7850098D39E00F9FAF900FDFDFE00FEFD
      FF00FDFEFE00FDFEFD00DAECDD007FD187007ED886006FD478004AC8550047C3
      530076C67E008CA28E00F6F7F6000000000086868600F0FBFF00D6E7E700D6E7
      E7008686860000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FB
      FF00C0C0C000D6E7E70090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C0000066
      6600006666000066660000666600CCCCCC0090A9AD0099999900000000000000
      000000000000000000000000000000000000E3D6CA00F7D6BD00FBD0BB00FCE1
      D000FDE5D600FEE8DB00FEECE100FEEFE600FEF2EA00FEF1EA00FEE8DF00FCDA
      CD00FCCAB800FABAA300FAAF9400FAAE9300FAB69D00FBC4AF00FEF1E800FFF3
      EB00FFF4ED00DF8F7A00B1ABA900FEFEFE000000000000000000000000000000
      000000000000000000000000000000000000E0E3E4003EB6E3004CDCFC0086FF
      FF0084FFFF007DF8FF0069E1EF004DC3D9003E9BAC007B655800E7D0CB00EBBB
      A800FFE6D900FFE5D700FFE4D700FFE6D800FFE6D800FFE6D800FFE6D700FFE6
      DA00F8D0BE00DFAC9C00CEB7B20097817300CBFFFF00CAFFFF00C5FFFF00C4FF
      FF005DC9E7000E95CD00F6F8F8000000000000000000FDFDFD00E7E7E7006BAA
      710057C1620049C656007FD8880084DA8C0084DA8C0084DA8C0085DA8E0083DB
      8E0084DB8D0085DC8C0084DB8C009FD6A300F9F9F900FDFEFD00FEFEFE00FEFE
      FE00FEFEFE00DDEEDE0086D38E0086DB8C0085DA8C0084D98C004CC9570046BE
      520073BC7B00C7CAC700FAFAFA000000000086868600F0FBFF00EAEAEA00D6E7
      E7007777770000000000F0FBFF00F0FBFF0000000000F0FBFF00F0FBFF00F0FB
      FF00C0C0C000D6E7E70090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000868686000000000090A9AD0090A9AD0099999900000000000000
      000000000000000000000000000000000000E8DFD700F8DABF00FBB89D00FBBE
      A300FBCAB400FCD4C100FDDFCF00FDE9DD00FDF0E700FEF4ED00FFF6F100FFF8
      F400FFFAF600FFFAF700FEF8F400FEF4EF00FEEAE000FDDED000FEF4ED00FFF5
      ED00FEF6F000CC776000C8C6C600000000000000000000000000000000000000
      000000000000000000000000000000000000D7DDDF0028ADDF0059E4FF0089FF
      FF0088FFFF0085FFFF0077EEF8005FD5E40041B4CB0052777800BE9B8D00E6C5
      BB00F4C7B400FFEADC00FFEADD00FFEADF00FFEADE00FFEADD00FFEADE00FFE4
      D800E4AE9C00E8D5D200A67B6A00C3CDC600E0FFFF00DEFFFF00D8FFFF00D5FF
      FF00A1ECFB000499CD00BBD8E200000000000000000000000000F6F7F600829E
      860079C980004CC459008DDD94008DDE94008DDC94008DDC94008DDD95008CDD
      95008DDD94008EDE940099D79E00F9FBFA00FDFEFE00FEFFFE00FDFEFD00FEFE
      FD00E0EEE1008ED694008EDE95008DDD95008CDD94008DDB950056C961006BC7
      7400699E7000EEEFEE00FEFEFE000000000086868600F0FBFF00D6E7E700D6E7
      E7008686860000000000F0FBFF0008080800C0C0C00008080800F0FBFF00F0FB
      FF00C0C0C000D6E7E70090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000868686000000000090A9AD0090A9AD0099999900000000000000
      000000000000000000000000000000000000F9F7F500EFD0A400FCD8C600FBB6
      9A00FBB59800FBB89D00FBBBA100FBC0A900FDEADE00FEF2EA00FEF5F000FFF8
      F300FFFAF600FFFBF800FFFAF700FFF9F600FFF9F400FFF8F300FFF7F100FFF6
      EF00FDF4EF00B6675200DAD7D700000000000000000000000000000000000000
      000000000000000000000000000000000000CFD5D8001FA6DD0066EDFF008FFF
      FF008FFFFF008DFFFF0086FEFF0078EEF5005DCFDF0046BBCF00676E6700B89C
      9100E3C5BE00E9BDAD00F5D2C300FBDBCD00FCDED000F8D8C700EFC7B700E3B4
      A300E1CECC00A07E71002F758A0031AFDD0060C5E60081D6EE00A0E5F500AEEE
      FB00BCF5FF0038B3DB0048A1C200F3F3F4000000000000000000FCFCFC00DDDE
      DD006AB071005DC3690094DE9B0096E09D0095DF9D0096DF9C0096E09C0096E0
      9C0096DE9C0096DE9C00C4E5C700FDFEFD00FEFFFF00FEFFFE00FEFEFE00E2EF
      E30096D99D0096DF9D0095DF9D0095DF9D0095E09D0095E09D005CC4660076C1
      7E00B2BBB300F8F9F800000000000000000086868600F0FBFF00D6E7E700D6E7
      E7008686860008080800F0FBFF00F0FBFF00F0FBFF00C0C0C00008080800F0FB
      FF00C0C0C000D6E7E70090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000868686000000000090A9AD0090A9AD0099999900000000000000
      00000000000000000000000000000000000000000000DCD2C300F7D29F00FBD8
      C300FCD7C900FCD8C900FBCEBB00FBBDA400FDE6DA00FEF0E700FEF4EE00FFF7
      F300FFF9F600FFFBF800FFFBF800FFFAF700FFFAF600FFF9F400FFF8F300FFF7
      F100FCF2ED00A8604D00E2E0E000000000000000000000000000000000000000
      000000000000000000000000000000000000C9D0D3001AA4DD0070F2FF0093FF
      FF0093FFFF0092FFFF0090FFFF0087FDFF0076E9F10062D6E20051B6C3006579
      7200AA817200D8BEB800EBD1CA00EBCBC100EAC7BD00EBCCC400E8D0CB00D2B8
      B20096705F007CA6A00036C6EB00019AD1002191BF001F94C600089AD300049D
      D30019A6D800099ED20047A0C200F3F3F300000000000000000000000000F7F8
      F70093A896007EC684009ADA9F009CE1A3009DE2A4009EE2A4009EE2A4009EE2
      A4009EE1A4009EE3A400B6E0BA00FEFEFE00FDFEFE00FDFEFE00E3F1E400A0DC
      A4009DE3A5009DE2A4009DE2A3009DE2A4009BE1A3009ADCA10080CB8600709D
      7400F2F3F200FEFEFE00000000000000000086868600F0FBFF00D6E7E700D6E7
      E7008686860000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF00C0C0C000F0FB
      FF00C0C0C000D6E7E70090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000868686000000000090A9AD0090A9AD0099999900000000000000
      00000000000000000000000000000000000000000000FEFEFD00D5C4AA00F7BB
      8000F89C7400F9987700FCD4C600FBBCA200FDE3D500FEEEE500FEF3EC00FFF7
      F200FFF9F600FFFBF900FFFCF900FFFBF800FFFAF700FFFAF600FFF9F500FFF8
      F300FAEAE500A3625100E8E6E600000000000000000000000000000000000000
      000000000000000000000000000000000000C3CBCF0013A2DD0071F3FF0097FF
      FF0097FFFF0097FFFF0096FFFF0094FFFF008DFFFF0083F5FA0073E5ED0065DB
      E60066ADAF0071796D008A786A00A28E8000AD9A8F00A18D7E00887A69008190
      82008AF2F1008FFFFF0073F4FF000AA9DE009CA9B00000000000F0F0F000D9DD
      DF00B0BEC500A0BCC800EFF2F30000000000000000000000000000000000FEFE
      FE00EEEFEE006E9C7200A8DBAD00A2E1A900A5E4AB00A6E6AC00A4E4AC00A4E4
      AC00A5E5AC00A4E5AB00A3E0A900CBE6CC00E7F3E800D5EAD500A7DFAB00A6E4
      AB00A4E4AB00A4E4AB00A5E5AC00A5E4AB00A2DFAB00ABDBB20068A86D00DADC
      DB00FCFCFC0000000000000000000000000086868600F0FBFF00EAEAEA00D6E7
      E7007777770000000000F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FB
      FF00C0C0C000EAEAEA0090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000868686000000000090A9AD0090A9AD0099999900000000000000
      0000000000000000000000000000000000000000000000000000FDFCFB00DEC4
      9F00F7B07A00F9956F00FBCDBC00FBBBA000FDE0D000FEEEE400FEF2EB00FEF6
      F100FFF9F600FFFBF900FFFCFA00FFFCF900FFFBF800FFFAF700FFFAF600FFF9
      F500F9E4DD009D635300EBE9E800000000000000000000000000000000000000
      000000000000000000000000000000000000D1D7DA0020ABE10043D6F5009AFF
      FF009CFFFF009CFFFF009BFFFF009AFFFF0097FFFF0093FFFF008BFCFE0080F3
      F80040C7E60046CAE50056C0D20063C0CB0075CCCF0080DADA008BF3F30090FF
      FF0092FFFF0095FFFF0075F5FF000AA9DE009CABB20000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FAFBFA00E7E8E7006DA37300B1DFB400AAE2AF00ACE7B100ABE6B300ABE7
      B300ACE8B200ACE7B300ACE6B200ABE5B100ACE6B100ACE6B000ABE6B300ADE7
      B200ACE6B300ACE6B300ACE7B200ABE3B000B3E0B90074B27C00CBCFCB00F9FA
      F9000000000000000000000000000000000086868600F0FBFF00D6E7E700D6E7
      E700868686000000000000000000000000000000000000000000080808000000
      0000B2B2B200D6E7E70090A9AD00F0FBFF00C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000868686000808080090A9AD0090A9AD0099999900080808000000
      000000000000000000000000000000000000000000000000000000000000FAF8
      F600DCBE9400F8A57600FBC8B500FBBBA100FCDCCB00FEEDE200FEF2EB00FEF6
      F100FFF9F600FFFCF900FFFDFB00FFFCFA00FFFCFA00FFFBF900FFFBF800FFFA
      F700F8E2DC009D635300EAE8E800000000000000000000000000000000000000
      000000000000000000000000000000000000FDFDFD0086C1D70003ACE50077F4
      FF00A0FFFF009FFFFF009FFFFF009FFFFF009EFFFF009EFFFF008BFFFF002AC1
      EA000E97D1002796C500189ED6000CA1DB00009FDD0000A6DE0010B1E5001EB8
      E70030C5ED0040CFF20035C9EF00009FD9009DB4BE0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FBFBFB00E6E7E6006F9D7200A8D9AC00B7E5BB00B3E6B800B3E7
      B900B4E8B900B5EAB900B4E9B700B3E8B700B5E9B900B4E7B700B5E8B800B4EA
      B800B3E8B800B4E7B900B3E4B800B6E2BA006DA97300CDD1CD00F9FAF9000000
      00000000000000000000000000000000000086868600F0FBFF00D6E7E700D6E7
      E700868686008686860077777700868686008686860086868600777777008686
      860086868600D6E7E70090A9AD00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FB
      FF00F0FBFF00F0FBFF000000000090A9AD0090A9AD0099999900000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F8F5F200E7C19400FAC3A500FBC2AA00FCD8C600FEEDE200FEF2EB00FEF6
      F100FFF9F600FFFCFA00FFFDFC00FFFDFB00FFFCFB00FFFCFA00FFFBF900FFFB
      F800FAEBE6009E625200E4E3E300000000000000000000000000000000000000
      00000000000000000000000000000000000000000000E9ECED000CAAE9002EC6
      F0008BFFFF0097FFFF009DFFFF009FFFFF00A1FFFF009FFFFF003BCDF10000A2
      E30090B1BF00D7D9DB00C8CFD300BCC6CB00AABEC70092B5C40073A3B8005BA7
      C5003A9AC20026A2D50012A3DC0037A6D100E4E5E60000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FBFCFB00F3F4F30091A794007BBC8200BAE3BE00BCE6
      C000B9E5BD00BCE9BF00BBE9BE00BAE9BE00BAE9BF00BBE8BE00BCE8BF00B9E7
      BD00B9E6BD00C1E7C5008DCA9200799E7D00E7E9E700FAFBFA00000000000000
      00000000000000000000000000000000000086868600F0FBFF00EAEAEA00D6E7
      E700EAEAEA00D6E7E700EAEAEA00D6E7E700EAEAEA00D6E7E700EAEAEA00D6E7
      E700EAEAEA00D6E7E70090A9AD0090A9AD0090A9AD0090A9AD0090A9AD0090A9
      AD0090A9AD0090A9AD0090A9AD0090A9AD0090A9AD0099999900000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000F7F3EF00E7C09700FCD7C500FBCFBB00FDECE200FEF2EB00FEF7
      F200FFFAF700FFFCFB00FFFEFD00FFFDFC00FFFDFC00FFFDFB00FFFCFA00FFFB
      F900FCF2EE00A45F4D00DEDCDC00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000097B9C9001DA1
      D800059CDC0001A0DE0006ACE50019B9E8002EC5EF0029C3EE00089CD8007EAD
      C200000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FCFCFC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FEFEFE00FAFAFA00DDE0DD007E9F82007AB7
      7D00A2D4A800C1E4C500C9E9CD00C7EACA00C6EAC900C9E9CB00C5E6C900ACD9
      B10083C18800729D7500C8CFC900F8F9F800FEFEFE0000000000000000000000
      00000000000000000000000000000000000086868600D6E7E700F0FBFF00F0FB
      FF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FB
      FF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FB
      FF00F0FBFF00F0FBFF00F0FBFF00F0FBFF00F0FBFF0099999900000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F5F1EC00E8CCA600FCE2CF00FDF1E800FDF6F000FEF9
      F500FEFBF900FFFDFC00FFFEFD00FFFEFD00FFFDFC00FFFDFC00FFFDFB00FFFC
      FA00FDF7F500AD635100D5D3D300000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FAFAFA00E4ED
      F100D8EBF200CDF1FE00B6E9FB0088D9F60064CBF2002DB6EB009DD9EE00F7F8
      F800000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FDFDFD00FAFAFA00EEEF
      EE00B6C0B70086A28900769F7B0076A27C0077A17B0076A07A007E9F8200A9B7
      AA00E5E7E500F8F8F800FCFDFC00FEFEFE000000000000000000000000000000
      0000000000000000000000000000000000008686860086868600868686008686
      8600868686008686860086868600868686008686860086868600868686008686
      8600868686008686860086868600868686008686860086868600868686008686
      8600868686008686860086868600868686008686860086868600868686000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FAF8F600E0D4BD00CCB79500E3C39A00E9C3
      9900E7BD9500EBBF9900F0C2A100F1C4A700F2C6AE00F3CAB700F4D0C100F5D6
      CB00F6D9D000BF6F5900E0DDDD00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FEFEFE00FEFE
      FE00FCFDFC00FCFDFC00F8F8F800F8F8F800F9F9F900F8F8F800FBFCFB00FDFD
      FD00FEFEFE00FEFEFE0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FEFEFE00FDFC
      FC00FBFBFA00FAF8F700F4EFEC00EEE8E300EAE1DB00E8DAD200E0CBC200DAC0
      B500D2B3A900DBCCC700FDFDFD00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000080000000600000000100010000000000000600000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFFFFFFFF0000000000000000
      FC07FFFFFFFFFFFF0000000000000000F001FFFFC3FFFFFF0000000000000000
      E000FFFF803FFFFF0000000000000000E3E07FFF0003FFFF0000000000000000
      E2187FFF00007FFF0000000000000000E2107F8700003FFF0000000000000000
      E2003003000007FF0000000000000000E0000001000000FF0000000000000000
      E00000010000007F0000000000000000F00000010000007F0000000000000000
      C00000010000007F0000000000000000800000010000003F0000000000000000
      800000010000003F0000000000000000800000010000003F0000000000000000
      800000010000007F0000000000000000800000010000007F0000000000000000
      C00000010000007F0000000000000000C00002010000003F0000000000000000
      E00002010000001F0000000000000000F00F0003000000070000000000000000
      F0060003000000030000000000000000F8000003000000030000000000000000
      F8000003000000010000000000000000FC000013000000000000000000000000
      FC8003C3000008000000000000000000FE403C0F00001C000000000000000000
      FF4FE07FC0003C010000000000000000FF3E07FFF8007F030000000000000000
      FF803FFFFF81FF870000000000000000FFC3FFFFFFFFFFFF0000000000000000
      FFFFFFFFFFFFFFFF0000000000000000FFFFFFE7FFFFFFFFFFF007FFFFFFFFFF
      F83FFF81FFFFFFFFFF8001FF0000001FF007FE01FFFFFFC7FE00007F0000001F
      F0001800C3FFFF83FC00003F0000000FE0000000801FFF03F800001F00000007
      E00000008003FE01F000000F00000007C000000080001C01E000000700000007
      C000000380000001C000000300000001C000000780000003C000000300000000
      8000001F80000003C0000003000000008000001F8000000F8000000100000000
      8000003F8000000F80000000000000008000003F8000000F8000000000000001
      8000007F8000000F80000000000000070000007F0000000F8000000000000007
      0000007F000000078000000000000007000000FF000000078000000000000007
      000000FF00000003800000000000000F000000FF00000003800000010000001F
      000000FF00000003800000010000001F000000FF00000001800000010000001F
      000001FF00000001C00000010000001F000001FF00000000C00000030000001F
      800001FF00000000E00000030000001F800001FF00000041E00000070000001F
      C00001FF0000007FF000000F0000001FE00001FF0000007FF800001F0000001F
      F00001FF8000007FFC00003F0000001FF80001FFC00FFDFFFE00007F0000001F
      FC0001FFC00FFFFFFF8000FF0000001FFE0001FFFFFFFFFFFFC003FFFFFFFFFF
      FFC001FFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object ToolBar_View_Menu: TTntPopupMenu
    Images = Menu_Image
    Left = 744
    Top = 232
    object ToolBar_View_Easy: TTntMenuItem
      AutoCheck = True
      Caption = 'a'
      RadioItem = True
      OnClick = M_View_EasyClick
    end
    object ToolBar_View_Sheet: TTntMenuItem
      AutoCheck = True
      Caption = 'b'
      RadioItem = True
      OnClick = M_View_SheetClick
    end
    object ToolBar_View_Tree: TTntMenuItem
      AutoCheck = True
      Caption = 'c'
      RadioItem = True
      OnClick = M_View_TreeClick
    end
    object ToolBar_View_Text: TTntMenuItem
      AutoCheck = True
      Caption = 'd'
      RadioItem = True
      OnClick = M_View_TextClick
    end
    object ToolBar_View_HTML: TTntMenuItem
      AutoCheck = True
      Caption = 'e'
      RadioItem = True
      OnClick = M_View_HTMLClick
    end
    object ToolBar_View_Custom: TTntMenuItem
      AutoCheck = True
      Caption = 'f'
      RadioItem = True
      OnClick = M_View_CustomClick
    end
    object N3: TTntMenuItem
      Caption = '-'
    end
    object ToolBar_View_System: TTntMenuItem
      Caption = 'Your System'
      ImageIndex = 6
      OnClick = M_View_SystemClick
    end
    object N4: TTntMenuItem
      Caption = '-'
    end
    object ToolBar_View_ShowMenu: TTntMenuItem
      Caption = 'Show menu'
      Visible = False
      OnClick = M_Options_ShowMenuClick
    end
  end
  object ToolBar_Image_Disabled: TImageList
    Height = 32
    Width = 32
    Left = 744
    Top = 184
  end
  object OpenDialog1: TTntOpenDialog
    Left = 744
    Top = 264
  end
end
