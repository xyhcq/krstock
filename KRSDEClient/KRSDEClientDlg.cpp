// KRSDEClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KRSDEClient.h"
#include "KRSDEClientDlg.h"
#include "HPClient/HPClient.h"
#include "HPClient/HPClientCommon.h"


#include "ComHeader/ComHeader.h"
#include "StockMgr/DirectoryMgr.h"
#include "StockMgr/StockCommon.h"
#include "StockMgr/ChuQuanDataMgr.h"
#include "StockMgr/TickerSymbolDataMgr.h"
#include "StockMgr/FinancialDataMgr.h"
#include "StockMgr/RTHQDataMgr.h"
#include "StockMgr/FenBiDataMgr.h"
#include "StockMgr/KLineDataMgr.h"
#include "StockMgr/HQBaseInfoMgr.h"
#include "StockMgr/HangYeDataMgr.h"
#include "StockMgr/ZSDataMgr.h"

#include "HPSocket/SocketDataMgr.h"
#include "StockMgr/ClientKLineDataMgr.h"
#include "Common/Common.h"
#include <strsafe.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKRSDEClientDlg dialog
BOOL DisposeFuQuan(const char * szLabel ,LPKLINEDATA  pKLData , INT nSize , INT nFuQuan);
void TransformKLine(DWORD dwPeriod ,LPKLINEDATA  pKLData , INT nSize , vector< KLINEDATA> & VKLNew);


HWND		g_hClientDlg = NULL;

namespace Globle
{
	CChuQuanDataMgr			ChuQuanDataMgr;
	CTickerSymbolDataMgr	TickerSymbolDataMgr;
	CFinancialDataMgr       FinancialDataMgr;
	CRTHQDataMgr			RTHQDataMgr;
	CFenBiDataMgr			FenBiDataMgr;
	CFileMgr				F10DirMgr;
	CClientKLineDataMgr     ClientKLineDataMgr;
	CHQBaseInfoDataMgr		HQBaseInfoDataMgr;
	CHangYeDataMgr			HangYeDataMgr;
	CZSDataMgr				ZSDataMgr;
};


#define  BKCOLOR				168,0,157


//#define	SERVER_ADDRESS			"127.0.0.1"
#define		SERVER_ADDRESS			"120.79.70.28"
enum
{
	TIMER_INITNETWORK,
	TIMER_HIDEKRPROGRESSWIDGET,
	TIMER_HIDEWINDOW
};

vector <HWND>	g_hVLoaderWnd;
static void VSendMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	for (INT i=0; i< g_hVLoaderWnd.size(); i++)
		SendMessage(::g_hVLoaderWnd[i],uMsg,wParam,lParam);
}

void WINAPI KRSDE_SetHWND(HWND hWnd)
{
	g_hVLoaderWnd.push_back(hWnd);
}



void WINAPI KRSDE_ShowWindow(KRSDE_SW krsd_sw)
{
	switch(krsd_sw)
	{
	case KRSDE_SW_SHOW:
		ShowWindow(g_hClientDlg,SW_SHOW);
		break;
	case KRSDE_SW_HIDE:
		ShowWindow(g_hClientDlg,SW_HIDE);
		break;
	case KRSDE_SW_AUTO:
		ShowWindow(g_hClientDlg,IsWindowVisible(g_hClientDlg)? SW_HIDE : SW_SHOW);
		break;
	}
}

//股票代码接口
const STOCK_TICKER_SYMBOL * WINAPI KRSDE_GetTickerSymbol(INT * lpnSize)
{
	if(lpnSize == NULL) 
		return NULL;

	*lpnSize = Globle::TickerSymbolDataMgr.GetSize();
	if(*lpnSize <=0)
		return NULL;
	return Globle::TickerSymbolDataMgr.GetTable();
}



const char * WINAPI KRSDE_TickerSymbolByName(const char * szFullLabel)
{
	if(szFullLabel == NULL) 
		return NULL;

	return Globle::TickerSymbolDataMgr.GetName(szFullLabel);
}


//财务接口
const FINANCIAL * WINAPI KRSDE_GetFinancial(const char * szFullLabel)
{
	if(szFullLabel == NULL || !IsStock(szFullLabel)) 
		return NULL;

	return (const FINANCIAL *) Globle::FinancialDataMgr.Select(szFullLabel);
}

//除权接口
const EXRIGHTS *  WINAPI KRSDE_GetExRights(const char * szFullLabel , INT * lpnSize)
{
	if(szFullLabel == NULL || lpnSize == NULL)
		return NULL;

	return (const EXRIGHTS *) Globle::ChuQuanDataMgr.Select(szFullLabel,lpnSize);
}



void WINAPI KRSDE_GetKLine_Local_Free(KLINEDATA ** pResult)
{
	delete []  *pResult;
}


BOOL  WINAPI KRSDE_GetKLine_Local(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan, KLINEDATA ** pResult , DWORD * pdwSize)
{
	if(szFullLabel == NULL) 
		return FALSE;

	vector<KLINEDATA> VKLFind;
	if(!Globle::ClientKLineDataMgr.Select(szFullLabel,dwPeriod,VKLFind))
		return FALSE;

	const RCV_REPORT_STRUCTExV3 *  pRRS = Globle::RTHQDataMgr.Select(szFullLabel);
	INT  nTTRSize        = 0;
	LPTRADINGRECORD pTRR = Globle::FenBiDataMgr.Select(szFullLabel,&nTTRSize);
	vector<KLINEDATA> VKLNew;


	//处理分笔 or 行情数据  得到完整的K线
	switch(dwPeriod)
	{
	case  KLINE_PERIOD_F1:
			FenBiToMultiKLine(pRRS,pTRR,nTTRSize,KTTableF1,sizeof(KTTableF1)/sizeof(KTIMETABLE),VKLNew);
			MergeKLine(VKLNew,VKLFind);
		break;
	case  KLINE_PERIOD_F5:
			FenBiToMultiKLine(pRRS,pTRR,nTTRSize,KTTableF5,sizeof(KTTableF5)/sizeof(KTIMETABLE),VKLNew);
			MergeKLine(VKLNew,VKLFind);
		break;
	case  KLINE_PERIOD_F15:
			FenBiToMultiKLine(pRRS,pTRR,nTTRSize,KTTableF15,sizeof(KTTableF15)/sizeof(KTIMETABLE),VKLNew);
			MergeKLine(VKLNew,VKLFind);
		break;
	case  KLINE_PERIOD_F30:
			FenBiToMultiKLine(pRRS,pTRR,nTTRSize,KTTableF30,sizeof(KTTableF30)/sizeof(KTIMETABLE),VKLNew);
			MergeKLine(VKLNew,VKLFind);
		break;
	case  KLINE_PERIOD_F60:
			FenBiToMultiKLine(pRRS,pTRR,nTTRSize,KTTableF60,sizeof(KTTableF60)/sizeof(KTIMETABLE),VKLNew);
			MergeKLine(VKLNew,VKLFind);
		break;
	case  KLINE_PERIOD_YEAR:
	case  KLINE_PERIOD_QUARTER:	
	case  KLINE_PERIOD_MONTH:
	case  KLINE_PERIOD_WEEK:
	case  KLINE_PERIOD_DAY:	 
		{
			KLINEDATA KLineData;
			KLineData.tUnix				= pRRS->m_time;
			KLineData.fOpen				= pRRS->m_fOpen;
			KLineData.fClose			= pRRS->m_fNewPrice;
			KLineData.fHigh				= pRRS->m_fHigh;
			KLineData.fLow				= pRRS->m_fLow;
			KLineData.fAmount			= pRRS->m_fAmount;
			KLineData.fVolume			= pRRS->m_fVolume;
			VKLFind[VKLFind.size() - 1] = KLineData;
		}break;
	}

	//处理复权
	DisposeFuQuan(szFullLabel,VKLFind.begin(),VKLFind.size(),nFuQuan);
	
	//转换K线
	vector<KLINEDATA> temp;
	TransformKLine(dwPeriod,VKLFind.begin(),VKLFind.size(),temp);
	
	*pResult = new KLINEDATA[temp.size()];
	if(*pResult == NULL)
		return FALSE;
	
	CopyMemory(*pResult,temp.begin(),temp.size() * sizeof(KLINEDATA));
	*pdwSize  = temp.size();

	return TRUE;
}





