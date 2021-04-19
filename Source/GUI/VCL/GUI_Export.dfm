object ExportF: TExportF
  Left = 290
  Top = 286
  BorderStyle = bsSingle
  Caption = 'Export...'
  ClientHeight = 355
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 14
  object Cancel: TButton
    Left = 375
    Top = 318
    Width = 122
    Height = 26
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 0
  end
  object OK: TButton
    Left = 500
    Top = 318
    Width = 122
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = OKClick
  end
  object Export_Choose: TGroupBox
    Left = 1
    Top = 0
    Width = 621
    Height = 249
    Caption = 'Choose the desired export format'
    TabOrder = 2
    object Export: TPageControl
      Left = 0
      Top = 18
      Width = 608
      Height = 224
      ActivePage = Export_Text
      MultiLine = True
      TabOrder = 0
      OnChange = ExportChange
      object Export_CSV: TTabSheet
        Caption = 'CSV'
        object CSV_Stream_Video_Caption: TLabel
          Left = 0
          Top = 9
          Width = 129
          Height = 14
          Caption = 'How many video streams?'
        end
        object CSV_Stream_Audio_Caption: TLabel
          Left = 0
          Top = 34
          Width = 129
          Height = 14
          Caption = 'How many audio streams?'
        end
        object CSV_Stream_Text_Caption: TLabel
          Left = 0
          Top = 60
          Width = 121
          Height = 14
          Caption = 'How many text streams?'
        end
        object CSV_Stream_Other_Caption: TLabel
          Left = 0
          Top = 86
          Width = 146
          Height = 14
          Caption = 'How many chapters streams?'
        end
        object CSV_Stream_Video_Warning: TLabel
          Left = 244
          Top = 9
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Stream_Audio_Warning: TLabel
          Left = 244
          Top = 34
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Stream_Text_Warning: TLabel
          Left = 244
          Top = 59
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Stream_Other_Warning: TLabel
          Left = 244
          Top = 86
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Quote_Caption: TLabel
          Left = 0
          Top = 112
          Width = 79
          Height = 14
          Caption = 'Quote character'
        end
        object CSV_Separator_Col_Caption: TLabel
          Left = 0
          Top = 138
          Width = 85
          Height = 14
          Caption = 'Column separator'
        end
        object CSV_Separator_Line_Caption: TLabel
          Left = 327
          Top = 138
          Width = 70
          Height = 14
          Caption = 'Line separator'
        end
        object CSV_Decimal_Caption: TLabel
          Left = 328
          Top = 112
          Width = 63
          Height = 14
          Caption = 'Decimal point'
          Enabled = False
        end
        object CSV_Stream_Video: TComboBox
          Left = 190
          Top = 0
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemIndex = 1
          TabOrder = 0
          Text = '1'
          OnChange = CSV_Stream_VideoChange
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8')
        end
        object CSV_Stream_Audio: TComboBox
          Left = 190
          Top = 26
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemIndex = 2
          TabOrder = 1
          Text = '2'
          OnChange = CSV_Stream_AudioChange
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16')
        end
        object CSV_Stream_Text: TComboBox
          Left = 190
          Top = 52
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemIndex = 2
          TabOrder = 2
          Text = '2'
          OnChange = CSV_Stream_TextChange
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20'
            '21'
            '22'
            '23'
            '24'
            '25'
            '26'
            '27'
            '28'
            '29'
            '30'
            '31'
            '32')
        end
        object CSV_Stream_Other: TComboBox
          Left = 190
          Top = 78
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemIndex = 1
          TabOrder = 3
          Text = '1'
          OnChange = CSV_Stream_OtherChange
          Items.Strings = (
            '0'
            '1')
        end
        object CSV_Quote: TComboBox
          Left = 190
          Top = 103
          Width = 61
          Height = 22
          TabOrder = 4
          Text = '"'
          Items.Strings = (
            '"')
        end
        object CSV_Separator_Col: TComboBox
          Left = 190
          Top = 129
          Width = 61
          Height = 22
          ItemIndex = 0
          TabOrder = 5
          Text = ';'
          Items.Strings = (
            ';'
            ','
            '(Tab)')
        end
        object CSV_Separator_Line: TComboBox
          Left = 517
          Top = 129
          Width = 61
          Height = 22
          TabOrder = 6
          Text = '\r\n'
          Items.Strings = (
            '(Default)'
            '\r\n'
            '\n')
        end
        object CSV_Advanced: TCheckBox
          Left = 0
          Top = 158
          Width = 208
          Height = 19
          Caption = 'Advanced : full information'
          TabOrder = 7
        end
        object CSV_Decimal: TComboBox
          Left = 518
          Top = 103
          Width = 61
          Height = 22
          Enabled = False
          TabOrder = 8
          Text = '.'
          Items.Strings = (
            '.'
            ',')
        end
      end
      object Export_Sheet: TTabSheet
        Caption = 'Sheet'
        object Sheet_Quote_Caption: TLabel
          Left = 0
          Top = 9
          Width = 79
          Height = 14
          Caption = 'Quote character'
        end
        object Sheet_Separator_Col_Caption: TLabel
          Left = 0
          Top = 34
          Width = 85
          Height = 14
          Caption = 'Column separator'
        end
        object Sheet_Separator_Line_Caption: TLabel
          Left = 327
          Top = 26
          Width = 70
          Height = 14
          Caption = 'Line separator'
        end
        object Sheet_Quote: TComboBox
          Left = 190
          Top = 0
          Width = 61
          Height = 22
          TabOrder = 0
          Text = '"'
          Items.Strings = (
            '"')
        end
        object Sheet_Separator_Col: TComboBox
          Left = 190
          Top = 26
          Width = 61
          Height = 22
          ItemIndex = 0
          TabOrder = 1
          Text = ';'
          Items.Strings = (
            ';'
            ','
            '(Tab)')
        end
        object Sheet_Separator_Line: TComboBox
          Left = 517
          Top = 17
          Width = 61
          Height = 22
          TabOrder = 2
          Text = '\r\n'
          Items.Strings = (
            '(Default)'
            '\r\n'
            '\n')
        end
      end
      object Export_Text: TTabSheet
        Caption = 'Text'
        ImageIndex = 1
        object Text_Advanced: TCheckBox
          Left = 0
          Top = 9
          Width = 585
          Height = 18
          Caption = 'Advanced : full information'
          TabOrder = 0
        end
      end
      object Export_HTML: TTabSheet
        Caption = 'HTML'
        ImageIndex = 2
        object HTML_Advanced: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Advanced : full information'
          TabOrder = 0
        end
      end
      object Export_XML: TTabSheet
        Caption = 'XML'
        object Export_XML_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.mediainfo.xml)'
          TabOrder = 0
          OnClick = Export_XML_SideCarClick
        end
      end
      object Export_JSON: TTabSheet
        Caption = 'JSON'
        object Export_JSON_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.mediainfo.json)'
          TabOrder = 0
          OnClick = Export_JSON_SideCarClick
        end
      end
      object Export_Graph_Svg: TTabSheet
        Caption = 'Graph'
        object Export_Graph_Svg_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.Graph.svg)'
          TabOrder = 0
          OnClick = Export_Graph_Svg_SideCarClick
        end
      end
      object Export_MPEG7: TTabSheet
        Caption = 'MPEG-7'
        object Export_MPEG7_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.mpeg7)'
          TabOrder = 0
          OnClick = Export_MPEG7_SideCarClick
        end
      end
      object Export_PBCore: TTabSheet
        Caption = 'PBCore 1.2'
        object Export_PBCore_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.PBCore.xml)'
          TabOrder = 0
          OnClick = Export_PBCore_SideCarClick
        end
      end
      object Export_PBCore2: TTabSheet
        Caption = 'PBCore 2.0'
        object Export_PBCore2_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.PBCore2.xml)'
          TabOrder = 0
          OnClick = Export_PBCore2_SideCarClick
        end
      end
      object Export_EBUCore_1_5: TTabSheet
        Caption = 'EBUCore 1.5'
        ImageIndex = 9
        object Export_EBUCore_1_5_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.EBUCore.xml)'
          TabOrder = 0
          OnClick = Export_EBUCore_1_5_SideCarClick
        end
      end
      object Export_EBUCore_1_6: TTabSheet
        Caption = 'EBUCore 1.6'
        ImageIndex = 9
        object Export_EBUCore_1_6_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.EBUCore.xml)'
          TabOrder = 0
          OnClick = Export_EBUCore_1_6_SideCarClick
        end
      end
      object Export_EBUCore_1_8_ps: TTabSheet
        Caption = 'EBUCore 1.8 (ps)'
        ImageIndex = 9
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Export_EBUCore_1_8_ps_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.EBUCore.xml)'
          TabOrder = 0
          OnClick = Export_EBUCore_1_8_ps_SideCarClick
        end
      end
      object Export_EBUCore_1_8_sp: TTabSheet
        Caption = 'EBUCore 1.8 (sp)'
        ImageIndex = 9
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Export_EBUCore_1_8_sp_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.EBUCore.xml)'
          TabOrder = 0
          OnClick = Export_EBUCore_1_8_sp_SideCarClick
        end
      end
      object Export_EBUCore_1_8_ps_json: TTabSheet
        Caption = 'EBUCore 1.8 (ps, json)'
        ImageIndex = 9
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Export_EBUCore_1_8_ps_json_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.EBUCore.json)'
          TabOrder = 0
          OnClick = Export_EBUCore_1_8_ps_json_SideCarClick
        end
      end
      object Export_EBUCore_1_8_sp_json: TTabSheet
        Caption = 'EBUCore 1.8 (sp, json)'
        ImageIndex = 9
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Export_EBUCore_1_8_sp_json_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.EBUCore.json)'
          TabOrder = 0
          OnClick = Export_EBUCore_1_8_sp_json_SideCarClick
        end
      end
      object Export_FIMS_1_1: TTabSheet
        Caption = 'FIMS 1.1'
        ImageIndex = 9
        object Export_FIMS_1_1_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.FIMS.xml)'
          TabOrder = 0
          OnClick = Export_FIMS_1_1_SideCarClick
        end
      end
      object Export_FIMS_1_2: TTabSheet
        Caption = 'FIMS 1.2'
        ImageIndex = 9
        object Export_FIMS_1_2_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.FIMS.xml)'
          TabOrder = 0
          OnClick = Export_FIMS_1_2_SideCarClick
        end
      end
      object Export_FIMS_1_3: TTabSheet
        Caption = 'FIMS 1.3'
        ImageIndex = 9
        object Export_FIMS_1_3_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.FIMS.xml)'
          TabOrder = 0
          OnClick = Export_FIMS_1_3_SideCarClick
        end
      end
      object Export_NISO_Z39_87: TTabSheet
        Caption = 'NISO Z39.87'
        object Export_NISO_Z39_87_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.NISO_Z39_87.xml)'
          TabOrder = 0
          OnClick = Export_NISO_Z39_87_SideCarClick
        end
      end
      object Export_reVTMD: TTabSheet
        Caption = 'reVTMD'
        object Export_reVTMD_SideCar: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'Sidecar file (filename.reVTMD.xml)'
          TabOrder = 0
          OnClick = Export_reVTMD_SideCarClick
        end
      end
      object Export_Custom: TTabSheet
        Caption = 'Custom'
        ImageIndex = 3
        object Custom_One: TCheckBox
          Left = 0
          Top = 9
          Width = 597
          Height = 18
          Caption = 'One output file per input file'
          TabOrder = 0
        end
      end
    end
  end
  object Name_Choose: TGroupBox
    Left = 1
    Top = 248
    Width = 621
    Height = 70
    Caption = 'Choose the desired filename'
    TabOrder = 3
    object Name: TEdit
      Left = 9
      Top = 22
      Width = 576
      Height = 22
      TabOrder = 0
    end
    object Name_FileSelect: TButton
      Left = 586
      Top = 22
      Width = 27
      Height = 22
      Caption = '...'
      TabOrder = 1
      OnClick = Name_FileSelectClick
    end
    object File_Append: TCheckBox
      Left = 9
      Top = 43
      Width = 600
      Height = 18
      Caption = 'Append to the existing file (Warning : parameters must be same)'
      TabOrder = 2
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 8
    Top = 320
  end
end
