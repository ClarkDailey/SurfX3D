// CPlotSplitter.cpp: implementation of the CPlotSplitter class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PlotSplitter.h"
#include "SurfX3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPlotSplitter, CSplitterWnd)

CPlotSplitter::CPlotSplitter()
{
	m_nIDCounter = 1;
}

// --------------------------------------------------------------
CPlotSplitter::~CPlotSplitter()
{

}

// --------------------------------------------------------------
int CPlotSplitter::AddView(int nRow, int nCol, CRuntimeClass * pViewClass, 
								   CCreateContext* pContext)
{
	// hide the current view of the pane
	int PreviousID = HideCurrentView(nRow, nCol);

	// create the new view, if fail, set the previous view current 
	if (CreateView(nRow, nCol, pViewClass, CSize(10,10), pContext) == 0)
	{
		if (PreviousID != -1)
			SetCurrentView(nRow, nCol, PreviousID);
		return -1;
	}

	// get and store the new view
	int NewViewID = m_nIDCounter;
	CWnd* pNewWnd = GetPane(nRow, nCol);
	CPoint pane(nRow, nCol);
	long paneID = MAKELONG(pane.x,pane.y);
	m_mapViewPane.insert(map<int, long>::value_type(NewViewID,paneID));
	m_mapIDViews.insert(map<int, CWnd*>::value_type(NewViewID, pNewWnd));

	// set the new view current
	SetCurrentView(nRow, nCol, NewViewID);

	RedrawWindow();
	m_nIDCounter ++;
	return NewViewID;
}

// --------------------------------------------------------------
void CPlotSplitter::ShowView(int nViewID)
{
	if (GetView(nViewID) == NULL)
		return;

	// find the pane containing the view 
	CPoint pane;
	GetPaneFromViewID(nViewID, &pane);

	// switch views
	HideCurrentView(pane.x, pane.y);
	SetCurrentView(pane.x, pane.y, nViewID);

	RecalcLayout();
}

// --------------------------------------------------------------
CWnd* CPlotSplitter::GetView(int nViewID)
{
	map<int, CWnd*>::iterator itView;

	itView = m_mapIDViews.find(nViewID);
	if(itView==m_mapIDViews.end())
		return NULL;
	else
		return (*itView).second;
}

// --------------------------------------------------------------
CWnd* CPlotSplitter::GetCurrentView(int nRow, int nCol, int * nCurID)
{
	long paneID = MAKELONG(nRow,nCol);
	
	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur == m_mapCurrentViews.end())
		return NULL;
	else
	{
		int PreviousID = (*itCur).second;
		*nCurID = PreviousID;
		return GetView(PreviousID);
	}
}

// --------------------------------------------------------------
void CPlotSplitter::SetCurrentView(int nRow, int nCol, int nViewID)
{
	long paneID = MAKELONG(nRow,nCol);

	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur != m_mapCurrentViews.end())
		(*itCur).second = nViewID;
	else
		m_mapCurrentViews.insert(map<long,int>::value_type(paneID,nViewID));

	CWnd * pView = GetView(nViewID);
	pView->SetDlgCtrlID(IdFromRowCol(nRow, nCol));
	pView->ShowWindow(SW_SHOW);
}

// --------------------------------------------------------------
int CPlotSplitter::HideCurrentView(int nRow, int nCol)
{
	int prevID;
	CWnd * pCurView = GetCurrentView(nRow, nCol, &prevID);
	if (pCurView == NULL)
		return -1;
	else
	{
		pCurView->SetDlgCtrlID(0);
		pCurView->ShowWindow(SW_HIDE);	
		return prevID;
	}
}

// --------------------------------------------------------------
void CPlotSplitter::GetPaneFromViewID(int nViewID, CPoint * pane)
{
	map<int, long>::iterator itPane;
	itPane = m_mapViewPane.find(nViewID);
	if (itPane==m_mapViewPane.end())
	{
		pane = NULL;
		return;
	}
	long paneID = (*itPane).second;
	CPoint p(paneID);
	pane->x = p.x;
	pane->y = p.y;
}

// --------------------------------------------------------------
void CPlotSplitter::OnSize(UINT nType, int cx, int cy) 
{
	theApp.UpdateMainWndSize();
}

// --------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPlotSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CPlotSplitter)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --------------------------------------------------------------
