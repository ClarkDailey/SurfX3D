// FormP.cpp : implementation file
//

#include "stdafx.h"
#include "SurfX3D.h"
#include "FormP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormP

IMPLEMENT_DYNCREATE(CFormP, CFormView)

CFormP::CFormP()
	: CFormView(CFormP::IDD)
{
	//{{AFX_DATA_INIT(CFormP)
	m_xPlotEquation = _T("");
	m_yPlotEquation = _T("");
	m_zPlotEquation = _T("");
	m_bPlotEquation = _T("");
	m_gPlotEquation = _T("");
	m_rPlotEquation = _T("");
	m_sC1 = _T("");
	m_sC2 = _T("");
	m_sC3 = _T("");
	m_sC4 = _T("");
	m_sC5 = _T("");
	m_sC6 = _T("");
	m_sC7 = _T("");
	m_sC8 = _T("");
	m_nulines = 0;
	m_nvlines = 0;
	m_umax = 0.0;
	m_umin = 0.0;
	m_vmax = 0.0;
	m_vmin = 0.0;
	m_draw_ulines = FALSE;
	m_draw_vlines = FALSE;
	m_show_axis = FALSE;
	m_is_color = FALSE;
	m_scale = 100.0;
	m_xytilt = 0.0;
	m_zrotate = 0.0;
	m_sSurfaceName = _T("");
	m_nxMesh = DEFAULT_MESH_SIZE;
	m_nyMesh = DEFAULT_MESH_SIZE;
	//}}AFX_DATA_INIT
	m_pTipCtrl = 0;
}

CFormP::~CFormP()
{
	delete m_pTipCtrl;
}

void CFormP::LoadDlg()
{
	m_sSurfaceName  = theApp.m_plot3DP.m_sSurfaceName;
	m_xPlotEquation = theApp.m_plot3DP.m_xPlotEquation;
	m_yPlotEquation = theApp.m_plot3DP.m_yPlotEquation;
	m_zPlotEquation = theApp.m_plot3DP.m_zPlotEquation;
	m_rPlotEquation = theApp.m_plot3DP.m_rPlotEquation;
	m_gPlotEquation = theApp.m_plot3DP.m_gPlotEquation;
	m_bPlotEquation = theApp.m_plot3DP.m_bPlotEquation;
	m_sC1           = theApp.m_plot3DP.m_sC1;
	m_sC2           = theApp.m_plot3DP.m_sC2;
	m_sC3           = theApp.m_plot3DP.m_sC3;
	m_sC4           = theApp.m_plot3DP.m_sC4;
	m_sC5           = theApp.m_plot3DP.m_sC5;
	m_sC6           = theApp.m_plot3DP.m_sC6;
	m_sC7           = theApp.m_plot3DP.m_sC7;
	m_sC8           = theApp.m_plot3DP.m_sC8;

	// common parameters
	m_zrotate       = theApp.m_plot3DP.m_zrotate;      
	m_xytilt        = theApp.m_plot3DP.m_xytilt;       
	m_scale         = theApp.m_plot3DP.m_scale;        
	m_line_color    = theApp.m_plot3DP.m_line_color;   
	m_back_color    = theApp.m_plot3DP.m_back_color;   
	m_is_color      = theApp.m_plot3DP.m_is_color;     
	m_show_axis     = theApp.m_plot3DP.m_show_axis;    

	// specific parameters
	m_umin          = theApp.m_plot3DP.m_umin;       
	m_umax          = theApp.m_plot3DP.m_umax;       
	m_vmin          = theApp.m_plot3DP.m_vmin;       
 	m_vmax          = theApp.m_plot3DP.m_vmax;       
	m_nulines       = theApp.m_plot3DP.m_nulines;    
	m_nvlines       = theApp.m_plot3DP.m_nvlines;    
	m_draw_ulines   = theApp.m_plot3DP.m_draw_ulines;
	m_draw_vlines   = theApp.m_plot3DP.m_draw_vlines;
	m_nxMesh        = theApp.m_plot3DP.m_nxMesh;    
	m_nyMesh        = theApp.m_plot3DP.m_nyMesh;    
	UpdateData(FALSE);
}

