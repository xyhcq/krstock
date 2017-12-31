// KLineWidget.cpp : implementation file
//

#include "stdafx.h"
#include "KLineWidget.h"
#include "Widget/MemoryDC.h"
#include "StockAlgorithm.h"
#include <algorithm>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKLineWidget
#define		KLINEFILLPIXEL		3
#define		MAX_LEFT_FILL		40


CKLineWidget::CKLineWidget()
{
	m_nKLineWidth = 2;
	m_pKLineBegin = NULL;
	m_pKLineEnd   = NULL;
	m_bShowCross  = FALSE;
	m_fMA10 = NULL;
	m_fMA5  = NULL;
	m_dwPeriod = KLINE_PERIOD_DAY;
	m_nFuQuan  = KLINE_FUQUAN_QIAN;
	ZeroMemory(m_szLabel,sizeof(m_szLabel));
	ZeroMemory(&m_RTQ,sizeof(REALTIMEQUOTES));


}


CKLineWidget::~CKLineWidget()
{
	if(m_fMA10) delete [] m_fMA10;
	if(m_fMA5) delete [] m_fMA5;
}



BEGIN_MESSAGE_MAP(CKLineWidget, CStatic)
	//{{AFX_MSG_MAP(CKLineWidget)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_KRSDE_KLINE_REMOTE,OnKRSDE_KLine_Remote)
	ON_MESSAGE(WM_KRSDE_REALTIMEQUOTES_REMOTE,OnKRSDE_RealTimeQuotes_Remote)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKLineWidget message handlers
INT CKLineWidget::GetTextWidth(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cx;
}

INT CKLineWidget::GetTextHeight(CDC * pDC,LPCTSTR lpszText)
{
	return pDC->GetTextExtent(lpszText).cy;
}

const char *	CKLineWidget::FormatPeriod(DWORD dwPeriod)
{
	switch(dwPeriod)
	{
	case KLINE_PERIOD_F1:{     return "1分钟"; }break;
	case KLINE_PERIOD_F5:{     return "5分钟"; }break;
	case KLINE_PERIOD_F15:{    return "15分钟";}break;
	case KLINE_PERIOD_F30:{    return "30分钟";}break;
	case KLINE_PERIOD_F60:{    return "60分钟";}break;
	case KLINE_PERIOD_DAY:{    return "日线";}break;
	case KLINE_PERIOD_WEEK:{   return "周线"; }break;
	case KLINE_PERIOD_MONTH:{  return "月线";}break;
	case KLINE_PERIOD_QUARTER:{return "季线";}break;
	case KLINE_PERIOD_YEAR:{   return "年线";}break;
	}
	return "";
}

const char * CKLineWidget::FormatFuQuan(INT nFuQuan)
{
	switch(nFuQuan)
	{
	case KLINE_FUQUAN_QIAN:
		return "前复权";
	case KLINE_FUQUAN_HOU:
		return "后复权";
	case KLINE_FUQUAN_NO:
		return "不复权";
	}
	return "未知复权";
}

void CKLineWidget::FormatTime(CTime t , CString & szText)
{
	char * pWeek[]={"","星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
	switch(m_dwPeriod)
	{
		
	case KLINE_PERIOD_F1:
	case KLINE_PERIOD_F5:
	case KLINE_PERIOD_F15:
	case KLINE_PERIOD_F30:
	case KLINE_PERIOD_F60:
		szText.Format("%04u-%02u-%02u %02u:%02u:%02u",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
		break;
	case KLINE_PERIOD_DAY:
	case KLINE_PERIOD_WEEK:
	case KLINE_PERIOD_MONTH:
	case KLINE_PERIOD_QUARTER:
	case KLINE_PERIOD_YEAR:
		szText.Format("%04u-%02u-%02u %s",t.GetYear(),t.GetMonth(),t.GetDay(),pWeek[t.GetDayOfWeek()]);
		break;
	}
}


void CKLineWidget::InitMacd()
{
	m_DIFF.clear();
	if(m_VKLine.size() < 26)
		return;

	vector<float> EMA26 =  MakeEMA(m_VKLine,26);
	vector<float> EMA12 =  MakeEMA(m_VKLine,12);

	//计算DIFF  =  EMA12 - EMA26
	for(INT i=0;i < EMA26.size(); i++)
		m_DIFF.push_back( EMA12[i] - EMA26[i]);

	//计算DEA
	m_DEA = MakeEMA(m_DIFF,9);

	//计算MACD
	m_MACD.clear();
	for (i=0;i< m_DIFF.size() ; i++)
		m_MACD.push_back((m_DIFF[i] - m_DEA[i]) * 2);
}


void CKLineWidget::InitKRMacd()
{
	m_KRMACD.clear();
	if(m_VKLine.size() < 11)
		return;

	if(m_fMA10) delete [] m_fMA10;
	if(m_fMA5) delete [] m_fMA5;

	m_fMA10 = MakeMA(&m_VKLine[0],m_VKLine.size(),10);
	if(m_fMA10 == NULL)
		return;

	m_fMA5  = MakeMA(&m_VKLine[0],m_VKLine.size(),5);
	if(m_fMA5 == NULL)
	{
		delete [] m_fMA10;
		return;
	}

	for (INT i=0; i< m_VKLine.size(); i++)
	{
		if(m_fMA10[i] == 0)
		{
			m_KRMACD.push_back(0.0f);
		}else
		{
			m_KRMACD.push_back( (m_fMA5[i] - m_fMA10[i]) * 2);
		}
	}	
}

void CKLineWidget::InitKLine()
{
	INT nDrawCount = GetUsableDrawLineCount();

	//防止越界
	if(nDrawCount < m_VKLine.size())
	{
		m_pKLineBegin =  &m_VKLine[ m_VKLine.size() - nDrawCount ];
		m_pKLineEnd   =  &m_VKLine[ m_VKLine.size() ];
	}else{
		m_pKLineBegin = &m_VKLine[0];
		m_pKLineEnd   = &m_VKLine[ m_VKLine.size() ];
	}
	Price_HH_LL(m_pKLineBegin,m_pKLineEnd,m_fHH,m_fLL);
}

void CKLineWidget::InitKLineMA()
{
	if(m_VKLine.size() == 0)
		return;
	MakeMA(m_VKLine.begin(),m_VKLine.size(),5,m_MA5);
	MakeMA(m_VKLine.begin(),m_VKLine.size(),10,m_MA10);
	MakeMA(m_VKLine.begin(),m_VKLine.size(),20,m_MA20);
	MakeMA(m_VKLine.begin(),m_VKLine.size(),30,m_MA30);
	MakeMA(m_VKLine.begin(),m_VKLine.size(),60,m_MA60);
	MakeMA(m_VKLine.begin(),m_VKLine.size(),120,m_MA120);
	MakeMA(m_VKLine.begin(),m_VKLine.size(),250,m_MA250);
}


void CKLineWidget::InitVolumeMA()
{
	if(m_VKLine.size() == 0)
		return;

	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),5,m_VolMA5);
	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),10,m_VolMA10);
	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),20,m_VolMA20);
	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),30,m_VolMA30);
	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),60,m_VolMA60);
	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),120,m_VolMA120);
	MakeVolumeMA(m_VKLine.begin(),m_VKLine.size(),250,m_VolMA250);
}


LRESULT CKLineWidget::OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam)
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

	if(pRTQ == NULL || Amount_Equality(pRTQ->m_fAmount , m_RTQ.m_fAmount) )	
		return FALSE;

	m_RTQ = *pRTQ;

	vector< KLINEDATA > VKLData;
	KRSDE.GetKLine_Local(m_szLabel,m_dwPeriod,m_nFuQuan,VKLData);
	

	/*
		更新K线要点 
		1 size 不一样 ，如果 m_pKLineEnd   =  &m_VKLine[ m_VKLine.size() ];
		则 m_pKLineEnd 继续指向尾部，同时 m_pKLineBegin 往右偏移
		m_nCrossIndex  往右偏移
		同时初始化控件显示
	*/

	INT nRange = VKLData.size() - m_VKLine.size();
	m_VKLine = VKLData;

 	if(nRange != 0)
 	{
 		SafeAdjustEnd(nRange);
 		SafeAdjustBegin(nRange);
 		m_nCrossIndex+= nRange;
		Price_HH_LL(m_pKLineBegin,m_pKLineEnd,m_fHH,m_fLL);
	}

	CRect rc;
	GetClientRect(rc);
	AdjustRC(rc);

	InitKLineMA();
	InitVolumeMA();
	InitMacd();
	InitKRMacd();
	InitWave(m_VKLine.begin() , m_VKLine.size() ,m_VKRWave);

	RedrawWindow();
	return TRUE;
}




enum{
		MENU_KLINE_PERIOD_F1 = 0x100,
		MENU_KLINE_PERIOD_F5,
		MENU_KLINE_PERIOD_F15,
		MENU_KLINE_PERIOD_F30,
		MENU_KLINE_PERIOD_F60,
		MENU_KLINE_PERIOD_DAY,
		MENU_KLINE_PERIOD_WEEK,
		MENU_KLINE_PERIOD_MONTH,
		MENU_KLINE_PERIOD_QUARTER,
		MENU_KLINE_PERIOD_YEAR,
		MENU_KLINE_MA_5,
		MENU_KLINE_MA_10,
		MENU_KLINE_MA_20,
		MENU_KLINE_MA_30,
		MENU_KLINE_MA_60,
		MENU_KLINE_MA_120,
		MENU_KLINE_MA_250,
		MENU_VOL_MA_5,
		MENU_VOL_MA_10,
		MENU_VOL_MA_20,
		MENU_VOL_MA_30,
		MENU_VOL_MA_60,
		MENU_VOL_MA_120,
		MENU_VOL_MA_250,
		MENU_KLINE_FUQUAN_NO,   
		MENU_KLINE_FUQUAN_QIAN, 
		MENU_KLINE_FUQUAN_HOU,
		MENU_KLINE_FUQUAN_VOL,
		MENU_DRAW_GAP,
		MENU_DRAW_WAVE,
		MENU_DRAW_WAVENUM
};

void CKLineWidget::SetLabel(const char * szLabel)
{
	KRSDE.GetKLine_Remote(szLabel,m_dwPeriod,m_nFuQuan,5000);
}


LRESULT CKLineWidget::OnKRSDE_KLine_Remote(WPARAM wParam, LPARAM lParam)
{
	LPTOKEN_KLINE_HEADINFO	lpTKLHeadInfo		= (LPTOKEN_KLINE_HEADINFO)wParam;
	LPKLINEDATA				lpKLData			= (LPKLINEDATA)lParam;
	
	strcpy(m_szLabel,lpTKLHeadInfo->szLabel);
	m_dwPeriod  = lpTKLHeadInfo->dwPeriod;
	m_nFuQuan   = lpTKLHeadInfo->nFuQuan;
	
	m_VKLine.reserve(lpTKLHeadInfo->dwCount * 2);
	m_VKLine.assign(lpKLData,lpKLData+ lpTKLHeadInfo->dwCount);


	CopyMemory(&m_RTQ,KRSDE.GetRealTimeQuotes(m_szLabel),sizeof(REALTIMEQUOTES));
	m_HQFunction.Init(&m_RTQ);

	m_SER.clear();
	KRSDE.GetExRights(m_szLabel,m_SER);


	//上证指数 日线数据
	vector<KLINEDATA> vkl;
	if(KRSDE.GetKLine_Local("SH000001",KLINE_PERIOD_DAY,KLINE_FUQUAN_NO,vkl))
	{
		m_STP.clear();
		for (INT i=0; i< vkl.size();i++)
		{
			CTime t1 (vkl[i].tUnix);
			CTime t2(t1.GetYear(),t1.GetMonth(),t1.GetDay(),0,0,0);
			m_STP.insert(t2.GetTime());
		}
	}


	//菜单选中
	for(INT i = MENU_KLINE_PERIOD_F1; i<=MENU_KLINE_PERIOD_YEAR;i++)
		m_KLineMenu_Period.CheckMenuItem(i, MF_BYCOMMAND | ( (i -MENU_KLINE_PERIOD_F1) == (m_dwPeriod -KLINE_PERIOD_F1) )  ? MF_CHECKED : MF_UNCHECKED); 

	m_nCrossIndex = m_VKLine.size() - 1;

	CRect rc;
	GetClientRect(rc);
	AdjustRC(rc);

	InitKLineMA();
	InitVolumeMA();
	InitMacd();
	InitKRMacd();
	InitKLine();
	InitWave(m_VKLine.begin() , m_VKLine.size() ,m_VKRWave);

 	RedrawWindow();
	return TRUE;
}



