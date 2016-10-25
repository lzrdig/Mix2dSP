/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 2000
//
/////////////////////////////////////////////////////////////////////////////
// DlgListCtrl.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "DlgListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgListCtrl 


CDlgListCtrl::CDlgListCtrl(CWnd* pParent /*=NULL*/)
	: CDialogTts(CDlgListCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgListCtrl)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	m_nListCtrlColumns = 0;

	m_ListData.SetSize(m_nListCtrlColumns);
	m_ColumnLabel.SetSize(m_nListCtrlColumns);
	m_ColumnFormat.SetSize(m_nListCtrlColumns);
	m_ColumnWidth.SetSize(m_nListCtrlColumns);
}


void CDlgListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogTts::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgListCtrl)
	DDX_Control(pDX, IDC_LIST_CTRL, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgListCtrl, CDialogTts)
	//{{AFX_MSG_MAP(CDlgListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgListCtrl 

void CDlgListCtrl::SetListCtrlColumnCount(int nListCtrlColumns)
{
	m_ColumnLabel.SetSize(nListCtrlColumns);
	m_ColumnFormat.SetSize(nListCtrlColumns);
	m_ColumnWidth.SetSize(nListCtrlColumns);

	for (int k = m_nListCtrlColumns; k < nListCtrlColumns; k++)
	{
		// default settings
		m_ColumnLabel[k].Empty();
		m_ColumnFormat[k] = LVCFMT_LEFT;
		m_ColumnWidth[k] = 0;
	}

	m_nListCtrlColumns = nListCtrlColumns;
}

void CDlgListCtrl::SetListCtrlColumnLabel(int nListCtrlColumn, LPCTSTR lpszLabel)
{
	ASSERT(nListCtrlColumn < m_nListCtrlColumns);
	m_ColumnLabel[nListCtrlColumn] = lpszLabel;
}

void CDlgListCtrl::SetListCtrlColumnFormat(int nListCtrlColumn, int nFormat)
{
	ASSERT(nListCtrlColumn < m_nListCtrlColumns);
	ASSERT(nFormat == LVCFMT_BITMAP_ON_RIGHT || 
		nFormat == LVCFMT_COL_HAS_IMAGES  || 
		nFormat == LVCFMT_IMAGE  || 
		nFormat == LVCFMT_LEFT || 
		nFormat == LVCFMT_CENTER || 
		nFormat == LVCFMT_RIGHT);
	m_ColumnFormat[nListCtrlColumn] = nFormat;
}

void CDlgListCtrl::SetListCtrlColumnWidth(int nListCtrlColumn, int nWidth)
{
	ASSERT(nListCtrlColumn < m_nListCtrlColumns);
	ASSERT(nWidth >= 0);
	m_ColumnWidth[nListCtrlColumn] = nWidth;
}

void CDlgListCtrl::InsertListCtrlHeader()
{
	// insert columns
	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for (i = 0; i < m_nListCtrlColumns; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = m_ColumnLabel[i].GetBuffer(m_ColumnLabel[i].GetLength());
		m_ColumnLabel[i].ReleaseBuffer();
		lvc.fmt = m_ColumnFormat[i];
		lvc.cx = m_ColumnWidth[i];

		m_ListCtrl.InsertColumn(i, &lvc);
	}
}

void CDlgListCtrl::InsertListCtrlRow(int i)
{
	CString text;
	int size = m_ListData.GetSize();

	// insert item
	LV_ITEM lvi;
	text = i*m_nListCtrlColumns < size ? m_ListData[i*m_nListCtrlColumns] : "";

	lvi.mask = LVIF_TEXT | LVIF_STATE;
	lvi.iItem = i;
	lvi.iSubItem = 0;
	lvi.pszText = text.GetBuffer(text.GetLength());
	text.ReleaseBuffer();
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(1);

	m_ListCtrl.InsertItem(&lvi);

	// set item text for additional columns
	for (int k = 1; k < m_nListCtrlColumns; k++)
	{
		text = i*m_nListCtrlColumns+k < size ? m_ListData[i*m_nListCtrlColumns+k] : "";
		m_ListCtrl.SetItemText(i, k, text);
	}
}

BOOL CDlgListCtrl::OnInitDialog() 
{
	BOOL bRet = CDialogTts::OnInitDialog();

	m_ListCtrl.SetHighlightType(HIGHLIGHT_ROW);
	InsertListCtrlHeader();

	ASSERT(m_ListCtrl.GetItemCount() == 0);
	int size = (int)ceil((double)m_ListData.GetSize()/(double)m_nListCtrlColumns);
	int i;
	for (i = 0; i < size; i++)
		InsertListCtrlRow(i);

	i = 0;
	while (m_ListCtrl.SetColumnWidth(i++, LVSCW_AUTOSIZE_USEHEADER));

	SetWindowText(m_strWindowTitle);
	ResizeParentToFit();

	return bRet;
}

void CDlgListCtrl::ResizeParentToFit()
{
	// adjust parent rect so list control rect is appropriate size
	CRect rectCtrlWnd;
	CDC* pDC = m_ListCtrl.GetDC();
	int offset = pDC->GetTextExtent(_T(" "), 1).cx;
	m_ListCtrl.ReleaseDC(pDC);

	// Resize list control
	m_ListCtrl.GetWindowRect(&rectCtrlWnd);
	CSize sizeDiff = m_ListCtrl.ApproximateViewRect();
	sizeDiff.cx -= rectCtrlWnd.Width()-offset;
	sizeDiff.cy -= rectCtrlWnd.Height();

	ScreenToClient(&rectCtrlWnd);
	rectCtrlWnd.InflateRect(0, 0, sizeDiff.cx, sizeDiff.cy);
	m_ListCtrl.MoveWindow(rectCtrlWnd);

	// Move all other controls
	for (CWnd* pChild = GetWindow(GW_CHILD); pChild; pChild = pChild->GetNextWindow(GW_HWNDNEXT)) 
	{
		if (pChild == &m_ListCtrl)
			continue;

		pChild->GetWindowRect(&rectCtrlWnd);
		ScreenToClient(&rectCtrlWnd);
		rectCtrlWnd.OffsetRect(sizeDiff.cx, sizeDiff.cy);
		pChild->MoveWindow(rectCtrlWnd);
	}

	// Resize dialog
	GetWindowRect(&rectCtrlWnd);
	rectCtrlWnd.InflateRect(0, 0, sizeDiff.cx, sizeDiff.cy);

	// Invalidate the whole dialog to get it paint properly
	InvalidateRect(NULL, FALSE);
	MoveWindow(rectCtrlWnd);
}