void CFormP::GetDlg()
{
	UpdateData(TRUE);
	theApp.m_plot3DP.m_sSurfaceName  = m_sSurfaceName ;
	theApp.m_plot3DP.m_xPlotEquation = m_xPlotEquation;
	theApp.m_plot3DP.m_yPlotEquation = m_yPlotEquation;
	theApp.m_plot3DP.m_zPlotEquation = m_zPlotEquation;
	theApp.m_plot3DP.m_rPlotEquation = m_rPlotEquation;
	theApp.m_plot3DP.m_gPlotEquation = m_gPlotEquation;
	theApp.m_plot3DP.m_bPlotEquation = m_bPlotEquation;
	theApp.m_plot3DP.m_sC1           = m_sC1          ;
	theApp.m_plot3DP.m_sC2           = m_sC2          ;
	theApp.m_plot3DP.m_sC3           = m_sC3          ;
	theApp.m_plot3DP.m_sC4           = m_sC4          ;
	theApp.m_plot3DP.m_sC5           = m_sC5          ;
	theApp.m_plot3DP.m_sC6           = m_sC6          ;
	theApp.m_plot3DP.m_sC7           = m_sC7          ;
	theApp.m_plot3DP.m_sC8           = m_sC8          ;

	// common parameters
	theApp.m_plot3DP.m_zrotate       = m_zrotate      ;      
	theApp.m_plot3DP.m_xytilt        = m_xytilt       ;       
	theApp.m_plot3DP.m_scale         = m_scale        ;        
	theApp.m_plot3DP.m_line_color    = m_line_color   ;   
	theApp.m_plot3DP.m_back_color    = m_back_color   ;   
	theApp.m_plot3DP.m_is_color      = m_is_color     ;     
	theApp.m_plot3DP.m_show_axis     = m_show_axis    ;    

	// specific parameters
	theApp.m_plot3DP.m_umin        = m_umin       ;       
	theApp.m_plot3DP.m_umax        = m_umax       ;       
	theApp.m_plot3DP.m_vmin        = m_vmin       ;       
 	theApp.m_plot3DP.m_vmax        = m_vmax       ;       
	theApp.m_plot3DP.m_nulines     = m_nulines    ;    
	theApp.m_plot3DP.m_nvlines     = m_nvlines    ;    
	theApp.m_plot3DP.m_draw_ulines = m_draw_ulines;
	theApp.m_plot3DP.m_draw_vlines = m_draw_vlines;
	theApp.m_plot3DP.m_nxMesh      = m_nxMesh     ;    
	theApp.m_plot3DP.m_nyMesh      = m_nyMesh     ;    
}

int CFormP::IsDataChanged()
{
	UpdateData(TRUE);
	if (theApp.m_plot3DP.m_sSurfaceName  != m_sSurfaceName ) return(1);
	if (theApp.m_plot3DP.m_xPlotEquation != m_xPlotEquation) return(1);
	if (theApp.m_plot3DP.m_yPlotEquation != m_yPlotEquation) return(1);
	if (theApp.m_plot3DP.m_zPlotEquation != m_zPlotEquation) return(1);
	if (theApp.m_plot3DP.m_rPlotEquation != m_rPlotEquation) return(1);
	if (theApp.m_plot3DP.m_gPlotEquation != m_gPlotEquation) return(1);
	if (theApp.m_plot3DP.m_bPlotEquation != m_bPlotEquation) return(1);
	if (theApp.m_plot3DP.m_sC1           != m_sC1          ) return(1);
	if (theApp.m_plot3DP.m_sC2           != m_sC2          ) return(1);
	if (theApp.m_plot3DP.m_sC3           != m_sC3          ) return(1);
	if (theApp.m_plot3DP.m_sC4           != m_sC4          ) return(1);
	if (theApp.m_plot3DP.m_sC5           != m_sC5          ) return(1);
	if (theApp.m_plot3DP.m_sC6           != m_sC6          ) return(1);
	if (theApp.m_plot3DP.m_sC7           != m_sC7          ) return(1);
	if (theApp.m_plot3DP.m_sC8           != m_sC8          ) return(1);

	// common parameters
	if (theApp.m_plot3DP.m_zrotate       != m_zrotate      ) return(1);      
	if (theApp.m_plot3DP.m_xytilt        != m_xytilt       ) return(1);       
	if (theApp.m_plot3DP.m_scale         != m_scale        ) return(1);        
	if (theApp.m_plot3DP.m_line_color    != m_line_color   ) return(1);   
	if (theApp.m_plot3DP.m_back_color    != m_back_color   ) return(1);   
	if (theApp.m_plot3DP.m_is_color      != m_is_color     ) return(1);     
	if (theApp.m_plot3DP.m_show_axis     != m_show_axis    ) return(1);    

	// specific parameters
	if (theApp.m_plot3DP.m_umin        != m_umin       ) return(1);       
	if (theApp.m_plot3DP.m_umax        != m_umax       ) return(1);       
	if (theApp.m_plot3DP.m_vmin        != m_vmin       ) return(1);       
 	if (theApp.m_plot3DP.m_vmax        != m_vmax       ) return(1);       
	if (theApp.m_plot3DP.m_nulines     != m_nulines    ) return(1);    
	if (theApp.m_plot3DP.m_nvlines     != m_nvlines    ) return(1);    
	if (theApp.m_plot3DP.m_draw_ulines != m_draw_ulines) return(1);
	if (theApp.m_plot3DP.m_draw_vlines != m_draw_vlines) return(1);
	if (theApp.m_plot3DP.m_nxMesh      != m_nxMesh     ) return(1);    
	if (theApp.m_plot3DP.m_nyMesh      != m_nyMesh     ) return(1);    
	return(0);
}

