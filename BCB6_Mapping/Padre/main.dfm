object Form1: TForm1
  Left = 192
  Top = 107
  Width = 403
  Height = 278
  Caption = 'Proceso padre'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  DesignSize = (
    395
    251)
  PixelsPerInch = 96
  TextHeight = 13
  object TLabel
    Left = 6
    Top = 8
    Width = 107
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Proceso padre'
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
  object Label1: TLabel
    Left = 122
    Top = 178
    Width = 58
    Height = 13
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Proceso hijo'
  end
  object Bevel2: TBevel
    Left = 188
    Top = 186
    Width = 205
    Height = 5
    Anchors = [akLeft, akRight, akBottom]
    Shape = bsTopLine
  end
  object Memo1: TMemo
    Left = 122
    Top = 2
    Width = 271
    Height = 171
    Anchors = [akLeft, akTop, akRight, akBottom]
    Lines.Strings = (
      'Este es el texto que se va a compartir entre la '
      'aplicaci'#243'n padre e hija')
    TabOrder = 0
    OnChange = Memo1Change
  end
  object lv_procesos: TListView
    Left = 122
    Top = 198
    Width = 271
    Height = 51
    Anchors = [akLeft, akRight, akBottom]
    Columns = <
      item
        Caption = '#'
        Width = 30
      end
      item
        Caption = 'ProcessHandle'
        Width = 100
      end
      item
        Caption = 'ProcessId'
        Width = 100
      end>
    ColumnClick = False
    FullDrag = True
    RowSelect = True
    PopupMenu = PopupMenu1
    TabOrder = 1
    ViewStyle = vsReport
  end
  object PopupMenu1: TPopupMenu
    Left = 32
    Top = 136
    object m_TerminarProceso: TMenuItem
      Caption = 'Terminar proceso'
      OnClick = m_TerminarProcesoClick
    end
  end
end
