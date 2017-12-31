#include "stdafx.h"
#include "StockCommon.h"
#include <VECTOR>
using namespace std;



KTIMETABLE KTTableF1[242]=
{
	{ 9,25, 9,31},
	{ 9,31, 9,32},{ 9,32, 9,33},{ 9,33, 9,34},{ 9,34, 9,35},{ 9,35, 9,36},
	{ 9,36, 9,37},{ 9,37, 9,38},{ 9,38, 9,39},{ 9,39, 9,40},{ 9,40, 9,41},
	{ 9,41, 9,42},{ 9,42, 9,43},{ 9,43, 9,44},{ 9,44, 9,45},{ 9,45, 9,46},
	{ 9,46, 9,47},{ 9,47, 9,48},{ 9,48, 9,49},{ 9,49, 9,50},{ 9,50, 9,51},
	{ 9,51, 9,52},{ 9,52, 9,53},{ 9,53, 9,54},{ 9,54, 9,55},{ 9,55, 9,56},
	{ 9,56, 9,57},{ 9,57, 9,58},{ 9,58, 9,59},{ 9,59,10, 0},{10, 0,10, 1},
	{10, 1,10, 2},{10, 2,10, 3},{10, 3,10, 4},{10, 4,10, 5},{10, 5,10, 6},
	{10, 6,10, 7},{10, 7,10, 8},{10, 8,10, 9},{10, 9,10,10},{10,10,10,11},
	{10,11,10,12},{10,12,10,13},{10,13,10,14},{10,14,10,15},{10,15,10,16},
	{10,16,10,17},{10,17,10,18},{10,18,10,19},{10,19,10,20},{10,20,10,21},
	{10,21,10,22},{10,22,10,23},{10,23,10,24},{10,24,10,25},{10,25,10,26},
	{10,26,10,27},{10,27,10,28},{10,28,10,29},{10,29,10,30},{10,30,10,31},
	{10,31,10,32},{10,32,10,33},{10,33,10,34},{10,34,10,35},{10,35,10,36},
	{10,36,10,37},{10,37,10,38},{10,38,10,39},{10,39,10,40},{10,40,10,41},
	{10,41,10,42},{10,42,10,43},{10,43,10,44},{10,44,10,45},{10,45,10,46},
	{10,46,10,47},{10,47,10,48},{10,48,10,49},{10,49,10,50},{10,50,10,51},
	{10,51,10,52},{10,52,10,53},{10,53,10,54},{10,54,10,55},{10,55,10,56},
	{10,56,10,57},{10,57,10,58},{10,58,10,59},{10,59,11, 0},{11, 0,11, 1},
	{11, 1,11, 2},{11, 2,11, 3},{11, 3,11, 4},{11, 4,11, 5},{11, 5,11, 6},
	{11, 6,11, 7},{11, 7,11, 8},{11, 8,11, 9},{11, 9,11,10},{11,10,11,11},
	{11,11,11,12},{11,12,11,13},{11,13,11,14},{11,14,11,15},{11,15,11,16},
	{11,16,11,17},{11,17,11,18},{11,18,11,19},{11,19,11,20},{11,20,11,21},
	{11,21,11,22},{11,22,11,23},{11,23,11,24},{11,24,11,25},{11,25,11,26},
	{11,26,11,27},{11,27,11,28},{11,28,11,29},{11,29,11, 30},
	{11,30,11, 40},
	{13, 0,13, 1},{13, 1,13, 2},{13, 2,13, 3},{13, 3,13, 4},{13, 4,13, 5},
	{13, 5,13, 6},{13, 6,13, 7},{13, 7,13, 8},{13, 8,13, 9},{13, 9,13,10},
	{13,10,13,11},{13,11,13,12},{13,12,13,13},{13,13,13,14},{13,14,13,15},
	{13,15,13,16},{13,16,13,17},{13,17,13,18},{13,18,13,19},{13,19,13,20},
	{13,20,13,21},{13,21,13,22},{13,22,13,23},{13,23,13,24},{13,24,13,25},
	{13,25,13,26},{13,26,13,27},{13,27,13,28},{13,28,13,29},{13,29,13,30},
	{13,30,13,31},{13,31,13,32},{13,32,13,33},{13,33,13,34},{13,34,13,35},
	{13,35,13,36},{13,36,13,37},{13,37,13,38},{13,38,13,39},{13,39,13,40},
	{13,40,13,41},{13,41,13,42},{13,42,13,43},{13,43,13,44},{13,44,13,45},
	{13,45,13,46},{13,46,13,47},{13,47,13,48},{13,48,13,49},{13,49,13,50},
	{13,50,13,51},{13,51,13,52},{13,52,13,53},{13,53,13,54},{13,54,13,55},
	{13,55,13,56},{13,56,13,57},{13,57,13,58},{13,58,13,59},{13,59,14, 0},
	{14, 0,14, 1},{14, 1,14, 2},{14, 2,14, 3},{14, 3,14, 4},{14, 4,14, 5},
	{14, 5,14, 6},{14, 6,14, 7},{14, 7,14, 8},{14, 8,14, 9},{14, 9,14,10},
	{14,10,14,11},{14,11,14,12},{14,12,14,13},{14,13,14,14},{14,14,14,15},
	{14,15,14,16},{14,16,14,17},{14,17,14,18},{14,18,14,19},{14,19,14,20},
	{14,20,14,21},{14,21,14,22},{14,22,14,23},{14,23,14,24},{14,24,14,25},
	{14,25,14,26},{14,26,14,27},{14,27,14,28},{14,28,14,29},{14,29,14,30},
	{14,30,14,31},{14,31,14,32},{14,32,14,33},{14,33,14,34},{14,34,14,35},
	{14,35,14,36},{14,36,14,37},{14,37,14,38},{14,38,14,39},{14,39,14,40},
	{14,40,14,41},{14,41,14,42},{14,42,14,43},{14,43,14,44},{14,44,14,45},
	{14,45,14,46},{14,46,14,47},{14,47,14,48},{14,48,14,49},{14,49,14,50},
	{14,50,14,51},{14,51,14,52},{14,52,14,53},{14,53,14,54},{14,54,14,55},
	{14,55,14,56},{14,56,14,57},{14,57,14,58},{14,58,14,59},{14,59,15,00},
	{15,00,15,10}
};

