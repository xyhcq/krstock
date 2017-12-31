// FenBiWidget.cpp : implementation file
//

#include "stdafx.h"
#include "FenBiWidget.h"
#include "Widget/MemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFenBiWidget

CFenBiWidget::CFenBiWidget()
{
	m_bSetPosEnd = FALSE;
	m_VIndex = 0;
	m_KRScrollVBar.SetCWnd(this);
	ZeroMemory(&m_RTQ,sizeof(REALTIMEQUOTES));
	m_pFinData= NULL;
}

CFenBiWidget::~CFenBiWidget()
{

}


BEGIN_MESSAGE_MAP(CFenBiWidget, CStatic)
	//{{AFX_MSG_MAP(CFenBiWidget)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KRSDE_REALTIMEQUOTES_REMOTE,OnKRSDE_RealTimeQuotes_Remote)
	ON_MESSAGE(WM_KRSDE_TRADINGRECORD_REMOTE,OnKRSDE_TradingRecord_Remote)
	ON_MESSAGE(WM_SCROLLCHANGE,OnScrollChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFenBiWidget message handlers
INT CFenBiWidget::GetTextWidth(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cx;
}

INT CFenBiWidget::GetTextHeight(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cy;
}


BOOL CFenBiWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


void CFenBiWidget::DrawList(CDC * pDc)
{
	INT nTextHeight			= GetTextHeight(pDc,"亿") * 2 ,
		nSingeTextHeight	= GetTextHeight(pDc,"亿"),
		nTextWidth			= GetTextHeight(pDc,"亿");
	
	INT nRowCount = (float)(m_RCTextList.Height()-4) / (float)nTextHeight;
	
	CString szText;
	INT nDrawStringCount = 0 ,  nYOffset = 4;
	INT nBlockWidth = (m_RCTextList.Width() - m_KRScrollVBar.GetWidth() )  / 3;

	INT nXOffset1 = 4, nXOffset2 = nBlockWidth,nXOffset3=nBlockWidth+nBlockWidth;

	float fVolume,fAmount;
	CPen  WhitePen(PS_DOT,1,RGB(100,100,100));
	pDc->SelectObject(WhitePen);


	for (INT i= m_VIndex; i< m_VTradingRecord.size(); i++)
	{
		CTime t(m_VTradingRecord[i].m_lTime);
		szText.Format("%02u:%02u:%02u",t.GetHour(),t.GetMinute(),t.GetSecond());

		pDc->SetTextColor(COLOR_GRAY);
		pDc->TextOut(nXOffset1,nYOffset,szText);

		szText.Format(" %.2f",m_VTradingRecord[i].m_fNewPrice);
		pDc->SetTextColor(GetColor(m_RTQ.m_fLastClose,m_VTradingRecord[i].m_fNewPrice));
		pDc->TextOut(nXOffset2 ,nYOffset,szText);

		if(i == 0)
		{
			fVolume = m_VTradingRecord[i].m_fVolume;
			fAmount = m_VTradingRecord[i].m_fAmount;
			
		}else
		{
			fVolume = m_VTradingRecord[i].m_fVolume - m_VTradingRecord[i-1].m_fVolume;
			fAmount = m_VTradingRecord[i].m_fAmount - m_VTradingRecord[i-1].m_fAmount;
		}
		

		CHQFunction::FormatVolume(fVolume,szText,"-");
		pDc->SetTextColor( fAmount > 300000 ? COLOR_VIOLET:COLOR_YELLOW);
		pDc->TextOut(nXOffset3 ,nYOffset,szText);

		CHQFunction::FormatAmount(fAmount,szText,"-");
		pDc->SetTextColor( COLOR_GRAY );
		pDc->TextOut(nXOffset3 ,nYOffset + nSingeTextHeight,szText);

		if(m_pFinData )
		{
			switch(m_VTradingRecord[i].m_bMark)
			{
			case  TRADINGRECORD_MARK_NORMAL:
				pDc->SetTextColor(COLOR_WHITE);
				pDc->TextOut(nXOffset2 + nTextWidth,nYOffset + nSingeTextHeight,"-");
				break;
			case  TRADINGRECORD_MARK_ZDBUY:
				pDc->SetTextColor(COLOR_RED);
				pDc->TextOut(nXOffset2 + nTextWidth,nYOffset + nSingeTextHeight,"主动买");
				break;
			case  TRADINGRECORD_MARK_ZDSELL:
				pDc->SetTextColor(COLOR_GREEN);
				pDc->TextOut(nXOffset2 + nTextWidth,nYOffset + nSingeTextHeight,"主动卖");
				break;
			}
		}

		if(nDrawStringCount > nRowCount)
			break;	

		nDrawStringCount++;
		nYOffset+=nTextHeight;

		pDc->MoveTo(0,nYOffset);
		pDc->LineTo(m_RCTextList.right - m_KRScrollVBar.GetWidth(),nYOffset);
	}


	m_KRScrollVBar.SetRange(0,m_VTradingRecord.size() - nRowCount,FALSE);
	m_KRScrollVBar.ShowWindow(m_VTradingRecord.size() > nRowCount ? SW_SHOW:SW_HIDE);
	if(m_bSetPosEnd)
	{
		m_KRScrollVBar.SetPosEnd();
		m_bSetPosEnd = FALSE;
	}
}



void CFenBiWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if((HFONT)m_Font==NULL)
		m_Font.CreatePointFont( 100, "Fixedsys" );
	
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK())
	{
		//填充背景色
		CDC * pDc = MemDC.GetMemoryDC();
		CBrush bgbs(  RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE)  );
		pDc->FillRect(rc,&bgbs);
		pDc->SetBkMode(TRANSPARENT);
		pDc->SelectObject(m_Font);

