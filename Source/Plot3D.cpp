// <><><><><><><><><><><><><>  Plot3D.cpp  <><><><><><><><><><><><><><>
//
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Used as a base class for several Plot3D classes
//
// ----------------------------------------------------

#include "stdafx.h"	// precompiled header
#include "plot3d.h"
#include "surfx3d.h"
#include <sys/stat.h>

#define FONT_LIST_ID		(1000)
#define SCALE_CAL_FACTOR	(1.3)

// -------------------------------------------------------------------
//    I N I T I A L I Z A T I O N
// -------------------------------------------------------------------
Plot3D::Plot3D()
{
	// zero dynamically allocated pointers
	m_pPoints      = 0;
	m_pLines       = 0;
	m_pTriangles   = 0;

	m_nPoints      = 0;
	m_nLines       = 0;
	m_nTriangles   = 0;

	m_maxPoints    = 0;
	m_maxLines     = 0;
	m_maxTriangles = 0;

	m_hRC  = 0;
	m_pWnd = 0;
	m_wndWidth  = 1;
	m_wndHeight = 1;
	m_is_rendered  = 0;
	m_is_rendering = 0;
	m_is_equation_ok = 0;

	// font for drawing text for axis
	LOGFONT logfont;
	logfont.lfHeight        = -12;
	logfont.lfWidth         = 0;
	logfont.lfEscapement    = 0;
	logfont.lfOrientation   = logfont.lfEscapement;
	logfont.lfWeight        = FW_EXTRABOLD;
	logfont.lfItalic        = FALSE;
	logfont.lfUnderline     = FALSE;
	logfont.lfStrikeOut     = FALSE;
	logfont.lfCharSet       = ANSI_CHARSET;
	logfont.lfOutPrecision  = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality       = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = FF_DONTCARE|DEFAULT_PITCH;
	strcpy(logfont.lfFaceName, PLOT3D_FONT_NAME);
	m_textFont.CreateFontIndirect(&logfont);

	InitBase();
}

// -------------------------------------------------------------------
Plot3D::~Plot3D()
{
	// release device context
	ReleaseRC();

	FreeTriangles();

	// no longer attached to a window
	m_pWnd = 0;
}

// ------------------------------------------------------------------------------------------
void Plot3D::InitBase()
{
	DefInputsBase();
	ClearRendered();
	m_is_equation_ok = 0;
}

// ------------------------------------------------------------------------------------------
// set default user inputs
void Plot3D::DefInputsBase(void)
{
	// set axis ranges
	m_xmin = -10.;
	m_xmax =  10.;
	m_ymin = -10.;
	m_ymax =  10.;
	m_zmin = -10.;
	m_zmax =  10.;

	// viewing angles
	m_xytilt   = 60.;
	m_zrotate  = 50.;
	m_scale    = 90;

	// axis parameters
	m_show_axis  = 0;
	m_axis_color = DEF_AXIS_COLOR;

	// color parameters
	m_is_color = 1;
	m_back_color=DEF_BKGND_COLOR;
	m_line_color=PLOT_COLOR_BLACK;
}

// ------------------------------------------------------------
// Calculate Persistence-of-Vision camera location
void Plot3D::GetPOVCamera(double* xc,double* yc,double* zc)
{
	double xdiam,ydiam,diam;
	double zp, scale;
	xdiam = fabs(m_xcmax-m_xcmin);
	ydiam = fabs(m_ycmax-m_ycmin);
	diam  = max(xdiam,ydiam);
	scale = fabs(m_scale);
	if (scale<1) scale=1;
	zp  = (SCALE_CAL_FACTOR+.2) * (diam/(scale/100.)) + m_zc;
	*xc = xof(m_xc,m_yc,zp);
	*yc = yof(m_xc,m_yc,zp);
	*zc = zof(m_xc,m_yc,zp);
}

// -------------------------------------------------------------------
// get plot window size
void Plot3D::GetWndSize(int* Width,int* Height)
{
	CRect rect;
	int wndWidth=1,wndHeight=1;
	if (m_pWnd)
	{
		m_pWnd->GetClientRect(&rect);
		wndWidth  = rect.Width();
		wndHeight = rect.Height();
	}
	// guarantee non-zero to avoid division by zero
	if (wndWidth < 1) wndWidth  = 1;				       
	if (wndHeight< 1) wndHeight = 1;
	// return window size
	*Width  = wndWidth;
	*Height = wndHeight;
}

