/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: September 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlDocument.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLDOCUMENT_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_GLDOCUMENT_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "WatchInThreadBase.h"

class CGlView;

class AFX_EXT_CLASS_GENERAL CGlDocument : public CDocument, public CWatchInThreadBase
{
	DECLARE_DYNAMIC(CGlDocument)
public:
	CGlDocument();

// Attributes
public:

// Operations
public:
	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	void AddStarToTitle();
	void RemoveStarFromTitle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlDocument)
	public:
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo, CGlView* pCallingView);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL OnReopenDocument(BOOL bUpdateViews = TRUE);
	virtual int OnFileAlarm(int nCause, LPVOID pExtra);
	protected:
	virtual BOOL SaveModified();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	virtual BOOL PostFileAlarmMsg(UINT message, int nCause);
	//}}AFX_VIRTUAL
	void CheckVersion(const CArchive& ar, const CString& strFileVersion);

// Implementation
public:
	virtual ~CGlDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	static UINT WatchThreadProc(LPVOID pParam);

protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CGlDocument)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline BOOL CGlDocument::PostFileAlarmMsg(UINT message, int nCause)
	{ return AfxGetApp()->PostThreadMessage(message, (WPARAM)this, (LPARAM)nCause); }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_GLDOCUMENT_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
