object Form1: TForm1
  Left = 190
  Top = 103
  Width = 316
  Height = 201
  BorderStyle = bsSizeToolWin
  Caption = 'Proceso hijo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  DesignSize = (
    308
    174)
  PixelsPerInch = 96
  TextHeight = 13
  object TLabel
    Left = 6
    Top = 8
    Width = 107
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Proceso hijo'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clTeal
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object TLabel
    Left = 8
    Top = 42
    Width = 105
    Height = 28
    Alignment = taCenter
    AutoSize = False
    Caption = 'Archivos proyectados en memoria'
    Transparent = True
    WordWrap = True
  end
  object Bevel1: TBevel
    Left = 0
    Top = 31
    Width = 115
    Height = 5
    Shape = bsTopLine
  end
  object l_jm: TLabel
    Left = 16
    Top = 89
    Width = 14
    Height = 13
    Cursor = crHandPoint
    AutoSize = False
    Caption = 'JM'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Transparent = True
    OnClick = l_jmClick
  end
  object TLabel
    Left = 34
    Top = 72
    Width = 53
    Height = 13
    AutoSize = False
    Caption = 'S'#237'ntesis 13'
    Transparent = True
  end
  object TLabel
    Left = 40
    Top = 89
    Width = 65
    Height = 13
    AutoSize = False
    Caption = 'Febrero/2003'
    Transparent = True
  end
  object Memo1: TMemo
    Left = 116
    Top = 6
    Width = 189
    Height = 168
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clBtnFace
    Lines.Strings = (
      'Memo1')
    ReadOnly = True
    TabOrder = 0
  end
  object Timer1: TTimer
    Interval = 200
    OnTimer = Timer1Timer
    Left = 224
    Top = 8
  end
end