// -------------------------------------------------------------------
//     P L O T T I N G     P R I M I T I V E S
// -------------------------------------------------------------------

void Plot3D::WinBegPlot()
{
	CRect	rect;
	if (!m_pWnd) return;	// don't crash

	ClearScreen();
	m_is_rendering = 1;
	m_abort_plotting = 0;
	theApp.UpdateToolBar();
	Paint(0);	// show wait message

	// calculate transformation matrix
	CalcTransMatrix();

	// save initial window size
	GetWndSize(&m_wndWidth,&m_wndHeight);

	theApp.SetStatusString("Render Begin");
	theApp.UpdateToolBar();
}

// -------------------------------------------------------------------
void Plot3D::WinEndPlot()
{
	ShowPlot();
	m_is_rendering = 0;
	m_is_rendered  = 1;
	theApp.SetStatusString("Render Complete");
	theApp.UpdateToolBar();
}

// -------------------------------------------------------------------
// return difference between two times
void Plot3D::CalcDiffTime(time_t begTime,time_t endTime,unsigned* minutes,unsigned* seconds)
{
	unsigned m, s;
	s = (unsigned)difftime(endTime,begTime); // total seconds
	m = s/60;
	s = s - m*60;
	*minutes = m;
	*seconds = s;
}

// --------------------------------------------------------------
// handle plot printing
void Plot3D::DoPrint(CDC* pPrintDC)
{
	BITMAPINFO	bmInfo;
	int			nBmBytes;
	char*		pBmPixels;

	if (!pPrintDC->IsPrinting()) return;

	// get window size
	int wndWidth, wndHeight;
	GetWndSize(&wndWidth,&wndHeight);
	CSize szWnd = CSize(wndWidth,wndHeight);

	// get printer page size
	CSize szPage(pPrintDC->GetDeviceCaps(HORZRES),
				 pPrintDC->GetDeviceCaps(VERTRES));

	// build bitmap info structure
	nBmBytes = ((wndWidth*3+3)&0xFFFFFFFC)*wndHeight;
	memset(&bmInfo,0,sizeof(bmInfo));
	bmInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth       = wndWidth;
	bmInfo.bmiHeader.biHeight      = wndHeight;
	bmInfo.bmiHeader.biPlanes      = 1; 
	bmInfo.bmiHeader.biBitCount    = 24;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage   = nBmBytes; 
	bmInfo.bmiHeader.biHeight      = wndHeight;
	
	// allocate memory for bitmap RGB bits
	pBmPixels = (char*)::GlobalAlloc(GMEM_FIXED,nBmBytes);
	if (!pBmPixels) return;

	// pull pixels out of OpenGL frame buffer
	Paint(0);	// refresh buffer
	::glReadBuffer(GL_FRONT);
	::glReadPixels(0,0,wndWidth,wndHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,pBmPixels);

	// calculate scaling
	float fWndRatio  = float(wndHeight) / wndWidth;
	float fPageRatio = float(szPage.cy) / szPage.cx;
	float fBmiPageRatio = fWndRatio / fPageRatio;
	CSize szTarget;
	if (fBmiPageRatio < 1)
	{
		// Landscape
		szTarget.cx = szPage.cx;
		szTarget.cy = long(fWndRatio * szPage.cx);
	}
	else
	{
		// Portrait page
		szTarget.cx = long(szPage.cy/fWndRatio);
		szTarget.cy = szPage.cy;
	}

	// offsets
	CSize szOffset((szPage.cx - szTarget.cx) / 2,
	               (szPage.cy - szTarget.cy) / 2);

	// Stretch image to fit in the target size
	::StretchDIBits(pPrintDC->m_hDC,
		                       szOffset.cx, szOffset.cy,
			                   szTarget.cx, szTarget.cy,
				               0, 0,
					           szWnd.cx,    szWnd.cy,
							   pBmPixels,
	                           &bmInfo,
		                       DIB_RGB_COLORS,
			                   SRCCOPY);
	// free allocated memory
	::GlobalFree(pBmPixels);
}

