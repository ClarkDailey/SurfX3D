// DlgNewSurf.cpp : implementation file
//

#include "stdafx.h"
#include "SurfX3D.h"
#include "DlgNewSurf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewSurf dialog


CDlgNewSurf::CDlgNewSurf(CWnd* pParent/*=NULL*/)
: CDialog(CDlgNewSurf::IDD, pParent)

{
	//{{AFX_DATA_INIT(CDlgNewSurf)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNewSurf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewSurf)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewSurf, CDialog)
	//{{AFX_MSG_MAP(CDlgNewSurf)
	ON_BN_CLICKED(IDC_RADIO_SURF_EXPLICIT, OnRadioSurfExplicit)
	ON_BN_CLICKED(IDC_RADIO_SURF_IMPLICIT, OnRadioSurfImplicit)
	ON_BN_CLICKED(IDC_RADIO_SURF_PARAMETRIC, OnRadioSurfParametric)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewSurf message handlers

BOOL CDlgNewSurf::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CButton* pBtn;
	switch(m_SurfaceType)
	{
	default:
	case SURFTYPE_P: pBtn=(CButton*)GetDlgItem(IDC_RADIO_SURF_PARAMETRIC); break;
	case SURFTYPE_Z: pBtn=(CButton*)GetDlgItem(IDC_RADIO_SURF_EXPLICIT  ); break;
	case SURFTYPE_F: pBtn=(CButton*)GetDlgItem(IDC_RADIO_SURF_IMPLICIT  ); break;
	}
	pBtn->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNewSurf::OnRadioSurfExplicit() 
{
	m_SurfaceType = SURFTYPE_Z;
}

void CDlgNewSurf::OnRadioSurfImplicit() 
{
	m_SurfaceType = SURFTYPE_F;
}

void CDlgNewSurf::OnRadioSurfParametric() 
{
	m_SurfaceType = SURFTYPE_P;
}

