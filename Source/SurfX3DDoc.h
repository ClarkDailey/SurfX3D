// SurfX3DDoc.h : interface of the CSurfX3DDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURFX3DDOC_H__ADEBB0B0_5EBA_4E56_B981_2D1C4EF4BB06__INCLUDED_)
#define AFX_SURFX3DDOC_H__ADEBB0B0_5EBA_4E56_B981_2D1C4EF4BB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSurfX3DDoc : public CDocument
{
protected: // create from serialization only
	CSurfX3DDoc();
	DECLARE_DYNCREATE(CSurfX3DDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurfX3DDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSurfX3DDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSurfX3DDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SURFX3DDOC_H__ADEBB0B0_5EBA_4E56_B981_2D1C4EF4BB06__INCLUDED_)