// -------------------------------------------------------------------
// returns: 0=ok, 1=no memory
int Plot3D::CopyPlotToClipBoard()
{
	int rc = 0;

	if (m_pWnd==0) return(1);	// don't crash

	// Open the clipboard
	if (OpenClipboard(NULL))
	{
		// create client device context
		CClientDC   dc(m_pWnd);
		CDC         memDC;
		CBitmap		bitmap;

    	BITMAPINFO	bmInfo;
    	int			nBmBytes;
    	char*		pBmPixels;
    
    	// get window size
    	int wndWidth, wndHeight;
    	GetWndSize(&wndWidth,&wndHeight);
    
    	// build bitmap info structure
    	nBmBytes = ((wndWidth*3+3)&0xFFFFFFFC)*wndHeight;
    	memset(&bmInfo,0,sizeof(bmInfo));
    	bmInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    	bmInfo.bmiHeader.biWidth       = wndWidth;
    	bmInfo.bmiHeader.biHeight      = wndHeight;
    	bmInfo.bmiHeader.biPlanes      = 1; 
    	bmInfo.bmiHeader.biBitCount    = 24;
    	bmInfo.bmiHeader.biCompression = BI_RGB;
    	bmInfo.bmiHeader.biSizeImage   = nBmBytes; 
    	bmInfo.bmiHeader.biHeight      = wndHeight;
    	
    	// allocate memory for bitmap RGB bits
    	pBmPixels = (char*)::GlobalAlloc(GMEM_FIXED,nBmBytes);
    	if (!pBmPixels) 
        {
    		AfxMessageBox( "Not enough memory to copy to clipboard" );
            return(1);
        }
    
    	// pull pixels out of OpenGL frame buffer
    	Paint(0);	// refresh buffer
    	::glReadBuffer(GL_FRONT);
    	::glReadPixels(0,0,wndWidth,wndHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,pBmPixels);

		// Create memDC
        memDC.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, wndWidth, wndHeight);

		// select bitmap into DC
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    	// Stretch image to fit in the target size
    	::StretchDIBits(memDC, 0, 0, wndWidth, wndHeight, 0, 0, wndWidth, wndHeight,
    						    pBmPixels, &bmInfo, DIB_RGB_COLORS, SRCCOPY);

        // restore state
		memDC.SelectObject(pOldBitmap);

		// Clean the contents of the clipboard
		EmptyClipboard();

		// Copy contents of memDC to clipboard
		SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
		CloseClipboard();

    	// free allocated memory
    	::GlobalFree(pBmPixels);
	}
	else
	{
		AfxMessageBox( "Clipboard unavailable" );
		rc = 1;
	}
	return(rc);
}

// -------------------------------------------------------------------
// keep window messages pumping to allow plot redraw
void Plot3D::MsgPump()
{
	MSG msg;
	while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
	{ 
		if ( !AfxGetApp()->PumpMessage( ) ) 
		{ 
			::PostQuitMessage(0); 
		} 
	}
}

// -------------------------------------------------------------------
//     A X I S     R O U T I N E S
// -------------------------------------------------------------------