LRESULT CKLineWidget::OnMyCommand(WPARAM wParam, LPARAM lParam)
{
	if(HIWORD(wParam) == 0 )
	{
		switch(LOWORD(wParam))
		{
		case MENU_KLINE_MA_5:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_5, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_5,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_KLINE_MA_10:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_10, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_10,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_KLINE_MA_20:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_20, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_20,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_KLINE_MA_30:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_30, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_30,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_KLINE_MA_60:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_60, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_60,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		case MENU_KLINE_MA_120:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_120, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_120,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		case MENU_KLINE_MA_250:
			m_KLineMenu_MA.CheckMenuItem(MENU_KLINE_MA_250, MF_BYCOMMAND | (m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_250,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;

		case MENU_KLINE_PERIOD_F1:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_F1,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_F5:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_F5,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_F15:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_F15,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_F30:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_F30,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_F60:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_F60,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_DAY:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_DAY,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_WEEK:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_WEEK,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_MONTH:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_MONTH,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_QUARTER:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_QUARTER,m_nFuQuan,5000);
			break;
		case MENU_KLINE_PERIOD_YEAR:
			KRSDE.GetKLine_Remote(m_szLabel,KLINE_PERIOD_YEAR,m_nFuQuan,5000);
			break;

		case MENU_VOL_MA_5:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_5, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_5,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_VOL_MA_10:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_10, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_10,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_VOL_MA_20:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_20, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_20,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_VOL_MA_30:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_30, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_30,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED); 
			break;
		case MENU_VOL_MA_60:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_60, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_60,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		case MENU_VOL_MA_120:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_120, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_120,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		case MENU_VOL_MA_250:
			m_VolMenu_MA.CheckMenuItem(MENU_VOL_MA_250, MF_BYCOMMAND | (m_VolMenu_MA.GetMenuState(MENU_VOL_MA_250,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;

		case MENU_KLINE_FUQUAN_NO:
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_NO,    MF_BYCOMMAND | MF_CHECKED);
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_QIAN  ,  MF_BYCOMMAND | MF_UNCHECKED);
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_HOU   ,   MF_BYCOMMAND | MF_UNCHECKED);
			m_FuQuanMenu_Context.EnableMenuItem(MENU_KLINE_FUQUAN_VOL  ,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			KRSDE.GetKLine_Remote(m_szLabel,m_dwPeriod,KLINE_FUQUAN_NO,5000);
			break;

		case MENU_KLINE_FUQUAN_QIAN:
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_QIAN,  MF_BYCOMMAND | MF_CHECKED);
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_NO, MF_BYCOMMAND  | MF_UNCHECKED);
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_HOU, MF_BYCOMMAND | MF_UNCHECKED);
			//m_FuQuanMenu_Context.EnableMenuItem(MENU_KLINE_FUQUAN_VOL  ,MF_BYCOMMAND | MF_ENABLED);
			KRSDE.GetKLine_Remote(m_szLabel,m_dwPeriod,KLINE_FUQUAN_QIAN,5000);
			break;
		case MENU_KLINE_FUQUAN_HOU:
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_HOU,  MF_BYCOMMAND | MF_CHECKED);
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_NO, MF_BYCOMMAND  | MF_UNCHECKED);
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_QIAN, MF_BYCOMMAND | MF_UNCHECKED);
			//m_FuQuanMenu_Context.EnableMenuItem(MENU_KLINE_FUQUAN_VOL  ,MF_BYCOMMAND | MF_ENABLED);
			KRSDE.GetKLine_Remote(m_szLabel,m_dwPeriod,KLINE_FUQUAN_HOU,5000);
			break;
		case MENU_KLINE_FUQUAN_VOL:
			m_FuQuanMenu_Context.CheckMenuItem(MENU_KLINE_FUQUAN_VOL,
				MF_BYCOMMAND | (m_FuQuanMenu_Context.GetMenuState(MENU_KLINE_FUQUAN_VOL,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		case MENU_DRAW_GAP:
			m_KLineMenu_Sub.CheckMenuItem(MENU_DRAW_GAP,MF_BYCOMMAND | (m_KLineMenu_Sub.GetMenuState(MENU_DRAW_GAP,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		case MENU_DRAW_WAVE:
			m_KLineMenu_Sub.CheckMenuItem(MENU_DRAW_WAVE,MF_BYCOMMAND | (m_KLineMenu_Sub.GetMenuState(MENU_DRAW_WAVE,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;

		case MENU_DRAW_WAVENUM:
			m_KLineMenu_Sub.CheckMenuItem(MENU_DRAW_WAVENUM,MF_BYCOMMAND | (m_KLineMenu_Sub.GetMenuState(MENU_DRAW_WAVENUM,MF_BYCOMMAND) == MF_CHECKED) ? MF_UNCHECKED:MF_CHECKED);
			break;
		}
		RedrawWindow();
	}
	return TRUE;
}




int CKLineWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	KRSDE.SetHWND(m_hWnd);

	m_KLineMenu.CreateMenu();	
	m_KLineMenu_Sub.CreatePopupMenu();
	m_KLineMenu.AppendMenu(MF_POPUP,(UINT)m_KLineMenu_Sub.m_hMenu,"K线右键菜单");

	m_KLineMenu_Period.CreatePopupMenu();
	m_KLineMenu_Sub.AppendMenu(MF_POPUP,(UINT)m_KLineMenu_Period.m_hMenu,"分析周期");
	m_KLineMenu_Sub.AppendMenu(MF_SEPARATOR);

	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_F1,"1分钟" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_F5,"5分钟" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_F15,"15分钟" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_F30,"30分钟" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_F60,"60分钟" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_DAY,"日线" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_WEEK,"周线" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_MONTH,"月线" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_QUARTER,"季线" );
	m_KLineMenu_Period.AppendMenu(MF_STRING,MENU_KLINE_PERIOD_YEAR,"年线" );


	m_KLineMenu_MA.CreatePopupMenu();
	m_KLineMenu_Sub.AppendMenu(MF_POPUP,(UINT)m_KLineMenu_MA.m_hMenu,"均线系统");
	

	m_KLineMenu_MA.AppendMenu(MF_STRING|MF_CHECKED,MENU_KLINE_MA_5,"5日线\tMA5");
	m_KLineMenu_MA.AppendMenu(MF_STRING|MF_CHECKED,MENU_KLINE_MA_10,"10日线\tMA10");
	m_KLineMenu_MA.AppendMenu(MF_STRING,MENU_KLINE_MA_20,"20日线\tMA20");
	m_KLineMenu_MA.AppendMenu(MF_STRING,MENU_KLINE_MA_30,"30日线\tMA30");
	m_KLineMenu_MA.AppendMenu(MF_STRING|MF_CHECKED,MENU_KLINE_MA_60,"60日线\tMA60");
	m_KLineMenu_MA.AppendMenu(MF_STRING|MF_CHECKED,MENU_KLINE_MA_120,"120日线\tMA120");
	m_KLineMenu_MA.AppendMenu(MF_STRING,MENU_KLINE_MA_250,"250日线\tMA250");

	m_KLineMenu_Sub.AppendMenu(MF_SEPARATOR);
	m_FuQuanMenu_Context.CreatePopupMenu();
	m_KLineMenu_Sub.AppendMenu(MF_POPUP,(UINT)m_FuQuanMenu_Context.m_hMenu,"复权");
	m_FuQuanMenu_Context.AppendMenu(MF_STRING|MF_CHECKED,MENU_KLINE_FUQUAN_QIAN,"向前复权");
	m_FuQuanMenu_Context.AppendMenu(MF_STRING,MENU_KLINE_FUQUAN_HOU,"向后复权");
	m_FuQuanMenu_Context.AppendMenu(MF_SEPARATOR);
	m_FuQuanMenu_Context.AppendMenu(MF_STRING,MENU_KLINE_FUQUAN_NO,"不复权");
	m_FuQuanMenu_Context.AppendMenu(MF_SEPARATOR);
	m_FuQuanMenu_Context.AppendMenu(MF_STRING|MF_DISABLED|MF_GRAYED,MENU_KLINE_FUQUAN_VOL,"成交量复权");

	m_KLineMenu_Sub.AppendMenu(MF_SEPARATOR);
	m_KLineMenu_Sub.AppendMenu(MF_STRING|MF_CHECKED,MENU_DRAW_GAP,"显示缺口");
	m_KLineMenu_Sub.AppendMenu(MF_STRING|MF_CHECKED,MENU_DRAW_WAVE,"自动画浪");
	m_KLineMenu_Sub.AppendMenu(MF_STRING|MF_CHECKED,MENU_DRAW_WAVENUM,"显示浪标");

	m_VolMenu.CreateMenu();	
	m_VolMenu_Sub.CreatePopupMenu();
	m_VolMenu.AppendMenu(MF_POPUP,(UINT)m_VolMenu_Sub.m_hMenu,"K线右键菜单");

	m_VolMenu_MA.CreatePopupMenu();
	m_VolMenu_Sub.AppendMenu(MF_POPUP,(UINT)m_VolMenu_MA.m_hMenu,"均线系统");
	m_VolMenu_MA.AppendMenu(MF_STRING|MF_CHECKED,MENU_VOL_MA_5,"5日线\tMA5");
	m_VolMenu_MA.AppendMenu(MF_STRING|MF_CHECKED,MENU_VOL_MA_10,"10日线\tMA10");
	m_VolMenu_MA.AppendMenu(MF_STRING,MENU_VOL_MA_20,"20日线\tMA20");
	m_VolMenu_MA.AppendMenu(MF_STRING,MENU_VOL_MA_30,"30日线\tMA30");
	m_VolMenu_MA.AppendMenu(MF_STRING,MENU_VOL_MA_60,"60日线\tMA60");
	m_VolMenu_MA.AppendMenu(MF_STRING,MENU_VOL_MA_120,"120日线\tMA120");
	m_VolMenu_MA.AppendMenu(MF_STRING,MENU_VOL_MA_250,"250日线\tMA250");
	


	m_Font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体"));
	
	CDC * pDc = GetWindowDC();
	pDc->SelectObject(m_Font);

	m_nSTextHeight = GetTextHeight(pDc,"1");
	m_nSTextWidth  = GetTextWidth(pDc,"1");
	ReleaseDC(pDc);

	return 0;
}





INT CKLineWidget::GetUsableDrawLineCount()
{
	INT nWidth = m_RCKLine.Width();
	if(m_nKLineWidth + KLINEFILLPIXEL == 0)
		return	nWidth;

	return  nWidth / (m_nKLineWidth + KLINEFILLPIXEL);
}


INT CKLineWidget::PriceToY(float fValue )
{
	if(m_fHH == m_fLL)
		return m_RCKLine.top + m_nSTextHeight;

	float YBase = (float)(m_RCKLine.Height() - m_nSTextHeight * 3) / ( m_fHH - m_fLL );
	INT Y =   ( m_fHH - fValue ) *  YBase  + m_RCKLine.top + m_nSTextHeight;

	if(Y <  m_RCKLine.top + m_nSTextHeight)
		Y =  m_RCKLine.top + m_nSTextHeight;

	if(Y >  m_RCKLine.bottom)
		Y =  m_RCKLine.bottom;

	return Y;
}



float CKLineWidget::YToPrice(INT Y)
{
	Y -= m_RCKLine.top + m_nSTextHeight;
	float YBase = (m_RCKLine.Height() - m_nSTextHeight * 2)  / ( m_fHH - m_fLL );
	return m_fHH - (float)Y / YBase;
}



INT CKLineWidget::IndexToX(INT nIndex)
{
	INT nBegin = m_pKLineBegin - &m_VKLine[ 0 ];
	if(nIndex < nBegin)
		return -1;
	
	nIndex = nIndex - nBegin;
	INT nEnd = m_VKLine.size() - (&m_VKLine[ m_VKLine.size() ] - m_pKLineEnd) - nBegin;
	if(nIndex >= nEnd) 
		return m_RCKLine.right;

	return (INT)(nIndex * (m_nKLineWidth + KLINEFILLPIXEL)) +  (m_nKLineWidth / 2);	
}


INT CKLineWidget::OffsetToX(INT nOffset){
	if(nOffset < 0 || nOffset > m_VKLine.size())
		return -1;

	return (INT)(nOffset * (m_nKLineWidth + KLINEFILLPIXEL));
}

INT CKLineWidget::OffsetToX(LPKLINEDATA  pOffset ){
	INT nOffset = pOffset - m_pKLineBegin;
	return OffsetToX(nOffset);
}

INT CKLineWidget::GetBeginIndex()
{
	return m_pKLineBegin - &m_VKLine[0];
}

INT CKLineWidget::GetEndIndex()
{
	return m_pKLineEnd - &m_VKLine[0];
}

void CKLineWidget::SafeAdjustBegin(INT nJust)
{
	if(m_pKLineBegin == NULL) 
		return;
	
	m_pKLineBegin += nJust;
	if(m_pKLineBegin < &m_VKLine[0])
		m_pKLineBegin = &m_VKLine[0];
}


void CKLineWidget::SafeAdjustEnd(INT nJust)
{
	if(m_pKLineEnd == NULL)
		return;
	m_pKLineEnd   += nJust;
	if(m_pKLineEnd > & m_VKLine[m_VKLine.size()])
		m_pKLineEnd = &m_VKLine[m_VKLine.size()];
}


INT CKLineWidget::XToIndex(INT X)
{
	//1 确定真实的nIndex
	INT nBegin = m_pKLineBegin - &m_VKLine[ 0 ];
	INT nIndex = X / (m_nKLineWidth+KLINEFILLPIXEL);
	return nIndex + nBegin;	
}


void CKLineWidget::DrawGap(CDC* pDC)
{
	INT nBegin = GetBeginIndex();
	INT nEnd   = GetEndIndex();

	for(INT i=nBegin+1; i< nEnd ; i++)
	{
		//有缺口
		if(m_VKLine[i].fHigh < m_VKLine[i-1].fLow)
		{
			BOOL bCheck = FALSE;
			float fPrice = m_VKLine[i].fHigh;
			
			//检查该缺口是否已经补了
			for(INT n=i; n< nEnd; n++)
			{
				if( m_VKLine[n].fHigh > fPrice )
					fPrice = m_VKLine[n].fHigh;
				if(m_VKLine[n].fHigh >= m_VKLine[i-1].fLow)
				{
					bCheck=TRUE;
					break;
				}
			}
			
			//缺口没有补
			if(bCheck == FALSE)
			{
				CBrush GrayBrush(COLOR_SHENHUI);
				CRect FRc;
				FRc.left = IndexToX(i-1);
				FRc.right = m_RCKLine.right;
				FRc.top   =  PriceToY(m_VKLine[i-1].fLow);
				FRc.bottom = PriceToY(fPrice);
				if(FRc.Height() < 1)
				  FRc.bottom+=1;
				
				pDC->FillRect(&FRc,&GrayBrush);
	
				CString szText,szTemp;
				m_HQFunction.PriceText(m_VKLine[i-1].fLow,szText);
				szText +=" - ";
				m_HQFunction.PriceText(m_VKLine[i].fHigh,szTemp);
				szText +=szTemp;
				KLineTextOut(pDC,FRc.left + GetTextWidth(pDC),FRc.top - GetTextHeight(pDC),szText,COLOR_SHENHUI);
			}
		}
	}
}




void CKLineWidget::KLineTextOut(CDC * pDc,int x, int y,LPCTSTR lpszText,COLORREF crColor ){

	CRect rc;
	rc.left = x;
	rc.right = x+ pDc->GetTextExtent(lpszText).cx;

	rc.top  = y;
	rc.bottom = y + pDc->GetTextExtent(lpszText).cy;

	if(rc.right > m_RCYInfo.left)
		rc.right = m_RCYInfo.left - 2;

	if(rc.bottom > m_RCKLine.bottom)
		rc.bottom = m_RCKLine.bottom - 2;

	pDc->SetTextColor(crColor);
	pDc->DrawText(lpszText,rc,DT_LEFT);

}


void CKLineWidget::DrawKLine(CDC * pDC)
{
	CPen  RedPen(PS_SOLID,1,COLOR_RED);
	CPen  BulePen(PS_SOLID,1,COLOR_TIANLAN);
	CPen  WhitePen(PS_SOLID,1,COLOR_WHITE);

	CBrush BuleBrush(COLOR_TIANLAN);
	CBrush RedBrush(COLOR_RED);
	CBrush BgBrush(RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE));


	CString szText,szTemp;
	INT xOffset = m_RCKLine.left;
	for (KLINEDATA * pKLineData =m_pKLineBegin; pKLineData < m_pKLineEnd ; pKLineData++ )
	{
		INT nClose = PriceToY(pKLineData->fClose) , 
			nOpen  = PriceToY(pKLineData->fOpen),
			nHigh  = PriceToY(pKLineData->fHigh),  
			nLow   = PriceToY(pKLineData->fLow);

		if(m_nKLineWidth > 1)
		{
			//阳线 
			if(pKLineData->fClose > pKLineData->fOpen)
			{
				pDC->SelectObject(RedPen);
				CRect FrameRc( xOffset , nClose , xOffset + m_nKLineWidth +  1, nOpen);
				pDC->FrameRect(&FrameRc,&RedBrush);
				//要填充背景色
				if(m_nKLineWidth >=2 &&  nOpen - nClose  > 4)
				{
					CRect FillRc( xOffset + 1, nClose + 1 , xOffset+m_nKLineWidth, nOpen - 1);
					pDC->FillRect(FillRc,&BgBrush);
				}
			}else if(pKLineData->fClose < pKLineData->fOpen) //阴线
			{
				pDC->SelectObject(BulePen);
				if(nOpen - nClose == 0)
				{
					pDC->MoveTo( xOffset,nClose  );
					pDC->LineTo( xOffset+m_nKLineWidth,nClose );
					pDC->MoveTo(xOffset,nOpen);
					pDC->LineTo(xOffset+m_nKLineWidth + 1,nOpen);
				}else
				{
					CRect FillRc( xOffset , nOpen , xOffset+m_nKLineWidth +  1, nClose);
					pDC->FillRect(&FillRc,&BuleBrush);
				}
			}else //一字版
			{
				pDC->SelectObject(WhitePen);
				pDC->MoveTo(xOffset,nClose);
				pDC->LineTo(xOffset+m_nKLineWidth + 1,nClose);
			}
			//上影线
			if(pKLineData->fHigh > pKLineData->fClose)
			{
				INT YHigh  = nHigh;
				if(nClose - YHigh < 1)
					YHigh = YHigh - 2;
				pDC->MoveTo(xOffset + (m_nKLineWidth / 2),YHigh);
				pDC->LineTo(xOffset + (m_nKLineWidth / 2),nClose);
			}
			//下影线
			if(pKLineData->fLow < pKLineData->fOpen)
			{
				INT YLow = nLow;
				if(YLow - nOpen  < 1)
					YLow = YLow + 2;
				pDC->MoveTo(xOffset + (m_nKLineWidth / 2),nOpen);
				pDC->LineTo(xOffset + (m_nKLineWidth / 2),YLow);
			}
		}else
		{
			if(pKLineData->fClose > pKLineData->fOpen)
			{
				pDC->SelectObject(RedPen);
				pDC->MoveTo(xOffset + (m_nKLineWidth / 2),PriceToY(pKLineData->fHigh));
				pDC->LineTo(xOffset + (m_nKLineWidth / 2),PriceToY(pKLineData->fLow));
			}
			else if(pKLineData->fClose < pKLineData->fOpen)
			{
				pDC->SelectObject(BulePen);
				pDC->MoveTo(xOffset + (m_nKLineWidth / 2),PriceToY(pKLineData->fHigh));
				pDC->LineTo(xOffset + (m_nKLineWidth / 2),PriceToY(pKLineData->fLow));
			}else 
			{
				pDC->SelectObject(WhitePen);
				pDC->SetPixel(xOffset,nClose,COLOR_WHITE);
			}
		}

		xOffset += KLINEFILLPIXEL;
		xOffset += m_nKLineWidth;
	}

	KLINEDATA * pHH, *pLL;
	Price_HH_LL(m_pKLineBegin,m_pKLineEnd,pHH,pLL);


	if(pHH > pLL)
	{
		szText = " ←";
		m_HQFunction.PriceText(pHH->fHigh,szTemp,"-");
		szText+=szTemp;

		m_HQFunction.ZhengFuText(pLL->fLow,pHH->fHigh,pLL->fLow,szTemp,"-");
		szText+=" +";
		szText+=szTemp;
	}
	else
	{
		szText = " ←";
		m_HQFunction.PriceText(pLL->fLow,szTemp,"-");
		szText+=szTemp;
	}
	KLineTextOut(pDC,OffsetToX(pHH), PriceToY(pHH->fHigh),szText, COLOR_RED);

	
	if(pLL > pHH)
	{
		szText = " ←";
		m_HQFunction.PriceText(pLL->fLow,szTemp,"-");
		szText+=szTemp;
		m_HQFunction.ZhengFuText(pLL->fHigh,pLL->fLow,pHH->fHigh,szTemp,"-");
		szText+=" ";
		szText+=szTemp;
	}
	else
	{
		szText = " ←";
		m_HQFunction.PriceText(pLL->fLow,szTemp,"-");
		szText+=szTemp;
	}
	KLineTextOut(pDC,OffsetToX(pLL), PriceToY(pLL->fLow) ,szText, COLOR_GREEN);


	//画Y 坐标 
	pDC->SetTextColor(COLOR_RED);
	pDC->SelectObject(RedPen);
	float fBase = (m_fHH - m_fLL) / 13;
	for(int i=0;i<=13; i++)
	{
		float f = m_fLL + fBase * i;
		m_HQFunction.PriceText(f,szText);
		float Y = PriceToY(f);
		pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,Y - GetTextHeight(pDC,szText) / 2,szText);
		pDC->MoveTo(m_RCYInfo.left+1,Y);
		pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,Y);
	}
	
	INT YBase = PriceToY((m_pKLineEnd - 1)->fClose);
	for (INT s=m_RCKLine.left ;s < m_RCYInfo.left; s++)
	{
		if(s % 4 ==0) 
			pDC->SetPixel(s,YBase,COLOR_WHITE);
	}

	pDC->SetBkMode(OPAQUE);
	pDC->SetBkColor(RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE));
	pDC->SetTextColor(::GetColor(m_RTQ.m_fLastClose,(m_pKLineEnd - 1)->fClose));
	m_HQFunction.PriceText((m_pKLineEnd - 1)->fClose,szText);
	szText+=" ";
	pDC->TextOut(m_RCYInfo.left + 1, YBase - GetTextHeight(pDC,szText) / 2,szText);
	pDC->SetBkMode(TRANSPARENT);
}

