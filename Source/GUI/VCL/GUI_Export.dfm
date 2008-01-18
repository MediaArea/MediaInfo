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
  object Cancel: TTntButton
    Left = 375
    Top = 318
    Width = 122
    Height = 26
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 0
  end
  object OK: TTntButton
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
    object Export: TTntPageControl
      Left = 9
      Top = 17
      Width = 608
      Height = 224
      ActivePage = Export_Sheet
      MultiLine = True
      TabOrder = 0
      OnChange = ExportChange
      object Export_CSV: TTntTabSheet
        Caption = 'CSV'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object CSV_Stream_Video_Caption: TTntLabel
          Left = 0
          Top = 9
          Width = 129
          Height = 14
          Caption = 'How many video streams?'
        end
        object CSV_Stream_Audio_Caption: TTntLabel
          Left = 0
          Top = 34
          Width = 129
          Height = 14
          Caption = 'How many audio streams?'
        end
        object CSV_Stream_Text_Caption: TTntLabel
          Left = 0
          Top = 60
          Width = 121
          Height = 14
          Caption = 'How many text streams?'
        end
        object CSV_Stream_Chapters_Caption: TTntLabel
          Left = 0
          Top = 86
          Width = 146
          Height = 14
          Caption = 'How many chapters streams?'
        end
        object CSV_Stream_Video_Warning: TTntLabel
          Left = 244
          Top = 9
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Stream_Audio_Warning: TTntLabel
          Left = 244
          Top = 34
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Stream_Text_Warning: TTntLabel
          Left = 244
          Top = 59
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Stream_Chapters_Warning: TTntLabel
          Left = 244
          Top = 86
          Width = 40
          Height = 14
          Caption = 'Warning'
          Visible = False
        end
        object CSV_Quote_Caption: TTntLabel
          Left = 0
          Top = 112
          Width = 79
          Height = 14
          Caption = 'Quote character'
        end
        object CSV_Separator_Col_Caption: TTntLabel
          Left = 0
          Top = 138
          Width = 85
          Height = 14
          Caption = 'Column separator'
        end
        object CSV_Separator_Line_Caption: TTntLabel
          Left = 327
          Top = 138
          Width = 70
          Height = 14
          Caption = 'Line separator'
        end
        object CSV_Decimal_Caption: TTntLabel
          Left = 328
          Top = 112
          Width = 63
          Height = 14
          Caption = 'Decimal point'
          Enabled = False
        end
        object CSV_Stream_Video: TTntComboBox
          Left = 190
          Top = 0
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemHeight = 14
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
        object CSV_Stream_Audio: TTntComboBox
          Left = 190
          Top = 26
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemHeight = 14
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
        object CSV_Stream_Text: TTntComboBox
          Left = 190
          Top = 52
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemHeight = 14
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
        object CSV_Stream_Chapters: TTntComboBox
          Left = 190
          Top = 78
          Width = 44
          Height = 22
          Style = csDropDownList
          ItemHeight = 14
          ItemIndex = 1
          TabOrder = 3
          Text = '1'
          OnChange = CSV_Stream_ChaptersChange
          Items.Strings = (
            '0'
            '1')
        end
        object CSV_Quote: TTntComboBox
          Left = 190
          Top = 103
          Width = 61
          Height = 22
          ItemHeight = 14
          TabOrder = 4
          Text = '"'
          Items.Strings = (
            '"')
        end
        object CSV_Separator_Col: TTntComboBox
          Left = 190
          Top = 129
          Width = 61
          Height = 22
          ItemHeight = 14
          ItemIndex = 0
          TabOrder = 5
          Text = ';'
          Items.Strings = (
            ';'
            ','
            '(Tab)')
        end
        object CSV_Separator_Line: TTntComboBox
          Left = 517
          Top = 129
          Width = 61
          Height = 22
          ItemHeight = 14
          TabOrder = 6
          Text = '\r\n'
          Items.Strings = (
            '(Default)'
            '\r\n'
            '\n')
        end
        object CSV_Advanced: TTntCheckBox
          Left = 0
          Top = 158
          Width = 208
          Height = 19
          Caption = 'Advanced : full information'
          TabOrder = 7
        end
        object CSV_Decimal: TTntComboBox
          Left = 518
          Top = 103
          Width = 61
          Height = 22
          Enabled = False
          ItemHeight = 14
          TabOrder = 8
          Text = '.'
          Items.Strings = (
            '.'
            ',')
        end
      end
      object Export_Sheet: TTntTabSheet
        Caption = 'Sheet'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Sheet_Quote_Caption: TTntLabel
          Left = 0
          Top = 9
          Width = 79
          Height = 14
          Caption = 'Quote character'
        end
        object Sheet_Separator_Col_Caption: TTntLabel
          Left = 0
          Top = 34
          Width = 85
          Height = 14
          Caption = 'Column separator'
        end
        object Sheet_Separator_Line_Caption: TTntLabel
          Left = 327
          Top = 26
          Width = 70
          Height = 14
          Caption = 'Line separator'
        end
        object Sheet_Quote: TTntComboBox
          Left = 190
          Top = 0
          Width = 61
          Height = 22
          ItemHeight = 14
          TabOrder = 0
          Text = '"'
          Items.Strings = (
            '"')
        end
        object Sheet_Separator_Col: TTntComboBox
          Left = 190
          Top = 26
          Width = 61
          Height = 22
          ItemHeight = 14
          ItemIndex = 0
          TabOrder = 1
          Text = ';'
          Items.Strings = (
            ';'
            ','
            '(Tab)')
        end
        object Sheet_Separator_Line: TTntComboBox
          Left = 517
          Top = 17
          Width = 61
          Height = 22
          ItemHeight = 14
          TabOrder = 2
          Text = '\r\n'
          Items.Strings = (
            '(Default)'
            '\r\n'
            '\n')
        end
      end
      object Export_Text: TTntTabSheet
        Caption = 'Text'
        ImageIndex = 1
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Text_Advanced: TTntCheckBox
          Left = 0
          Top = 9
          Width = 208
          Height = 18
          Caption = 'Advanced : full information'
          TabOrder = 0
        end
      end
      object Export_HTML: TTntTabSheet
        Caption = 'HTML'
        ImageIndex = 2
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object HTML_Advanced: TTntCheckBox
          Left = 0
          Top = 9
          Width = 208
          Height = 18
          Caption = 'Advanced : full information'
          TabOrder = 0
        end
      end
      object Export_Custom: TTntTabSheet
        Caption = 'Custom'
        ImageIndex = 3
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object Custom_One: TTntCheckBox
          Left = 0
          Top = 9
          Width = 208
          Height = 18
          Caption = 'One output file per input file'
          TabOrder = 0
        end
      end
    end
  end
  object Name_Choose: TTntGroupBox
    Left = 1
    Top = 248
    Width = 621
    Height = 70
    Caption = 'Choose the desired filename'
    TabOrder = 3
    object Name: TTntEdit
      Left = 9
      Top = 22
      Width = 576
      Height = 22
      TabOrder = 0
    end
    object Name_FileSelect: TTntButton
      Left = 586
      Top = 22
      Width = 27
      Height = 22
      Caption = '...'
      TabOrder = 1
      OnClick = Name_FileSelectClick
    end
    object File_Append: TTntCheckBox
      Left = 9
      Top = 43
      Width = 600
      Height = 18
      Caption = 'Append to the existing file (Warning : parameters must be same)'
      TabOrder = 2
    end
  end
  object SaveDialog1: TTntSaveDialog
    Left = 8
    Top = 320
  end
end
