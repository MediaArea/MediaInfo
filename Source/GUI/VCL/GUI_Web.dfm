object WebF: TWebF
  Left = 192
  Top = 114
  AlphaBlend = True
  AlphaBlendValue = 0
  Caption = 'Verifying version...'
  ClientHeight = 57
  ClientWidth = 226
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Position = poDesktopCenter
  WindowState = wsMinimized
  TextHeight = 13
  object Browser: TWebBrowser
    Left = 8
    Top = 8
    Width = 210
    Height = 41
    TabOrder = 0
    OnNavigateComplete2 = BrowserNavigateComplete2
    ControlData = {
      4C000000B41500003D0400000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E126208000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
end
