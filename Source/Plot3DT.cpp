//	<><><><><><><><><><><><><>  Plot3DT.cpp  <><><><><><><><><><><><><><>
//		
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Implementation of PlotTriUV C++ class
//
// --------------------------------------------------------------------------
//

#include "stdafx.h"	// precompiled header
#include <stdio.h>
#include <float.h>
#include "Plot3DT.h"
#include "SurfX3D.h"


// ------------------------------------------------------------
//     PlotTriUV methods
// ------------------------------------------------------------

PlotTriUV::PlotTriUV()
{
	// set default values
	m_is_color_xyz = 0;
	DefInputsUV();
}

PlotTriUV::~PlotTriUV()
{
	FreeTriangles();
}

// ------------------------------------------------------------
void PlotTriUV::DefInputsUV(void)
{
	FreeTriangles();	// free memory and clear pointers

	// set default values
	m_back_color = DEF_BKGND_COLOR;
	m_is_color = 1;
	m_nulines  = 50;
	m_nvlines  = 50;
	m_draw_ulines   = 1;
	m_draw_vlines   = 1;
	m_umin = -10;
	m_umax =  10;
	m_vmin = -10;
	m_vmax =  10;
}

// ------------------------------------------------------------
#define	NORMAL_VECTOR_DELTA		(1000)	// delta for partial derivative calculation

// calculate points for U,V parametric mesh
// returns: 0=ok, 1=not enough memory
int PlotTriUV::CalcPointsUV()
{
	Point3DT* pPnt;
	double x,y,z;
	double x1,y1,z1;
	double x2,y2,z2;
	double dx1,dy1,dz1;
	double dx2,dy2,dz2;
	double nx,ny,nz, len;
	double du, dv, un, vn;
	double u,v;
	double udelta, vdelta;
	double red=PLOT3D_MAX_COLOR, green=PLOT3D_MAX_COLOR, blue=PLOT3D_MAX_COLOR;
	unsigned i,j;
    char   zbad;

    // initialize
    m_zbad = 0;

	// range check to avoid division by zero
	if (m_nulines<2) m_nulines=2;
	if (m_nvlines<2) m_nvlines=2;
	if (m_nxMesh <2) m_nxMesh =2;
	if (m_nyMesh <2) m_nyMesh =2;

	// generate points using parametric equations
	du = (m_umax - m_umin)/double(NORMAL_VECTOR_DELTA);
	dv = (m_vmax - m_vmin)/double(NORMAL_VECTOR_DELTA);

	// user supplied parametric equations
	udelta = (m_umax - m_umin)/double(m_nxMesh-1);
	vdelta = (m_vmax - m_vmin)/double(m_nyMesh-1);
	for (i=0, u=m_umin; i<m_nxMesh; i++,u+=udelta)
	{
		for (j=0, v=m_vmin; j<m_nyMesh; j++,v+=vdelta)
		{
			// calculate coordinates of point
			x  = XParametric(u, v);
			y  = YParametric(u, v);
			z  = ZParametric(u, v);
            zbad = IsInvalidZ();
			if (m_is_color_xyz)
			{
				red   = RedColor  (x,y,z);
				green = GreenColor(x,y,z);
				blue  = BlueColor (x,y,z);
			}
			else
			{
				red   = RedColor  (u,v,0);
				green = GreenColor(u,v,0);
				blue  = BlueColor (u,v,0);
			}
			pPnt = AddPoint(x,y,z,red,green,blue,zbad);

			// calculate normal surface vector at the point
			un = u+du;
			vn = v+dv;
			x1 = XParametric(un,v);
			y1 = YParametric(un,v);
			z1 = ZParametric(un,v);
			x2 = XParametric(u, vn);
			y2 = YParametric(u, vn);
			z2 = ZParametric(u, vn);
			dx1 = x1 - x;
			dy1 = y1 - y;
			dz1 = z1 - z;
			dx2 = x2 - x;
			dy2 = y2 - y;
			dz2 = z2 - z;
			nx =  dy1*dz2 - dy2*dz1;
			ny = -dx1*dz2 + dx2*dz1;
			nz =  dx1*dy2 - dx2*dy1;
			len = sqrt(nx*nx+ny*ny+nz*nz);
			if (len == 0.0)
			{
				nx = ny = nz = 0.;
			}
			else
			{
				// unit normal vector
				nx = nx/len;
				ny = ny/len;
				nz = nz/len;
			}
			// store the unit normal vector
			pPnt->SetNormVect(nx,ny,nz);
		} // for j
	} // for i

	// project points and calculate max and min projections
	// default min and max to extreme values
	m_xmin  = m_ymin  = m_zmin  =  DBL_MAX; // a very large number
	m_xmax  = m_ymax  = m_zmax  = -DBL_MAX; // a very small (negative) number
 	for (i=0; i<m_nxMesh; i++)
 	{
 		for (j=0; j<m_nyMesh; j++)
 		{
			pPnt = PointUV(i,j);
			x = pPnt->m_xf;
			y = pPnt->m_yf;
			z = pPnt->m_zf;
			// adjust min and max surface values
			if (x<m_xmin) m_xmin = x;
			if (y<m_ymin) m_ymin = y;
			if (z<m_zmin) m_zmin = z;
			if (x>m_xmax) m_xmax = x;
			if (y>m_ymax) m_ymax = y;
			if (z>m_zmax) m_zmax = z;
 		} // for j
 	} // for i
	return(0);
}

