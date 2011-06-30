//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Unidad: Heap.pas
//
// Prop�sito:
//    Crea las clases necesarias para dar una definici�n orientada a objetos del sistema de
//    gesti�n de montones del API Win32.
//
//    Las clases inclu�das son:
//        THeapBlock: representa un bloque reservado con HeapAlloc.
//        THeap:      representa un mont�n creado con HeapCreate. Se compone de uno o varios
//                    bloques de tipo THeapBlock.
//
// Autor:          Jos� Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/12/2002
//
// Observaciones:  Unidad creada en Delphi 5 para el art�culo "Los montones en Win32" publicado
//                 en http://users.servicios.retecal.es/sapivi/prog/win32
//
// Copyright:      Este c�digo es de dominio p�blico y se puede utilizar y/o mejorar siempre que
//                 SE HAGA REFERENCIA AL AUTOR ORIGINAL, ya sea a trav�s de estos comentarios
//                 o de cualquier otro modo.
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

unit Heap;

interface

uses classes, Windows;

const
  // constantes de API (definidas en winnt.h)
  HEAP_NO_SERIALIZE          = $00000001;
  HEAP_GENERATE_EXCEPTIONS   = $00000004;
  HEAP_ZERO_MEMORY           = $00000008;
  HEAP_REALLOC_IN_PLACE_ONLY = $00000010;

  // constantes propias
  INITIAL_SIZE = 1024 * 64; // 64 KB

