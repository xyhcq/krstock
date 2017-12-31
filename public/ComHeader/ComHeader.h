#ifndef ComHeader_h__
#define ComHeader_h__

//下面这句代码只在VC6.0 下使用
//如果还是出现大量的4786警告  
//请将该代码放到stdafx.h中即可
#pragma warning(disable:4786) 

#include <TCHAR.H>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <VECTOR>
#include <set>
#include <map>
#include <string>
using namespace std;
#include  <math.h>

/*
//本函数请自行添加到CPP问题 本文件只是一个头文件
//VC6.0下  没有round函数，无奈自写....
float KRRound(float val, int places) 
{
	float f = fabs(val);
	f = f*pow(10,places);
    f += 0.5;
    f = ceil(f);
	f = f/pow(10,places);

	if(val > 0 ) 
		return f;
	return -f;
}
*/


#define  Price_Equality(a,b)  ( (bool)( fabs( (float)a - (float)b ) <= 0.0001f ))

#define  Amount_Equality(a,b) ( (int)a == (int)b )

#define  Volume_Equality(a,b) ( (int)a == (int)b )

#pragma pack(push,1)

#define		STKLABEL_LEN			10			// 股号数据长度,国内市场股号编码兼容钱龙
#define		STKNAME_LEN				32			// 股名长度
#define     MKTNAME_LEN             16          // 码表所代表的市场名称


typedef struct _KLINEDATA
{
	time_t tUnix;
	float fOpen;		//开盘价
	float fHigh;		//最高价
	float fLow;			//最低价
	float fClose;		//收盘价
	float fVolume;		//成交量	
	float fAmount;		//金额	
}KLINEDATA,*LPKLINEDATA;


enum{ TRADINGRECORD_MARK_NORMAL,TRADINGRECORD_MARK_ZDBUY,TRADINGRECORD_MARK_ZDSELL};
typedef struct _TRADINGRECORD
{
	long		m_lTime;					// 
	float		m_fHigh;					// 最高
	float		m_fLow;						// 最低 
	float		m_fNewPrice;				// 最新 
	float		m_fVolume;					// 成交量
	float		m_fAmount;					// 成交额
	BYTE		m_bMark;					// 动作标志
}TRADINGRECORD,*LPTRADINGRECORD;


typedef struct  _TICKER_SYMBOL
{
	WORD   m_wMarket;
    char   m_szLabel[STKLABEL_LEN];							// 股票代码,包含市场类型,以'\0'结尾,如 "600868" , "300431"
    char   m_szName[STKNAME_LEN];							// 股票名称,以'\0'结尾,"上证指数"
	WORD   m_cProperty;										// 每手股数
}STOCK_TICKER_SYMBOL,*LPSTOCK_TICKER_SYMBOL;


typedef struct _EXRIGHTS
{
	time_t	m_time;				// UCT
	float	m_fGive;			// 每股送
	float	m_fPei;				// 每股配
	float	m_fPeiPrice;		// 配股价,仅当 m_fPei!=0.0f 时有效
	float	m_fProfit;			// 每股红利
}*LPEXRIGHTS,EXRIGHTS;




typedef struct  _FINANCIAL
{
    WORD   m_wMarket;       // 股票市场类型
    WORD   N1;              // 保留字段
    char   m_szLabel[10];   // 股票代码,以'\0'结尾,如 "600050"  10个字节 同通视规范定义
    long   BGRQ ;           // 财务数据的日期 如半年报 季报等 如 20090630 表示 2009年半年报
    float  ZGB;             // 总股本
    float  GJG;             // 国家股
    float  FQFRG;           // 发起人法人股
    float  FRG;             // 法人股
    float  BGS;             // B股
    float  HGS;             // H股
    float  MQLT;            // 目前流通
    float  ZGG;             // 职工股
    float  A2ZPG;           // A2转配股
    float  ZZC;             // 总资产(千元)
    float  LDZC;            // 流动资产
    float  GDZC;            // 固定资产
    float  WXZC;            // 无形资产
    float  CQTZ;            // 长期投资
    float  LDFZ;            // 流动负债
    float  CQFZ;            // 长期负债
    float  ZBGJJ;           // 资本公积金
    float  MGGJJ;           // 每股公积金
    float  GDQY;            // 股东权益
    float  ZYSR;            // 主营收入
    float  ZYLR;            // 主营利润
    float  QTLR;            // 其他利润
    float  YYLR;            // 营业利润
    float  TZSY;            // 投资收益
    float  BTSR;            // 补贴收入
    float  YYWSZ;           // 营业外收支
    float  SNSYTZ;          // 上年损益调整
    float  LRZE;            // 利润总额
    float  SHLR;            // 税后利润
    float  JLR;             // 净利润
    float  WFPLR;           // 未分配利润
    float  MGWFP;           // 每股未分配
    float  MGSY;            // 每股收益
    float  MGJZC;           // 每股净资产
    float  TZMGJZC;         // 调整每股净资产
    float  GDQYB;           // 股东权益比
    float  JZCSYL;          // 净资收益率
}FINANCIAL,*LPFINANCIAL;


