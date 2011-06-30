object MainForm: TMainForm
  Left = 188
  Top = 97
  Width = 432
  Height = 366
  Caption = 'Pruebas CHeapObject'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  DesignSize = (
    424
    339)
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel2: TBevel
    Left = 128
    Top = -6
    Width = 289
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Shape = bsBottomLine
  end
  object Bevel1: TBevel
    Left = 8
    Top = 118
    Width = 409
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Shape = bsBottomLine
  end
  object i_icono: TImage
    Left = 27
    Top = 8
    Width = 32
    Height = 32
  end
  object bv_bottom: TBevel
    Left = 0
    Top = 286
    Width = 417
    Height = 17
    Anchors = [akLeft, akRight, akBottom]
    Shape = bsBottomLine
  end
  object TLabel
    Left = 10
    Top = 52
    Width = 66
    Height = 13
    Caption = 'Los montones'
  end
  object TLabel
    Left = 10
    Top = 69
    Width = 65
    Height = 13
    Caption = 'S'#237'ntesis n'#186' 12'
  end
  object TLabel
    Left = 5
    Top = 85
    Width = 97
    Height = 13
    Caption = 'JM - Diciembre/2002'
  end
  object l_jm: TLabel
    Left = 5
    Top = 85
    Width = 13
    Height = 13
    Cursor = crHandPoint
    Hint = 'mailto: jose_manuel_navarro@yahoo.es'
    Caption = 'JM'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    OnClick = l_jmClick
  end
  object TLabel
    Left = 5
    Top = 126
    Width = 83
    Height = 13
    Caption = 'Log de ejecuci'#243'n '
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object TLabel
    Left = 128
    Top = 2
    Width = 82
    Height = 13
    Caption = 'Objetos creados '
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object b_crear: TButton
    Left = 240
    Top = 310
    Width = 81
    Height = 23
    Anchors = [akRight, akBottom]
    Caption = 'Crear objeto'
    TabOrder = 0
    OnClick = CrearObjeto
  end
  object b_destruir: TButton
    Left = 328
    Top = 310
    Width = 89
    Height = 23
    Anchors = [akRight, akBottom]
    Caption = 'Destruir objeto'
    Enabled = False
    TabOrder = 1
    OnClick = DestruirObjeto
  end
  object lb_objetos: TListBox
    Left = 128
    Top = 18
    Width = 289
    Height = 108
    Anchors = [akLeft, akTop, akRight]
    IntegralHeight = True
    ItemHeight = 13
    TabOrder = 2
  end
  object lb_log: TListBox
    Left = 5
    Top = 144
    Width = 412
    Height = 151
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 13
    TabOrder = 3
  end
  object b_verMontones: TButton
    Left = 5
    Top = 310
    Width = 148
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Ver montones del proceso'
    TabOrder = 4
    OnClick = VerMontonesProceso
  end
end
