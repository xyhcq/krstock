// HQWidget.cpp : implementation file
//

#include "stdafx.h"
#include "HQWidget.h"
#include "Widget/MemoryDC.h"
#include "BTWidget.h"
#include  <math.h>
#include  <stdlib.h>
#include "StockAlgorithm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHQWidget

CHQWidget::CHQWidget()
{
	ZeroMemory(&m_OldRTQ,sizeof(REALTIMEQUOTES));
	ZeroMemory(&m_RTQ,sizeof(REALTIMEQUOTES));
}



CHQWidget::~CHQWidget()
{

}


BEGIN_MESSAGE_MAP(CHQWidget, CStatic)
	//{{AFX_MSG_MAP(CHQWidget)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KRSDE_REALTIMEQUOTES_REMOTE,OnKRSDE_RealTimeQuotes_Remote)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

LRESULT CHQWidget::OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam)
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


	m_OldRTQ = m_RTQ;
	m_RTQ	 = *pRTQ;

	if(GetSafeHwnd()) 
		RedrawWindow();
	
	return TRUE;
}



void CHQWidget::SetLabel(const char * szLabel)
{
	const REALTIMEQUOTES * pRTQ = KRSDE.GetRealTimeQuotes(szLabel);
	if(pRTQ == NULL)
		return;

	ZeroMemory(&m_OldRTQ,sizeof(REALTIMEQUOTES));
	
	m_OldRTQ	= *pRTQ;
	m_RTQ		= *pRTQ;

	m_HQFunction.Init(&m_RTQ);
	
	if(GetSafeHwnd()) 
		RedrawWindow();
}



INT CHQWidget::GetTextWidth(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cx;
}


INT CHQWidget::GetTextHeight(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cy;
}

CRect CHQWidget::SetListText(CDC * pDc, CRect & rc , int nColumnAve, int nColumn, int nLine, 
							 LPCTSTR lpszText , int nTextHeight, int nTextLineFill , COLORREF nRGB ,UINT uFormat,int nColspan /*=0*/)
{
	int  nColumnAveWidth = rc.Width() / nColumnAve;
	CRect RcItem;
	RcItem.left		= rc.left + nColumnAveWidth *  nColumn;
	RcItem.right    = RcItem.left + nColumnAveWidth;
	if(nColspan){ RcItem.right += nColspan * nColumnAveWidth; }

	RcItem.top		= rc.top + nLine * (nTextHeight + nTextLineFill) + nTextLineFill;
	RcItem.bottom   = RcItem.top + nTextHeight;

	pDc->SetTextColor(nRGB);
	pDc->DrawText(lpszText,RcItem,uFormat);
	return RcItem;
}


void CHQWidget::DrawLine(CDC * pDc, CPen & pen ,CPoint & a, CPoint & b)
{
	pDc->SelectObject(pen);
	pDc->MoveTo(a);
	pDc->LineTo(b);
}


void CHQWidget::AdjustRC(CRect & rc)
{
	m_RCTitle = rc;
	m_RCTitle.bottom = 24;

	m_RC5DPK = m_RCTitle;
	m_RC5DPK.top = m_RCTitle.bottom;
	m_RC5DPK.bottom = m_RCTitle.bottom + 265;

	m_RCHQInfo = m_RC5DPK;
	m_RCHQInfo.top = m_RC5DPK.bottom;
	m_RCHQInfo.bottom = m_RC5DPK.bottom + 235;
}



void CHQWidget::DrawAllRC(CDC * pDC)
{
	CPen  RedPen(PS_SOLID,1,RGB(255,0,0));
	CBrush RedBrush(COLOR_RED);
	pDC->SelectObject(RedPen);
	pDC->FrameRect(m_RCTitle,&RedBrush);
	pDC->FrameRect(m_RC5DPK,&RedBrush);
	pDC->FrameRect(m_RCHQInfo,&RedBrush);
}




