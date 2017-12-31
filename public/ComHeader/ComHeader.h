#ifndef ComHeader_h__
#define ComHeader_h__

//����������ֻ��VC6.0 ��ʹ��
//������ǳ��ִ�����4786����  
//�뽫�ô���ŵ�stdafx.h�м���
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
//��������������ӵ�CPP���� ���ļ�ֻ��һ��ͷ�ļ�
//VC6.0��  û��round������������д....
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

#define		STKLABEL_LEN			10			// �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define		STKNAME_LEN				32			// ��������
#define     MKTNAME_LEN             16          // �����������г�����


typedef struct _KLINEDATA
{
	time_t tUnix;
	float fOpen;		//���̼�
	float fHigh;		//��߼�
	float fLow;			//��ͼ�
	float fClose;		//���̼�
	float fVolume;		//�ɽ���	
	float fAmount;		//���	
}KLINEDATA,*LPKLINEDATA;


enum{ TRADINGRECORD_MARK_NORMAL,TRADINGRECORD_MARK_ZDBUY,TRADINGRECORD_MARK_ZDSELL};
typedef struct _TRADINGRECORD
{
	long		m_lTime;					// 
	float		m_fHigh;					// ���
	float		m_fLow;						// ��� 
	float		m_fNewPrice;				// ���� 
	float		m_fVolume;					// �ɽ���
	float		m_fAmount;					// �ɽ���
	BYTE		m_bMark;					// ������־
}TRADINGRECORD,*LPTRADINGRECORD;


typedef struct  _TICKER_SYMBOL
{
	WORD   m_wMarket;
    char   m_szLabel[STKLABEL_LEN];							// ��Ʊ����,�����г�����,��'\0'��β,�� "600868" , "300431"
    char   m_szName[STKNAME_LEN];							// ��Ʊ����,��'\0'��β,"��ָ֤��"
	WORD   m_cProperty;										// ÿ�ֹ���
}STOCK_TICKER_SYMBOL,*LPSTOCK_TICKER_SYMBOL;


typedef struct _EXRIGHTS
{
	time_t	m_time;				// UCT
	float	m_fGive;			// ÿ����
	float	m_fPei;				// ÿ����
	float	m_fPeiPrice;		// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
	float	m_fProfit;			// ÿ�ɺ���
}*LPEXRIGHTS,EXRIGHTS;




typedef struct  _FINANCIAL
{
    WORD   m_wMarket;       // ��Ʊ�г�����
    WORD   N1;              // �����ֶ�
    char   m_szLabel[10];   // ��Ʊ����,��'\0'��β,�� "600050"  10���ֽ� ͬͨ�ӹ淶����
    long   BGRQ ;           // �������ݵ����� ����걨 ������ �� 20090630 ��ʾ 2009����걨
    float  ZGB;             // �ܹɱ�
    float  GJG;             // ���ҹ�
    float  FQFRG;           // �����˷��˹�
    float  FRG;             // ���˹�
    float  BGS;             // B��
    float  HGS;             // H��
    float  MQLT;            // Ŀǰ��ͨ
    float  ZGG;             // ְ����
    float  A2ZPG;           // A2ת���
    float  ZZC;             // ���ʲ�(ǧԪ)
    float  LDZC;            // �����ʲ�
    float  GDZC;            // �̶��ʲ�
    float  WXZC;            // �����ʲ�
    float  CQTZ;            // ����Ͷ��
    float  LDFZ;            // ������ծ
    float  CQFZ;            // ���ڸ�ծ
    float  ZBGJJ;           // �ʱ�������
    float  MGGJJ;           // ÿ�ɹ�����
    float  GDQY;            // �ɶ�Ȩ��
    float  ZYSR;            // ��Ӫ����
    float  ZYLR;            // ��Ӫ����
    float  QTLR;            // ��������
    float  YYLR;            // Ӫҵ����
    float  TZSY;            // Ͷ������
    float  BTSR;            // ��������
    float  YYWSZ;           // Ӫҵ����֧
    float  SNSYTZ;          // �����������
    float  LRZE;            // �����ܶ�
    float  SHLR;            // ˰������
    float  JLR;             // ������
    float  WFPLR;           // δ��������
    float  MGWFP;           // ÿ��δ����
    float  MGSY;            // ÿ������
    float  MGJZC;           // ÿ�ɾ��ʲ�
    float  TZMGJZC;         // ����ÿ�ɾ��ʲ�
    float  GDQYB;           // �ɶ�Ȩ���
    float  JZCSYL;          // ����������
}FINANCIAL,*LPFINANCIAL;