void Plot3D::GenAxis()
{
	CString str;
	double dx,dy,dz,x1,y1,z1,dt,t;
	unsigned int n;

	if (!m_show_axis) return;

	double rline = (GetRValue(m_axis_color)/255.);
	double gline = (GetGValue(m_axis_color)/255.);
	double bline = (GetBValue(m_axis_color)/255.);
	::glColor3d(rline,gline,bline);
	::glLineWidth(3.0f);
	::glDisable(GL_LINE_SMOOTH);

	double xcorn[2], ycorn[2], zcorn[2];
	xcorn[0] =  m_xmin - m_xc;
	xcorn[1] =  m_xmax - m_xc;
	ycorn[0] =  m_ymin - m_yc;
	ycorn[1] =  m_ymax - m_yc;
	zcorn[0] = -m_zmin + m_zc;
	zcorn[1] = -m_zmax + m_zc;

	// draw containing box
	::glBegin(GL_LINES);
	::glVertex3d(xcorn[0], ycorn[0], zcorn[0]);	::glVertex3d(xcorn[1], ycorn[0], zcorn[0]);
	::glVertex3d(xcorn[1], ycorn[0], zcorn[0]);	::glVertex3d(xcorn[1], ycorn[1], zcorn[0]);
	::glVertex3d(xcorn[1], ycorn[1], zcorn[0]);	::glVertex3d(xcorn[0], ycorn[1], zcorn[0]);
	::glVertex3d(xcorn[0], ycorn[1], zcorn[0]);	::glVertex3d(xcorn[0], ycorn[0], zcorn[0]);
	::glVertex3d(xcorn[0], ycorn[0], zcorn[1]);	::glVertex3d(xcorn[1], ycorn[0], zcorn[1]);
	::glVertex3d(xcorn[1], ycorn[0], zcorn[1]);	::glVertex3d(xcorn[1], ycorn[1], zcorn[1]);
	::glVertex3d(xcorn[1], ycorn[1], zcorn[1]);	::glVertex3d(xcorn[0], ycorn[1], zcorn[1]);
	::glVertex3d(xcorn[0], ycorn[1], zcorn[1]);	::glVertex3d(xcorn[0], ycorn[0], zcorn[1]);
	::glVertex3d(xcorn[0], ycorn[0], zcorn[0]);	::glVertex3d(xcorn[0], ycorn[0], zcorn[1]);
	::glVertex3d(xcorn[1], ycorn[0], zcorn[0]);	::glVertex3d(xcorn[1], ycorn[0], zcorn[1]);
	::glVertex3d(xcorn[1], ycorn[1], zcorn[0]);	::glVertex3d(xcorn[1], ycorn[1], zcorn[1]);
	::glVertex3d(xcorn[0], ycorn[1], zcorn[0]);	::glVertex3d(xcorn[0], ycorn[1], zcorn[1]);
	::glEnd();

	// -------------
	// X tick marks
	// -------------
	unsigned int nxticks = 3;
	dx = xcorn[1] - xcorn[0];
	x1 = xcorn[0];
	y1 = ycorn[0];
	z1 = zcorn[0];
	dx = dx/(double)(nxticks-1);
    dt = (m_xmax-m_xmin)/(nxticks-1);
    t  = m_xmin;
	for (n=0;n<nxticks;n++,x1+=dx,t+=dt)
	{
		str.Format(" x =%8.4lf",t);
		WinPlotText(-90., x1,y1,z1,str);
	}

	// -------------
	// Y tick marks
	// -------------
	unsigned int nyticks = 3;
	dy = ycorn[1] - ycorn[0];
	x1 = xcorn[1];
	y1 = ycorn[0];
	z1 = zcorn[0];
	dy = dy/(double)(nyticks-1);
    dt = (m_ymax-m_ymin)/(nyticks-1);
    t  = m_ymin;
	for (n=0;n<nyticks;n++,y1+=dy,t+=dt)
	{
		str.Format(" y =%8.4lf",t);
		WinPlotText(0., x1,y1,z1,str);
	}

	// -------------
	// Z tick marks
	// -------------
	unsigned int nzticks = 3;
	dz = zcorn[1] - zcorn[0];
	x1 = xcorn[0];
	y1 = ycorn[1];
	z1 = zcorn[0];
	dz = dz/(double)(nzticks-1);
    dt = (m_zmax-m_zmin)/(nzticks-1);
    t  = m_zmin;
	for (n=0;n<nzticks;n++,z1+=dz,t+=dt)
	{
		str.Format(" z =%8.4lf",t);
		WinPlotText(90., x1,y1,z1,str);
	}
}

// -------------------------------------------------------------------
void Plot3D::WinPlotText(double Angle,double x,double y,double z,const char* sText)
{
	::glPushMatrix();
	glTranslated(x, y, z);
	::glRotated(Angle, 0.0f, 0.0f, 1.0f );
	::glScaled(m_fontScale,m_fontScale,m_fontScale);

	char byte = ' ';	// print space as initial separator
	while ( byte )
	{
		glCallList(FONT_LIST_ID + __toascii(byte)) ;
		byte = *sText++; 
	}
	::glPopMatrix();
}

// -------------------------------------------------------------------
//     U T I L I T Y     R O U T I N E S
// -------------------------------------------------------------------

// returns: 0=no, 1=ini plot file exists
int DoesFileExist(CString sFilePath)
{
	struct _stat fileInfo;
	int rc = ::_stat(sFilePath,&fileInfo);
	return(rc==0?1:0);
//	return( PathFileExists(sFilePath) );
}