// ------------------------------------------------------------
int PlotTriUV::GenULines()
{
	CString sPercent;
	double x1,y1,z1,x2,y2,z2;
	double u,v,udelta,vdelta;
	unsigned i,j,percent_complete;
    char z1bad, z2bad;

	if (!m_draw_ulines) return(0);

	udelta = (m_umax - m_umin)/double(m_nulines-1);
	vdelta = (m_vmax - m_vmin)/double(m_nyMesh-1);
	for (i=0, u=m_umin; i<m_nulines; i++,u+=udelta)
	{
		if (IsAborted()) break;
		v = m_vmin;
		// calculate coordinates of point1
		x1 = XParametric(u, v);
		y1 = YParametric(u, v);
		z1 = ZParametric(u, v);
        z1bad = IsInvalidZ();
		v += vdelta;
		for (j=0; j<m_nyMesh-1; j++,v+=vdelta)
		{
			// calculate coordinates of point2
			x2 = XParametric(u, v);
			y2 = YParametric(u, v);
			z2 = ZParametric(u, v);
            z2bad = IsInvalidZ();

            if (z1bad==0 && z2bad==0)
    			AddLine(x1, y1, z1, x2, y2, z2); 
			x1 = x2;
			y1 = y2;
			z1 = z2;
            z1bad = z2bad;

		} // for j

		// show percent complete status
		percent_complete = (int)(100.*(i+1)/m_nulines);
		sPercent.Format("Generate First Contour Lines: %u%% complete", percent_complete);
		theApp.SetStatusString(sPercent);

	} // for i
	return(0);
}

// ------------------------------------------------------------
int PlotTriUV::GenVLines()
{
	CString sPercent;
	double x1,y1,z1,x2,y2,z2;
	double u,v,udelta,vdelta;
	unsigned i,j,percent_complete;
    char z1bad, z2bad;

	if (!m_draw_vlines) return(0);

	vdelta = (m_vmax - m_vmin)/double(m_nvlines-1);
	udelta = (m_umax - m_umin)/double(m_nxMesh-1);
	for (i=0, v=m_vmin; i<m_nvlines; i++,v+=vdelta)
	{
		if (IsAborted()) break;
		u = m_umin;
		// calculate coordinates of point1
		x1 = XParametric(u, v);
		y1 = YParametric(u, v);
		z1 = ZParametric(u, v);
        z1bad = IsInvalidZ();

		u += udelta;
		for (j=0; j<m_nxMesh-1; j++,u+=udelta)
		{
			// calculate coordinates of point2
			x2 = XParametric(u, v);
			y2 = YParametric(u, v);
			z2 = ZParametric(u, v);
            z2bad = IsInvalidZ();

            if (z1bad==0 && z2bad==0)
    			AddLine(x1, y1, z1, x2, y2, z2); 
			x1 = x2;
			y1 = y2;
			z1 = z2;
            z1bad = z2bad;

		} // for j

		// show percent complete status
		percent_complete = (int)(100.*(i+1)/m_nvlines);
		sPercent.Format("Generate Second Contour Lines: %u%% complete", percent_complete);
		theApp.SetStatusString(sPercent);

	} // for i
	return(0);
}

