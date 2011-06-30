//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Archivo: Heap.h
//
// Propósito:
//    Define las clases necesarias para dar una definición orientada a objetos del sistema de
//    gestión de montones del API Win32.
//
//    Las clases definidas son:
//        CHeapBlock:     representa un bloque reservado con HeapAlloc.
//        CHeap:          representa un montón creado con HeapCreate. Se compone de uno o 
//                        varios bloques de tipo CHeapBlock.
//        CHeapList:      representa una lista de punteros almacenada en un montón dedicado.
//        CHeapBlockList: especialización de CHeapList que maneja objetos de tipo CHeapBlock
//                        en vez de punteros genéricos.
//
// Autor:          José Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/11/2002: Creación
//                 15/11/2002: Se incluyen los símbolos IN, OUT y OPTIONAL en los parámetros
//                             Se cambia los tipos de datos int por INT y void por VOID
//
// Observaciones:  Clases creadas en Visual C++ 6 para el artículo "Los montones en Win32"
//                 publicado en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
// Copyright:      Este código es de dominio público y se puede utilizar y/o mejorar siempre 
//                 que SE HAGA REFERENCIA AL AUTOR ORIGINAL, ya sea a través de estos 
//                 comentarios o de cualquier otro modo.
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

#include <windows.h>


//
// símbolo condicional HEAP_EXPORTS indica que las funciones se exportarán.
// Este símbolo se define en "Heap.cpp", así que sólo se exportarán las funciones cuando se
// incluya su implementación. En caso contrario, se importarán.
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
// Descripción: representa un bloque de memoria reservada dentro de un montón.
//
// Uso: Se puede crear este objeto asociado a un montón dado (a través del constructor
//      sobrecargado) o bien llamando al método Allocate de la clase CHeap.
//
// Modificaciones:
//     JM. 01/11/2002. Versión inicial para el artículo "Los montones en Win32" publicado
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
// Descripción: representa un montón dentro de Win32.
//
// Uso: al crear un objeto de este tipo, se crea un montón dedicado a través de la función
//      HeapCreate. Para almacenar datos en él, se debe llamar a la función Allocate, la
//      cual devuelve un objeto CHeapBlock (que representa el bloque de memoria reservada).
//      Se pueden almacenar datos en el bloque a través del método CHeapBlock.GetMemory()
//
// Modificaciones:
//     JM. 01/11/2002. Versión inicial para el artículo "Los montones en Win32" publicado
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
    // crea un nuevo montón indicando sus datos
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
// Descripción: representa una lista de punteros (igual que la clase CList), que almacena
//              sus datos en un montón dedicado.
//
// Uso: el mismo que la clase TList: creando el objeto a través de un constructor (indicando
//      el montón a utilizar o bien las características del montón a crear), y añadiendo
//      elementos a través de los métodos Add(Pointer) e Insert(Pointer).
//      Si los elementos apuntan a registros creados en tiempo de ejecución, es recomendable
//      que la memoria de estos sea reservada dentro del mismo montón, a través del método
//      CHeapList.GetHeap.Allocate(tamaño)
//
// Modificaciones:
//     JM. 01/11/2002. Versión inicial para el artículo "Los montones en Win32" publicado
//                     en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class HEAP_API CHeapList
{
private:
    static const INT MAX_LIST_SIZE; // constante común a todos los objetos de esta clase.
                                    // después de la declaración de la clase.

    CHeap        *m_heap;    // el montón que se utiliza para almacenar los elementos
    CHeapBlock   *m_block;   // el bloque de memoria dentro del montón, donde reside FList

    BOOL    m_destroyHeap;   // indica si debe destruirse el montón

    LPVOID    *m_list;       // el puntero al array de punteros
    INT        m_count;      // el nº actual de elementos
    INT        m_capacity;   // del nº de elementos para el que se ha reservado memoria

protected:
    // aplica el algoritmo de crecimiento
    virtual VOID Grow( VOID );

public:
    // constructor indicando los datos del nuevo montón
    CHeapList( 
        IN OPTIONAL CONST DWORD dwInitialSize = INITIAL_SIZE,
        IN OPTIONAL CONST DWORD flOptions     = HEAP_NO_SERIALIZE,
        IN OPTIONAL CONST DWORD dwMaximumSize = 0 );
    // constructor indicando el montón a utilizar
    CHeapList(
        IN CHeap *a_heap );

    virtual ~CHeapList( VOID );

    // añadir elementos
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

    // establece el nuevo nº de elementos, y hace crecer la lista si es necesario
    VOID SetCount( 
        IN CONST INT NewCount );

    INT GetCount( VOID ) const;
};

// así se inicializa una variable miembro constante estática
const INT CHeapList::MAX_LIST_SIZE = 134217727; 

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Clase: CHeapBlockList
//
// Descripción: representa una lista de objetos de tipo CHeapBlock. Además, esta lista
//              hereda de CHeapList, por lo que los datos los almacena en un montón dinámico
//              (creado por la propia CHeapList o bien pasado en el constructor.
//
// Uso: igual que la clase CHeapList, excepto que los objetos que maneja son de tipo
//      CHeapBlock en vez de LPVOID.
//
// Modificaciones:
//     JM. 01/11/2002. Versión inicial para el artículo "Los montones en Win32" publicado
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