//取云端K线接口
BOOL WINAPI KRSDE_GetKLine_Remote(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan,DWORD dwCount , WORD wMark)
{
	if(szFullLabel == NULL) 
		return NULL;

	TOKEN_KLINE_HEADINFO TKLH;
	StringCbCopy(TKLH.szLabel,sizeof(TKLH.szLabel),szFullLabel);
	TKLH.dwPeriod = dwPeriod;
	TKLH.dwCount  = dwCount;
	TKLH.nFuQuan  = nFuQuan;
	TKLH.wMark    = wMark;

	CSocketDataMgr SDM;
	SDM.MakeSendData(TOKEN_KLINE,0,(LPBYTE)&TKLH,sizeof(TOKEN_KLINE_HEADINFO));
	if(!HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize()))
		return FALSE;

	//默认请求一份日线 用作数据分析
	if(dwPeriod != KLINE_PERIOD_DAY)
	{
		if(!Globle::ClientKLineDataMgr.Check(szFullLabel,KLINE_PERIOD_DAY))
		{
			TOKEN_KLINE_HEADINFO TKLHDay;
			StringCbCopy(TKLHDay.szLabel,sizeof(TKLHDay.szLabel),szFullLabel);
			TKLHDay.dwPeriod = KLINE_PERIOD_DAY;
			TKLHDay.dwCount  = 0;
			TKLHDay.nFuQuan  = KLINE_FUQUAN_NO;
			TKLHDay.wMark    = KLINE_MARK_NOMESSAGE; 
			SDM.MakeSendData(TOKEN_KLINE,0,(LPBYTE)&TKLHDay,sizeof(TOKEN_KLINE_HEADINFO));
			HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		}
	}
	return TRUE;
}

//取盘口接口
const REALTIMEQUOTES *  WINAPI KRSDE_GetRealTimeQuotes(const char * szFullLabel)
{
	if(szFullLabel == NULL) 
		return NULL;
	return (const REALTIMEQUOTES *)Globle::RTHQDataMgr.Select(szFullLabel);
}

DWORD  WINAPI KRSDE_GetAllRealTimeQuotes( REALTIMEQUOTES *  pRTQ )
{
	map<string , RCV_REPORT_STRUCTExV3 >  * pMap      = Globle::RTHQDataMgr.GetMapPoint();
	if(pMap == NULL || pMap->size() == 0)
		return 0;
	
	if(pRTQ == NULL)
		return pMap->size();

	REALTIMEQUOTES * pOffset = pRTQ;
	map<string , RCV_REPORT_STRUCTExV3 >::iterator it = pMap->begin();
	for (it; it != pMap->end(); ++it)
	{
		CopyMemory(pOffset , &it->second, sizeof(REALTIMEQUOTES) );
		pOffset++;
	}
	return pMap->size();
}



const LPTRADINGRECORD  WINAPI KRSDE_GetTradingRecord_Local (const char * szFullLabel,INT * pnSize)
{
	return Globle::FenBiDataMgr.Select(szFullLabel,pnSize);
}



//取云端分笔接口
BOOL  WINAPI KRSDE_GetTradingRecord_Remote (const char * szFullLabel,WORD  wYear,WORD  wMonth,WORD  wDay)
{
	if(szFullLabel == NULL) 
		return NULL;

	//本地已经存在数据 则发送本地数据
	if(wYear == 0 && wMonth == 0 && wDay==0)
	{
		INT nSize = 0;
		const LPTRADINGRECORD pTTR = Globle::FenBiDataMgr.Select(szFullLabel,&nSize);
		if(pTTR)
		{
			TOKEN_TRADINGRECORD_HEADINFO	TTRHeader;
			TTRHeader.dwCount = nSize;
			CTime t = pTTR->m_lTime;
			TTRHeader.wYear = t.GetYear();
			TTRHeader.wMonth = t.GetMonth();
			TTRHeader.wDay  = t.GetDay();
			StringCbCopy(TTRHeader.szLabel,sizeof(TTRHeader.szLabel),szFullLabel);
			::VSendMessage(WM_KRSDE_TRADINGRECORD_REMOTE,(WPARAM)&TTRHeader,(LPARAM)pTTR);
			return TRUE;
		}
	}

	TOKEN_TRADINGRECORD_HEADINFO TFBH;
	StringCbCopy(TFBH.szLabel,sizeof(TFBH.szLabel),szFullLabel);
	TFBH.wYear = wYear;
	TFBH.wMonth = wMonth;
	TFBH.wDay  = wDay;
	CSocketDataMgr SDM;
	SDM.MakeSendData(TOKEN_FENBIDATA,0,(LPBYTE)&TFBH,sizeof(TOKEN_TRADINGRECORD_HEADINFO));
	return HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
}



BOOL WINAPI KRSDE_GetF10Data_Remote(const char * szFullLabel)
{
	char	 *	pBuffer = new char[5 * 1024 * 1024];
	DWORD		dwSize	= 5 * 1024 * 1024;
	if(pBuffer == NULL)
		return FALSE;
	
	TOKEN_F10_HEADINFO TF10H;
	TF10H.dwCount			= 0;
	TF10H.dwSignature		= 0;
	StringCbCopy(TF10H.szLabel,sizeof(TF10H.szLabel),szFullLabel);


	CHAR szFileName[30]={0};
	StringCbPrintf(szFileName,sizeof(szFileName),"%s.txt",szFullLabel);

	CSocketDataMgr SDM;
	if(Globle::F10DirMgr.LoadFile(szFileName,pBuffer,dwSize))
		TF10H.dwSignature = SDM.CRC32((BYTE *)pBuffer,dwSize);

	SDM.MakeSendData(TOKEN_F10DATA,0,(LPBYTE)&TF10H,sizeof(TOKEN_F10_HEADINFO));
	return HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
}

BOOL WINAPI KRSDE_GetHQBaseInfo(const char * szFullLabel, LPHQBASE_INFO lpBaseInfo )
{
	if(szFullLabel == NULL || lpBaseInfo == NULL)
		return FALSE;

	LPHQBASE_INFO lpFind = Globle::HQBaseInfoDataMgr.Select(szFullLabel);
	if(lpFind == NULL)
		return FALSE;

	CopyMemory(lpBaseInfo,lpFind,sizeof(HQBASE_INFO));
	return TRUE;
}


float WINAPI  KRSDE_GetZhangSu(const char * szLabel , INT nMinutePeriod)
{
	return Globle::ZSDataMgr.Select(szLabel,nMinutePeriod);
}