// --------------------------------------------------------------
// extract folder path (without ending backslash) from fully qualified filename
CString ExtractPathDir(CString pathName)
{
	CString sDir, fn=pathName;
	int indx = fn.ReverseFind('\\');  // find last backslash
	if (indx>0) sDir = fn.Left(indx);
	return(sDir);
}

// --------------------------------------------------------------
// extract filename (no extension) from fully qualified filename
CString ExtractFileName(CString fullName)
{
	CString sName = fullName;

	// find file extension
	int indx = sName.ReverseFind('.');
	if (indx>=0) sName = sName.Left(indx);	// strip extension

	// find end of folder name
	indx = sName.ReverseFind('\\');
	if (indx>=0)
		sName = sName.Mid(indx+1);	// extract filename
	else
	{
		// check for forward slash
		indx = sName.ReverseFind('/');
		if (indx>=0)
			sName = sName.Mid(indx+1);	// extract filename
	}
	return(sName);
}

// -------------------------------------------------------------------
//     I N I    F I L E     R O U T I N E S
// -------------------------------------------------------------------
CString Plot3D::GetIniStr(CString keyName,CString defValue)
{
	char buffer[256];
	CString value = defValue;
	memset(buffer,0,sizeof(buffer));
	DWORD cnt = ::GetPrivateProfileString(m_sIniHdr, keyName, defValue, buffer, sizeof(buffer), m_FilePath);
	if (cnt) value = buffer;
	return(value);
}

// -------------------------------------------------------------------
double Plot3D::GetIniDouble(CString keyName,double defValue)
{
	double value = defValue;
	char buffer[256];
	CString defStr;
	defStr.Format("%lf", defValue);
	memset(buffer,0,sizeof(buffer));
	DWORD cnt = ::GetPrivateProfileString(m_sIniHdr, keyName, defStr, buffer, sizeof(buffer), m_FilePath);
	if (cnt)
	{
		double dtemp;
		int rc=sscanf(buffer,"%lf", &dtemp);
		if (rc==1) value = dtemp;
	}
	return(value);
}

// -------------------------------------------------------------------
long Plot3D::GetIniLong(CString keyName,long defValue)
{
	long value = defValue;
	value = ::GetPrivateProfileInt(m_sIniHdr, keyName, defValue, m_FilePath);
	return(value);
}

// -------------------------------------------------------------------
// returns: 0=ok
int Plot3D::SetIniStr(CString keyName,CString value)
{
	BOOL rc = ::WritePrivateProfileString(m_sIniHdr, keyName, value, m_FilePath);
	return(rc?0:1);
}

// -------------------------------------------------------------------
// returns: 0=ok
int Plot3D::SetIniDouble(CString keyName,double value)
{
	CString strValue;
	strValue.Format("%lf", value);
	BOOL rc = ::WritePrivateProfileString(m_sIniHdr, keyName, strValue, m_FilePath);
	return(rc?0:1);
}

// -------------------------------------------------------------------
// returns: 0=ok
int Plot3D::SetIniLong(CString keyName,long value)
{
	CString strValue;
	strValue.Format("%ld", value);
	BOOL rc = ::WritePrivateProfileString(m_sIniHdr, keyName, strValue, m_FilePath);
	return(rc?0:1);
}

