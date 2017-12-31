
#ifndef __STOCKDRV_H__
#define __STOCKDRV_H__


// 工作方式类型定义
#define RCV_WORK_SENDMSG			4					//消息工作方式

/*
指数代号-名称
1A0001  上证综合指数		1A0002  上证Ａ股指数		1A0003  上证Ｂ股指数
1B0001  上证工业指数		1B0002  上证商业指数		1B0004  上证地产指数
1B0005  上证公用事业		1B0006  上证综合			1B0007  上证30指数
*/

/////////////////////////////////////////////////////////////////////////////////////////

// 证券市场
#define		SH_MARKET_EX			'HS'		// 上海
#define		SZ_MARKET_EX			'ZS'		// 深圳
#define		HK_MARKET_EX			'KH'		// 香港

// 文件数据类型
// 结构数组形式的文件数据
#define		FILE_HISTORY_EX			2			// 补日线数据
#define		FILE_MINUTE_EX			4			// 补分钟线数据
#define		FILE_POWER_EX			6			// 补充除权数据
#define     FILE_5MINUTE_EX         81 		    // 补5分钟线数据

#define		FILE_BASE_EX			0x1000		
#define		FILE_NEWS_EX			0x1002		// 新闻类,其类型由m_szFileName中子目录名来定
#define		FILE_HTML_EX			0x1004		// HTML文件,m_szFileName为URL

#define		FILE_TYPE_RES			-1			// 保留

// 消息子类型
#define		News_Sha_Ex				 2			// 上证消息
#define		News_Szn_Ex				 4			// 深证消息
#define		News_Fin_Ex				 6			// 财经报道
#define		News_TVSta_Ex			 8			// 电视台通知
#define		News_Unknown_Ex			 -1			// 未知提供者

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define		RI_IDSTRING				1			// 厂商名称,返回(LPCSTR)厂商名
#define		RI_IDCODE				2			// 卡号
#define		RI_VERSION				3			// 驱动程序版本
#define		RI_V2SUPPORT			6			// 支持深圳SJS库结构

#define		STKLABEL_LEN			10			// 股号数据长度,国内市场股号编码
#define		STKNAME_LEN				32			// 股名长度
#define     MKTNAME_LEN             16          // 码表所代表的市场名称


#pragma pack(push)
#pragma pack(1)                                 // 改为按字节对齐


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//行情数据(版本3数据结构RCV_WORK_SENDMSG，兼容RCV_REPORT_STRUCTExV2结构，使用m_cbSize区分不同结构)
//版本3支持5档买卖盘

typedef struct tagRCV_REPORT_STRUCTExV3	
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
} RCV_REPORT_STRUCTExV3,*PRCV_REPORT_STRUCTExV3,*LPRCV_REPORT_STRUCTExV3;


//////////////////////////////////////////////////////////////////////////////////



//分笔数据///////////////////////////////////
typedef struct tagRCV_FENBI_STRUCTEx
{
	long		m_lTime;					// hhmmss 例：93056 表明9:
	float		m_fHigh;					// 最高
	float		m_fLow;						// 最低 
	float		m_fNewPrice;				// 最新 
	float		m_fVolume;					// 成交量
	float		m_fAmount;					// 成交额
	long		m_lStroke;					// 保留

	float		m_fBuyPrice[5];				// 申买价1,2,3,4,5
	float		m_fBuyVolume[5];			// 申买量1,2,3,4,5
	float		m_fSellPrice[5];			// 申卖价1,2,3,4,5
	float		m_fSellVolume[5];			// 申卖量1,2,3,4,5
}RCV_FENBI_STRUCTEx,*PRCV_FENBI_STRUCTEx,FENBIDATA,*LPFENBIDATA;


typedef struct tagRCV_FENBI
{
	WORD					m_wMarket;					// 股票市场类型
	char					m_szLabel[STKLABEL_LEN];	// 股票代码,以'\0'结尾
	long					m_lDate;					// 分笔数据的日期 FORMAT:
	float					m_fLastClose;				// 昨收
	float					m_fOpen;					// 今开
	WORD					m_nCount;					// m_Data的数据量分笔笔数
	RCV_FENBI_STRUCTEx*		m_Data;						// 长度为m_nCount
}RCV_FENBI,*PRCV_FENBI;



//补充数据头
//数据头 m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG	0xffffffff

typedef struct	tagRCV_EKE_HEADEx
{
	DWORD	m_dwHeadTag;								// = EKE_HEAD_TAG
	WORD	m_wMarket;									// 市场类型
	char	m_szLabel[STKLABEL_LEN];					// 股票代码
}RCV_EKE_HEADEx;

