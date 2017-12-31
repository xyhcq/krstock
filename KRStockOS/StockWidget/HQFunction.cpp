#include "StdAfx.h"
#include "HQFunction.h"



CHQFunction::CHQFunction()
{
	m_pRTQ		= NULL;
	m_pFinData  = NULL;
	ZeroMemory(&m_HQBaseInfo,sizeof(HQBASE_INFO));
}



BOOL CHQFunction::Init( const REALTIMEQUOTES * pRTQ )
{
	if(pRTQ == NULL)
		return FALSE;

	m_pRTQ		= pRTQ;
	m_pFinData  = NULL;
	m_pFinData  = KRSDE.GetFinancial((const char *)&m_pRTQ->m_wMarket);
	ZeroMemory(&m_HQBaseInfo,sizeof(HQBASE_INFO));
	KRSDE.GetHQBaseInfo((const char *)&m_pRTQ->m_wMarket,&m_HQBaseInfo);
	m_bIsJiJin  = IsJiJin();
	m_bIsST     = IsST();
	return TRUE;
}


void CHQFunction::FormatAmount(float f, CString & szText,LPCTSTR szDefault)
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
	
	if(f > 100000000)
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



void CHQFunction::FormatVolume(float f, CString & szText,LPCTSTR szDefault)
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


INT CHQFunction::GetOpenTimeCount( time_t t )
{
	CTime tCur = t;
	switch(tCur.GetHour())
	{
	case 9:
		return tCur.GetMinute() - 30 + 1;
	case 10:
		return tCur.GetMinute()  + 30 + 1;
	case 11:
		return tCur.GetMinute()  + 90 + 1;
	case 13:
		return tCur.GetMinute() + 120 + 1;
	case 14:
		return tCur.GetMinute()  + 180 + 1;
	}
	return 240;
}


BOOL CHQFunction::IsTP()
{
	if(m_pRTQ == NULL)
		return FALSE;
	
	return (m_pRTQ->m_fNewPrice ==0.00f)? TRUE : FALSE;
}


BOOL CHQFunction::IsST()
{
	if(m_pRTQ == NULL)
		return FALSE;
	
	return strstr(m_pRTQ->m_szName,"ST") ? TRUE : FALSE;
}


BOOL CHQFunction::IsJiJin()
{
	if(m_pRTQ == NULL)
		return FALSE;
	
	return (m_pRTQ->m_szLabel[0] == '1' && m_pRTQ->m_szLabel[1] == '5');
}



DWORD CHQFunction::GuDongShu()
{
	return m_HQBaseInfo.dwGDZS;
}


DWORD CHQFunction::GuDongShuText(CString & szText ,LPCTSTR szDefault)
{
	if(m_HQBaseInfo.dwGDZS ==0 )
	{
		szText = szDefault;
		return 0;
	}
	szText.Format("%d",m_HQBaseInfo.dwGDZS);
	return m_HQBaseInfo.dwGDZS;
}



float CHQFunction::RenJunChiGu()
{
	if(m_HQBaseInfo.dwGDZS ==0 || m_pFinData == NULL || m_pFinData->MQLT < 1)
		return 0.00f;

	return m_pFinData->MQLT* 10000 / m_HQBaseInfo.dwGDZS;
}


float CHQFunction::RenJunChiGuSZText(CString & szText ,LPCTSTR szDefault )
{
	float f= RenJunChiGu();
	if(f <= 0  || m_pRTQ == NULL)
	{
		szText = szDefault;
		return 0.00f;
	}

	f = f * m_pRTQ->m_fNewPrice;
	szText.Format("%.2f万",f/10000);
	return f;
}



float CHQFunction::SHRenJunChiGu()
{
	if(m_HQBaseInfo.dwGDZS ==0 || m_pFinData == NULL || m_pFinData->MQLT < 1)
		return 0.00f;
	return (m_pFinData->MQLT * 10000 - m_HQBaseInfo.fQSDLTGDCC) / m_HQBaseInfo.dwGDZS;
}



float CHQFunction::SHRenJunChiGuSZText(CString & szText ,LPCTSTR szDefault )
{
	float f= SHRenJunChiGu();
	if(f <= 0  || m_pRTQ == NULL)
	{
		szText = szDefault;
		return 0.00f;
	}
	f = f * m_pRTQ->m_fNewPrice;
	szText.Format("%.2f万",f/10000);
	return f;
}