// ------------------------------------------------------------
// calculate triangles for U,V parametric mesh
// returns: 0=ok, 1=not enough memory
int PlotTriUV::CalcTrianglesUV()
{
	Triangle3DT* pTrngl;
	unsigned i,j,it;
	unsigned nutriangles, nvtriangles, ntriangles;

 	// allocate memory for triangles
	nutriangles = m_nxMesh-1;
	nvtriangles = m_nyMesh-1;
 	ntriangles  = nutriangles * nvtriangles * 2;

	// associate points to triangles and calculate maximums
	for (i=0,it=0; i<nutriangles; i++)
	{
		for (j=0; j<nvtriangles; j++)
		{
			pTrngl = AddTriangle(PointUV(i,j+1), PointUV(i,j),     PointUV(i+1,j));
			pTrngl = AddTriangle(PointUV(i+1,j), PointUV(i+1,j+1), PointUV(i,j+1));
		} // for j
	} // for i

	return(0);
}

// ------------------------------------------------------------
// returns: 0=ok
int PlotTriUV::InitPlotUV()
{
	if (InitEquations()) return(1);

	FreeTriangles();
	if (CalcPointsUV()) return(1);	// calculate mesh points
	if (CalcTrianglesUV()) return(1);

	return(0);
}

// ------------------------------------------------------------
void PlotTriUV::ResetUV(void)
{
	InitBase();
	DefInputsUV();
}

