object PluginF: TPluginF
  Left = 290
  Top = 286
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Install Plugin'
  ClientHeight = 146
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Position = poOwnerFormCenter
  OnShow = FormShow
  TextHeight = 14
  object InfoLabel: TLabel
    Left = 5
    Top = 5
    Width = 340
    Height = 15
    AutoSize = False
    Caption = 'You need the following plugin to use this feature:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object NameLabel: TLabel
    Left = 5
    Top = 20
    Width = 340
    Height = 15
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object AskLabel: TLabel
    Left = 5
    Top = 40
    Width = 340
    Height = 15
    AutoSize = False
    Caption = 'Would you like to download and install this plugin?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object DownloadLabel: TLabel
    Left = 5
    Top = 60
    Width = 340
    Height = 15
    AutoSize = False
    Caption = 'Downloading installer...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    Visible = False
  end
  object InstallLabel: TLabel
    Left = 5
    Top = 75
    Width = 340
    Height = 15
    AutoSize = False
    Caption = 'Running installer...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    Visible = False
  end
  object Cancel: TButton
    Left = 62
    Top = 112
    Width = 122
    Height = 26
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = CancelClick
  end
  object Install: TButton
    Left = 190
    Top = 112
    Width = 122
    Height = 26
    Caption = 'Install'
    Default = True
    TabOrder = 0
    OnClick = InstallClick
  end
end
