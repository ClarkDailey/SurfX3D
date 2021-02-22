// FormF.cpp : implementation file
//

#include "stdafx.h"
#include "SurfX3D.h"
#include "FormF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormF

IMPLEMENT_DYNCREATE(CFormF, CFormView)

CFormF::CFormF()
	: CFormView(CFormF::IDD)
{
	//{{AFX_DATA_INIT(CFormF)
	m_sC1 = _T("");
	m_sC2 = _T("");
	m_sC3 = _T("");
	m_sC4 = _T("");
	m_sC5 = _T("");
	m_sC6 = _T("");
	m_sC7 = _T("");
	m_sC8 = _T("");
	m_sBEquation = _T("");
	m_sGEquation = _T("");
	m_sREquation = _T("");
	m_sFuncEquation = _T("");
	m_scale = 100.0;
	m_show_axis = FALSE;
	m_is_color = FALSE;
	m_xmax = 0.0;
	m_xmin = 0.0;
	m_xytilt = 0.0;
	m_ymax = 0.0;
	m_ymin = 0.0;
	m_zmax = 0.0;
	m_zmin = 0.0;
	m_zrotate = 0.0;
	m_sSurfaceName = _T("");
	m_nxplanes = DEFAULT_LINE_COUNT;
	m_nyplanes = DEFAULT_LINE_COUNT;
	m_nzplanes = DEFAULT_LINE_COUNT;
	m_draw_xlines = FALSE;
	m_draw_ylines = FALSE;
	m_draw_zlines = FALSE;
	m_nxMesh = DEFAULT_MESH_SIZE;
	m_nyMesh = DEFAULT_MESH_SIZE;
	m_nzMesh = DEFAULT_MESH_SIZE;
	//}}AFX_DATA_INIT
	m_pTipCtrl = 0;
}

CFormF::~CFormF()
{
	delete m_pTipCtrl;
}

void CFormF::LoadDlg()
{
	m_sSurfaceName  = theApp.m_plot3DF.m_sSurfaceName;
	m_sC1           = theApp.m_plot3DF.m_sC1;
	m_sC2           = theApp.m_plot3DF.m_sC2;
	m_sC3           = theApp.m_plot3DF.m_sC3;
	m_sC4           = theApp.m_plot3DF.m_sC4;
	m_sC5           = theApp.m_plot3DF.m_sC5;
	m_sC6           = theApp.m_plot3DF.m_sC6;
	m_sC7           = theApp.m_plot3DF.m_sC7;
	m_sC8           = theApp.m_plot3DF.m_sC8;
	m_sFuncEquation = theApp.m_plot3DF.m_sFuncEquation;
	m_sBEquation    = theApp.m_plot3DF.m_sBEquation;
	m_sREquation    = theApp.m_plot3DF.m_sREquation;
	m_sGEquation    = theApp.m_plot3DF.m_sGEquation;

	// common parameters
	m_zrotate       = theApp.m_plot3DF.m_zrotate;      
	m_xytilt        = theApp.m_plot3DF.m_xytilt;       
	m_scale         = theApp.m_plot3DF.m_scale;        
	m_line_color    = theApp.m_plot3DF.m_line_color;   
	m_back_color    = theApp.m_plot3DF.m_back_color;   
	m_is_color      = theApp.m_plot3DF.m_is_color;     
	m_show_axis     = theApp.m_plot3DF.m_show_axis;    

	// specific parameters
	m_xmin             = theApp.m_plot3DF.m_xmin;       
	m_xmax             = theApp.m_plot3DF.m_xmax;       
	m_ymin             = theApp.m_plot3DF.m_ymin;       
 	m_ymax             = theApp.m_plot3DF.m_ymax;       
	m_zmin             = theApp.m_plot3DF.m_zmin;       
 	m_zmax             = theApp.m_plot3DF.m_zmax;       
	m_nxMesh           = theApp.m_plot3DF.m_nxMesh;    
	m_nyMesh           = theApp.m_plot3DF.m_nyMesh;    
	m_nzMesh           = theApp.m_plot3DF.m_nzMesh;    
	m_nxplanes         = theApp.m_plot3DF.m_nxplanes;    
	m_nyplanes         = theApp.m_plot3DF.m_nyplanes;    
	m_nzplanes         = theApp.m_plot3DF.m_nzplanes;    
	m_draw_xlines      = theApp.m_plot3DF.m_draw_xlines     ;
	m_draw_ylines      = theApp.m_plot3DF.m_draw_ylines     ;
	m_draw_zlines      = theApp.m_plot3DF.m_draw_zlines     ;
	UpdateData(FALSE);	// set values into dialog
}