float CHQFunction::GDJSQBH()
{
	return m_HQBaseInfo.fJSQBH;
}

float CHQFunction::GDJSQBHText(CString & szText ,LPCTSTR szDefault)
{
	float f= GDJSQBH();
	if(f == 0.00f )
	{
		szText = szDefault;
		return 0.00f;
	}
	szText.Format("%.2f%c",m_HQBaseInfo.fJSQBH,'%');
	return f;
}


float CHQFunction::QSDLTGD()
{	
	if(m_pFinData == NULL || m_pFinData->MQLT < 1)
		return 0.00f;
	return m_HQBaseInfo.fQSDLTGDCC;
}


float CHQFunction::QSDLTGDZBText(CString & szText ,LPCTSTR szDefault)
{
	float f= QSDLTGD();
	if(f == 0.00f )
	{
		szText = szDefault;
		return 0.00f;
	}

	f = f /  (m_pFinData->MQLT * 10000) * 100;
	szText.Format("%.2f%c",f,'%');
	return f;
}





LPCTSTR CHQFunction::SuoShuHangYe()
{
	return m_HQBaseInfo.szSSHY;
}

LPCTSTR CHQFunction::SuoShuHangYeText(CString & szText ,LPCTSTR szDefault)
{
	if(m_HQBaseInfo.szSSHY[0])
		szText = m_HQBaseInfo.szSSHY;
	else
		szText = szDefault;

	return m_HQBaseInfo.szSSHY;
}


LPCTSTR  CHQFunction::ZhuYingYeWu()
{
	return m_HQBaseInfo.szZYYW;
}

LPCTSTR  CHQFunction::ZhuYingYeWuText(CString & szText ,LPCTSTR szDefault)
{
	if(m_HQBaseInfo.szZYYW[0])
		szText = m_HQBaseInfo.szZYYW;
	else
		szText = szDefault;
	return m_HQBaseInfo.szZYYW;
}




float CHQFunction::HangYeZhangFu()
{
	return KRSDE.GetHYZhangFu(SuoShuHangYe());
}

float  CHQFunction::HangYeZhangFuText(CString & szText ,LPCTSTR szDefault)
{
	float fHangYeZhangFu =KRSDE.GetHYZhangFu(SuoShuHangYe());
	if(fHangYeZhangFu == 0.00f)
	{
		szText = szDefault;
	}
	else
	{
		if(fHangYeZhangFu > 0)
			szText.Format("+%.2f%c",fHangYeZhangFu,'%');
		else
			szText.Format("%.2f%c",fHangYeZhangFu,'%');
	}
	return fHangYeZhangFu;
}

float CHQFunction::Round(float val)
{
	INT places = m_bIsJiJin ? 3:2;
	return Round(val,places);
}


float CHQFunction::Round(float f,int n)
{
	if(f == 0.00f)
		return 0.00f;
	
	n=n+1;
	
	int dec,sign;
	char * pstr =fcvt(f,n,&dec,&sign);	
	char szText[128]={0};
	
	if(sign == 1) //负数
	{
		szText[0]='-';
		szText[1]='\0';
		
		strncat(szText,pstr,dec);
		szText[dec+1]='.';
		strcpy(&szText[dec+2],pstr + dec);
		
		if(szText[dec + n + 1] >= '5')
		{
			szText[dec + n + 1] = '0';
			szText[dec + n ] += 1;
		}
	}else
	{
		strncat(szText,pstr,dec);
		szText[dec]='.';
		strcpy(&szText[dec+1],pstr + dec);
		
		if(szText[dec + n] >= '5')
		{
			szText[dec + n] = '0';
			szText[dec + n-1] += 1;
		}
	}
	return atof(szText);
}





float	CHQFunction::ZhangTing(float fPrice)
{
	float fRet;
	
	if(m_bIsST)
		fRet =fPrice * 1.05;
	else
		fRet = fPrice * 1.1;
	
	if( m_bIsJiJin )
		fRet = Round(fRet,3);
	else
		fRet = Round(fRet,2);
	
	return fRet;
}


float	CHQFunction::DieTing(float fPrice)
{
	float fRet;

	if(m_bIsST)
		fRet = fPrice * 0.95;
	else
		fRet = fPrice * 0.9;

	if( m_bIsJiJin )
		fRet = Round(fRet,3);
	else
		fRet = Round(fRet,2);
	
	return fRet;
}