/*
-----------------------
委比    --- 委差 ----
-----------------------
均卖价  --- 总额 ----
-----------------------
卖⑤    1          2
卖④
卖③
卖②
卖①
----------------------
买①    2          2
买②
买③
买④
买⑤
----------------------
均买价  --- 总额 ----
----------------------
*/


void CHQWidget::Draw5DPK(CDC * pDc)
{
	pDc->SetBkMode(TRANSPARENT);
	CPen  RedPen(PS_SOLID,1,COLOR_RED);
	int nTextHeight = GetTextHeight(pDc,"委比");
	int nTextLineFill = 4;
	CString szText;
	float   f = 0.00f;
	CRect  rc = m_RC5DPK;
	rc.left += 2;
	rc.right-= 2;
	CRect  rcItem;

	rcItem = SetListText(pDc,rc,4,0,0,"委比",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	DrawLine(pDc,RedPen,CPoint(m_RC5DPK.left,rcItem.bottom+2),CPoint(m_RC5DPK.right,rcItem.bottom+2));
	float fWB = m_HQFunction.WeiBiText(szText,"-");
	SetListText(pDc,rc,4,1,0,szText,nTextHeight,nTextLineFill,GetColor(fWB),DT_LEFT);

	float fWC = m_HQFunction.WeiChaText(szText,"-");
	SetListText(pDc,rc,4,3,0,szText,nTextHeight,nTextLineFill,GetColor(fWC),DT_LEFT);


	rcItem = SetListText(pDc,rc,4,0,1,"均卖价",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	DrawLine(pDc,RedPen,CPoint(m_RC5DPK.left,rcItem.bottom+2),CPoint(m_RC5DPK.right,rcItem.bottom+2));
	float fJSP = m_HQFunction.JunSellJiaText(szText,"-");
	SetListText(pDc,rc,4,1,1,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fJSP),DT_RIGHT);


	m_HQFunction.TotleSellAmountText(szText,"-");
	SetListText(pDc,rc,4,2,1,"总额",nTextHeight,nTextLineFill,COLOR_GRAY,DT_RIGHT);
	SetListText(pDc,rc,4,3,1,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);


	SetListText(pDc,rc,4,0, 2,"卖⑤",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fSellPrice5 = m_HQFunction.SellPriceText(5,szText,"-");
	SetListText(pDc,rc,4,1, 2,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fSellPrice5),DT_LEFT);
	m_HQFunction.SellVolumeText(5,szText,"-");
	SetListText(pDc,rc,4,2, 2,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);


	SetListText(pDc,rc,4,0, 3,"卖④",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fSellPrice4 = m_HQFunction.SellPriceText(4,szText,"-");
	SetListText(pDc,rc,4,1, 3,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fSellPrice4),DT_LEFT);
	m_HQFunction.SellVolumeText(4,szText,"-");
	SetListText(pDc,rc,4,2, 3,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);

	
	SetListText(pDc,rc,4,0, 4,"卖③",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fSellPrice3 = m_HQFunction.SellPriceText(3,szText,"-");
	SetListText(pDc,rc,4,1, 4,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fSellPrice3),DT_LEFT);
	m_HQFunction.SellVolumeText(3,szText,"-");
	SetListText(pDc,rc,4,2, 4,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);


	SetListText(pDc,rc,4,0, 5,"卖②",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fSellPrice2 = m_HQFunction.SellPriceText(2,szText,"-");
	SetListText(pDc,rc,4,1, 5,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fSellPrice2),DT_LEFT);
	m_HQFunction.SellVolumeText(2,szText,"-");
	SetListText(pDc,rc,4,2, 5,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);



	rcItem = SetListText(pDc,rc,4,0,6 ,"卖①",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fSellPrice1 = m_HQFunction.SellPriceText(1,szText,"-");
	SetListText(pDc,rc,4,1, 6,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fSellPrice1),DT_LEFT);
	m_HQFunction.SellVolumeText(1,szText,"-");
	SetListText(pDc,rc,4,2, 6,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);

	DrawLine(pDc,RedPen,CPoint(m_RC5DPK.left,rcItem.bottom+2),CPoint(m_RC5DPK.right,rcItem.bottom+2));
	
	SetListText(pDc,rc,4,0, 7 ,"买①",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fBuyPrice1 = m_HQFunction.BuyPriceText(1,szText,"-");
	SetListText(pDc,rc,4,1, 7,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fBuyPrice1),DT_LEFT);
	m_HQFunction.BuyVolumeText(1,szText,"-");
	SetListText(pDc,rc,4,2, 7,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);
	
	SetListText(pDc,rc,4,0, 8 ,"买②",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fBuyPrice2 = m_HQFunction.BuyPriceText(2,szText,"-");
	SetListText(pDc,rc,4,1, 8,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fBuyPrice2),DT_LEFT);
	m_HQFunction.BuyVolumeText(2,szText,"-");
	SetListText(pDc,rc,4,2, 8,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);

	SetListText(pDc,rc,4,0, 9 ,"买③",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fBuyPrice3 = m_HQFunction.BuyPriceText(3,szText,"-");
	SetListText(pDc,rc,4,1, 9,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fBuyPrice3),DT_LEFT);
	m_HQFunction.BuyVolumeText(3,szText,"-");
	SetListText(pDc,rc,4,2, 9,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);

	SetListText(pDc,rc,4,0,10 ,"买④",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fBuyPrice4 = m_HQFunction.BuyPriceText(4,szText,"-");
	SetListText(pDc,rc,4,1, 10,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fBuyPrice4),DT_LEFT);
	m_HQFunction.BuyVolumeText(4,szText,"-");
	SetListText(pDc,rc,4,2, 10,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);
	
	rcItem=SetListText(pDc,rc,4,0,11 ,"买⑤",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fBuyPrice5 = m_HQFunction.BuyPriceText(5,szText,"-");
	SetListText(pDc,rc,4,1, 11,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fBuyPrice5),DT_LEFT);
	m_HQFunction.BuyVolumeText(5,szText,"-");
	SetListText(pDc,rc,4,2, 11,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_RIGHT);


	DrawLine(pDc,RedPen,CPoint(m_RC5DPK.left,rcItem.bottom+2),CPoint(m_RC5DPK.right,rcItem.bottom+2));
	SetListText(pDc,rc,4,0,12,"均买价",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);

	float fJBP = m_HQFunction.JunBuyJiaText(szText,"-");
	SetListText(pDc,rc,4,1,12,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose ,fJBP),DT_RIGHT);
	
	m_HQFunction.TotleBuyAmountText(szText,"-");
	SetListText(pDc,rc,4,2,12,"总额",nTextHeight,nTextLineFill,COLOR_GRAY,DT_RIGHT);
	SetListText(pDc,rc,4,3,12,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);

	
	//显示买卖量误差
	float	fBuyPrice[5];						
	float	fBuyVolume[5];						
	float	fSellPrice[5];						
	float	fSellVolume[5];			
	
	CopyMemory(fBuyPrice,m_RTQ.m_fBuyPrice, sizeof(m_RTQ.m_fBuyPrice));
	fBuyPrice[3] = m_RTQ.m_fBuyPrice4; fBuyPrice[4] = m_RTQ.m_fBuyPrice5; 
	CopyMemory(fBuyVolume,m_RTQ.m_fBuyVolume, sizeof(m_RTQ.m_fBuyVolume));
	fBuyVolume[3] = m_RTQ.m_fBuyVolume4; fBuyVolume[4] = m_RTQ.m_fBuyVolume5; 
	CopyMemory(fSellPrice,m_RTQ.m_fSellPrice, sizeof(m_RTQ.m_fSellPrice));
	fSellPrice[3] = m_RTQ.m_fSellPrice4; fSellPrice[4] = m_RTQ.m_fSellPrice5; 
	CopyMemory(fSellVolume,m_RTQ.m_fSellVolume, sizeof(m_RTQ.m_fSellVolume));
	fSellVolume[3] = m_RTQ.m_fSellVolume4; fSellVolume[4] = m_RTQ.m_fSellVolume5; 
	
	float	fOldBuyPrice[5];						
	float	fOldBuyVolume[5];						
	float	fOldSellPrice[5];						
	float	fOldSellVolume[5];			
	CopyMemory(fOldBuyPrice,m_OldRTQ.m_fBuyPrice, sizeof(m_OldRTQ.m_fBuyPrice));
	fOldBuyPrice[3] = m_OldRTQ.m_fBuyPrice4; fOldBuyPrice[4] = m_OldRTQ.m_fBuyPrice5; 
	CopyMemory(fOldBuyVolume,m_OldRTQ.m_fBuyVolume, sizeof(m_OldRTQ.m_fBuyVolume));
	fOldBuyVolume[3] = m_OldRTQ.m_fBuyVolume4; fOldBuyVolume[4] = m_OldRTQ.m_fBuyVolume5; 
	CopyMemory(fOldSellPrice,m_OldRTQ.m_fSellPrice, sizeof(m_OldRTQ.m_fSellPrice));
	fOldSellPrice[3] = m_OldRTQ.m_fSellPrice4; fOldSellPrice[4] = m_OldRTQ.m_fSellPrice5; 
	CopyMemory(fOldSellVolume,m_OldRTQ.m_fSellVolume, sizeof(m_OldRTQ.m_fSellVolume));
	fOldSellVolume[3] = m_OldRTQ.m_fSellVolume4; fOldSellVolume[4] = m_OldRTQ.m_fSellVolume5; 


	for (INT i=0;i<5; i++)
	{
		szText = "  -";
		float fDifferSellVolume = 0.00f;
		for (INT n=0;n<5; n++)
		{
			if(Price_Equality(fOldSellPrice[n],fSellPrice[5-1-i]) && !Price_Equality(fOldSellPrice[n],0.00f))
			{
				fDifferSellVolume = fSellVolume[5-1-i] - fOldSellVolume[n];
				break;
			}
		}
		
		if(!Price_Equality(fDifferSellVolume,0.00f))
		{
			FormatSignedInt((INT)fDifferSellVolume,szText);
			pDc->SelectObject(m_FontSmall);
			SetListText(pDc,rc,4,3, 2 + i ,szText,nTextHeight,nTextLineFill,fDifferSellVolume > 0.00f ? COLOR_RED : COLOR_GREEN ,DT_LEFT);
			pDc->SelectObject(m_Font);
		}
		
		float fDifferBuyVolume = 0.00f;
		for (n=0;n<5; n++)
		{
			if(Price_Equality(fOldBuyPrice[n],fBuyPrice[i])&& !Price_Equality(fOldBuyPrice[n],0.00f))
			{
				fDifferBuyVolume = fBuyVolume[i] - fOldBuyVolume[n];
				break;
			}
		}

		if(!Price_Equality(fDifferBuyVolume,0.00f))
		{
			FormatSignedInt((INT)fDifferBuyVolume,szText);
			pDc->SelectObject(m_FontSmall);
			SetListText(pDc,rc,4,3, 7 + i  ,szText,nTextHeight,nTextLineFill,fDifferBuyVolume > 0.00f ? COLOR_RED : COLOR_GREEN ,DT_LEFT);
			pDc->SelectObject(m_Font);
		}
	}

	pDc->SelectObject(RedPen);
	pDc->MoveTo(m_RC5DPK.left,m_RC5DPK.bottom);
	pDc->LineTo(m_RC5DPK.right,m_RC5DPK.bottom);
}




