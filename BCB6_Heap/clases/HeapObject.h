//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
// Unidad: HeapObject.h
//
// Prop�sito:
//      Interfaz de la clase CHeapObject, que representa un objeto que ser� almacenado
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

#ifndef HEAPOBJECT_H
#define HEAPOBJECT_H


#include <windows.h>

#define INITIAL_SIZE    1024 * 64


class CHeapObject
{
private:
    static HANDLE   s_hHeap;
    static UINT     s_NumBlocks;

public:

    CHeapObject();
    virtual ~CHeapObject();

    HANDLE  static GetHeap( VOID );

    void *operator new( SIZE_T size );
    void operator delete( void *obj );
};

#endif // HEAPOBJECT_H