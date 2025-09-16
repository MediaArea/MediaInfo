object SponsorFrame: TSponsorFrame
  Left = 0
  Top = 0
  Width = 690
  Height = 200
  Align = alBottom
  TabOrder = 0
  OnResize = OnResize
  object BannerImage: TImage
    Left = 0
    Top = 0
    Width = 672
    Height = 200
    Align = alClient
    Center = True
    Proportional = True
    Stretch = True
    OnClick = BannerImageClick
    OnMouseEnter = BannerImageMouseInter
    OnMouseLeave = BannerImageMouseLeave
  end
  object RightPanel: TPanel
    Left = 672
    Top = 0
    Width = 18
    Height = 200
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      18
      200)
    object CloseButton: TButton
      Left = 0
      Top = 0
      Width = 18
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'X'
      TabOrder = 0
      OnClick = CloseButtonClick
    end
  end
end