void    CKLineWidget::DrawKLineMakeHistory(CDC * pDC)
{
	//非日线级别 不显示
	if(m_dwPeriod != KLINE_PERIOD_DAY)
		return;


	float fLastClose = 0.00f;
	INT nTextHeight = pDC->GetTextExtent("涨").cy;
	INT nTextWidth  = pDC->GetTextExtent("涨").cx;
	pDC->SetBkMode(OPAQUE);
	

	for ( KLINEDATA * pOffset = m_pKLineBegin; pOffset < m_pKLineEnd; pOffset++ )
	{
		if(pOffset == m_pKLineBegin)
			continue;

		//一字版跳过
		if(pOffset->fHigh == pOffset->fLow)
			continue;

// 		CTime t(pOffset->tUnix);
// 		if(t.GetDay() == 15 && t.GetMonth() == 8)
// 		{
// 			pOffset = pOffset;
// 		}

		CTime tTemp = pOffset->tUnix;
		CTime tKL(tTemp.GetYear(), tTemp.GetMonth(),tTemp.GetDay(),0,0,0);
		
		//停牌
		set<time_t>::const_iterator it = m_STP.find(tKL.GetTime());
		if(it!= m_STP.end())
		{
			it--;
			CTime tSH = *it;
			CTime t1  = (pOffset-1)->tUnix;
			CTime t2 (t1.GetYear(), t1.GetMonth(),t1.GetDay(),0,0,0);
			if(tSH != t2)
			{
				pDC->SetBkColor(COLOR_VIOLET);
				pDC->SetTextColor(RGB(255,255,255));
				pDC->TextOut( OffsetToX(pOffset-1) + m_nKLineWidth/2 - nTextWidth/2 ,  m_RCKLine.bottom - nTextHeight ,"停");
				continue;
			}
		}

		
		//除权
		if( m_SER.count(tKL.GetTime()) )
		{
			pDC->SetBkColor(COLOR_TIANLAN);
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOut( OffsetToX(pOffset) + m_nKLineWidth/2 - nTextWidth/2 ,  m_RCKLine.bottom - nTextHeight ,"权");
			continue;
		}


		fLastClose = (pOffset-1)->fClose;
		
		if( Price_Equality(m_HQFunction.ZhangTing(fLastClose), m_HQFunction.Round(pOffset->fHigh)  ) )
		{
			pDC->SetBkColor(COLOR_RED);
			pDC->SetTextColor(RGB(255,255,255));
			pDC->TextOut( OffsetToX(pOffset) + m_nKLineWidth/2 - nTextWidth/2 ,  m_RCKLine.bottom - nTextHeight ,"涨");
			continue;
		}

		if( Price_Equality(m_HQFunction.DieTing(fLastClose), pOffset->fLow) )
		{
			pDC->SetBkColor(COLOR_GREEN);
			pDC->SetTextColor(RGB(255,255,255));
			pDC->TextOut( OffsetToX(pOffset) + m_nKLineWidth/2 - nTextWidth/2 ,  m_RCKLine.bottom - nTextHeight ,"跌");
			continue;
		}
		
		if( (pOffset->fHigh - pOffset->fLow) / (pOffset-1)->fClose  > 0.11)
		{
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(COLOR_YELLOW);
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOut( OffsetToX(pOffset),  m_RCKLine.bottom - nTextHeight ,"振");
			continue;
		}
	}

	pDC->SetBkMode(TRANSPARENT);
}