KTIMETABLE KTTableF5[48]=
{
	{ 9,25, 9,35},
	{ 9,35, 9,40},{ 9,40, 9,45},{ 9,45, 9,50},{ 9,50, 9,55},{ 9,55,10, 0},
	{10, 0,10, 5},{10, 5,10,10},{10,10,10,15},{10,15,10,20},{10,20,10,25},
	{10,25,10,30},{10,30,10,35},{10,35,10,40},{10,40,10,45},{10,45,10,50},
	{10,50,10,55},{10,55,11, 0},{11, 0,11, 5},{11, 5,11,10},{11,10,11,15},
	{11,15,11,20},{11,20,11,25},{11,25,11,40},

	{13, 0,13, 5},{13, 5,13,10},{13,10,13,15},{13,15,13,20},
	{13,20,13,25},{13,25,13,30},{13,30,13,35},{13,35,13,40},{13,40,13,45},
	{13,45,13,50},{13,50,13,55},{13,55,14, 0},{14, 0,14, 5},{14, 5,14,10},
	{14,10,14,15},{14,15,14,20},{14,20,14,25},{14,25,14,30},{14,30,14,35},
	{14,35,14,40},{14,40,14,45},{14,45,14,50},{14,50,14,55},{14,55,15, 10}
};


KTIMETABLE KTTableF15[16]=
{
	{ 9,25, 9,45},
	{ 9,45,10, 0},{10, 0,10,15},{10,15,10,30},{10,30,10,45},{10,45,11, 0},
	{11, 0,11,15},{11,15,11,40},

	{13, 0,13,15},{13,15,13,30},
	{13,30,13,45},{13,45,14, 0},{14, 0,14,15},{14,15,14,30},{14,30,14,45},
	{14,45,15, 10}
};

KTIMETABLE KTTableF30[8]=
{	
	{ 9,25, 10,00},
	{10, 0,10,30},
	{10,30,11, 0}, {11, 0,11,40},
	{13, 0,13,30}, {13,30,14, 0}, {14, 0,14,30},
	{14,30,15, 10}
};

KTIMETABLE KTTableF60[4]=
{	
	{ 9,25, 10,30},
	{10, 30,11,40},
	{13, 0,14,00}, 
	{14,00,15, 10}
};


BOOL MarketIsOpenedTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	DWORD dwMinute = st.wHour * 60 + st.wMinute;
	
	//允许一定误差
	if(dwMinute >= (9 * 60 + 25)  && dwMinute < (11 * 60 + 40) )
		return TRUE;
	
	//允许一定误差
	if(dwMinute >= (13 * 60 + 00)  && dwMinute < (15 * 60 + 10) )
		return TRUE;
	
	return FALSE;
}





BOOL CheckLabel(const char * lpszLabel)
{
	//000001
	if(lpszLabel[0]=='0' && lpszLabel[1]=='0')
		return TRUE;
	//300000
	if(lpszLabel[0]=='3' && lpszLabel[1]=='0')
		return TRUE;
	//600000 
	if(lpszLabel[0]=='6' && lpszLabel[1]=='0')
		return TRUE;
	//399000 指数 
	if(lpszLabel[0]=='3' && lpszLabel[1]=='9')
		return TRUE;
	//150000 场内基金 
	if(lpszLabel[0]=='1' && lpszLabel[1]=='5' )
		return TRUE;
	//900000 B指
	if(lpszLabel[0]=='9' && lpszLabel[1]=='0' )
		return TRUE;
	return FALSE;
}



void MakeFullLabel(char * szRetFullLabel, const char * szLabel, WORD wMarket)
{
	szRetFullLabel[0] = LOBYTE(wMarket);
	szRetFullLabel[1] = HIBYTE(wMarket);

	if(szRetFullLabel[0] == 0x00) 
		szRetFullLabel[0]='?';

	if(szRetFullLabel[1] == 0x00) 
		szRetFullLabel[1]='?';

	szRetFullLabel[2] = 0x00;
	strcat(szRetFullLabel,szLabel);
}

