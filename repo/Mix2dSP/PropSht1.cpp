// PropSht1.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "PropSht1.h"
#include "Plot3dView.h"
#include "GraphDlg.h"


// CPropSht1 dialog

IMPLEMENT_DYNAMIC(CPropSht1, CDialog)

CPropSht1::CPropSht1(CWnd* pParent /*=NULL*/)
	: CDialog(CPropSht1::IDD, pParent)
	, m_bFrame(TRUE)
	, m_bAxisLabel(FALSE)
	, m_nGrid(1)
{

}

CPropSht1::~CPropSht1()
{
}

void CPropSht1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_FRAME, m_bFrame);
	DDX_Check(pDX, IDC_LABEL, m_bAxisLabel);
	DDX_Check(pDX, IDC_GRID, m_nGrid);
}


BEGIN_MESSAGE_MAP(CPropSht1, CDialog)
	ON_BN_CLICKED(IDC_FRAME, &CPropSht1::OnChckFrame)
	ON_BN_CLICKED(IDC_GRID, &CPropSht1::OnChkGrid)
END_MESSAGE_MAP()


// CPropSht1 message handlers

void CPropSht1::OnChckFrame()
{
	// TODO: Add your control notification handler code here
	CPlot3dView *pPlotView;
	CWnd *pParent;
	//CWnd *curParent;

	// For some reason it does not stop when pParent is of type CGraphDlg
	//curParent = this;
	//pParent = NULL;
	//if(curParent)
	//{
	//	while(curParent != NULL)
	//	{
	//		if(!curParent->IsKindOf(RUNTIME_CLASS(CGraphDlg)))
	//			pParent = curParent;
	//		curParent = curParent->GetParent();
	//	}
	//}

	pParent = GetParent()->GetParent();
	if(pParent)
	{
		pPlotView = ((CGraphDlg*)pParent)->m_pPlot3DView;
		if(pPlotView)
		{
			pPlotView->m_bScaleShowFrame = !m_bFrame;
			CWnd *p_cLabel = GetDlgItem(IDC_LABEL);
			CWnd *p_cGrid = GetDlgItem(IDC_GRID);
			if(!m_bFrame)
			{
				p_cLabel->EnableWindow(true);
				p_cGrid->EnableWindow(true);
			}
			else
			{
				p_cGrid->EnableWindow(false);
				p_cLabel->EnableWindow(false);
			}
			pPlotView->Invalidate();
			UpdateData(true);
		}
	}
}

void CPropSht1::OnChkGrid()
{
	// TODO: Add your control notification handler code here
	CPlot3dView *pPlotView;
	CWnd *pParent;
	//CWnd *curParent;

	pParent = GetParent()->GetParent();
	if(pParent)
	{
		pPlotView = (CPlot3dView *)(((CGraphDlg*)pParent)->m_pPlot3DView);
		if(pPlotView)
		{
			UpdateData(true);
			CWnd *p_cGrid = GetDlgItem(IDC_GRID);
			if(m_bFrame)
			{
				switch(m_nGrid)
				{
				case 1:
					p_cGrid->SetWindowTextA(_T("Grid (coarse)"));
					break;
				case 2:
					p_cGrid->SetWindowTextA(_T("Grid (fine)"));
					break;
				default :
					p_cGrid->SetWindowTextA(_T("Grid"));
					break;
				}
				pPlotView->m_nScaleShowGrid = m_nGrid;
			}
			pPlotView->Invalidate();
			UpdateData(true);
		}
	}
}