typedef struct _REALTIMEQUOTES
{
	WORD	m_cbSize;									// �ṹ��С
	time_t	m_time;										// �ɽ�ʱ��
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;								// ���� �����ڻ��洢��������㣬�������������һ�ʼ۸�
	float	m_fOpen;									// ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���
	float	m_fAmount;									// �ɽ��� �����ڻ��洢���ǳֲ���
	
	float	m_fBuyPrice[3];								// �����1,2,3
	float	m_fBuyVolume[3];							// ������1,2,3
	float	m_fSellPrice[3];							// ������1,2,3
	float	m_fSellVolume[3];							// ������1,2,3
	
	float	m_fBuyPrice4;								// �����4
	float	m_fBuyVolume4;								// ������4
	float	m_fSellPrice4;								// ������4
	float	m_fSellVolume4;								// ������4
	
	float	m_fBuyPrice5;								// �����5
	float	m_fBuyVolume5;								// ������5
	float	m_fSellPrice5;								// ������5
	float	m_fSellVolume5;								// ������5
}REALTIMEQUOTES,*LPREALTIMEQUOTES;


enum{ 
	KLINE_MARK_DEFUALT=911, 
	KLINE_MARK_NOMESSAGE 
};

typedef struct _TOKEN_KLINE_HEADINFO
{
	char  szLabel[10];		//��Ʊ���� SZ300431
	DWORD dwPeriod;			//K�߼���  
	DWORD dwCount;			//K������  0��Ϊȫ��
	INT   nFuQuan;          //0 ����Ȩ 1 ǰ��Ȩ  2 ��Ȩ
	WORD   wMark;           //���
	DWORD  wExt1;           //Ԥ��
	DWORD  wExt2;           //Ԥ��
}TOKEN_KLINE_HEADINFO ,*LPTOKEN_KLINE_HEADINFO;


typedef struct _TOKEN_TRADINGRECORD_HEADINFO{
	char  szLabel[10];			//��Ʊ���� SZ300431
	WORD  wYear;
	WORD  wMonth;
	WORD  wDay;				
	DWORD dwCount;				//�ֱ����� 0��Ϊȫ��	
}TOKEN_TRADINGRECORD_HEADINFO,*LPTOKEN_TRADINGRECORD_HEADINFO;


typedef struct _TOKEN_F10_HEADINFO
{
	char  szLabel[10];		//��Ʊ���� SZ300431
	DWORD dwSignature;
	DWORD dwCount;		    //	
}TOKEN_F10_HEADINFO ,*LPTOKEN_F10_HEADINFO;




typedef struct _HQBASE_INFO
{
	//char  szLabel[8];

	WORD   wMarket;
    char   szLabel[8];

	float f5DayVolume;   //5�ճɽ�����
	float f5DayAmount;   //5�ճɽ��ܽ��

	char  szZYYW[600];//-��Ӫҵ��
	char  szSSHY[15];//-������ҵ
	char  szSJGN[180];//-�漰���� || --����ǿ������

	float fSYLDT_BL;    //-��ӯ��(��̬) 
	float fSYLJT_BL;    //-��ӯ��(��̬) 

	float fSJL;      //-�о���

	char  szZXJJ[11];//-���½������
	DWORD dwGDZS;	//�ɶ�����
	float fJSQBH;//-�����ڱ仯
	float fQSDLTGDCC;//-ǰʮ����ͨ�ɶ��ֲ�
}HQBASE_INFO,*LPHQBASE_INFO;


#pragma pack(pop)



// ֪ͨ��Ϣ
enum
{
		WM_KRSDE_INITSUCCESS=(WM_USER+0x111),			//��ʼ���ɹ�
		WM_KRSDE_INITFAILURE,							//��ʼ��ʧ��
		WM_KRSDE_DISCONNECT,							//��������Ͽ�����
		WM_KRSDE_KLINE_REMOTE,							//K������
		WM_KRSDE_REALTIMEQUOTES_REMOTE,					//ʵʱ��������
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
		KLINE_FUQUAN_NO,   //����Ȩ
		KLINE_FUQUAN_QIAN, //ǰ��Ȩ
		KLINE_FUQUAN_HOU   //��Ȩ
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