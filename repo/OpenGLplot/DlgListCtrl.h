/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 2000
//
/////////////////////////////////////////////////////////////////////////////
// DlgListCtrl.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGLISTCTRL_H__DE84C9E4_73B0_11D4_8C19_00001CD5E4D1__INCLUDED_)
#define AFX_DLGLISTCTRL_H__DE84C9E4_73B0_11D4_8C19_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DialogTts.h"
#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgListCtrl 

class CDlgListCtrl : public CDialogTts
{
// Konstruktion
public:
	CDlgListCtrl(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgListCtrl)
	enum { IDD = IDD_DLG_LIST_CTRL };
	CListCtrlEx	m_ListCtrl;
	//}}AFX_DATA

protected:
	CString m_strWindowTitle;
	int m_nListCtrlColumns;
	CStringArray m_ListData;
	CStringArray m_ColumnLabel;
	CArray<int, int> m_ColumnFormat;
	CArray<int, int> m_ColumnWidth;


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgListCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
public:
	int GetListCtrlColumnCount() { return m_nListCtrlColumns; };
	int GetDataCount() { return m_ListData.GetSize(); };
	void SetWindowTitleString(LPCTSTR lpszTitle) { m_strWindowTitle = lpszTitle; };
	void SetListCtrlColumnCount(int nListCtrlColumns);
	void SetListCtrlColumnLabel(int nListCtrlColumn, LPCTSTR lpszLabel);
	void SetListCtrlColumnFormat(int nListCtrlColumn, int nFormat);
	void SetListCtrlColumnWidth(int nListCtrlColumn, int nWidth);
	void AddDataString(CString str) { m_ListData.Add(str); };
	void AddDataString(UINT nID) { CString str; str.LoadString(nID); m_ListData.Add(str); };

	void ResizeParentToFit();

protected:
	void InsertListCtrlHeader();
	void InsertListCtrlRow(int i);

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgListCtrl)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DLGLISTCTRL_H__DE84C9E4_73B0_11D4_8C19_00001CD5E4D1__INCLUDED_
