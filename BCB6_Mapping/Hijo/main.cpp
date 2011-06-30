//---------------------------------------------------------------------------------------------
//
// Archivo: main.cpp
//
// Propósito:
//    Formulario principal del proceso hijo para probar archivos proyectados en memoria.
//    Hace uso de la técnica de herencia de descriptores para compartir datos entre una
//    aplicación padre con sus procesos hijos.
//
// Autor:          José Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/02/2003
//
// Observaciones:  Clase creada en C++ Builder 6 para el artículo "Archivos proyectados en
//                 memoria", publicado en http://users.servicios.retecal.es/sapivi/prog/win32
//
// Copyright:      Este código es de dominio público y se puede utilizar y/o mejorar siempre 
//                 que SE HAGA REFERENCIA AL AUTOR ORIGINAL, ya sea a través de estos 
//                 comentarios o de cualquier otro modo.
//
//---------------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;



//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) { }



//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   TCHAR   szMap[64];
   HANDLE  hMapPadre;

   ::ZeroMemory(szMap, 64);

   //
   // Se recupera la variable de entorno que contiene el descriptor
   // del objeto proyección en el proceso padre.
   //
   if ( GetEnvironmentVariable(VAR_MAP, szMap, 64) == 0 )
   {
       ::MessageBox(GetActiveWindow(),
                   "El proceso padre debe ser arrancado antes que los procesos hijos.",
                   "Proceso Hijo", MB_ICONSTOP);
       this->Hide();
       Application->Terminate();
   }
   else
   {
       //
       // Ahora se crea la vista directamente, ya que el descriptor ha sido heredado a este
       // proceso.
       //
       hMapPadre = (HANDLE) atoi(szMap);

       m_vista = (LPSTR) ::MapViewOfFile(
                               hMapPadre,
                               FILE_MAP_WRITE, 0, 0, MAX_FILEMAPPING_CHARS );
       //
       // Una vez que no se va a utilizar más un objeto proyección, puede cerrarse,
       // aunque las vistas sí que se vayan a utilizar.
       //
       ::CloseHandle(hMapPadre);

       if ( !m_vista )
          RaiseLastWin32Error();
   }
}



//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
   //
   // Se copia el contenido de la vista en el memo, sólo si la vista es válida y
   // la bandera de modificación está levantada.
   // Se puede comprobar el estado de la bandera de modificación leyendo sobre el primer
   // byte de la vista.
   //
   if ( m_vista && m_vista[0] != 0 )
   {
      Memo1->Lines->SetText( m_vista + 1 );
      //
      // Ahora se baja la bandera de modificación, porque el contenido ya lo hemos
      // copiado.
      //
      m_vista[0] = 0;
   }
}



//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
   if ( m_vista )
      ::UnmapViewOfFile(m_vista);
}



//---------------------------------------------------------------------------
void __fastcall TForm1::l_jmClick(TObject *Sender)
{
   ::ShellExecute( ::GetForegroundWindow(),
                   NULL,
                   "mailto:jose_manuel_navarro@yahoo.es",
                   NULL,
                   NULL,
                   SW_NORMAL);

  l_jm->Font->Color = clPurple;
}
