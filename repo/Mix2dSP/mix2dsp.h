// Mix2dsp.h : main header file for the MIX2DSP application
//

#if !defined(AFX_MIX2DSP_H__583C2D44_3D4A_11D4_9234_00A0240EAC17__INCLUDED_)
#define AFX_MIX2DSP_H__583C2D44_3D4A_11D4_9234_00A0240EAC17__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "GlWinApp.h"

// object of view
//	do not edit since we do 
//	maths upon these constants
typedef enum tagOBJECTOFVIEW
{
	OOV_NOTHING = 0,
	OOV_DEMO_SIMPLE = 1,
	OOV_DEMO_ZOOM_ISO = 2,
	OOV_DEMO_TWO_OBJECTS = 3,
	OOV_DEMO_ANI = 4
}
OBJECTOFVIEW;

// kind of view
//	do not edit since we do 
//	maths upon these constants
typedef enum tagKINDOFVIEW
{
	KOV_NOTHING = 0,
	KOV_PLOT_2D = 1,
	KOV_PLOT_3D = 2,
	KOV_SURF = 3,
	KOV_TOP_VIEW = 4
}
KINDOFVIEW;

// kind of scale
//	do not edit since we do 
//	maths upon these constants
typedef enum tagKINDOFSCALE
{
	KOS_NOTHING = 0,
	KOS_AXIS_EQUAL = 1,
	KOS_AXIS_EQUAL_CENTER = 2,
	KOS_AXIS_SINGLE_CENTER = 3,
	KOS_AXIS_SINGLE_CENTER_ZERO = 4
}
KINDOFSCALE;

// kind of (last) action
//	do not edit since we do 
//	maths upon these constants
typedef enum tagKINDOFACTION
{
	KOA_NOTHING = 0,
	KOA_DEMO_PLOT_2D = 1,
	KOA_DEMO_PLOT_3D = 2,
	KOA_DEMO_SURF = 3,
	KOA_DEMO_TOP_VIEW = 4,
	KOA_DEMO_ZOOM = 5,
	KOA_DEMO_ZOOM_ISOVIEW = 6,
	KOA_DEMO_TWO_OBJECTS = 7,
	KOA_DEMO_ANI = 8
}
KINDOFACTION;

/////////////////////////////////////////////////////////////////////////////
// CMix2dApp:
// See Mix2dsp.cpp for the implementation of this class
//

class CMix2dApp : public CGlWinApp
{
public:
	CMix2dApp();
	CFrameWnd* pPlotWnd;	// pointer to the frame of the graph window

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMix2dApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMix2dApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIX2DSP_H__583C2D44_3D4A_11D4_9234_00A0240EAC17__INCLUDED_)
