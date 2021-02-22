//	<><><><><><><><><><><><><>  Plot3DF.h  <><><><><><><><><><><><><><> 
//
// -----------------------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Plot3DF is a C++ class used to
// plot a 3D view of a function defined as f(x,y,z)=0
//
// Limitations:
//   f(x,y,z) must produce a sign change in order to be plottable
//
// -----------------------------------------------------------------

#ifndef	_PLOT3DF_H_	 	// avoid repeated including
#define	_PLOT3DF_H_	1	// has been included

#include "plot3d.h"

// defaults
#define DEFAULT_MESH_SIZE	(100)
#define DEFAULT_LINE_COUNT	(50)

// default equation
#define DEFAULT_F_NAME		"Untitled"
#define DEFAULT_F_EQUATION	"x^2+y^2+z^2-1.4"	// sphere
#define DEFAULT_F_RED		"(y+1)*50"
#define DEFAULT_F_GREEN		"(x+1)*50"
#define DEFAULT_F_BLUE		"0"

class Plot3DF : public Plot3D
{
	friend class CFormF;
public:
	Plot3DF();
	virtual ~Plot3DF();

	// get routines
	CString GetSurfName() { return(m_sSurfaceName); }

	// file export
	int ExportPOV (CString  povFilePath,CString sPovObjectName);
	int ExportVRML(CString vrmlFilePath,CString sVrmlSurfaceName);
	int ExportDXF (CString sDxfFilePath,CString sDxfSurfaceName);
    int ExportSTL (CString sStlFilePath,CString sStlSurfaceName);

	// load / save
	int ReadFile(void);
	int WriteFile(void);

	// plotting
	int Plot(CWnd* pWnd);
	void Reset(void)
	{
		InitBase();
		DefInputsF();
	}

	// surface function
	double Func(double x,double y,double z) { return( m_FuncPlotCalc.Calc(x,y,z) ); }

protected:
	int  InitPlotF(void);
	void DefInputsF(void);
	int  InitEquations(void);
	void CalcNormVectors(void);
	void PlotMask(unsigned iy);
	void GenTriangles(void);
	void GenCubeTriangles(double xmin,double ymin,double zmin,double xmax,double ymax,double zmax,unsigned char lutValue);
	int  GenXLines(void);
	int  GenYLines(void);
	int  GenZLines(void);
	Point3DT FindLineInstersection(double x1,double y1,double z1,double x2,double y2,double z2);

protected:
	// ---------------
	// user input data
	// ---------------

	// surface definition
	CString m_sSurfaceName;
	CString	m_sC1;
	CString	m_sC2;
	CString	m_sC3;
	CString	m_sC4;
	CString	m_sC5;
	CString	m_sC6;
	CString	m_sC7;
	CString	m_sC8;
 	CString	m_sFuncEquation;
 	CString	m_sREquation;
 	CString	m_sGEquation;
 	CString	m_sBEquation;

	// number of constant cutting planes
	unsigned m_nxplanes;
	unsigned m_nyplanes;
	unsigned m_nzplanes;

	// flags
	BOOL m_draw_xlines;		// 0=no, 1=draw x contour lines
	BOOL m_draw_ylines;		// 0=no, 1=draw y contour lines
	BOOL m_draw_zlines;		// 0=no, 1=draw z contour lines

	// calculators
	PlotCalc m_FuncPlotCalc; 	// Equation Calculator
	PlotCalc m_RPlotCalc;		// red    equation calculator
	PlotCalc m_GPlotCalc;		// green  equation calculator
	PlotCalc m_BPlotCalc;		// blue   equation calculator
};


#endif	/* if _PLOT3DF_H_ included */

/*	<><><><><><><><><><><><><>  Plot3DF.h  <><><><><><><><><><><><><><> */
