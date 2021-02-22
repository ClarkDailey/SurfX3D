#if !defined(AFX_FORMN_H__C67C8082_190B_49D9_93E5_5085EBD6349E__INCLUDED_)
#define AFX_FORMN_H__C67C8082_190B_49D9_93E5_5085EBD6349E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormN.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormN form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormN : public CFormView
{
protected:
	CFormN();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormN)

public:
	void LoadDlg(void);

// Form Data
public:
	//{{AFX_DATA(CFormN)
	enum { IDD = IDD_FORMN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormN();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormN)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMN_H__C67C8082_190B_49D9_93E5_5085EBD6349E__INCLUDED_)