float CHQFunction::ZhangTing()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f;
	if(m_bIsST)
		f = m_pRTQ->m_fLastClose * 1.05;
	else
		f = m_pRTQ->m_fLastClose * 1.1;
	
	if( m_bIsJiJin )
		f = Round(f,3);
	else
		f = Round(f,2);
	
	return f;
}


float CHQFunction::DieTing()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f;
	if(m_bIsST)
		f = m_pRTQ->m_fLastClose * 0.95;
	else
		f = m_pRTQ->m_fLastClose * 0.9;

	if( m_bIsJiJin )
		f = Round(f,3);
	else
		f = Round(f,2);
	
	return f;
}




float CHQFunction::DieTingText(CString & szText,LPCTSTR szDefault)
{
	float fDieTing = DieTing();
	if(fDieTing == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}

	if( m_bIsJiJin )
		szText.Format("%.3f",fDieTing);
	else
		szText.Format("%.2f",fDieTing);
	
	return fDieTing;
}




float CHQFunction::ZhangTingText(CString & szText,LPCTSTR szDefault)
{
	float fZhangTing = ZhangTing();
	if(fZhangTing == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}

	if( m_bIsJiJin )
		szText.Format("%.3f",fZhangTing);
	else
		szText.Format("%.2f",fZhangTing);

	return fZhangTing;
}



float	CHQFunction::ZhangSu(INT nMinutePeriod)
{
	if(m_pRTQ == NULL)
		return 0.00f;

	return KRSDE.GetZhangSu((const char *)&m_pRTQ->m_wMarket, nMinutePeriod);
}



float	CHQFunction::ZhangSuText(INT nMinutePeriod , CString & szText,LPCTSTR szDefault)
{
	float p = ZhangSu(nMinutePeriod);
	if(m_pRTQ->m_fNewPrice == 0.00f || p == 0.00f)
	{
		szText = szDefault;
		return p;
	}

	float f = (m_pRTQ->m_fNewPrice - p) / p * 100;

	if(f > 0)
		szText.Format("+%.2f%c",f,'%');
	else
		szText.Format("%.2f%c",f,'%');

	return f;
}


float CHQFunction::ZhengFu()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f = (m_pRTQ->m_fHigh - m_pRTQ->m_fLow) / m_pRTQ->m_fLastClose * 100;
	return f;
}




float CHQFunction::ZhengFuText(CString & szText ,LPCTSTR szDefault)
{
	float fZhengFu = ZhengFu();

	if(m_pRTQ->m_fHigh == 0.00f || m_pRTQ->m_fLow== 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}



	if(fZhengFu == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f%c",fZhengFu,'%');
	
	return fZhengFu;
}

float CHQFunction::ZhengFuText(float fLastClose,float fHigh , float fLow ,CString & szText,LPCTSTR szDefault)
{
	if(fLastClose == 0.00f || fHigh == 0.00f || fLow == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}

	float f = (fHigh - fLow) / fLastClose * 100;
	szText.Format("%.2f%c",f,'%');

	return fHigh - fLow;
}






float CHQFunction::ZhangFu()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f = (m_pRTQ->m_fNewPrice - m_pRTQ->m_fLastClose)/m_pRTQ->m_fLastClose;
	return f * 100;
}


float CHQFunction::ZhangFuText(CString & szText ,LPCTSTR szDefault)
{
	float fZhangFu = ZhangFu();
	if(m_pRTQ->m_fNewPrice == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}

	if( fZhangFu > 0 )
		szText.Format("+%.2f%c",fZhangFu,'%');
	else
		szText.Format("%.2f%c",fZhangFu,'%');

	return fZhangFu;
}


float CHQFunction::ZhangFuText(float fLastClose,float fCurPrice,CString & szText,LPCTSTR szDefault )
{
	if(fLastClose == 0.00f || fCurPrice == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}
	
	float f = (fCurPrice - fLastClose)/fLastClose * 100;
	szText.Format("%.2f%c",f,'%');

	return (fCurPrice - fLastClose);
}




float CHQFunction::ZhangFuValue()
{
	if(m_pRTQ == NULL)
		return 0.00f;
	
	return m_pRTQ->m_fNewPrice - m_pRTQ->m_fLastClose;
}

float CHQFunction::ZhangFuValueText(CString & szText ,LPCTSTR szDefault)
{
	float fZhangFuValue = ZhangFuValue();

	if( fZhangFuValue > 0 )
		szText.Format("+%.2f",fZhangFuValue);
	else
		szText.Format("%.2f",fZhangFuValue);

	return fZhangFuValue;
}


