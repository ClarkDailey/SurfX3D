// <><><><><><><><><><><><><>  Plot3D.h  <><><><><><><><><><><><><><>
//
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Used as a base class for several Plot3D classes
//
// ----------------------------------------------------

#ifndef	_PLOT3D_H_	 	// avoid repeated including
#define	_PLOT3D_H_	1	// has been included

#include <math.h>
#include <assert.h>
#include "plotcalc.h"
#include "trimesh.h"

#ifdef _DEBUG
 #define PLOT_ASSERT(v)	assert(v)
#else
 #define PLOT_ASSERT(v)	// do nothing
#endif

// degrees/radians conversions
inline double Deg2Rad(double deg) { return((deg*2.*3.1415925359)/360.);   }
inline double Rad2Deg(double rad) { return((rad*360.)/(2.*3.1415925359)); }

// color values
#define	PLOT3D_MIN_COLOR	(0)
#define	PLOT3D_MAX_COLOR	(100)
#define	WIN_MIN_COLOR		(0)
#define	WIN_MAX_COLOR		(255)

// convert plot color component to windows color component
#define	PCOLOR_TO_WINCOLOR(d)		((int)((double)d*(double)WIN_MAX_COLOR/(double)PLOT3D_MAX_COLOR))

// Pesistence-of-Vision simulate line drawing using thin cylinders
#define POV_LINE_RADIUS (1000.)	// divisor of object range

// misc
#define PLOT3D_FONT_NAME	"Courier New" // works for all Windows versions
#define	PLOT3D_DBL_MAX	(1.e300)	// largest double value

// defined colors
#define  PLOT_COLOR_BLACK	RGB(  0,  0,  0)	// black color
#define  PLOT_COLOR_WHITE	RGB(255,255,255)	// white color
#define  PLOT_COLOR_LTGRAY	RGB(212,208,200)	// gray background color

// default color mappings
#define	 DEF_LINE_COLOR		PLOT_COLOR_BLACK	// default line color
#define	 DEF_AXIS_COLOR		PLOT_COLOR_BLACK	// default axis color
#define	 DEF_BKGND_COLOR 	PLOT_COLOR_LTGRAY	// default background color

// ------------------------------------------------------------------------------------------
//   STL File Format
//
//     https://en.wikipedia.org/wiki/STL_(file_format)
//     STL uses little-endian; Windows is littel-endian, so no conversion is needed
// ------------------------------------------------------------------------------------------

#pragma pack(1)  // structure packing on byte alignment

// STL File Format Header
typedef struct STL_HEADER_s
{
    unsigned char hdr[80];
    unsigned long nTriangles;  // number of triangles
} STL_HEADER;  // 84 bytes

// STL File Format Point
typedef struct STL_POINT_s
{
    // Note: float must be 4 bytes per STL spec
	float x;
	float y;
	float z;
} STL_POINT; // 12 bytes

// STL File Format Triangle
typedef struct STL_TRIANGLE_s
{
    STL_POINT normVector;   // normal vector - points outward from object - set to 0,0,0 and software will calculate
    STL_POINT vertex1;      // vertex1 point
    STL_POINT vertex2;      // vertex2 point
    STL_POINT vertex3;      // vertex3 point
    unsigned short attribute; // 2 bytes set to zero per spec
} STL_TRIANGLE; // 50 bytes

#pragma pack() // back to normal packing

// ------------------------------------------------------------------------------------------
// base class for several Plot3D classes
class Plot3D
{
public:
	Plot3D();
	virtual ~Plot3D();

	// set routines
	void SetFilePath(CString filePath) { m_FilePath = filePath; }
	CString GetFilePath(void) { return(m_FilePath); }
	void RequestCancel(void) { m_abort_plotting = 1; }

	// status routines
	int IsRendering() { return(m_is_rendering); }
	int IsRendered()  { return(m_is_rendered);  }
	void Paint(int swapBuf);

	// get plot window size
	void GetWndSize(int* Width,int* Height);

	// zooming functions
	void ZoomIn()
	{
		if (!m_is_rendered || !m_pWnd) return;
		::glScaled(1.1, 1.1, 1.1);
		m_pWnd->Invalidate();
	}

	void ZoomOut()
	{
		if (!m_is_rendered || !m_pWnd) return;
		::glScaled(0.9, 0.9, 0.9);
		m_pWnd->Invalidate();
	}

	void ShowPlot(void);
	void ClearScreen(void)
	{
		ClearRendered();
		if (m_pWnd)
		{
			CRect rect;
			m_pWnd->GetClientRect(&rect);
			CDC* pDC = m_pWnd->GetDC();
			if (pDC)
				pDC->FillSolidRect(&rect,RGB(255,255,255));	// white
		}
	}

	void ClearRendered()
	{
		m_is_rendering   = 0;
		m_is_rendered    = 0;
	}