//补充日线数据
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		time_t	m_time;				//UCT
		float	m_fOpen;			//开盘
		float	m_fHigh;			//最高
		float	m_fLow;				//最低
		float	m_fClose;			//收盘
		float	m_fVolume;			//量
		float	m_fAmount;			//额
		WORD	m_wAdvance;			//涨数,仅大盘有效
		WORD	m_wDecline;			//跌数,仅大盘有效
	};
	RCV_EKE_HEADEx	m_head;
}RCV_HISTORY_STRUCTEx;


//补充分时线数据
typedef union tagRCV_MINUTE_STRUCTEx
{
	struct{
		time_t	m_time;				// UCT
		float	m_fPrice;
		float	m_fVolume;
		float	m_fAmount;
	};
	RCV_EKE_HEADEx	m_head; 
}RCV_MINUTE_STRUCTEx;

//补充除权数据
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		time_t	m_time;				// UCT
		float	m_fGive;			// 每股送
		float	m_fPei;				// 每股配
		float	m_fPeiPrice;		// 配股价,仅当 m_fPei!=0.0f 时有效
		float	m_fProfit;			// 每股红利
	};
	RCV_EKE_HEADEx	m_head;
}RCV_POWER_STRUCTEx;



//补充历史五分钟K线数据,每一数据结构都应通过 m_time == EKE_HEAD_TAG,判断是否为 m_head,然后再作解释
typedef union tagRCV_HISMINUTE_STRUCTEx
{
	struct
    {
        time_t   m_time;         	   // UCT
        float    m_fOpen;		       //开盘
        float    m_fHigh;		       //最高
		float    m_fLow;               //最低
		float    m_fClose;             //收盘
		float    m_fVolume;            //量
		float    m_fAmount;            //额
		float    m_fActiveBuyVol;      //主动买量如没有计算m_fActiveBuyVol=0
	};
    RCV_EKE_HEADEx  m_head;
}RCV_HISMINUTE_STRUCTEx;


typedef struct tagRCV_FILE_HEADEx
{
	DWORD	m_dwAttrib;							// 文件子类型
	DWORD	m_dwLen;							// 文件长度
	DWORD	m_dwSerialNo;						// 序列号
	char	m_szFileName[MAX_PATH];				// 文件名 or URL
} RCV_FILE_HEADEx;

//////////////////////////////////////////////////////////////////////////////////
//  数据通知消息
//  直接数据引用通知消息
//	wParam = RCV_WPARAM;
//  lParam 指向 RCV_DATA结构;
//	返回 1 已经处理, 0 未处理或不能处理

#define RCV_REPORT			0x3f001234
#define RCV_FILEDATA		0x3f001235
#define RCV_FENBIDATA		0x3f001258 
#define RCV_MKTTBLDATA      0x3f001259           //接收到市场码表数据
#define RCV_FINANCEDATA     0x3f001300           //接收到财务文件数据



typedef struct tagRCV_DATA
{
	int					m_wDataType;			// 文件类型
	int					m_nPacketNum;			// 记录数,参见注一
	RCV_FILE_HEADEx		m_File;					// 文件接口
	BOOL				m_bDISK;				// 文件是否已存盘的文件
	union
	{
		RCV_REPORT_STRUCTExV3*	m_pReportV3;
		RCV_HISTORY_STRUCTEx *	m_pDay;
		RCV_MINUTE_STRUCTEx  *	m_pMinute;
		RCV_POWER_STRUCTEx   *	m_pPower;
		RCV_HISMINUTE_STRUCTEx *	m_p5Min;
		void		     *	m_pData;		// 参见注二
	};
} RCV_DATA;



typedef struct  tagRCV_TABLE_STRUCT 
{														  // 证券
    char   m_szLabel[STKLABEL_LEN] ;                      // 股票代码,以'\0'结尾,如 "500500"
    char   m_szName[STKNAME_LEN];                         // 股票名称,以'\0'结尾,"上证指数"
    WORD   m_cProperty;                                   // 每手股数
}RCV_TABLE_STRUCT,* PRCV_TABLE_STRUCT;


typedef struct  tagHLMarketType                //市场内容
{
    WORD   m_wMarket;			         //市场代码
    char   m_Name[MKTNAME_LEN];			 //市场名称,以'\0'结尾
    DWORD  m_lProperty;					 //市场属性（未定义）
    DWORD  m_lDate;						 //数据日期（20030114）
    WORD   m_PeriodCount;			     //交易时段个数
    WORD   m_OpenTime[5];		         //开市时间 1,2,3,4,5
    WORD   m_CloseTime[5];	             //收市时间 1,2,3,4,5
    WORD   m_nCount;                     //该市场的证券个数
    RCV_TABLE_STRUCT* m_Data;	         //长度为m_nCou
}HLMarketType,*PHLMarketType;



typedef struct  tagFin_LJF_STRUCTEx
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
}Fin_LJF_STRUCTEx,*PFin_LJF_STRUCTEx;


#pragma pack(pop)










#endif // __STOCKDRV_H__