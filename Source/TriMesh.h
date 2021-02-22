//	<><><><><><><><><><><><><>  TriMesh.h  <><><><><><><><><><><><><><> 
//
// -----------------------------------------------------------------
// (C) Copyright Clark Dailey, Inc. 2006
//
// Basic classes for triangle mesh generation
//
// -----------------------------------------------------------------

#ifndef	_TRIMESH_H_	 	// avoid repeated including
#define	_TRIMESH_H_	1	// has been included

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define DEBUG_HASH_TABLE 1

#include "plot3d.h"

// ----------------
//   point class
// ----------------
class Point3DT
{
public:
	Point3DT() { }
	~Point3DT() { }
	Point3DT& operator=(const Point3DT &pt) // assignment operator
	{
		m_xf		  = pt.m_xf;        
		m_yf		  = pt.m_yf;        
		m_zf		  = pt.m_zf;        
		m_xn		  = pt.m_xn;        
		m_yn		  = pt.m_yn;        
		m_zn		  = pt.m_zn;        
		m_red		  = pt.m_red;       
		m_green		  = pt.m_green;     
		m_blue		  = pt.m_blue;      
		return(*this);
	}

	void Load(double x,double y,double z,double r,double g,double b)
	{
		m_xf    = x;
		m_yf    = y;
		m_zf    = z;
		m_red   = r;
		m_green = g;
		m_blue  = b;
	}

	void SetNormVect(double xn,double yn,double zn)	{ m_xn=xn; m_yn=yn; m_zn=zn; }

// public to avoid access functions
public: // 
	double m_xf,m_yf,m_zf;	// function coordinates
	double m_xn,m_yn,m_zn;	// normal surface vector
	double m_red, m_green, m_blue;	// color of point
};

// --------------
//  3D Line class
// --------------
class Line3DT
{
public:
	Line3DT() { }
	~Line3DT() { }
	void Load(double x1,double y1,double z1,double x2,double y2,double z2)
	{
		m_x1 = x1; 	m_y1 = y1;	m_z1 = z1;
		m_x2 = x2;	m_y2 = y2;	m_z2 = z2;
	}

	// output line to open gl
	void OutOpenGL(double xc,double yc,double zc)
	{
		::glVertex3d(m_x1-xc, m_y1-yc, -m_z1+zc);
		::glVertex3d(m_x2-xc, m_y2-yc, -m_z2+zc);
	}

	// data
public:
	double m_x1,m_y1,m_z1;
	double m_x2,m_y2,m_z2;
};


// ----------------
//  triangle class
// ----------------
class Triangle3DT
{
public:
	Triangle3DT() { }
	~Triangle3DT() { }
	Point3DT* GetPoint(unsigned index) { return(&m_Pnt[index]); }
	// get color of points
	double RedPnt  (unsigned i) { return(m_Pnt[i].m_red);   }
	double GreenPnt(unsigned i) { return(m_Pnt[i].m_green); }
	double BluePnt (unsigned i) { return(m_Pnt[i].m_blue);  }
	double RedTri  () { return(m_red);   }
	double GreenTri() { return(m_green); }
	double BlueTri () { return(m_blue);  }

	// get color of triangle
	long GetColor() { return(m_color); }
	void Load(Point3DT* pP1,Point3DT* pP2,Point3DT* pP3);
	void SetColor(double r,double g,double b);

	// cut triangle with X,Y,Z plane
	// returns: 0=ok (points valid), 1=no intersection
	int XSlice(double x,Point3DT* pP1,Point3DT* pP2);
	int YSlice(double y,Point3DT* pP1,Point3DT* pP2);
	int ZSlice(double z,Point3DT* pP1,Point3DT* pP2);

	// output triangle to open gl
	void OutOpenGL(double xc,double yc,double zc)
	{
		::glColor3d(m_Pnt[0].m_red/100.,m_Pnt[0].m_green/100.,m_Pnt[0].m_blue/100.);
		::glVertex3d(m_Pnt[0].m_xf-xc,m_Pnt[0].m_yf-yc,-m_Pnt[0].m_zf+zc);
		::glVertex3d(m_Pnt[1].m_xf-xc,m_Pnt[1].m_yf-yc,-m_Pnt[1].m_zf+zc);
		::glVertex3d(m_Pnt[2].m_xf-xc,m_Pnt[2].m_yf-yc,-m_Pnt[2].m_zf+zc);
	}

public: // data
	Point3DT m_Pnt[3]; // three defining points
	long   m_color;	// triangle color (Windows)
	double m_red; 	// 0-100
	double m_green;	// 0-100
	double m_blue; 	// 0-100
};

#endif	/* if _TRIMESH_H_ included */

/*	<><><><><><><><><><><><><>  TriMesh.h  <><><><><><><><><><><><><><> */