float WINAPI KRSDE_GetHYZhangFu(const char * szHangYe)
{
	const VPRRS * VS = Globle::HangYeDataMgr.Select(szHangYe);
	if(VS  == NULL)
		return 0.00f;
	
	INT nCount  = 0;
	float fRetZhangFu = 0.00f;
	for(INT i=0; i< VS->size(); i++)
	{
		PRCV_REPORT_STRUCTExV3 pRRS = VS->at(i);
		//B 股不参与计算
		if(pRRS->m_szLabel[0] == '9')
			continue;

		//一字版不参与计算
		if( pRRS->m_fHigh == pRRS->m_fLow  && 
			pRRS->m_fOpen == pRRS->m_fNewPrice && 
			pRRS->m_fOpen > pRRS->m_fLastClose )
			continue;

		if(pRRS->m_fNewPrice == 0.00f)
			continue;
	
		fRetZhangFu += (pRRS->m_fNewPrice - pRRS->m_fLastClose) / pRRS->m_fLastClose * 100;
		nCount++;
	}

	if(nCount == 0)
		return 0.00f;
	
	return fRetZhangFu / nCount;
}


CKRSDEClientDlg::CKRSDEClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKRSDEClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKRSDEClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bConnected = FALSE;
	m_dwRequstCount  = 0;
	m_dwRequstSize   = 0;
	m_tServer = CTime::GetCurrentTime();
}


void CKRSDEClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKRSDEClientDlg)
	DDX_Control(pDX, IDC_TEXTLIST, m_TextListWidget);
	DDX_Control(pDX, IDC_PORGRESS, m_ProgressWidget);
	DDX_Control(pDX, IDC_SYSMIN, m_MinimizeBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKRSDEClientDlg, CDialog)
	//{{AFX_MSG_MAP(CKRSDEClientDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HPCLIENT_CONNECT,OnHPClient_Connect)
	ON_MESSAGE(WM_HPCLIENT_RECEIVE,OnHPClient_Receive)
	ON_MESSAGE(WM_HPCLIENT_CLOSE,OnHPClient_Close)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKRSDEClientDlg message handlers

BOOL CKRSDEClientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	g_hClientDlg = m_hWnd;

	m_MinimizeBtn.SetStyle(KRSYSBTN_MINIMIZEBOX);
	m_MinimizeBtn.SetHotColor(202,100,195);
	m_MinimizeBtn.SetBkColor(BKCOLOR);

	HWND hWnd = ::FindWindow("Shell_TrayWnd", NULL);
	if(hWnd)
	{
		RECT rcTray;
		::GetWindowRect(hWnd, &rcTray);
		CRect rcClient;
		GetClientRect(rcClient);
		::SetWindowPos(m_hWnd,HWND_TOPMOST,rcTray.right - rcClient.Width() - 1 , rcTray.top - rcClient.Height() - 1 ,0,0,SWP_NOSIZE);
	}
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);


	m_szTitle.LoadString(IDS_TITLE);
	SetWindowText(m_szTitle);

	m_ProgressWidget.SetBkColor(BKCOLOR);
	m_ProgressWidget.SetRange(0,99);
	m_ProgressWidget.SetPos(1);
	m_ProgressWidget.SetAutoPos(TRUE,200);
	
	m_TextListWidget.SetBkColor(BKCOLOR);
	m_TextListWidget.InsertString(0,"[0]正在连接服务器...");

	HPClient_SetHWND(m_hWnd);
	if(!HPClient_Create())
	{
		MessageBox("初始化网络失败\r\n请检查你的网络!","警告",MB_ICONERROR | MB_OK);
		return FALSE;
	}
	HPClient_Start(SERVER_ADDRESS,8000,TRUE);

	Globle::F10DirMgr.Init("F10Data");

	SetTimer(TIMER_INITNETWORK,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKRSDEClientDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK())
	{
		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );
		SolidBrush SBBK(Color(BKCOLOR));
		
		raphics.FillRectangle(&SBBK,0,0,rc.Width(),rc.Height());
		raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		raphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
		
		
		Font TitleFont(L"Arial", 9);
		StringFormat TitleFormat;                         
		TitleFormat.SetAlignment(StringAlignmentNear);
		TitleFormat.SetLineAlignment(StringAlignmentNear); 
		
		SolidBrush TitleBrush(Color(255, 255, 255));
		
		CTRString TRString;
		WCHAR * pszTitle = TRString.ANSIToUnicode(m_szTitle);

		raphics.DrawString(pszTitle,wcslen(pszTitle),
			&TitleFont,
			RectF(20,4,(float)rc.Width(),(float)rc.Height()),
			&TitleFormat,
			&TitleBrush);
		
		Pen penLine(Color(100,255,255,255),1);
		raphics.DrawLine(&penLine,Point(0,22),Point(rc.right,22));
		
		if(m_ProgressWidget.IsWindowVisible())
			return;
		
		CRect DrawRc;
		m_ProgressWidget.GetClientRect(DrawRc);
		
		{
			StringFormat FFormat;                         
			FFormat.SetAlignment(StringAlignmentCenter);
			FFormat.SetLineAlignment(StringAlignmentCenter); 
			
			Font  FFontA(L"Arial", 45); 
			Font  FFontB(L"Arial", 30); 
			
			wchar_t szNum[100]={0};
			StringCchPrintfW(szNum,sizeof(szNum)/sizeof(wchar_t),L"%d",m_dwRequstCount);
			raphics.DrawString(szNum,wcslen(szNum),
				m_dwRequstCount < 10000 ? &FFontA:&FFontB,
				RectF(DrawRc.left,DrawRc.top + 30,(float)DrawRc.Width(),(float)DrawRc.Height()),
				&FFormat,
				&TitleBrush);
			
			Pen penEllipse(Color(255,255,255),4);
			raphics.DrawEllipse(&penEllipse,43,45,150,150);
		}
		
		{
			Font		 FFont(L"Arial", 9);
			StringFormat FFormat;                         
			FFormat.SetAlignment(StringAlignmentCenter);
			FFormat.SetLineAlignment(StringAlignmentFar); 
			
			WCHAR szText[100]={0};
			StringCchPrintfW(szText,sizeof(szText)/sizeof(wchar_t),L"已经处理请求\r\n%.1fKB",(float)m_dwRequstSize/1024);
			
			raphics.DrawString(szText,wcslen(szText),
				&FFont,
				RectF(DrawRc.left,DrawRc.top+ 48,(float)DrawRc.Width(),(float)DrawRc.Height() - 50),
				&FFormat,
				&TitleBrush);
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}



BOOL CKRSDEClientDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CKRSDEClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case  TIMER_INITNETWORK:
		{
			VSendMessage(WM_KRSDE_INITFAILURE,0,0);
			InsertRunLog("连接服务器超时,请重试!");
			m_ProgressWidget.SetAutoPos(FALSE);
			KillTimer(TIMER_INITNETWORK);
		}break;

	case  TIMER_HIDEKRPROGRESSWIDGET:
		{
			InsertRunLog("恭喜,初始化完毕!");
			m_ProgressWidget.ShowWindow(SW_HIDE);
			VSendMessage(WM_KRSDE_INITSUCCESS,0,0);
			KillTimer(TIMER_HIDEKRPROGRESSWIDGET);

		

//			SetTimer(TIMER_HIDEWINDOW,1000,NULL);

// 			INT nSize = 0;
// 			const char * pstr	= KRSDE_TickerSymbolByName("SH000001");
// 			const EXRIGHTS * p = KRSDE_GetExRights("SZ002568",&nSize);
//			KRSDE_F10Data_Remote("SH600052");
		}break;
	case  TIMER_HIDEWINDOW:
		{
			KillTimer(TIMER_HIDEWINDOW);
			ShowWindow(SW_HIDE);
		}break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CKRSDEClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	HPClient_Stop();
	HPClient_Destory();

	Globle::TickerSymbolDataMgr.Destory();
	Globle::FinancialDataMgr.Destory();
	Globle::ChuQuanDataMgr.Destory();
	Globle::HQBaseInfoDataMgr.Destory();
	Globle::RTHQDataMgr.Destory();

	CDialog::OnClose();
}

void CKRSDEClientDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if(nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		return;
	}
	CDialog::OnSysCommand(nID, lParam);
}


void CKRSDEClientDlg::InsertRunLog(LPCTSTR szFormat,... )
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	TCHAR szBuffer[1024]={0};
	StringCbVPrintf(szBuffer,sizeof(szBuffer)/sizeof(TCHAR),szFormat,vagrc);
	va_end(vagrc);
	TCHAR szText[2048]={0};
	StringCchPrintf(szText,sizeof(szText)/sizeof(TCHAR),TEXT("[%d]%s"),m_TextListWidget.GetCount(),szBuffer);
	m_TextListWidget.InsertString(0,szText);
}


void TransformKLine(DWORD dwPeriod ,LPKLINEDATA  pKLData , INT nSize , vector< KLINEDATA> & VKLNew)
{
	//日线以上级别 服务器发送过来的是日K 需要本地转换
	switch(dwPeriod)
	{
	case KLINE_PERIOD_WEEK:
		{
			VKL vklDay;
			vklDay.assign(pKLData,pKLData + nSize);
			DayKL_To_WeekKL(vklDay,VKLNew);
		}
		return;
	
	case KLINE_PERIOD_MONTH:
		{
			VKL vklDay;
			vklDay.assign(pKLData,pKLData + nSize);
			DayKL_To_MonthKL(vklDay,VKLNew);
		}return;


	case KLINE_PERIOD_QUARTER:
		{
			VKL vklDay;
			vklDay.assign(pKLData,pKLData + nSize);
			DayKL_To_QuarterKL(vklDay,VKLNew);
		}
		return;
	
	case KLINE_PERIOD_YEAR:
		{
			VKL vklDay;
			vklDay.assign(pKLData,pKLData + nSize);
			DayKL_To_YearKL(vklDay,VKLNew);	
		}return;

	case  KLINE_PERIOD_F1: 
	case  KLINE_PERIOD_F5:
	case  KLINE_PERIOD_F15:
	case  KLINE_PERIOD_F30: 
	case  KLINE_PERIOD_F60:
	case  KLINE_PERIOD_DAY:
			VKLNew.assign(pKLData,pKLData + nSize);
		break;
	}
}




BOOL FindKLine(const LPKLINEDATA  pBegin,const LPKLINEDATA  pEnd  , CTime tFind , LPKLINEDATA &  pResult)
{
	for (LPKLINEDATA pOffset = pEnd - 1 ; pOffset > pBegin + 1; pOffset--)
	{
		CTime tCur = (pOffset-0)->tUnix;
		CTime tBef = (pOffset-1)->tUnix;
		if( tCur >= tFind &&  tBef < tFind)
		{
			pResult = pOffset;
			return TRUE;
		}
	}
	return FALSE;
}

void DisposeFuQuan_Qian(const LPKLINEDATA  pBegin,const LPKLINEDATA  pEnd ,const RCV_POWER_STRUCTEx & RPS)
{
	#define QFQ(P) ( ( P - RPS.m_fProfit) + (RPS.m_fPei * RPS.m_fPeiPrice) ) / (1.00f + RPS.m_fGive)
	for (LPKLINEDATA pOffset = pEnd; pOffset >= pBegin; pOffset--)
	{
		pOffset->fOpen   = QFQ(pOffset->fOpen);
		pOffset->fClose  = QFQ(pOffset->fClose);
		pOffset->fLow    = QFQ(pOffset->fLow);
		pOffset->fHigh   = QFQ(pOffset->fHigh);
	}
}

void DisposeFuQuan_Hou(const LPKLINEDATA  pBegin,const LPKLINEDATA  pEnd ,const RCV_POWER_STRUCTEx & RPS)
{
	#define HFQ(P) (P * (1.00f + RPS.m_fGive) + RPS.m_fProfit) - (RPS.m_fPei * RPS.m_fPeiPrice)
	for (LPKLINEDATA pOffset = pBegin; pOffset < pEnd; pOffset++)
	{
		pOffset->fOpen   = HFQ(pOffset->fOpen);
		pOffset->fClose  = HFQ(pOffset->fClose);
		pOffset->fLow    = HFQ(pOffset->fLow);
		pOffset->fHigh   = HFQ(pOffset->fHigh);
	}
}


BOOL DisposeFuQuan(const char * szLabel ,LPKLINEDATA  pKLData , INT nSize , INT nFuQuan)
{
	if(nFuQuan == KLINE_FUQUAN_NO)
		return FALSE;

	INT nRPSSize = 0;
	const RCV_POWER_STRUCTEx * pRPS = Globle::ChuQuanDataMgr.Select(szLabel,&nRPSSize);
	if(pRPS == NULL)
		return FALSE;

	LPKLINEDATA  pBegin = pKLData;
	LPKLINEDATA  pEnd   = pKLData + nSize;
	LPKLINEDATA  pCurrent  = NULL , pBefore = NULL , pNext = NULL;
	RCV_POWER_STRUCTEx RPSTotal;
	BOOL bFrist = TRUE;
	if(nFuQuan == KLINE_FUQUAN_QIAN)
	{
 		for(INT i=0; i< nRPSSize ; i++)
		{
			if(pRPS[i].m_time == -1)
				continue;
			if(!FindKLine(pBegin,pEnd,pRPS[i].m_time,pCurrent))
				return FALSE;
			if( i + 1 < nRPSSize)
			{
				if(!FindKLine(pBegin,pEnd,pRPS[i+1].m_time,pBefore))
					pBefore = pBegin;
			}else
			{
				pBefore = pBegin;
			}
			if(bFrist == TRUE)
			{
				RPSTotal = pRPS[i];
				bFrist = FALSE;
			}else
			{
				RPSTotal.m_fProfit		+= pRPS[i].m_fProfit;
				RPSTotal.m_fPeiPrice	+= pRPS[i].m_fPeiPrice;
				RPSTotal.m_fPei			+= pRPS[i].m_fPei;
				RPSTotal.m_fGive		+= pRPS[i].m_fGive;
			}
			DisposeFuQuan_Qian(pBefore,pCurrent - 1,RPSTotal);
		}
	}else if(nFuQuan == KLINE_FUQUAN_HOU)
	{
		// 除权时间 是倒序的  
		// 例: 0 0000-00-00  1 2016-4-9 2 2015-5-7...
		// 后复权是时间是正序的 
		for(INT i= nRPSSize-1; i >=0  ; i-- )
		{
			if(pRPS[i].m_time == -1)
				continue;

			if(!FindKLine(pBegin,pEnd,pRPS[i].m_time,pCurrent))
				return FALSE;

			if( i - 1  > 0)
			{
				if(!FindKLine(pBegin,pEnd,pRPS[i-1].m_time,pNext))
					pNext = pEnd;
			}else
			{
				pNext =  pEnd;
			}

			if(bFrist == TRUE)
			{
				RPSTotal = pRPS[i];
				bFrist = FALSE;
			}else
			{
				RPSTotal.m_fProfit		+= pRPS[i].m_fProfit;
				RPSTotal.m_fPeiPrice	+= pRPS[i].m_fPeiPrice;
				RPSTotal.m_fPei			+= pRPS[i].m_fPei;
				RPSTotal.m_fGive		+= pRPS[i].m_fGive;
			}
			DisposeFuQuan_Hou(pCurrent,pNext,RPSTotal);
		}
	}
	return TRUE;
}