void CFormF::GetDlg()
{
	UpdateData(TRUE);	// get values from dialog
	theApp.m_plot3DF.m_sSurfaceName  = m_sSurfaceName  ;
	theApp.m_plot3DF.m_sC1           = m_sC1           ;
	theApp.m_plot3DF.m_sC2           = m_sC2           ;
	theApp.m_plot3DF.m_sC3           = m_sC3           ;
	theApp.m_plot3DF.m_sC4           = m_sC4           ;
	theApp.m_plot3DF.m_sC5           = m_sC5           ;
	theApp.m_plot3DF.m_sC6           = m_sC6           ;
	theApp.m_plot3DF.m_sC7           = m_sC7           ;
	theApp.m_plot3DF.m_sC8           = m_sC8           ;
	theApp.m_plot3DF.m_sFuncEquation = m_sFuncEquation ;
	theApp.m_plot3DF.m_sBEquation    = m_sBEquation;
	theApp.m_plot3DF.m_sREquation    = m_sREquation;
	theApp.m_plot3DF.m_sGEquation    = m_sGEquation;

	// common parameters
	theApp.m_plot3DF.m_zrotate       = m_zrotate      ;      
	theApp.m_plot3DF.m_xytilt        = m_xytilt       ;       
	theApp.m_plot3DF.m_scale         = m_scale        ;        
	theApp.m_plot3DF.m_line_color    = m_line_color   ;   
	theApp.m_plot3DF.m_back_color    = m_back_color   ;   
	theApp.m_plot3DF.m_is_color      = m_is_color     ;     
	theApp.m_plot3DF.m_show_axis     = m_show_axis    ;    

	// specific parameters
	theApp.m_plot3DF.m_xmin             = m_xmin            ;       
	theApp.m_plot3DF.m_xmax             = m_xmax            ;       
	theApp.m_plot3DF.m_ymin             = m_ymin            ;       
 	theApp.m_plot3DF.m_ymax             = m_ymax            ;       
	theApp.m_plot3DF.m_zmin             = m_zmin            ;       
 	theApp.m_plot3DF.m_zmax             = m_zmax            ;       
	theApp.m_plot3DF.m_nxMesh           = m_nxMesh          ;    
	theApp.m_plot3DF.m_nyMesh           = m_nyMesh          ;    
	theApp.m_plot3DF.m_nzMesh           = m_nzMesh          ;    
	theApp.m_plot3DF.m_nxplanes         = m_nxplanes        ;    
	theApp.m_plot3DF.m_nyplanes         = m_nyplanes        ;    
	theApp.m_plot3DF.m_nzplanes         = m_nzplanes        ;    
	theApp.m_plot3DF.m_draw_xlines      = m_draw_xlines     ;
	theApp.m_plot3DF.m_draw_ylines      = m_draw_ylines     ;
	theApp.m_plot3DF.m_draw_zlines      = m_draw_zlines     ;
}