typedef struct _REALTIMEQUOTES
{
	WORD	m_cbSize;									// 结构大小
	time_t	m_time;										// 成交时间
	WORD	m_wMarket;									// 股票市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码,以'\0'结尾
	char	m_szName[STKNAME_LEN];						// 股票名称,以'\0'结尾
	
	float	m_fLastClose;								// 昨收 对于期货存储的是昨结算，而不是昨天最后一笔价格
	float	m_fOpen;									// 今开
	float	m_fHigh;									// 最高
	float	m_fLow;										// 最低
	float	m_fNewPrice;								// 最新
	float	m_fVolume;									// 成交量
	float	m_fAmount;									// 成交额 对于期货存储的是持仓量
	
	float	m_fBuyPrice[3];								// 申买价1,2,3
	float	m_fBuyVolume[3];							// 申买量1,2,3
	float	m_fSellPrice[3];							// 申卖价1,2,3
	float	m_fSellVolume[3];							// 申卖量1,2,3
	
	float	m_fBuyPrice4;								// 申买价4
	float	m_fBuyVolume4;								// 申买量4
	float	m_fSellPrice4;								// 申卖价4
	float	m_fSellVolume4;								// 申卖量4
	
	float	m_fBuyPrice5;								// 申买价5
	float	m_fBuyVolume5;								// 申买量5
	float	m_fSellPrice5;								// 申卖价5
	float	m_fSellVolume5;								// 申卖量5
}REALTIMEQUOTES,*LPREALTIMEQUOTES;


enum{ 
	KLINE_MARK_DEFUALT=911, 
	KLINE_MARK_NOMESSAGE 
};

typedef struct _TOKEN_KLINE_HEADINFO
{
	char  szLabel[10];		//股票代码 SZ300431
	DWORD dwPeriod;			//K线级别  
	DWORD dwCount;			//K线数量  0则为全部
	INT   nFuQuan;          //0 不复权 1 前复权  2 后复权
	WORD   wMark;           //标记
	DWORD  wExt1;           //预留
	DWORD  wExt2;           //预留
}TOKEN_KLINE_HEADINFO ,*LPTOKEN_KLINE_HEADINFO;


typedef struct _TOKEN_TRADINGRECORD_HEADINFO{
	char  szLabel[10];			//股票代码 SZ300431
	WORD  wYear;
	WORD  wMonth;
	WORD  wDay;				
	DWORD dwCount;				//分笔数量 0则为全部	
}TOKEN_TRADINGRECORD_HEADINFO,*LPTOKEN_TRADINGRECORD_HEADINFO;


typedef struct _TOKEN_F10_HEADINFO
{
	char  szLabel[10];		//股票代码 SZ300431
	DWORD dwSignature;
	DWORD dwCount;		    //	
}TOKEN_F10_HEADINFO ,*LPTOKEN_F10_HEADINFO;




typedef struct _HQBASE_INFO
{
	//char  szLabel[8];

	WORD   wMarket;
    char   szLabel[8];

	float f5DayVolume;   //5日成交总量
	float f5DayAmount;   //5日成交总金额

	char  szZYYW[600];//-主营业务
	char  szSSHY[15];//-所属行业
	char  szSJGN[180];//-涉及概念 || --概念强弱排名

	float fSYLDT_BL;    //-市盈率(动态) 
	float fSYLJT_BL;    //-市盈率(静态) 

	float fSJL;      //-市净率

	char  szZXJJ[11];//-最新解禁日期
	DWORD dwGDZS;	//股东总数
	float fJSQBH;//-较上期变化
	float fQSDLTGDCC;//-前十大流通股东持仓
}HQBASE_INFO,*LPHQBASE_INFO;


