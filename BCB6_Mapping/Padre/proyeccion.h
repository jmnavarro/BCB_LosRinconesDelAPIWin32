//---------------------------------------------------------------------------------------------
//
// Archivo: proyeccion.h
//
// Prop�sito:
//    Se define la clase CProyeccion, que permite manejar una proyeccion de archivos
//
// Autor:          Jos� Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/11/2002
//
// Observaciones:  Clase creada en C++ Builder 6 para el art�culo "Archivos proyectados en
//                 memoria", publicado en http://users.servicios.retecal.es/sapivi/prog/win32
//
// Copyright:      Este c�digo es de dominio p�blico y se puede utilizar y/o mejorar siempre 
//                 que SE HAGA REFERENCIA AL AUTOR ORIGINAL, ya sea a trav�s de estos 
//                 comentarios o de cualquier otro modo.
//
//---------------------------------------------------------------------------------------------

#ifndef proyeccionH
#define proyeccionH

#include <windows.h>


#include "..\comun.h"


//---------------------------------------------------------------------------


class CProyeccion
{
public:
   HANDLE  m_hProyeccion;
   LPSTR   m_vista;

public:
   CProyeccion();
   virtual ~CProyeccion();

   DWORD Abrir();
   void Cerrar();
   void Actualizar(LPCSTR texto);
};



#endif
