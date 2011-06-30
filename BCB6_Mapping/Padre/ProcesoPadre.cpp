//---------------------------------------------------------------------------------------------
//
// Proyecto: ProcesoPadre
//
// Propósito:
//    Demuestra el uso de la herencia de descriptores entre procesos padre e hijo.
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


//---------------------------------------------------------------------------
USEFORM("main.cpp", Form1);


//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->Title = "Proceso Padre";
       Application->CreateForm(__classid(TForm1), &Form1);
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   catch (...)
   {
       try
       {
           throw Exception("");
       }
       catch (Exception &exception)
       {
           Application->ShowException(&exception);
       }
   }
   return 0;
}
//---------------------------------------------------------------------------