BOOL SmallRSSTo(LPSMALLRRS  pSmallRRS ,INT nSize , LPRCV_REPORT_STRUCTExV3 & pRRS )
{
	pRRS = new RCV_REPORT_STRUCTExV3[nSize];
	if(pRRS == NULL)
		return FALSE;
	
	ZeroMemory(pRRS,sizeof(RCV_REPORT_STRUCTExV3) * nSize );
	
	CTime t = pSmallRRS->m_time;
	STOCK_TICKER_SYMBOL *  pSTS = NULL;
	for (INT i=0;i<nSize;i++)
	{
		pSTS = Globle::TickerSymbolDataMgr.Select(pSmallRRS[i].nIndex);
		if(pSTS == NULL)
			continue;

		//不能改动 pTempRRS
		const RCV_REPORT_STRUCTExV3 * pTempRRS = Globle::RTHQDataMgr.Select((char *)&pSTS->m_wMarket);
		if(pTempRRS == NULL)
			continue;
		
// 		if(t != pSmallRRS[i].m_time ) 
// 		{
// 			CString szText;
// 			szText.Format("%d %d %d %d",i,nSize,t.GetTime(),pSmallRRS[i].m_time);
// 			t = pSmallRRS[i].m_time;
// 			OutputDebugString(szText);
// 		}


		CopyMemory(&pRRS[i],pTempRRS,sizeof(RCV_REPORT_STRUCTExV3));

		float  fBase            = pSmallRRS[i].m_fNewPrice;
		pRRS[i].m_fNewPrice		= pSmallRRS[i].m_fNewPrice; 
 		pRRS[i].m_time			= pSmallRRS[i].m_time;	
		pRRS[i].m_fVolume		= pSmallRRS[i].m_fVolume;
		pRRS[i].m_fAmount		= pSmallRRS[i].m_fAmount;
		

		if(pSmallRRS[i].m_fSellPrice[4] != 911.00f) pRRS[i].m_fSellPrice5    =  pSmallRRS[i].m_fSellPrice[4]    + fBase;  
		if(pSmallRRS[i].m_fSellPrice[3] != 911.00f) pRRS[i].m_fSellPrice4    =  pSmallRRS[i].m_fSellPrice[3]    + fBase;  
		if(pSmallRRS[i].m_fSellPrice[2] != 911.00f) pRRS[i].m_fSellPrice[2]  =  pSmallRRS[i].m_fSellPrice[2]    + fBase;  
		if(pSmallRRS[i].m_fSellPrice[1] != 911.00f) pRRS[i].m_fSellPrice[1]  =  pSmallRRS[i].m_fSellPrice[1]    + fBase;  
		if(pSmallRRS[i].m_fSellPrice[0] != 911.00f) pRRS[i].m_fSellPrice[0]  =  pSmallRRS[i].m_fSellPrice[0]    + fBase; 

		if(pSmallRRS[i].m_fBuyPrice[4] != 911.00f)  pRRS[i].m_fBuyPrice5     =  fBase - pSmallRRS[i].m_fBuyPrice[4];  
		if(pSmallRRS[i].m_fBuyPrice[3] != 911.00f)  pRRS[i].m_fBuyPrice4     =  fBase - pSmallRRS[i].m_fBuyPrice[3];  
		if(pSmallRRS[i].m_fBuyPrice[2] != 911.00f)  pRRS[i].m_fBuyPrice[2]   =  fBase - pSmallRRS[i].m_fBuyPrice[2];  
		if(pSmallRRS[i].m_fBuyPrice[1] != 911.00f)  pRRS[i].m_fBuyPrice[1]   =  fBase - pSmallRRS[i].m_fBuyPrice[1];  
		if(pSmallRRS[i].m_fBuyPrice[0] != 911.00f)  pRRS[i].m_fBuyPrice[0]   =  fBase - pSmallRRS[i].m_fBuyPrice[0];

		pRRS[i].m_fSellVolume5		= pSmallRRS[i].m_fSellVolume[4];
		pRRS[i].m_fSellVolume4		= pSmallRRS[i].m_fSellVolume[3];
		pRRS[i].m_fSellVolume[2]	= pSmallRRS[i].m_fSellVolume[2];
		pRRS[i].m_fSellVolume[1]	= pSmallRRS[i].m_fSellVolume[1];
		pRRS[i].m_fSellVolume[0]	= pSmallRRS[i].m_fSellVolume[0];
		
		pRRS[i].m_fBuyVolume5		= pSmallRRS[i].m_fBuyVolume[4];
		pRRS[i].m_fBuyVolume4		= pSmallRRS[i].m_fBuyVolume[3];
		pRRS[i].m_fBuyVolume[2]		= pSmallRRS[i].m_fBuyVolume[2];
		pRRS[i].m_fBuyVolume[1]		= pSmallRRS[i].m_fBuyVolume[1];
		pRRS[i].m_fBuyVolume[0]		= pSmallRRS[i].m_fBuyVolume[0];


 		//还未开盘 
 		CTime t = pSmallRRS[i].m_time;
 		if( t.GetHour() == 9  && t.GetMinute() < 25 )
 			continue;

		if(pRRS[i].m_fOpen == 0.00f )  
		{
			pRRS[i].m_fOpen= pSmallRRS[i].m_fNewPrice;
			pRRS[i].m_fHigh= pSmallRRS[i].m_fNewPrice;
			pRRS[i].m_fLow = pSmallRRS[i].m_fNewPrice;
			continue;
		}

		//更新最高价 最低价
		if(pRRS[i].m_fHigh < pSmallRRS[i].m_fNewPrice) 
			pRRS[i].m_fHigh = pSmallRRS[i].m_fNewPrice;

		if(pRRS[i].m_fLow > pSmallRRS[i].m_fNewPrice) 
			pRRS[i].m_fLow = pSmallRRS[i].m_fNewPrice;
	}
	return TRUE;
}



