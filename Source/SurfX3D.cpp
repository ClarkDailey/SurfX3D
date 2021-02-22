// SurfX3D.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxpriv.h"
#include "SurfX3D.h"

#include "MainFrm.h"
#include "SurfX3DDoc.h"
#include "SurfX3DView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DApp

BEGIN_MESSAGE_MAP(CSurfX3DApp, CWinApp)
	//{{AFX_MSG_MAP(CSurfX3DApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_TIPS, OnHelpTips)
	ON_COMMAND(ID_HELP_FUNCTIONS, OnHelpFunctions)
	ON_COMMAND(ID_OPEN_HELP_FILE, OnHelpOpenHelpFile)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DApp construction

CSurfX3DApp::CSurfX3DApp()
{
	// Place all significant initialization in InitInstance
	m_isPlotOpened  = 0;
	m_is_printing   = 0;
	m_is_data_unsaved = 0;
	m_lockLeftRight = 0;
	m_lockUpDown    = 0;
    m_firstTimePlot = 0;
	m_SurfaceType = SURFTYPE_NONE;
	m_autoDraw    = 1;
	m_pMainFrame  = 0;	// indicates initialized
	m_pFormZ      = 0;
	m_pFormF      = 0;
	m_pFormP      = 0;
	m_pRightView  = 0;
	m_pRightWnd   = 0;
	m_sDefExt     = EXT_P;
	m_wndX        = 75;
	m_wndY        =	75;
	m_wndSizeX    = 700;
	m_wndSizeY    =	600;
	m_wndSplitX   = 200;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSurfX3DApp object

CSurfX3DApp theApp;

#define NMRU_LIST	9	// number of Most Recently Used files

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DApp initialization

BOOL CSurfX3DApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

  #ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
  #else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
  #endif

    // get path of executable file
    char path[MAX_PATH];
    ::GetModuleFileName(NULL, path, MAX_PATH);
    m_sExeFullPath = path;
    m_sExeFolder   = ExtractPathDir(path);

//    m_sExeFolder  = "C:\\Projects\\SurfX3D"; // ### testing while debugging only

    // build help file path if file exists
    CString sTemp = m_sExeFolder + "\\" HELP_FILE_NAME;
    if (DoesFileExist(sTemp))
        m_sHelpFile = sTemp;

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("SurfX3D"));

    // file associations - need admin rights to set
    // "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\"

	LoadStdProfileSettings(NMRU_LIST);  // Load standard INI file options (including MRU)
	LoadUserSettings();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSurfX3DDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSurfX3DView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	m_pMainFrame = (CMainFrame*)GetMainWnd();
	m_pRecentFileList->m_nMaxDisplayLength = 200;  // MRU file length
	m_pMainFrame->MoveWindow(m_wndX,m_wndY,m_wndSizeX,m_wndSizeY,TRUE);

    // plot
    if (m_firstTimePlot)
    {
        // first time plot; show an example
        OpenSurface(m_sPlotFilePath);
    }
    else
    {
        // command line plotting
        if (strlen(m_lpCmdLine) > 0) OpenSurface(m_lpCmdLine);
    }

	return TRUE;
}

// --------------------------------------------------------------
void CSurfX3DApp::SetSurfType(int surfType)
{
	m_SurfaceType = surfType;
	m_sDefExt = GetSurfaceExt(m_SurfaceType);
}