void CKLineWidget::DrawMALine(CDC * pDC,vector<float> & MA, COLORREF color)
{
	if(MA.size() == 0)
		return;

	CPen  MAPen(PS_SOLID,1,color);
	pDC->SelectObject(MAPen);
	BOOL bMAFirst	=	TRUE;
	INT nBegin		= GetBeginIndex();
	INT nEnd		= GetEndIndex();
	for (INT i = nBegin; i< nEnd; i++ )
	{
		if(MA[i] > 0.0f)
		{
			if(bMAFirst == TRUE)
			{
				pDC->MoveTo(IndexToX(i),PriceToY(MA[i]));
				bMAFirst = FALSE;
			}else
			{
				pDC->LineTo(IndexToX(i),PriceToY(MA[i]));
				if(i + 1 < nEnd ) pDC->MoveTo(IndexToX(i),PriceToY(MA[i]));
			}
		}
	}
}



void CKLineWidget::DrawMALine(CDC * pDc)
{
	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_5,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA5,COLOR_WHITE);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_10,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA10,COLOR_YELLOW);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_20,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA20,COLOR_VIOLET);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_30,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA30,COLOR_SHENLAN);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_60,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA60,COLOR_GREEN);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_120,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA120,COLOR_GRAY);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_250,MF_BYCOMMAND) == MF_CHECKED)
		DrawMALine(pDc,m_MA250,COLOR_BLUE);
}



void CKLineWidget::DrawVolumeMALine(CDC * pDC,vector<float> & MA, COLORREF color)
{
	if(MA.size() == 0)
		return;

	CPen  MAPen(PS_SOLID,1,color);
	pDC->SelectObject(MAPen);

	double VHH,VLL;
	Vol_HH_LL(m_pKLineBegin,m_pKLineEnd,VHH,VLL);
	
	#define VOLMAKE_Y(d) MakeYCoord(m_RCVol,d,VHH,VLL)
	#define VOLMAKE_X(n) OffsetToX(n)

	BOOL bMAFirst	=	TRUE;
	INT nBegin		= GetBeginIndex();
	INT nEnd		= GetEndIndex();
	for (INT i = nBegin; i< nEnd; i++ )
	{
		if(MA[i] > 0.00f)
		{
			if(bMAFirst == TRUE)
			{
				pDC->MoveTo(VOLMAKE_X(i-nBegin),VOLMAKE_Y(MA[i]));
				bMAFirst = FALSE;
			}else
			{
				pDC->LineTo(VOLMAKE_X(i-nBegin),VOLMAKE_Y(MA[i]));
				if(i + 1 < nEnd ) 
					pDC->MoveTo(VOLMAKE_X(i-nBegin),VOLMAKE_Y(MA[i]));
			}
		}
	}
}



void CKLineWidget::DrawVolumeMALine(CDC * pDc)
{
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_5,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA5,COLOR_WHITE);
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_10,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA10,COLOR_YELLOW);
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_20,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA20,COLOR_VIOLET);
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_30,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA30,COLOR_TIANLAN);
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_60,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA60,COLOR_GREEN);
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_120,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA120,COLOR_GRAY);
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_250,MF_BYCOMMAND) == MF_CHECKED)
		DrawVolumeMALine(pDc,m_VolMA250,COLOR_BLUE);
}


void CKLineWidget::DrawWave(CDC * pDC)
{
	if(m_VKRWave.size() < 2)
		return;

	//画浪
	CPen  WavetPen(PS_SOLID ,1,COLOR_WHITE);
	BOOL bPoleFirst = TRUE;
	
	//1 确定画浪起点
	INT nBegin = m_pKLineBegin - &m_VKLine[ 0 ];

	INT nWaveBegin = 0;
	for (INT i=0;i<m_VKRWave.size();i++)
	{
		if(m_VKRWave[i].nOffset > nBegin)
		{
			if( i > 1) nWaveBegin= i-1;
			break;
		}
	}
	
	//2 确定画浪终点
	INT nEnd = m_VKLine.size() - (&m_VKLine[ m_VKLine.size() ] - m_pKLineEnd) ;
	
	INT nWaveEnd = m_VKRWave.size();
	for (i=nWaveBegin;i<m_VKRWave.size();i++)
	{
		if(m_VKRWave[i].nOffset > nEnd)
		{
			nWaveEnd = i - 1;
			break;
		}
	}

	BOOL bDrawNum = m_KLineMenu_Sub.GetMenuState(MENU_DRAW_WAVENUM,MF_BYCOMMAND) == MF_CHECKED;

	for(i=nWaveBegin; i < nWaveEnd; i++)
	{
		//画浪
		if(bPoleFirst == TRUE)
		{
			pDC->MoveTo(IndexToX( m_VKRWave[i].nOffset) ,PriceToY(m_VKRWave[i].fPole));
			bPoleFirst = FALSE;
		}else
		{
			pDC->SelectObject(WavetPen);
			pDC->LineTo(IndexToX(m_VKRWave[i].nOffset),PriceToY(m_VKRWave[i].fPole));

			if(bDrawNum)
			{
				CString szText;
				szText.Format("%u",i);
				pDC->SetTextColor(COLOR_WHITE);
				pDC->TextOut( IndexToX(m_VKRWave[i].nOffset) - GetTextWidth(pDC,szText),
					PriceToY(m_VKRWave[i].fPole) - (m_VKRWave[i].bMark == WAVEMARK_UP ? GetTextHeight(pDC,szText) : 0) ,szText);
			}

			if(i +1 < nEnd ) 
				pDC->MoveTo(IndexToX(m_VKRWave[i].nOffset),PriceToY(m_VKRWave[i].fPole));
		}	
	}
}


void CKLineWidget::DrawAllRC(CDC * pDc)
{
	CPen LinePen(PS_SOLID,1,COLOR_RED);
	pDc->SelectObject(LinePen);

	pDc->MoveTo(m_RCTopInfo.left,m_RCTopInfo.bottom);
	pDc->LineTo(m_RCYInfo.right,m_RCTopInfo.bottom);

 	pDc->MoveTo(m_RCYInfo.left,m_RCTopInfo.bottom);
 	pDc->LineTo(m_RCYInfo.left,m_RCYInfo.bottom);

	pDc->MoveTo(m_RCKLine.left,m_RCKLine.bottom);
 	pDc->LineTo(m_RCYInfo.left,m_RCKLine.bottom);

	pDc->MoveTo(m_RCVol.left,m_RCVol.bottom);
 	pDc->LineTo(m_RCYInfo.left,m_RCVol.bottom);

	pDc->MoveTo(m_RCKRMacd.left,m_RCKRMacd.bottom);
 	pDc->LineTo(m_RCYInfo.left,m_RCKRMacd.bottom);

	pDc->MoveTo(m_RCCommon.left,m_RCCommon.bottom);
 	pDc->LineTo(m_RCYInfo.left,m_RCCommon.bottom);

/*
	CBrush bgbs(RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE));
	CRect RCTemp   = m_RCVol;
	RCTemp.top    +=2;
	RCTemp.bottom  = RCTemp.top + m_nSTextHeight;
	pDc->FillRect(RCTemp,&bgbs);

	RCTemp = m_RCKRMacd;
	RCTemp.top    +=2;
	RCTemp.bottom  = RCTemp.top + m_nSTextHeight;
	pDc->FillRect(RCTemp,&bgbs);

	RCTemp = m_RCCommon;
	RCTemp.top    +=2;
	RCTemp.bottom  = RCTemp.top + m_nSTextHeight;
	pDc->FillRect(RCTemp,&bgbs);
*/
}



INT CKLineWidget::MakeYCoord(CRect & rc,double fValue , double fHH,double fLL)
{
	if(fHH == fLL)
		return  rc.top  + m_nSTextHeight;

	double YBase = (double)(rc.Height() - m_nSTextHeight)  / ( fHH - fLL );
	return  ( fHH - fValue ) *  YBase  + rc.top  + m_nSTextHeight;
}


