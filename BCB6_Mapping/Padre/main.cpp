//---------------------------------------------------------------------------------------------
//
// Archivo: main.cpp
//
// Propósito:
//    Formulario principal del proceso padre para probar archivos proyectados en memoria.
//    Hace uso de la técnica de herencia de descriptores para compartir datos entre una
//    aplicación padre con sus procesos hijos.
//
// Autor:          José Manuel Navarro (jose.man_ARROBA_iespana.es)
//
// Fecha:          01/11/2002
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

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;



//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
   hProcesoHijo = 0;
}



//---------------------------------------------------------------------------
void __fastcall TForm1::CrearProcesoHijo()
{
   SECURITY_ATTRIBUTES     sa;
   STARTUPINFO             si;
   PROCESS_INFORMATION     pi;

   //
   // inicializar los valores de SECURITY_ATTRIBUTES
   //
   ::ZeroMemory( &sa, sizeof(sa) );
   sa.nLength        = sizeof(sa);
   sa.bInheritHandle = TRUE;

   //
   // inicializar los valores de STARTUPINFO
   //
   ::ZeroMemory( &si, sizeof(si) );
   si.cb          = sizeof(si);
   si.wShowWindow = SW_NORMAL;

   //
   // Intento ejecutar el hijo en su propia carpeta
   //
   LPSTR   cmd = "..\\Hijo\\ProcesoHijo.exe";
   do {
      if ( !::CreateProcess( cmd, NULL, NULL, NULL, true,
                             0, NULL, NULL, &si, &pi ) )
      {
          //
          // Si no encuentra el archivo y es la primera vez,
          // lo intenta en la misma en que está el padre. Si es la segunda vez,
          // muestra el error
          //
          if ( ::GetLastError() == 2 )
             if ( cmd[0] == '.' )
                cmd = "ProcesoHijo.exe";
             else
             {
                ::MessageBox( GetActiveWindow(),
                              "No se ha encontrado el proceso hijo en el ejecutable \"ProcesoHijo.exe\".",
                              "Proceso Padre", MB_ICONERROR );
                break;
             }
          else
             RaiseLastWin32Error();
      }
      else
      {
         //
         // si ejecuta correctamente el proceso, salimos el bucle
         //
         break;
      }
   }
   while ( true );

   //
   // Si el proceso se ha creado correctamente, se introducen los datos
   //
   if ( pi.hProcess != 0 )
   {
      this->hProcesoHijo = pi.hProcess;

      //
      // Se introducen en el ListView los datos del proceso hijo
      //
      TListItem   *item;
      lv_procesos->Items->BeginUpdate();

      item = lv_procesos->Items->Add();
      item->Caption = IntToStr(lv_procesos->Items->Count);
      item->SubItems->Add( IntToStr( pi.hProcess ) );
      item->SubItems->Add( IntToStr( pi.dwProcessId ) );

      lv_procesos->Items->EndUpdate();
   }
}



//---------------------------------------------------------------------------
void __fastcall TForm1::TerminarProcesoHijo()
{
   if ( hProcesoHijo == NULL )
       return;

   if ( ! ::TerminateProcess( hProcesoHijo, 0 ) )
       ::MessageBox( GetForegroundWindow(), "No se ha encontrado el proceso hijo.",
                     "Terminar proceso", MB_ICONWARNING );
   else
       hProcesoHijo = NULL;

   lv_procesos->Items->Clear();
}



//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   m_proyeccion = new CProyeccion;

   m_proyeccion->Abrir();
   this->Memo1Change(NULL);

   this->CrearProcesoHijo();
}



//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
   TerminarProcesoHijo();
   
   delete m_proyeccion;
}



//---------------------------------------------------------------------------
void __fastcall TForm1::Memo1Change(TObject *Sender)
{
   m_proyeccion->Actualizar(Memo1->Text.c_str());
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



//---------------------------------------------------------------------------
void __fastcall TForm1::m_TerminarProcesoClick(TObject *Sender)
{
   TerminarProcesoHijo();
}

