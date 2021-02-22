#if !defined(AFX_DLGNEWSURF_H__01A1CF6E_FA46_4C03_8D2A_2C6DDF145CAE__INCLUDED_)
#define AFX_DLGNEWSURF_H__01A1CF6E_FA46_4C03_8D2A_2C6DDF145CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewSurf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewSurf dialog

class CDlgNewSurf : public CDialog
{
// Construction
public:
	CDlgNewSurf(CWnd* pParent = NULL);   // standard constructor

public: // data
	int m_SurfaceType;

// Dialog Data
	//{{AFX_DATA(CDlgNewSurf)
	enum { IDD = IDD_SURF_TYPE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewSurf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewSurf)
	afx_msg void OnRadioSurfExplicit();
	afx_msg void OnRadioSurfImplicit();
	afx_msg void OnRadioSurfParametric();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWSURF_H__01A1CF6E_FA46_4C03_8D2A_2C6DDF145CAE__INCLUDED_)
