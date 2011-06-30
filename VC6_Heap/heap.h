//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Archivo: Heap.h
//
// Prop�sito:
//    Define las clases necesarias para dar una definici�n orientada a objetos del sistema de
//    gesti�n de montones del API Win32.
//
//    Las clases definidas son:
//        CHeapBlock:     representa un bloque reservado con HeapAlloc.
//        CHeap:          representa un mont�n creado con HeapCreate. Se compone de uno o 
//                        varios bloques de tipo CHeapBlock.
//        CHeapList:      representa una lista de punteros almacenada en un mont�n dedicado.
//        CHeapBlockList: especializaci�n de CHeapList que maneja objetos de tipo CHeapBlock
//                        en vez de punteros gen�ricos.
//
// Autor:          Jos� Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/11/2002: Creaci�n
//                 15/11/2002: Se incluyen los s�mbolos IN, OUT y OPTIONAL en los par�metros
//                             Se cambia los tipos de datos int por INT y void por VOID
//
// Observaciones:  Clases creadas en Visual C++ 6 para el art�culo "Los montones en Win32"
//                 publicado en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
// Copyright:      Este c�digo es de dominio p�blico y se puede utilizar y/o mejorar siempre 
//                 que SE HAGA REFERENCIA AL AUTOR ORIGINAL, ya sea a trav�s de estos 
//                 comentarios o de cualquier otro modo.
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

#include <windows.h>


//
// s�mbolo condicional HEAP_EXPORTS indica que las funciones se exportar�n.
// Este s�mbolo se define en "Heap.cpp", as� que s�lo se exportar�n las funciones cuando se
// incluya su implementaci�n. En caso contrario, se importar�n.
//
#ifdef HEAP_EXPORTS
  #define    HEAP_API    __declspec(dllexport)
#else
  #define    HEAP_API    __declspec(dllimport)
#endif


#define INITIAL_SIZE    1024 * 64


class CHeap;

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Clase: CHeapBlock
//
// Descripci�n: representa un bloque de memoria reservada dentro de un mont�n.
//
// Uso: Se puede crear este objeto asociado a un mont�n dado (a trav�s del constructor
//      sobrecargado) o bien llamando al m�todo Allocate de la clase CHeap.
//
// Modificaciones:
//     JM. 01/11/2002. Versi�n inicial para el art�culo "Los montones en Win32" publicado
//                     en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
class HEAP_API CHeapBlock
{
private:
    CHeap     *m_heap;
    DWORD     m_size;
    LPVOID    m_memory;

public:
    CHeapBlock( 
        IN       CHeap  *a_heap, 
        IN CONST DWORD  a_size );

    virtual ~CHeapBlock( VOID );

    VOID SetSize( 
        IN CONST DWORD size );

    INT GetSize( VOID ) const;

    LPVOID GetMemory( VOID ) const;
};



class CHeapBlockList;



//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Clase: CHeap
//
// Descripci�n: representa un mont�n dentro de Win32.
//
// Uso: al crear un objeto de este tipo, se crea un mont�n dedicado a trav�s de la funci�n
//      HeapCreate. Para almacenar datos en �l, se debe llamar a la funci�n Allocate, la
//      cual devuelve un objeto CHeapBlock (que representa el bloque de memoria reservada).
//      Se pueden almacenar datos en el bloque a trav�s del m�todo CHeapBlock.GetMemory()
//
// Modificaciones:
//     JM. 01/11/2002. Versi�n inicial para el art�culo "Los montones en Win32" publicado
//                     en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
class HEAP_API CHeap
{
private:
    HANDLE          m_handle;
    CHeapBlockList  *m_blocks;
    DWORD           m_serialize;
    BOOL            m_autoExtend;

public:
    // crea un nuevo mont�n indicando sus datos
    CHeap( 
        IN OPTIONAL CONST DWORD dwInitialSize = INITIAL_SIZE,
        IN OPTIONAL CONST DWORD flOptions     = HEAP_NO_SERIALIZE,
        IN OPTIONAL CONST DWORD dwMaximumSize = 0 );

    virtual ~CHeap( VOID );

    CHeapBlock *GetBlock(
        IN CONST INT i ) const;

    CHeapBlockList *GetBlocks( VOID ) const;

    CHeapBlock *Allocate( 
        IN CONST DWORD size );

    DWORD Compact( VOID );

