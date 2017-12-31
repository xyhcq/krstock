// AmountAnalyzeWidget.cpp : implementation file
//

#include "stdafx.h"
#include "AmountAnalyzeWidget.h"
#include "Widget/MemoryDC.h"
#include "BTWidget.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmountAnalyzeWidget

CAmountAnalyzeWidget::CAmountAnalyzeWidget()
{
	ZeroMemory(&m_ZJLRLCInfo,sizeof(ZJLRLCINFO));
	m_pFinData = NULL;
	ZeroMemory(&m_RTQ,sizeof(REALTIMEQUOTES));
}


CAmountAnalyzeWidget::~CAmountAnalyzeWidget()
{
}


BEGIN_MESSAGE_MAP(CAmountAnalyzeWidget, CStatic)
	//{{AFX_MSG_MAP(CAmountAnalyzeWidget)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KRSDE_TRADINGRECORD_REMOTE,OnKRSDE_TradingRecord_Remote)
	ON_MESSAGE(WM_KRSDE_REALTIMEQUOTES_REMOTE,OnKRSDE_RealTimeQuotes_Remote)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmountAnalyzeWidget message handlers
INT CAmountAnalyzeWidget::GetTextWidth(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cx;
}

INT CAmountAnalyzeWidget::GetTextHeight(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cy;
}