LPCTSTR CHQFunction::ZuiXinJieJin(CString & szText ,LPCTSTR szDefault /* = "-" */)
{
	if(m_HQBaseInfo.szZXJJ[0])
		szText = m_HQBaseInfo.szZXJJ;
	else
		szText = szDefault;

	return m_HQBaseInfo.szZXJJ;
}


LPCTSTR CHQFunction::SheJiGaiNain()
{
	return m_HQBaseInfo.szSJGN;
}

LPCTSTR CHQFunction::SheJiGaiNainText(CString & szText ,LPCTSTR szDefault)
{
	if(m_HQBaseInfo.szSJGN[0])
		szText = m_HQBaseInfo.szSJGN;
	else
		szText = szDefault;
	
	return m_HQBaseInfo.szSJGN;
}


float CHQFunction::LiuTongBiLi()
{
	if(m_pFinData == NULL )
		return 0.00f;

	return m_pFinData->MQLT / m_pFinData->ZGB * 100;
}


float CHQFunction::LiuTongBiLiText(CString & szText ,LPCTSTR szDefault)
{
	float fLTBL = LiuTongBiLi();
	if(fLTBL == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f%c",fLTBL,'%');
	
	return fLTBL;
}

float CHQFunction::LiuTongShiZhi()
{
	if(m_pFinData)
		return m_pFinData->MQLT * m_pRTQ->m_fNewPrice * 10000;
	
	return 0.00f;
}


float CHQFunction::ZongShiZhi()
{
	if(m_pFinData)
		return m_pFinData->ZGB * m_pRTQ->m_fNewPrice* 10000;
	
	return 0.00f;
}

float CHQFunction::LiuTongZhiText(CString & szText ,LPCTSTR szDefault)
{
	float fLTSZ = LiuTongShiZhi();
	FormatAmount(fLTSZ,szText,szDefault);
	return fLTSZ;
}


float CHQFunction::ZongShiZhiText(CString & szText ,LPCTSTR szDefault)
{
	float fZSZ = ZongShiZhi();
	FormatAmount(fZSZ,szText,szDefault);
	return fZSZ;
}


float CHQFunction::LiangBi()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	if(m_HQBaseInfo.f5DayVolume != 0.00f)
		return m_pRTQ->m_fVolume / ( m_HQBaseInfo.f5DayVolume / 5 / 240   *  GetOpenTimeCount(m_pRTQ->m_time) );

	return 0.0f;
}


