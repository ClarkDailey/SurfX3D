//	<><><><><><><><><><><><><>  Plot3DP.h  <><><><><><><><><><><><><><> 
//
// -----------------------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Plot3DP is a C++ class used to
// plot a 3D view of a function defined parametrically
//
// -----------------------------------------------------------------

#ifndef	_PLOT3DP_H_	 	// avoid repeated including
#define	_PLOT3DP_H_	1	// has been included

#include "plot3dt.h"


// ------------------------------
//  3D parametric plotting class
// ------------------------------
class Plot3DP : public PlotTriUV
{
	friend class CFormP;
public:
	Plot3DP();
	virtual ~Plot3DP();

	int Plot(CWnd* pWnd);

	// reset
	void Reset(void);
	void DefInputsP(void);

	// load / save
	int ReadFile(void);
	int WriteFile(void);
	CString GetSurfName() { return(m_sSurfaceName); }

	// parametric equations
	double XParametric(double u,double v) { return( m_XPlotCalc.Calc(u,v) ); }
	double YParametric(double u,double v) { return( m_YPlotCalc.Calc(u,v) ); }
	double ZParametric(double u,double v) { return( m_ZPlotCalc.Calc(u,v) ); }
	int  InitEquations(void);

	// color info
	double RedColor  (double u,double v,double w) { return( ValidateColorValue( m_RPlotCalc.Calc(u,v) ) ); }
	double GreenColor(double u,double v,double w) { return( ValidateColorValue( m_GPlotCalc.Calc(u,v) ) ); }
	double BlueColor (double u,double v,double w) { return( ValidateColorValue( m_BPlotCalc.Calc(u,v) ) ); }

public:	// data
	PlotCalc m_XPlotCalc;  // X Equation Calculator
	PlotCalc m_YPlotCalc;  // Y Equation Calculator
	PlotCalc m_ZPlotCalc;  // Z Equation Calculator
	PlotCalc m_RPlotCalc;  // R Equation Calculator
	PlotCalc m_GPlotCalc;  // G Equation Calculator
	PlotCalc m_BPlotCalc;  // B Equation Calculator

public: // dialog parameters
	// surface definition
	CString m_sSurfaceName;
	CString	m_xPlotEquation;
	CString	m_yPlotEquation;
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
};


#endif	/* if _PLOT3DP_H_ included */

/*	<><><><><><><><><><><><><>  Plot3DP.h  <><><><><><><><><><><><><><> */
