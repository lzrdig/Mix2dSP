// MixDoc.h : interface of the CMixDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXDOC_H__583C2D4A_3D4A_11D4_9234_00A0240EAC17__INCLUDED_)
#define AFX_MIXDOC_H__583C2D4A_3D4A_11D4_9234_00A0240EAC17__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMixDoc : public CDocument
{
protected: // create from serialization only
	CMixDoc();
	DECLARE_DYNCREATE(CMixDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMixDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMixDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMixDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIXDOC_H__583C2D4A_3D4A_11D4_9234_00A0240EAC17__INCLUDED_)