void CKLineWidget::DrawVolTitle(CDC * pDc,INT nIndex)
{
	pDc->SetBkMode(TRANSPARENT);
	
	CString szText,szTemp;
	INT nTextWidth = GetTextWidth(pDc);
	
	szText.Format("成交量(%s)",FormatFuQuan(m_nFuQuan));
	pDc->SetTextColor(COLOR_GRAY);
	
	INT nYBase = 2;
	pDc->TextOut(m_RCVol.left+4,m_RCVol.top+nYBase,szText);
	
	INT nXOffset =  GetTextWidth(pDc,szText) + 4;
	float fLastVolume = (nIndex==0)? 0.00f : m_VKLine[nIndex - 1].fVolume , fZhangFu = 0.00f;
	
	if(fLastVolume == 0.00f)
	{
		CHQFunction::FormatAmount(m_VKLine[nIndex].fAmount,szTemp);
		pDc->SetTextColor(COLOR_RED);
		szText.Format(":%.0f 金额:%s ",m_VKLine[nIndex].fVolume,szTemp);
	}else
	{
		CHQFunction::FormatAmount(m_VKLine[nIndex].fAmount,szTemp);
		fZhangFu = (m_VKLine[nIndex].fVolume - fLastVolume) / fLastVolume * 100;
		pDc->SetTextColor(fZhangFu > 0.00f ? COLOR_RED : COLOR_GREEN);
		szText.Format(":%.0f 金额:%s 涨幅:%.2f%c",m_VKLine[nIndex].fVolume,szTemp,fZhangFu,'%');
	}
	pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
	

	nXOffset +=  GetTextWidth(pDc,szText) + nTextWidth;
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_5,MF_BYCOMMAND) == MF_CHECKED && m_VolMA5.size() )
	{
		szText.Format("MA5:%.2f",m_VolMA5[nIndex]);
		pDc->SetTextColor(COLOR_WHITE);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_10,MF_BYCOMMAND) == MF_CHECKED && m_VolMA10.size())
	{	
		szText.Format("MA10:%.2f",m_VolMA10[nIndex]);
		pDc->SetTextColor(COLOR_YELLOW);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	

	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_20,MF_BYCOMMAND) == MF_CHECKED && m_VolMA20.size())
	{	
		szText.Format("MA20:%.2f",m_VolMA20[nIndex]);
		pDc->SetTextColor(COLOR_VIOLET);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	

	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_30,MF_BYCOMMAND) == MF_CHECKED && m_VolMA30.size())
	{	
		szText.Format("MA30:%.2f",m_VolMA30[nIndex]);
		pDc->SetTextColor(COLOR_TIANLAN);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_60,MF_BYCOMMAND) == MF_CHECKED && m_VolMA60.size())
	{	
		szText.Format("MA60:%.2f",m_VolMA60[nIndex]);
		pDc->SetTextColor(COLOR_GREEN);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_120,MF_BYCOMMAND) == MF_CHECKED && m_VolMA120.size())
	{	
		szText.Format("MA120:%.2f",m_VolMA120[nIndex]);
		pDc->SetTextColor(COLOR_GRAY);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	
	if(m_VolMenu_MA.GetMenuState(MENU_VOL_MA_250,MF_BYCOMMAND) == MF_CHECKED && m_VolMA250.size())
	{	
		szText.Format("MA250:%.2f",m_VolMA250[nIndex]);
		pDc->SetTextColor(COLOR_BLUE);
		pDc->TextOut(nXOffset,m_RCVol.top+nYBase,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
}



void CKLineWidget::DrawVol(CDC * pDC)
{
	CBrush RedBrush(COLOR_RED);
	CBrush BuleBrush(COLOR_TIANLAN);
	CBrush WhiteBrush(COLOR_WHITE);

	CPen  RedPen(PS_SOLID,1,COLOR_RED);
	CPen  BulePen(PS_SOLID,1,COLOR_TIANLAN);
	CPen  WhitePen(PS_SOLID,1,COLOR_WHITE);

	double VHH,VLL;
	Vol_HH_LL(m_pKLineBegin,m_pKLineEnd,VHH,VLL);
	
	#define VOLMAKE_Y(d) MakeYCoord(m_RCVol,d,VHH,VLL)
	#define VOLMAKE_X(n) OffsetToX(n)

	for (KLINEDATA * pKLineData = m_pKLineBegin; pKLineData < m_pKLineEnd ; pKLineData++ )
	{
		if(m_nKLineWidth > 1){
			CRect VolRc;
			VolRc.left = VOLMAKE_X(pKLineData);
			VolRc.right = VolRc.left + m_nKLineWidth  + 1;
			VolRc.top = VOLMAKE_Y(pKLineData->fVolume);
			VolRc.bottom = m_RCVol.bottom;
			
			if(pKLineData->fClose > pKLineData->fOpen)
				pDC->FrameRect(&VolRc,&RedBrush);
			
			if(pKLineData->fClose < pKLineData->fOpen)
				pDC->FillRect(VolRc,&BuleBrush);
			
			if(pKLineData->fClose == pKLineData->fOpen)
				pDC->FillRect(VolRc,&WhiteBrush);
		}else
		{
			if(pKLineData->fClose > pKLineData->fOpen)
				pDC->SelectObject(RedPen);
			
			if(pKLineData->fClose < pKLineData->fOpen)
				pDC->SelectObject(BulePen);
			
			if(pKLineData->fClose == pKLineData->fOpen)
				pDC->SelectObject(WhitePen);
			
			pDC->MoveTo(VOLMAKE_X(pKLineData),VOLMAKE_Y(pKLineData->fVolume));
			pDC->LineTo(VOLMAKE_X(pKLineData),m_RCVol.bottom);
		}
	}

	//画Y 坐标 
	CString szText;
	pDC->SetTextColor(COLOR_RED);
	pDC->SelectObject(RedPen);
	double fBase = (VHH-VLL) / 2;
	for(INT i=1;i<=2; i++)
	{
		double f = VLL + fBase * i;
		CHQFunction::FormatVolume(f,szText);
		float Y = VOLMAKE_Y(f);
		pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,Y - GetTextHeight(pDC,szText) / 2,szText);
		pDC->MoveTo(m_RCYInfo.left+1,Y );
		pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,Y);
	}
}



void  CKLineWidget::KRMacdTextOut(CDC * pDc,int x, int y, int z,const CString & str,COLORREF crColor)
{
	if(y + m_nSTextHeight > m_RCKRMacd.bottom)
		y = m_RCKRMacd.bottom - m_nSTextHeight - 2;
	
	if( y  <= (m_RCKRMacd.top+m_nSTextHeight)  )
	{
		y = m_RCKRMacd.top+ m_nSTextHeight;
		x += m_nSTextWidth;
	}
	pDc->SetTextColor(crColor);
	pDc->TextOut(x,y,str);
}


void CKLineWidget::DrawKRMacdlTitle(CDC * pDc,INT nIndex)
{
	if(m_KRMACD.size() ==0) 
		return;

	pDc->SetBkMode(TRANSPARENT);
	CString szText;
	szText.Format("KRMacd(MA5,MA10)");
	pDc->SetTextColor(COLOR_GRAY);

	INT nYBase = 2;
	pDc->TextOut(m_RCKRMacd.left+4,m_RCKRMacd.top+nYBase,szText);

	INT nXOffset = GetTextWidth(pDc,szText) + m_RCKRMacd.left+4;
	szText.Format(":%.2f",m_KRMACD[nIndex]);
	if(nIndex == 0)
	{
		pDc->SetTextColor(COLOR_WHITE);
	}else
	{
		pDc->SetTextColor(m_KRMACD[nIndex - 1] < m_KRMACD[nIndex ] ? RGB(255,0,0):RGB(0,255,0));
	}
	pDc->TextOut(nXOffset,m_RCKRMacd.top+nYBase,szText);
}

void CKLineWidget::DrawKRMacd(CDC * pDC)
{
	if(m_KRMACD.size() <=0 )
		return;

	INT nBegin = m_pKLineBegin - &m_VKLine[0];
	INT nEnd   = m_pKLineEnd  - &m_VKLine[0];
	vector<float> KRMACD;
	KRMACD.assign(&m_KRMACD[nBegin],&m_KRMACD[nEnd]);

	float fHH , fLL;
	V_HH_LL(KRMACD,fHH,fLL);
	
	CRect OleRc = m_RCKRMacd;
	m_RCKRMacd.bottom -= 4;
	
	#define KRMACDMAKE_Y(d) MakeYCoord(m_RCKRMacd,d,fHH,fLL)
	#define KRMACDMAKE_X(n) OffsetToX(n)
	
	CPen  RedPen(PS_SOLID,1,COLOR_RED);
	CPen  BluePen(PS_SOLID,1,COLOR_TIANLAN);
	
 	INT KRMACDY0 = KRMACDMAKE_Y(0);

// 	for (INT i=0;i < OleRc.right; i++)
// 		if(i % 3 ==0) pDC->SetPixel(i,KRMACDY0,RGB(255,255,255));
		
	//画KRMACD
	for (INT i=0;i< KRMACD.size() ; i++)
	{
		if(KRMACD[i] > 0)
		{
			pDC->SelectObject(RedPen);
			pDC->MoveTo(KRMACDMAKE_X(i)+ (m_nKLineWidth / 2),KRMACDMAKE_Y(KRMACD[i]) - 2);
			pDC->LineTo(KRMACDMAKE_X(i)+ (m_nKLineWidth / 2),KRMACDY0);
		}else if(KRMACD[i] < 0)
		{
			pDC->SelectObject(BluePen);
			pDC->MoveTo(KRMACDMAKE_X(i)+ (m_nKLineWidth / 2),KRMACDMAKE_Y(KRMACD[i]) + 2);
			pDC->LineTo(KRMACDMAKE_X(i)+ (m_nKLineWidth / 2),KRMACDY0);
		}
	}


	//标记数值 
	BOOL bMark = KRMACD[i] > 0 ? TRUE : FALSE;
	INT  nMarkOffset = 0;
	CString szText;
	for (i=0;i< KRMACD.size() ; i++)
	{
		if(KRMACD[i] > 0)
		{
			if(bMark == FALSE)
			{
				if(i - nMarkOffset >= 5)
				{
					float fSum=0.00f;
					INT   nLLOffset = 0;
					FLLIndex(&KRMACD[nMarkOffset],&KRMACD[i - 1],nLLOffset,fSum);
					szText.Format("%.0f",fabs(fSum * 1000));
					nLLOffset+= nMarkOffset;
					KRMacdTextOut(pDC,KRMACDMAKE_X(nLLOffset),KRMACDMAKE_Y(KRMACD[nLLOffset]) + 2,KRMACDY0,szText,COLOR_TIANLAN);
				}
				nMarkOffset = i;
			}
			bMark = TRUE;
		}else if(KRMACD[i] < 0)
		{
			if(bMark == TRUE)
			{
				if(i - nMarkOffset >= 5)
				{
					float fSum=0.00f;
					INT   nHOffset = 0;
					FHHIndex(&KRMACD[nMarkOffset],&KRMACD[i - 1],nHOffset,fSum);
					szText.Format("%.0f",fSum * 1000);
					nHOffset+= nMarkOffset;
					KRMacdTextOut(pDC,KRMACDMAKE_X(nHOffset),KRMACDMAKE_Y(KRMACD[nHOffset]) - GetTextHeight(pDC) - 2,KRMACDY0,szText,COLOR_RED);
				}
				nMarkOffset = i;
			}
			bMark = FALSE;
		}
	}
	m_RCKRMacd = OleRc;


	//画0轴
	szText="0.00";
	pDC->SetTextColor(COLOR_RED);
	pDC->SelectObject(RedPen);

	INT YOffset = KRMACDMAKE_Y(0.00f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);

	float f = fHH / 2;
	szText.Format("%.2f",f);
	YOffset = KRMACDMAKE_Y(f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);
	
	f = fHH;
	szText.Format("%.2f",f);
	YOffset = KRMACDMAKE_Y(f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);

	f = fLL / 2;
	pDC->SetTextColor(COLOR_TIANLAN);
	szText.Format("%.2f",fabs(f));
	YOffset = KRMACDMAKE_Y(f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);	
}

void  CKLineWidget::MacdTextOut(CDC * pDc,int x, int y, int z,const CString & str,COLORREF crColor)
{
	
	if(y + m_nSTextHeight > m_RCCommon.bottom)
		y = m_RCCommon.bottom - m_nSTextHeight - 2;
	
	if( y  <= (m_RCCommon.top+m_nSTextHeight)  )
	{
		y = m_RCCommon.top+ m_nSTextHeight;
		x += m_nSTextWidth;
	}
	
	pDc->SetTextColor(crColor);
	pDc->TextOut(x,y,str);
}



