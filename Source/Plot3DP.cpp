//	<><><><><><><><><><><><><>  Plot3DP.cpp  <><><><><><><><><><><><><><>
//		
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Implementation of Plot3DP C++ class
//
// --------------------------------------------------------------------------
//

#include "stdafx.h"	// precompiled header
#include <stdio.h>
#include <float.h>
#include "Plot3DP.h"
#include "SurfX3D.h"


// ------------------------------------------------------------
//     Plot3DP methods
// ------------------------------------------------------------

Plot3DP::Plot3DP()
{
	m_is_color_xyz = 0;	// u,v color
	DefInputsP();
	SetIniHeader("Parametric");
}

Plot3DP::~Plot3DP()
{
}

// ------------------------------------------------------------
void Plot3DP::Reset(void)
{
	ResetUV();
	DefInputsP();
}

// ------------------------------------------------------------
// does the actual work of plotting
// returns: 0=ok, 1=user abort, 2=not enough memory to plot
int Plot3DP::Plot(CWnd* pWnd)
{
	time_t begTime, endTime;

	SetWnd(pWnd);

	theApp.SetStatusString("Render Begin");
	time(&begTime);

	if (InitPlotUV()) return(2);

	WinBegPlot();
	GenULines();
	GenVLines();

	WinEndPlot();
	time(&endTime);

	// calculate render time
	unsigned minutes, seconds;
	CalcDiffTime(begTime,endTime,&minutes,&seconds);

	// show rendering time
	CString sRenderTime;
	sRenderTime.Format("Render Complete    %u:%02u (m:s)", minutes, seconds);
	theApp.SetStatusString(sRenderTime);

	return(0);
}

// ------------------------------------------------------------
// returns: 0=ok, else error
int Plot3DP::InitEquations()
{
	int xcol, ycol, zcol, rcol, gcol, bcol;

	// check input parameters
	if (m_umin >= m_umax)
	{
		m_is_equation_ok = 0;
		theApp.SetStatusString("Error: Umin >= Umax");
		return(1);
	}
	if (m_vmin >= m_vmax)
	{
		m_is_equation_ok = 0;
		theApp.SetStatusString("Error: Vmin >= Vmax");
		return(1);
	}

	// update calculator - load constants and program
 	xcol = m_XPlotCalc.LoadProgram("X(u,v)",(LPCTSTR)m_xPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8,1);

 	ycol = m_YPlotCalc.LoadProgram("Y(u,v)",(LPCTSTR)m_yPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8,1);

 	zcol = m_ZPlotCalc.LoadProgram("Z(u,v)",(LPCTSTR)m_zPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8,1);

 	rcol = m_RPlotCalc.LoadProgram("R(u,v)",(LPCTSTR)m_rPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8,1);

 	gcol = m_GPlotCalc.LoadProgram("G(u,v)",(LPCTSTR)m_gPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8,1);

 	bcol = m_BPlotCalc.LoadProgram("B(u,v)",(LPCTSTR)m_bPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8,1);

	m_is_equation_ok = (xcol | ycol | zcol | rcol | gcol | bcol) ? 0 : 1;

	// show status string
	if (xcol)
	 	m_sEquationStatus = m_XPlotCalc.GetErrString();
	else if (ycol)
	 	m_sEquationStatus = m_YPlotCalc.GetErrString();
	else if (zcol)
	 	m_sEquationStatus = m_ZPlotCalc.GetErrString();
	else if (rcol)
	 	m_sEquationStatus = m_RPlotCalc.GetErrString();
	else if (gcol)
	 	m_sEquationStatus = m_GPlotCalc.GetErrString();
	else
	 	m_sEquationStatus = m_BPlotCalc.GetErrString();

	if (!m_is_equation_ok) theApp.SetStatusString(m_sEquationStatus);
	return(m_is_equation_ok?0:1);
}

// ------------------------------------------------------------
// default user inputs
void Plot3DP::DefInputsP()
{
	// initialize plot equation
	m_sSurfaceName  = "Untitled";
	m_xPlotEquation = "cos(u)";
	m_yPlotEquation = "v";
	m_zPlotEquation = "sin(u)";
	m_rPlotEquation = "(sin(u)+1)*50";
	m_gPlotEquation = "(cos(u)+1)*50";
	m_bPlotEquation = "0";
	m_sC1 = "1";
	m_sC2 = "1";
	m_sC3 = "1";
	m_sC4 = "1";
	m_sC5 = "1";
	m_sC6 = "1";
	m_sC7 = "1";
	m_sC8 = "1";

	// base settings
	m_umin = 0.;
	m_umax = 5.;
	m_vmin = 0.;
	m_vmax = 5.;
	m_nxMesh = DEFAULT_MESH_SIZE;
	m_nyMesh = DEFAULT_MESH_SIZE;
}

