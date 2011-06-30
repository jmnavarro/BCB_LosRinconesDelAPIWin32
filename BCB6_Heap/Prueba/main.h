//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
    TButton *b_crear;
    TImage *i_icono;
    TButton *b_destruir;
    TListBox *lb_objetos;
    TBevel *bv_bottom;
    TListBox *lb_log;
    TLabel *l_jm;
    TButton *b_verMontones;
    TBevel *Bevel1;
    TBevel *Bevel2;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);

    void __fastcall CrearObjeto(TObject *Sender);
    void __fastcall l_jmClick(TObject *Sender);
    void __fastcall DestruirObjeto(TObject *Sender);
    void __fastcall VerMontonesProceso(TObject *Sender);

private:
    TList   *FObjetos;
    HANDLE   FHeap;

public:
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
