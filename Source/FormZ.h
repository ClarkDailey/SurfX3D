#if !defined(AFX_FORMZ_H__67E67D20_FF18_4856_A281_8CFDEA8BD72C__INCLUDED_)
#define AFX_FORMZ_H__67E67D20_FF18_4856_A281_8CFDEA8BD72C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormZ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormZ form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormZ : public CFormView
{
protected:
	CFormZ();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormZ)

public:
	void LoadDlg(void);
	void GetDlg(void);
	int  IsDataChanged(void);

	CToolTipCtrl* m_pTipCtrl;

// Form Data
public:
	//{{AFX_DATA(CFormZ)
	enum { IDD = IDD_FORMZ };
	CString	m_sSurfaceName;
	CString	m_zPlotEquation;
	CString	m_rPlotEquation;
	CString	m_gPlotEquation;
	CString	m_bPlotEquation;
	CString	m_sC1;
	CString	m_sC2;
	CString	m_sC3;
	CString	m_sC4;
	CString	m_sC5;
	CString	m_sC6;
	CString	m_sC7;
	CString	m_sC8;
	double	m_scale;
	double	m_xmax;
	double	m_xmin;
	double	m_ymax;
	double	m_ymin;
	double	m_zmax;
	double	m_zmin;
	double	m_zrotate;
	double	m_xytilt;
	UINT	m_nulines;
	UINT	m_nvlines;
	BOOL	m_draw_ulines;
	BOOL	m_draw_vlines;
	BOOL	m_is_color;
	BOOL	m_show_axis;
	BOOL	m_limit_z;
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
	//{{AFX_VIRTUAL(CFormZ)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormZ();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormZ)
	afx_msg void OnBtnBackColor();
	afx_msg void OnBtnLineColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMZ_H__67E67D20_FF18_4856_A281_8CFDEA8BD72C__INCLUDED_)