// ------------------------------------------------------------
// returns: 0=ok, 1=can't open file
int  Plot3DP::ReadFile()
{
	if (!DoesFileExist(m_FilePath)) return(1);

	// surface definition
	m_sSurfaceName  = GetIniStr   ("Name",          	"Untitled"    );
	m_xPlotEquation = GetIniStr   ("XPlotEquation", 	""    );
	m_yPlotEquation = GetIniStr   ("YPlotEquation", 	""    );
	m_zPlotEquation = GetIniStr   ("ZPlotEquation", 	""    );
	m_rPlotEquation = GetIniStr   ("RPlotEquation", 	""    );
	m_gPlotEquation = GetIniStr   ("GPlotEquation", 	""    );
	m_bPlotEquation = GetIniStr   ("BPlotEquation", 	""    );
	m_sC1           = GetIniStr   ("C1",	          	""    );
	m_sC2           = GetIniStr   ("C2",	          	""    );
	m_sC3           = GetIniStr   ("C3",	          	""    );
	m_sC4           = GetIniStr   ("C4",	          	""    );
	m_sC5           = GetIniStr   ("C5",	          	""    );
	m_sC6           = GetIniStr   ("C6",	          	""    );
	m_sC7           = GetIniStr   ("C7",	          	""    );
	m_sC8           = GetIniStr   ("C8",	          	""    );

	// common parameters
	m_zrotate       = GetIniDouble("ZRotate",       	30.0  );
	m_xytilt        = GetIniDouble("XYTilt",        	60.0  );
	m_scale         = GetIniDouble("PlotScale",        100.0  );
	m_line_color    = GetIniLong  ("LineColor",     DEF_LINE_COLOR  );
	m_back_color    = GetIniLong  ("BackColor",     DEF_BKGND_COLOR );
	m_is_color      = GetIniLong  ("ShowColor",     	 1    );
	m_show_axis     = GetIniLong  ("ShowAxis",      	 0    );

	// specific parameters
	m_umin          = GetIniDouble("UMin",              -10.  );
	m_umax          = GetIniDouble("UMax",               10.  );
	m_vmin          = GetIniDouble("VMin",              -10.  );
 	m_vmax          = GetIniDouble("VMax",               10.  );
	m_nulines       = GetIniLong  ("NULines",            30   );
	m_nvlines       = GetIniLong  ("NVLines",            30   );
	m_draw_ulines   = GetIniLong  ("DrawULines",    	 1    );
	m_draw_vlines   = GetIniLong  ("DrawVLines",    	 1    );
	m_nxMesh        = GetIniLong  ("nXMesh",          DEFAULT_MESH_SIZE  );
	m_nyMesh        = GetIniLong  ("nYMesh",          DEFAULT_MESH_SIZE  );

	return(0);
}

// ------------------------------------------------------------
int  Plot3DP::WriteFile()
{
	// surface definition
	SetIniStr   ("Name",          m_sSurfaceName);
 	SetIniStr   ("XPlotEquation", m_xPlotEquation);
 	SetIniStr   ("YPlotEquation", m_yPlotEquation);
 	SetIniStr   ("ZPlotEquation", m_zPlotEquation);
 	SetIniStr   ("RPlotEquation", m_rPlotEquation);
 	SetIniStr   ("GPlotEquation", m_gPlotEquation);
 	SetIniStr   ("BPlotEquation", m_bPlotEquation);
	SetIniStr   ("C1",	          m_sC1);
	SetIniStr   ("C2",	          m_sC2);
	SetIniStr   ("C3",	          m_sC3);
	SetIniStr   ("C4",	          m_sC4);
	SetIniStr   ("C5",	          m_sC5);
	SetIniStr   ("C6",	          m_sC6);
	SetIniStr   ("C7",	          m_sC7);
	SetIniStr   ("C8",	          m_sC8);

	// common parameters
	SetIniDouble("ZRotate",       m_zrotate);
	SetIniDouble("XYTilt",        m_xytilt);
	SetIniDouble("PlotScale",     m_scale);
	SetIniLong  ("LineColor",     m_line_color);
	SetIniLong  ("BackColor",     m_back_color);
	SetIniLong  ("ShowColor",     m_is_color);
 	SetIniLong  ("ShowAxis",      m_show_axis);

	// specific parameters
	SetIniDouble("UMin",          m_umin);
	SetIniDouble("UMax",          m_umax);
	SetIniDouble("VMin",          m_vmin);
	SetIniDouble("VMax",          m_vmax);
	SetIniLong  ("NULines",       m_nulines);
	SetIniLong  ("NVLines",       m_nvlines);
	SetIniLong  ("ShowU",         m_draw_ulines);
	SetIniLong  ("ShowV",         m_draw_vlines);
	SetIniLong  ("nXMesh",        m_nxMesh);
	SetIniLong  ("nYMesh",        m_nyMesh);

	return(0);
}

//	<><><><><><><><><><><><><>  Plot3DP.cpp  <><><><><><><><><><><><><><> //














