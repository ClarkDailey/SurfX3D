// FormN.cpp : implementation file
//

#include "stdafx.h"
#include "SurfX3D.h"
#include "FormN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormN

IMPLEMENT_DYNCREATE(CFormN, CFormView)

CFormN::CFormN()
	: CFormView(CFormN::IDD)
{
	//{{AFX_DATA_INIT(CFormN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFormN::~CFormN()
{
}

void CFormN::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormN, CFormView)
	//{{AFX_MSG_MAP(CFormN)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormN diagnostics

#ifdef _DEBUG
void CFormN::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormN::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormN message handlers