type


  THeap = class;


  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  //
  // Clase: THeapBlock
  //
  // Descripci�n: representa un bloque de memoria reservada dentro de un mont�n.
  //
  // Uso: Se puede crear este objeto asociado a un mont�n dado (a trav�s del constructor
  //      sobrecargado) o bien llamando al m�todo Allocate de la clase THeap.
  //
  // Modificaciones:
  //     01/12/2002 - JM - Versi�n inicial para S�ntesis n� 12 (http://www.grupoalbor.com)
  //
  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  THeapBlock = class(TObject)
  private
    FHeap: THeap;

    FSize:   integer;
    FMemory: Pointer;

    procedure SetSize(const ASize: integer);

  public
    constructor Create(const AHeap: THeap;
                       const ASize: integer); reintroduce; overload;
    destructor  Destroy; override;

    property Size:   integer read FSize write SetSize;
    property Memory: Pointer read FMemory;
  end;


  THeapBlockList = class;


  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  //
  // Clase: THeap
  //
  // Descripci�n: representa un mont�n dentro de Win32.
  //
  // Uso: al crear un objeto de este tipo, se crea un mont�n dedicado a trav�s de la funci�n
  //      HeapCreate. Para almacenar datos en �l, se debe llamar a la funci�n Allocate, la
  //      cual devuelve un objeto THeapBlock (que representa el bloque de memoria reservada).
  //      Se pueden almacenar datos en el bloque a trav�s de la propiedad THeapBlock.Memory
  //
  // Modificaciones:
  //     01/12/2002 - JM - Versi�n inicial para S�ntesis n� 12 (http://www.grupoalbor.com)
  //
  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  THeap = class(TObject)
  private
    FHandle:      THandle;
    FBlocks:      THeapBlockList;
    FSerialize:   LongWord;
    FAutoExtend:  boolean;

    function GetSerialize: boolean;
    procedure SetSerialize(const value: boolean);

    function GetBlock(const i: integer): THeapBlock;

  public
    // crea un nuevo mont�n indicando sus datos
    constructor Create(const dwInitialSize:  LongWord = INITIAL_SIZE;
                       const flOptions:      LongWord = HEAP_NO_SERIALIZE;
                       const dwMaximumSize:  LongWord = 0); reintroduce; overload;

    destructor Destroy; override;

    function Allocate(const size: integer): THeapBlock;
    function Compact: LongWord;

    property Handle:     THandle read FHandle;
    property Serialize:  boolean read GetSerialize write SetSerialize;
    property AutoExtend: boolean read FAutoExtend;

    property Block[const i: integer]: THeapBlock read GetBlock;
    property Blocks: THeapBlockList              read FBlocks;
  end;



  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  //
  // Clase: THeapList
  //
  // Descripci�n: representa una lista de punteros (igual que la clase TList), que almacena
  //              sus datos en un mont�n dedicado.
  //
  // Uso: el mismo que la clase TList: creando el objeto a trav�s de un constructor (indicando
  //      el mont�n a utilizar o bien las caracter�sticas del mont�n a crear), y a�adiendo
  //      elementos a trav�s de los m�todos Add(Pointer) e Insert(Pointer).
  //      Si los elementos apuntan a registros creados en tiempo de ejecuci�n, es recomendable
  //      que la memoria de estos sea reservada dentro del mismo mont�n, a trav�s del m�todo
  //      THeapList.Heap.Allocate(tama�o)
  //
  // Modificaciones:
  //     01/12/2002 - JM - Versi�n inicial para S�ntesis n� 12 (http://www.grupoalbor.com)
  //
  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  THeapList = class(TObject)
  private
    FHeap:     THeap;         // el mont�n que se utiliza para almacenar los elementos
    FBlock:    THeapBlock;    // el bloque de memoria dentro del mont�n, donde reside FList

    FDestroyHeap: boolean;    // indica si debe destruirse el mont�n

    FList:     PPointerList;  // el puntero al array de punteros
    FCount:    integer;       // el n� actual de elementos
    FCapacity: integer;       // del n� de elementos para el que se ha reservado memoria

  protected
    function  Get(index: integer): Pointer;
    procedure Put(index: integer; item: Pointer);

    // aplica el algoritmo de crecimiento
    procedure Grow; virtual;

    // reserva nueva memoria para los elementos
    procedure SetCapacity(NewCapacity: integer);

    // establece el nuevo n� de elementos, y hace crecer la lista si es necesario
    procedure SetCount(NewCount: integer);

  public
    // constructor indicando los datos del nuevo mont�n
    constructor Create(const dwInitialSize:  LongWord = INITIAL_SIZE;
                       const flOptions:      LongWord = HEAP_NO_SERIALIZE;
                       const dwMaximumSize:  LongWord = 0); reintroduce; overload;
    // constructor indicando el mont�n a utilizar
    constructor Create(AHeap: THeap); reintroduce; overload;

    destructor Destroy; override;

    // a�adir elementos
    function Add(item: Pointer): integer;
    procedure Insert(index: integer; item: Pointer);

    // eliminar elementos
    procedure Clear; virtual;
    procedure Delete(index: integer);

    // buscar elementos
    function IndexOf(item: Pointer): integer;

    property Capacity: integer              read FCapacity write SetCapacity;
    property Count: integer                 read FCount    write SetCount;
    property Items[index: integer]: Pointer read Get       write Put; default;
    property List: PPointerList             read FList;
    property Heap: THeap                    read FHeap;
  end;



  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  //
  // Clase: THeapBlockList
  //
  // Descripci�n: representa una lista de objeto de tipo THeapBlock. Adem�s, esta lista
  //              hereda de THeapList, por lo que los datos los almacena en un mont�n din�mico
  //              (creado por la propia THeapList o bien pasado en el constructor.
  //
  // Uso: igual que la clase THeapList, excepto que los objetos que maneja son de tipo
  //      THeapBlock en vez de Pointer.
  //
  // Modificaciones:
  //     01/12/2002 - JM - Versi�n inicial para S�ntesis n� 12 (http://www.grupoalbor.com)
  //
  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  THeapBlockList = class(THeapList)
  private
    function  GetBlock(const index: integer): THeapBlock;
    procedure PutBlock(const index: integer; const value: THeapBlock);

  public
    property Items[const index: integer]: THeapBlock read GetBlock write PutBlock; default;

  end;



implementation


uses SysUtils;