static BOOL g_RTQHFullOK = FALSE;
LRESULT CKRSDEClientDlg::OnHPClient_Receive(WPARAM wParam,LPARAM lParam)
{
	m_dwRequstCount++;

	HP_Client pClient = (HP_Client)wParam;
	LPHPCLIENT_RECVDATA pHR = (LPHPCLIENT_RECVDATA)lParam;
	m_dwRequstSize+= pHR->iLenght;
	RedrawWindow();

	CSocketDataMgr SDM; 
	if(!SDM.Unpack(pHR->pData,pHR->iLenght))
	{
		InsertRunLog("接收到错误数据lenght:%d",pHR->iLenght);
		return FALSE;
	}

	DWORD  dwToken; 
 	DWORD  dwErrorCode;
	LPBYTE  pData		= SDM.GetUnpackData();
	DWORD   dwDataSize	= SDM.GetUnpackSize();
	SDM.MakeRecvData(dwToken,dwErrorCode,pData,dwDataSize);

	switch(dwToken)
	{
	case TOKEN_KLINE:
		{
			LPTOKEN_KLINE_HEADINFO	pTKLDHeader;
			LPKLINEDATA				pKLData;
			DWORD					dwKLDataSize  = 0;
			SDM.MakeRecvData(SDM.GetUnpackData(),SDM.GetUnpackSize(),(LPBYTE &)pTKLDHeader,sizeof(TOKEN_KLINE_HEADINFO),(LPBYTE &)pKLData,dwKLDataSize);
			
			if(dwErrorCode ==TOKEN_KLINE_NOTFOUND){
				InsertRunLog("未找到K线!%s p:%d c:%d",pTKLDHeader->szLabel,pTKLDHeader->dwPeriod,pTKLDHeader->dwCount);
			}

			if(dwErrorCode ==TOKEN_KLINE_PERIODERROR){
				InsertRunLog("级别错误!%s p:%d c:%d",pTKLDHeader->szLabel,pTKLDHeader->dwPeriod,pTKLDHeader->dwCount);
			}
			if(dwErrorCode ==TOKEN_KLINE_OK)
			{
				Globle::ClientKLineDataMgr.Insert(pTKLDHeader->szLabel,pKLData,pTKLDHeader->dwCount,pTKLDHeader->dwPeriod);

				//先复权
				DisposeFuQuan(pTKLDHeader->szLabel,pKLData,pTKLDHeader->dwCount,pTKLDHeader->nFuQuan);
				
				//转换K线
				vector<KLINEDATA> VklTF;
				TransformKLine(pTKLDHeader->dwPeriod,pKLData,pTKLDHeader->dwCount,VklTF);
				pTKLDHeader->dwCount = VklTF.size();
				
				if(pTKLDHeader->wMark != KLINE_MARK_NOMESSAGE)
				{
					::VSendMessage(WM_KRSDE_KLINE_REMOTE,(WPARAM)pTKLDHeader,(LPARAM)VklTF.begin());
					InsertRunLog("%s p:%d c:%d",pTKLDHeader->szLabel,pTKLDHeader->dwPeriod,pTKLDHeader->dwCount);
				}
			}
		}break;


	case TOKEN_FENBIDATA:
		{
			LPTOKEN_TRADINGRECORD_HEADINFO	pTTRHeader;
			LPTRADINGRECORD					pTTR;
			DWORD							dwTTRSize  = 0;
			SDM.MakeRecvData(SDM.GetUnpackData(),SDM.GetUnpackSize(),(LPBYTE &)pTTRHeader,sizeof(TOKEN_TRADINGRECORD_HEADINFO),(LPBYTE &)pTTR,dwTTRSize);
			
			if(dwErrorCode ==TOKEN_FENBIDATA_NOTFOUND){
				InsertRunLog("未找到分笔!%s %04u/%2u/%2u",pTTRHeader->szLabel,pTTRHeader->wYear, pTTRHeader->wMonth,pTTRHeader->wDay);
			}

			if(dwErrorCode ==TOKEN_FENBIDATA_ISEMPTY){
				InsertRunLog("该日期无分笔!%s %04u/%2u/%2u",pTTRHeader->szLabel,pTTRHeader->wYear, pTTRHeader->wMonth,pTTRHeader->wDay);
			}

			if(dwErrorCode ==TOKEN_FENBIDATA_OK){
				Globle::FenBiDataMgr.Insert(pTTRHeader->szLabel,pTTR,pTTRHeader->dwCount);
				::VSendMessage(WM_KRSDE_TRADINGRECORD_REMOTE,(WPARAM)pTTRHeader,(LPARAM)pTTR);
				InsertRunLog("取分笔%s %04u-%2u-%2u",pTTRHeader->szLabel,pTTRHeader->wYear, pTTRHeader->wMonth,pTTRHeader->wDay);
			}
		}break;

	case TOKEN_F10DATA:
		{
			LPTOKEN_F10_HEADINFO			pF10RHeader;
			char	*						pszBuffer;
			DWORD							dwBufferSize  = 0;
			SDM.MakeRecvData(SDM.GetUnpackData(),SDM.GetUnpackSize(),(LPBYTE &)pF10RHeader,sizeof(TOKEN_F10_HEADINFO),(LPBYTE &)pszBuffer,dwBufferSize);

			if(dwErrorCode ==TOKEN_F10DATA_NOTFOUND){
				InsertRunLog("未找F10资料! %s ",pF10RHeader->szLabel);
			}
			if(dwErrorCode ==TOKEN_F10DATA_SERVERERROR){
				InsertRunLog("取F10资料错误! %s ",pF10RHeader->szLabel);
			}
			if(dwErrorCode ==TOKEN_F10DATA_NOUPDATE)
			{
				char	 *	pBuffer = new char[5 * 1024 * 1024];
				DWORD		dwSize	= 5 * 1024 * 1024;
				if(pBuffer == NULL)
					return FALSE;
				CHAR szFileName[30]={0};
				StringCbPrintf(szFileName,sizeof(szFileName),"%s.txt",pF10RHeader->szLabel);
				if(!Globle::F10DirMgr.LoadFile(szFileName,pBuffer,dwSize))
				{
					delete [] pBuffer;
					return FALSE;
				}
				pF10RHeader->dwCount = dwSize;
				::VSendMessage(WM_KRSDE_F10DATA_REMOTE,(WPARAM)pF10RHeader,(LPARAM)pBuffer);
				InsertRunLog("取F10资料成功!%s [0]",pF10RHeader->szLabel);
				delete [] pBuffer;
			}
			if(dwErrorCode ==TOKEN_F10DATA_OK){
				::VSendMessage(WM_KRSDE_F10DATA_REMOTE,(WPARAM)pF10RHeader,(LPARAM)pszBuffer);
				InsertRunLog("取F10资料成功!%s [1]",pF10RHeader->szLabel);
				CHAR szFileName[30]={0};
				StringCbPrintf(szFileName,sizeof(szFileName),"%s.txt",pF10RHeader->szLabel);
				Globle::F10DirMgr.SaveFile(szFileName,pszBuffer,pF10RHeader->dwCount);
			}
		}break;

	case  TOKEN_RTQH_FULL:
		{
			DWORD dwFenBiCount = 0;
			RCV_REPORT_STRUCTExV3 * pRRS = (RCV_REPORT_STRUCTExV3 * )pData;
			for(int i=0; i< dwDataSize / sizeof(RCV_REPORT_STRUCTExV3); i++)
			{
				if(!CheckLabel(pRRS->m_szLabel))
					continue;

				//已经开盘 记录分笔数据
				if(!Price_Equality(pRRS->m_fOpen,0.00f))
				{
					const RCV_REPORT_STRUCTExV3 * pOldRRS = Globle::RTHQDataMgr.Select((const char *)&pRRS->m_wMarket);
					if(Globle::FenBiDataMgr.Insert(pRRS , pOldRRS , FALSE))
						dwFenBiCount++;
				}

				Globle::RTHQDataMgr.Insert(*pRRS);
				pRRS++;
			}
			VSendMessage(WM_KRSDE_REALTIMEQUOTES_REMOTE,(WPARAM)pData,(LPARAM)dwDataSize / sizeof(RCV_REPORT_STRUCTExV3));
			InsertRunLog("实时行情F %d 分笔%d",(LPARAM)dwDataSize / sizeof(RCV_REPORT_STRUCTExV3) , dwFenBiCount);
		}break;

	case  TOKEN_RTQH_SMALL:
		{
			if(g_RTQHFullOK == FALSE)
			{
				InsertRunLog("实时行情未初始化！");
				break;
			}
			INT nSize = dwDataSize / sizeof(SMALLRRS);
			if(nSize <= 0)
				break;

			RCV_REPORT_STRUCTExV3 * pRRS = NULL;
			if(!SmallRSSTo((LPSMALLRRS)pData , nSize , pRRS ))
				break;

			RCV_REPORT_STRUCTExV3 * pTemp = pRRS;
 			DWORD dwFenBiCount = 0;
			for(int i=0; i< nSize; i++)
			{
 				//已经开盘 记录分笔数据
 				if(!Price_Equality(pRRS->m_fOpen,0.00f))
 				{
 					const RCV_REPORT_STRUCTExV3 * pOldRRS = Globle::RTHQDataMgr.Select((const char *)&pRRS->m_wMarket);
 					if(Globle::FenBiDataMgr.Insert(pRRS , pOldRRS , FALSE))
 						dwFenBiCount++;
 
 					Globle::ZSDataMgr.Insert(pRRS->m_time,pRRS->m_fNewPrice,(const char *)&pRRS->m_wMarket);
 				}
				Globle::RTHQDataMgr.Insert(*pRRS);
				pRRS++;
			} 
			VSendMessage(WM_KRSDE_REALTIMEQUOTES_REMOTE,(WPARAM)pTemp,(LPARAM)nSize);
			delete [] pTemp;
			InsertRunLog("实时行情S %d 分笔%d",nSize , dwFenBiCount);
		}break;




	case  TOKEN_TICKERSYMBOL:
		{
			if(dwErrorCode == TOKEN_TICKERSYMBOL_NOUPDATE)
			{
				InsertRunLog("接收码表信息成功(无更新)!");
				m_ProgressWidget.SetPos(30);
			}
			if(dwErrorCode == TOKEN_TICKERSYMBOL_ERROR)
			{
				InsertRunLog("接收码表信息失败,请重试!");
				m_ProgressWidget.SetPos(99);
			}
			if(dwErrorCode == TOKEN_TICKERSYMBOL_OK)
			{
				Globle::TickerSymbolDataMgr.Init( ( STOCK_TICKER_SYMBOL  *)pData,dwDataSize);
				InsertRunLog("更新码表信息%d条!",Globle::TickerSymbolDataMgr.GetSize());
				m_ProgressWidget.SetPos(30);
			}
		}break;


	case  TOKEN_HQBASE:
		{
			if(dwErrorCode == TOKEN_HQBASE_NOUPDATE)
			{
				InsertRunLog("接收基础信息成功(无更新)!");
				m_ProgressWidget.SetPos(50);
			}
			if(dwErrorCode == TOKEN_HQBASE_ERROR)
			{
				InsertRunLog("接收基础信息失败,请重试!");
				m_ProgressWidget.SetPos(99);
			}
			if(dwErrorCode == TOKEN_HQBASE_OK)
			{
				Globle::HQBaseInfoDataMgr.Init((HQBASE_INFO *)pData,dwDataSize);
				InsertRunLog("更新基础信息%d条!",Globle::HQBaseInfoDataMgr.GetSize());
				m_ProgressWidget.SetPos(50);
			}
		}break;


	case  TOKEN_RTQH_INIT:
		{
			if(dwErrorCode == TOKEN_RTQH_INIT_NOUPDATE)
			{
				Globle::HangYeDataMgr.Init(Globle::RTHQDataMgr.GetMapPoint(),Globle::HQBaseInfoDataMgr.GetMapPoint());

				DWORD dwMemoryObjectSize = 0;
				BYTE * pMemoryObject = Globle::RTHQDataMgr.GetMemoryObject(dwMemoryObjectSize);
				VSendMessage(WM_KRSDE_REALTIMEQUOTES_REMOTE,(WPARAM)pMemoryObject,(LPARAM)dwMemoryObjectSize / sizeof(RCV_REPORT_STRUCTExV3));
				Globle::RTHQDataMgr.FreeMemoryObject(pMemoryObject,dwMemoryObjectSize);

				InsertRunLog("接收行情信息成功(无更新)!");
				m_ProgressWidget.SetPos(65);

				g_RTQHFullOK = TRUE;
			}

			if(dwErrorCode == TOKEN_RTQH_INIT_ERROR)
			{
				InsertRunLog("接收行情信息失败,请重试!");
				m_ProgressWidget.SetPos(99);
			}

			if(dwErrorCode == TOKEN_RTQH_INIT_OK)
			{
				g_RTQHFullOK = TRUE;

				Globle::RTHQDataMgr.Init((RCV_REPORT_STRUCTExV3 *)pData,dwDataSize);
				Globle::HangYeDataMgr.Init(Globle::RTHQDataMgr.GetMapPoint(),Globle::HQBaseInfoDataMgr.GetMapPoint());

				InsertRunLog("更新行情信息%d条!",Globle::RTHQDataMgr.GetSize());
				m_ProgressWidget.SetPos(65);
				VSendMessage(WM_KRSDE_REALTIMEQUOTES_REMOTE,(WPARAM)pData,(LPARAM)dwDataSize / sizeof(RCV_REPORT_STRUCTExV3));
			}
		}break;



	case  TOKEN_FINANCIAL:
		{
			if(dwErrorCode == TOKEN_FINANCIAL_NOUPDATE)
			{
				InsertRunLog("接收财务信息成功(无更新)!");
				m_ProgressWidget.SetPos(75);
			}
			if(dwErrorCode == TOKEN_FINANCIAL_ERROR)
			{
				InsertRunLog("接收财务信息失败,请重试!");
				m_ProgressWidget.SetPos(99);
			}
			if(dwErrorCode == TOKEN_FINANCIAL_OK)
			{
				Globle::FinancialDataMgr.Init((Fin_LJF_STRUCTEx *)pData,dwDataSize);
				InsertRunLog("更新接收财务信息%d条!",Globle::FinancialDataMgr.GetSize());
				m_ProgressWidget.SetPos(75);
			}
		}break;


	case  TOKEN_CHUQUAN:
		{
			if(dwErrorCode == TOKEN_CHUQUAN_NOUPDATE)
			{
				InsertRunLog("接收除权信息成功(无更新)!");
				m_ProgressWidget.SetPos(99);
			}
			if(dwErrorCode == TOKEN_CHUQUAN_ERROR)
			{
				InsertRunLog("接收除权信息失败,请重试!");
				m_ProgressWidget.SetPos(99);
			}
			if(dwErrorCode == TOKEN_CHUQUAN_OK)
			{
				Globle::ChuQuanDataMgr.Init((RCV_POWER_STRUCTEx *)pData,dwDataSize);
				InsertRunLog("更新除权信息%d条!",Globle::ChuQuanDataMgr.GetSize());
				m_ProgressWidget.SetPos(99);
			}
			SetTimer(TIMER_HIDEKRPROGRESSWIDGET,500,NULL);
		}break;
	}


	return TRUE;
}