BOOL CheckF5KLine( vector<KLINEDATA> &  VKLF5)
{
	if(VKLF5.size() != 48)
		return FALSE;

	for (INT i=0; i< sizeof(KTTableF5) / sizeof(KTIMETABLE); i++)
	{
		CTime tCur = VKLF5[i].tUnix;
		if(KTTableF5[i].wEndHour == 11 && KTTableF5[i].wEndMinute == 40)
			VKLF5[i].tUnix  = CTime(tCur.GetYear(),tCur.GetMonth(),tCur.GetDay(),11,30,0).GetTime(); 
		else if(KTTableF5[i].wEndHour == 15 && KTTableF5[i].wEndMinute == 10)
			VKLF5[i].tUnix  = CTime(tCur.GetYear(),tCur.GetMonth(),tCur.GetDay(),15,00,0).GetTime(); 
		else
			VKLF5[i].tUnix  = CTime(tCur.GetYear(),tCur.GetMonth(),tCur.GetDay(),KTTableF5[i].wEndHour,KTTableF5[i].wEndMinute,0).GetTime();  
	}
	return TRUE;
}	

/*
 能处理2种情况下的K线
 
 1 2 3 4 5 6 7 8 9        -- 已存在的旧文件数据
         5 6 7 8 9 10 11  -- 接收到的新数据 
 1 2 3 4 5 6 7 8 9 10 11  -- 处理完的数据

  
  1   2  3  4  5  6  7  8  9              -- 已存在的旧文件数据
  10 11 12 13 14 15 16		              -- 接收到的新数据 
  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  -- 处理完的新数据
*/

INT MergeKLine( vector<KLINEDATA> & VklNew,vector<KLINEDATA> & VklOld)
{
	if(VklNew.size() == 0)
		return 0;

	CTime TNew(VklNew[0].tUnix);
	BOOL bCheck = FALSE;
	for (INT i=0; i < VklOld.size(); i++)
	{
		CTime TOld(VklOld[i].tUnix);
		if(TNew.GetYear() == TOld.GetYear() &&
			TNew.GetMonth() == TOld.GetMonth() &&
			TNew.GetDay() == TOld.GetDay())
		{
			bCheck = TRUE;
			break;
		}
	}

	if(bCheck == FALSE) 
	{
		//跟旧K线没有对比上 则尾部追加
		VklOld.insert(&VklOld[VklOld.size()],VklNew.begin(),VklNew.end());
		return 1;
	}
	
	//跟旧K线对比上了 把旧K线的当前日期之后的全部删除 
	VklOld.erase(&VklOld[i],VklOld.end());
	//插入新K线
	VklOld.insert(&VklOld[i],VklNew.begin(),VklNew.end());
	return 2;
}


/*
typedef struct _KTIMETABLE
{
WORD wBeginHour;
WORD wBeginMinute;
WORD wEndHour;
WORD wEndMinute;
}KTIMETABLE,*LPKTIMETABLE;

{ 9,25, 9,31},
{ 9,31, 9,32},
{11,29,11, 30},
{11,30,11, 40},
{13, 0,13, 1},
{13, 1,13, 2},
{14,59,15,00},
{15,00,15,10}
*/

/*
	设计思路:
	
	当前分笔时间: 
	CTime TRRTime
	DWORD dwTRRMinute = TRRTime.GetHour() * 60  + TRRTime.GetMinute();
	
	起始时间: DWORD dwKTTBegin =  KTT.wBeginHour * 60 + KTT.wBeginMinute;
	结束时间: DWORD dwKTTEnd   =  KTT.wEndHour * 60 + KTT.wEndMinute;

	查找条件:
	1 起点  dwTRRMinute 大于等于 dwKTTBegin 并且 小于等于 dwKTTEnd
	2 终点  dwTRRMinute 大于等于 dwKTTEnd 
	
	没有起点 则没有终点 
	有起点没有终点 则终点等于最后分笔 pTRREnd

 
	返回 0   起点 终点都没查找到
	返回 1   起点 终点都查找到了 
    pTRRBegin 指向下一条分笔  最后会指向 pTRREnd
*/


BOOL FindFenBi(KTIMETABLE  KTT,LPTRADINGRECORD  & pTRRBegin , const LPTRADINGRECORD  pTRREnd ,
			  LPTRADINGRECORD & pTRRFindBegin , LPTRADINGRECORD & pTRRFindEnd)
{
	
	//初始化参数
	pTRRFindBegin=NULL,pTRRFindEnd = NULL;

	CTime TRRTime;
	DWORD dwTRRMinute,dwKTTBegin,dwKTTEnd;
	dwKTTBegin =  KTT.wBeginHour * 60 + KTT.wBeginMinute;
	dwKTTEnd   =  KTT.wEndHour * 60 + KTT.wEndMinute;
	
	//查找起点
	while( pTRRBegin < pTRREnd )
	{
 		TRRTime = pTRRBegin->m_lTime;
		dwTRRMinute = TRRTime.GetHour() * 60  + TRRTime.GetMinute();
// #ifdef _DEBUG
// 		CString szText;
// 		szText.Format("%02u:%02u:%02u",TRRTime.GetHour(),TRRTime.GetMinute(),TRRTime.GetSecond());
// #endif
 		if(dwTRRMinute > dwKTTEnd)
 			break;

		if(dwTRRMinute >= dwKTTBegin)
		{
			pTRRFindBegin = pTRRBegin;
			break;
		}
		pTRRBegin++;
	}

 	if(pTRRFindBegin == NULL)
		return FALSE;

	//查找终点
	pTRRBegin++;
	pTRRFindEnd = pTRRBegin;

	while( pTRRBegin < pTRREnd )
	{
		TRRTime = pTRRBegin->m_lTime;
		dwTRRMinute = TRRTime.GetHour() * 60  + TRRTime.GetMinute();
		
// #ifdef _DEBUG
// 		CString szText;
// 		szText.Format("%02u:%02u:%02u",TRRTime.GetHour(),TRRTime.GetMinute(),TRRTime.GetSecond());
// #endif
		if( dwTRRMinute >= dwKTTEnd)
			break;

		pTRRBegin++;
		pTRRFindEnd = pTRRBegin;
	}
	return TRUE;
}