float CHQFunction::LiangBiText(CString & szText ,LPCTSTR szDefault /*= " -"*/ )
{
	float fLB = LiangBi();
	if(fLB == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f",fLB);

	return fLB;
}



float CHQFunction::ShiYingDong( )
{
	if(m_pRTQ == NULL)
		return 0.00f;

	return m_pRTQ->m_fNewPrice * m_HQBaseInfo.fSYLDT_BL;
}


float CHQFunction::ShiYingDongText( CString & szText ,LPCTSTR szDefault /*= " -"*/ )
{
	float fSYD = ShiYingDong();

	if(fSYD == 0.00f)
	{
		if(m_pFinData && m_pFinData->MGSY != 0.00f)
			szText = "亏损";
		else
			szText = szDefault;
	}
	else
		szText.Format("%.2f",fSYD);
	return fSYD;
}





float CHQFunction::Day5Amount()
{
	return m_HQBaseInfo.f5DayAmount/5;
}

float CHQFunction::Day5AmountText(CString & szText ,LPCTSTR szDefault /* = " -" */)
{
	FormatAmount(m_HQBaseInfo.f5DayAmount/5,szText,"-");
	return m_HQBaseInfo.f5DayAmount/5;
}




float CHQFunction::Day5HuanShou()
{
	if(m_HQBaseInfo.f5DayVolume == 0.00f)
		return 0.00f;
	
	if(m_pFinData == NULL || m_pFinData->MQLT < 1)
		return 0.00f;
	
	return m_HQBaseInfo.f5DayVolume / m_pFinData->MQLT / 5;
}

float CHQFunction::Day5HuanShouText(CString & szText ,LPCTSTR szDefault /*= " -"*/ )
{
	float fDay5HS = Day5HuanShou();

	if(fDay5HS == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f%c",fDay5HS,'%');

	return fDay5HS;
}



float CHQFunction::HuanShou()
{
	if(m_pRTQ == NULL || m_pFinData == NULL || m_pFinData->MQLT < 1)
		return 0.00f;

	return m_pRTQ->m_fVolume / m_pFinData->MQLT;
}



float CHQFunction::HuanShouText( CString & szText ,LPCTSTR szDefault /*= " -"*/ )
{
	float fHS = HuanShou();
	if(fHS == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f%c",fHS,'%');
	return fHS;
}

void  CHQFunction::HuanShouText( float fVolume, CString & szText ,LPCTSTR szDefault)
{
	if(m_pRTQ == NULL || m_pFinData == NULL || m_pFinData->MQLT < 1)
	{
		szText = szDefault;
		return;
	}

	float fHS =  fVolume / m_pFinData->MQLT;
	if(fHS == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f%c",fHS,'%');
}

float CHQFunction::MeiGuShowYi()
{
	if(m_pFinData == NULL)
		return 0.00f;

	return m_pFinData->MGSY;
}


float CHQFunction::MeiGuShowYiText( CString & szText ,LPCTSTR szDefault)
{
	float fMGSY = MeiGuShowYi();
	if(fMGSY == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f元",fMGSY);

	return fMGSY;
}



float CHQFunction::ShiJing()
{
	if(m_pRTQ == NULL || m_pFinData == NULL)
		return 0.00f;

	return m_pRTQ->m_fNewPrice/ m_pFinData->MGJZC;
}


float CHQFunction::ShiJingText( CString & szText ,LPCTSTR szDefault /*= " -"*/ )
{
	float fSJ = ShiJing();
	if(fSJ == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.2f",fSJ);
	return fSJ;
}



float CHQFunction::JingZhi()
{
	if(m_pFinData == NULL)
		return 0.00f;

	return m_pFinData->MGJZC;
}



float CHQFunction::JingZhiText( CString & szText ,LPCTSTR szDefault /*= " -"*/ )
{
	float fJZ = JingZhi();
	
	if(fJZ == 0.00f)
		szText = szDefault;
	else
		szText.Format("%.3f",fJZ);

	return fJZ;
}




float CHQFunction::AmountText(CString & szText,LPCTSTR szDefault /* =" -" */)
{
	FormatAmount(m_pRTQ->m_fAmount,szText,"-");
	return m_pRTQ->m_fAmount;
}

float CHQFunction::VolumeText(CString & szText,LPCTSTR szDefault /* =" -" */)
{
	FormatVolume(m_pRTQ->m_fVolume,szText,"-");
	return m_pRTQ->m_fVolume;
}



float CHQFunction::JunSellJia()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	return( m_pRTQ->m_fSellPrice4 + m_pRTQ->m_fSellPrice5 + m_pRTQ->m_fSellPrice[2] + m_pRTQ->m_fSellPrice[1] + m_pRTQ->m_fSellPrice[0]) / 5;
}

float CHQFunction::JunSellJiaText(CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	float f = JunSellJia();
	if(f ==0.00f)
	{
		szText = szDefault;
		return f;
	}
	
	if( m_bIsJiJin )
		szText.Format("%.3f",f);
	else
		szText.Format("%.2f",f);

	return f;
}





float CHQFunction::JunBuyJia() 
{
	if(m_pRTQ == NULL )
		return 0.00f;

	return( m_pRTQ->m_fBuyPrice4 + m_pRTQ->m_fBuyPrice5 + m_pRTQ->m_fBuyPrice[2] + m_pRTQ->m_fBuyPrice[1] + m_pRTQ->m_fBuyPrice[0]) / 5;
}



float CHQFunction::JunBuyJiaText(CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	float f = JunBuyJia();
	if(f ==0.00f)
	{
		szText = szDefault;
		return f;
	}
	

	if( m_bIsJiJin )
		szText.Format("%.3f",f);
	else
		szText.Format("%.2f",f);

	return f;
}


void CHQFunction::PriceText(float fPrice, CString & szText, LPCTSTR szDefault /* =" -" */)
{
	if(fPrice == 0.00f)
	{
		szText = szDefault;
	}else
	{
		if( m_bIsJiJin )
			szText.Format("%.3f",fPrice);
		else
			szText.Format("%.2f",fPrice);
	}
}





float CHQFunction::OpenPriceText(CString & szText,  LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL || m_pRTQ->m_fOpen == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}

	if( m_bIsJiJin )
		szText.Format("%.3f",m_pRTQ->m_fOpen);
	else
		szText.Format("%.2f",m_pRTQ->m_fOpen);
	
	return m_pRTQ->m_fOpen;
}



float CHQFunction::LastCloseText(CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL ||  m_pRTQ->m_fLastClose == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}


	if( m_bIsJiJin )
		szText.Format("%.3f",m_pRTQ->m_fLastClose);
	else
		szText.Format("%.2f",m_pRTQ->m_fLastClose);
	
	return m_pRTQ->m_fLastClose;
}



float CHQFunction::LowPriceText(CString & szText,  LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL || m_pRTQ->m_fLow == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}


	if( m_bIsJiJin )
		szText.Format("%.3f",m_pRTQ->m_fLow);
	else
		szText.Format("%.2f",m_pRTQ->m_fLow);
	
	return m_pRTQ->m_fLow;
}


float CHQFunction::HighPriceText( CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL || m_pRTQ->m_fHigh == 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}

	if( m_bIsJiJin )
		szText.Format("%.3f",m_pRTQ->m_fHigh );
	else
		szText.Format("%.2f",m_pRTQ->m_fHigh);
	
	return m_pRTQ->m_fHigh;
}


float CHQFunction::NewPriceText(CString & szText,  LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL || m_pRTQ->m_fNewPrice== 0.00f)
	{
		szText = szDefault;
		return 0.00f;
	}


	if( m_bIsJiJin )
		szText.Format("%.3f",m_pRTQ->m_fNewPrice);
	else
		szText.Format("%.2f",m_pRTQ->m_fNewPrice);

	return m_pRTQ->m_fNewPrice;
}





