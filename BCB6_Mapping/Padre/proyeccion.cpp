//---------------------------------------------------------------------------------------------
//
// Archivo: proyeccion.cpp
//
// Prop�sito:
//    Se implementa la clase CProyeccion, que permite manejar una proyeccion de archivos
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

#pragma hdrstop

#include "proyeccion.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



//---------------------------------------------------------------------------
CProyeccion::CProyeccion()
{
   m_hProyeccion = 0;
   m_vista = NULL;
}



//---------------------------------------------------------------------------
CProyeccion::~CProyeccion()
{
   this->Cerrar();
}



//---------------------------------------------------------------------------
DWORD CProyeccion::Abrir()
{
   if ( !m_hProyeccion )
   {
       SECURITY_ATTRIBUTES     sa;
       TCHAR                   buff[64];

       ::ZeroMemory( &sa, sizeof(sa) );
       sa.nLength        = sizeof(sa);
       sa.bInheritHandle = TRUE;
       //
       // Se crea el objeto proyecci�n
       //
       m_hProyeccion = ::CreateFileMapping(
                               (void *) 0xFFFFFFFF,     // en el archivo de paginaci�n
                               &sa,                     // crear el descriptor heradable
                               PAGE_READWRITE,          // lectura/escritura
                               0, MAX_FILEMAPPING_CHARS,// tama�o m�ximo
                               NULL );
       if ( !m_hProyeccion )
           return 0;

       //
       // Almaceno el handle de la proyecci�n en las variables de entorno
       // para poder ser duplicado desde el proceso hijo
       //
       wsprintf(buff, "%d", m_hProyeccion);
       SetEnvironmentVariable(VAR_MAP, buff);
   }

   if ( !m_vista )
   {
       //
       // Se crea la vista
       //
       m_vista = (TCHAR *) ::MapViewOfFile(
                                 m_hProyeccion,         // objeto proyecci�n
                                 FILE_MAP_WRITE,        // lectura /escritura
                                 0, 0,                  // desplazamiento: desde el inicio
                                 MAX_FILEMAPPING_CHARS);// tama�o: toda la proyecci�n
       if ( !m_vista )
           return 0;
       else if ( GetLastError() == ERROR_ALREADY_EXISTS )
           return ERROR_ALREADY_EXISTS;
   }

   return 1;
}



//---------------------------------------------------------------------------
void CProyeccion::Cerrar()
{
   if ( m_vista && ::UnmapViewOfFile(m_vista) )
       m_vista = NULL;

   if ( m_hProyeccion )
   {
       ::CloseHandle(m_hProyeccion);
       m_hProyeccion = 0;
       //
       // borrar la variable de entorno
       //
       SetEnvironmentVariable(VAR_MAP, NULL);
   }
}



//---------------------------------------------------------------------------
void CProyeccion::Actualizar(LPCSTR texto)
{
   //
   // el primer byte de la vista se utiliza como bandera de modificaci�n, indicando
   // un valor distinto de 0 cuando la vista ha sido modificada, y 0 cuando el contenido
   // de la vista no est� modificado.
   //
   PBYTE   flag_modif = m_vista;
   LPSTR   contenido  = m_vista + 1;
   //
   // Se copia el contenido del par�metro "texto" al contenido de la vista.
   //
   lstrcpy(contenido, texto);
   //
   // se levanta la bandera
   //
   *flag_modif = 0xFF;
}

