//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "HeapObject.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // creo la lista que contendrá los objetos creados
    FObjetos  = new TList;
    FHeap     = NULL;
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    HICON   icono = (HICON) SendMessage(this->Handle, WM_GETICON, ICON_BIG, 0);

    i_icono->Picture->Icon->Handle = icono;
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
    while ( FObjetos->Count > 0)
    {
        lb_objetos->Selected[0] = true;
        DestruirObjeto( NULL );
    }

    delete FObjetos;
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::l_jmClick(TObject *Sender)
{
    ShellExecute(GetActiveWindow(), NULL, "mailto:jose_manuel_navarro@yahoo.es", NULL, NULL, SW_NORMAL);
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::CrearObjeto(TObject *Sender)
{
    TCHAR           str[255];
    CHeapObject    *ptr;

    ptr = new CHeapObject();
    FObjetos->Add( ptr );

    if ( lb_log->Count > 0 )
        lb_log->ItemIndex = lb_log->Items->Add( "" );

    wsprintf( str, "Objeto %i: 0x%X", lb_objetos->Items->Count + 1, ptr );
    lb_objetos->ItemIndex = lb_objetos->Items->Add( str );

    if ( FHeap == 0 && FHeap != CHeapObject::GetHeap() )
    {
        FHeap = CHeapObject::GetHeap();

        wsprintf( str, "Creado montón con handle 0x%X", FHeap );
        lb_log->ItemIndex = lb_log->Items->Add( str );
    }

    wsprintf( str, "Creado objeto de tipo CHeapObject en la dirección 0x%X", ptr );
    lb_log->ItemIndex = lb_log->Items->Add( str );

    b_destruir->Enabled = true;
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::DestruirObjeto(TObject *Sender)
{
    TCHAR           str[255];
    CHeapObject    *ptr;
    INT             ind = lb_objetos->ItemIndex;

    // si no hay nada seleccionado, no puedo destruír
    if ( ind == -1 )
        return;

    ptr = (CHeapObject *) FObjetos->Items[ind];

    // libero la memoria y elimino el objeto de las lista
    delete (CHeapObject *) FObjetos->Items[ind];
    FObjetos->Delete(ind);

    lb_log->ItemIndex = lb_log->Items->Add( "" );
    lb_objetos->DeleteSelected();
    wsprintf( str, "Destruido objeto %i (0x%X)", ind + 1, ptr );
    lb_log->ItemIndex = lb_log->Items->Add( str );

    // si el heap actual es distinto al que tengo apuntado, es que ha sido borrado
    if ( CHeapObject::GetHeap() != FHeap )
    {
        wsprintf( str, "Montón con handle 0x%X destruido.", FHeap );
        lb_log->ItemIndex = lb_log->Items->Add( str );

        FHeap = NULL;
    }
    else
        lb_objetos->ItemIndex = 0;

    b_destruir->Enabled = (lb_objetos->Count > 0);
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::VerMontonesProceso(TObject *Sender)
{
	PHANDLE		vector;
	DWORD		i;
	DWORD		NumeroMontones;
	TCHAR		DescMonton[255] = "";

	// obtener el número total de montones
	NumeroMontones = ::GetProcessHeaps( 0, vector );

	// crear un vector dinámico a través de punteros y inicializa a 0
	vector = (PHANDLE) calloc( NumeroMontones, sizeof(THandle) );

	// se obtienen la lista de descriptores
	::GetProcessHeaps( NumeroMontones, vector );

    if ( lb_log->Count > 0 )
        lb_log->ItemIndex = lb_log->Items->Add( "" );

    wsprintf( DescMonton, " - Montones del proceso - " );
    lb_log->ItemIndex = lb_log->Items->Add( DescMonton );

	for ( i = 0; i < NumeroMontones; i++ )
	{
		if ( i == 0 )
			wsprintf( DescMonton, "   Montón por defecto con handle 0x%X", vector[i] );
		else
			wsprintf( DescMonton, "   Montón número %i con handle 0x%X", i, vector[i] );

        lb_log->ItemIndex = lb_log->Items->Add( DescMonton );
	}

	free( vector );
}

//---------------------------------------------------------------------------