float CHQFunction::SellVolumeText( INT nIndex,CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL)
	{
		szText = szDefault;
		return 0.00f;
	}
	
	float f = 0.00f;
	switch (nIndex)
	{
	case 1: f = m_pRTQ->m_fSellVolume[0];break;
	case 2: f = m_pRTQ->m_fSellVolume[1];break;
	case 3: f = m_pRTQ->m_fSellVolume[2];break;
	case 4: f = m_pRTQ->m_fSellVolume4;break;
	case 5: f = m_pRTQ->m_fSellVolume5;break;
	}
	
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	
	FormatVolume(f,szText,szDefault);
	return f;
}

float CHQFunction::BuyVolumeText( INT nIndex,CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL)
	{
		szText = szDefault;
		return 0.00f;
	}
	
	float f = 0.00f;
	switch (nIndex)
	{
	case 1: f = m_pRTQ->m_fBuyVolume[0];break;
	case 2: f = m_pRTQ->m_fBuyVolume[1];break;
	case 3: f = m_pRTQ->m_fBuyVolume[2];break;
	case 4: f = m_pRTQ->m_fBuyVolume4;break;
	case 5: f = m_pRTQ->m_fBuyVolume5;break;
	}
	
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	
	FormatVolume(f,szText,szDefault);
	return f;
}



float CHQFunction::SellPriceText( INT nIndex,CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL)
	{
		szText = szDefault;
		return 0.00f;
	}

	float f = 0.00f;
	switch (nIndex)
	{
		case 1: f = m_pRTQ->m_fSellPrice[0];break;
		case 2: f = m_pRTQ->m_fSellPrice[1];break;
		case 3: f = m_pRTQ->m_fSellPrice[2];break;
		case 4: f = m_pRTQ->m_fSellPrice4;break;
		case 5: f = m_pRTQ->m_fSellPrice5;break;
	}
	
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	
	if( m_bIsJiJin )
		szText.Format("%.3f",f);
	else
		szText.Format("%.2f",f);
	
	return f;
}



float CHQFunction::BuyPriceText( INT nIndex,CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	if(m_pRTQ == NULL)
	{
		szText = szDefault;
		return 0.00f;
	}

	float f = 0.00f;
	switch (nIndex)
	{
		case 1: f = m_pRTQ->m_fBuyPrice[0];break;
		case 2: f = m_pRTQ->m_fBuyPrice[1];break;
		case 3: f = m_pRTQ->m_fBuyPrice[2];break;
		case 4: f = m_pRTQ->m_fBuyPrice4;break;
		case 5: f = m_pRTQ->m_fBuyPrice5;break;
	}
	
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	
	if( m_bIsJiJin )
		szText.Format("%.3f",f);
	else
		szText.Format("%.2f",f);

	return f;
}