VOID FenBiToSingleKLine(const LPTRADINGRECORD   pTRR ,  const LPTRADINGRECORD  pTRRFindBegin , const LPTRADINGRECORD  pTRRFindEnd ,KLINEDATA & KLineData)
{
	KLineData.fOpen = pTRRFindBegin->m_fNewPrice;
	KLineData.fHigh = pTRRFindBegin->m_fNewPrice;
	KLineData.fLow  = pTRRFindBegin->m_fNewPrice;

	for (LPTRADINGRECORD pOffset = pTRRFindBegin; pOffset < pTRRFindEnd; pOffset++)
	{
		if(pOffset->m_fNewPrice > KLineData.fHigh)
			KLineData.fHigh = pOffset->m_fNewPrice;
		
		if(pOffset->m_fNewPrice < KLineData.fLow)
			KLineData.fLow = pOffset->m_fNewPrice;
	}

	if(pTRRFindBegin == pTRR)
	{
		KLineData.fAmount = (pTRRFindEnd-1)->m_fAmount;
		KLineData.fVolume = (pTRRFindEnd-1)->m_fVolume;
	}else
	{
		KLineData.fAmount = (pTRRFindEnd-1)->m_fAmount - (pTRRFindBegin-1)->m_fAmount;
		KLineData.fVolume = (pTRRFindEnd-1)->m_fVolume - (pTRRFindBegin-1)->m_fVolume;
	}

	KLineData.tUnix   = (pTRRFindEnd-1)->m_lTime;
	KLineData.fClose  = (pTRRFindEnd-1)->m_fNewPrice;
}


BOOL FenBiToMultiKLine(const RCV_REPORT_STRUCTExV3 * pCurRRS,LPTRADINGRECORD pTRR , INT nTRRSize,const KTIMETABLE * TTable , INT nTTableSize , vector<KLINEDATA> & VKLNew , BOOL bFull/*=FALSE*/)
{
	if(pTRR == NULL || nTRRSize == 0)
		return FALSE;

	KLINEDATA KLineData;
	LPTRADINGRECORD pTRRBegin = pTRR;
	LPTRADINGRECORD pTRREnd   = pTRR + nTRRSize;
	LPTRADINGRECORD pTRRFindBegin=NULL,pTRRFindEnd = NULL;
	for (INT i = 0; i< nTTableSize; i++)
	{
		if(FindFenBi(TTable[i],pTRRBegin,pTRREnd,pTRRFindBegin,pTRRFindEnd))
		{
#ifdef _DEBUG
			CTime TRRTime;
			CString szFindBegin;
			TRRTime= pTRRFindBegin->m_lTime;
			szFindBegin.Format("[%3d] %02u:%02u:%02u",i,TRRTime.GetHour(),TRRTime.GetMinute(),TRRTime.GetSecond());
			OutputDebugString(szFindBegin);

			CString szFindEnd;
			TRRTime= (pTRRFindEnd-1)->m_lTime;
			szFindEnd.Format("[%3d] %02u:%02u:%02u",i,TRRTime.GetHour(),TRRTime.GetMinute(),TRRTime.GetSecond());
			OutputDebugString(szFindEnd);
#endif
			FenBiToSingleKLine(pTRR,pTRRFindBegin,pTRRFindEnd,KLineData);

			VKLNew.push_back(KLineData);
		}else 
		{
			if(VKLNew.size())
			{
				KLINEDATA *pLastKLineData = &VKLNew[ VKLNew.size() - 1]; 
				KLineData.fOpen   = pLastKLineData->fClose;
				KLineData.fClose  = pLastKLineData->fClose;
				KLineData.fHigh   = pLastKLineData->fClose;
				KLineData.fLow    = pLastKLineData->fClose;
				KLineData.fAmount = 0;
				KLineData.fVolume = 0;
				CTime tLast = pLastKLineData->tUnix;
				KLineData.tUnix   = CTime(tLast.GetYear(),tLast.GetMonth(),tLast.GetDay(),TTable[i].wEndHour,TTable[i].wEndMinute,0).GetTime();
				VKLNew.push_back(KLineData);
			}else if(pCurRRS)
			{
				KLineData.fOpen   = pCurRRS->m_fLastClose;
				KLineData.fClose  = pCurRRS->m_fLastClose;
				KLineData.fHigh   = pCurRRS->m_fLastClose;
				KLineData.fLow    = pCurRRS->m_fLastClose;
				KLineData.fAmount = 0;
				KLineData.fVolume = 0;
				CTime tLast = pCurRRS->m_time;
				KLineData.tUnix   = CTime(tLast.GetYear(),tLast.GetMonth(),tLast.GetDay(),TTable[i].wEndHour,TTable[i].wEndMinute,0).GetTime();
				VKLNew.push_back(KLineData);
			}
		}

		if(pTRRFindEnd == pTRREnd && bFull == FALSE)
			break;
	}
	return VKLNew.size() ? TRUE : FALSE;
}