// ------------------------------------------------------------
// export file to Persistence-of-Vision
int PlotTriUV::ExportPOV(CString povFilePath,CString sPovObjectName)
{
	CString sPovTextureName = "Aluminum";
	Triangle3DT* pTri;
	Point3DT* Pnt1;
	Point3DT* Pnt2;
	Point3DT* Pnt3;
	int is_color=m_is_color;
	double xo,yo,zo; // object centroid
	double xc,yc,zc; // camera coordinate
	double xsky,ysky,zsky; // unit vector point to sky
	double xat,yat,zat; // vector pointing from camera to object centroid
	double xright,yright,zright; // unit vector pointing to right side of view
	double red_bg, green_bg, blue_bg; // color components
	double xlight[3], ylight[3], zlight[3]; // coordinates of light sources
	double at_dist;
	unsigned i,nt,nLines;
	long bgcolor;
	FILE* fhpov=0;

	if (!IsRendered()) return(0);

	CalcTransMatrix(m_xyMouseAngle,m_zMouseAngle);	// use current mouse viewing

	// open file for writing
	fhpov = fopen((LPCTSTR)povFilePath,"w");
	if (fhpov == NULL) return(1);

	// calculate background color values
	bgcolor  = GetBackColor();
	red_bg   = (double)GetRValue(bgcolor)/255.;
	green_bg = (double)GetGValue(bgcolor)/255.;
	blue_bg  = (double)GetBValue(bgcolor)/255.;

	// calculate camera and object coordinates
	GetCentroid(&xo,&yo,&zo); // get center of object
	GetPOVCamera(&xc,&yc,&zc);
	zc = -zc; // POV is a lefty
	zo = -zo; // POV is a lefty

	// calclulate sky unit vector
	xsky = xof(0., 1., 0.) - xof(0., 0., 0.);
	ysky = yof(0., 1., 0.) - yof(0., 0., 0.);
	zsky = zof(0., 1., 0.) - zof(0., 0., 0.);
	zsky = -zsky; // POV uses a left-hand coordinate system

	// calculate right unit vector
	xright = xof(1., 0., 0.) - xof(0., 0., 0.);
	yright = yof(1., 0., 0.) - yof(0., 0., 0.);
	zright = zof(1., 0., 0.) - zof(0., 0., 0.);
	zright = -zright; // POV is a lefty

	// calculate look_at vector
	xat = xc-xo;
	yat = yc-yo;
	zat = zc-zo;
	at_dist = sqrt(xat*xat+yat*yat+zat*zat); // distance from camera to object centroid

	// calculate light source coordinates
	// light#0
	xlight[0] = xo + (xat)*.6 + (xsky*at_dist)*0.6 + (xright*at_dist)*0.6;
	ylight[0] = yo + (yat)*.6 + (ysky*at_dist)*0.6 + (yright*at_dist)*0.6;
	zlight[0] = zo + (zat)*.6 + (zsky*at_dist)*0.6 + (zright*at_dist)*0.6;
	// light#1
	xlight[1] = xo + (xat)*.6 + (xsky*at_dist)*0.6 - (xright*at_dist)*0.6;
	ylight[1] = yo + (yat)*.6 + (ysky*at_dist)*0.6 - (yright*at_dist)*0.6;
	zlight[1] = zo + (zat)*.6 + (zsky*at_dist)*0.6 - (zright*at_dist)*0.6;
	// light#2
	xlight[2] = xo + (xat)*.9 - (xsky*at_dist)*0.6 + (xright*at_dist)*.2;
	ylight[2] = yo + (yat)*.9 - (ysky*at_dist)*0.6 + (yright*at_dist)*.2;
	zlight[2] = zo + (zat)*.9 - (zsky*at_dist)*0.6 + (zright*at_dist)*.2;

	// output POV header commands
	fprintf(fhpov,"// POV Image File Generated from SurfX3D\n");
	fprintf(fhpov,"//   Surface: %s\n\n", (LPCTSTR)sPovObjectName);
	fprintf(fhpov,"#include \"colors.inc\"\n");
	fprintf(fhpov,"#include \"textures.inc\"\n\n");
	fprintf(fhpov,"camera {\n");                 
	fprintf(fhpov,"  location <%.6lf, %.6lf, %.6lf>\n",xc,yc,zc); 
	fprintf(fhpov,"  // orthographic\n"); 
	fprintf(fhpov,"  sky <%.6lf, %.6lf, %.6lf>\n",xsky,ysky,zsky); 
	fprintf(fhpov,"  look_at <%.6lf, %.6lf, %.6lf>\n",xo,yo,zo);     
	fprintf(fhpov,"}\n");                        
	fprintf(fhpov,"light_source { <%.6lf, %.6lf, %.6lf> colour White }\n",xlight[0],ylight[0],zlight[0]);
	fprintf(fhpov,"light_source { <%.6lf, %.6lf, %.6lf> colour White }\n",xlight[1],ylight[1],zlight[1]);
	fprintf(fhpov,"light_source { <%.6lf, %.6lf, %.6lf> colour White }\n",xlight[2],ylight[2],zlight[2]);
	fprintf(fhpov,"background { color rgb < %.6lf, %.6lf, %.6lf > }\n", red_bg, green_bg, blue_bg);
	fprintf(fhpov,"\n");

	// loop through each triangle
	nt = TriangleCount();
	fprintf(fhpov,"#declare SURFACE1 = union { // define the object\n");
	for (i=0; i<nt; i++)
	{
		pTri = GetTrianglePtr(i);
		Pnt1 = pTri->GetPoint(0);
		Pnt2 = pTri->GetPoint(1);
		Pnt3 = pTri->GetPoint(2);
		fprintf(fhpov," smooth_triangle {<%.6lf,%.6lf,%.6lf>,<%.6lf,%.6lf,%.6lf>,<%.6lf,%.6lf,%.6lf>,<%.6lf,%.6lf,%.6lf>,<%.6lf,%.6lf,%.6lf>,<%.6lf,%.6lf,%.6lf>",
			Pnt1->m_xf,Pnt1->m_yf,-Pnt1->m_zf, -Pnt1->m_xn,-Pnt1->m_yn,Pnt1->m_zn,
			Pnt2->m_xf,Pnt2->m_yf,-Pnt2->m_zf, -Pnt2->m_xn,-Pnt2->m_yn,Pnt2->m_zn,
			Pnt3->m_xf,Pnt3->m_yf,-Pnt3->m_zf, -Pnt3->m_xn,-Pnt3->m_yn,Pnt3->m_zn);

		if (is_color)
		{
			fprintf(fhpov," pigment {color rgb <%.6lf,%.6lf,%.6lf>}}\n",
				pTri->RedTri()  /(double)PLOT3D_MAX_COLOR,	// 0-1
				pTri->GreenTri()/(double)PLOT3D_MAX_COLOR,	// 0-1
				pTri->BlueTri() /(double)PLOT3D_MAX_COLOR);	// 0-1
		}
		else
		{
			fprintf(fhpov,"}\n");
		}

	} // for i

	// ------------------------------
	// Export Lines as thin cylinders
	// ------------------------------
	nLines = LineCount();
	if (nLines)
	{
		Line3DT* pLine;
		double lineLen,dx,dy,dz;
		double rline = (float)(GetRValue(m_line_color)/255.);
		double gline = (float)(GetGValue(m_line_color)/255.);
		double bline = (float)(GetBValue(m_line_color)/255.);
		double lineRadius = max(fabs(m_xmax-m_xmin),fabs(m_ymax-m_ymin));
		lineRadius = max(fabs(m_zmax-m_zmin),lineRadius)/POV_LINE_RADIUS;
		for (i=0; i<nLines; i++)
	 	{
			pLine = GetLinePtr(i);
			dx=pLine->m_x2-pLine->m_x1;
			dy=pLine->m_y2-pLine->m_y1;
			dz=pLine->m_z2-pLine->m_z1;
			lineLen = sqrt(dx*dx+dy*dy+dz*dz);
			if (lineLen < lineRadius) continue;	// skip degenerate line
			fprintf(fhpov," cylinder { <%10.6lf,%10.6lf,%10.6lf>,<%10.6lf,%10.6lf,%10.6lf>,%10.6lf texture {pigment {color rgb <%.6lf,%.6lf,%.6lf>}}}\n",
				pLine->m_x1, pLine->m_y1, -pLine->m_z1, pLine->m_x2, pLine->m_y2, -pLine->m_z2,lineRadius,rline,gline,bline);
		} // for i
	}

	fprintf(fhpov," texture { %s } }\n", (LPCTSTR)sPovTextureName);

	// usage
	fprintf(fhpov,"\nobject { SURFACE1 } // use the object\n");

	// cleanup
	fclose(fhpov);
	return(0);
}