void CKLineWidget::DrawMacdTitle(CDC * pDc,INT nIndex)
{
	if(m_MACD.size() == 0)
		return;

	pDc->SetBkMode(TRANSPARENT);
	
	CString szText;
	szText.Format("MACD(12,26,9)");
	pDc->SetTextColor(COLOR_GRAY);

	INT nYBase = 2;
	pDc->TextOut(m_RCCommon.left+4,m_RCCommon.top+nYBase,szText);
	INT nXOffset = GetTextWidth(pDc,szText) + m_RCCommon.left+4;

	szText.Format("MACD:%.2f",m_MACD[nIndex]);
	if(nIndex == 0)
	{
		pDc->SetTextColor(COLOR_WHITE);
	}else
	{
		pDc->SetTextColor(m_MACD[nIndex - 1] < m_MACD[nIndex ] ? COLOR_RED:COLOR_GREEN);
	}
	pDc->TextOut(nXOffset,m_RCCommon.top+nYBase,szText);

	INT nTextWidth = GetTextWidth(pDc);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	szText.Format("DEA:%.2f",m_DEA[nIndex]);
	if(nIndex == 0)
	{
		pDc->SetTextColor(COLOR_WHITE);
	}else
	{
		pDc->SetTextColor(m_DEA[nIndex - 1] < m_DEA[nIndex ] ? COLOR_RED:COLOR_GREEN);
	}
	pDc->TextOut(nXOffset,m_RCCommon.top+nYBase,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	szText.Format("DIFF:%.2f",m_DIFF[nIndex]);
	if(nIndex == 0)
	{
		pDc->SetTextColor(COLOR_WHITE);
	}else{
		pDc->SetTextColor(m_DIFF[nIndex - 1] < m_DIFF[nIndex ] ? COLOR_RED:COLOR_GREEN);
	}
	pDc->TextOut(nXOffset,m_RCCommon.top+nYBase,szText);
}


/*
DIF:EMA(CLOSE,SHORT)-EMA(CLOSE,LONG);
DEA:EMA(DIF,MID);
MACD:(DIF-DEA)*2,COLORSTICK;
*/

void CKLineWidget::DrawMacd(CDC * pDC)
{
	if(m_MACD.size() <=0 )
		return;

	INT nBegin = m_pKLineBegin - &m_VKLine[0];
	INT nEnd   = m_pKLineEnd  - &m_VKLine[0];

	vector<float> DIFF;
	DIFF.assign(&m_DIFF[nBegin],&m_DIFF[nEnd]);

	vector<float> MACD;
	MACD.assign(&m_MACD[nBegin],&m_MACD[nEnd]);

	vector<float> DEA;
	DEA.assign(&m_DEA[nBegin],&m_DEA[nEnd]);

	float fDIFF_HH , fDIFF_LL;
	V_HH_LL(DIFF,fDIFF_HH,fDIFF_LL);

	float fMACD_HH , fMACD_LL;
	V_HH_LL(MACD,fMACD_HH,fMACD_LL);

	float fHH = max(fDIFF_HH,fMACD_HH);
	float fLL = min(fDIFF_LL,fMACD_LL);

	CRect OleRc = m_RCCommon;
	m_RCCommon.bottom -= 4;

	#define MACDMAKE_Y(d) MakeYCoord(m_RCCommon,d,fHH,fLL)
	#define MACDMAKE_X(n) OffsetToX(n)

	CPen  RedPen(PS_SOLID,1,COLOR_RED);
	CPen  BluePen(PS_SOLID,1,COLOR_TIANLAN);

// 	//画0轴
 	INT MACDY0 = MACDMAKE_Y(0);
// 	for ( INT i=0;i < OleRc.right; i++)
// 		if(i % 3 ==0) pDC->SetPixel(i,MACDY0,RGB(255,255,255));

	//画MACD
	for (INT i=0;i< MACD.size() ; i++)
	{
		if(MACD[i] > 0)
		{
			pDC->SelectObject(RedPen);
			pDC->MoveTo(MACDMAKE_X(i)+ (m_nKLineWidth / 2),MACDMAKE_Y(MACD[i]) - 2 );
			pDC->LineTo(MACDMAKE_X(i)+ (m_nKLineWidth / 2),MACDY0);
		}else if(MACD[i] < 0)
		{
			pDC->SelectObject(BluePen);
			pDC->MoveTo(MACDMAKE_X(i)+ (m_nKLineWidth / 2),MACDMAKE_Y(MACD[i]) + 2 );
			pDC->LineTo(MACDMAKE_X(i)+ (m_nKLineWidth / 2),MACDY0);
		}
	}


	//画DIFF
	CPen  WhitePen(PS_SOLID,1,COLOR_WHITE);
	pDC->SelectObject(WhitePen);
	BOOL bFirstDrawLine = TRUE;
	for (i=0 ;i< DIFF.size() ; i++)
	{
		if(bFirstDrawLine == TRUE)
		{
			pDC->MoveTo(MACDMAKE_X(i),MACDMAKE_Y(DIFF[i]));
			bFirstDrawLine = FALSE;
		}else
		{	
			pDC->LineTo(MACDMAKE_X(i)+(m_nKLineWidth / 2),MACDMAKE_Y(DIFF[i]));
			if(i+ 1 < DIFF.size())
				pDC->MoveTo(MACDMAKE_X(i)+(m_nKLineWidth / 2),MACDMAKE_Y(DIFF[i]));
		}
	}


	//画DEA
	CPen  YellowPen(PS_SOLID,1,COLOR_YELLOW);
	pDC->SelectObject(YellowPen);
	
	bFirstDrawLine = TRUE;
	for (i=0 ;i< DEA.size() ; i++)
	{
		if(bFirstDrawLine == TRUE)
		{
			pDC->MoveTo(MACDMAKE_X(i),MACDMAKE_Y(DEA[i]));
			bFirstDrawLine = FALSE;
		}else
		{
			pDC->LineTo(MACDMAKE_X(i) + (m_nKLineWidth / 2),MACDMAKE_Y(DEA[i]));
			if(i+ 1 < DEA.size())
				pDC->MoveTo(MACDMAKE_X(i) + (m_nKLineWidth / 2),MACDMAKE_Y(DEA[i]));
		}
	}
	
	//标记数值 
	BOOL bMark = MACD[i] > 0 ? TRUE : FALSE;
	INT  nMarkOffset = 0;
	CString szText;
	for (i=0;i< MACD.size() ; i++)
	{
		if(MACD[i] > 0)
		{
			if(bMark == FALSE)
			{
				if(i - nMarkOffset >= 5)
				{
					float fSum=0.00f;
					INT   nLLOffset = 0;
					FLLIndex(&MACD[nMarkOffset],&MACD[i - 1],nLLOffset,fSum);
					szText.Format("%.0f",fabs(fSum * 1000));
					nLLOffset+= nMarkOffset;
					MacdTextOut(pDC,MACDMAKE_X(nLLOffset),MACDMAKE_Y(MACD[nLLOffset]) + 2,MACDY0,szText,COLOR_TIANLAN);

					if(i - nMarkOffset >= 13)
						MacdTextOut(pDC,MACDMAKE_X(i-1) - m_nSTextWidth / 2 ,MACDMAKE_Y(DIFF[i-1]) + 2,MACDY0,"↑",COLOR_RED);
				}
				nMarkOffset = i;
			}
			bMark = TRUE;
		}else if(MACD[i] < 0)
		{
			if(bMark == TRUE)
			{
				if(i - nMarkOffset >= 5)
				{
					float fSum=0.00f;
					INT   nHOffset = 0;
					FHHIndex(&MACD[nMarkOffset],&MACD[i - 1],nHOffset,fSum);
					szText.Format("%.0f",fSum * 1000);
					nHOffset+= nMarkOffset;
					MacdTextOut(pDC,MACDMAKE_X(nHOffset),MACDMAKE_Y(MACD[nHOffset]) - GetTextHeight(pDC) - 2,MACDY0,szText,COLOR_RED);

					if(i - nMarkOffset >= 13)
						MacdTextOut(pDC,MACDMAKE_X(i-1)  ,MACDMAKE_Y(DEA[i-1]) - GetTextHeight(pDC) - 2,MACDY0,"↓",COLOR_GREEN);
				}
				nMarkOffset = i;
			}
			bMark = FALSE;
		}
	}


	if(nMarkOffset < MACD.size() - 1)
	{
		if(bMark == FALSE)
		{
			if( MACD.size() - nMarkOffset >= 5)
			{
				float fSum=0.00f;
				INT   nLLOffset = 0;
				FLLIndex(&MACD[nMarkOffset],&MACD[i - 1],nLLOffset,fSum);
				szText.Format("%.0f",fabs(fSum * 1000));
				nLLOffset+= nMarkOffset;
				MacdTextOut(pDC,MACDMAKE_X(nLLOffset),MACDMAKE_Y(MACD[nLLOffset]) + 2,MACDY0,szText,COLOR_TIANLAN);
			}
		}else
		{
			if(MACD.size() - nMarkOffset >= 5)
			{
				float fSum=0.00f;
				INT   nHOffset = 0;
				FHHIndex(&MACD[nMarkOffset],&MACD[i - 1],nHOffset,fSum);
				szText.Format("%.0f",fSum * 1000);
				nHOffset+= nMarkOffset;
				MacdTextOut(pDC,MACDMAKE_X(nHOffset),MACDMAKE_Y(MACD[nHOffset]) - GetTextHeight(pDC) - 2,MACDY0,szText,COLOR_RED);
			}
		}
	}



	//画0轴
	szText="0.00";
	pDC->SetTextColor(COLOR_RED);
	pDC->SelectObject(RedPen);
	
	INT YOffset = MACDMAKE_Y(0.00f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);
	
	float f = fHH / 2;
	szText.Format("%.2f",f);
	YOffset = MACDMAKE_Y(f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);
	
	f = fHH;
	szText.Format("%.2f",f);
	YOffset = MACDMAKE_Y(f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);
	
	f = fLL / 2;
	pDC->SetTextColor(COLOR_TIANLAN);
	szText.Format("%.2f",fabs(f));
	YOffset = MACDMAKE_Y(f);
	pDC->TextOut(m_RCYInfo.left+m_nSTextWidth,YOffset - GetTextHeight(pDC,szText) / 2 ,szText);
	pDC->MoveTo(m_RCYInfo.left+1,YOffset );
	pDC->LineTo(m_RCYInfo.left + m_nSTextWidth - 1,YOffset);
	
	m_RCCommon = OleRc;
}





void CKLineWidget::DrawTitle(CDC * pDc,INT nIndex)
{
	pDc->SetBkMode(TRANSPARENT);
	
	INT nXOffset   = m_RCTopInfo.left + 4;
	INT nYOffset   = m_RCTopInfo.top  + 2;

	CString szText,szTemp;
	szText.Format("%s %s (%s %s) ",m_szLabel,KRSDE.Get_TickerSymbolByName(m_szLabel),FormatPeriod(m_dwPeriod),FormatFuQuan(m_nFuQuan));
	pDc->SetTextColor(COLOR_GRAY);
	pDc->TextOut(nXOffset,nYOffset,szText);

	INT nTextWidth = GetTextWidth(pDc);
	nXOffset   += GetTextWidth(pDc,szText);
 
	szText="[";
	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_5,MF_BYCOMMAND) == MF_CHECKED && m_MA5.size())
	{
		m_HQFunction.PriceText(m_MA5[nIndex],szTemp,"-");
		szText="MA5:"; szText+=szTemp;
		pDc->SetTextColor(COLOR_WHITE);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}


	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_10,MF_BYCOMMAND) == MF_CHECKED && m_MA10.size() )
	{	
		m_HQFunction.PriceText(m_MA10[nIndex],szTemp,"-");
		szText="MA10:"; szText+=szTemp;

		pDc->SetTextColor(COLOR_YELLOW);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	
	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_20,MF_BYCOMMAND) == MF_CHECKED && m_MA20.size())
	{	
		m_HQFunction.PriceText(m_MA20[nIndex],szTemp,"-");
		szText="MA20:"; szText+=szTemp;

		pDc->SetTextColor(COLOR_VIOLET);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_30,MF_BYCOMMAND) == MF_CHECKED && m_MA30.size())
	{	
		m_HQFunction.PriceText(m_MA30[nIndex],szTemp,"-");
		szText="MA30:"; szText+=szTemp;

		pDc->SetTextColor(COLOR_SHENLAN);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_60,MF_BYCOMMAND) == MF_CHECKED && m_MA60.size())
	{	
		m_HQFunction.PriceText(m_MA60[nIndex],szTemp,"-");
		szText="MA60:"; szText+=szTemp;
		pDc->SetTextColor(COLOR_GREEN);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}
	
	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_120,MF_BYCOMMAND) == MF_CHECKED && m_MA120.size())
	{	
		m_HQFunction.PriceText(m_MA120[nIndex],szTemp,"-");
		szText="MA120:"; szText+=szTemp;

		pDc->SetTextColor(COLOR_GRAY);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}

	if(m_KLineMenu_MA.GetMenuState(MENU_KLINE_MA_250,MF_BYCOMMAND) == MF_CHECKED && m_MA250.size())
	{	
		m_HQFunction.PriceText(m_MA250[nIndex],szTemp,"-");
		szText="MA250:"; szText+=szTemp;

		pDc->SetTextColor(COLOR_BLUE);
		pDc->TextOut(nXOffset,nYOffset,szText);
		nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	}

	szText="]   [";
	pDc->SetTextColor(COLOR_WHITE);
	nXOffset -= GetTextWidth(pDc);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	FormatTime(m_VKLine[nIndex].tUnix,szText);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	
	float fLastPrice = (nIndex==0)? 0.00f : m_VKLine[nIndex - 1].fClose;

	m_HQFunction.PriceText(m_VKLine[nIndex].fOpen,szTemp,"-");
	szText="开:"; szText+= szTemp;
	pDc->SetTextColor(::GetColor(fLastPrice,m_VKLine[nIndex].fOpen));
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	m_HQFunction.PriceText(m_VKLine[nIndex].fClose,szTemp,"-");
	szText="收:"; szText+= szTemp;
	pDc->SetTextColor(::GetColor(fLastPrice,m_VKLine[nIndex].fClose));
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	m_HQFunction.PriceText(m_VKLine[nIndex].fHigh,szTemp,"-");
	szText="高:"; szText+= szTemp;
	pDc->SetTextColor(::GetColor(fLastPrice,m_VKLine[nIndex].fHigh));
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	m_HQFunction.PriceText(m_VKLine[nIndex].fLow,szTemp,"-");
	szText="低:"; szText+= szTemp;
	pDc->SetTextColor(::GetColor(fLastPrice,m_VKLine[nIndex].fLow));
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	CHQFunction::FormatVolume(m_VKLine[nIndex].fVolume,szTemp,"-");
	szText="量:"; szText+= szTemp;
	pDc->SetTextColor(COLOR_SHENLAN);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	CHQFunction::FormatAmount(m_VKLine[nIndex].fAmount,szTemp,"-");
	szText="额:"; szText+= szTemp;
	pDc->SetTextColor(COLOR_SHENLAN);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	float fZhangFu = m_HQFunction.ZhangFuText(fLastPrice,m_VKLine[nIndex].fClose,szTemp,"-");
	szText.Format("涨:%s %.2f",szTemp,fZhangFu);
	pDc->SetTextColor(::GetColor(fZhangFu));
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	float fZhengFu = m_HQFunction.ZhengFuText(fLastPrice,m_VKLine[nIndex].fHigh , m_VKLine[nIndex].fLow,szTemp,"-");
	szText.Format("振:%s %.2f",szTemp,fZhengFu);
	pDc->SetTextColor(COLOR_SHENLAN);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;
	
	m_HQFunction.HuanShouText(m_VKLine[nIndex].fVolume,szTemp,"-");
	szText="换手:"; szText+= szTemp;
	pDc->SetTextColor(COLOR_SHENLAN);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText) + nTextWidth;

	szText="]";
	pDc->SetTextColor(COLOR_WHITE);
	nXOffset -= GetTextWidth(pDc);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	nYOffset  = GetTextHeight(pDc) + 6;
	nXOffset  = m_RCTopInfo.top + 2;

	m_HQFunction.ZuiXinJieJin(szTemp,"-");
	szText ="[最新解禁:";
	szText+= szTemp;
	szText+="]";
	pDc->SetTextColor(COLOR_RED);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);
	
	m_HQFunction.GuDongShuText(szTemp,"-");
	szText =" [股东数:";
	szText+= szTemp;
	szText+=" 人均市值:";
	m_HQFunction.RenJunChiGuSZText(szTemp,"-");
	szText+= szTemp;

	szText+=" 散户人均市值:";
	m_HQFunction.SHRenJunChiGuSZText(szTemp,"-");
	szText+= szTemp;

	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	float fGDJSQB=  m_HQFunction.GDJSQBHText(szTemp,"-");
	szText= " 较上期变化:";
	szText+= szTemp;
	pDc->SetTextColor(::GetColor(fGDJSQB));
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	m_HQFunction.QSDLTGDZBText(szTemp,"-");
	szText=" 前十大流通股东占比:";
	szText+= szTemp;
	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	m_HQFunction.LiuTongZhiText(szTemp,"-");
	szText=" 流通市值:";
	szText+= szTemp;
	szText+= "] ";
	pDc->SetTextColor(COLOR_SHENLAN);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	m_HQFunction.SuoShuHangYeText(szTemp,"-");
	szText =" [行业:";
	szText+= szTemp;
	szText+="]";
	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);
	

	m_HQFunction.ZhuYingYeWuText(szTemp,"-");
	szText =" [主营:";
	szText+= szTemp;
	szText+="]";
	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);

	m_HQFunction.SheJiGaiNainText(szTemp,"-");
	szText =" [概念:";
	szText+= szTemp;
	szText+="]";
	pDc->SetTextColor(COLOR_WHITE);
	pDc->TextOut(nXOffset,nYOffset,szText);
	nXOffset += GetTextWidth(pDc,szText);
}	