BOOL SmallKL_To_BigKL(vector<KLINEDATA> & VSmallKL , vector<KLINEDATA> & VBigKL,INT nMultiple)
{
	if(VSmallKL.size() == 0)
		return FALSE;

	VBigKL.reserve(VSmallKL.size() / nMultiple  + 1);

	LPKLINEDATA pBegin = VSmallKL.begin();
	LPKLINEDATA pEnd   = VSmallKL.end();

	KLINEDATA KLineData;
	INT  nCount = 0;

	for (pBegin; pBegin != pEnd; pBegin++)
	{
		if(nCount == 0)
		{
			KLineData.fOpen = pBegin->fOpen;
			KLineData.fHigh = pBegin->fHigh;
			KLineData.fLow  = pBegin->fLow;
			KLineData.fVolume = 0.00f;
			KLineData.fAmount = 0.00f;
		}

		if(pBegin->fHigh > KLineData.fHigh)
			KLineData.fHigh = pBegin->fHigh;

		if(pBegin->fLow < KLineData.fLow)
			KLineData.fLow = pBegin->fLow;
		
		KLineData.tUnix  = pBegin->tUnix;
		KLineData.fClose = pBegin->fClose;
		KLineData.fVolume += pBegin->fVolume;
		KLineData.fAmount += pBegin->fAmount;
		
		nCount++;
		
		if(nCount == nMultiple)
		{
			nCount = 0;
			VBigKL.push_back(KLineData);
			continue;
		}
	}
	if(nCount != 0)
		VBigKL.push_back(KLineData);

 	return VBigKL.size() ? TRUE:FALSE;
}


BOOL DayKL_To_MonthKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VMonthKLine)
{
	if(VDayKLine.size() == 0)
		return FALSE;
	
	VMonthKLine.reserve(VDayKLine.size() / 31  + 1);
	
	LPKLINEDATA pBegin = VDayKLine.begin();
	LPKLINEDATA pEnd   = VDayKLine.end();

	KLINEDATA KLineData;
	KLineData.fOpen = pBegin->fOpen;
	KLineData.fHigh = pBegin->fHigh;
	KLineData.fLow  = pBegin->fLow;
	KLineData.fVolume = 0.00f;
	KLineData.fAmount = 0.00f;

	CTime tCurrent  = pBegin->tUnix;
 	CTime tBefore   = pBegin->tUnix;

	for (pBegin; pBegin != pEnd; pBegin++)
	{
		if(tCurrent != tBefore)
		   tBefore  = tCurrent;

		tCurrent = pBegin->tUnix;
		if(tCurrent.GetMonth() != tBefore.GetMonth())
		{
			VMonthKLine.push_back(KLineData);
			KLineData.fOpen = pBegin->fOpen;
			KLineData.fHigh = pBegin->fHigh;
			KLineData.fLow  = pBegin->fLow;
			KLineData.fVolume = 0.00f;
			KLineData.fAmount = 0.00f;
		}

		if(pBegin->fHigh > KLineData.fHigh)
			KLineData.fHigh = pBegin->fHigh;
		
		if(pBegin->fLow < KLineData.fLow)
			KLineData.fLow = pBegin->fLow;
		
		KLineData.tUnix  = pBegin->tUnix;
		KLineData.fClose = pBegin->fClose;
		KLineData.fVolume += pBegin->fVolume;
		KLineData.fAmount += pBegin->fAmount;
	}

	if(KLineData.fVolume != 0.00f )
		VMonthKLine.push_back(KLineData);

 	return VMonthKLine.size() ? TRUE:FALSE;
}


BOOL DayKL_To_YearKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VYearKLine)
{
	if(VDayKLine.size() == 0)
		return FALSE;
	
	VYearKLine.reserve(VDayKLine.size() / 365  + 1);
	
	LPKLINEDATA pBegin = VDayKLine.begin();
	LPKLINEDATA pEnd   = VDayKLine.end();
	
	KLINEDATA KLineData;
	KLineData.fOpen = pBegin->fOpen;
	KLineData.fHigh = pBegin->fHigh;
	KLineData.fLow  = pBegin->fLow;
	KLineData.fVolume = 0.00f;
	KLineData.fAmount = 0.00f;
	
	CTime tCurrent  = pBegin->tUnix;
	CTime tBefore   = pBegin->tUnix;
	
	for (pBegin; pBegin != pEnd; pBegin++)
	{
		if(tCurrent != tBefore)
			tBefore  = tCurrent;
		
		tCurrent = pBegin->tUnix;
		if(tCurrent.GetYear() != tBefore.GetYear())
		{
			VYearKLine.push_back(KLineData);
			KLineData.fOpen = pBegin->fOpen;
			KLineData.fHigh = pBegin->fHigh;
			KLineData.fLow  = pBegin->fLow;
			KLineData.fVolume = 0.00f;
			KLineData.fAmount = 0.00f;
		}
		
		if(pBegin->fHigh > KLineData.fHigh)
			KLineData.fHigh = pBegin->fHigh;
		
		if(pBegin->fLow < KLineData.fLow)
			KLineData.fLow = pBegin->fLow;
		
		KLineData.tUnix  = pBegin->tUnix;
		KLineData.fClose = pBegin->fClose;
		KLineData.fVolume += pBegin->fVolume;
		KLineData.fAmount += pBegin->fAmount;
	}
	
	if(KLineData.fVolume != 0.00f )
		VYearKLine.push_back(KLineData);
	return VYearKLine.size() ? TRUE:FALSE;
}


