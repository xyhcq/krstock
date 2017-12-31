// IndexWidget.cpp : implementation file
//

#include "stdafx.h"
#include "IndexWidget.h"
#include "Widget/MemoryDC.h"
#include "StockAlgorithm.h"
#include "../KRStockOSDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexWidget

CIndexWidget::CIndexWidget()
{
	m_bRealTimeQuotesOK = FALSE;
}

CIndexWidget::~CIndexWidget()
{
}


BEGIN_MESSAGE_MAP(CIndexWidget, CStatic)
	//{{AFX_MSG_MAP(CIndexWidget)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KRSDE_REALTIMEQUOTES_REMOTE,OnKRSDE_RealTimeQuotes_Remote)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexWidget message handlers
enum{TIMER_FIRST_UPDATE};

INT CIndexWidget::GetTextWidth(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cx;
}

INT CIndexWidget::GetTextHeight(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cy;
}



BOOL GetSZZS_ZhangDieCount(vector<REALTIMEQUOTES> & VRTQ ,DWORD & dwRedCount ,DWORD & dwGreenCount  ,DWORD & dwWhiteCount)
{
	if(VRTQ.size() == 0) return FALSE;
	for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
	{
		if(it->m_szLabel[0] != '6' && it->m_szLabel[0] != '9')	
			continue;
		
		if(it->m_fNewPrice >  it->m_fLastClose)
			dwRedCount++;
		else if(it->m_fNewPrice <  it->m_fLastClose)
			dwGreenCount++;
		else
			dwWhiteCount++;
	}
	return TRUE;
}



BOOL GetSZCZ_ZhangDieCount(vector<REALTIMEQUOTES> & VRTQ ,DWORD & dwRedCount ,DWORD & dwGreenCount  ,DWORD & dwWhiteCount)
{
	if(VRTQ.size() == 0) return FALSE;
	for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
	{
		if(it->m_wMarket !=  0x5A53)
			continue;
		if( (it->m_szLabel[0]=='3' && it->m_szLabel[1]=='0') || 
			(it->m_szLabel[0]=='0' && it->m_szLabel[1]=='0') || 
			(it->m_szLabel[1]=='0' && it->m_szLabel[2]=='2'))
		{
			if(it->m_fNewPrice >  it->m_fLastClose)
				dwRedCount++;
			else if(it->m_fNewPrice <  it->m_fLastClose)
				dwGreenCount++;
			else
				dwWhiteCount++;
		}
	}
	return TRUE;
}


BOOL GetCYBZ_ZhangDieCount(vector<REALTIMEQUOTES> & VRTQ ,DWORD & dwRedCount ,DWORD & dwGreenCount  ,DWORD & dwWhiteCount)
{
	if(VRTQ.size() == 0) return FALSE;
	for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
	{
		if(it->m_wMarket !=  0x5A53)
			continue;

		if( (it->m_szLabel[0]=='3' && it->m_szLabel[1]=='0') )
		{
			if(it->m_fNewPrice >  it->m_fLastClose)
				dwRedCount++;
 			else if(it->m_fNewPrice <  it->m_fLastClose)
 				dwGreenCount++;
 			else 	
				dwWhiteCount++;
		}
	}
	return TRUE;
}


BOOL GetZXBZ_ZhangDieCount(vector<REALTIMEQUOTES> & VRTQ ,DWORD & dwRedCount ,DWORD & dwGreenCount  ,DWORD & dwWhiteCount)
{
	if(VRTQ.size() == 0) return FALSE;
	for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
	{
		if(it->m_wMarket !=  0x5A53)
			continue;
		
		if( (it->m_szLabel[1]=='0' && it->m_szLabel[2]=='2') )
		{
			if(it->m_fNewPrice >  it->m_fLastClose)
				dwRedCount++;
			else if(it->m_fNewPrice <  it->m_fLastClose)
				dwGreenCount++;
			else 	
				dwWhiteCount++;
		}
	}
	return TRUE;
}





BOOL CIndexWidget::DrawIndex(CDC * pDc, CRect rc, const char * szTitle, const REALTIMEQUOTES * pRTQ , DWORD dwRedCount ,DWORD dwGreenCount  ,DWORD  dwWhiteCount)
{
	if(pRTQ == NULL || pDc == NULL || szTitle == NULL)
		return FALSE;

	INT nYOffset = rc.top;
	INT nXOffset = rc.left + 4;

	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szTitle);
	nXOffset += GetTextWidth(pDc,szTitle);
	

	CString szText,szTemp;
	CHQFunction HQFunction;
	HQFunction.Init(pRTQ);

	HQFunction.NewPriceText(szTemp,"-");
	szText+=szTemp;
	szText+=' ';
	
	HQFunction.ZhangFuValueText(szTemp,"-");
	szText+=szTemp;
	szText+=' ';
	
	float fZhangFu = HQFunction.ZhangFuText(szTemp,"-");
	szText+=szTemp;
	szText+=' ';
	
	pDc->SetTextColor(GetColor(fZhangFu));	
	pDc->TextOut(nXOffset, nYOffset, szText);
	
	nXOffset += GetTextWidth(pDc,szText);
	HQFunction.AmountText(szText,"-");
	pDc->SetTextColor(COLOR_SHENLAN);
 	pDc->TextOut(nXOffset, nYOffset, szText);
	
	nXOffset += GetTextWidth(pDc,szText);
	pDc->SetTextColor(COLOR_RED);
	szText.Format(" %d/",dwRedCount);
	
	CRect TextRc;
	TextRc.left   = nXOffset;
	TextRc.right  = rc.right;
	TextRc.top    = nYOffset;
	TextRc.bottom = rc.bottom;
	pDc->DrawText(szText,TextRc,DT_LEFT);
	
	nXOffset += GetTextWidth(pDc,szText);
	pDc->SetTextColor(COLOR_WHITE);
	szText.Format("/%d/",dwWhiteCount);
	TextRc.left   = nXOffset;
	pDc->DrawText(szText,TextRc,DT_LEFT);
	
	nXOffset += GetTextWidth(pDc,szText);
	pDc->SetTextColor(COLOR_GREEN);
	szText.Format("%d家 ",dwGreenCount);
	TextRc.left   = nXOffset;
	pDc->DrawText(szText,TextRc,DT_LEFT);
	
	nXOffset += GetTextWidth(pDc,szText);
	pDc->SetTextColor(COLOR_WHITE);
	TextRc.left   = nXOffset;
	pDc->DrawText("量比",TextRc,DT_LEFT);
	
	nXOffset += GetTextWidth(pDc,"量比");
	float fLB = HQFunction.LiangBiText(szText,"-");
	pDc->SetTextColor(GetLiangBiColor(fLB));
	TextRc.left   = nXOffset;
	pDc->DrawText(szText,TextRc,DT_LEFT);

	return TRUE;
}



