// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "io.h"
#include "SurfX3D.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FormN.h"
#include "FormZ.h"
#include "FormP.h"
#include "FormF.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_PLOT_AUTODRAW, OnPlotAutodraw)
	ON_COMMAND(ID_PLOT_DRAWPLOT, OnPlotDrawplot)
	ON_UPDATE_COMMAND_UI(ID_PLOT_AUTODRAW, OnUpdatePlotAutodraw)
	ON_UPDATE_COMMAND_UI(ID_PLOT_DRAWPLOT, OnUpdatePlotDrawplot)
	ON_COMMAND(ID_EXPORT_POV, OnExportPov)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_POV, OnUpdateExportPov)
	ON_COMMAND(ID_EXPORT_VRML, OnExportVrml)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_VRML, OnUpdateExportVrml)
	ON_WM_SIZE()
	ON_COMMAND(ID_PLOT_CANCEL, OnPlotCancel)
	ON_UPDATE_COMMAND_UI(ID_PLOT_CANCEL, OnUpdatePlotCancel)
	ON_WM_SIZING()
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnUpdateFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_EXPORT_DXF, OnExportDxf)
	ON_COMMAND(ID_EXPORT_STL, OnExportStl)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_DXF, OnUpdateExportDxf)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_STL, OnUpdateExportStl)
	ON_WM_CLOSE()
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_COMMAND(ID_BTN_LOCK_LR, OnBtnLockLr)
	ON_UPDATE_COMMAND_UI(ID_BTN_LOCK_LR, OnUpdateBtnLockLr)
	ON_COMMAND(ID_BTN_LOCK_UD, OnBtnLockUd)
	ON_UPDATE_COMMAND_UI(ID_BTN_LOCK_UD, OnUpdateBtnLockUd)
	//}}AFX_MSG_MAP
	// Global help commands
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_INDICATOR_ANGLES,
	IDS_INDICATOR_WINSIZE,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

// --------------------------------------------------------------
CMainFrame::~CMainFrame()
{
}

// --------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

// --------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

// --------------------------------------------------------------
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_Splitter.CreateStatic(this,1,2))
		return FALSE;

	m_Splitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(200, 200), pContext);

	m_ViewID[SURFTYPE_NONE] = m_Splitter.AddView(0, 0, RUNTIME_CLASS(CFormN), pContext);
	m_ViewID[SURFTYPE_P   ] = m_Splitter.AddView(0, 0, RUNTIME_CLASS(CFormP), pContext);
	m_ViewID[SURFTYPE_Z   ] = m_Splitter.AddView(0, 0, RUNTIME_CLASS(CFormZ), pContext);
	m_ViewID[SURFTYPE_F   ] = m_Splitter.AddView(0, 0, RUNTIME_CLASS(CFormF), pContext);

	theApp.m_pFormN = (CFormN*)m_Splitter.GetView(m_ViewID[SURFTYPE_NONE]);
	theApp.m_pFormP = (CFormP*)m_Splitter.GetView(m_ViewID[SURFTYPE_P   ]);
	theApp.m_pFormZ = (CFormZ*)m_Splitter.GetView(m_ViewID[SURFTYPE_Z   ]);
	theApp.m_pFormF = (CFormF*)m_Splitter.GetView(m_ViewID[SURFTYPE_F   ]);

	m_Splitter.SetColumnInfo( 0, theApp.m_wndSplitX, 0 );
	m_Splitter.ShowView(m_ViewID[SURFTYPE_NONE]);
	
	return TRUE;
}

// --------------------------------------------------------------
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	theApp.UpdateMainWndSize();
}

// --------------------------------------------------------------
void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	theApp.UpdateMainWndSize();
}

// --------------------------------------------------------------
void CMainFrame::OnFileOpen() 
{
	theApp.OnFileOpen();
}

// --------------------------------------------------------------
void CMainFrame::OnFileSaveAs() 
{
	theApp.OnFileSaveAs();
}

// --------------------------------------------------------------
void CMainFrame::OnEditCopy() 
{
	theApp.CopyPlotToClipBoard();
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
void CMainFrame::OnPlotAutodraw() 
{
	theApp.m_autoDraw = !theApp.m_autoDraw;	// toggle mode
}

// --------------------------------------------------------------
void CMainFrame::OnPlotDrawplot() 
{
	theApp.PlotSurface();
}

// --------------------------------------------------------------
void CMainFrame::OnUpdatePlotAutodraw(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.m_autoDraw);
}

// --------------------------------------------------------------
void CMainFrame::OnUpdatePlotDrawplot(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsPlotOpened() && !theApp.IsRendering());
}

// --------------------------------------------------------------
void CMainFrame::OnPlotCancel() 
{
	theApp.GetBasePlot()->RequestCancel();
}