BOOL CheckWeek(CTime & tCurrent ,CTime & tBefore)
{
	CTimeSpan  ts = tCurrent - tBefore;
	INT nDays = ts.GetDays();
	if(nDays  <= 1)
		return FALSE;

	CTime tNext = tBefore;
	for (INT n=0; n< nDays ; n++)
	{
		tNext +=CTimeSpan(1,0,0,0);
		//不连续的日线 包含一个星期六
		if(tNext.GetDayOfWeek() == 7)
			return TRUE;
	}
	return FALSE;
}


BOOL DayKL_To_WeekKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VWeekKLine)
{
	if(VDayKLine.size() == 0)
		return FALSE;
	
	VWeekKLine.reserve(VDayKLine.size() / 5  + 1);
	
	LPKLINEDATA pBegin = VDayKLine.begin();
	LPKLINEDATA pEnd   = VDayKLine.end();
	
	KLINEDATA KLineData;
	KLineData.fOpen = pBegin->fOpen;
	KLineData.fHigh = pBegin->fHigh;
	KLineData.fLow  = pBegin->fLow;
	KLineData.fVolume = 0.00f;
	KLineData.fAmount = 0.00f;
	
	CTime tCurrent  = pBegin->tUnix;
	CTime tBefore   = pBegin->tUnix;
	
	for (pBegin; pBegin != pEnd; pBegin++)
	{
		if(tCurrent != tBefore)
			tBefore  = tCurrent;

		tCurrent = pBegin->tUnix;
		if(CheckWeek(tCurrent,tBefore))
		{
			VWeekKLine.push_back(KLineData);
			KLineData.fOpen = pBegin->fOpen;
			KLineData.fHigh = pBegin->fHigh;
			KLineData.fLow  = pBegin->fLow;
			KLineData.fVolume = 0.00f;
			KLineData.fAmount = 0.00f;
		}
		
		if(pBegin->fHigh > KLineData.fHigh)
			KLineData.fHigh = pBegin->fHigh;
		
		if(pBegin->fLow < KLineData.fLow)
			KLineData.fLow = pBegin->fLow;
		
		KLineData.tUnix  = pBegin->tUnix;
		KLineData.fClose = pBegin->fClose;
		KLineData.fVolume += pBegin->fVolume;
		KLineData.fAmount += pBegin->fAmount;
	}
	
	if(KLineData.fVolume != 0.00f )
		VWeekKLine.push_back(KLineData);

	return VWeekKLine.size() ? TRUE:FALSE;
}

BOOL CheckQuarter(CTime & tCurrent ,CTime & tBefore)
{
	// 第一季度：1月－3月
	// 第二季度：4月－6月
	// 第三季度：7月－9月
	// 第四季度：10月－12月
	INT  nCurrentQuarter,nBeforeQuarter;
	switch(tCurrent.GetMonth())
	{
	case 1:
	case 2:
	case 3:
		nCurrentQuarter = 1;
		break;
	case 4:
	case 5:
	case 6:
		nCurrentQuarter = 2;
		break;
	case 7:
	case 8:
	case 9:
		nCurrentQuarter = 3;
		break;
	case 10:
	case 11:
	case 12:
		nCurrentQuarter = 4;
		break;
	}

	switch(tBefore.GetMonth())
	{
	case 1:
	case 2:
	case 3:
		nBeforeQuarter = 1;
		break;
	case 4:
	case 5:
	case 6:
		nBeforeQuarter = 2;
		break;
	case 7:
	case 8:
	case 9:
		nBeforeQuarter = 3;
		break;
	case 10:
	case 11:
	case 12:
		nBeforeQuarter = 4;
		break;
	}

   if(nCurrentQuarter != nBeforeQuarter)
	   return TRUE;

	return FALSE;
}

BOOL DayKL_To_QuarterKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VQuarterKLine)
{
	if(VDayKLine.size() == 0)
		return FALSE;
	
	VQuarterKLine.reserve(VDayKLine.size() / 100  + 1);
	
	LPKLINEDATA pBegin = VDayKLine.begin();
	LPKLINEDATA pEnd   = VDayKLine.end();
	
	KLINEDATA KLineData;
	KLineData.fOpen = pBegin->fOpen;
	KLineData.fHigh = pBegin->fHigh;
	KLineData.fLow  = pBegin->fLow;
	KLineData.fVolume = 0.00f;
	KLineData.fAmount = 0.00f;
	
	CTime tCurrent  = pBegin->tUnix;
	CTime tBefore   = pBegin->tUnix;
	
	for (pBegin; pBegin != pEnd; pBegin++)
	{
		if(tCurrent != tBefore)
			tBefore  = tCurrent;
		
		tCurrent = pBegin->tUnix;
		if(CheckQuarter(tCurrent,tBefore))
		{
			VQuarterKLine.push_back(KLineData);
			KLineData.fOpen = pBegin->fOpen;
			KLineData.fHigh = pBegin->fHigh;
			KLineData.fLow  = pBegin->fLow;
			KLineData.fVolume = 0.00f;
			KLineData.fAmount = 0.00f;
		}
		
		if(pBegin->fHigh > KLineData.fHigh)
			KLineData.fHigh = pBegin->fHigh;
		
		if(pBegin->fLow < KLineData.fLow)
			KLineData.fLow = pBegin->fLow;
		
		KLineData.tUnix  = pBegin->tUnix;
		KLineData.fClose = pBegin->fClose;
		KLineData.fVolume += pBegin->fVolume;
		KLineData.fAmount += pBegin->fAmount;
	}
	
	if(KLineData.fVolume != 0.00f )
		VQuarterKLine.push_back(KLineData);
	
	return VQuarterKLine.size() ? TRUE:FALSE;
}




