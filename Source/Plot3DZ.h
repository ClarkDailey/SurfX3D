// <><><><><><><><><><><><><>  Plot3DZ.h  <><><><><><><><><><><><><><>
//
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Implementation of class Plot3DZ
//
// ----------------------------------------------------

#ifndef	_PLOT3DZ_H_	 	// avoid repeated including
#define	_PLOT3DZ_H_	1	// has been included

#include "plot3dt.h"


// default equation
#define DEFAULT_Z_NAME		"Untitled"
#define DEFAULT_Z_EQUATION	"x^2+y^2"	// parabola
#define DEFAULT_Z_RED		"(y+1)*50"
#define DEFAULT_Z_GREEN		"(x+1)*50"
#define DEFAULT_Z_BLUE		"0"

class Plot3DZ : public PlotTriUV
{
	friend class CFormZ;
public:
	Plot3DZ();
	virtual ~Plot3DZ();

	// plotting
	int Plot(CWnd* pWnd);

	double Zfunc(double x,double y)
	{
		double z = m_PlotCalc.Calc(x,y);
		if (!m_limit_z) return(z);
		// limit z values to user defined
		if (z < m_zmin) z = m_zmin;
		if (z > m_zmax) z = m_zmax;
		return(z);
	}
	
	void Reset(void)
	{
		ResetUV();
		InitBase();
		DefInputsZ();
	}

	void DefInputsZ();

	// load / save
	int ReadFile(void);
	int WriteFile(void);
	CString GetSurfName() { return(m_sSurfaceName); }

	// parametric equations
	double XParametric(double x,double y) { return(x); }
	double YParametric(double x,double y) { return(y); }
	double ZParametric(double x,double y) { return(Zfunc(x,y)); }
	int InitEquations(void);

	// color info
	double RedColor  (double x,double y,double z) { return( ValidateColorValue( m_RPlotCalc.Calc(x,y,z) ) ); }
	double GreenColor(double x,double y,double z) { return( ValidateColorValue( m_GPlotCalc.Calc(x,y,z) ) ); }
	double BlueColor (double x,double y,double z) { return( ValidateColorValue( m_BPlotCalc.Calc(x,y,z) ) ); }

public:	// data
	PlotCalc m_PlotCalc;		// Equation Calculator
	PlotCalc m_RPlotCalc;		// red    equation calculator
	PlotCalc m_GPlotCalc;		// green  equation calculator
	PlotCalc m_BPlotCalc;		// blue   equation calculator

private:	// user intputs
	// surface definition
	CString m_sSurfaceName;
	CString	m_zPlotEquation;
	CString	m_rPlotEquation;
	CString	m_gPlotEquation;
	CString	m_bPlotEquation;
	CString	m_sC1;
	CString	m_sC2;
	CString	m_sC3;
	CString	m_sC4;
	CString	m_sC5;
	CString	m_sC6;
	CString	m_sC7;
	CString	m_sC8;
	int m_limit_z;
};

#endif	/* if _PLOT3DZ_H_ included */

/*	<><><><><><><><><><><><><>  Plot3DZ.h  <><><><><><><><><><><><><><> */
