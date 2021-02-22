// SurfX3DDoc.cpp : implementation of the CSurfX3DDoc class
//

#include "stdafx.h"
#include "SurfX3D.h"

#include "SurfX3DDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SurfX3D.h"
#include "DlgNewSurf.h"

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DDoc

IMPLEMENT_DYNCREATE(CSurfX3DDoc, CDocument)

BEGIN_MESSAGE_MAP(CSurfX3DDoc, CDocument)
	//{{AFX_MSG_MAP(CSurfX3DDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DDoc construction/destruction

CSurfX3DDoc::CSurfX3DDoc()
{
}

CSurfX3DDoc::~CSurfX3DDoc()
{
}

BOOL CSurfX3DDoc::OnNewDocument()
{
	if (!theApp.IsInitialized()) return(TRUE); // not FALSE, program will terminate

	if (!CDocument::OnNewDocument())
		return FALSE;

	CDlgNewSurf dlg;
	dlg.m_SurfaceType = theApp.GetSurfType();
	int rc = dlg.DoModal();
	if (rc != IDOK) return(FALSE);

	// open new surface
	theApp.NewSurface(dlg.m_SurfaceType);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSurfX3DDoc serialization

void CSurfX3DDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DDoc diagnostics

#ifdef _DEBUG
void CSurfX3DDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSurfX3DDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DDoc commands

void CSurfX3DDoc::OnFileSave() 
{
	if (theApp.GetFilePath().IsEmpty())
	{
		// prompt user for file name
		OnFileSaveAs();
		return;
	}
	theApp.WriteFile();
}

void CSurfX3DDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	int isEnabled = theApp.IsPlotOpened() &&
		!theApp.m_sPlotFilePath.IsEmpty() &&
		!theApp.IsRendering();
	pCmdUI->Enable(isEnabled);
}

// --------------------------------------------------------------
void CSurfX3DDoc::OnFileSaveAs() 
{
	theApp.OnFileSaveAs();
}

void CSurfX3DDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	int isEnabled = theApp.IsPlotOpened() && !theApp.IsRendering();
	pCmdUI->Enable(isEnabled);
}

// --------------------------------------------------------------