void CAmountAnalyzeWidget::DrawZJInfo(CDC * pDC)
{
	pDC->SetBkMode(TRANSPARENT);
	CRect RCEllipse = m_RCZJInfo;
	INT nBase = (INT)(m_RCZJInfo.Width() / 2);

	RCEllipse.top   += 4;
	RCEllipse.right -= 4;
	RCEllipse.left  = RCEllipse.right - nBase;
	RCEllipse.bottom=RCEllipse.top + nBase;

	CBTWidget BTWidget;
	BTWidget.Create(RCEllipse);
	BTWidget.Draw(pDC,(INT)m_ZJLRLCInfo.fLRDaDan,(INT)m_ZJLRLCInfo.fLRZhongDan,(INT)m_ZJLRLCInfo.fLRXiaoDan,
		(INT)m_ZJLRLCInfo.fLCDaDan,(INT)m_ZJLRLCInfo.fLCZhongDan,(INT)m_ZJLRLCInfo.fLCXiaoDan);

	//用GDI 修一下边
	Graphics raphics( pDC->GetSafeHdc() );
	raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	raphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

	Pen penEllipse(Color(0,0,0),2);
	raphics.DrawEllipse(&penEllipse, 
		RCEllipse.left,RCEllipse.top ,
		RCEllipse.Width()-2,
		RCEllipse.Height()-2);

	Pen WhitePen(Color(150,255,255,255),2);
	raphics.DrawEllipse(&WhitePen, 
		RCEllipse.left,RCEllipse.top ,
		RCEllipse.Width()-2,
		RCEllipse.Height()-2);

	CRect RCTotle;
	RCTotle.left = 4;
	RCTotle.right = RCEllipse.left - 4;
	RCTotle.top  = RCEllipse.top;
	RCTotle.bottom = RCEllipse.bottom + 2;


	CPen  LPen(PS_SOLID,1,RGB(50,50,50));
	pDC->SelectObject(LPen);
	pDC->MoveTo(RCTotle.right,RCTotle.top);
	pDC->LineTo(RCTotle.right,RCTotle.bottom);

	pDC->MoveTo(m_RCZJInfo.left+2,RCTotle.bottom);
	pDC->LineTo(m_RCZJInfo.right-2,RCTotle.bottom);

	pDC->MoveTo(m_RCZJInfo.left+2,RCTotle.top + RCTotle.Height()/2);
	pDC->LineTo(RCTotle.right,RCTotle.top + RCTotle.Height()/2);


	CString szText ="  -";
	pDC->SetTextColor(COLOR_GRAY);
	pDC->TextOut(RCTotle.left,RCTotle.top,"流入(万元)");
	

	float fLRTotle = m_ZJLRLCInfo.fLRDaDan+m_ZJLRLCInfo.fLRZhongDan+m_ZJLRLCInfo.fLRXiaoDan;
	
	if(m_pFinData) szText.Format("%.2f",fLRTotle);


	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOut(RCTotle.left,RCTotle.top+GetTextHeight(pDC,"流入(万元)") + 6,szText);
	
	INT nYBase = RCTotle.top + RCTotle.Height()/2 + 4;

	pDC->SetTextColor(COLOR_GRAY);
	pDC->TextOut(RCTotle.left,nYBase,"流出(万元)");

	float fLCTotle = m_ZJLRLCInfo.fLCDaDan+m_ZJLRLCInfo.fLCZhongDan+m_ZJLRLCInfo.fLCXiaoDan;
	
	if(m_pFinData) 	szText.Format("%.2f",fLCTotle);

	pDC->SetTextColor(RGB(0,255,0));
	pDC->TextOut(RCTotle.left,nYBase+GetTextHeight(pDC,"流出(万元)") + 6,szText);

	pDC->SetTextColor(COLOR_GRAY);
	if(fLRTotle > fLCTotle)
	{
		nYBase = RCTotle.bottom + 4;
		if(m_pFinData) szText.Format("%.2f",fLRTotle - fLCTotle);
		pDC->TextOut(RCTotle.left,nYBase,"净流入(万元):");
		pDC->SetTextColor(RGB(255,0,0));
		pDC->TextOut(RCTotle.left + GetTextWidth(pDC,"净流入(万元):"),nYBase,szText);
	}else{

		nYBase = RCTotle.bottom + 4;
		if(m_pFinData) szText.Format("%.2f",fLCTotle-fLRTotle);
		pDC->TextOut(RCTotle.left,nYBase,"净流出(万元):");
		pDC->SetTextColor(RGB(0,255,0));
		pDC->TextOut(RCTotle.left + GetTextWidth(pDC,"净流出(万元):"),nYBase,szText);
	}

	nYBase+= GetTextHeight(pDC,"净流出(万元):") +2;
	CPen  RedPen(PS_SOLID,1,RGB(255,0,0));
	pDC->SelectObject(RedPen);
	pDC->MoveTo(m_RCZJInfo.left+2,nYBase);
	pDC->LineTo(m_RCZJInfo.right-2,nYBase);

	INT nXBase = m_RCZJInfo.left+2 + GetTextWidth(pDC,"大单") +2;
	nYBase+=2;
	pDC->SetTextColor(COLOR_GRAY);
	pDC->SelectObject(LPen);

	pDC->TextOut(nXBase,nYBase,"流入(万元)    流出(万元)");
	pDC->MoveTo(nXBase + GetTextWidth(pDC,"流入(万元)   ") - GetTextWidth(pDC," ")/2,nYBase);
	pDC->LineTo(nXBase + GetTextWidth(pDC,"流入(万元)   ") - GetTextWidth(pDC," ")/2,m_RCZJInfo.bottom-2);

	nYBase += GetTextHeight(pDC,"流入(万元)")+2;
	
	pDC->MoveTo(m_RCZJInfo.left+2,nYBase);
	pDC->LineTo(m_RCZJInfo.right-2,nYBase);
	pDC->MoveTo(nXBase,nYBase);
	pDC->LineTo(nXBase,m_RCZJInfo.bottom-2);

	nYBase+=2;
	INT nLRXBase= m_RCZJInfo.left+2 + GetTextWidth(pDC,"大单  ");
	INT nLCXBase = nXBase + m_RCZJInfo.left+2 + GetTextWidth(pDC,"流入(万元)  ");

	pDC->SetTextColor(COLOR_GRAY);
	pDC->TextOut(RCTotle.left,nYBase,"大单");
	pDC->SetTextColor(RGB(177,28,30));


	if(m_pFinData) szText.Format("%.2f",m_ZJLRLCInfo.fLRDaDan);
	pDC->TextOut(nLRXBase,nYBase,szText);
	
	pDC->SetTextColor(RGB(80,114,20));
	if(m_pFinData) szText.Format("%.2f",m_ZJLRLCInfo.fLCDaDan);
	pDC->TextOut(nLCXBase,nYBase,szText);

	pDC->SetTextColor(COLOR_GRAY);
	nYBase += GetTextHeight(pDC,"大单") +2;
	pDC->TextOut(RCTotle.left,nYBase,"中单");
	pDC->SetTextColor(RGB(203,78,80));
	if(m_pFinData) szText.Format("%.2f",m_ZJLRLCInfo.fLRZhongDan);
	pDC->TextOut(nLRXBase,nYBase,szText);

	pDC->SetTextColor(RGB(112,161,32));
	if(m_pFinData) szText.Format("%.2f",m_ZJLRLCInfo.fLCZhongDan);
	pDC->TextOut(nLCXBase,nYBase,szText);
	
	pDC->MoveTo(m_RCZJInfo.left+2,nYBase);
	pDC->LineTo(m_RCZJInfo.right-2,nYBase);

	pDC->SetTextColor(COLOR_GRAY);
	nYBase += GetTextHeight(pDC,"中单") +2;
	pDC->TextOut(RCTotle.left,nYBase,"小单");
	pDC->SetTextColor(RGB(221,118,118));
	if(m_pFinData) szText.Format("%.2f",m_ZJLRLCInfo.fLRXiaoDan);
	pDC->TextOut(nLRXBase,nYBase,szText);

	pDC->SetTextColor(RGB(181,197,102));
	if(m_pFinData) szText.Format("%.2f",m_ZJLRLCInfo.fLCXiaoDan);
	pDC->TextOut(nLCXBase,nYBase,szText);

	pDC->MoveTo(m_RCZJInfo.left+2,nYBase);
	pDC->LineTo(m_RCZJInfo.right-2,nYBase);
}