void CFormP::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormP)
	DDX_Text(pDX, IDC_EDIT_XPLOT_EQUATION, m_xPlotEquation);
	DDX_Text(pDX, IDC_EDIT_YPLOT_EQUATION, m_yPlotEquation);
	DDX_Text(pDX, IDC_EDIT_ZPLOT_EQUATION, m_zPlotEquation);
	DDX_Text(pDX, IDC_EDIT_BLUE_EQUATION, m_bPlotEquation);
	DDX_Text(pDX, IDC_EDIT_GREEN_EQUATION, m_gPlotEquation);
	DDX_Text(pDX, IDC_EDIT_RED_EQUATION, m_rPlotEquation);
	DDX_Text(pDX, IDC_EDIT_C1, m_sC1);
	DDX_Text(pDX, IDC_EDIT_C2, m_sC2);
	DDX_Text(pDX, IDC_EDIT_C3, m_sC3);
	DDX_Text(pDX, IDC_EDIT_C4, m_sC4);
	DDX_Text(pDX, IDC_EDIT_C5, m_sC5);
	DDX_Text(pDX, IDC_EDIT_C6, m_sC6);
	DDX_Text(pDX, IDC_EDIT_C7, m_sC7);
	DDX_Text(pDX, IDC_EDIT_C8, m_sC8);
	DDX_Text(pDX, IDC_EDIT_ULINES, m_nulines);
	DDX_Text(pDX, IDC_EDIT_VLINES, m_nvlines);
	DDX_Text(pDX, IDC_EDIT_UMAX, m_umax);
	DDX_Text(pDX, IDC_EDIT_UMIN, m_umin);
	DDX_Text(pDX, IDC_EDIT_VMAX, m_vmax);
	DDX_Text(pDX, IDC_EDIT_VMIN, m_vmin);
	DDX_Check(pDX, IDC_CHECK_DRAW_ULINES, m_draw_ulines);
	DDX_Check(pDX, IDC_CHECK_DRAW_VLINES, m_draw_vlines);
	DDX_Check(pDX, IDC_CHECK_SHOW_AXIS, m_show_axis);
	DDX_Check(pDX, IDC_CHECK_SHOW_COLOR, m_is_color);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_scale);
	DDX_Text(pDX, IDC_EDIT_XYTILT, m_xytilt);
	DDX_Text(pDX, IDC_EDIT_ZROTATE, m_zrotate);
	DDX_Text(pDX, IDC_EDIT_SURFNAME, m_sSurfaceName);
	DDX_Text(pDX, IDC_EDIT_NX_MESH, m_nxMesh);
	DDX_Text(pDX, IDC_EDIT_NY_MESH, m_nyMesh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormP, CFormView)
	//{{AFX_MSG_MAP(CFormP)
	ON_BN_CLICKED(IDC_BTN_BACK_COLOR, OnBtnBackColor)
	ON_BN_CLICKED(IDC_BTN_LINE_COLOR, OnBtnLineColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormP diagnostics

#ifdef _DEBUG
void CFormP::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormP::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormP message handlers

void CFormP::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	LoadDlg();
}

