//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Archivo: Heap.cpp
//
// Propósito:
//    Crea las clases necesarias para dar una definición orientada a objetos del sistema de
//    gestión de montones del API Win32.
//
//    Las clases incluídas son:
//        CHeapBlock:     representa un bloque reservado con HeapAlloc.
//        CHeap:          representa un montón creado con HeapCreate. Se compone de uno o 
//                        varios bloques de tipo CHeapBlock.
//        CHeapList:      representa una lista de punteros almacenada en un montón dedicado.
//        CHeapBlockList: especialización de CHeapList que maneja objetos de tipo CHeapBlock
//                        en vez de punteros genéricos.
//
// Autor:          José Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/11/2002
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
#include "stdafx.h"

#define HEAP_EXPORTS

#include "Heap.h"

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
CHeapBlock::CHeapBlock( IN       CHeap *a_heap, 
                        IN CONST DWORD a_size )
{
    m_memory = NULL;
    m_heap   = a_heap;

    this->SetSize( a_size );
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapBlock::~CHeapBlock( VOID )
{
    INT ind;

    // se libera el bloque de memoria
    ::HeapFree( m_heap->GetHandle(), m_heap->GetSerialize(), m_memory );

    // elimina el bloque de la lista.
    ind = m_heap->GetBlocks()->GetIndex( this );
    if ( ind > -1 )
        m_heap->GetBlocks()->Delete( ind );
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void CHeapBlock::SetSize( IN CONST DWORD size )
{
    if ( size != m_size && size > 0 )
    {
        //
        // asigna un bloque (la primera vez) o lo re-asigna
        //
        if ( m_memory == NULL )
            m_memory = ::HeapAlloc(
                            m_heap->GetHandle(),
                            m_heap->GetSerialize() | HEAP_ZERO_MEMORY,
                            size );
        else
            m_memory = ::HeapReAlloc(
                            m_heap->GetHandle(),
                            m_heap->GetSerialize() | HEAP_ZERO_MEMORY | HEAP_REALLOC_IN_PLACE_ONLY,
                            m_memory,
                            size );

        if ( m_memory == NULL )
        {
            m_size = -1;
            return;
        }
        else
            m_size = ::HeapSize( m_heap->GetHandle(), m_heap->GetSerialize(), m_memory );
    }
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
int CHeapBlock::GetSize() const
{
    return m_size;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
LPVOID CHeapBlock::GetMemory() const
{
    return m_memory;
}




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
CHeap::CHeap( IN OPTIONAL CONST DWORD dwInitialSize, 
              IN OPTIONAL CONST DWORD flOptions, 
              IN OPTIONAL CONST DWORD dwMaximumSize )
{
    // se crea el Heap asociado
    m_handle = ::HeapCreate( flOptions, dwInitialSize, dwMaximumSize );
    if ( m_handle == 0 )
        return;

    m_serialize = flOptions && HEAP_NO_SERIALIZE;

    // se crea la lista de bloques asociados (se llenará desde Allocate)
    m_blocks = new CHeapBlockList( this );

    // si dwMaximumSize = 0, es un montón auto-extensible
    m_autoExtend = (dwMaximumSize == 0);
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeap::~CHeap( VOID )
{
    int     i;

    // se liberan los bloques creados
    for ( i = m_blocks->GetCount() - 1; i >= 0; i-- )
        delete m_blocks->GetItem(i);

    delete m_blocks;

    // se destruye el Heap del API
    if ( !::HeapDestroy( m_handle ) )
        return;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapBlock *CHeap::GetBlock( IN CONST INT i ) const
{
    return m_blocks->GetBlock(i);
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapBlockList *CHeap::GetBlocks() const
{
    return m_blocks;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapBlock *CHeap::Allocate( IN CONST DWORD size )
{
    if ( ! m_autoExtend || (m_autoExtend && size < 0x7FFF8) )
    {
        CHeapBlock    *result = new CHeapBlock( this, size );

        m_blocks->Add( result );
        
        return result;
    }
  else
    return NULL;

}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
DWORD CHeap::Compact( VOID )
{
    return ::HeapCompact( m_handle, m_serialize );
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
BOOL CHeap::GetSerialize( VOID ) const
{
    return (m_serialize == 0);
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void CHeap::SetSerialize( IN CONST BOOL value )
{
    if ( value )
        m_serialize = 0;
    else
        m_serialize = HEAP_NO_SERIALIZE;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
HANDLE CHeap::GetHandle( VOID ) const
{
    return m_handle;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
BOOL CHeap::GetAutoExtend( VOID ) const
{
    return m_autoExtend;
}




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
CHeapList::CHeapList( IN OPTIONAL CONST DWORD dwInitialSize, 
                      IN OPTIONAL CONST DWORD flOptions, 
                      IN OPTIONAL CONST DWORD dwMaximumSize )
{
    m_heap        = new CHeap( flOptions, dwInitialSize, dwMaximumSize );
    m_destroyHeap = true;

    m_block = NULL;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapList::CHeapList( IN CHeap *a_heap )
{
    m_heap        = a_heap;
    m_destroyHeap = false;

    m_block = NULL;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapList::~CHeapList( VOID )
{
    // se elimina el montón, y con ello toda la memoria reservada
    if ( m_destroyHeap )
        delete m_heap;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
LPVOID CHeapList::GetItem( IN CONST INT index ) const
{
    if ( index >= 0 && index < m_count ) 
        return m_list[index];
    else
        return NULL;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void CHeapList::SetItem( IN CONST INT    index, 
                         IN CONST LPVOID item )
{
    if ( index >= 0 && index < m_count ) 
        m_list[index] = item;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void CHeapList::Grow( VOID )
{
    int   delta;

    // esto define el algoritmo de crecimiento:
    //    Si la lista tiene más de 64 elementos, crece un 25%
    //    Si la lista tiene entre 9 y 63, crece 16 elementos
    //    Si la lista tiene menos de 9, crece 4 elementos.
    if ( m_capacity > 64 )
        delta = m_capacity / 4;
    else
    {
        if ( m_capacity > 8 )
            delta = 16;
        else
            delta = 4;
    }

    SetCapacity( m_capacity + delta );
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
int CHeapList::Add( IN CONST LPVOID item )
{
    int   aux;

    aux = m_count;

    // si ha llegado al límite, extendemos la lista
    if ( m_count == m_capacity )
        Grow();

    m_list[aux] = item;
    ++m_count;

    return aux;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
VOID CHeapList::Insert( IN CONST INT    index, 
                        IN CONST LPVOID item )
{
    if (index < 0 || index >= m_count )
        return;

    // si ha llegado al límite, extendemos la lista
    if ( m_count == m_capacity )
        Grow();

    // mueve lo que haya por detrás, para dejar un espacio libre
    if ( index < m_count )
        memcpy(m_list[index + 1], m_list[index], (m_count - index) * sizeof(LPVOID) );


    m_list[index] = item;
    ++m_count;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
VOID CHeapList::Clear( VOID )
{
    SetCount(0);
    SetCapacity(0);
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
VOID CHeapList::Delete( IN CONST INT index )
{
    if ( index < 0 || index >= m_count ) 
        return;

    --m_count;

    // se mueven los que estaban por detrás para "tapar el hueco".
    if (index < m_count )
        memcpy( m_list[index], m_list[index + 1], (m_count - index) * sizeof(LPVOID) );
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
INT CHeapList::GetIndex( IN CONST LPVOID item ) const
{
    INT    i = 0;

    while ( i < m_count && m_list[i] != item )
        i++;

    if ( i == m_count )
        return 0;
    else
        return i;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
LPVOID *CHeapList::GetList( VOID ) const
{
    return m_list;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeap *CHeapList::GetHeap( VOID ) const
{
    return m_heap;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void CHeapList::SetCapacity( IN CONST INT NewCapacity )
{
    if ( NewCapacity < m_count || NewCapacity > MAX_LIST_SIZE )
        return;

    if ( NewCapacity != m_capacity )
    {
        m_capacity = NewCapacity;

        if ( m_block == NULL )
            m_block = new CHeapBlock(m_heap, NewCapacity * sizeof(LPVOID) );
        else
            m_block->SetSize( NewCapacity * sizeof(LPVOID) );

        // la lista es el bloque de memoria que acabo de reservar
        m_list = (LPVOID *) m_block->GetMemory();
    }
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
INT CHeapList::GetCapacity() const
{
    return m_capacity;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
VOID CHeapList::SetCount( IN CONST INT NewCount )
{
    if ( NewCount < 0 || NewCount > MAX_LIST_SIZE )
        return;

    if ( NewCount > m_capacity )
        SetCapacity( NewCount );

    if ( NewCount > m_count )
        ::ZeroMemory( &m_list[m_count], (NewCount - m_count) * sizeof(LPVOID) );
    else
        for ( int i = m_count - 1; i >= NewCount; i-- )
            this->Delete(i);

  m_count = NewCount;
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
INT CHeapList::GetCount( VOID ) const
{
    return m_count;
}



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
CHeapBlockList::CHeapBlockList( IN CHeap *a_heap ) :
    CHeapList( a_heap )
{
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapBlock *CHeapBlockList::GetBlock( IN CONST INT index )
{
    return (CHeapBlock *) CHeapList::GetItem(index);
}

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
CHeapBlockList::SetBlock( IN CONST INT    index, 
                          IN            CHeapBlock *item )
{
    CHeapList::SetItem( index, item );
}

