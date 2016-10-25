/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: May 2002
//
/////////////////////////////////////////////////////////////////////////////
// ButtonEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTONEX_H__D37C5C14_4A89_47F9_B216_5C271507B0CD__INCLUDED_)
#define AFX_BUTTONEX_H__D37C5C14_4A89_47F9_B216_5C271507B0CD__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Fenster CButtonEx 

class AFX_EXT_CLASS_GENERAL CButtonEx : public CButton
{
// Konstruktion
public:
	CButtonEx();

// Attribute
private:
	static CFont m_SymbolFont;
	static int m_nInstanceCounter;
	BOOL m_bDoReadOnly;
	int m_nCheck;
protected:
	BOOL m_bIsReadOnly;
	UINT m_nStyle;

// Operationen
private:
	static void InitializeSymbolFont()
	{
		LOGFONT lf;
		lf.lfHeight = -13; 
		lf.lfWidth = 0; 
		lf.lfEscapement = 0; 
		lf.lfOrientation = 0; 
		lf.lfWeight = FW_NORMAL; 
		lf.lfItalic = FALSE; 
		lf.lfUnderline = FALSE; 
		lf.lfStrikeOut = FALSE; 
		lf.lfCharSet = DEFAULT_CHARSET; 
		lf.lfOutPrecision = OUT_TT_PRECIS; 
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
		lf.lfQuality = DEFAULT_QUALITY; 
		lf.lfPitchAndFamily = DEFAULT_PITCH | TMPF_TRUETYPE | FF_DECORATIVE; 

		CString strFontName("Marlett");
		memset(lf.lfFaceName, 0, sizeof(lf.lfFaceName));
		_tcsncpy(lf.lfFaceName, strFontName, min(32-1, strFontName.GetLength()));

		m_SymbolFont.CreateFontIndirect(&lf);
	}
protected:
	BOOL AllowReadOnly();
public:
	BOOL SetReadOnly(BOOL bReadOnly = TRUE);
	BOOL IsReadOnly() const;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CButtonEx();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	afx_msg LRESULT OnBmGetcheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBmSetcheck(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CButtonEx)
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

inline BOOL CButtonEx::IsReadOnly() const
	{ return m_bIsReadOnly; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_BUTTONEX_H__D37C5C14_4A89_47F9_B216_5C271507B0CD__INCLUDED_