int CFormF::IsDataChanged()
{
	UpdateData(TRUE);
	if (theApp.m_plot3DF.m_sSurfaceName  != m_sSurfaceName  ) return(1);
	if (theApp.m_plot3DF.m_sC1           != m_sC1           ) return(1);
	if (theApp.m_plot3DF.m_sC2           != m_sC2           ) return(1);
	if (theApp.m_plot3DF.m_sC3           != m_sC3           ) return(1);
	if (theApp.m_plot3DF.m_sC4           != m_sC4           ) return(1);
	if (theApp.m_plot3DF.m_sC5           != m_sC5           ) return(1);
	if (theApp.m_plot3DF.m_sC6           != m_sC6           ) return(1);
	if (theApp.m_plot3DF.m_sC7           != m_sC7           ) return(1);
	if (theApp.m_plot3DF.m_sC8           != m_sC8           ) return(1);
	if (theApp.m_plot3DF.m_sFuncEquation != m_sFuncEquation ) return(1);
	if (theApp.m_plot3DF.m_sBEquation    != m_sBEquation    ) return(1);
	if (theApp.m_plot3DF.m_sREquation    != m_sREquation    ) return(1);
	if (theApp.m_plot3DF.m_sGEquation    != m_sGEquation    ) return(1);

	// common parameters
	if (theApp.m_plot3DF.m_zrotate       != m_zrotate      ) return(1);      
	if (theApp.m_plot3DF.m_xytilt        != m_xytilt       ) return(1);       
	if (theApp.m_plot3DF.m_scale         != m_scale        ) return(1);        
	if (theApp.m_plot3DF.m_line_color    != m_line_color   ) return(1);   
	if (theApp.m_plot3DF.m_back_color    != m_back_color   ) return(1);   
	if (theApp.m_plot3DF.m_is_color      != m_is_color     ) return(1);     
	if (theApp.m_plot3DF.m_show_axis     != m_show_axis    ) return(1);    

	// specific parameters
	if (theApp.m_plot3DF.m_xmin             != m_xmin            ) return(1);       
	if (theApp.m_plot3DF.m_xmax             != m_xmax            ) return(1);       
	if (theApp.m_plot3DF.m_ymin             != m_ymin            ) return(1);       
 	if (theApp.m_plot3DF.m_ymax             != m_ymax            ) return(1);       
	if (theApp.m_plot3DF.m_zmin             != m_zmin            ) return(1);       
 	if (theApp.m_plot3DF.m_zmax             != m_zmax            ) return(1);       
	if (theApp.m_plot3DF.m_nxMesh           != m_nxMesh          ) return(1);    
	if (theApp.m_plot3DF.m_nyMesh           != m_nyMesh          ) return(1);    
	if (theApp.m_plot3DF.m_nzMesh           != m_nzMesh          ) return(1);    
	if (theApp.m_plot3DF.m_nxplanes         != m_nxplanes        ) return(1);    
	if (theApp.m_plot3DF.m_nyplanes         != m_nyplanes        ) return(1);    
	if (theApp.m_plot3DF.m_nzplanes         != m_nzplanes        ) return(1);    
	if (theApp.m_plot3DF.m_draw_xlines      != m_draw_xlines     ) return(1);
	if (theApp.m_plot3DF.m_draw_ylines      != m_draw_ylines     ) return(1);
	if (theApp.m_plot3DF.m_draw_zlines      != m_draw_zlines     ) return(1);
	return(0);
}

void CFormF::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormF)
	DDX_Text(pDX, IDC_EDIT_C1, m_sC1);
	DDX_Text(pDX, IDC_EDIT_C2, m_sC2);
	DDX_Text(pDX, IDC_EDIT_C3, m_sC3);
	DDX_Text(pDX, IDC_EDIT_C4, m_sC4);
	DDX_Text(pDX, IDC_EDIT_C5, m_sC5);
	DDX_Text(pDX, IDC_EDIT_C6, m_sC6);
	DDX_Text(pDX, IDC_EDIT_C7, m_sC7);
	DDX_Text(pDX, IDC_EDIT_C8, m_sC8);
	DDX_Text(pDX, IDC_EDIT_BLUE_EQUATION, m_sBEquation);
	DDX_Text(pDX, IDC_EDIT_GREEN_EQUATION, m_sGEquation);
	DDX_Text(pDX, IDC_EDIT_RED_EQUATION, m_sREquation);
	DDX_Text(pDX, IDC_EDIT_PLOT_EQUATION, m_sFuncEquation);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_scale);
	DDX_Check(pDX, IDC_CHECK_SHOW_AXIS, m_show_axis);
	DDX_Check(pDX, IDC_CHECK_SHOW_COLOR, m_is_color);
	DDX_Text(pDX, IDC_EDIT_XMAX, m_xmax);
	DDX_Text(pDX, IDC_EDIT_XMIN, m_xmin);
	DDX_Text(pDX, IDC_EDIT_XYTILT, m_xytilt);
	DDX_Text(pDX, IDC_EDIT_YMAX, m_ymax);
	DDX_Text(pDX, IDC_EDIT_YMIN, m_ymin);
	DDX_Text(pDX, IDC_EDIT_ZMAX, m_zmax);
	DDX_Text(pDX, IDC_EDIT_ZMIN, m_zmin);
	DDX_Text(pDX, IDC_EDIT_ZROTATE, m_zrotate);
	DDX_Text(pDX, IDC_EDIT_SURFNAME, m_sSurfaceName);
	DDX_Text(pDX, IDC_EDIT_NXLINES, m_nxplanes);
	DDX_Text(pDX, IDC_EDIT_NYLINES, m_nyplanes);
	DDX_Text(pDX, IDC_EDIT_NZLINES, m_nzplanes);
	DDX_Check(pDX, IDC_CHECK_DRAW_XLINES, m_draw_xlines);
	DDX_Check(pDX, IDC_CHECK_DRAW_YLINES, m_draw_ylines);
	DDX_Check(pDX, IDC_CHECK_DRAW_ZLINES, m_draw_zlines);
	DDX_Text(pDX, IDC_EDIT_NX_MESH, m_nxMesh);
	DDX_Text(pDX, IDC_EDIT_NY_MESH, m_nyMesh);
	DDX_Text(pDX, IDC_EDIT_NZ_MESH, m_nzMesh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormF, CFormView)
	//{{AFX_MSG_MAP(CFormF)
	ON_BN_CLICKED(IDC_BTN_BACK_COLOR, OnBtnBackColor)
	ON_BN_CLICKED(IDC_BTN_LINE_COLOR, OnBtnLineColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormF diagnostics

#ifdef _DEBUG
void CFormF::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormF::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormF message handlers

void CFormF::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	LoadDlg();
}