float CHQFunction::TotleBuyAmount()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f=  m_pRTQ->m_fBuyVolume5   * m_pRTQ->m_fBuyPrice5    + \
		m_pRTQ->m_fBuyVolume4   * m_pRTQ->m_fBuyPrice4    + \
		m_pRTQ->m_fBuyVolume[2] * m_pRTQ->m_fBuyPrice[2]  + \
		m_pRTQ->m_fBuyVolume[1] * m_pRTQ->m_fBuyPrice[1]  + \
		m_pRTQ->m_fBuyVolume[0] * m_pRTQ->m_fBuyPrice[0];
	return f * 100;
}



float CHQFunction::TotleSellAmount()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f=    m_pRTQ->m_fSellVolume5   * m_pRTQ->m_fSellPrice5    + \
		m_pRTQ->m_fSellVolume4   * m_pRTQ->m_fSellPrice4    + \
		m_pRTQ->m_fSellVolume[2] * m_pRTQ->m_fSellPrice[2]  + \
		m_pRTQ->m_fSellVolume[1] * m_pRTQ->m_fSellPrice[1]  + \
		m_pRTQ->m_fSellVolume[0] * m_pRTQ->m_fSellPrice[0];
	
	return f * 100;
}


float CHQFunction::TotleSellVolume()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f = m_pRTQ->m_fSellVolume5 + m_pRTQ->m_fSellVolume4 + m_pRTQ->m_fSellVolume[2] + m_pRTQ->m_fSellVolume[1];
	if(m_pRTQ->m_fSellPrice[0] != 0.00f)
		f+=m_pRTQ->m_fSellVolume[0];
	
	return f;
}



float CHQFunction::TotleBuyVolume()
{
	if(m_pRTQ == NULL)
		return 0.00f;

	float f = m_pRTQ->m_fBuyVolume5 + m_pRTQ->m_fBuyVolume4 + m_pRTQ->m_fBuyVolume[2] + m_pRTQ->m_fBuyVolume[1];
	if(m_pRTQ->m_fBuyPrice[0] != 0.00f)
		f+=m_pRTQ->m_fBuyVolume[0];
	
	return f;
}



float CHQFunction::TotleBuyVolumeText(CString & szText, LPCTSTR szDefault)
{
	float f = TotleBuyVolume();
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	FormatVolume(f,szText,szDefault);
	return f;
}



float CHQFunction::TotleBuyAmountText(CString & szText, LPCTSTR szDefault)
{
	float f = TotleBuyAmount();
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	FormatAmount(f,szText,szDefault);
	return f;
}


float CHQFunction::TotleSellVolumeText(CString & szText, LPCTSTR szDefault)
{
	float f = TotleSellVolume();
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	FormatVolume(f,szText,szDefault);
	return f;
}



float CHQFunction::TotleSellAmountText(CString & szText, LPCTSTR szDefault)
{
	float f = TotleSellAmount();
	if(f == 0.00f)
	{
		szText = szDefault;
		return f;
	}
	FormatAmount(f,szText,szDefault);
	return f;
}



float CHQFunction::WeiBi()
{
	float fts = TotleSellVolume();
	float ftb = TotleBuyVolume();

	if(fts == 0.00f && ftb == 0.00f)
		return 0.00f;

	//委比=(委买手数－委卖手数)/(委买手数+委卖手数)×100%
	return (ftb - fts) / (ftb + fts) * 100;
}


float CHQFunction::WeiCha()
{
	float fts = TotleSellVolume();
	float ftb = TotleBuyVolume();
	return (ftb - fts);
}


float CHQFunction::WeiChaText(CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	float fWeiCha = WeiCha();
	if(fWeiCha == 0.00f)
	{
		szText = szDefault;
		return fWeiCha;
	}	

	if(fWeiCha > 0)
		szText.Format("+%d",(INT)fWeiCha);
	else
		szText.Format("%d",(INT)fWeiCha);
	
	return fWeiCha;
}



float CHQFunction::WeiBiText(CString & szText, LPCTSTR szDefault /*=" -"*/ )
{
	float fWeiBi = WeiBi();
	if(fWeiBi == 0.00f)
	{
		szText = szDefault;
		return fWeiBi;
	}		

	if(fWeiBi > 0)
		szText.Format("+%.2f%c",fWeiBi,'%');
	else
		szText.Format("%.2f%c",fWeiBi,'%');

	return fWeiBi;
}







