#ifndef HQFunction_h__
#define HQFunction_h__

#include "ComHeader/ComHeader.h"


class CHQFunction
{
public:
	static void FormatAmount(float f, CString & szText,LPCTSTR szDefault= "-");
	static void FormatVolume(float f, CString & szText,LPCTSTR szDefault= "-");
	INT			GetOpenTimeCount(time_t t);
public:
	CHQFunction();
	BOOL Init(const REALTIMEQUOTES	* pRTQ);

	float		Round(float val, int places);
	float       Round(float vaL);

	float		LiuTongShiZhi();
	float		ZongShiZhi();
	float		LiuTongZhiText(CString & szText ,LPCTSTR szDefault = "-");
	float       LiuTongBiLi();
	float		LiuTongBiLiText(CString & szText ,LPCTSTR szDefault = "-");
	float		ZongShiZhiText(CString & szText ,LPCTSTR szDefault = "-");

	LPCTSTR     ZuiXinJieJin(CString & szText ,LPCTSTR szDefault = "-");

	LPCTSTR     SuoShuHangYe();
	LPCTSTR     SuoShuHangYeText(CString & szText ,LPCTSTR szDefault = "-");
	
	LPCTSTR     ZhuYingYeWu();
	LPCTSTR     ZhuYingYeWuText(CString & szText ,LPCTSTR szDefault = "-");


	float       HangYeZhangFu();
	float       HangYeZhangFuText(CString & szText ,LPCTSTR szDefault = "-");	

	LPCTSTR     SheJiGaiNain();
	LPCTSTR     SheJiGaiNainText(CString & szText ,LPCTSTR szDefault = "-");


	DWORD GuDongShu();
	DWORD GuDongShuText(CString & szText ,LPCTSTR szDefault = "-");

	float RenJunChiGu();
	float RenJunChiGuSZText(CString & szText ,LPCTSTR szDefault = "-");

	//SH  散户
	float SHRenJunChiGu();
	float SHRenJunChiGuSZText(CString & szText ,LPCTSTR szDefault = "-");



	float GDJSQBH(); //股东人数较上期变化
	float GDJSQBHText(CString & szText ,LPCTSTR szDefault = "-");

	float QSDLTGD(); //前10大流通股东
	float QSDLTGDZBText(CString & szText ,LPCTSTR szDefault = "-");
	

	float LiangBi();
	float LiangBiText(CString & szText ,LPCTSTR szDefault = "-");

	float ShiYingDong();
	float ShiYingDongText(CString & szText ,LPCTSTR szDefault = "-");

	float Day5HuanShou();
	float Day5HuanShouText(CString & szText ,LPCTSTR szDefault = "-");
	
	float Day5Amount();
	float Day5AmountText(CString & szText ,LPCTSTR szDefault = "-");

	float HuanShou();
	float HuanShouText( CString & szText ,LPCTSTR szDefault = "-");
	void  HuanShouText( float fVolume, CString & szText ,LPCTSTR szDefault = "-");
	
	float MeiGuShowYi();
	float MeiGuShowYiText( CString & szText ,LPCTSTR szDefault = "-");

	float ShiJing();
	float ShiJingText( CString & szText ,LPCTSTR szDefault = "-");

	float JingZhi();
	float JingZhiText( CString & szText ,LPCTSTR szDefault = "-");

	//-----------盘口有关函数----------------------------------
	float		JunSellJia();
	float		JunSellJiaText(CString & szText, LPCTSTR szDefault ="-");

	float		JunBuyJia();
	float		JunBuyJiaText(CString & szText, LPCTSTR szDefault ="-");

	float		OpenPriceText(CString & szText, LPCTSTR szDefault ="-");
	float		LastCloseText(CString & szText, LPCTSTR szDefault ="-");
	float		LowPriceText(CString & szText,  LPCTSTR szDefault ="-");
	float		HighPriceText(CString & szText, LPCTSTR szDefault ="-");
	float		NewPriceText(CString & szText,  LPCTSTR szDefault ="-");

	float		SellPriceText(INT nIndex,CString & szText, LPCTSTR szDefault ="-");
	float		SellVolumeText(INT nIndex,CString & szText, LPCTSTR szDefault ="-");

	float		BuyPriceText(INT nIndex,CString & szText, LPCTSTR szDefault ="-");
	float		BuyVolumeText(INT nIndex,CString & szText, LPCTSTR szDefault ="-");

	void		PriceText(float fPrice, CString & szText, LPCTSTR szDefault ="-");
	
	float		TotleBuyVolume();
	float		TotleBuyVolumeText(CString & szText, LPCTSTR szDefault ="-");

	float		TotleBuyAmount();
	float		TotleBuyAmountText(CString & szText, LPCTSTR szDefault ="-");

	float		TotleSellAmount();
	float		TotleSellAmountText(CString & szText, LPCTSTR szDefault ="-");

	float		TotleSellVolume();
	float		TotleSellVolumeText(CString & szText, LPCTSTR szDefault ="-");

	float		WeiCha();
	float		WeiChaText(CString & szText,LPCTSTR szDefault ="-");

	float		WeiBi();
	float		WeiBiText(CString & szText,LPCTSTR szDefault ="-");

	float       AmountText(CString & szText,LPCTSTR szDefault ="-");
	float       VolumeText(CString & szText,LPCTSTR szDefault ="-");

	
	//-----------行情有关函数----------------------------------
	float		ZhangTing();
	float		ZhangTingText(CString & szText,LPCTSTR szDefault ="-");

	float		DieTing();
	float		DieTingText(CString & szText,LPCTSTR szDefault ="-");

	float		DieTing(float f);
	float		ZhangTing(float f);
	
	float		ZhengFu();
	float		ZhengFuText(CString & szText,LPCTSTR szDefault ="-");
	float		ZhengFuText(float fLastClose,float fHigh , float fLow,CString & szText,LPCTSTR szDefault ="-");


	float		ZhangFu();
	float		ZhangFuText(CString & szText,LPCTSTR szDefault ="-");
	float		ZhangFuText(float fLastClose,float fCurPrice,CString & szText,LPCTSTR szDefault ="-");

	float		ZhangSu(INT nMinutePeriod);
	float		ZhangSuText(INT nMinutePeriod , CString & szText,LPCTSTR szDefault ="-");

	float       ZhangFuValue();
	float		ZhangFuValueText(CString & szText,LPCTSTR szDefault ="-");

	BOOL		IsJiJin();	
	BOOL		IsST();
	BOOL		IsTP();
private:
	const REALTIMEQUOTES	* m_pRTQ;
	const FINANCIAL			* m_pFinData;
	HQBASE_INFO				  m_HQBaseInfo;
	BOOL					  m_bIsJiJin;
	BOOL					  m_bIsST;
};



#endif // HQFunction_h__