/*
开盘    --- 最高 ----
最收    --- 最低 ----
*/
void CHQWidget::DrawHQInfo(CDC * pDc)
{
	pDc->SetBkMode(TRANSPARENT);
	CRect  rc = m_RCHQInfo;
	rc.left += 2;
	rc.right-= 2;
	CRect  rcItem;
	CString szText;

	int nTextHeight   = GetTextHeight(pDc,"开盘");
	int nTextLineFill = 4;
	float f           = 0.0f;

	SetListText(pDc,rc,4,0,0,"开盘",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fOpen = m_HQFunction.OpenPriceText(szText,"-");
	SetListText(pDc,rc,4,1,0,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fOpen),DT_LEFT);

	SetListText(pDc,rc,4,0,1,"昨收",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fLastClose = m_HQFunction.LastCloseText(szText,"-");
	SetListText(pDc,rc,4,1,1,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fLastClose),DT_LEFT);

	SetListText(pDc,rc,4,2,0,"最高",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fHigh = m_HQFunction.HighPriceText(szText,"-");
	SetListText(pDc,rc,4,3,0,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fHigh),DT_LEFT);
	
 	SetListText(pDc,rc,4,2,1,"最低",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fLow = m_HQFunction.LowPriceText(szText,"-");
 	SetListText(pDc,rc,4,3,1,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fLow),DT_LEFT);
	
	SetListText(pDc,rc,4,0,2,"涨幅",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fZhangFu = m_HQFunction.ZhangFuText(szText,"-");
	SetListText(pDc,rc,4,1,2,szText,nTextHeight,nTextLineFill,GetColor(fZhangFu),DT_LEFT);
	
	SetListText(pDc,rc,4,0,3,"涨速",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fZhangSu = m_HQFunction.ZhangSuText(5,szText,"-");
	SetListText(pDc,rc,4,1,3,szText,nTextHeight,nTextLineFill,GetColor(fZhangSu),DT_LEFT);


	SetListText(pDc,rc,4,2,2,"最新",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fNewPrice = m_HQFunction.NewPriceText(szText);
	SetListText(pDc,rc,4,3,2,szText,nTextHeight,nTextLineFill,GetColor(m_RTQ.m_fLastClose,fNewPrice),DT_LEFT);
	
	SetListText(pDc,rc,4,2,3,"振幅",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fZhengFu = m_HQFunction.ZhengFuText(szText,"-");
	SetListText(pDc,rc,4,3,3,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);
	

	SetListText(pDc,rc,4,0,4,"涨停",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	m_HQFunction.ZhangTingText(szText,"-");
	SetListText(pDc,rc,4,1,4,szText,nTextHeight,nTextLineFill,COLOR_RED,DT_LEFT);
	
	SetListText(pDc,rc,4,2,4,"跌停",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	m_HQFunction.DieTingText(szText,"-");
	SetListText(pDc,rc,4,3,4,szText,nTextHeight,nTextLineFill,COLOR_GREEN,DT_LEFT);

	SetListText(pDc,rc,4,0,5,"总手",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	m_HQFunction.VolumeText(szText,"-");
	SetListText(pDc,rc,4,1,5,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);
	
	SetListText(pDc,rc,4,2,5,"金额",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	m_HQFunction.AmountText(szText,"-");
	SetListText(pDc,rc,4,3,5,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);
	
	SetListText(pDc,rc,4,0,6,"量比",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float  fLB = m_HQFunction.LiangBiText(szText,"-");
	SetListText(pDc,rc,4,1,6,szText,nTextHeight,nTextLineFill,GetLiangBiColor(fLB),DT_LEFT);

	SetListText(pDc,rc,4,2,6,"换手",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float  fHS = m_HQFunction.HuanShouText(szText,"-");
	SetListText(pDc,rc,4,3,6,szText,nTextHeight,nTextLineFill,GetHuanShouColor(fHS),DT_LEFT);

	SetListText(pDc,rc,4,0,7,"市净率",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float  fSJ = m_HQFunction.ShiJingText(szText,"-");
	SetListText(pDc,rc,4,1,7,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);

	if(m_HQFunction.IsJiJin())
	{
		SetListText(pDc,rc,4,2,7,"净值",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
		float fJZ = m_HQFunction.JingZhiText(szText,"-");
		SetListText(pDc,rc,4,3,7,szText,nTextHeight,nTextLineFill,GetColor(1.00f,fJZ),DT_LEFT);
	}else
	{
		SetListText(pDc,rc,4,2,7,"市盈动",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
		m_HQFunction.ShiYingDongText(szText,"-");
		SetListText(pDc,rc,4,3,7,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);
	}


	SetListText(pDc,rc,4,0,8,"流通值",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	m_HQFunction.LiuTongZhiText(szText,"-");
	SetListText(pDc,rc,4,1,8,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);
	
	SetListText(pDc,rc,4,2,8,"总市值",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	m_HQFunction.ZongShiZhiText(szText,"-");
	SetListText(pDc,rc,4,3,8,szText,nTextHeight,nTextLineFill,COLOR_SHENLAN,DT_LEFT);

	SetListText(pDc,rc,4,0,9,"5日均额",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fDay5Amount = m_HQFunction.Day5AmountText(szText,"-");
	SetListText(pDc,rc,4,1,9,szText,nTextHeight,nTextLineFill, GetAmountColor(fDay5Amount),DT_LEFT,3);

	SetListText(pDc,rc,4,2,9,"5日均换手",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fDay5HuanShou = m_HQFunction.Day5HuanShouText(szText,"-");
    SetListText(pDc,rc,4,3,9,szText,nTextHeight,nTextLineFill,GetHuanShouColor(fDay5HuanShou),DT_LEFT,3);

	SetListText(pDc,rc,4,0,10,"板块",nTextHeight,nTextLineFill,COLOR_GRAY,DT_LEFT);
	float fBKZhangFu = m_HQFunction.HangYeZhangFu();
	szText.Format("%s %s%.2f",m_HQFunction.SuoShuHangYe(),(fBKZhangFu > 0)? "+" : "",fBKZhangFu);
	SetListText(pDc,rc,4,1,10,szText,nTextHeight,nTextLineFill,GetColor(fBKZhangFu),DT_LEFT,3);
}




void CHQWidget::DrawTitle(CDC * pDc)
{
	pDc->SetBkMode(TRANSPARENT);
	pDc->SetTextColor( COLOR_YELLOW );
	pDc->TextOut(8,4,(const char *)&m_RTQ.m_wMarket);
	pDc->SetTextColor(COLOR_SHENLAN);
	

	CString szName = m_RTQ.m_szName;
	if(strstr(m_HQFunction.SheJiGaiNain() ,"融"))
		szName += "R";

	float fDay5HuanShou = m_HQFunction.Day5HuanShou();
	if(fDay5HuanShou > 4.00f)
	{
		pDc->SetTextColor(COLOR_RED);
		szName += " Hot";
	}

	pDc->TextOut(GetTextWidth(pDc,(const char *)&m_RTQ.m_wMarket) + 12 +8,4,szName);
	
	CPen RedPen(PS_SOLID,1,COLOR_RED);
	pDc->SelectObject(RedPen);
	pDc->MoveTo(m_RCTitle.left,m_RCTitle.bottom);
	pDc->LineTo(m_RCTitle.right,m_RCTitle.bottom);
}




void CHQWidget::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustRC(CRect(0,0,cx,cy));
	RedrawWindow();
}



void CHQWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
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

		if(strlen(m_RTQ.m_szName) ==0)
			return;

		pDc->SelectObject(m_Font);
		DrawTitle(pDc);
		Draw5DPK(pDc);
		DrawHQInfo(pDc);
	}
	// Do not call CStatic::OnPaint() for painting messages
}





BOOL CHQWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CStatic::OnEraseBkgnd(pDC);
}






int CHQWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	KRSDE.SetHWND(m_hWnd);
	m_Font.CreatePointFont( 100, "System" );
	m_FontSmall.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体"));	
	return 0;
}