// ------------------------------------------------------------
// export file to Virtual-Reality-Modeling-Language (VRML) file format
int PlotTriUV::ExportVRML(CString sVrmlFilePath,CString sVrmlSurfaceName)
{
	Point3DT* pPnt;
	float red=0.,green=0.,blue=0.;
	int i1,i2,i3,i4;
	unsigned i,j,np,nLines;
	FILE* fhvrml=0;

	if (!IsRendered()) return(0);

	// open file for writing
	fhvrml = fopen((LPCTSTR)sVrmlFilePath,"w");
	if (fhvrml == NULL) return(1);

	// ----------------
	// VRML File Header
	// ----------------
	fprintf(fhvrml,"#VRML V2.0 utf8\n");
	fprintf(fhvrml,"# Generated by SurfX3D\n");
	fprintf(fhvrml,"\n");
	fprintf(fhvrml,"WorldInfo { title \"%s\" }\n", sVrmlSurfaceName);
	fprintf(fhvrml,"\n");

	// background color
	red   = (float)(GetRValue(m_back_color)/255.);
	green = (float)(GetGValue(m_back_color)/255.);
	blue  = (float)(GetBValue(m_back_color)/255.);

	fprintf(fhvrml,"Background {\n");
	fprintf(fhvrml,"  skyColor    [ %.4f %.4f %.4f ]", red, green, blue);
	fprintf(fhvrml,"  groundColor [ %.4f %.4f %.4f ]", red, green, blue);
	fprintf(fhvrml,"}");
	fprintf(fhvrml,"\n");

	// -------------------
	// Point Surface Shape
	// -------------------
	if (m_is_color)
	{
		fprintf(fhvrml,"DEF SURFACE1 Shape {\n");
		fprintf(fhvrml," geometry IndexedFaceSet {\n");
		fprintf(fhvrml,"   solid FALSE\n");

		// export coordinates
		fprintf(fhvrml,"   coord Coordinate {\n");
		fprintf(fhvrml,"     point [\n");
		for (j=np=0; j<m_nxMesh; j++)
	 	{
		 	for (i=0; i<m_nyMesh; i++)
	 		{
				pPnt = PointUV(i,j);
				fprintf(fhvrml,"      %10.6lf %10.6lf %10.6lf # %u\n", pPnt->m_xf, pPnt->m_yf, pPnt->m_zf, np++);
			} // for j
		} // for i
		fprintf(fhvrml,"     ]\n");	// point
		fprintf(fhvrml,"   }\n");	// coord

		// export color values
		fprintf(fhvrml,"   color Color\n");
		fprintf(fhvrml,"   {\n");
		fprintf(fhvrml,"     color [\n");
		for (j=np=0; j<m_nxMesh; j++)
	 	{
		 	for (i=0; i<m_nyMesh; i++)
	 		{
				pPnt = PointUV(i,j);
				fprintf(fhvrml,"      %10.6lf %10.6lf %10.6lf # %u\n", pPnt->m_red/100., pPnt->m_green/100., pPnt->m_blue/100., np++);
			} // for j
		} // for i
		fprintf(fhvrml,"     ]\n");
		fprintf(fhvrml,"   }\n");
	
		// face set normal vectors
		fprintf(fhvrml,"   normal Normal {\n");
		fprintf(fhvrml,"     vector [\n");
		for (j=np=0; j<m_nxMesh; j++)
		{
			for (i=0; i<m_nyMesh; i++)
			{
				pPnt = PointUV(i,j);
				fprintf(fhvrml,"      %10.6lf %10.6lf %10.6lf # %u\n", pPnt->m_xn, pPnt->m_yn, pPnt->m_zn, np++);
			} // for j
		} // for i
		fprintf(fhvrml,"     ]\n");
		fprintf(fhvrml,"   }\n");
	
		// face set coordinate indexes
		fprintf(fhvrml,"   coordIndex [\n");
		for (j=0; j<m_nxMesh-1; j++)
	 	{
		 	for (i=0; i<m_nyMesh-1; i++)
	 		{
				i1 = j*m_nxMesh+i;
				i2 = i1 + 1;
				i3 = i1 + m_nxMesh;
				i4 = i3 + 1;
				fprintf(fhvrml,"      %6u %6u %6u -1\n", i1,i2,i3);
				fprintf(fhvrml,"      %6u %6u %6u -1\n", i2,i4,i3);
			} // for j
		} // for i
		fprintf(fhvrml,"   ]\n");
	
		// trailer
		fprintf(fhvrml," }\n");
		fprintf(fhvrml,"}\n");
	} // m_is_color

	// -------------------------
	// Line Color Components 0-1
	// -------------------------
	red   = (float)(GetRValue(m_line_color)/255.);
	green = (float)(GetGValue(m_line_color)/255.);
	blue  = (float)(GetBValue(m_line_color)/255.);

	// ------------
	// Export Lines
	// ------------
	nLines = LineCount();
	if (nLines)
	{
		Line3DT* pLine;
		unsigned np;

		// export lines
		fprintf(fhvrml,"DEF LINESET2 Shape {\n");
		fprintf(fhvrml," appearance Appearance { material Material { emissiveColor %.4f %.4f %.4f } }\n", red, green, blue);
		fprintf(fhvrml," geometry IndexedLineSet {\n");
		fprintf(fhvrml," coord Coordinate {\n");
		fprintf(fhvrml,"   point [\n");
		// export points
		for (i=np=0; i<nLines; i++)
	 	{
			pLine = GetLinePtr(i);
			fprintf(fhvrml,"    %10.6lf %10.6lf %10.6lf # %u\n", pLine->m_x1, pLine->m_y1, pLine->m_z1, np++);
			fprintf(fhvrml,"    %10.6lf %10.6lf %10.6lf # %u\n", pLine->m_x2, pLine->m_y2, pLine->m_z2, np++);
		} // for i
		fprintf(fhvrml,"     ]\n");	// point
		fprintf(fhvrml,"   }\n");	// coord
		fprintf(fhvrml,"   coordIndex [\n");
		for (i=np=0; i<nLines; i++,np+=2)
		{
			fprintf(fhvrml,"      %6u %6u -1\n", np,np+1);
		} // for i
		fprintf(fhvrml,"   ]\n");
		fprintf(fhvrml," }\n");
		fprintf(fhvrml,"}\n");
	}

	// -------
	// cleanup
	// -------
	fclose(fhvrml);
	return(0);
}