// --------------------------------------------------------------
void CMainFrame::OnUpdatePlotCancel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendering());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateExportVrml(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateExportPov(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateExportDxf(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateExportStl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
// guarantee file has certain extension
CString CMainFrame::CheckFileExt(CString fileName,CString extName)
{
	CString fn = fileName;
	extName = '.' + extName;
	CString cmpName = fileName.Right( extName.GetLength() );
	if (extName.CompareNoCase(cmpName)!=0)
		fn = fn + extName;	// add extension name
	return(fn);
}

// --------------------------------------------------------------
// build a default filepath with same name but different file extension
CString CMainFrame::MakeDefaultName(CString fullName,CString destPath,CString extName)
{
	CString sName = ExtractFileName(fullName);
	CString sPath = destPath + "\\" + sName + "." + extName;
	return(sPath);
}

// --------------------------------------------------------------
void CMainFrame::OnExportPov() 
{
	static char szFilter[] = "POV Files (*.pov;*.inc)|*.pov;*.inc||";
	CString sSaveAsFile;
	int rc=0;

	// build a default file name
	sSaveAsFile = MakeDefaultName(theApp.m_sPlotFilePath,theApp.m_sLastSubDirPOV,POV_FILE_EXTENSION);

	// allow user to select file
	CFileDialog fdlg(FALSE,POV_FILE_EXTENSION,(LPCTSTR)sSaveAsFile,
		OFN_LONGNAMES | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		szFilter,this);
	if (fdlg.DoModal() != IDOK) return;
	sSaveAsFile = CheckFileExt(fdlg.GetPathName(),POV_FILE_EXTENSION);
	
	// remember path
	theApp.m_sLastSubDirPOV = ExtractPathDir(sSaveAsFile);

	// write the file
	switch (theApp.GetSurfType())
	{
	case SURFTYPE_Z:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DZ.ExportPOV(sSaveAsFile, theApp.m_plot3DZ.GetSurfName());
		}
		break;

	case SURFTYPE_P:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DP.ExportPOV(sSaveAsFile, theApp.m_plot3DP.GetSurfName());
		}

	case SURFTYPE_F:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DF.ExportPOV(sSaveAsFile, theApp.m_plot3DF.GetSurfName());
		}
		break;
	} // switch

	if (rc) MessageBox("Error exporting file to POV");
}

// --------------------------------------------------------------
void CMainFrame::OnExportVrml() 
{
	static char szFilter[] = "VRML Files (*.wrl)|*.wrl||";
	CString sSaveAsFile;
	int rc=0;

	// build a default file name
	sSaveAsFile = MakeDefaultName(theApp.m_sPlotFilePath,theApp.m_sLastSubDirVRML,VRML_FILE_EXTENSION);

	// allow user to select file
	CFileDialog fdlg(FALSE,VRML_FILE_EXTENSION,(LPCTSTR)sSaveAsFile,
		OFN_LONGNAMES | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		szFilter,this);
	if (fdlg.DoModal() != IDOK) return;
	sSaveAsFile = CheckFileExt(fdlg.GetPathName(),VRML_FILE_EXTENSION);

	// remember path
	theApp.m_sLastSubDirVRML = ExtractPathDir(sSaveAsFile);

	// write the file
	switch (theApp.GetSurfType())
	{
	case SURFTYPE_Z:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DZ.ExportVRML(sSaveAsFile, theApp.m_plot3DZ.GetSurfName());
		}
		break;

	case SURFTYPE_P:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DP.ExportVRML(sSaveAsFile, theApp.m_plot3DP.GetSurfName());
		}
		break;

	case SURFTYPE_F:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DF.ExportVRML(sSaveAsFile, theApp.m_plot3DF.GetSurfName());
		}
		break;
	} // switch

	if (rc) MessageBox("Error exporting file to VRML");
}

// --------------------------------------------------------------
void CMainFrame::OnExportDxf() 
{
	static char szFilter[] = "Drawing Exchange Files (*.dxf)|*.dxf||";
	CString sSaveAsFile;
	int rc=0;

	// build a default file name
	sSaveAsFile = MakeDefaultName(theApp.m_sPlotFilePath,theApp.m_sLastSubDirDXF,DXF_FILE_EXTENSION);

	// allow user to select file
	CFileDialog fdlg(FALSE,DXF_FILE_EXTENSION,(LPCTSTR)sSaveAsFile,
		OFN_LONGNAMES | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		szFilter,this);
	if (fdlg.DoModal() != IDOK) return;
	sSaveAsFile = CheckFileExt(fdlg.GetPathName(),DXF_FILE_EXTENSION);

	// remember path
	theApp.m_sLastSubDirDXF = ExtractPathDir(sSaveAsFile);

	// write the file
	switch (theApp.GetSurfType())
	{
	case SURFTYPE_Z:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DZ.ExportDXF(sSaveAsFile, theApp.m_plot3DZ.GetSurfName());
		}
		break;

	case SURFTYPE_P:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DP.ExportDXF(sSaveAsFile, theApp.m_plot3DP.GetSurfName());
		}
		break;

	case SURFTYPE_F:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DF.ExportDXF(sSaveAsFile, theApp.m_plot3DF.GetSurfName());
		}
		break;
	} // switch

	if (rc) MessageBox("Error exporting file to DXF");
}