	BOOL InitOpenGL(CDC* pDC);
	void ReleaseRC(void)
	{
		// release font lists
		if (m_fontLists) ::glDeleteLists(m_fontLists, 256);
	    m_fontLists = 0;

		// release open gl context
		if (m_hRC) ::wglDeleteContext(m_hRC);
		m_hRC = 0;
	}

	void SetWnd(CWnd* pWnd) { m_pWnd = pWnd; }
	CWnd* GetWnd() { return(m_pWnd); }
	void CalcDiffTime(time_t begTime,time_t endTime,unsigned* minutes,unsigned* seconds);
	int  CopyPlotToClipBoard();
	void DoPrint(CDC* pDC);
	void MsgPump(void);
	void GenAxis(void);

	// utility
	void GetPOVCamera(double* xc,double* yc,double* zc);

	// get centroid of object
	void GetCentroid(double* xc,double* yc,double* zc)
	{
		*xc = m_xc;
		*yc = m_yc;
		*zc = m_zc;
	}

	// calculate centroid of object
	void CalcCentroid(void)
	{
		m_xc = (m_xcmin + m_xcmax)/2.;
		m_yc = (m_ycmin + m_ycmax)/2.;
		m_zc = (m_zcmin + m_zcmax)/2.;
	}

	void TestRange(double x,double y,double z)
	{
		if (x < m_xcmin) m_xcmin = x;
		if (y < m_ycmin) m_ycmin = y;
		if (z < m_zcmin) m_zcmin = z;
		if (x > m_xcmax) m_xcmax = x;
		if (y > m_ycmax) m_ycmax = y;
		if (z > m_zcmax) m_zcmax = z;
	}

	void SetMousePos(int x,int y,int reset)
	{
		// remember last mouse position
		if (reset)
		{
			m_mouse_xlast = x;
			m_mouse_ylast = y;
		}
		else
		{
			m_mouse_xlast = m_mouse_x;
			m_mouse_ylast = m_mouse_y;
		}
		// save new position
		m_mouse_x = x;
		m_mouse_y = y;
	}

	// ------------------------------------------------------------
	//             Transformation Equations
	// ------------------------------------------------------------
	// object space to plot space
	double xpf(double xo,double yo,double zo) { return(m_tm[0][0]*xo + m_tm[0][1]*yo + m_tm[0][2]*zo); }
	double ypf(double xo,double yo,double zo) { return(m_tm[1][0]*xo + m_tm[1][1]*yo + m_tm[1][2]*zo); }
	double zpf(double xo,double yo,double zo) { return(m_tm[2][0]*xo + m_tm[2][1]*yo + m_tm[2][2]*zo); } 
	
	// plot space to object space
	double xof(double xp,double yp,double zp) { return(m_tm[0][0]*xp + m_tm[1][0]*yp + m_tm[2][0]*zp); }
	double yof(double xp,double yp,double zp) { return(m_tm[0][1]*xp + m_tm[1][1]*yp + m_tm[2][1]*zp); }
	double zof(double xp,double yp,double zp) { return(m_tm[0][2]*xp + m_tm[1][2]*yp + m_tm[2][2]*zp); }

	// calculate transformation matrix
	void CalcTransMatrix(double xytiltin,double zrotin)
	{
		double	zrotate,sin_zrotate,cos_zrotate;
		double	xytilt,sin_xytilt,cos_xytilt;
	
		zrotate = Deg2Rad(zrotin);
		xytilt  = Deg2Rad(xytiltin);
		sin_zrotate = sin(zrotate);
		cos_zrotate = cos(zrotate);
		sin_xytilt  = sin(xytilt);
		cos_xytilt  = cos(xytilt);
	
		m_tm[0][0] =  cos_zrotate;
		m_tm[0][1] =  sin_zrotate;
		m_tm[0][2] =  0;
		m_tm[1][0] = -sin_zrotate * cos_xytilt;
		m_tm[1][1] =  cos_zrotate * cos_xytilt;
		m_tm[1][2] =                sin_xytilt;
		m_tm[2][0] =  sin_zrotate * sin_xytilt;
		m_tm[2][1] = -cos_zrotate * sin_xytilt;
		m_tm[2][2] =                cos_xytilt;
	}

	void CalcTransMatrix(void)
	{
		CalcTransMatrix(m_xytilt, m_zrotate);	// use current settings
	}

	void WinPlotText(double Angle,double x,double y,double z,const char* sText);

protected:	// routines
	void FreeTriangles(void);
	Triangle3DT* AddTriangle(Point3DT* pP1,Point3DT* pP2,Point3DT* pP3);
	Point3DT*    AddPoint(double x,double y,double z,double r,double g,double b);
	Line3DT*     AddLine(double x1,double y1,double z1,double x2,double y2,double z2);
	Point3DT*    GetPointPtr(unsigned int i)    { return((Point3DT   *)&m_pPoints[i]); }
	Line3DT*     GetLinePtr(unsigned int i)     { return((Line3DT    *)&m_pLines[i]); }
	Triangle3DT* GetTrianglePtr(unsigned int i) { return((Triangle3DT*)&m_pTriangles[i]); }

