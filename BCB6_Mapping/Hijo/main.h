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

#ifndef mainH
#define mainH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <ExtCtrls.hpp>

#include "..\comun.h"


//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo1;
   TTimer *Timer1;
   TBevel *Bevel1;
   TLabel *l_jm;
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall l_jmClick(TObject *Sender);

public:		// User declarations
   __fastcall TForm1(TComponent* Owner);

private:
   LPSTR   m_vista;
};

//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------

#endif