    BOOL GetSerialize( VOID ) const;

    VOID SetSerialize(
        IN CONST BOOL value );

    HANDLE GetHandle( VOID ) const;

    BOOL GetAutoExtend( VOID ) const;
};



//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Clase: CHeapList
//
// Descripci�n: representa una lista de punteros (igual que la clase CList), que almacena
//              sus datos en un mont�n dedicado.
//
// Uso: el mismo que la clase TList: creando el objeto a trav�s de un constructor (indicando
//      el mont�n a utilizar o bien las caracter�sticas del mont�n a crear), y a�adiendo
//      elementos a trav�s de los m�todos Add(Pointer) e Insert(Pointer).
//      Si los elementos apuntan a registros creados en tiempo de ejecuci�n, es recomendable
//      que la memoria de estos sea reservada dentro del mismo mont�n, a trav�s del m�todo
//      CHeapList.GetHeap.Allocate(tama�o)
//
// Modificaciones:
//     JM. 01/11/2002. Versi�n inicial para el art�culo "Los montones en Win32" publicado
//                     en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class HEAP_API CHeapList
{
private:
    static const INT MAX_LIST_SIZE; // constante com�n a todos los objetos de esta clase.
                                    // despu�s de la declaraci�n de la clase.

    CHeap        *m_heap;    // el mont�n que se utiliza para almacenar los elementos
    CHeapBlock   *m_block;   // el bloque de memoria dentro del mont�n, donde reside FList

    BOOL    m_destroyHeap;   // indica si debe destruirse el mont�n

    LPVOID    *m_list;       // el puntero al array de punteros
    INT        m_count;      // el n� actual de elementos
    INT        m_capacity;   // del n� de elementos para el que se ha reservado memoria

protected:
    // aplica el algoritmo de crecimiento
    virtual VOID Grow( VOID );

public:
    // constructor indicando los datos del nuevo mont�n
    CHeapList( 
        IN OPTIONAL CONST DWORD dwInitialSize = INITIAL_SIZE,
        IN OPTIONAL CONST DWORD flOptions     = HEAP_NO_SERIALIZE,
        IN OPTIONAL CONST DWORD dwMaximumSize = 0 );
    // constructor indicando el mont�n a utilizar
    CHeapList(
        IN CHeap *a_heap );

    virtual ~CHeapList( VOID );

    // a�adir elementos
    INT Add(
        IN CONST LPVOID item );

    VOID Insert(
        IN CONST INT    index, 
        IN CONST LPVOID item );

    // eliminar elementos
    virtual VOID Clear( VOID );
    VOID Delete( 
        IN CONST INT index );

    LPVOID GetItem(
        IN CONST INT index ) const;
    
    VOID SetItem(
        IN CONST INT    index, 
        IN CONST LPVOID item );

    // buscar elementos
    INT GetIndex( 
        IN CONST LPVOID item ) const;

    LPVOID *GetList( VOID ) const;

    CHeap  *GetHeap( VOID ) const;

    // reserva nueva memoria para los elementos
    VOID SetCapacity( 
        IN CONST INT NewCapacity );

    INT GetCapacity( VOID ) const;

    // establece el nuevo n� de elementos, y hace crecer la lista si es necesario
    VOID SetCount( 
        IN CONST INT NewCount );

    INT GetCount( VOID ) const;
};

// as� se inicializa una variable miembro constante est�tica
const INT CHeapList::MAX_LIST_SIZE = 134217727; 

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Clase: CHeapBlockList
//
// Descripci�n: representa una lista de objetos de tipo CHeapBlock. Adem�s, esta lista
//              hereda de CHeapList, por lo que los datos los almacena en un mont�n din�mico
//              (creado por la propia CHeapList o bien pasado en el constructor.
//
// Uso: igual que la clase CHeapList, excepto que los objetos que maneja son de tipo
//      CHeapBlock en vez de LPVOID.
//
// Modificaciones:
//     JM. 01/11/2002. Versi�n inicial para el art�culo "Los montones en Win32" publicado
//                     en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
class HEAP_API CHeapBlockList: public CHeapList
{
public:
    CHeapBlockList( 
        IN CHeap *a_heap );

    CHeapBlock *GetBlock( 
        IN CONST INT index );

    SetBlock( 
        IN CONST INT        index, 
        IN       CHeapBlock *item );
};