// --------------------------------------------------------------
void CMainFrame::OnExportStl() 
{
	static char szFilter[] = "Drawing Exchange Files (*.stl)|*.stl||";
	CString sSaveAsFile;
	int rc=0;

	// build a default file name
	sSaveAsFile = MakeDefaultName(theApp.m_sPlotFilePath,theApp.m_sLastSubDirSTL,STL_FILE_EXTENSION);

	// allow user to select file
	CFileDialog fdlg(FALSE,STL_FILE_EXTENSION,(LPCTSTR)sSaveAsFile,
		OFN_LONGNAMES | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		szFilter,this);
	if (fdlg.DoModal() != IDOK) return;
	sSaveAsFile = CheckFileExt(fdlg.GetPathName(),STL_FILE_EXTENSION);

	// remember path
	theApp.m_sLastSubDirSTL = ExtractPathDir(sSaveAsFile);

	// write the file
	switch (theApp.GetSurfType())
	{
	case SURFTYPE_Z:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DZ.ExportSTL(sSaveAsFile, theApp.m_plot3DZ.GetSurfName());
		}
		break;

	case SURFTYPE_P:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DP.ExportSTL(sSaveAsFile, theApp.m_plot3DP.GetSurfName());
		}
		break;

	case SURFTYPE_F:
		{
		CWaitCursor wait;	// put up hour glass
		rc = theApp.m_plot3DF.ExportSTL(sSaveAsFile, theApp.m_plot3DF.GetSurfName());
		}
		break;
	} // switch

	if (rc) MessageBox("Error exporting file to STL");
}

// --------------------------------------------------------------
// enable/disable toolbar buttons
void CMainFrame::UpdateToolBar() 
{
	CToolBarCtrl* tbar = &m_wndToolBar.GetToolBarCtrl();
	int isOpened, isRendered, isRendering, isPrinting, notActive;

	// get program states
	isOpened    = theApp.IsPlotOpened();
	isRendered  = theApp.IsRendered();
	isRendering = theApp.IsRendering();
	isPrinting  = theApp.IsPrinting();
	notActive   = !isRendering && !isPrinting;

	// set button states
	tbar->EnableButton(ID_FILE_NEW,      notActive);
	tbar->EnableButton(ID_FILE_OPEN,     notActive);
	tbar->EnableButton(ID_FILE_SAVE,     isOpened && notActive);
	tbar->EnableButton(ID_EDIT_COPY,     isRendered);
	tbar->EnableButton(ID_PLOT_CANCEL,   isRendering);
	tbar->EnableButton(ID_PLOT_DRAWPLOT, isOpened && notActive);
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.IsRendering() && !theApp.IsPrinting());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.IsRendering() && !theApp.IsPrinting());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered() && !theApp.IsPrinting());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateFilePrintSetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered() && !theApp.IsPrinting());
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered() && !theApp.IsPrinting());

	// MUST have right window as active view for printing
	if (theApp.m_pRightView)
		SetActiveView(theApp.m_pRightView);
}

// --------------------------------------------------------------
// handles title bar [x] app close
void CMainFrame::OnClose() 
{
	if (theApp.IsRendering())
	{
		theApp.GetBasePlot()->RequestCancel();
		return;
	}
	theApp.SaveOnExit();
	
	CFrameWnd::OnClose();
}

// --------------------------------------------------------------
void CMainFrame::OnZoomIn() 
{
	Plot3D* pPlot = theApp.GetBasePlot();
	if (pPlot) pPlot->ZoomIn();
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
void CMainFrame::OnZoomOut() 
{
	Plot3D* pPlot = theApp.GetBasePlot();
	if (pPlot) pPlot->ZoomOut();
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
}

// --------------------------------------------------------------
void CMainFrame::OnBtnLockLr() 
{
	theApp.m_lockLeftRight = !theApp.m_lockLeftRight;
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateBtnLockLr(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
	pCmdUI->SetCheck(theApp.m_lockLeftRight);
}

// --------------------------------------------------------------
void CMainFrame::OnBtnLockUd() 
{
	theApp.m_lockUpDown = !theApp.m_lockUpDown;
}

// --------------------------------------------------------------
void CMainFrame::OnUpdateBtnLockUd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.IsRendered());
	pCmdUI->SetCheck(theApp.m_lockUpDown);
}

// --------------------------------------------------------------