// --------------------------------------------------------------
void CSurfX3DApp::ShowSurfForm()
{
	if (!IsInitialized()) return;	// don't crash

	int viewID;
	viewID = m_pMainFrame->m_ViewID[GetSurfType()];
	m_pMainFrame->m_Splitter.ShowView(viewID);

	switch (GetSurfType())
	{
	case SURFTYPE_Z: m_pFormZ->UpdateForm(); break;
	case SURFTYPE_P: m_pFormP->UpdateForm(); break;
	case SURFTYPE_F: m_pFormF->UpdateForm(); break;
	} // switch

	// allow form dialog to update
	int i=0;
	MSG msg;
	Sleep(100);
	while (i++<100 && ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
	{ 
		PumpMessage();
	}
}

// --------------------------------------------------------------
void CSurfX3DApp::UpdateMainWndSize()
{
	if (!IsInitialized()) return;	// don't crash

	RECT rect;
	m_pMainFrame->GetWindowRect(&rect);
	m_wndX     = rect.left;
	m_wndY     = rect.top;
	m_wndSizeX = rect.right  - rect.left;
	m_wndSizeY = rect.bottom - rect.top;

	int cx,cy;
	m_pMainFrame->m_Splitter.GetColumnInfo(0,cx,cy);
	m_wndSplitX = cx;
	
	m_pRightWnd = m_pMainFrame->m_Splitter.GetPane(0, 1);
	m_pRightWnd->GetClientRect(&rect);	// GetWindowRect()
	int wndPlotSizeX = rect.right  - rect.left;
	int wndPlotSizeY = rect.bottom - rect.top;

	CString sStatus;
	sStatus.Format("%ux%u", wndPlotSizeX, wndPlotSizeY);
	m_pMainFrame->m_wndStatusBar.SetPaneText(2,sStatus);
}

// --------------------------------------------------------------
void CSurfX3DApp::ShowRotAngles(double xyAngle,double zAngle)
{
	if (!IsInitialized()) return;	// don't crash

	CString sAngles;
	sAngles.Format("xy=%-.1lf z=%-.1lf   ", xyAngle, zAngle);
	m_pMainFrame->m_wndStatusBar.SetPaneText(1,sAngles);
}

// --------------------------------------------------------------
void CSurfX3DApp::PlotSurface()
{
	if (theApp.IsRendering()) return;

	// get info from dialog
	GetDlg();
	UpdateMainWndSize();

	// plot the surface
	switch (GetSurfType())
	{
	case SURFTYPE_Z: m_plot3DZ.Plot(m_pRightWnd); break;
	case SURFTYPE_P: m_plot3DP.Plot(m_pRightWnd); break;
	case SURFTYPE_F: m_plot3DF.Plot(m_pRightWnd); break;
	} // switch
}

// --------------------------------------------------------------
// get info from dialog
void CSurfX3DApp::GetDlg()
{
	if (!IsInitialized()) return;	// don't crash

	int is_data_changed = 0;
	switch (GetSurfType())
	{
	case SURFTYPE_Z:
		is_data_changed = m_pFormZ->IsDataChanged();
		m_pFormZ->GetDlg();
		break;
	case SURFTYPE_P:
		is_data_changed = m_pFormP->IsDataChanged();
		m_pFormP->GetDlg();
		break;
	case SURFTYPE_F:
		is_data_changed = m_pFormF->IsDataChanged();
		m_pFormF->GetDlg();
		break;
	} // switch
	if (is_data_changed)
		m_is_data_unsaved = 1;
}

// --------------------------------------------------------------
void CSurfX3DApp::SetDocTitle(CString surfTitle)
{
	if (!IsInitialized()) return;	// don't crash

	// set document title
	CDocument* pDoc = m_pMainFrame->GetActiveDocument();
	if (pDoc) pDoc->SetTitle(surfTitle);
}

// --------------------------------------------------------------
// returns: 0=ok, 1=error
int CSurfX3DApp::OpenSurface(CString surfName)
{
	// let pass ok to allow command line operation
	if (!IsInitialized()) return(0);

	// remove quoted path for long filenames
	surfName.Remove('"');

	int rc=1;
	if (!DoesFileExist(surfName))
	{
		CString sErr;
		sErr.Format("Surface File %s not found", surfName);
		SetStatusString(sErr);
		return(1);
	}

	CString sExt;
	int len  = surfName.GetLength();
	int indx = surfName.ReverseFind('.');
	if (len<5) return(1);
	if (len>0 && indx>0 && len>indx)
	{
		sExt = surfName.Right(len-indx-1);
	}

	// reset all plots (only one active at a time)
	m_plot3DZ.Reset();
	m_plot3DP.Reset();
	m_plot3DF.Reset();

	SetDocTitle(surfName);
	SetFilePath(surfName);
	if      (sExt.CompareNoCase(EXT_Z)==0)
	{
		m_plot3DZ.SetFilePath(m_sPlotFilePath);
		if (m_plot3DZ.ReadFile()==0)
		{
			SetSurfType(SURFTYPE_Z);
			m_plot3DZ.SetWnd(m_pRightWnd);
			m_plot3DZ.ClearScreen();
			m_pFormZ->LoadDlg();
			m_isPlotOpened = 1;
			rc = 0;
		}
	}
	else if (sExt.CompareNoCase(EXT_F)==0)
	{
		m_plot3DF.SetFilePath(m_sPlotFilePath);
		if (m_plot3DF.ReadFile()==0)
		{
			SetSurfType(SURFTYPE_F);
			m_plot3DF.SetWnd(m_pRightWnd);
			m_plot3DF.ClearScreen();
			m_pFormF->LoadDlg();
			m_isPlotOpened = 1;
			rc = 0;
		}
	}
	else if (sExt.CompareNoCase(EXT_P)==0)
	{
		m_plot3DP.SetFilePath(m_sPlotFilePath);
		if (m_plot3DP.ReadFile()==0)
		{
			SetSurfType(SURFTYPE_P);
			m_plot3DP.SetWnd(m_pRightWnd);
			m_plot3DP.ClearScreen();
			m_pFormP->LoadDlg();
			m_isPlotOpened = 1;
			rc = 0;
		}
	}
	else
	{
		SetSurfType(SURFTYPE_NONE);
	}

	UpdateToolBar();
	ShowSurfForm();
	m_is_data_unsaved = 0;	// clear unsaved flag

	// draw plot if auto draw
	if (m_autoDraw)
		PlotSurface();
	return(rc);
}

// --------------------------------------------------------------
void CSurfX3DApp::SaveUserSettings(void)
{
    if (m_sPlotFilePath.GetLength() < 1) return; // dont save if not set
   	WriteProfileString("Settings","LastSurface", m_sPlotFilePath  );
	WriteProfileString("Settings","PathPOV",     m_sLastSubDirPOV );
	WriteProfileString("Settings","PathVRML",    m_sLastSubDirVRML);
	WriteProfileString("Settings","PathDXF",     m_sLastSubDirDXF );

	WriteProfileInt("Settings","AutoDraw",    m_autoDraw);
	WriteProfileInt("Settings","SurfaceType", m_SurfaceType);
	WriteProfileInt("Settings","WinSizeX",    m_wndSizeX);
	WriteProfileInt("Settings","WinSizeY",    m_wndSizeY);
	WriteProfileInt("Settings","WinX",        m_wndX);
	WriteProfileInt("Settings","WinY",        m_wndY);
	WriteProfileInt("Settings","WinSplitX",   m_wndSplitX);
}

// --------------------------------------------------------------
void CSurfX3DApp::LoadUserSettings(void)
{
	m_sPlotFilePath   = GetProfileString("Settings","LastSurface", "");
	m_sLastSubDirPOV  = GetProfileString("Settings","PathPOV",     "");
	m_sLastSubDirVRML = GetProfileString("Settings","PathVRML",    "");
	m_sLastSubDirDXF  = GetProfileString("Settings","PathDXF",     "");

	m_autoDraw    = GetProfileInt("Settings","AutoDraw",    1);
	m_SurfaceType = GetProfileInt("Settings","SurfaceType", SURFTYPE_P);
	m_wndSizeX    = GetProfileInt("Settings","WinSizeX",    1020);
	m_wndSizeY    = GetProfileInt("Settings","WinSizeY",    800);
	m_wndX        = GetProfileInt("Settings","WinX",        75);
	m_wndY        = GetProfileInt("Settings","WinY",        75);
	m_wndSplitX   = GetProfileInt("Settings","WinSplitX",   350);

    // initialize on first time
    if (m_sPlotFilePath.GetLength() < 1)
    {
        CString sTemp = m_sExeFolder + "\\" EXAMPLES_FOLDER_NAME + "\\" + TREFOIL_EXAMPE_NAME;
        if (DoesFileExist(sTemp))
        {
            m_sPlotFilePath = sTemp;
            m_firstTimePlot = 1;
        }
    }
}

// --------------------------------------------------------------
// get plot's base class
Plot3D* CSurfX3DApp::GetBasePlot()
{
	Plot3D* pPlot = NULL;
	switch (GetSurfType())
	{
	default:
	case SURFTYPE_Z: pPlot = (Plot3D*)&m_plot3DZ;	break;
	case SURFTYPE_P: pPlot = (Plot3D*)&m_plot3DP;	break;
	case SURFTYPE_F: pPlot = (Plot3D*)&m_plot3DF;	break;
	} // switch
	return(pPlot);
}

// --------------------------------------------------------------
// returns: 0=no, 1=plot is rendered
int CSurfX3DApp::IsRendered(void)
{
	return( GetBasePlot()->IsRendered() );
}

// --------------------------------------------------------------
// returns: 0=no, 1=plot is rendering
int CSurfX3DApp::IsRendering(void)
{
	return( GetBasePlot()->IsRendering() );
}

// --------------------------------------------------------------
void CSurfX3DApp::SetFilePath(CString filePath)
{
	m_sPlotFilePath = filePath;
	GetBasePlot()->SetFilePath(filePath);
	AddToRecentFileList(filePath);
}

// --------------------------------------------------------------
CString CSurfX3DApp::GetFilePath(void)
{
	CString fPath;
	if (GetBasePlot())
		fPath = GetBasePlot()->GetFilePath();
	return(fPath);
}

// --------------------------------------------------------------
void CSurfX3DApp::WriteFile()
{
	CWaitCursor hourGlass;
	// get info from dialog
	GetDlg();
	switch (GetSurfType())
	{
	default:
	case SURFTYPE_Z: m_plot3DZ.WriteFile();	break;
	case SURFTYPE_P: m_plot3DP.WriteFile();	break;
	case SURFTYPE_F: m_plot3DF.WriteFile();	break;
	} // switch
	m_is_data_unsaved = 0;
}

// --------------------------------------------------------------
void CSurfX3DApp::NewSurface(int surfType)
{
	SetSurfType(surfType);
	CString sEmpty;
	SetFilePath(sEmpty);	// clear surface file path

	// reset all functions
	m_plot3DZ.Reset();
	m_plot3DP.Reset();
	m_plot3DF.Reset();

	m_is_data_unsaved = 1;  // new surface has not been saved
	switch (surfType)
	{
	case SURFTYPE_Z:
		m_plot3DZ.ClearScreen();
		m_pFormZ->LoadDlg();
		m_isPlotOpened = 1;
		break;
	case SURFTYPE_P:
		m_plot3DP.ClearScreen();
		m_pFormP->LoadDlg();
		m_isPlotOpened = 1;
		break;
	case SURFTYPE_F:
		m_plot3DF.ClearScreen();
		m_pFormF->LoadDlg();
		m_isPlotOpened = 1;
		break;
	}
	ShowSurfForm();

	// draw plot if auto draw
	if (m_autoDraw)
		PlotSurface();
}

// --------------------------------------------------------------
void CSurfX3DApp::Repaint()
{
	Plot3D* pPlot = GetBasePlot();
	if (pPlot && (IsRendered() || IsRendering()))
	{
		pPlot->Paint(1);
	}
}

// --------------------------------------------------------------
void CSurfX3DApp::CopyPlotToClipBoard()
{
	GetBasePlot()->CopyPlotToClipBoard();
}

// --------------------------------------------------------------
void CSurfX3DApp::SetStatusString(CString sStatus)
{
	if (m_pMainFrame)
		m_pMainFrame->m_wndStatusBar.SetPaneText(0,sStatus);
}

// --------------------------------------------------------------
// returns: 0=no, 1=ok to open a surface file
int CSurfX3DApp::IsReadyToOpen()
{
	int rc;

	// not while rendering or printing
	if (IsRendering() || IsPrinting()) return(0);

	// check if current data is saved
	GetDlg();
	if (!IsDataUnsaved()) return(1);

	// ask if want to save current surface
	rc = AfxMessageBox("Save surface parameters?",MB_YESNOCANCEL);
	if (rc == IDYES)
	{
		WriteFile();
		return(1);
	}
	if (rc == IDCANCEL) return(0);
	return(1);
}

// --------------------------------------------------------------
void CSurfX3DApp::OnFileOpen()
{
	#define PARMP "Parametric Surface   x,y,z(u,v)   (*.X3P)|*.X3P|"	// EXT_P
    #define PARMZ "Explicit Surface   z=f(x,y)   (*.X3Z)|*.X3Z|"		// EXT_Z
    #define PARMF "Implicit Surface   f(x,y,z)=0   (*.X3F)|*.X3F|"		// EXT_F
    #define PARMN "|"

	static char* szFilter[4] = 
	{
		PARMP PARMZ PARMF PARMN,	// SURFTYPE_NONE
		PARMP PARMZ PARMF PARMN,	// SURFTYPE_P
		PARMZ PARMP PARMF PARMN,	// SURFTYPE_Z
		PARMF PARMZ PARMP PARMN,	// SURFTYPE_F
	};

	if (!IsInitialized()) return;	// don't crash
	if (!IsReadyToOpen()) return;

	int ifilter = GetSurfType();
	if (ifilter<0 || ifilter>3) ifilter=0;
	char* pszFilter = szFilter[ifilter];
	m_pRightWnd = m_pMainFrame->m_Splitter.GetPane(0, 1);
	CFileDialog dlg(TRUE, m_sDefExt,(LPCTSTR)m_sPlotFilePath,
		OFN_LONGNAMES | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		pszFilter,m_pRightWnd);
	if (dlg.DoModal() != IDOK) return;
	OpenSurface(dlg.GetPathName());
}

// --------------------------------------------------------------
void CSurfX3DApp::OnFileSaveAs()
{
	#define PARMP "Parametric Surface   x,y,z(u,v)   (*.X3P)|*.X3P|"	// EXT_P
    #define PARMZ "Explicit Surface   z=f(x,y)   (*.X3Z)|*.X3Z|"		// EXT_Z
    #define PARMF "Implicit Surface   f(x,y,z)=0   (*.X3F)|*.X3F|"		// EXT_F
    #define PARMN "|"

	static char* szFilter[4] = 
	{
		PARMN,	// SURFTYPE_NONE
		PARMP,	// SURFTYPE_P
		PARMZ,	// SURFTYPE_Z
		PARMF,	// SURFTYPE_F
	};

	int ifilter = GetSurfType();
	if (ifilter<0 || ifilter>3) ifilter=0;
	char* pszFilter = szFilter[ifilter];

	CString sExt = GetSurfaceExt(GetSurfType());
	CFileDialog dlg(FALSE, sExt, (LPCTSTR)m_sPlotFilePath,
		OFN_LONGNAMES | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		pszFilter,m_pRightWnd);
	if (dlg.DoModal() != IDOK) return;

	SetFilePath( dlg.GetPathName() );
	WriteFile();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

// --------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

// --------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------------------------------------------
// App command to run the dialog
void CSurfX3DApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHelpTipsDlg dialog used for App Help Tips

class CHelpTipsDlg : public CDialog
{
public:
	CHelpTipsDlg();

// Dialog Data
	//{{AFX_DATA(CHelpTipsDlg)
	enum { IDD = IDD_TIPS };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpTipsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CHelpTipsDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------
CHelpTipsDlg::CHelpTipsDlg() : CDialog(CHelpTipsDlg::IDD)
{
	//{{AFX_DATA_INIT(CHelpTipsDlg)
	//}}AFX_DATA_INIT
}

// --------------------------------------------------------------
void CHelpTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpTipsDlg)
	//}}AFX_DATA_MAP
}

// --------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHelpTipsDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpTipsDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------------------------------------------
// App command to run the help tips dialog
void CSurfX3DApp::OnHelpTips()
{
	CHelpTipsDlg tipsDlg;
	tipsDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHelpFunctionDlg dialog used for App Help Function

class CHelpFunctionDlg : public CDialog
{
public:
	CHelpFunctionDlg();

// Dialog Data
	//{{AFX_DATA(CHelpFunctionDlg)
	enum { IDD = IDD_FUNCTIONS };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpFunctionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CHelpFunctionDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------
CHelpFunctionDlg::CHelpFunctionDlg() : CDialog(CHelpFunctionDlg::IDD)
{
	//{{AFX_DATA_INIT(CHelpFunctionDlg)
	//}}AFX_DATA_INIT
}

// --------------------------------------------------------------
void CHelpFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpFunctionDlg)
	//}}AFX_DATA_MAP
}

// --------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHelpFunctionDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpFunctionDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------------------------------------------
// App command to run the help functions dialog
void CSurfX3DApp::OnHelpFunctions()
{
	CHelpFunctionDlg functionDlg;
	functionDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHelpOpenHelpDlg dialog used for App Help Function

class CHelpOpenHelpDlg : public CDialog
{
public:
	CHelpOpenHelpDlg();

// Dialog Data
	//{{AFX_DATA(CHelpOpenHelpDlg)
	enum { IDD = IDD_FUNCTIONS };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpOpenHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CHelpOpenHelpDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------
CHelpOpenHelpDlg::CHelpOpenHelpDlg() : CDialog(CHelpOpenHelpDlg::IDD)
{
	//{{AFX_DATA_INIT(CHelpOpenHelpDlg)
	//}}AFX_DATA_INIT
}

// --------------------------------------------------------------
void CHelpOpenHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpOpenHelpDlg)
	//}}AFX_DATA_MAP
}

// --------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHelpOpenHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpOpenHelpDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------------------------------------------
// App command to run the dialog
void CSurfX3DApp::OnHelpOpenHelpFile()
{
    if (m_sHelpFile.GetLength() < 1)
    {
        AfxMessageBox("Can't find '" HELP_FILE_NAME "' help file");
        return;
    }

    // open the file
    ::ShellExecuteA(NULL,"open", m_sHelpFile, NULL, NULL, SW_SHOWMAXIMIZED);
}


/////////////////////////////////////////////////////////////////////////////
// CSurfX3DApp message handlers

CDocument* CSurfX3DApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	CString sFileName = lpszFileName;

	if (IsReadyToOpen())
	{
		if (OpenSurface(sFileName))
			return(NULL);
	}
	
	return CWinApp::OpenDocumentFile(sFileName);
}

// --------------------------------------------------------------
void CSurfX3DApp::SaveOnExit()
{
	if (IsDataUnsaved())
	{
		int rc;
		rc = AfxMessageBox("Save surface parameters before exiting?",MB_YESNO);
		if (rc == IDYES)
			OnFileSaveAs();
		m_is_data_unsaved = 0;	// don't ask twice
	}
}

// --------------------------------------------------------------
int CSurfX3DApp::ExitInstance() 
{
	// too late to call GetDlg()
	SaveOnExit();
	SaveUserSettings();
	return CWinApp::ExitInstance();
}

// --------------------------------------------------------------
void CSurfX3DApp::UpdateToolBar() 
{
	if (m_pMainFrame)
		m_pMainFrame->UpdateToolBar();
}

// --------------------------------------------------------------
void CSurfX3DApp::OnAppExit() 
{
	if (IsRendering())
	{
		AfxMessageBox( "Cancel Plot first" );
		return;
	}
	GetDlg();
	SaveOnExit();
	CWinApp::OnAppExit();
}

// --------------------------------------------------------------
