#if !defined(AFX_FORMF_H__20750324_C9CE_4FCD_A04B_6375E6E058C6__INCLUDED_)
#define AFX_FORMF_H__20750324_C9CE_4FCD_A04B_6375E6E058C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormF form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormF : public CFormView
{
protected:
	CFormF();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormF)

public:
	void LoadDlg(void);
	void GetDlg(void);
	int  IsDataChanged(void);

	CToolTipCtrl* m_pTipCtrl;

// Form Data
public:
	//{{AFX_DATA(CFormF)
	enum { IDD = IDD_FORMF };
	CString	m_sC1;
	CString	m_sC2;
	CString	m_sC3;
	CString	m_sC4;
	CString	m_sC5;
	CString	m_sC6;
	CString	m_sC7;
	CString	m_sC8;
	CString	m_sBEquation;
	CString	m_sGEquation;
	CString	m_sREquation;
	CString	m_sFuncEquation;
	double	m_scale;
	BOOL	m_show_axis;
	BOOL	m_is_color;
	double	m_xmax;
	double	m_xmin;
	double	m_xytilt;
	double	m_ymax;
	double	m_ymin;
	double	m_zmax;
	double	m_zmin;
	double	m_zrotate;
	CString	m_sSurfaceName;
	UINT	m_nxplanes;
	UINT	m_nyplanes;
	UINT	m_nzplanes;
	BOOL	m_draw_xlines;
	BOOL	m_draw_ylines;
	BOOL	m_draw_zlines;
	UINT	m_nxMesh;
	UINT	m_nyMesh;
	UINT	m_nzMesh;
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
	//{{AFX_VIRTUAL(CFormF)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormF();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormF)
	afx_msg void OnBtnBackColor();
	afx_msg void OnBtnLineColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMF_H__20750324_C9CE_4FCD_A04B_6375E6E058C6__INCLUDED_)