// ------------------------------------------------------------
// export file to Drawing Exchange File (DXF) format
int PlotTriUV::ExportDXF(CString sDxfFilePath,CString sDxfSurfaceName)
{
	Point3DT* pPnt;
	unsigned i,j;
	FILE* fhdxf=0;

	if (!IsRendered()) return(0);

	// open file for writing
	fhdxf = fopen((LPCTSTR)sDxfFilePath,"w");
	if (fhdxf == NULL) return(1);

	// ---------------
	// DXF File Header
	// ---------------
	fprintf(fhdxf,"999\n");
	fprintf(fhdxf,"DXF created from SurfX3D: %s\n", sDxfSurfaceName);
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"SECTION\n");
	fprintf(fhdxf,"2\n");
	fprintf(fhdxf,"HEADER\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"ENDSEC\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"SECTION\n");
	fprintf(fhdxf,"2\n");
	fprintf(fhdxf,"TABLES\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"ENDSEC\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"SECTION\n");
	fprintf(fhdxf,"2\n");
	fprintf(fhdxf,"BLOCKS\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"ENDSEC\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"SECTION\n");
	fprintf(fhdxf,"2\n");
	fprintf(fhdxf,"ENTITIES\n");

	// -------------
	// Surface Shape
	// -------------
	for (j=0; j<m_nyMesh-1; j++)
	{
	 	for (i=0; i<m_nxMesh-1; i++)
		{
			fprintf(fhdxf,"0\n");
			fprintf(fhdxf,"3DFACE\n");
			fprintf(fhdxf,"8\n");
			fprintf(fhdxf,"0\n");
			fprintf(fhdxf,"70\n");
			fprintf(fhdxf,"0\n");

			pPnt = PointUV(i,j);
			fprintf(fhdxf,"10\n");
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_xf);
			fprintf(fhdxf,"20\n");
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_yf);
			fprintf(fhdxf,"30\n");
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_zf);

			pPnt = PointUV(i,j+1);
			fprintf(fhdxf,"11\n");
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_xf);
			fprintf(fhdxf,"21\n");	            
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_yf);
			fprintf(fhdxf,"31\n");	            
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_zf);

			pPnt = PointUV(i+1,j+1);
			fprintf(fhdxf,"12\n");
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_xf);
			fprintf(fhdxf,"22\n");	            
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_yf);
			fprintf(fhdxf,"32\n");	            
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_zf);

			pPnt = PointUV(i+1,j);
			fprintf(fhdxf,"13\n");
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_xf);
			fprintf(fhdxf,"23\n");	            
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_yf);
			fprintf(fhdxf,"33\n");	            
			fprintf(fhdxf,"%10.6lf\n",pPnt->m_zf);
		} // for j
	} // for i

	// -------
	// cleanup
	// -------
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"ENDSEC\n");
	fprintf(fhdxf,"0\n");
	fprintf(fhdxf,"EOF\n");

	fclose(fhdxf);
	return(0);
}