#pragma pack(pop)



// 通知消息
enum
{
		WM_KRSDE_INITSUCCESS=(WM_USER+0x111),			//初始化成功
		WM_KRSDE_INITFAILURE,							//初始化失败
		WM_KRSDE_DISCONNECT,							//与服务器断开连接
		WM_KRSDE_KLINE_REMOTE,							//K线数据
		WM_KRSDE_REALTIMEQUOTES_REMOTE,					//实时行情数据
		WM_KRSDE_TRADINGRECORD_REMOTE,
		WM_KRSDE_F10DATA_REMOTE
};

enum{
		KLINE_PERIOD_F1,
		KLINE_PERIOD_F5,
		KLINE_PERIOD_F15,
		KLINE_PERIOD_F30,
		KLINE_PERIOD_F60,
		KLINE_PERIOD_DAY,
		KLINE_PERIOD_WEEK,
		KLINE_PERIOD_MONTH,
		KLINE_PERIOD_QUARTER,
		KLINE_PERIOD_YEAR
};

enum{
		KLINE_FUQUAN_NO,   //不复权
		KLINE_FUQUAN_QIAN, //前复权
		KLINE_FUQUAN_HOU   //后复权
};






enum KRSDE_SW {KRSDE_SW_HIDE,KRSDE_SW_SHOW,KRSDE_SW_AUTO} ;


void									WINAPI	KRSDE_SetHWND(HWND hWnd);
BOOL									WINAPI	KRSDE_Start();
BOOL									WINAPI	KRSDE_Stop();
const STOCK_TICKER_SYMBOL *				WINAPI	KRSDE_GetTickerSymbol(INT * lpnSize);
const char *							WINAPI	KRSDE_TickerSymbolByName(const char * szFullLabel);
const FINANCIAL *						WINAPI	KRSDE_GetFinancial(const char * szFullLabel);
const EXRIGHTS *						WINAPI	KRSDE_GetExRights(const char * szFullLabel , INT * lpnSize);
BOOL									WINAPI	KRSDE_GetKLine_Remote(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan,DWORD dwCount,WORD wMark);
const REALTIMEQUOTES *					WINAPI	KRSDE_GetRealTimeQuotes(const char * szFullLabel);
DWORD									WINAPI  KRSDE_GetAllRealTimeQuotes( REALTIMEQUOTES *  pRTQ );
BOOL									WINAPI	KRSDE_GetTradingRecord_Remote (const char * szFullLabel,WORD  wYear,WORD  wMonth,WORD  wDay);
const LPTRADINGRECORD					WINAPI  KRSDE_GetTradingRecord_Local (const char * szFullLabel,INT * pnSize);

void									WINAPI	KRSDE_ShowWindow(BOOL bShow);


BOOL									WINAPI  KRSDE_GetKLine_Local(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan, KLINEDATA ** pResult , DWORD * pdwSize);
void									WINAPI  KRSDE_GetKLine_Local_Free(KLINEDATA ** pResult);

BOOL									WINAPI  KRSDE_GetHQBaseInfo(const char * szFullLabel, LPHQBASE_INFO lpBaseInfo );
float									WINAPI  KRSDE_GetHYZhangFu(const char * szHangYe);
float									WINAPI  KRSDE_GetZhangSu(const char * szLabel , INT nMinutePeriod);