void CKLineWidget::DrawYCoord(CDC * pDc)
{
	CString szText;
	CPen  RedPen(PS_SOLID,1,COLOR_RED);
	pDc->SelectObject(RedPen);
	pDc->SetTextColor(COLOR_RED);

	switch(m_dwPeriod)
	{
	case KLINE_PERIOD_F1:
	case KLINE_PERIOD_F5:
	case KLINE_PERIOD_F15:
	case KLINE_PERIOD_F30:
		{
			for(INT i=GetBeginIndex(); i< GetEndIndex() -1 ; i++)
			{
				CTime t1(m_VKLine[i].tUnix);
				CTime t2(m_VKLine[i+1].tUnix);
				if(t1.GetDay() != t2.GetDay())
				{
					INT nXBase = IndexToX(i);
					for (INT s=m_RCKLine.top ;s < m_RCCommon.bottom; s++)
						if(s % 4 ==0) pDc->SetPixel(nXBase,s,COLOR_RED);
					
					if(m_dwPeriod == KLINE_PERIOD_F30 )
						szText.Format("%u",t1.GetDay());
					else
						szText.Format("%u-%u",t1.GetMonth(),t1.GetDay());

					pDc->TextOut(nXBase+2, m_RCCommon.bottom+2 ,szText);
					pDc->MoveTo(nXBase,m_RCCommon.bottom);
					pDc->LineTo(nXBase,m_RC.bottom);
				}
			}
		}break;
	case KLINE_PERIOD_F60:
		{
			char * pWeek[]={"","日","一","二","三","四","五","六"};
			for(INT i=GetBeginIndex(); i< GetEndIndex() -1 ; i++)
			{
				CTime t1(m_VKLine[i].tUnix);
				CTime t2(m_VKLine[i+1].tUnix);
				CTimeSpan  t = t2 - t1;
				if(t.GetDays() >= 2 )
				{
					INT nXBase = IndexToX(i);
					for (INT s=m_RCKLine.top ;s < m_RCCommon.bottom; s++)
						if(s % 4 ==0) pDc->SetPixel(nXBase,s,COLOR_RED);
						
					pDc->TextOut(nXBase+2, m_RCCommon.bottom+2 ,pWeek[t2.GetDayOfWeek()]);
					pDc->MoveTo(nXBase,m_RCCommon.bottom);
					pDc->LineTo(nXBase,m_RC.bottom);
				}
			}	
		}break;

	case KLINE_PERIOD_DAY:
		{
			for(INT i=GetBeginIndex(); i< GetEndIndex() -1 ; i++)
			{
				CTime t1(m_VKLine[i].tUnix);
				CTime t2(m_VKLine[i+1].tUnix);
				if(t2.GetMonth() != t1.GetMonth())
				{
					INT nXBase = IndexToX(i);
					
					for (INT s=m_RCKLine.top ;s < m_RCCommon.bottom; s++)
						if(s % 4 ==0) pDc->SetPixel(nXBase,s,COLOR_RED);
						
						szText.Format("%u",t1.GetMonth());
						pDc->TextOut(nXBase+2, m_RCCommon.bottom+2,szText);
						pDc->MoveTo(nXBase,m_RCCommon.bottom);
						pDc->LineTo(nXBase,m_RC.bottom);
				}
			}
		}break;

	case KLINE_PERIOD_WEEK:
		{
			for(INT i=GetBeginIndex(); i< GetEndIndex() -1 ; i++)
			{
				CTime t1(m_VKLine[i].tUnix);
				CTime t2(m_VKLine[i+1].tUnix);
				if(t2.GetYear() != t1.GetYear())
				{
					INT nXBase = IndexToX(i);
					
					for (INT s=m_RCKLine.top ;s < m_RCCommon.bottom; s++)
						if(s % 4 ==0) pDc->SetPixel(nXBase,s,COLOR_RED);
						
						szText.Format("%u",t1.GetYear());
						pDc->TextOut(nXBase+2, m_RCCommon.bottom+2,szText);
						pDc->MoveTo(nXBase,m_RCCommon.bottom);
						pDc->LineTo(nXBase,m_RC.bottom);
				}
			}
		}break;
	//不画分割线
	case KLINE_PERIOD_MONTH:
	case KLINE_PERIOD_QUARTER:
	case KLINE_PERIOD_YEAR:
		{
			CTime t1 = m_pKLineBegin->tUnix;
			CTime t2  = (m_pKLineEnd-1)->tUnix;
			szText.Format("%u",t1.GetYear());
			pDc->TextOut(OffsetToX(m_pKLineBegin) + 4 , m_RCCommon.bottom+2,szText);
			pDc->MoveTo(OffsetToX(m_pKLineBegin) +2   ,m_RCCommon.bottom);
			pDc->LineTo(OffsetToX(m_pKLineBegin) +2  ,m_RC.bottom);

			szText.Format("%u",t2.GetYear());
			pDc->TextOut(OffsetToX(m_pKLineEnd) + 4 , m_RCCommon.bottom+2,szText);
			pDc->MoveTo(OffsetToX(m_pKLineEnd) +2   ,m_RCCommon.bottom);
			pDc->LineTo(OffsetToX(m_pKLineEnd) +2   ,m_RC.bottom);

		}break;
	}

	switch(m_dwPeriod)
	{
	case KLINE_PERIOD_F1:
		{
			for(INT i=GetBeginIndex(); i< GetEndIndex(); i++)
			{
				CTime t(m_VKLine[i].tUnix);
				if((t.GetHour() == 13 && t.GetMinute() == 00) || 
					(t.GetHour() == 10 && t.GetMinute() == 30) || 
					(t.GetHour() == 14 && t.GetMinute() == 30))
				{
					szText.Format("%02u:%02u",t.GetHour(),t.GetMinute());
					pDc->TextOut(IndexToX(i)+ 4, m_RCCommon.bottom +2 ,szText);
					pDc->MoveTo(IndexToX(i)+ 2,m_RCCommon.bottom);
					pDc->LineTo(IndexToX(i)+ 2,m_RC.bottom);
				}
			}
		}break;
	case KLINE_PERIOD_F5:
		{
			for(INT i=GetBeginIndex(); i< GetEndIndex(); i++)
			{
				CTime t(m_VKLine[i].tUnix);
				if((t.GetHour() == 13 && t.GetMinute() == 05))
				{
					szText.Format("%02u:%02u",t.GetHour(),t.GetMinute());
					pDc->TextOut(IndexToX(i)+ 4, m_RCCommon.bottom +2 ,szText);
					pDc->MoveTo(IndexToX(i)+ 2,m_RCCommon.bottom);
					pDc->LineTo(IndexToX(i)+ 2,m_RC.bottom);
				}
			}
		}break;
	}
}


/*
|--------------------------------------|--|
|              顶部信息栏              |Y |
|--------------------------------------|坐|
|                                      |标|
|                                      |信|
|              K线绘制区               |息|
|                                      |栏|
|                                      |  |
|--------------------------------------|  |
|             指标窗口1                |  |
|                                      |  |
|--------------------------------------|  |
|             指标窗口2                |  |
|                                      |  |
|--------------------------------------|  |
|             指标窗口3                |  |
|                                      |  |
|--------------------------------------|--|
|        X坐标信息栏                      |
|-----------------------------------------|
*/

