/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2000
//
// based on a sample of
// Alessandro Falappa
//
/////////////////////////////////////////////////////////////////////////////
// DlgOpenGLInfo.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "DlgOpenGLInfo.h"
#include "DlgListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenGLInfo 


CDlgOpenGLInfo::CDlgOpenGLInfo(CWnd* pParent /*=NULL*/)
	: CDialogTts(CDlgOpenGLInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOpenGLInfo)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CDlgOpenGLInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogTts::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpenGLInfo)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_GL_VENDOR, m_info.strGlVendor);
	DDX_Text(pDX, IDC_STATIC_GL_RENDERER, m_info.strGlRenderer);
	DDX_Text(pDX, IDC_STATIC_GL_VERSION, m_info.strGlVersion);
	DDX_Text(pDX, IDC_STATIC_GL_ACCELERATION, m_info.strGlAcceleration);
	DDX_Text(pDX, IDC_EDIT_GL_EXTENSIONS, m_info.strGlExtensions);
	DDX_Text(pDX, IDC_STATIC_GLU_VERSION, m_info.strGluVersion);
	DDX_Text(pDX, IDC_EDIT_GLU_EXTENSIONS, m_info.strGluExtensions);
}


BEGIN_MESSAGE_MAP(CDlgOpenGLInfo, CDialogTts)
	//{{AFX_MSG_MAP(CDlgOpenGLInfo)
	ON_BN_CLICKED(IDC_BUTTON_DETAILS, OnButtonDetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgOpenGLInfo 

void CDlgOpenGLInfo::OnButtonDetails() 
{
	CDlgListCtrl dlg;
	CString strDetails, strDummy;

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_MAX_LIGHTS);
	strDetails.Format("%d", m_info.nMaxLights);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_CLIP_PLANES);
	strDetails.Format("%d", m_info.nMaxClipPlanes);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_MAX_MODEL_STACK_DEPTH);
	strDetails.Format("%d", m_info.nMaxModelStackDepth);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_PROJ_STACK_DEPTH);
	strDetails.Format("%d", m_info.nMaxProjectionStackDepth);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_TEX_STACK_DEPTH);
	strDetails.Format("%d", m_info.nMaxTextureStackDepth);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_NAME_STACK_DEPTH);
	strDetails.Format("%d", m_info.nMaxNameStackDepth);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_ATTR_STACK_DEPTH);
	strDetails.Format("%d", m_info.nMaxAttributeStackDepth);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_MAX_TEX_SIZE);
	strDetails.Format("%d", m_info.nMaxTextureSize);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_LIST_NESTING);
	strDetails.Format("%d", m_info.nMaxListNesting);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_EVAL_ORDER);
	strDetails.Format("%d", m_info.nMaxEvalOrder);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_MAX_VIEWPORT_DIMS);
	strDetails.Format("%dx%d", m_info.maxViewportDims.cx, m_info.maxViewportDims.cy);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_AUX_BUF);
	strDetails.Format("%d", m_info.nAuxiliaryBuffers);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_SMOOTH_POINT_SIZE);
	strDetails.Format("%g-%g", m_info.minSmoothPointSize, m_info.maxSmoothPointSize);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_SMOOTH_POINT_GRAN);
	strDetails.Format("%g", m_info.smoothPointGranularity);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_SMOOTH_LINE_SIZE);
	strDetails.Format("%g-%g", m_info.minSmoothLineSize, m_info.maxSmoothLineSize);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_SMOOTH_LINE_GRAN);
	strDetails.Format("%g", m_info.smoothLineGranularity);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(strDummy);
	dlg.AddDataString(strDummy);

	dlg.AddDataString(IDS_DATA_COLOR_BITS);
	strDetails.Format(IDS_FORMAT_COLOR_BITS, m_info.nRedBits, m_info.nGreenBits, m_info.nBlueBits, m_info.nAlphaBits);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_DEPTH_BITS);
	strDetails.Format("%d Bit", m_info.nDepthBits);
	dlg.AddDataString(strDetails);

	dlg.AddDataString(IDS_DATA_STENCIL_BITS);
	strDetails.Format("%d Bit", m_info.nStencilBits);
	dlg.AddDataString(strDetails);

	strDetails.LoadString(IDS_DETAIL_OGL_INFO_TITLE);
	dlg.SetWindowTitleString(strDetails);
	if (dlg.GetDataCount() > 0)
	{
		dlg.SetListCtrlColumnCount(2);
		strDetails.LoadString(IDS_STR_IDENT);
		dlg.SetListCtrlColumnLabel(0, strDetails);
		strDetails.LoadString(IDS_STR_VAL);
		dlg.SetListCtrlColumnLabel(1, strDetails);
	}

	dlg.m_ListCtrl.SetReadOnly();
	dlg.DoModal();
}

BOOL CDlgOpenGLInfo::OnInitDialog() 
{
	// convert the spaces in newlines (in extensions fields) 
	int pos;

	pos = 0;
	while ((pos = m_info.strGlExtensions.Find(_T(' '))) != -1)
	{
		// substitute space with LF
		m_info.strGlExtensions.SetAt(pos, _T('\n'));
		// insert a CR before LF
#if _MSC_VER >= 1200
		m_info.strGlExtensions.Insert(pos, _T('\r'));
#else // _MSC_VER >= 1200
		m_info.strGlExtensions = m_info.strGlExtensions.Left(pos) + _T('\r') + m_info.strGlExtensions.Right(m_info.strGlExtensions.GetLength() - pos);
#endif // _MSC_VER >= 1200
	}
	pos = 0;
	while ((pos = m_info.strGluExtensions.Find(_T(' '))) != -1)
	{
		// substitute space with LF
		m_info.strGluExtensions.SetAt(pos, _T('\n'));
		// insert a CR before LF
#if _MSC_VER >= 1200
		m_info.strGluExtensions.Insert(pos, _T('\r'));
#else // _MSC_VER >= 1200
		m_info.strGluExtensions = m_info.strGluExtensions.Left(pos) + _T('\r') + m_info.strGluExtensions.Right(m_info.strGluExtensions.GetLength() - pos);
#endif // _MSC_VER >= 1200
	}

	BOOL bRet = CDialogTts::OnInitDialog();

	return bRet;
}