// ------------------------------------------------------
void CFormF::UpdateForm()
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
void CFormF::OnBtnBackColor() 
{
	CColorDialog dlg(m_back_color,CC_ANYCOLOR,this);
	if (dlg.DoModal() != IDOK) return;
	m_back_color = dlg.GetColor();
	UpdateForm();
}

// ------------------------------------------------------
void CFormF::OnBtnLineColor() 
{
	CColorDialog dlg(m_line_color,CC_ANYCOLOR,this);
	if (dlg.DoModal() != IDOK) return;
	m_line_color = dlg.GetColor();
	UpdateForm();
}

// ------------------------------------------------------
void CFormF::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	UpdateForm();
	// Do not call CFormView::OnPaint() for painting messages
}

// ------------------------------------------------------
void CFormF::OnInitialUpdate() 
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
	
			// common controls for implicit surface type
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_SURFNAME              ),     "Description of the surface");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C1                    ),     "C1 Sub-expression can use x,y,z, math functions, C2...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C2                    ),     "C2 Sub-expression can use x,y,z, math functions, C3...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C3                    ),     "C3 Sub-expression can use x,y,z, math functions, C4...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C4                    ),     "C4 Sub-expression can use x,y,z, math functions, C5...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C5                    ),     "C5 Sub-expression can use x,y,z, math functions, C6...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C6                    ),     "C6 Sub-expression can use x,y,z, math functions, C7,C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C7                    ),     "C7 Sub-expression can use x,y,z, math functions, C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_C8                    ),     "C8 Sub-expression can use x,y,z, math functions");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_XYTILT                ),     "Tilt angle from X-Y plane (degrees) (0=top view, 90=side view)");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_ZROTATE               ),     "Rotation about Z (up) axis (degrees)");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_RED_EQUATION          ),     "Red Component    (0=off 100=fully on)   Can use x,y,z, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_GREEN_EQUATION        ),     "Green Component  (0=off 100=fully on)   Can use x,y,z, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_BLUE_EQUATION         ),     "Blue Component   (0=off 100=fully on)   Can use x,y,z, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_BTN_BACK_COLOR             ),     "Background color");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_BTN_LINE_COLOR             ),     "Line color");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_SCALE                 ),     "Drawing Scale (percent)");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_SHOW_COLOR           ),     "If checked, surface will be drawn with color");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_SHOW_AXIS            ),     "If checked, X,Y,Z coordinate axis drawn");

			// unique to implicit surface type
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_PLOT_EQUATION         ),     "Surface equation definition f(x,y,z)=0;  Can use x,y,z, math functions, C1...C8");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_XMIN                  ),     "Minimum X value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_XMAX                  ),     "Maximum X value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_YMIN                  ),     "Minimum Y value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_YMAX                  ),     "Maximum Y value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_ZMIN                  ),     "Minimum Z value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_ZMAX                  ),     "Maximum Z value of viewing region");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_NX_MESH               ),     "Number of search intervals in X direction for surface resolution");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_NY_MESH               ),     "Number of search intervals in Y direction for surface resolution");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_NZ_MESH               ),     "Number of search intervals in Z direction for surface resolution");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_NXLINES               ),     "Number of X-Lines to draw");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_NYLINES               ),     "Number of Y-Lines to draw");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_EDIT_NZLINES               ),     "Number of Z-Lines to draw");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_DRAW_XLINES          ),     "If checked, draw X-Lines");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_DRAW_YLINES          ),     "If checked, draw Y-Lines");
			m_pTipCtrl->AddTool( GetDlgItem(IDC_CHECK_DRAW_ZLINES          ),     "If checked, draw Z-Lines");
			
			// make it active
			m_pTipCtrl->Activate(TRUE);
		}
	}
}

// ------------------------------------------------------
BOOL CFormF::PreTranslateMessage(MSG* pMsg) 
{
	if (m_pTipCtrl != NULL)
	{
		m_pTipCtrl->RelayEvent(pMsg);
	}

	return CFormView::PreTranslateMessage(pMsg);
}

// ------------------------------------------------------
