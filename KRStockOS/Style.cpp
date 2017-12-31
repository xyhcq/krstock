#include "StdAfx.h"
#include "Style.h"

COLORREF GetColor(float fLastClose , float fCur)
{
	if(fLastClose < fCur )
		return COLOR_RED;
	
	if(fLastClose > fCur )
		return COLOR_GREEN;
	
	return COLOR_WHITE;
}


COLORREF GetColor(float f)
{
	if(f > 0.00 )
		return COLOR_RED;
	
	if(f < 0.00 )
		return COLOR_GREEN;
	
	return COLOR_WHITE;
}




COLORREF GetAmountColor(float f)
{
	//10亿
	if(f > 1000000000.00f )
		return COLOR_VIOLET;

	//5亿
	if(f > 500000000.00f )
		return COLOR_RED;
	
	//2亿
	//if(f > 200000000.00f)
	//	return COLOR_WHITE;
	
	return COLOR_GRAY;
}



COLORREF GetHuanShouColor(float f)
{
	if(f > 10.00f )
		return COLOR_VIOLET;

	if(f > 5.00f )
		return COLOR_RED;
	
//	if(f > 3.00 )
//		return COLOR_WHITE;
	

	return COLOR_GRAY;
}


COLORREF GetLiangBiColor(float f)
{
	if(f > 3.00f )
		return COLOR_VIOLET; //紫色

	if(f > 1.00 )
		return COLOR_RED; // 红色

	return COLOR_GREEN;    //绿色 
}



void FormatAmount(float f, CString & szText,LPCTSTR szDefault)
{
	//资金不可能为负数
	if(f <= 0)
	{
		szText = szDefault;
		return;
	}

	if(f > 100000000000)
	{
		szText.Format("%.0f亿",f/100000000);
		return;
	}

	if(f > 10000000000)
	{
		szText.Format("%.1f亿",f/100000000);
		return;
	}

	if(f > 1000000000)
	{
		szText.Format("%.2f亿",f/100000000);
		return;
	}

	if(f > 10000000)
	{
		szText.Format("%.0f万",f/10000);
		return;
	}

	if(f > 10000)
	{
		szText.Format("%.1f万",f / 10000);
		return;
	}
	szText.Format("%.0f",f);
}


void FormatVolume(float f, CString & szText,LPCTSTR szDefault)
{
	if(f <= 0)
	{
		szText = szDefault;
		return;
	}

	if(f > 1000000000)
	{
		szText.Format("%.2f亿",f/1000000000);
		return;
	}


	if(f > 10000000)
	{
		szText.Format("%.0f万",f/10000);
		return;
	}
	
	if(f > 1000000)
	{
		szText.Format("%.1f万",f/10000);
		return;
	}

	
	if(f > 100000)
	{
		szText.Format("%.2f万",f/10000);
		return;
	}

	szText.Format("%.0f",f);
}




INT FormatSignedInt(INT i , CString & szText)
{
	if(i > 0 )
		szText.Format(" +%d",i);
	else
		szText.Format(" %d",i);
	
	return szText.GetLength();
}




//委比=(委买手数－委卖手数)/(委买手数+委卖手数)×100%
float StockFun_WeiBi(float TotleBuyVol, float TotleSellVol)
{
	float fWB = 0.00f;
	if(!Price_Equality(TotleBuyVol,0.00f) && !Price_Equality(TotleSellVol,0.00f)) 
		fWB  = (TotleBuyVol - TotleSellVol)  / (TotleBuyVol+TotleSellVol) * 100.00f;

	return fWB;
}

float StockFun_ZhengFu(float fHigh,float fLow,float fLastClose)
{
	float f = (fHigh - fLastClose)/fLastClose +  (float)fabs( (double)((fLow - fLastClose)/fLastClose) );
	return f * 100;
}

float StockFun_ZhangTing(float fLastClose,const char * szName)
{
	if(strstr(szName,"ST"))
		return fLastClose * 0.51;

	return fLastClose * 1.1;
}


float StockFun_DieTing(float fLastClose,const char * szName)
{
	if(strstr(szName,"ST"))
		return fLastClose * 0.49;

	return fLastClose * 0.9;
}