// ------------------------------------------------------------
// export file to Sterolithography (STL) file format
int PlotTriUV::ExportSTL(CString sStlFilePath,CString sStlSurfaceName)
{
	Point3DT* pPnt1;
	Point3DT* pPnt2;
	Point3DT* pPnt3;
	Point3DT* pPnt4;
    STL_HEADER   stlHdr;
    STL_TRIANGLE stlTri;
    double xn, yn, zn;
	unsigned i,j;
	FILE* fhstl=0;

	if (!IsRendered()) return(0);

	// open file for writing  (binary mode)
	fhstl = fopen((LPCTSTR)sStlFilePath,"wb");
	if (fhstl == NULL) return(1);

	// ---------------
	// STL File Header
	// ---------------
    memset(&stlHdr, 0, sizeof(stlHdr)); // clear
	sprintf((char*)&stlHdr.hdr, "Created from SurfX3D: %s", sStlSurfaceName);
    stlHdr.nTriangles = (2L*(m_nyMesh-1)*(m_nxMesh-1));
    fwrite(&stlHdr, 1, sizeof(stlHdr), fhstl);  // write header

	// -------------
	// Surface Shape
	// -------------
	for (j=0; j<m_nyMesh-1; j++)
	{
	 	for (i=0; i<m_nxMesh-1; i++)
		{
            // get four corners of mesh
			pPnt1 = PointUV(i,  j  );
			pPnt2 = PointUV(i,  j+1);
			pPnt3 = PointUV(i+1,j  );
			pPnt4 = PointUV(i+1,j+1);

            // clear triangle data
            memset(&stlTri, 0, sizeof(stlTri));

            // --------------
            // First Triangle
            //  Pnts 1,2,4
            // --------------
            // vertex 1
            stlTri.vertex1.x = (float)pPnt1->m_xf;
            stlTri.vertex1.y = (float)pPnt1->m_yf;
            stlTri.vertex1.z = (float)pPnt1->m_zf;
    
            // vertex 2
            stlTri.vertex2.x = (float)pPnt2->m_xf;
            stlTri.vertex2.y = (float)pPnt2->m_yf;
            stlTri.vertex2.z = (float)pPnt2->m_zf;
    
            // vertex 3
            stlTri.vertex3.x = (float)pPnt4->m_xf;
            stlTri.vertex3.y = (float)pPnt4->m_yf;
            stlTri.vertex3.z = (float)pPnt4->m_zf;
    
            // calc normal vector
            CalcNormalUnitVector(stlTri.vertex1.x, stlTri.vertex1.y, stlTri.vertex1.z,
                                 stlTri.vertex2.x, stlTri.vertex2.y, stlTri.vertex2.z,
                                 stlTri.vertex3.x, stlTri.vertex3.y, stlTri.vertex3.z,
                                               &xn,              &yn,              &zn);
            stlTri.normVector.x = (float)xn;
            stlTri.normVector.y = (float)yn;
            stlTri.normVector.z = (float)zn;
    
            // write triangle data to file
            fwrite(&stlTri, 1, sizeof(stlTri), fhstl);

            // ---------------
            // Second Triangle
            //  Pnts 1,3,4
            // ---------------
            // vertex 2
            stlTri.vertex2.x = (float)pPnt3->m_xf;
            stlTri.vertex2.y = (float)pPnt3->m_yf;
            stlTri.vertex2.z = (float)pPnt3->m_zf;
 
            // calc normal vector
            CalcNormalUnitVector(stlTri.vertex1.x, stlTri.vertex1.y, stlTri.vertex1.z,
                                 stlTri.vertex2.x, stlTri.vertex2.y, stlTri.vertex2.z,
                                 stlTri.vertex3.x, stlTri.vertex3.y, stlTri.vertex3.z,
                                               &xn,              &yn,              &zn);
            stlTri.normVector.x = (float)xn;
            stlTri.normVector.y = (float)yn;
            stlTri.normVector.z = (float)zn;
    
            // write triangle data to file
            fwrite(&stlTri, 1, sizeof(stlTri), fhstl);

		} // for j
	} // for i

	fclose(fhstl);
	return(0);
}

//	<><><><><><><><><><><><><>  Plot3DT.cpp  <><><><><><><><><><><><><><> //