static BOOL bFirst = TRUE;

void CIndexWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
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
		pDc->SetBkMode(TRANSPARENT);

		if(m_bRealTimeQuotesOK == FALSE)
			return;


		CPen RedPen(PS_SOLID,1, COLOR_RED );
		pDc->SelectObject(&RedPen);

		INT nXBase = rc.Width() / 4;
		for (INT i=1 ; i< 4; i++)
		{
		  pDc->MoveTo(nXBase * i, rc.top);
		  pDc->LineTo(nXBase * i, rc.bottom);
		}

 		vector<REALTIMEQUOTES> VRTQ;
		KRSDE.GetAllRealTimeQuotes(VRTQ);

		if( VRTQ.size() )
		{
			DWORD dwRedCount= 0 , dwGreenCount = 0 ,dwWhiteCount = 0;
			GetSZZS_ZhangDieCount(VRTQ,dwRedCount,dwGreenCount,dwWhiteCount);
			CRect TextRc;
			TextRc.left  = 0;
			TextRc.right = nXBase * 1;
			TextRc.top   = 1;
			TextRc.bottom = rc.bottom;
 			DrawIndex(pDc,TextRc,"沪",KRSDE.GetRealTimeQuotes("SH000001"),dwRedCount,dwGreenCount,dwWhiteCount);

			dwRedCount= 0 , dwGreenCount = 0 ,dwWhiteCount = 0;
			GetSZCZ_ZhangDieCount(VRTQ,dwRedCount,dwGreenCount,dwWhiteCount);
			TextRc.left  = nXBase * 1;
			TextRc.right = nXBase * 2;
			TextRc.top   = 1;
			TextRc.bottom = rc.bottom;
 			DrawIndex(pDc,TextRc,"深",KRSDE.GetRealTimeQuotes("SZ399001"),dwRedCount,dwGreenCount,dwWhiteCount);

			dwRedCount= 0 , dwGreenCount = 0 ,dwWhiteCount = 0;
			GetCYBZ_ZhangDieCount(VRTQ,dwRedCount,dwGreenCount,dwWhiteCount);
			TextRc.left  = nXBase * 2;
			TextRc.right = nXBase * 3;
			TextRc.top   = 1;
			TextRc.bottom = rc.bottom;
 			DrawIndex(pDc,TextRc,"创",KRSDE.GetRealTimeQuotes("SZ399006"),dwRedCount,dwGreenCount,dwWhiteCount);

			dwRedCount= 0 , dwGreenCount = 0 ,dwWhiteCount = 0;
			GetZXBZ_ZhangDieCount(VRTQ,dwRedCount,dwGreenCount,dwWhiteCount);
			TextRc.left  = nXBase * 3;
			TextRc.right = nXBase * 4;
			TextRc.top   = 1;
			TextRc.bottom = rc.bottom;
 			DrawIndex(pDc,TextRc,"中小",KRSDE.GetRealTimeQuotes("SZ399005"),dwRedCount,dwGreenCount,dwWhiteCount);
		}
	}
	// Do not call CStatic::OnPaint() for painting messages
}




BOOL CIndexWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

LRESULT CIndexWidget::OnKRSDE_RealTimeQuotes_Remote( WPARAM wParam,LPARAM lParam )
{
	m_bRealTimeQuotesOK= TRUE;
	RedrawWindow();
	return TRUE;
}


extern HWND g_hKRStockOSDlg;
void CIndexWidget::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();

	CRect rc;
	GetClientRect(rc);
	INT nXBase = rc.Width() / 4; 

	//上证指数
	if(point.x < nXBase)
	{
		::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)"SH000001");
	}else if( point.x > nXBase && point.x < nXBase * 2)
	{
		::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)"SZ399001");
	}else if( point.x > nXBase * 2 && point.x < nXBase * 3)
	{
		::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)"SZ399006");
	}else if( point.x > nXBase * 3 && point.x < nXBase * 4)
	{
		::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)"SZ399005");
	}
	CStatic::OnLButtonDown(nFlags, point);
}



void CIndexWidget::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(::LoadCursor(NULL,IDC_HAND));
	CStatic::OnMouseMove(nFlags, point);
}


int CIndexWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	KRSDE.SetHWND(m_hWnd);
	m_Font.CreatePointFont( 100, "System" );
	return 0;
}