// -------------------------------------------------------------------
void Plot3D::Paint(int swapBuf)
{
	if (m_is_rendering)
	{
		// show wait message during rendering
	    CBitmap bitmap;
	    BITMAP bm;
	    CClientDC dc(m_pWnd);
	    bitmap.LoadBitmap(IDB_RENDERING1);
	    bitmap.GetObject (sizeof (BITMAP), &bm);
	    CPoint size(bm.bmWidth, bm.bmHeight);
	    CDC dcMem;
	    dcMem.CreateCompatibleDC(&dc);
	    dcMem.SelectObject(bitmap);
	    dc.BitBlt(0, 0, size.x, size.y, &dcMem, 0, 0, SRCCOPY);
		return;
	}

	// adjust using mouse
	int dxm = m_mouse_x - m_mouse_xlast;
	int dym = m_mouse_y - m_mouse_ylast;
	double dz  = 360.*(double)dxm / (double)m_wndWidth;
	double dxy = 360.*(double)dym / (double)m_wndHeight;
	if (!theApp.m_lockLeftRight) m_zMouseAngle  -= dz;
	if (!theApp.m_lockUpDown   ) m_xyMouseAngle -= dxy;

	// clear mouse movement
	m_mouse_xlast = m_mouse_x;
	m_mouse_ylast = m_mouse_y;

	::glPushMatrix();
	::glRotated(-m_zMouseAngle, 0.0f, 0.0f, 1.0f );
	::glRotated( m_xyMouseAngle,  cos(Deg2Rad(m_zMouseAngle)), sin(Deg2Rad(m_zMouseAngle)), 0.0f );
	::glCallList(1);
	::glFinish();
	::glPopMatrix();

	// trim angles to within 0-360 degrees
	if (m_xyMouseAngle<  0.) m_xyMouseAngle += 360.;
	if (m_zMouseAngle <  0.) m_zMouseAngle  += 360.;
	if (m_xyMouseAngle>360.) m_xyMouseAngle -= 360.;
	if (m_zMouseAngle >360.) m_zMouseAngle  -= 360.;

	// show current angles
	theApp.ShowRotAngles(m_xyMouseAngle,m_zMouseAngle);

	// Swap the OpenGL buffers if requested
	if (swapBuf)
	{
		if (m_pWnd)
		{
			CDC* pDC = m_pWnd->GetDC();
			if (pDC)
			{
				HDC hDC = pDC->m_hDC;
				if (hDC)
				{
					try
					{
						BOOL brc = ::SwapBuffers(hDC);
						if (brc==0)
						{
							// int rc = GetLastError();
							pDC = m_pWnd->GetDC();
						}
					}
					catch(...)
					{
						pDC = m_pWnd->GetDC();
					}
				}
				m_pWnd->ReleaseDC(pDC);
			}
		}
	}
}

