#if !defined(AFX_FORMP_H__950E9B4F_322C_448A_AB9D_5B9B6C27F645__INCLUDED_)
#define AFX_FORMP_H__950E9B4F_322C_448A_AB9D_5B9B6C27F645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormP form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormP : public CFormView
{
protected:
	CFormP();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormP)

public:
	void LoadDlg(void);
	void GetDlg(void);
	int  IsDataChanged(void);

	CToolTipCtrl* m_pTipCtrl;

// Form Data
public:
	//{{AFX_DATA(CFormP)
	enum { IDD = IDD_FORMP };
	CString	m_xPlotEquation;
	CString	m_yPlotEquation;
	CString	m_zPlotEquation;
	CString	m_bPlotEquation;
	CString	m_gPlotEquation;
	CString	m_rPlotEquation;
	CString	m_sC1;
	CString	m_sC2;
	CString	m_sC3;
	CString	m_sC4;
	CString	m_sC5;
	CString	m_sC6;
	CString	m_sC7;
	CString	m_sC8;
	UINT	m_nulines;
	UINT	m_nvlines;
	double	m_umax;
	double	m_umin;
	double	m_vmax;
	double	m_vmin;
	BOOL	m_draw_ulines;
	BOOL	m_draw_vlines;
	BOOL	m_show_axis;
	BOOL	m_is_color;
	double	m_scale;
	double	m_xytilt;
	double	m_zrotate;
	CString	m_sSurfaceName;
	UINT	m_nxMesh;
	UINT	m_nyMesh;
	//}}AFX_DATA

// Attributes
public:
	COLORREF m_line_color;
	COLORREF m_back_color;

// Operations
public:
	void UpdateForm(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormP)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormP();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormP)
	afx_msg void OnBtnBackColor();
	afx_msg void OnBtnLineColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMP_H__950E9B4F_322C_448A_AB9D_5B9B6C27F645__INCLUDED_)
