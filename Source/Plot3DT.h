//	<><><><><><><><><><><><><>  Plot3DT.h  <><><><><><><><><><><><><><> 
//
// -----------------------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// PlotTriUV is a base class for triangle mesh plot using U,V parameters
//
// -----------------------------------------------------------------

#ifndef	_PLOT3DT_H_	 	// avoid repeated including
#define	_PLOT3DT_H_	1	// has been included

#include "plot3d.h"

// -----------------------------------------------------------
//  3D triangle mesh plotting class for U,V parametric format
//          used for explicit and parametric forms
// -----------------------------------------------------------

class PlotTriUV : public Plot3D
{
public:
	PlotTriUV();
	virtual ~PlotTriUV();

	int ExportPOV (CString  povFilePath,CString sPovObjectName);
	int ExportVRML(CString vrmlFilePath,CString sVrmlSurfaceName);
	int ExportDXF (CString sDxfFilePath,CString sDxfSurfaceName);
    int ExportSTL (CString sStlFilePath,CString sStlSurfaceName);

protected:	// routines
	// parametric equations
	virtual double XParametric(double u,double v)=0;
	virtual double YParametric(double u,double v)=0;
	virtual double ZParametric(double u,double v)=0;
	virtual int InitEquations(void)=0;
    char IsInvalidZ(void) { return(m_zbad); }

	// color info
	virtual double RedColor  (double u,double v,double w)=0;
	virtual double GreenColor(double u,double v,double w)=0;
	virtual double BlueColor (double u,double v,double w)=0;

	void ResetUV(void);
	int  InitPlotUV(void);
	int  CalcPointsUV(void);
	int  CalcTrianglesUV(void);
	void DefInputsUV(void);
	int  GenULines(void);
	int  GenVLines(void);
	Point3DT* PointUV(unsigned i,unsigned j)
	{
		unsigned index = (i*m_nyMesh) + (j);
		return(GetPointPtr(index));
	}

protected:
	// ----------
	// user input
	// ----------
	// parameters
	unsigned m_nulines, m_nvlines;	// number of u,v contour lines to draw
	double	 m_umin, m_umax;	   	// range of u parameter
	double	 m_vmin, m_vmax;	   	// range of v parameter

	// flags
	BOOL     m_draw_ulines;			// 0=no, 1=draw U lines
	BOOL     m_draw_vlines;			// 0=no, 1=draw V lines
	unsigned m_is_color_xyz;		// 0=u,v 1=color uses x,y,z
    char     m_zbad;                // 0=ok, 1=z is limited and outside range
};

#endif	/* if _PLOT3DT_H_ included */

/*	<><><><><><><><><><><><><>  Plot3DT.h  <><><><><><><><><><><><><><> */