// ------------------------------------------------------------
BOOL Plot3D::InitOpenGL(CDC* pDC)
{
	static PIXELFORMATDESCRIPTOR s_pfd = 
	{
	    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
	    1,                              // version number
	    PFD_DRAW_TO_WINDOW |            // support window
	      PFD_SUPPORT_OPENGL |          // support OpenGL
	      PFD_DOUBLEBUFFER,             // double buffered
	    PFD_TYPE_RGBA,                  // RGBA type
	    24,                             // 24-bit color depth
	    0, 0, 0, 0, 0, 0,               // color bits ignored
	    0,                              // no alpha buffer
	    0,                              // shift bit ignored
	    0,                              // no accumulation buffer
	    0, 0, 0, 0,                     // accum bits ignored
	    16,                             // 16-bit z-buffer
	    0,                              // no stencil buffer
	    0,                              // no auxiliary buffer
	    PFD_MAIN_PLANE,                 // main layer
	    0,                              // reserved
	    0, 0, 0                         // layer masks ignored
	};

	/*
    ::gluPerspective(40.0f, aspect_ratio, .1f, 20.0f);
	::glOrtho( -2.0f, 2.0f, -2.0f, 2.0f, -.10f, 20.0f );
	*/

	ReleaseRC();

	int pixelFormat = ::ChoosePixelFormat(pDC->m_hDC, &s_pfd);
    if ( 0 == pixelFormat)
		return FALSE;
    if ( FALSE == ::SetPixelFormat(pDC->m_hDC, pixelFormat, &s_pfd) )
		return FALSE;

	//	For Color-Index mode, you'd probably create your palette here, right
	//	after you select the pixel format
    m_hRC = ::wglCreateContext( pDC->m_hDC );
    if ( 0 == m_hRC )
		return FALSE;

    if ( FALSE == ::wglMakeCurrent( pDC->m_hDC, m_hRC ) )
		return FALSE;

	// generate font lists
	GLYPHMETRICSFLOAT gmf[256];
	CFont* pFont;
	pFont = pDC->SelectObject(&m_textFont);
	::wglUseFontOutlines(pDC->m_hDC,0,256,FONT_LIST_ID,0.0,0.0,WGL_FONT_POLYGONS,gmf);
    pDC->SelectObject(pFont);

	// initialize angles
	m_zMouseAngle  = m_zrotate;
	m_xyMouseAngle = m_xytilt;

	// background color
	float red   = (float)(GetRValue(m_back_color)/255.);
	float green = (float)(GetGValue(m_back_color)/255.);
	float blue  = (float)(GetBValue(m_back_color)/255.);
    ::glClearColor(red,green,blue,0.0f);

	// specify the back of the buffer as clear depth
    ::glClearDepth(1.0f);

	// enable depth testing
    ::glEnable(GL_DEPTH_TEST);

	if (m_wndWidth  == 0) m_wndWidth  = 1; // avoid division by zero
	if (m_wndHeight == 0) m_wndHeight = 1; // avoid division by zero

	double m_maxDelta = max(fabs(m_xmax-m_xmin),fabs(m_ymax-m_ymin));
	m_maxDelta = max(fabs(m_zmax-m_zmin),m_maxDelta);
	if (m_maxDelta == 0.0) m_maxDelta = CLOSE_TO_ZERO;
	double scale = SCALE_CAL_FACTOR*(m_scale/100.)/m_maxDelta;
	double xscale,yscale;
	xscale = scale;
	yscale = scale;
	if (m_wndWidth > m_wndHeight)
		xscale = scale * m_wndHeight/m_wndWidth;
	else
		yscale = scale * m_wndWidth/m_wndHeight;
	::glScaled(xscale,yscale,scale);
	m_fontScale = m_maxDelta/20.;
    ::glViewport(0, 0, m_wndWidth, m_wndHeight);

	// ---------------------
	// Create a Display List
	// ---------------------
	Triangle3DT* pTrngl;
	Line3DT* pLine;
	unsigned i;

	// calculate centroid
	m_xcmin = m_ycmin = m_zcmin =  PLOT3D_DBL_MAX; // very large positive number
	m_xcmax = m_ycmax = m_zcmax = -PLOT3D_DBL_MAX; // very negative number
	for (i=0; i<TriangleCount(); i++)
	{
		pTrngl = GetTrianglePtr(i);
		TestRange(pTrngl->m_Pnt[0].m_xf,pTrngl->m_Pnt[0].m_yf,pTrngl->m_Pnt[0].m_zf);
		TestRange(pTrngl->m_Pnt[1].m_xf,pTrngl->m_Pnt[1].m_yf,pTrngl->m_Pnt[1].m_zf);
		TestRange(pTrngl->m_Pnt[2].m_xf,pTrngl->m_Pnt[2].m_yf,pTrngl->m_Pnt[2].m_zf);
	}
	for (i=0; i<LineCount(); i++)
	{
		pLine = GetLinePtr(i);
		TestRange(pLine->m_x1,pLine->m_y1,pLine->m_z1);
		TestRange(pLine->m_x2,pLine->m_y2,pLine->m_z2);
	}
	CalcCentroid();

	// start of an OpenGL list
	::glNewList(1,GL_COMPILE);

	// Clear out the color & depth buffers
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// surface triangles
	if (m_is_color)
	{
		::glBegin(GL_TRIANGLES);
		for (i=0; i<TriangleCount(); i++)
		{
			pTrngl = GetTrianglePtr(i);
			pTrngl->OutOpenGL(m_xc,m_yc,m_zc);
		}
		::glEnd();
	}

	// surface lines
	// surface line color
	double rline = (GetRValue(m_line_color)/255.);
	double gline = (GetGValue(m_line_color)/255.);
	double bline = (GetBValue(m_line_color)/255.);
	::glColor3d(rline,gline,bline);
	::glLineWidth(3.0f);
	::glDisable(GL_LINE_SMOOTH);
	::glBegin(GL_LINES);
	for (i=0; i<LineCount(); i++)
	{
		pLine = GetLinePtr(i);
		pLine->OutOpenGL(m_xc,m_yc,m_zc);
	}
	::glEnd();

	// axis lines
	GenAxis();

	::glEndList();

	return TRUE;
}

// ------------------------------------------------------------
void Plot3D::ShowPlot()
{
	if (!m_pWnd) return;	// don't crash

	// get a device context
	CClientDC dc(m_pWnd);
	BOOL brc = InitOpenGL(&dc);
	if (brc==0)
	{
		brc = 0;
	}

	// force a repaint; onPaint handles rest
	m_pWnd->Invalidate();
}