typedef BOOL(WINAPI *								KRSDEStartT)();
typedef BOOL(WINAPI *								KRSDEStopT)();
typedef void(WINAPI *								KRSDE_SetHWNDT)(HWND hWnd);
typedef BOOL(WINAPI *								KRSDE_GetKLine_RemoteT)(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan,DWORD dwKLineNum,WORD wMark);
typedef BOOL(WINAPI *								KRSDE_GetKLine_LocalT)(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan, KLINEDATA ** pResult , DWORD * pdwSize);
typedef void(WINAPI *								KRSDE_GetKLine_Local_FreeT)(KLINEDATA ** pResult);
typedef const REALTIMEQUOTES *( WINAPI *			KRSDE_GetRealTimeQuotesT)(const char * szFullLabel);
typedef const FINANCIAL *( WINAPI *					KRSDE_GetFinancialT)(const char * szFullLabel);
typedef BOOL( WINAPI *								KRSDE_GetTradingRecord_RemoteT)(const char * szFullLabel,WORD  wYear,WORD  wMonth,WORD  wDay);
typedef const char *( WINAPI *						KRSDE_TickerSymbolByNameT) (const char * szFullLabel);
typedef DWORD (	WINAPI *							KRSDE_GetAllRealTimeQuotesT)(REALTIMEQUOTES *  pRTQ);
typedef const STOCK_TICKER_SYMBOL * (	WINAPI *	KRSDE_GetTickerSymbolT)(INT * lpnSize);
typedef void ( WINAPI   *                     	    KRSDE_ShowWindowT)(KRSDE_SW krsd_sw);
typedef BOOL ( WINAPI   *                           KRSDE_GetHQBaseInfoT)(const char * szFullLabel, LPHQBASE_INFO lpBaseInfo );
typedef float( WINAPI   *							KRSDE_GetHYZhangFuT)(const char * szHangYe);
typedef float( WINAPI   *							KRSDE_GetZhangSuT)(const char * szLabel , INT nMinutePeriod);
typedef const LPTRADINGRECORD  ( WINAPI   *			KRSDE_GetTradingRecord_LocalT) (const char * szFullLabel,INT * pnSize);
typedef const EXRIGHTS *  ( WINAPI *				KRSDE_GetExRightsT)(const char * szFullLabel , INT * lpnSize);



class  CKRStockDataEngine
{
public:
	CKRStockDataEngine()
	{
		m_hModule = NULL;
	}
	