resourcestring
  SIndiceIncorrecto = 'Error en THeapList:'#13'El �ndice %i no es v�lido.';



//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Clase THeapBlock
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
constructor THeapBlock.Create(const AHeap: THeap; const ASize: integer);
begin
  inherited Create;

  FMemory := nil;
  FHeap   := AHeap;

  SetSize(ASize);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
destructor THeapBlock.Destroy;
var
  ind: integer;
begin
  // se libera el bloque de memoria
  HeapFree(FHeap.handle, FHeap.FSerialize, FMemory);

  // elimina el bloque de la lista.
  ind := FHeap.FBlocks.IndexOf(self);
  if (ind > -1)  then
    FHeap.FBlocks.Delete(ind);

  inherited;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapBlock.SetSize(const ASize: integer);
begin
  if (ASize <> FSize) and (ASize > 0) then
  begin

    // asigna un bloque (la primera vez) o lo re-asigna
    if FMemory = nil then
      FMemory := HeapAlloc(
                      FHeap.handle,
                      FHeap.FSerialize or HEAP_ZERO_MEMORY,
                      ASize)
    else
      FMemory := HeapReAlloc(
                      FHeap.handle,
                      FHeap.FSerialize or HEAP_ZERO_MEMORY or HEAP_REALLOC_IN_PLACE_ONLY,
                      FMemory,
                      ASize);

    if FMemory = nil then
      RaiseLastWin32Error;

    FSize := HeapSize(FHeap.handle, FHeap.FSerialize, FMemory);

  end;
end;



//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Clase THeap
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
constructor THeap.Create(const dwInitialSize:  LongWord;
                         const flOptions:      LongWord;
                         const dwMaximumSize:  LongWord);
begin
  inherited Create;

  // se crea el Heap asociado
  FHandle := HeapCreate(flOptions, dwInitialSize, dwMaximumSize);
  if FHandle = 0 then
    RaiseLastWin32Error;

  FSerialize := flOptions and HEAP_NO_SERIALIZE;

  // se crea la lista de bloques asociados (se llenar� desde Allocate)
  FBlocks := THeapBlockList.Create(self);

  // si dwMaximumSize = 0, es un mont�n auto-extensible
  FAutoExtend := (dwMaximumSize = 0);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
destructor THeap.Destroy;
var
  i: integer;
begin
  // se liberan los bloques creados
  for i := FBlocks.count - 1 downto 0 do
    TObject(FBlocks[i]).Free;

  FBlocks.Free;

  // se destruye el Heap del API
  if not HeapDestroy(FHandle) then
    RaiseLastWin32Error;

  inherited;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeap.GetSerialize: boolean;
begin
  result := (FSerialize = 0);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeap.SetSerialize(const value: boolean);
begin
  if value <> Serialize then
  begin
    if value then
      FSerialize := 0
    else
      FSerialize := HEAP_NO_SERIALIZE;
  end;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeap.GetBlock(const i: integer): THeapBlock;
begin
  result := FBlocks[i];
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeap.Allocate(const size: integer): THeapBlock;
begin
  if not FAutoExtend or (FAutoExtend and (size < $7FFF8)) then
  begin
    result := THeapBlock.Create(self, size);
    Self.FBlocks.Add(result);
  end
  else
    result := nil;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeap.Compact: LongWord;
begin
  result := HeapCompact(FHandle, FSerialize);
  if result = 0 then
    RaiseLastWin32Error;
end;



//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Clase THeapList
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
constructor THeapList.Create(const dwInitialSize:  LongWord;
                             const flOptions:      LongWord;
                             const dwMaximumSize:  LongWord);
begin
  inherited Create;

  FHeap        := THeap.Create(flOptions, dwInitialSize, dwMaximumSize);
  FDestroyHeap := true;

  FBlock := nil;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
constructor THeapList.Create(AHeap: THeap);
begin
  inherited Create;

  FHeap        := AHeap;
  FDestroyHeap := false;

  FBlock := nil;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