BOOL CheckTradingRecord(const LPTRADINGRECORD pTRR ,vector<KLINEDATA> &  vkl )
{
	if(vkl.size() == 0)
		return TRUE;
	if(pTRR == NULL)
		return FALSE;
	
	CTime tCheckTTR = pTRR->m_lTime;
	CTime tCheckVKL = vkl[vkl.size() - 1].tUnix;
	if(tCheckTTR.GetDay() == tCheckVKL.GetDay() )
		return FALSE;
	return TRUE;
}


BOOL MakeFullDayKLine(vector<KLINEDATA> &  vkl ,const RCV_REPORT_STRUCTExV3 * pRRS)
{
	if(pRRS == NULL || Amount_Equality(pRRS->m_fAmount,0) || Price_Equality(pRRS->m_fOpen,0.00f))
		return FALSE;
	
	BOOL bCheck = FALSE;
	
	do 
	{
		if(vkl.size() == NULL){
			bCheck = TRUE;
			break;
		}
		
		CTime tVKL=vkl[vkl.size() - 1].tUnix;
		CTime tRTQ= pRRS->m_time;
		if(tVKL.GetDay() != tRTQ.GetDay() ){
			bCheck = TRUE;
			break;
		}
	} while (FALSE);
	
	if(bCheck)
	{
		KLINEDATA KLineData;
		KLineData.tUnix		= pRRS->m_time;
		KLineData.fOpen		= pRRS->m_fOpen;
		KLineData.fClose	= pRRS->m_fNewPrice;
		KLineData.fHigh		= pRRS->m_fHigh;
		KLineData.fLow		= pRRS->m_fLow;
		KLineData.fAmount	= pRRS->m_fAmount;
		KLineData.fVolume	= pRRS->m_fVolume;
		vkl.push_back(KLineData);
	}

	return bCheck;
}

BOOL IsStock( const char * pszLabel )
{
	if(pszLabel[0] == 'S' && pszLabel[1] == 'H')
	{
		switch(pszLabel[2])
		{
		case '6':
			return TRUE;
		case '9':
			return TRUE;
		}
	}

	if(pszLabel[0] == 'S' && pszLabel[1] == 'Z')
	{
		if(pszLabel[2] == '0' && pszLabel[3] == '0')
			return TRUE;
		
		if(pszLabel[2] == '3' && pszLabel[3] == '0')
			return TRUE;
	}
	return FALSE;	
}