// ------------------------------------------------------
void CFormP::UpdateForm()
{
	// set color buttons
	CWnd* pWnd;
	CDC* pDC;
	RECT rect;
	pWnd = (CWnd *)GetDlgItem(IDC_STATIC_FG);
	if (pWnd)
	{
		pWnd->GetClientRect(&rect);
		pDC = pWnd->GetDC();
		if (pDC)
		{
			pDC->FillSolidRect(&rect,m_line_color);
			ReleaseDC(pDC);
		}
		pWnd->Invalidate();
	}

	pWnd = (CWnd *)GetDlgItem(IDC_STATIC_BG);
	if (pWnd)
	{
		pWnd->GetClientRect(&rect);
		pDC = pWnd->GetDC();
		if (pDC)
		{
			pDC->FillSolidRect(&rect,m_back_color);
			ReleaseDC(pDC);
		}
		pWnd->Invalidate();
	}
}

// ------------------------------------------------------
void CFormP::OnBtnBackColor() 
{
	CColorDialog dlg(m_back_color,CC_ANYCOLOR,this);
	if (dlg.DoModal() != IDOK) return;
	m_back_color = dlg.GetColor();
	UpdateForm();
}

// ------------------------------------------------------
void CFormP::OnBtnLineColor() 
{
	CColorDialog dlg(m_line_color,CC_ANYCOLOR,this);
	if (dlg.DoModal() != IDOK) return;
	m_line_color = dlg.GetColor();
	UpdateForm();
}

// ------------------------------------------------------
void CFormP::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	UpdateForm();
	// Do not call CFormView::OnPaint() for painting messages
}

// ------------------------------------------------------
void CFormP::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// create tool tip control
	if (!m_pTipCtrl)
	{
		m_pTipCtrl = new CToolTipCtrl;
		if (m_pTipCtrl)
		{
			// add tool tip text
			m_pTipCtrl->Create(this);
	
			// common controls for parametric surface type
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_SURFNAME              ),     "Description of the surface");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C1                    ),     "C1 Sub-expression can use u,v, math functions, C2...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C2                    ),     "C2 Sub-expression can use u,v, math functions, C3...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C3                    ),     "C3 Sub-expression can use u,v, math functions, C4...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C4                    ),     "C4 Sub-expression can use u,v, math functions, C5...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C5                    ),     "C5 Sub-expression can use u,v, math functions, C6...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C6                    ),     "C6 Sub-expression can use u,v, math functions, C7,C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C7                    ),     "C7 Sub-expression can use u,v, math functions, C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C8                    ),     "C8 Sub-expression can use u,v, math functions");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_XYTILT                ),     "Tilt angle from X-Y plane (degrees) (0=top view, 90=side view)");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_ZROTATE               ),     "Rotation about Z (up) axis (degrees)");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_RED_EQUATION          ),     "Red Component    (0=off 100=fully on)   Can use u,v, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_GREEN_EQUATION        ),     "Green Component  (0=off 100=fully on)   Can use u,v, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_BLUE_EQUATION         ),     "Blue Component   (0=off 100=fully on)   Can use u,v, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_BTN_BACK_COLOR             ),     "Background color");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_BTN_LINE_COLOR             ),     "Line color");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_SCALE                 ),     "Drawing Scale (percent)");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_SHOW_COLOR           ),     "If checked, surface will be drawn with color");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_SHOW_AXIS            ),     "If checked, X,Y,Z coordinate axis drawn");

			// unique to parametric surface type
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_XPLOT_EQUATION        ),     "X-Component of surface definition x(u,v);  Can use u,v, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_YPLOT_EQUATION        ),     "Y-Component of surface definition y(u,v);  Can use u,v, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_ZPLOT_EQUATION        ),     "Z-Component of surface definition z(u,v);  Can use u,v, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_UMIN                  ),     "Minimum U value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_UMAX                  ),     "Maximum U value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_VMIN                  ),     "Minimum V value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_VMAX                  ),     "Maximum V value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_ULINES                ),     "Number of U-Lines to draw");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_VLINES                ),     "Number of V-Lines to draw");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_DRAW_ULINES          ),     "If checked, draw U-Lines");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_DRAW_VLINES          ),     "If checked, draw V-Lines");
			
			// make it active
			m_pTipCtrl->Activate(TRUE);
		}
	}
}

// ------------------------------------------------------
BOOL CFormP::PreTranslateMessage(MSG* pMsg) 
{
	if (m_pTipCtrl != NULL)
	{
		m_pTipCtrl->RelayEvent(pMsg);
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}

// ------------------------------------------------------