	void DefInputsBase(void);	// set default inputs for base class
	void InitBase(void);	// initialization

	// validate color value
	double ValidateColorValue(double c)
	{
		if (c < PLOT3D_MIN_COLOR) return(PLOT3D_MIN_COLOR);
		if (c > PLOT3D_MAX_COLOR) return(PLOT3D_MAX_COLOR);
		return(c);
	}

	// INI file input
	void SetIniHeader(CString iniHdr) { m_sIniHdr=iniHdr; }
	CString GetIniStr   (CString keyName, CString defValue);
	double  GetIniDouble(CString keyName, double  defValue);
	long    GetIniLong  (CString keyName, long    defValue);

	// INI file output
	int     SetIniStr   (CString keyName, CString value);
	int     SetIniDouble(CString keyName, double  value);
	int     SetIniLong  (CString keyName, long    value);
	
	// begin / end plot routines
	void WinBegPlot(void);
	void WinEndPlot(void);

	// check for user abort
	int  IsAborted(void)
	{
		if (m_abort_plotting) return(1);
		MsgPump();	// keep window messages pumping to allow plot redraw
		return(0);
	}

	// color
	long GetBackColor() { return(m_back_color); }

	// retrieve count info
	unsigned PointCount()    { return(m_nPoints);    }
	unsigned LineCount()     { return(m_nLines);     }
	unsigned TriangleCount() { return(m_nTriangles); }

	enum { ALLOC_SIZE=50000 };	// size of re-allocation elements

	// --------
	//   Data
	// --------
public:
	// Open GL info
	HGLRC	m_hRC;			// open gl context

	double  m_xyMouseAngle;		// x-y tilt angle via mouse
	double  m_zMouseAngle;		// z rotation angle via mouse

protected:
	Point3DT*    m_pPoints;   	// array of points 
	Line3DT*     m_pLines;	    // array of lines 
	Triangle3DT* m_pTriangles;	// array of triangles

	unsigned     m_nPoints;		// number of points    in m_pPoints[]
	unsigned     m_nLines;		// number of lines     in m_pLines[]
	unsigned     m_nTriangles;	// number of triangles in m_pTriangles[]

	unsigned     m_maxPoints;	// max points    allowed in m_pPoints[]
	unsigned     m_maxLines;	// max lines     allowed in m_pLines[]    
	unsigned     m_maxTriangles;// max triangles allowed in m_pTriangles[]

	// x,y,z min/max ranges
	double	m_xc,  m_yc,  m_zc;
	double	m_xmin,  m_xmax,  m_ymin,  m_ymax,  m_zmin,  m_zmax;

	// x,y,z centroid ranges
	double	m_xcmin, m_xcmax, m_ycmin, m_ycmax, m_zcmin, m_zcmax;

	// viewing angles
	double	m_xytilt, m_zrotate;	// degrees

	// plotting scale
	double	m_scale;

	// transformation matrix
	double m_tm[3][3];

	// color info
	COLORREF m_line_color;
	COLORREF m_back_color;
	COLORREF m_axis_color;
	int		 m_is_color;

	// axis parameters
	BOOL  m_show_axis;  // 0=no, 1=draw axis

	// control flags
	int	m_abort_plotting;	// 0=no, 1=abort plotting
	int m_is_equation_ok;	// 0=no, 1=plotting equation syntax is ok

	// strings
	CString m_sEquationStatus;  // equation status string
	CString m_FilePath;	// fully qualified file path
	CString m_sIniHdr;	// INI section header

	// windows drawing
	CWnd* m_pWnd;		// pointer to our window (we don't own this resource)
	BOOL m_is_rendering;// 0=no, 1=rendering image
	BOOL m_is_rendered; // 0=no, 1=image has been rendered
	int  m_wndWidth;	// drawing window width  (pixels) (window size at render start)
	int  m_wndHeight;	// drawing window height (pixels)

	// text info
    GLuint m_fontLists;	// font character lists
	CFont  m_textFont;	// font for drawing text for axis
	double m_fontScale;
	double m_maxDelta;	// max x,y,z delta

	// mouse info for rotations
	int		m_mouse_x;
	int		m_mouse_y;
	int		m_mouse_xlast;
	int		m_mouse_ylast;

	// function mesh table
	unsigned int   m_nxMesh, m_nyMesh, m_nzMesh;	
};

// Prototyping
int DoesFileExist(CString sFilePath);
CString ExtractPathDir(CString pathName);
CString ExtractFileName(CString fullName);
void CalcNormalUnitVector(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3,double* xn,double* yn,double* zn);

#endif	/* if _PLOT3D_H_ included */

/*	<><><><><><><><><><><><><>  Plot3D.h  <><><><><><><><><><><><><><> */



