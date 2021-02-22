// CPlotSplitter.h: interface for the CPlotSplitter class.
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_CPlotSplitter_H__464C08E9_8989_11D4_B4E3_005004D85AA1__INCLUDED_)
#define AFX_CPlotSplitter_H__464C08E9_8989_11D4_B4E3_005004D85AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <map>
using namespace std;

// --------------------------------------------------------------
class CPlotSplitter : public CSplitterWnd  
{
protected:
	DECLARE_DYNCREATE(CPlotSplitter)

public:

	CPlotSplitter();
	virtual ~CPlotSplitter();
	int AddView(int nRow, int nCol, CRuntimeClass * pViewClass, 
				CCreateContext* pContext);
	void ShowView(int nViewID);
	CWnd* GetView(int nViewID);

protected:
	map<int, long> m_mapViewPane;
	map<long, int> m_mapCurrentViews;
	map<int, CWnd*> m_mapIDViews;

	int m_nIDCounter;

	void SetCurrentView(int nRow, int nCol, int nViewID);
	int HideCurrentView(int nRow, int nCol);
	void GetPaneFromViewID(int nViewID, CPoint * pane);
	CWnd* GetCurrentView(int nRow, int nCol, int * nCurID);

	// Generated message map functions
	//{{AFX_MSG(CFormF)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

#endif // !defined(AFX_CPlotSplitter_H__464C08E9_8989_11D4_B4E3_005004D85AA1__INCLUDED_)
