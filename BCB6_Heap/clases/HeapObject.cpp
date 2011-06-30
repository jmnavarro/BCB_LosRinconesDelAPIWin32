//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Archivo: HeapObject.cpp
//
// Prop�sito:
//      Implementaci�n de la clase CHeapObject, que representa un objeto que ser� almacenado
//      en un mont�n dedicado a los objetos de su clase.
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
// Fecha:          01/11/2002
//
// Observaciones:  Clase creada en C++ Builder 6 para el art�culo "Los montones en Win32"
//                 publicado en http://users.servicios.retecal.es/sapivi/prog/cpp/montones.html
//
// Copyright:      Este c�digo es de dominio p�blico y se puede utilizar y/o mejorar siempre 
//                 que SE HAGA REFERENCIA AL AUTOR ORIGINAL, ya sea a trav�s de estos 
//                 comentarios o de cualquier otro modo.
//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

#include "HeapObject.h"


//
// variables est�ticas de la clase
//
HANDLE CHeapObject::s_hHeap     = NULL;
UINT   CHeapObject::s_NumBlocks = 0;


//---------------------------------------------------------------------------
CHeapObject::CHeapObject()
{
}


//---------------------------------------------------------------------------
CHeapObject::~CHeapObject()
{
}


//---------------------------------------------------------------------------
HANDLE  CHeapObject::GetHeap( VOID )
{
    return CHeapObject::s_hHeap;
}


//---------------------------------------------------------------------------
void *CHeapObject::operator new( SIZE_T size )
{
    //
    // si todav�a no tenemos mont�n, lo creamos.
    //
    if( !s_hHeap )
    {
        s_hHeap = ::HeapCreate( HEAP_NO_SERIALIZE, INITIAL_SIZE, 0 );

        if( !s_hHeap )
            return NULL;
    }

    void    *obj = (void *) HeapAlloc( s_hHeap, 0, size );

    if( !obj )
        return NULL;
    //
    // se incrementa el contador de objetos de este tipo
    //
    s_NumBlocks++;

    return  obj;
}


//---------------------------------------------------------------------------
void CHeapObject::operator delete( void *obj )
{
    //
    // se decrementa el contador de objetos de este tipo
    //
    if( HeapFree( s_hHeap, 0, obj ) )
        s_NumBlocks--;

    //
    // si ya no quedan objetos, libero el mont�n.
    //
    if( s_NumBlocks == 0 )
    {
        if( HeapDestroy( s_hHeap ) )
            s_hHeap = NULL;
    }
}



