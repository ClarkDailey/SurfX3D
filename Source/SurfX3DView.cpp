// SurfX3DView.cpp : implementation of the CSurfX3DView class
//

#include "stdafx.h"
#include "SurfX3D.h"

#include "SurfX3DDoc.h"
#include "SurfX3DView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DView

IMPLEMENT_DYNCREATE(CSurfX3DView, CView)

BEGIN_MESSAGE_MAP(CSurfX3DView, CView)
	//{{AFX_MSG_MAP(CSurfX3DView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DView construction/destruction

CSurfX3DView::CSurfX3DView()
{
	theApp.m_pRightView = this;
	m_last_mouse = 0;
}

CSurfX3DView::~CSurfX3DView()
{
}

BOOL CSurfX3DView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// Required for OpenGL (don't use CS_PARENTDC)
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DView printing

BOOL CSurfX3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CSurfX3DView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	pInfo->SetMaxPage(1);
	theApp.m_is_printing = pInfo->m_bPreview ? 2 : 1;
	CView::OnBeginPrinting(pDC, pInfo);
}

void CSurfX3DView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);
	theApp.m_is_printing   = 0;
}

void CSurfX3DView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	theApp.m_is_printing   = 0;
}

void CSurfX3DView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	Plot3D* pPlot = theApp.GetBasePlot();
	if (pPlot)
		pPlot->DoPrint(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DView diagnostics

#ifdef _DEBUG
void CSurfX3DView::AssertValid() const
{
	CView::AssertValid();
}

void CSurfX3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSurfX3DDoc* CSurfX3DView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSurfX3DDoc)));
	return (CSurfX3DDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSurfX3DView message handlers

void CSurfX3DView::OnDraw(CDC* pDC)
{
	theApp.Repaint();
}

void CSurfX3DView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	theApp.Repaint();
	
	// Do not call CView::OnPaint() for painting messages
}

void CSurfX3DView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	theApp.UpdateMainWndSize();
	theApp.Repaint();
}


BOOL CSurfX3DView::OnEraseBkgnd(CDC* pDC) 
{
	if (theApp.IsRendered())
		return(FALSE);	// Open GL handles this
	return CView::OnEraseBkgnd(pDC);
}

void CSurfX3DView::OnMouseMove(UINT nFlags, CPoint point) 
{
	Plot3D* pPlot = theApp.GetBasePlot();
	if (pPlot)
	{
		if (pPlot->IsRendered())
		{
			if (nFlags & MK_LBUTTON)
			{
				pPlot->SetMousePos(point.x,point.y,((m_last_mouse & MK_LBUTTON)==0));
				pPlot->GetWnd()->Invalidate();
			}
		}
	}
	m_last_mouse = nFlags;
	
	CView::OnMouseMove(nFlags, point);
}

BOOL CSurfX3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	Plot3D* pPlot = theApp.GetBasePlot();
	if (pPlot)
	{
		if (pPlot->IsRendered())
		{
			if (zDelta<0)
				pPlot->ZoomIn();
			else
				pPlot->ZoomOut();
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