LRESULT CKRSDEClientDlg::OnHPClient_Connect(WPARAM wParam,LPARAM lParam)
{
	KillTimer(TIMER_INITNETWORK);
	LPHPCLIENT_LISTENDATA  lpHPLD = (LPHPCLIENT_LISTENDATA)wParam; 
	
	if(!lParam)	
	{
		InsertRunLog("连接服务器%s:%d错误!请重试!%s",lpHPLD->szAddress,lpHPLD->usPort,HPClient_GetErrorMsg());
		return FALSE;
	}
	
	m_bConnected = TRUE;
	InsertRunLog("连接服务器%s:%d成功!",lpHPLD->szAddress,lpHPLD->usPort);
	

	//请求码表数据
	if(Globle::TickerSymbolDataMgr.Init())
	{
		DWORD dwMemoryObjectSize = 0;
		BYTE * pMemoryObject = Globle::TickerSymbolDataMgr.GetMemoryObject(dwMemoryObjectSize);
		
		CSocketDataMgr SDM;
		UINT uCRC32 =  SDM.CRC32(pMemoryObject,dwMemoryObjectSize);
		SDM.MakeSendData(TOKEN_TICKERSYMBOL,uCRC32,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("更新码表数据!本地%d条",Globle::TickerSymbolDataMgr.GetSize());

		Globle::TickerSymbolDataMgr.FreeMemoryObject(pMemoryObject,dwMemoryObjectSize);
	}else
	{
		CSocketDataMgr SDM;
		SDM.MakeSendData(TOKEN_TICKERSYMBOL,0,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("获取码表数据中....");
	}

	KRSDE_GetKLine_Remote("SH000001",KLINE_PERIOD_DAY,KLINE_FUQUAN_NO,0,KLINE_MARK_NOMESSAGE);


	//请求基础信息
	if(Globle::HQBaseInfoDataMgr.Init())
	{
		DWORD dwMemoryObjectSize = 0;
		BYTE * pMemoryObject = Globle::HQBaseInfoDataMgr.GetMemoryObject(dwMemoryObjectSize);
		
		CSocketDataMgr SDM;
		UINT uCRC32 =  SDM.CRC32(pMemoryObject,dwMemoryObjectSize);
		SDM.MakeSendData(TOKEN_HQBASE,uCRC32,NULL,0);

		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("更新基础信息!本地%d条",Globle::HQBaseInfoDataMgr.GetSize());

		Globle::HQBaseInfoDataMgr.FreeMemoryObject(pMemoryObject,dwMemoryObjectSize);
	}else
	{
		CSocketDataMgr SDM;
		SDM.MakeSendData(TOKEN_HQBASE,0,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("获取基础信息中....");
	}

	


	//请求行情信息
	if(Globle::RTHQDataMgr.Init())
	{
		DWORD dwMemoryObjectSize = 0;
		BYTE * pMemoryObject = Globle::RTHQDataMgr.GetMemoryObject(dwMemoryObjectSize);
		
		CSocketDataMgr SDM;
		UINT uCRC32 =  SDM.CRC32(pMemoryObject,dwMemoryObjectSize);
		SDM.MakeSendData(TOKEN_RTQH_INIT,uCRC32,NULL,0);
		
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("更新行情信息!本地%d条",Globle::RTHQDataMgr.GetSize());
		
		Globle::RTHQDataMgr.FreeMemoryObject(pMemoryObject,dwMemoryObjectSize);
	}else
	{
		CSocketDataMgr SDM;
		SDM.MakeSendData(TOKEN_RTQH_INIT,0,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("获行情信息中....");
	}



	//请求财务信息
	if(Globle::FinancialDataMgr.Init())
	{
		DWORD dwMemoryObjectSize = 0;
		BYTE * pMemoryObject = Globle::FinancialDataMgr.GetMemoryObject(dwMemoryObjectSize);
		CSocketDataMgr SDM;
		UINT uCRC32 =  SDM.CRC32(pMemoryObject,dwMemoryObjectSize);
		SDM.MakeSendData(TOKEN_FINANCIAL,uCRC32,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("更新财务数据!本地%d条",Globle::FinancialDataMgr.GetSize());

		Globle::FinancialDataMgr.FreeMemoryObject(pMemoryObject,dwMemoryObjectSize);
	}else
	{
		CSocketDataMgr SDM;
		SDM.MakeSendData(TOKEN_FINANCIAL,0,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("获取财务数据中......");
	}
	

	//请求除权信息
	if(Globle::ChuQuanDataMgr.Init())
	{
		DWORD dwMemoryObjectSize = 0;
		BYTE * pMemoryObject = Globle::ChuQuanDataMgr.GetMemoryObject(dwMemoryObjectSize);
		CSocketDataMgr SDM;
		UINT uCRC32 =  SDM.CRC32(pMemoryObject,dwMemoryObjectSize);
		SDM.MakeSendData(TOKEN_CHUQUAN,uCRC32,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("更新除权数据!本地%d条",Globle::ChuQuanDataMgr.GetSize());
		Globle::ChuQuanDataMgr.FreeMemoryObject(pMemoryObject,dwMemoryObjectSize);
	}else
	{
		CSocketDataMgr SDM;
		SDM.MakeSendData(TOKEN_CHUQUAN,0,NULL,0);
		HPClient_SendData(SDM.GetSendData(),SDM.GetSendDataSize());
		InsertRunLog("获取除权数据中......");
	}

	

// 	KRSDE_GetKLine_Remote("SZ399001",KLINE_PERIOD_DAY,KLINE_FUQUAN_NO,0,KLINE_MARK_NOMESSAGE);
// 	KRSDE_GetKLine_Remote("SZ399006",KLINE_PERIOD_DAY,KLINE_FUQUAN_NO,0,KLINE_MARK_NOMESSAGE);
// 	KRSDE_GetKLine_Remote("SZ399005",KLINE_PERIOD_DAY,KLINE_FUQUAN_NO,0,KLINE_MARK_NOMESSAGE);
	return TRUE;
}






UINT CKRSDEClientDlg::ReConnect(LPVOID lpParameter)
{
	CKRSDEClientDlg * lpKRSDEClientDlg = (CKRSDEClientDlg *)lpParameter;
	HPClient_Stop();
	HPClient_Destory();

	if(!HPClient_Create())
	{
		lpKRSDEClientDlg->MessageBox("初始化网络失败\r\n请检查你的网络!","警告",MB_ICONERROR | MB_OK);
		return FALSE;	
	}
	return HPClient_Start(SERVER_ADDRESS,8000,TRUE);;
}



LRESULT CKRSDEClientDlg::OnHPClient_Close(WPARAM wParam,LPARAM lParam)
{
	KillTimer(TIMER_INITNETWORK);
	if(wParam == 5) 
		return FALSE;
	
	if(!m_bConnected)
	{
		InsertRunLog("连接服务器失败,请重试!");
		InsertRunLog("%d %s",HPClient_GetLastError(),HPClient_GetErrorMsg());
		m_ProgressWidget.SetAutoPos(FALSE);
	}else
	{
		VSendMessage(WM_KRSDE_DISCONNECT,0,0);
		InsertRunLog("%d %s",HPClient_GetLastError(),HPClient_GetErrorMsg());
		InsertRunLog("与服务器的连接中断!重试中....");
		AfxBeginThread(ReConnect,this);
	}
	m_bConnected  = FALSE;
	return TRUE;
}


void CKRSDEClientDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags == MK_LBUTTON )
 		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