void CKLineWidget::AdjustRC(CRect & rc)
{
	if(rc.Height() == 0 || rc.Width() == 0)
		return;

	m_RCYInfo.left      = rc.right - m_nSTextWidth * 8;
	m_RCYInfo.right     = rc.right;
	m_RCYInfo.top       = rc.top;
	m_RCYInfo.bottom    = rc.bottom;
	
	m_RCTopInfo.right	= m_RCYInfo.left;
	m_RCTopInfo.left	= rc.left;
	m_RCTopInfo.top		= rc.top;
	m_RCTopInfo.bottom	= m_nSTextHeight * 2 + 8;

	CRect RowRc;
	RowRc.top    = m_RCTopInfo.bottom;
	RowRc.bottom = rc.bottom-20;
	RowRc.left   = rc.left;
	RowRc.right  = m_RCYInfo.left - MAX_LEFT_FILL;
	m_SplitterRowWidget.SetRect(RowRc);

	static BOOL bFirst = TRUE;
	if(bFirst){

		m_SplitterRowWidget.BindTopRect(&m_RCKLine);
		m_SplitterRowWidget.BindBottomRow(&m_SplitterRowSubWidget);
		m_SplitterRowWidget.SetLine(RowRc.top + 300 , SRW_FixedMode_Bottom);
		

		CRect RCRowWidget = m_SplitterRowWidget.GetBottomRect();
		m_SplitterRowSubWidget.BindTopRect(&m_RCVol);
		m_SplitterRowSubWidget.BindBottomRow(&m_SplitterRowSubSubWidget);
		m_SplitterRowSubWidget.SetRect(RCRowWidget);
		m_SplitterRowSubWidget.SetLine(RCRowWidget.top+100 , SRW_FixedMode_Bottom);

		CRect SubRCRowWidget = m_SplitterRowSubWidget.GetBottomRect();
		m_SplitterRowSubSubWidget.BindTopRect(&m_RCKRMacd);
		m_SplitterRowSubSubWidget.BindBottomRect(&m_RCCommon);
		m_SplitterRowSubSubWidget.SetRect(SubRCRowWidget);
		m_SplitterRowSubSubWidget.SetLine(SubRCRowWidget.top+100 , SRW_FixedMode_Bottom);
		bFirst = FALSE;
	}
	
	m_RC = rc;
}

void CKLineWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{
		CDC * pDc = MemDC.GetMemoryDC();

		//填充背景色
		CBrush bgbs(RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE));

		pDc->FillRect(rc,&bgbs);
		pDc->SelectObject(m_Font);

		pDc->SetTextColor(RGB(KLWIDGET_TEXTCOLOR_RED,KLWIDGET_TEXTCOLOR_GREEN,KLWIDGET_TEXTCOLOR_BLUE));
		pDc->SetBkMode(TRANSPARENT);
		
		DrawAllRC(pDc);
		if(m_pKLineBegin == NULL || m_pKLineEnd == NULL)
			return;

		DrawYCoord(pDc);
		DrawTitle(pDc,m_nCrossIndex);
		DrawKLine(pDc);
		DrawKLineMakeHistory(pDc);

		if(m_KLineMenu_Sub.GetMenuState(MENU_DRAW_GAP,MF_BYCOMMAND) == MF_CHECKED)
			DrawGap(pDc);

		if(m_KLineMenu_Sub.GetMenuState(MENU_DRAW_WAVE,MF_BYCOMMAND) == MF_CHECKED)
			DrawWave(pDc);

		DrawMacd(pDc);
		DrawMacdTitle(pDc,m_nCrossIndex);
		DrawKRMacd(pDc);
		DrawKRMacdlTitle(pDc,m_nCrossIndex);
		DrawVol(pDc);
		DrawVolTitle(pDc,m_nCrossIndex);
		DrawVolumeMALine(pDc);
		DrawMALine(pDc);
	}
	// Do not call CWnd::OnPaint() for painting messages
}


BOOL CKLineWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}


void CKLineWidget::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	rc.left = 0;
	rc.top  = 0;
	rc.right = cx;
	rc.bottom = cy;
	AdjustRC(rc);
	if(m_pKLineBegin  &&  m_pKLineEnd)
		InitKLine();
	
	RedrawWindow();
}




void CKLineWidget::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_VKLine.size() == 0)
		return;

	SetFocus();
	m_bShowCross=TRUE;
	OnMouseMove(nFlags,point);
}


void CKLineWidget::OnRButtonDown( UINT, CPoint point )
{
	if( m_VKLine.size() == 0)
		return;

	if(m_RCKLine.PtInRect(point))
	{
		ClientToScreen(&point);
		CMenu * pSubMenu = m_KLineMenu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN| TPM_LEFTBUTTON| TPM_RIGHTBUTTON,point.x,point.y,this); 
	}

	if(m_RCVol.PtInRect(point))
	{
		ClientToScreen(&point);
		CMenu * pSubMenu = m_VolMenu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN| TPM_LEFTBUTTON| TPM_RIGHTBUTTON,point.x,point.y,this); 
	}
}



void CKLineWidget::DrawCross(CPoint & point)
{
	if(!m_bShowCross) return;
	if(point.y < m_RCKLine.top || point.x > m_RCKLine.right)
		return;
	
	
	//需要对X取K线中间 
	INT nIndex = XToIndex(point.x);
	if(nIndex >=  m_VKLine.size() )
		m_nCrossIndex = m_VKLine.size() -1;
	else
		m_nCrossIndex = nIndex;
	
	RedrawWindow();
	
	CDC * pDC = GetDC();
	CPen  WhitePen(PS_SOLID,1,COLOR_WHITE);
	pDC->SelectObject(WhitePen); 
	pDC->SelectObject(m_Font);
	
	pDC->MoveTo(m_RCKLine.left, point.y);
	pDC->LineTo(m_RCYInfo.left,point.y);
	
	pDC->MoveTo(IndexToX(m_nCrossIndex),m_RCKLine.top);
	pDC->LineTo(IndexToX(m_nCrossIndex),m_RCCommon.bottom);
	
	CString szText;
	CTime t = m_VKLine[m_nCrossIndex].tUnix;
	szText.Format("%02u-%02u %02u:%02u",t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
	pDC->TextOut(IndexToX(m_nCrossIndex),m_RCCommon.bottom + 2,szText);
	
	if(m_RCKLine.PtInRect(point))
	{
		float fPrice = YToPrice(point.y);
		szText.Format("%.2f",fPrice);
		pDC->TextOut(m_RCYInfo.left + m_nSTextWidth,point.y,szText);
	}else if(m_RCVol.PtInRect(point))
	{
		double VHH,VLL;
		Vol_HH_LL(m_pKLineBegin,m_pKLineEnd,VHH,VLL);
		INT Y = point.y;
		Y -= m_RCVol.top + m_nSTextHeight;
		float YBase = (m_RCVol.Height() - m_nSTextHeight)  / ( VHH - VLL );
		float fVol  = VHH - (float)Y / YBase;
		CHQFunction::FormatVolume(fVol,szText);
		pDC->TextOut(m_RCYInfo.left + m_nSTextWidth,point.y,szText);
	}
	ReleaseDC(pDC);
}

void CKLineWidget::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	DrawCross(point);
	CWnd::OnMouseMove(nFlags, point);
}


void CKLineWidget::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bShowCross = FALSE;
	m_nCrossIndex = m_VKLine.size() - 1;
	RedrawWindow();
	CWnd::OnLButtonDblClk(nFlags, point);
}



BOOL CKLineWidget::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!m_bShowCross && m_SplitterRowWidget.PreTranslateMessage(this,pMsg))
		return TRUE;


	if(pMsg->message == WM_COMMAND)
		OnMyCommand(pMsg->wParam,pMsg->lParam);

	if(pMsg -> message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			//放大和缩小 保持最新K线在右边
			INT nOleWidth = m_nKLineWidth;
			m_nKLineWidth += (pMsg->wParam == VK_DOWN ? -2 : 2);

			INT nDrawCount = GetUsableDrawLineCount();
			if( (nDrawCount > m_VKLine.size() && m_nKLineWidth < 0)  ||  m_nKLineWidth > MAX_LEFT_FILL)
			{
				m_nKLineWidth = nOleWidth; 
				return TRUE;
			}

			//普通放大和缩小
			if(m_bShowCross == FALSE)
			{
				if(nDrawCount < m_VKLine.size())
				{
					m_pKLineBegin =  &m_VKLine[ m_VKLine.size() - nDrawCount ];
					m_pKLineEnd   =  &m_VKLine[ m_VKLine.size() ];
				}else{
					m_pKLineBegin = &m_VKLine[0];
					m_pKLineEnd   = &m_VKLine[ m_VKLine.size() ];
				}
				Price_HH_LL(m_pKLineBegin,m_pKLineEnd,m_fHH,m_fLL);
				RedrawWindow();
			}else
			{   
				INT nBegin  = m_pKLineBegin - &m_VKLine[0];
				INT nCenter = (m_pKLineEnd - m_pKLineBegin)/2 + nBegin;
				INT nSubCount = abs( (m_pKLineEnd - m_pKLineBegin) - nDrawCount);
				if(m_nCrossIndex < nCenter)
				{  
					SafeAdjustBegin(-nSubCount);
					m_pKLineEnd = m_pKLineBegin + GetUsableDrawLineCount();
					SafeAdjustEnd(0);
					INT nNewCenter = nNewCenter = (m_pKLineEnd - m_pKLineBegin)/2 + m_pKLineBegin - &m_VKLine[0];
					if(nNewCenter < m_nCrossIndex){
						SafeAdjustBegin(m_nCrossIndex - nNewCenter);
						SafeAdjustEnd(m_nCrossIndex - nNewCenter);
					}
				}else if(m_nCrossIndex > nCenter)
				{   
					SafeAdjustEnd(nSubCount);
					m_pKLineBegin = m_pKLineEnd - GetUsableDrawLineCount();
					SafeAdjustBegin(0);

					INT nNewCenter = nNewCenter = (m_pKLineEnd - m_pKLineBegin)/2 + m_pKLineBegin - &m_VKLine[0];
					if(nNewCenter > m_nCrossIndex){
						SafeAdjustBegin(m_nCrossIndex - nNewCenter);
						SafeAdjustEnd(m_nCrossIndex - nNewCenter);
					}
				}else if(m_nCrossIndex == nCenter) 
				{
					nSubCount = nSubCount / 2;
					SafeAdjustBegin( pMsg->wParam == VK_UP? nSubCount:-nSubCount);
					SafeAdjustEnd( pMsg->wParam == VK_UP? -nSubCount:nSubCount);
				}
	
				Price_HH_LL(m_pKLineBegin,m_pKLineEnd,m_fHH,m_fLL);
				CPoint Point;
				Point.x = IndexToX(m_nCrossIndex);
				Point.y = PriceToY(m_VKLine[m_nCrossIndex].fClose);
				DrawCross(Point);
			}	
		}

		if(pMsg->wParam == VK_LEFT)
		{
			if(m_bShowCross && m_nCrossIndex > 0)
			{
				m_nCrossIndex--;
			}else if(m_bShowCross == FALSE)
			{
				m_nCrossIndex = m_pKLineEnd - &m_VKLine[0] - 1;
				m_bShowCross  = TRUE;
			}
			if( m_pKLineBegin - &m_VKLine[0] > m_nCrossIndex  )
			{
				SafeAdjustBegin(-1);
				SafeAdjustEnd(-1);
				Price_HH_LL(m_pKLineBegin,m_pKLineEnd,m_fHH,m_fLL);
			}
			CPoint Point;
			Point.x = IndexToX(m_nCrossIndex);
			Point.y = PriceToY(m_VKLine[m_nCrossIndex].fClose);
			DrawCross(Point);
		}
		if(pMsg->wParam == VK_RIGHT)
		{
			if(m_bShowCross && m_nCrossIndex < m_VKLine.size() - 1)
			{
				m_nCrossIndex++;
			}else if(m_bShowCross == FALSE)
			{
				m_nCrossIndex = m_pKLineEnd - &m_VKLine[0] - 1;
				m_bShowCross  = TRUE;
			}
			if( m_pKLineEnd - &m_VKLine[0] <= m_nCrossIndex  )
			{
				SafeAdjustBegin(1);
				SafeAdjustEnd(1);
				Price_HH_LL(m_pKLineBegin,m_pKLineEnd,m_fHH,m_fLL);
			}
			CPoint Point;
			Point.x = IndexToX(m_nCrossIndex);
			Point.y = PriceToY(m_VKLine[m_nCrossIndex].fClose);
			DrawCross(Point);
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}





