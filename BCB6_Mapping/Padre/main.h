//---------------------------------------------------------------------------------------------
//
// Archivo: main.h
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

#ifndef mainH
#define mainH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "proyeccion.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>


//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo1;
   TBevel *Bevel1;
   TLabel *l_jm;
   TListView *lv_procesos;
   TLabel *Label1;
   TBevel *Bevel2;
   TPopupMenu *PopupMenu1;
   TMenuItem *m_TerminarProceso;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall Memo1Change(TObject *Sender);
   void __fastcall l_jmClick(TObject *Sender);
   void __fastcall m_TerminarProcesoClick(TObject *Sender);

public:		// User declarations
   __fastcall TForm1(TComponent* Owner);

private:
   CProyeccion *m_proyeccion;
   HANDLE       hProcesoHijo;

   void __fastcall CrearProcesoHijo();
   void __fastcall TerminarProcesoHijo();

};


//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------


#endif