	~CKRStockDataEngine()
	{
		if(m_hModule) 
		{
			//FreeLibraryAndExitThread(m_hModule,0);
			FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}
		
	BOOL Init()
	{
		GetModuleFileName(NULL,m_szModulePath,MAX_PATH);
		PathRemoveFileSpec(m_szModulePath);
		
		_tcscat(m_szModulePath,TEXT("\\KRSDEClient.dll"));
		m_hModule = LoadLibrary(m_szModulePath);
		if(m_hModule == NULL)
			return FALSE;
		
		KRSDEStartProc = (KRSDEStartT)GetProcAddress(m_hModule,"KRSDE_Start");
		if(KRSDEStartProc == NULL) 
			return FALSE;

		KRSDEStopProc  = (KRSDEStopT)GetProcAddress(m_hModule,"KRSDE_Stop");
		if(KRSDEStopProc == NULL) 
			return FALSE;

		KRSDE_SetHWND  = (KRSDE_SetHWNDT)GetProcAddress(m_hModule,"KRSDE_SetHWND");
		if(KRSDE_SetHWND == NULL) 
			return FALSE;

		KRSDE_GetKLine_Remote = (KRSDE_GetKLine_RemoteT)GetProcAddress(m_hModule,"KRSDE_GetKLine_Remote");
		if(KRSDE_GetKLine_Remote == NULL) 
			return FALSE;

		KRSDE_GetKLine_Local = (KRSDE_GetKLine_LocalT)GetProcAddress(m_hModule,"KRSDE_GetKLine_Local");
		if(KRSDE_GetKLine_Local == NULL) 
			return FALSE;

		KRSDE_GetKLine_Local_Free = (KRSDE_GetKLine_Local_FreeT)GetProcAddress(m_hModule,"KRSDE_GetKLine_Local_Free");
		if(KRSDE_GetKLine_Local_Free == NULL)
			return FALSE;


		KRSDE_GetRealTimeQuotes = (KRSDE_GetRealTimeQuotesT)GetProcAddress(m_hModule,"KRSDE_GetRealTimeQuotes");
		if(KRSDE_GetRealTimeQuotes ==NULL)
			return FALSE;

		KRSDE_GetAllRealTimeQuotes = (KRSDE_GetAllRealTimeQuotesT)GetProcAddress(m_hModule,"KRSDE_GetAllRealTimeQuotes");
		if(KRSDE_GetAllRealTimeQuotes == NULL)
			return FALSE;

		KRSDE_GetFinancial = (KRSDE_GetFinancialT)GetProcAddress(m_hModule,"KRSDE_GetFinancial");
		if(KRSDE_GetFinancial ==NULL)
			return FALSE;
		
		KRSDE_GetTradingRecord_Remote = (KRSDE_GetTradingRecord_RemoteT)GetProcAddress(m_hModule,"KRSDE_GetTradingRecord_Remote");
		if(KRSDE_GetTradingRecord_Remote ==NULL)
			return FALSE;

		KRSDE_GetTradingRecord_Local = (KRSDE_GetTradingRecord_LocalT)GetProcAddress(m_hModule,"KRSDE_GetTradingRecord_Local");
		if(KRSDE_GetTradingRecord_Local == NULL)
			return FALSE;

		KRSDE_TickerSymbolByName = (KRSDE_TickerSymbolByNameT)GetProcAddress(m_hModule,"KRSDE_TickerSymbolByName");
		if(KRSDE_TickerSymbolByName == NULL)
			return FALSE;

		KRSDE_GetTickerSymbol= (KRSDE_GetTickerSymbolT)GetProcAddress(m_hModule,"KRSDE_GetTickerSymbol");
		if(KRSDE_GetTickerSymbol == NULL)
			return FALSE;

		KRSDE_ShowWindow = (KRSDE_ShowWindowT)GetProcAddress(m_hModule,"KRSDE_ShowWindow");
		if(KRSDE_ShowWindow == NULL)
			return FALSE;
		
		KRSDE_GetHQBaseInfo = (KRSDE_GetHQBaseInfoT)GetProcAddress(m_hModule,"KRSDE_GetHQBaseInfo");
		if(KRSDE_GetHQBaseInfo == NULL)
			return FALSE;

		KRSDE_GetHYZhangFu= (KRSDE_GetHYZhangFuT)GetProcAddress(m_hModule,"KRSDE_GetHYZhangFu");
		if(KRSDE_GetHYZhangFu == NULL)
			return FALSE;

		KRSDE_GetZhangSu = (KRSDE_GetZhangSuT)GetProcAddress(m_hModule,"KRSDE_GetZhangSu");
		if(KRSDE_GetZhangSu == NULL)
			return FALSE;
		
		KRSDE_GetExRights = (KRSDE_GetExRightsT)GetProcAddress(m_hModule,"KRSDE_GetExRights");
		if(KRSDE_GetExRights == NULL)
			return FALSE;

		return TRUE;
	}
	
	BOOL Start()
	{ 
		return KRSDEStartProc();
	}

	BOOL Stop()
	{
		return KRSDEStopProc();  
	}

	void SetHWND(HWND hWnd)
	{
		KRSDE_SetHWND(hWnd);
	}
	
	BOOL GetKLine_Remote(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan,DWORD dwKLineNum,WORD wMark = KLINE_MARK_DEFUALT)
	{
		return KRSDE_GetKLine_Remote(szFullLabel,dwPeriod,nFuQuan,dwKLineNum,wMark);
	}

	BOOL GetKLine_Local(const char * szFullLabel,DWORD dwPeriod,INT nFuQuan, vector<KLINEDATA> & Result)
	{
		KLINEDATA * pBuffer = NULL;
		DWORD       dwSize  = 0;
		if(!KRSDE_GetKLine_Local(szFullLabel,dwPeriod,nFuQuan,&pBuffer,&dwSize))
		{
			if(pBuffer) 	
				KRSDE_GetKLine_Local_Free(&pBuffer);
			return FALSE;
		}


		Result.assign(pBuffer, pBuffer + dwSize);
		KRSDE_GetKLine_Local_Free(&pBuffer);
		return TRUE;
	}



	const REALTIMEQUOTES * GetRealTimeQuotes(const char * szFullLabel)
	{
		return KRSDE_GetRealTimeQuotes(szFullLabel);
	}
		
	BOOL  GetAllRealTimeQuotes(vector<REALTIMEQUOTES>  & VRTQ )
	{
		REALTIMEQUOTES * pRTQ = NULL;
		DWORD dwSize = KRSDE_GetAllRealTimeQuotes(pRTQ);
		if(dwSize == 0) 
			return FALSE;

		VRTQ.resize(dwSize);
		KRSDE_GetAllRealTimeQuotes(VRTQ.begin());	

		return TRUE;
	}

	

	const FINANCIAL * GetFinancial(const char * szFullLabel)
	{
		return KRSDE_GetFinancial(szFullLabel);
	}

	BOOL GetTradingRecord_Remote(const char * szFullLabel,WORD  wYear,WORD  wMonth,WORD  wDay)
	{
		return KRSDE_GetTradingRecord_Remote(szFullLabel,wYear,wMonth,wDay);
	}

	BOOL  GetTradingRecord_Local(const char * szFullLabel , vector<TRADINGRECORD> & TRD)
	{
		INT nSize = 0;
		const LPTRADINGRECORD pTRD = KRSDE_GetTradingRecord_Local(szFullLabel,&nSize);

		if(pTRD ==NULL)
			return FALSE;

		TRD.assign(pTRD, pTRD + nSize);
		return TRUE;
	}

	const char * Get_TickerSymbolByName(const char * szFullLabel){
		return KRSDE_TickerSymbolByName(szFullLabel);
	}

	const STOCK_TICKER_SYMBOL * GetTickerSymbol(INT * lpnSize)
	{
		return KRSDE_GetTickerSymbol(lpnSize);
	}

	BOOL GetHQBaseInfo(const char * szFullLabel, LPHQBASE_INFO lpBaseInfo )
	{
		return KRSDE_GetHQBaseInfo(szFullLabel,lpBaseInfo);
	}
	
	float GetHYZhangFu(const char * szHangYe)
	{
		return KRSDE_GetHYZhangFu(szHangYe);
	}

	float GetZhangSu(const char * szLabel , INT nMinutePeriod){
		return KRSDE_GetZhangSu(szLabel,nMinutePeriod);
	}

	void ShowWindow(KRSDE_SW krsd_sw)
	{
		KRSDE_ShowWindow(krsd_sw);
	}

	BOOL GetExRights(const char * szLabel , set<time_t> & SER){
		INT nSize = 0;
		const EXRIGHTS * pER = KRSDE_GetExRights(szLabel,&nSize);
		if(pER == NULL)
			return FALSE;

		for (INT i=0; i< nSize; i++)
		{
			if(pER->m_time != -1)
				SER.insert(pER->m_time);
			
			pER++;
		}
		return TRUE;
	}



private:
	TCHAR m_szModulePath[MAX_PATH];
	HMODULE m_hModule;
	KRSDEStartT						KRSDEStartProc;
	KRSDEStopT						KRSDEStopProc;
	KRSDE_SetHWNDT					KRSDE_SetHWND;
	KRSDE_GetKLine_RemoteT			KRSDE_GetKLine_Remote;
	KRSDE_GetKLine_LocalT			KRSDE_GetKLine_Local;
	KRSDE_GetKLine_Local_FreeT		KRSDE_GetKLine_Local_Free;
	KRSDE_GetRealTimeQuotesT		KRSDE_GetRealTimeQuotes;
	KRSDE_GetFinancialT				KRSDE_GetFinancial;
	KRSDE_GetTradingRecord_RemoteT  KRSDE_GetTradingRecord_Remote;
	KRSDE_GetTradingRecord_LocalT	KRSDE_GetTradingRecord_Local;
	KRSDE_TickerSymbolByNameT		KRSDE_TickerSymbolByName;
	KRSDE_GetAllRealTimeQuotesT		KRSDE_GetAllRealTimeQuotes;
	KRSDE_GetTickerSymbolT			KRSDE_GetTickerSymbol;
	KRSDE_ShowWindowT				KRSDE_ShowWindow;
	KRSDE_GetHQBaseInfoT			KRSDE_GetHQBaseInfo;
	KRSDE_GetHYZhangFuT				KRSDE_GetHYZhangFu;
	KRSDE_GetZhangSuT				KRSDE_GetZhangSu;
	KRSDE_GetExRightsT				KRSDE_GetExRights;
};



#endif // ComHeader_h__