void CAmountAnalyzeWidget::DrawSmall(CDC * pDc)
{
	float fLCTotle = m_ZJLRLCInfo.fLCDaDan+m_ZJLRLCInfo.fLCZhongDan+m_ZJLRLCInfo.fLCXiaoDan;
	float fLRTotle = m_ZJLRLCInfo.fLRDaDan+m_ZJLRLCInfo.fLRZhongDan+m_ZJLRLCInfo.fLRXiaoDan;
	
	pDc->SetBkMode(TRANSPARENT);
	pDc->SetTextColor(COLOR_GRAY);
	
	INT nXOffset = 2;
	pDc->TextOut(2,2,"流入: ");
	
	nXOffset += pDc->GetTextExtent("流入: ").cx;
	CString szText=" -";
	
	if(m_pFinData)
		szText.Format("%.0f万",fLRTotle);
	
	pDc->SetTextColor(COLOR_RED);
	pDc->TextOut(nXOffset, 2 , szText);
	
	nXOffset += pDc->GetTextExtent(szText).cx;
	pDc->SetTextColor(COLOR_GRAY);
	pDc->TextOut(nXOffset,2," 流出: ");
	nXOffset += pDc->GetTextExtent(" 流出: ").cx;
	
	szText = " -";
	if(m_pFinData)
		szText.Format("%.0f万",fLCTotle);
	
	pDc->SetTextColor(COLOR_GREEN);
	pDc->TextOut(nXOffset, 2 , szText);
}



void CAmountAnalyzeWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if((HFONT)m_Font==NULL)
		m_Font.CreatePointFont( 100, "System" );
	
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK())
	{
		//填充背景色
		CDC * pDc = MemDC.GetMemoryDC();
		CBrush bgbs(  RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE)  );
		pDc->FillRect(rc,&bgbs);
		pDc->SelectObject(m_Font);

		//CBrush RedBrush(COLOR_RED);
		//pDc->FrameRect(m_RCZJInfo,&RedBrush);

		if(rc.Height() > 22)
			DrawZJInfo(pDc);
		else
			DrawSmall(pDc);
	}
	// Do not call CStatic::OnPaint() for painting messages
}


BOOL CAmountAnalyzeWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CAmountAnalyzeWidget::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_RCZJInfo.left = 0;
	m_RCZJInfo.top  = 0;
	m_RCZJInfo.right = cx;
	m_RCZJInfo.bottom = cy;
	RedrawWindow();
}


int CAmountAnalyzeWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	KRSDE.SetHWND(m_hWnd);
	return 0;
}

BOOL InitZJLRLCINFO( vector<TRADINGRECORD> & VTradingRecord , ZJLRLCINFO & tz)
{
	ZeroMemory(&tz,sizeof(ZJLRLCINFO));
	float fAmount = 0.00f;
	for(INT i = 0 ; i< VTradingRecord.size(); i++)
	{
		if(i == 0)
			fAmount = VTradingRecord[i].m_fAmount;
		else
			fAmount = VTradingRecord[i].m_fAmount - VTradingRecord[i-1].m_fAmount;
		
		if(VTradingRecord[i].m_bMark == TRADINGRECORD_MARK_ZDBUY)
		{
			//大于30万
			if( fAmount > 300000 )
				tz.fLRDaDan += fAmount;
			else if( fAmount > 50000)
 				tz.fLRZhongDan += fAmount;
			else
				tz.fLRXiaoDan  += fAmount;

		}else if(VTradingRecord[i].m_bMark == TRADINGRECORD_MARK_ZDSELL)
		{
			if( fAmount > 300000 )
				tz.fLCDaDan += fAmount;
			else if( fAmount > 50000)
				tz.fLCZhongDan += fAmount;
			else
				tz.fLCXiaoDan  += fAmount;
		}
	}

	tz.fLRDaDan		/= 10000.00f;
 	tz.fLRZhongDan	/= 10000.00f;
 	tz.fLRXiaoDan	/= 10000.00f; 
 	tz.fLCDaDan		/= 10000.00f;
	tz.fLCZhongDan	/= 10000.00f;
 	tz.fLCXiaoDan	/= 10000.00f;


	return TRUE;
}



LRESULT CAmountAnalyzeWidget::OnKRSDE_TradingRecord_Remote(WPARAM wParam, LPARAM lParam)
{
	LPTOKEN_TRADINGRECORD_HEADINFO	pTTRHeader = (LPTOKEN_TRADINGRECORD_HEADINFO)wParam;
	LPTRADINGRECORD					pTTR       = (LPTRADINGRECORD)lParam;

	m_VTradingRecord.assign(pTTR,pTTR + pTTRHeader->dwCount);

	InitZJLRLCINFO(m_VTradingRecord, m_ZJLRLCInfo);

	if(GetSafeHwnd()) 
		RedrawWindow();
	return 0;
}




void CAmountAnalyzeWidget::SetLabel( const char * szLabel )
{
	const REALTIMEQUOTES * pRTQ = KRSDE.GetRealTimeQuotes(szLabel);
	if(pRTQ == NULL)
		return;
	
	m_RTQ = *pRTQ;
	KRSDE.GetTradingRecord_Remote(szLabel,0,0,0);
	m_pFinData	= KRSDE.GetFinancial((const char * )&m_RTQ.m_wMarket);
}



LRESULT CAmountAnalyzeWidget::OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam)
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


	KRSDE.GetTradingRecord_Remote((const char * )&m_RTQ.m_wMarket,0,0,0);
	return TRUE;
}

