#ifndef StockCommon_h__
#define StockCommon_h__





#include <VECTOR>
using namespace std;

typedef struct _KTIMETABLE
{
	WORD wBeginHour;
	WORD wBeginMinute;
	WORD wEndHour;
	WORD wEndMinute;
}KTIMETABLE,*LPKTIMETABLE;

extern KTIMETABLE KTTableF1[242];
extern KTIMETABLE KTTableF5[48];
extern KTIMETABLE KTTableF15[16];
extern KTIMETABLE KTTableF30[8];
extern KTIMETABLE KTTableF60[4];

BOOL FenBiToMultiKLine(const RCV_REPORT_STRUCTExV3 * pCurRRS , LPTRADINGRECORD pTTR , INT nTTRSize,const KTIMETABLE * TTable , INT nTTableSize , vector<KLINEDATA> & VKLNew,BOOL bFull = FALSE);
BOOL CheckF5KLine( vector<KLINEDATA> &  VKLF5);
BOOL SmallKL_To_BigKL(vector<KLINEDATA> & VSmallKL , vector<KLINEDATA> & VBigKL,INT nMultiple);
BOOL DayKL_To_WeekKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VWeekKLine);
BOOL DayKL_To_MonthKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VMonthKLine);
BOOL DayKL_To_QuarterKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VQuarterKLine);
BOOL DayKL_To_YearKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VMonthKLine);


BOOL	MarketIsOpenedTime();
BOOL	CheckLabel(const char * lpszLabel);
void	MakeFullLabel(char * szRetFullLabel, const char * szLabel, WORD wMarket);
INT     MergeKLine( vector<KLINEDATA> & VklNew,vector<KLINEDATA> & VklOld);


BOOL CheckTradingRecord(const LPTRADINGRECORD pTRR ,vector<KLINEDATA> &  vkl );
BOOL MakeFullDayKLine(vector<KLINEDATA> &  vkl ,const RCV_REPORT_STRUCTExV3 * pRRS);

BOOL IsStock(const char * lpszLabel);

//BOOL FenBiToKLine(LPTRADINGRECORD pTTR , INT nTTRSize,const KLINETIMETABLE * TTable , INT nTTableSize , vector<KLINEDATA> & VKLOld);


/*
typedef struct _KLINETIMETABLE
{
	WORD wHour;
	WORD wMinute;
	BOOL bUsed;
}KLINETIMETABLE,*LPKLINETIMETABLE;
extern KLINETIMETABLE F1Table[241];
extern KLINETIMETABLE F5Table[49];

enum	{SCT_0915_0920,SCT_0920_0925,SCT_0925_0930,SCT_0930_1130,SCT_1300_1500,SCT_Other};
INT     StockCheckTime();
BOOL	MarketIsOpenedTime();
void	MakeFullLabel(char * szRetFullLabel, const char * szLabel, WORD wSCType);
BOOL	CheckLabel(const char * lpszLabel);
time_t	GetNextF5Time(CTime  TCur);
BOOL    CheckF5Time(CTime t);
void	FenBiTo1MinuteKLine(LPTRADINGRECORD lpTR , INT nCount , vector<KLINEDATA> & KLine1Muinte);
void	VKLineToSingleKLine(vector < KLINEDATA>  & VKL , KLINEDATA & KLineData);
BOOL	SmallKL_To_BigKL(vector<KLINEDATA> & VSmallKL , vector<KLINEDATA> & VBigKL,INT nMultiple);

BOOL	DayKL_To_WeekKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VWeekKLine);
BOOL	DayKL_To_MonthKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VMonthKLine);
BOOL	DayKL_To_YearKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VYearKLine);
*/

#endif // StockCommon_h__