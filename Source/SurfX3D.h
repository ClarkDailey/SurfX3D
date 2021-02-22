// SurfX3D.h : main header file for the SURFX3D application
//

#if !defined(AFX_SURFX3D_H__4163AC4E_8D6B_452D_907F_E582EA1AC713__INCLUDED_)
#define AFX_SURFX3D_H__4163AC4E_8D6B_452D_907F_E582EA1AC713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "mainfrm.h"
#include "resource.h"       // main symbols

#include "FormN.h"
#include "FormZ.h"
#include "FormF.h"
#include "FormP.h"
#include "Plot3D.h"
#include "Plot3DZ.h"
#include "Plot3DF.h"
#include "Plot3DP.h"


// Surface Types
#define SURFTYPE_NONE	0	// none
#define SURFTYPE_P		1	// parametric
#define SURFTYPE_Z		2	// explicit
#define SURFTYPE_F		3	// implicit

// File Extensions
#define EXT_P   "X3P"		// parametric
#define EXT_Z   "X3Z"		// explicit  
#define EXT_F   "X3F"		// implicit  

// Export File Extensions
#define	DXF_FILE_EXTENSION		"dxf"
#define	VRML_FILE_EXTENSION		"wrl"
#define	POV_FILE_EXTENSION		"pov"
#define	STL_FILE_EXTENSION		"stl"

// Help File
#define HELP_FILE_NAME          "SurfX3D.pdf"

// Examples
#define EXAMPLES_FOLDER_NAME    "examples"
#define TREFOIL_EXAMPE_NAME     "Trefoil2.x3p"

// --------------------------------------------------------------
_inline CString GetSurfaceExt(int SurfType)
{
	CString sExt = "";
	switch (SurfType)
	{
	case SURFTYPE_Z: sExt = EXT_Z; break;
	case SURFTYPE_F: sExt = EXT_F; break;
	case SURFTYPE_P: sExt = EXT_P; break;
	}
	return(sExt);
}

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DApp:
// See SurfX3D.cpp for the implementation of this class
//

class CSurfX3DApp : public CWinApp
{
public:
	CSurfX3DApp();
	int  IsInitialized(void) { return(m_pMainFrame?1:0); }
	void ShowSurfForm(void);
	void UpdateMainWndSize(void);
	void PlotSurface(void);
	void Repaint(void);
	void SetSurfType(int surfType);
	int  GetSurfType(void) { return(m_SurfaceType); }
	void NewSurface(int surfType);
	int  OpenSurface(CString surfName);
	void CopyPlotToClipBoard(void);
	void OnFileNew(void);
	void OnFileOpen(void);
	void OnFileSaveAs(void);
	void SetDocTitle(CString surfTitle);
	void SetFilePath(CString filePath);
	CString GetFilePath(void);
	void WriteFile(void);
	void GetDlg(void);
	void SetStatusString(CString sStatus);
	void SaveUserSettings(void);
	void SaveOnExit(void);
	void LoadUserSettings(void);
	Plot3D* GetBasePlot(void);
	void UpdateToolBar(void);
	int  IsReadyToOpen(void);
	int  IsPlotOpened(void)  { return(m_isPlotOpened); }
	int  IsDataUnsaved(void) { return(m_is_data_unsaved); }
	int  IsRendered(void);
	int  IsRendering(void);
	int  IsPrinting(void)   { return(m_is_printing  ); }
	int  IsPreviewing(void) { return(m_is_printing==2?1:0); }
	void ShowRotAngles(double xyAngle,double zAngle);

public:
	CMainFrame* m_pMainFrame;
	CString m_sPlotFilePath;// last plot file
	CString m_sDefExt;	   	// default plot extension
    int m_firstTimePlot;    // 0=no, 1=first time plot
	int m_SurfaceType;		// SURFTYPE_xx
	int m_isPlotOpened;		// 0=no, 1=yes
	int m_isInitialized;	// 0=no, 1=app is initialized
	int m_is_data_unsaved; 	// 0=no, 1=data has been changed but not saved
	int m_is_printing;		// 0=no, 1=printing in progress, 2=preview in progress
	int m_autoDraw;			// 0=no, 1=draw after opening
	int m_wndX;				// x loc of mainfreme window
	int m_wndY;				// y loc of mainframe window
	int m_wndSizeX;			// size of mainframe window
	int m_wndSizeY;			// size of mainframe window
	int m_wndSplitX;		// width of splitter window
	int m_lockLeftRight;	// 0=no, 1=lock movement of z-rotation via mouse
	int m_lockUpDown;		// 0=no, 1=lock movement of xy-tilt via mouse
	Plot3DZ	 m_plot3DZ;	// explicit plot
	Plot3DF	 m_plot3DF;	// implicit plot
	Plot3DP	 m_plot3DP;	// parametric plot

	// pointer to dialog forms
	CFormN*	 m_pFormN;
	CFormZ*	 m_pFormZ;
	CFormF*	 m_pFormF;
	CFormP*	 m_pFormP;
	CView*   m_pRightView;
	CWnd*    m_pRightWnd;

	// subdirectories
    CString  m_sExeFullPath;    // SurfX3D.exe full path
    CString  m_sExeFolder;      // SurfX3D.exe folder (no trailing \)
    CString  m_sHelpFile;       // full path of SurfX3D.pdf
	CString  m_sLastSubDirPOV;	// POV files
	CString  m_sLastSubDirVRML;	// VRML files
	CString  m_sLastSubDirDXF;	// DXF files
	CString  m_sLastSubDirSTL;	// STL files

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurfX3DApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSurfX3DApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpTips();
	afx_msg void OnHelpFunctions();
	afx_msg void OnHelpOpenHelpFile();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern	CSurfX3DApp theApp;	// access the windows application

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SURFX3D_H__4163AC4E_8D6B_452D_907F_E582EA1AC713__INCLUDED_)
