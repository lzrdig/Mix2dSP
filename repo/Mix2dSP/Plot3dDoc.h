/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
// based on a sample of
// Pierre Alliez <pierre.alliez@cnet.francetelecom.fr>
//
/////////////////////////////////////////////////////////////////////////////
// OpenGLSampleDoc.h : Schnittstelle der Klasse COpenGLSampleDoc
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLSAMPLEDOC_H__D02404FF_14C5_11D2_B619_0000C0F55FDF__INCLUDED_)
#define AFX_OPENGLSAMPLEDOC_H__D02404FF_14C5_11D2_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "Matrix.h"
#include "Scale.h"
#include "GlDocument.h"
#include "mix2dsp.h"

#include "MixView.h"


class CPlot3dDoc : public CGlDocument
{
protected: // Nur aus Serialisierung erzeugen
	CPlot3dDoc();
	DECLARE_DYNCREATE(CPlot3dDoc)

// Attribute
public:
	// accessed by Plot2d, Plot3d and Surf
	CMatrix m_X;
	CMatrix m_Y;
	CMatrix m_Z;
	CMatrix m_X2;
	CMatrix m_Y2;
	CMatrix m_Z2;
	CMatrix m_C2;
	CScale m_S;
	KINDOFACTION m_nLastAction;
protected:
	CMatrix m_tmpMatrix;
	BOOL m_bIsValid;

// Operationen
public:
	void ClearViews(BOOL bUpdateViews = TRUE);
	void ShowViews(BOOL bActivate = TRUE);
	void LoadPlotData(CGraphDlg *pGraphDlg);
	void ShowPlot(CGraphDlg *pGraphDlg);

protected:
	void Scale2DView(const KINDOFSCALE& nKindOfScale, CScale& matS, CMatrix& matX, CMatrix& matY);
	void Scale3DView(const KINDOFSCALE& nKindOfScale, CScale& matS, CMatrix& matX, CMatrix& matY, CMatrix& matZ);
	void ComputeDemoOne();
	void ComputeDemoTwo();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CPlot3dDoc)
public:
	virtual void DeleteContents();
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnReopenDocument(BOOL bUpdateViews = TRUE);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CPlot3dDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CPlot3dDoc)
	afx_msg void OnDemoOne();
	afx_msg void OnDemoTwo();
	afx_msg void OnDemoZoom();
	afx_msg void OnDemoZoomIso();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_OPENGLSAMPLEDOC_H__D02404FF_14C5_11D2_B619_0000C0F55FDF__INCLUDED_)