// ------------------------------------------------------------
void Plot3D::FreeTriangles(void)
{
	// free point,line,triangle arrays
	free(m_pLines);
	free(m_pPoints);
	free(m_pTriangles);
	m_nPoints   = m_nLines   = m_nTriangles   = 0;
	m_maxPoints = m_maxLines = m_maxTriangles = 0;
	m_pPoints   = 0;
	m_pLines    = 0;
	m_pTriangles= 0;
}

// ------------------------------------------------------------
Point3DT* Plot3D::AddPoint(double x,double y,double z,double r,double g,double b,char zbad)
{
	Point3DT* pPnt;

	// need reallocation?
	if ((m_nPoints+1) >=m_maxPoints)
	{
		// allocate array of points
		pPnt = (Point3DT*)calloc(sizeof(Point3DT),m_maxPoints+ALLOC_SIZE);
		if (m_pPoints)
		{
			// copy old array to new array
			memcpy(pPnt,m_pPoints,sizeof(Point3DT)*m_maxPoints);
			free(m_pPoints);
		}
		m_pPoints = (Point3DT*)pPnt;
		m_maxPoints += ALLOC_SIZE;
	}

	// get pointer to new object
	pPnt = GetPointPtr(m_nPoints);
	pPnt->Load(x,y,z,r,g,b,zbad);
	m_nPoints++;	// one more point
	return(pPnt);
}

// ------------------------------------------------------------
Line3DT* Plot3D::AddLine(double x1,double y1,double z1,double x2,double y2,double z2)
{
	Line3DT* pLine;

	// need reallocation?
	if ((m_nLines+1) >=m_maxLines)
	{
		// allocate array of lines
		pLine = (Line3DT*)calloc(sizeof(Line3DT),m_maxLines+ALLOC_SIZE);
		if (m_pLines)
		{
			// copy old array to new array
			memcpy(pLine,m_pLines,sizeof(Line3DT)*m_maxLines);
			free(m_pLines);
		}
		m_pLines = (Line3DT*)pLine;
		m_maxLines += ALLOC_SIZE;
	}

	// get pointer to new object
	pLine = GetLinePtr(m_nLines);
	pLine->Load(x1,y1,z1,x2,y2,z2);
	m_nLines++;	// one more line
	return(pLine);
}

// ------------------------------------------------------------
// returns NULL if z is invalid on any of the three points
Triangle3DT* Plot3D::AddTriangle(Point3DT* pP1,Point3DT* pP2,Point3DT* pP3)
{
	double r0,g0,b0,r1,g1,b1,r2,g2,b2,rt,gt,bt;
	Triangle3DT* pTri;

    if (pP1->m_zbad && pP2->m_zbad && pP3->m_zbad) return(NULL);

	// need reallocation?
	if ((m_nTriangles+1) >=m_maxTriangles)
	{
		// allocate array of triangles
		pTri = (Triangle3DT*)calloc(sizeof(Triangle3DT),m_maxTriangles+ALLOC_SIZE);
		if (m_pTriangles)
		{
			// copy old array to new array
			memcpy(pTri,m_pTriangles,m_maxTriangles*sizeof(Triangle3DT));
			free(m_pTriangles);
		}
		m_pTriangles = (Triangle3DT*)pTri;
		m_maxTriangles += ALLOC_SIZE;
	}

	// get pointer to new object
	pTri = GetTrianglePtr(m_nTriangles);
	pTri->Load(pP1,pP2,pP3);

	// calculate color of triangle from average of points
	r0=pTri->RedPnt(0); g0=pTri->GreenPnt(0); b0=pTri->BluePnt(0);
	r1=pTri->RedPnt(1); g1=pTri->GreenPnt(1); b1=pTri->BluePnt(1);
	r2=pTri->RedPnt(2); g2=pTri->GreenPnt(2); b2=pTri->BluePnt(2);
	// average the three points
	rt = (r0+r1+r2)/3.;
	gt = (g0+g1+g2)/3.;
	bt = (b0+b1+b2)/3.;
	// set the color of the triangle
	pTri->SetColor(rt,gt,bt);
	m_nTriangles++;	// one more triangle
	return(pTri);
}

/*	<><><><><><><><><><><><><>  Plot3D.cpp  <><><><><><><><><><><><><><> */