destructor THeapList.Destroy;
begin
  // se elimina el mont�n, y con ello toda la memoria reservada
  if FDestroyHeap then
    FHeap.Free;

  inherited;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeapList.Get(index: integer): Pointer;
begin
  if (index < 0) or (index >= FCount) then
    TList.Error(@SIndiceIncorrecto, index);

  result := FList^[index];
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.Put(index: integer; item: Pointer);
begin
  if (index < 0) or (index >= FCount) then
    TList.Error(@SIndiceIncorrecto, index);

  FList^[index] := item;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.Grow;
var
  Delta: integer;
begin
  // esto define el algoritmo de crecimiento:
  //    Si la lista tiene m�s de 64 elementos, crece un 25%
  //    Si la lista tiene entre 9 y 63, crece 16 elementos
  //    Si la lista tiene menos de 9, crece 4 elementos.
  if FCapacity > 64 then
    Delta := FCapacity div 4
  else
    if FCapacity > 8 then
      Delta := 16
    else
      Delta := 4;

  SetCapacity(FCapacity + Delta);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.SetCapacity(NewCapacity: integer);
begin
  if (NewCapacity < FCount) or (NewCapacity > MaxListSize) then
    TList.Error(@SIndiceIncorrecto, NewCapacity);

  if NewCapacity <> FCapacity then
  begin
    FCapacity := NewCapacity;

    if FBlock = nil then
      FBlock := THeapBlock.Create(FHeap, NewCapacity * sizeof(Pointer))
    else
      FBlock.Size := NewCapacity * sizeof(Pointer);

    // la lista el el bloque de memoria que acabo de reservar
    FList := FBlock.FMemory;
  end;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.SetCount(NewCount: integer);
var
  I: integer;
begin
  if (NewCount < 0) or (NewCount > MaxListSize) then
    TList.Error(@SIndiceIncorrecto, NewCount);

  if NewCount > FCapacity then
    SetCapacity(NewCount);

  if NewCount > FCount then
    FillChar(FList^[FCount], (NewCount - FCount) * SizeOf(Pointer), 0)
  else
    for I := FCount - 1 downto NewCount do
      Delete(I);
  FCount := NewCount;
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeapList.Add(item: Pointer): integer;
begin
  result := FCount;

  // si ha llegado al l�mite, extendemos la lista
  if result = FCapacity then
    Grow;

  FList^[result] := item;
  Inc(FCount);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.Insert(index: integer; item: Pointer);
begin
  if (index < 0) or (index > FCount) then
    TList.Error(@SIndiceIncorrecto, index);

  // si ha llegado al l�mite, extendemos la lista
  if FCount = FCapacity then
    Grow;

  // mueve lo que haya por detr�s, para dejar un espacio libre
  if index < FCount then
    System.Move(FList^[index], FList^[index + 1], (FCount - index) * SizeOf(Pointer));

  FList^[index] := item;
  Inc(FCount);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.Clear;
begin
  SetCount(0);
  SetCapacity(0);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapList.Delete(index: integer);
begin
  if (index < 0) or (index >= FCount) then
    TList.Error(@SIndiceIncorrecto, index);

  Dec(FCount);

  // se mueven los que estaban por detr�s para "tapar el hueco".
  if index < FCount then
    System.Move(FList^[index + 1], FList^[index], (FCount - index) * SizeOf(Pointer));
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeapList.IndexOf(item: Pointer): integer;
begin
  result := 0;

  while (result < FCount) and (FList^[result] <> item) do
    Inc(result);

  if result = FCount then
    result := -1;
end;



//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Clase THeapBlockList
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
function THeapBlockList.GetBlock(const index: integer): THeapBlock;
begin
  result := THeapBlock(inherited items[index]);
end;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
procedure THeapBlockList.PutBlock(const index: integer; const value: THeapBlock);
begin
  inherited items[index] := value;
end;


end.
