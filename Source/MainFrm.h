// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A7C91ADA_2453_414A_9034_B939DB9D52B6__INCLUDED_)
#define AFX_MAINFRM_H__A7C91ADA_2453_414A_9034_B939DB9D52B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlotSplitter.h"


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CPlotSplitter m_Splitter;
	int m_ViewID[4];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	void UpdateToolBar(void);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// status bar
	CStatusBar  m_wndStatusBar;

// Operations
protected:
	CString CheckFileExt(CString fileName,CString extName);
	CString MakeDefaultName(CString fullName,CString destPath,CString extName);

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnPlotAutodraw();
	afx_msg void OnPlotDrawplot();
	afx_msg void OnUpdatePlotAutodraw(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlotDrawplot(CCmdUI* pCmdUI);
	afx_msg void OnExportPov();
	afx_msg void OnUpdateExportPov(CCmdUI* pCmdUI);
	afx_msg void OnExportVrml();
	afx_msg void OnUpdateExportVrml(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPlotCancel();
	afx_msg void OnUpdatePlotCancel(CCmdUI* pCmdUI);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintSetup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnExportDxf();
	afx_msg void OnExportStl();
	afx_msg void OnUpdateExportDxf(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExportStl(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnBtnLockLr();
	afx_msg void OnUpdateBtnLockLr(CCmdUI* pCmdUI);
	afx_msg void OnBtnLockUd();
	afx_msg void OnUpdateBtnLockUd(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A7C91ADA_2453_414A_9034_B939DB9D52B6__INCLUDED_)