//		CBrush RedBrush(COLOR_RED );
//		pDc->FrameRect(m_RCTextList,&RedBrush);
		if(m_VTradingRecord.size() == 0)
			return;

		DrawList(pDc);
		Graphics raphics( pDc->GetSafeHdc() );
		raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		raphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
		m_KRScrollVBar.OnPaint(raphics,m_RCTextList);
	}
	// Do not call CStatic::OnPaint() for painting messages
}





void CFenBiWidget::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_RCTextList.left= 0;
	m_RCTextList.top = 0;
	m_RCTextList.right = cx;
	m_RCTextList.bottom = cy;
	RedrawWindow();
}



void CFenBiWidget::SetLabel( const char * szLabel )
{
	const REALTIMEQUOTES * pRTQ = KRSDE.GetRealTimeQuotes(szLabel);
	if(pRTQ == NULL)
		return;

	m_RTQ = *pRTQ;
	KRSDE.GetTradingRecord_Remote(szLabel,0,0,0);
}




LRESULT CFenBiWidget::OnKRSDE_TradingRecord_Remote(WPARAM wParam, LPARAM lParam)
{
	LPTOKEN_TRADINGRECORD_HEADINFO	pTTRHeader = (LPTOKEN_TRADINGRECORD_HEADINFO)wParam;
	LPTRADINGRECORD					pTTR       = (LPTRADINGRECORD)lParam;
	m_VTradingRecord.assign(pTTR,pTTR + pTTRHeader->dwCount);
	m_bSetPosEnd=TRUE;
	m_pFinData	= KRSDE.GetFinancial((const char * )&m_RTQ.m_wMarket);

	if(GetSafeHwnd()) 
		RedrawWindow();
	return 0;
}



LRESULT CFenBiWidget::OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam)
{
	if(m_RTQ.m_wMarket == 0)
		return FALSE;
	
	const REALTIMEQUOTES * pBegin = (const REALTIMEQUOTES *)wParam;
	const REALTIMEQUOTES * pRTQ= NULL;
	for (INT i=0; i< lParam;i++)
	{
		if(stricmp((const char * )&pBegin->m_wMarket , (const char * )&m_RTQ.m_wMarket) == 0)
		{
			pRTQ = pBegin;
			break;
		}
		pBegin++;
	}
	if(pRTQ == NULL)	
		return FALSE;

	if(Amount_Equality(pRTQ->m_fAmount , m_RTQ.m_fAmount))
		return FALSE;

	m_pFinData	= KRSDE.GetFinancial((const char * )&m_RTQ.m_wMarket);
	KRSDE.GetTradingRecord_Remote((const char * )&m_RTQ.m_wMarket,0,0,0);
	return TRUE;
}




int CFenBiWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	KRSDE.SetHWND(m_hWnd);
	return 0;
}


LRESULT CFenBiWidget::OnScrollChange(LPARAM lParam,WPARAM wParam)
{
	m_VIndex = m_KRScrollVBar.GetPos();
	RedrawWindow();
	return TRUE;
}


BOOL CFenBiWidget::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_KRScrollVBar.PreTranslateMessage(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}
