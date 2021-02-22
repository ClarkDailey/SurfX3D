//	<><><><><><><><><><><><><>  Plot3DZ.cpp  <><><><><><><><><><><><><><>
//		
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Implementation of Plot3DZ C++ class
//
// ----------------------------------------------------

#include "stdafx.h"	// precompiled header
#include <stdio.h>
#include "Plot3DZ.h"
#include "SurfX3D.h"


// ------------------------------------------------------------
Plot3DZ::Plot3DZ()
{
	// use default values
	m_is_color_xyz = 1;	// x,y,z color
	DefInputsZ();	
	SetIniHeader("Explicit");
}

// ------------------------------------------------------------
Plot3DZ::~Plot3DZ()
{
}

// ------------------------------------------------------------
// does the actual work of plotting
// returns: 0=ok, 1=user abort, 2=not enough memory to plot
int Plot3DZ::Plot(CWnd* pWnd)
{
	time_t begTime, endTime;
	double xmin, xmax, ymin, ymax, zmin, zmax;

	SetWnd(pWnd);

	theApp.SetStatusString("Render Begin");
	time(&begTime);

	// save value since base class modifies them
	xmin = m_xmin;
	xmax = m_xmax;
	ymin = m_ymin;
	ymax = m_ymax;
	zmin = m_zmin;
	zmax = m_zmax;

	m_umin = m_xmin;
	m_umax = m_xmax;
	m_vmin = m_ymin;
	m_vmax = m_ymax;

	if (InitPlotUV()) return(2);

	WinBegPlot();
	GenULines();
	GenVLines();

	WinEndPlot();
	time(&endTime);

	// restore values
	m_xmin = xmin;
	m_xmax = xmax;
	m_ymin = ymin;
	m_ymax = ymax;
	m_zmin = zmin;
	m_zmax = zmax;

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
// default user inputs
void Plot3DZ::DefInputsZ()
{
	// initialize plot equation
	m_sSurfaceName  = DEFAULT_Z_NAME;
	m_zPlotEquation = DEFAULT_Z_EQUATION;
	m_rPlotEquation = DEFAULT_Z_RED;
	m_gPlotEquation = DEFAULT_Z_GREEN;
	m_bPlotEquation = DEFAULT_Z_BLUE;
	m_sC1 = "1";
	m_sC2 = "1";
	m_sC3 = "1";
	m_sC4 = "1";
	m_sC5 = "1";
	m_sC6 = "1";
	m_sC7 = "1";
	m_sC8 = "1";

	m_nulines = 30;
	m_nvlines = 30;
	m_nxMesh  = DEFAULT_MESH_SIZE;
	m_nyMesh  = DEFAULT_MESH_SIZE;

	// set axis ranges
	m_xmin=-1.;
	m_xmax =1.;
	m_ymin=-1.;
	m_ymax= 1.;
	m_zmin=-1.;
	m_zmax= 1.;
}

// ------------------------------------------------------------
int  Plot3DZ::InitEquations()
{
	int zcol, rcol, gcol, bcol;

	// check input parameters
	if (m_xmin >= m_xmax)
	{
		m_is_equation_ok = 0;
		theApp.SetStatusString("Error: Xmin >= Xmax");
		return(1);
	}
	if (m_ymin >= m_ymax)
	{
		m_is_equation_ok = 0;
		theApp.SetStatusString("Error: Ymin >= Ymax");
		return(1);
	}
	if (m_zmin >= m_zmax)
	{
		m_is_equation_ok = 0;
		theApp.SetStatusString("Error: Zmin >= Zmax");
		return(1);
	}

	// load and check equations
 	zcol = m_PlotCalc.LoadProgram("Z(x,y)",(LPCTSTR)m_zPlotEquation,0,
				(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
				(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8);

 	rcol = m_RPlotCalc.LoadProgram("R(x,y,z)",(LPCTSTR)m_rPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8);

 	gcol = m_GPlotCalc.LoadProgram("G(x,y,z)",(LPCTSTR)m_gPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8);

 	bcol = m_BPlotCalc.LoadProgram("B(x,y,z)",(LPCTSTR)m_bPlotEquation,1,
			(LPCTSTR)m_sC1,(LPCTSTR)m_sC2,(LPCTSTR)m_sC3,(LPCTSTR)m_sC4,
			(LPCTSTR)m_sC5,(LPCTSTR)m_sC6,(LPCTSTR)m_sC7,(LPCTSTR)m_sC8);

	m_is_equation_ok = (zcol | rcol | gcol | bcol) ? 0 : 1;

	// show status string
	if (zcol)
	 	m_sEquationStatus = m_PlotCalc.GetErrString();
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
// returns: 0=ok, 1=can't open file
int  Plot3DZ::ReadFile()
{
	if (!DoesFileExist(m_FilePath)) return(1);

	// surface definition
	m_sSurfaceName  = GetIniStr   ("Name",          	DEFAULT_Z_NAME     );
	m_zPlotEquation = GetIniStr   ("PlotEquation", 	    DEFAULT_Z_EQUATION );
	m_rPlotEquation = GetIniStr   ("RPlotEquation", 	DEFAULT_Z_RED      );
	m_gPlotEquation = GetIniStr   ("GPlotEquation", 	DEFAULT_Z_GREEN    );
	m_bPlotEquation = GetIniStr   ("BPlotEquation", 	DEFAULT_Z_BLUE     );
	m_sC1           = GetIniStr   ("C1",	          	"1"    );
	m_sC2           = GetIniStr   ("C2",	          	"1"    );
	m_sC3           = GetIniStr   ("C3",	          	"1"    );
	m_sC4           = GetIniStr   ("C4",	          	"1"    );
	m_sC5           = GetIniStr   ("C5",	          	"1"    );
	m_sC6           = GetIniStr   ("C6",	          	"1"    );
	m_sC7           = GetIniStr   ("C7",	          	"1"    );
	m_sC8           = GetIniStr   ("C8",	          	"1"    );

	// common parameters
	m_zrotate       = GetIniDouble("ZRotate",       	30.0  );
	m_xytilt        = GetIniDouble("XYTilt",        	60.0  );
	m_scale         = GetIniDouble("PlotScale",        100.0  );
	m_line_color    = GetIniLong  ("LineColor",     	 DEF_LINE_COLOR    );
	m_back_color    = GetIniLong  ("BackColor",          DEF_BKGND_COLOR   );
	m_is_color      = GetIniLong  ("ShowColor",     	 1    );
	m_show_axis     = GetIniLong  ("ShowAxis",      	 0    );

	// specific parameters
	m_xmin          = GetIniDouble("XMin",              -1.  );
	m_xmax          = GetIniDouble("XMax",               1.  );
	m_ymin          = GetIniDouble("YMin",              -1.  );
 	m_ymax          = GetIniDouble("YMax",               1.  );
	m_zmin          = GetIniDouble("ZMin",              -1.  );
 	m_zmax          = GetIniDouble("ZMax",               1.  );
	m_nulines       = GetIniLong  ("NXLines",            30   );
	m_nvlines       = GetIniLong  ("NYLines",            30   );
	m_draw_ulines   = GetIniLong  ("ShowX",    			 1    );
	m_draw_vlines   = GetIniLong  ("ShowY",		    	 0    );
	m_limit_z       = GetIniLong  ("LimitZ",  	    	 0    );
	m_nxMesh        = GetIniLong  ("nXMesh",          DEFAULT_MESH_SIZE  );
	m_nyMesh        = GetIniLong  ("nYMesh",          DEFAULT_MESH_SIZE  );

	return(0);
}

// ------------------------------------------------------------
int  Plot3DZ::WriteFile()
{
	// surface definition
	SetIniStr   ("Name",          m_sSurfaceName);
 	SetIniStr   ("PlotEquation",  m_zPlotEquation);
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
	SetIniDouble("XMin",          m_xmin);
	SetIniDouble("XMax",          m_xmax);
	SetIniDouble("YMin",          m_ymin);
	SetIniDouble("YMax",          m_ymax);
	SetIniDouble("ZMin",          m_zmin);
	SetIniDouble("ZMax",          m_zmax);
	SetIniLong  ("NXLines",       m_nulines);
	SetIniLong  ("NYLines",       m_nvlines);
	SetIniLong  ("ShowX",         m_draw_ulines);
	SetIniLong  ("ShowY",         m_draw_vlines);
	SetIniLong  ("LimitZ",        m_limit_z);
	SetIniLong  ("nXMesh",        m_nxMesh);
	SetIniLong  ("nYMesh",        m_nyMesh);

	return(0);
}

/*	<><><><><><><><><><><><><>  Plot3DZ.cpp  <><><><><><><><><><><><><><> */


















