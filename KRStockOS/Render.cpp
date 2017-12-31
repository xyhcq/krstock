#include "StdAfx.h"
#include "Render.h"

extern CKRStockDataEngine KRSDE;

CKLineAnalyze::CKLineAnalyze()
{
	m_bFirst = TRUE;
}

void CKLineAnalyze::Create(CWnd * pWnd)
{

	m_KLineWidget.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),pWnd,KLineAnalyze_ID_KLineWidget);
	m_HQWidget.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(0,0,0,0),pWnd,KLineAnalyze_ID_HQWidget);
	m_AmountAnalyzeWidget.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(0,0,0,0),pWnd,KLineAnalyze_ID_AmountAnalyzeWidget);
	m_FenBiWidget.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(0,0,0,0),pWnd,KLineAnalyze_ID_FenBiWidget);
	m_StockToolsWiget.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(0,0,0,0),pWnd,KLineAnalyze_ID_ToolsWidget);
	m_SplitterColWidget.BindLeft(&m_KLineWidget);
}




void CKLineAnalyze::AdjuestWidget(int left, int top, int right, int bottom)
{
	m_Rect.left  = left   + 2;
	m_Rect.top   = top    + 2;
	m_Rect.right = right  - 2;
	m_Rect.bottom= bottom - 2;
	
	m_SplitterColWidget.SetRect(m_Rect);

	if(m_bFirst)
	{
		m_SplitterColWidget.SetLine(m_Rect.Width() - m_HQWidget.GetDefaultWidth() , SCW_FixedMode_Right);

		m_SplitterColWidget.BindRightRow(&m_SplitterRowidget);

		CRect RCColRight;
		RCColRight = m_SplitterColWidget.GetRightRect();
		
		m_SplitterRowidget.BindTop(&m_HQWidget);
		m_SplitterRowidget.SetRect(RCColRight);
		m_SplitterRowidget.SetLine(m_HQWidget.GetDefaultHeight() , SRW_FixedMode_Top);

		CRect RCRowBottom = m_SplitterRowidget.GetBottomRect();
		m_SplitterRowidget.BindBottomRow(&m_SplitterSubRowidget);

		m_SplitterSubRowidget.BindTop(&m_AmountAnalyzeWidget);
		m_SplitterSubRowidget.BindBottom(&m_FenBiWidget);
		m_SplitterSubRowidget.SetRect(RCRowBottom);
		

		m_bFirst = FALSE;
	}

	CRect RCRowBottom = m_SplitterRowidget.GetBottomRect();
	if(RCRowBottom.Height() > m_AmountAnalyzeWidget.GetDefaultHeight() )
	{
		m_SplitterSubRowidget.SetLine(RCRowBottom.top + m_AmountAnalyzeWidget.GetDefaultHeight(),SRW_FixedMode_Top);
	}else
	{
		m_SplitterSubRowidget.SetLine(RCRowBottom.top + 22,SRW_FixedMode_Top);
	}
}



void CKLineAnalyze::SetLabel( const char * szLabel )
{
	m_KLineWidget.SetLabel(szLabel);
	m_HQWidget.SetLabel(szLabel);
	m_AmountAnalyzeWidget.SetLabel(szLabel);
	m_FenBiWidget.SetLabel(szLabel);
	m_StockToolsWiget.SetLabel(szLabel);
}



BOOL CKLineAnalyze::PreTranslateMessage( CWnd * pWnd,MSG* pMsg )
{
	return m_SplitterColWidget.PreTranslateMessage(pWnd,pMsg);
}
