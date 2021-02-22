// SurfX3DView.h : interface of the CSurfX3DView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURFX3DVIEW_H__EEBE50A5_1EE8_442A_9E94_9D50A2435A30__INCLUDED_)
#define AFX_SURFX3DVIEW_H__EEBE50A5_1EE8_442A_9E94_9D50A2435A30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSurfX3DView : public CView
{
protected: // create from serialization only
	CSurfX3DView();
	DECLARE_DYNCREATE(CSurfX3DView)

// Attributes
public:
	CSurfX3DDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurfX3DView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSurfX3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT m_last_mouse;	// last mouse flags

// Generated message map functions
protected:
	//{{AFX_MSG(CSurfX3DView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in SurfX3DView.cpp
inline CSurfX3DDoc* CSurfX3DView::GetDocument()
   { return (CSurfX3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SURFX3DVIEW_H__EEBE50A5_1EE8_442A_9E94_9D50A2435A30__INCLUDED_)
