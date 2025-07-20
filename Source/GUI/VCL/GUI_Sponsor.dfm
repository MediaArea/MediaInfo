object SponsorFrame: TSponsorFrame
  Left = 0
  Top = 0
  Width = 690
  Height = 200
  TabOrder = 0
  OnResize = FormResize
  DesignSize = (
    690
    200)
  object BannerImage: TImage
    Left = 0
    Top = 0
    Width = 673
    Height = 197
    Anchors = [akLeft, akTop, akRight, akBottom]
    Center = True
    Proportional = True
    Stretch = True
    OnClick = BannerImageClick
  end
  object CloseButton: TButton
    Left = 670
    Top = 0
    Width = 18
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'X'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = CloseButtonClick
  end
end
