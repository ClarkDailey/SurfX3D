//	<><><><><><><><><><><><><>  TriMesh.cpp  <><><><><><><><><><><><><><> */
//		
// ----------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Implementation of TriMesh C++ class
//
// --------------------------------------------------------------------------
//

#include "stdafx.h"	// precompiled header
#include <stdio.h>
#include <float.h>
#include "SurfX3D.h"

// ------------------------------------------------------------
//     Triangle3DT methods
// ------------------------------------------------------------

void Triangle3DT::SetColor(double r,double g,double b)
{
	int ir,ig,ib;
	// r,g,b (1->100)
	m_red   = r;
	m_green = g;
	m_blue  = b;
	// convert to 0-255 scale
	ir = PCOLOR_TO_WINCOLOR(r);
	ig = PCOLOR_TO_WINCOLOR(g);
	ib = PCOLOR_TO_WINCOLOR(b);
	if (ir<0) ir=0;
	if (ig<0) ig=0;
	if (ib<0) ib=0;
	if (ir>255) ir=255;
	if (ig>255) ig=255;
	if (ib>255) ib=255;
	m_color = RGB(ir,ig,ib); // convert to RGB
}

// ------------------------------------------------------------
void Triangle3DT::Load(Point3DT* pP1,Point3DT* pP2,Point3DT* pP3)
{
 	m_Pnt[0] = *pP1;
 	m_Pnt[1] = *pP2;
 	m_Pnt[2] = *pP3;
	m_color=PLOT_COLOR_WHITE;
}

inline IsInBetween(double a,double b,double c)
{
	return((a>=b && a<=c) || (a>=c && a<=b)?1:0);
}


// ------------------------------------------------------------
// cut triangle with X plane
// returns: 0=ok (points valid), 1=no intersection
int Triangle3DT::XSlice(double x,Point3DT* pP1,Point3DT* pP2)
{
	double xcut[3],ycut[3],zcut[3];
	double dx,dy,dz,y,z;
	unsigned int i,j,nCuts=0;
	
	// check between points
	for (i=0; i<3; i++)
	{
		j = i+1;
		if (j>2) j=0;
		if (!IsInBetween(x, m_Pnt[i].m_xf, m_Pnt[j].m_xf)) continue;
		
		dx = m_Pnt[i].m_xf - m_Pnt[j].m_xf;
		if (dx==0.0)
		{
			dy = dz = 0.0;
		}
		else
		{
			// equal ratios
			dx = (x -  m_Pnt[i].m_xf)/dx;
			dy = dx * (m_Pnt[i].m_yf - m_Pnt[j].m_yf);
			dz = dx * (m_Pnt[i].m_zf - m_Pnt[j].m_zf);
		}
		y = m_Pnt[i].m_yf + dy;
		z = m_Pnt[i].m_zf + dz;
		xcut[nCuts] = x;
		ycut[nCuts] = y;
		zcut[nCuts] = z;
		nCuts++;
	}

	if (nCuts<2) return(1);

	// return intersection points
	pP1->m_xf = xcut[0];
	pP1->m_yf = ycut[0];
	pP1->m_zf = zcut[0];
	pP2->m_xf = xcut[1];
	pP2->m_yf = ycut[1];
	pP2->m_zf = zcut[1];
	return(0);
}

// ------------------------------------------------------------
// cut triangle with Y plane
// returns: 0=ok (points valid), 1=no intersection
int Triangle3DT::YSlice(double y,Point3DT* pP1,Point3DT* pP2)
{
	double xcut[3],ycut[3],zcut[3];
	double dx,dy,dz,x,z;
	unsigned int i,j,nCuts=0;
	
	// check between points
	for (i=0; i<3; i++)
	{
		j = i+1;
		if (j>2) j=0;
		if (!IsInBetween(y, m_Pnt[i].m_yf, m_Pnt[j].m_yf)) continue;
		
		dy = m_Pnt[i].m_yf - m_Pnt[j].m_yf;
		if (dy==0.0)
		{
			dx = dz = 0.0;
		}
		else
		{
			// equal ratios
			dy = (y -  m_Pnt[i].m_yf)/dy;
			dx = dy * (m_Pnt[i].m_xf - m_Pnt[j].m_xf);
			dz = dy * (m_Pnt[i].m_zf - m_Pnt[j].m_zf);
		}
		x = m_Pnt[i].m_xf + dx;
		z = m_Pnt[i].m_zf + dz;
		xcut[nCuts] = x;
		ycut[nCuts] = y;
		zcut[nCuts] = z;
		nCuts++;
	}

	if (nCuts<2) return(1);

	// return intersection points
	pP1->m_xf = xcut[0];
	pP1->m_yf = ycut[0];
	pP1->m_zf = zcut[0];
	pP2->m_xf = xcut[1];
	pP2->m_yf = ycut[1];
	pP2->m_zf = zcut[1];
	return(0);
}

// ------------------------------------------------------------
// cut triangle with Z plane
// returns: 0=ok (points valid), 1=no intersection
int Triangle3DT::ZSlice(double z,Point3DT* pP1,Point3DT* pP2)
{
	double xcut[3],ycut[3],zcut[3];
	double dx,dy,dz,x,y;
	unsigned int i,j,nCuts=0;
	
	// check between points
	for (i=0; i<3; i++)
	{
		j = i+1;
		if (j>2) j=0;
		if (!IsInBetween(z, m_Pnt[i].m_zf, m_Pnt[j].m_zf)) continue;
		
		dz = m_Pnt[i].m_zf - m_Pnt[j].m_zf;
		if (dz==0.0)
		{
			dx = dy = 0.0;
		}
		else
		{
			// equal ratios
			dz = (z -  m_Pnt[i].m_zf)/dz;
			dx = dz * (m_Pnt[i].m_xf - m_Pnt[j].m_xf);
			dy = dz * (m_Pnt[i].m_yf - m_Pnt[j].m_yf);
		}
		x = m_Pnt[i].m_xf + dx;
		y = m_Pnt[i].m_yf + dy;
		xcut[nCuts] = x;
		ycut[nCuts] = y;
		zcut[nCuts] = z;
		nCuts++;
	}

	if (nCuts<2) return(1);

	// return intersection points
	pP1->m_xf = xcut[0];
	pP1->m_yf = ycut[0];
	pP1->m_zf = zcut[0];
	pP2->m_xf = xcut[1];
	pP2->m_yf = ycut[1];
	pP2->m_zf = zcut[1];
	return(0);
}

//	<><><><><><><><><><><><><>  TriMesh.cpp  <><><><><><><><><><><><><><> //