/*
void VKLineToSingleKLine(vector < KLINEDATA>  & VKL , KLINEDATA & KLineData)
{
	double dVolume=0.0f;        //成交量
	double dAmount=0.0f;		//金额	
	
	float fHigh = VKL[0].fHigh;
	float fLow  = VKL[0].fLow;
	for (INT i = 0; i< VKL.size(); i++)
	{
		dVolume += VKL[i].fVolume;
		dAmount += VKL[i].fAmount;
		
		if(VKL[i].fHigh > fHigh)
			fHigh = VKL[i].fHigh ;
		
		if(VKL[i].fLow < fLow)
			fLow = VKL[i].fLow ;
	}
	
	KLineData.fAmount = dAmount;
	KLineData.fVolume = dVolume;
	KLineData.fHigh = fHigh;
	KLineData.fLow  = fLow;
	KLineData.fOpen = VKL[0].fOpen;
	KLineData.fClose = VKL[VKL.size() - 1].fClose;
	KLineData.tUnix = VKL[VKL.size() - 1].tUnix;
}


BOOL SmallKL_To_BigKL(vector<KLINEDATA> & VSmallKL , vector<KLINEDATA> & VBigKL,INT nMultiple)
{
	vector<KLINEDATA> temp;
	temp.reserve(nMultiple);
	VBigKL.reserve(VSmallKL.size() / nMultiple  + 1);

	for (INT i=0; i< VSmallKL.size(); i++)
	{
		temp.push_back(VSmallKL[i]);
		if( temp.size() == nMultiple )
		{
			KLINEDATA mkl;
			VKLineToSingleKLine(temp,mkl);
			VBigKL.push_back(mkl);
			temp.clear();
		}
	}
	if(temp.size())
	{
		KLINEDATA mkl;
		VKLineToSingleKLine(temp,mkl);
		VBigKL.push_back(mkl);
	}
	return VBigKL.size() ? TRUE:FALSE;
}


BOOL DayKL_To_WeekKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VWeekKLine)
{
	INT nOffset = 0;
	
	CTime TFirst(VDayKLine[0].tUnix);

	//6 代表星期五 星期一 等于 2
	if(6 == TFirst.GetDayOfWeek())
	{
		VWeekKLine.push_back(VDayKLine[0]);
		nOffset++;
	}
	
	vector<KLINEDATA> temp;
	for (INT i = nOffset; i< VDayKLine.size() - 1; i++)
	{
		CTime TBegin(VDayKLine[i].tUnix);
		CTime TEnd(VDayKLine[i + 1].tUnix);
		CTimeSpan  ts = TEnd - TBegin;
		
		if(ts.GetDays() == 1 )
		{
			temp.push_back(VDayKLine[i]);
			continue;
		}
		
		CTime TCur(VDayKLine[i].tUnix);
		if(6 == TCur.GetDayOfWeek() )
		{
			temp.push_back(VDayKLine[i]);
			KLINEDATA mkl;
			VKLineToSingleKLine(temp,mkl);
			VWeekKLine.push_back(mkl);
			temp.clear();
		}else
		{
			for (INT n=0; n< ts.GetDays(); n++)
			{
				TBegin+=CTimeSpan(1,0,0,0);
				//不连续的日线 包含一个周五
				if(TBegin.GetDayOfWeek() == 6)
				{
					temp.push_back(VDayKLine[i]);
					KLINEDATA mkl;
					VKLineToSingleKLine(temp,mkl);
					VWeekKLine.push_back(mkl);
					temp.clear();
					break;
				}
			}	
		}
	}

	if(temp.size())
	{
		temp.push_back(VDayKLine[i]);
		KLINEDATA mkl;
		VKLineToSingleKLine(temp,mkl);
		VWeekKLine.push_back(mkl);
	}
	return TRUE;
}





BOOL DayKL_To_YearKL(vector<KLINEDATA> & VDayKLine,vector<KLINEDATA> & VYearKLine)
{
	vector<KLINEDATA> temp;
	for (INT i = 0; i< VDayKLine.size() - 1; i++)
	{
		temp.push_back(VDayKLine[i]);
		CTime TCur (   VDayKLine[i].tUnix   );
		CTime TNext(  VDayKLine[i + 1].tUnix);
		
		if(TCur.GetMonth() !=  TNext.GetMonth())
		{
			KLINEDATA mkl;
			VKLineToSingleKLine(temp,mkl);
			VYearKLine.push_back(mkl);
			temp.clear();
		}
	}
	
	if(temp.size())
	{
		temp.push_back(VDayKLine[i]);
		KLINEDATA mkl;
		VKLineToSingleKLine(temp,mkl);
		VYearKLine.push_back(mkl);
	}
	return TRUE;
}


BOOL CheckLabel(const char * lpszLabel)
{
	//000001
	if(lpszLabel[0]=='0' && lpszLabel[1]=='0')
		return TRUE;

	//300000
	if(lpszLabel[0]=='3' && lpszLabel[1]=='0')
		return TRUE;

	//600000 
	if(lpszLabel[0]=='6' && lpszLabel[1]=='0')
		return TRUE;

	//399000 指数 
	if(lpszLabel[0]=='3' && lpszLabel[1]=='9')
		return TRUE;

	//150000 场内基金 
	if(lpszLabel[0]=='1' && lpszLabel[1]=='5' )
		return TRUE;

	//900000 B指
	if(lpszLabel[0]=='9' && lpszLabel[1]=='0' )
		return TRUE;

	return FALSE;
}

void MakeFullLabel(char * szRetFullLabel, const char * szLabel, WORD wSCType)
{
	szRetFullLabel[0] = LOBYTE(wSCType);
	szRetFullLabel[1] = HIBYTE(wSCType);
	if(szRetFullLabel[0] == 0x00) 
		szRetFullLabel[0]='?';
	if(szRetFullLabel[1] == 0x00) 
		szRetFullLabel[1]='?';
	szRetFullLabel[2] = 0x00;
	strcat(szRetFullLabel,szLabel);
}

INT StockCheckTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	DWORD dwMinute = st.wHour * 60 + st.wMinute;

	if(dwMinute >= (9 * 60 + 15)  && dwMinute < (9 * 60 + 20) )
		return SCT_0915_0920;

	if(dwMinute >= (9 * 60 + 20)  && dwMinute < (9 * 60 + 25) )
		return SCT_0920_0925;

	if(dwMinute >= (9 * 60 + 25)  && dwMinute < (9 * 60 + 30) )
		return SCT_0925_0930;

	//允许一定误差
	if(dwMinute >= (9 * 60 + 30)  && dwMinute < (11 * 60 + 40) )
		return SCT_0930_1130;

	//允许一定误差
	if(dwMinute >= (13 * 60 + 00)  && dwMinute < (15 * 60 + 10) )
		return SCT_1300_1500;

	return SCT_Other;
}


BOOL MarketIsOpenedTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	DWORD dwMinute = st.wHour * 60 + st.wMinute;

	//允许一定误差
	if(dwMinute >= (9 * 60 + 25)  && dwMinute < (11 * 60 + 40) )
		return TRUE;
	
	//允许一定误差
	if(dwMinute >= (13 * 60 + 00)  && dwMinute < (15 * 60 + 10) )
		return TRUE;

	return FALSE;
}*/

