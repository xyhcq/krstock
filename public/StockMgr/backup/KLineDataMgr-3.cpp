#include "StdAfx.h"
#include "KLineDataMgr.h"
#include <algorithm>

CKLineDataMgr::CKLineDataMgr()
{
	m_DirMgr.Init("KLineData");
	m_LoadKLineBuffer = new KLINEDATA[LOAD_BUFFER_SIZE];
}


void CKLineDataMgr::Destory()
{
	if(m_LoadKLineBuffer) 
		delete [] m_LoadKLineBuffer;
}


BOOL CKLineDataMgr::LoadFile( VKL & vkl , LPCTSTR szLabel,LPCTSTR lpszFileName )
{
	DWORD dwSize  = sizeof(KLINEDATA) * LOAD_BUFFER_SIZE;
	if(m_DirMgr.LoadFile(szLabel,lpszFileName,m_LoadKLineBuffer,dwSize))
	{
		//校验数据
		if(dwSize == 0 || (INT)dwSize < 0  || dwSize % sizeof(KLINEDATA) != 0 || dwSize > sizeof(KLINEDATA) * LOAD_BUFFER_SIZE )
			return FALSE;

		vkl.assign(&m_LoadKLineBuffer[0],&m_LoadKLineBuffer[ dwSize / sizeof(KLINEDATA) ]);

		return TRUE;
	}
	return FALSE;
}


BOOL CKLineDataMgr::SaveFile( VKL & vkl , LPCTSTR szLabel,LPCTSTR lpszFileName )
{
	DWORD dwSize = vkl.size() * sizeof(KLINEDATA);
	return m_DirMgr.SaveFile(szLabel,lpszFileName,&vkl[0] ,dwSize);	
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

INT CKLineDataMgr::MergeKLine( VKL & VklNew,VKL & VklOld)
{
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



static bool  Sort_Proc(const KLINEDATA & K1, const KLINEDATA & K2) 
{
	return K1.tUnix < K2.tUnix;
}

static time_t GetNextF5Time(time_t t)
{
	CTime  TCur = t;
	if(TCur.GetHour() == 11 && TCur.GetMinute() == 30 )
	{
		CTime TNext(TCur.GetYear(),TCur.GetMonth(),TCur.GetDay(),13,5,0);
		return TNext.GetTime();
	}
	if(TCur.GetHour() == 15 && TCur.GetMinute() == 00 )
	{
		CTime TNext(TCur.GetYear(),TCur.GetMonth(),TCur.GetDay(),9,35,0);
		return TNext.GetTime();
	}
	CTime TNext= TCur;
	TNext+= CTimeSpan(0,0,5,0);
	return TNext.GetTime();
}



// 返回 -1  文件系统失败  
// 返回 0   文件新建
// 返回 1   与旧的K线存在相同的 合并处理
// 返回 2   与旧的K线不存在相同 尾部追加
INT CKLineDataMgr::SaveDayKLine( VKL & VklDay_New,LPCTSTR szLabel )
{
	//按时间排序 
	sort(VklDay_New.begin(),VklDay_New.end(),Sort_Proc);

	//非法数据
	vector<KLINEDATA>::iterator it;
	for (it = VklDay_New.begin(); it!= VklDay_New.end(); ++it)
	{
		if( it+1 != VklDay_New.end())
		{
			if(it->tUnix == (it+1)->tUnix)
				it = VklDay_New.erase(it);
		}
		if(it == VklDay_New.end())
			break;
	}

	vector<KLINEDATA> VklDay_Old;
	if(!LoadFile(VklDay_Old,szLabel,"Day.dat"))
		return SaveFile(VklDay_New,szLabel,"Day.dat") ? 0 : -1;

	//1 表示尾部追加   0 表示跟旧数据存在一个合并过程
	INT nResult = MergeKLine(VklDay_New,VklDay_Old);

	VklDay_New = VklDay_Old;
	return SaveFile(VklDay_Old,szLabel,"Day.dat") ?  nResult: - 1;
}

INT CKLineDataMgr::SaveF5KLine( VKL & VklF5_New , LPCTSTR szLabel )
{
	//按时间排序 
	sort(VklF5_New.begin(),VklF5_New.end(),Sort_Proc);
	
	//非法数据
	vector<KLINEDATA>::iterator it;
	for (it = VklF5_New.begin(); it!= VklF5_New.end(); ++it)
	{
		if( it+1 != VklF5_New.end())
		{
			if(it->tUnix == (it+1)->tUnix)
				it = VklF5_New.erase(it);
		}
		if(it == VklF5_New.end())
			break;
	}
	
	//5F 必须保证其连续性 主要修复熔断时出现的问题 
	for (INT i=0; i< VklF5_New.size() - 1 ; i++)
	{
		CTime Temp = GetNextF5Time(VklF5_New[i].tUnix);
		CTime TNext(VklF5_New[i+1].tUnix);
		if(TNext.GetHour() != Temp.GetHour() || TNext.GetMinute() != Temp.GetMinute())
		{
			KLINEDATA KLD;
			KLD.fVolume = 0;
			KLD.fAmount = 0;
			KLD.fOpen   = VklF5_New[i].fClose;
			KLD.fClose  = VklF5_New[i].fClose;
			KLD.fHigh   = VklF5_New[i].fClose;
			KLD.fLow    = VklF5_New[i].fClose;
			KLD.tUnix   = Temp.GetTime();
			VklF5_New.insert( &VklF5_New[i+1],KLD);
		}
	}
	
	vector<KLINEDATA> VklF5_Old;
	if(!LoadFile(VklF5_Old,szLabel,"F5.dat"))
		return SaveFile(VklF5_New,szLabel,"F5.dat") ? 0 : -1;

	INT nResult = MergeKLine(VklF5_New,VklF5_Old);

	VklF5_New = VklF5_Old;

	return SaveFile(VklF5_Old,szLabel,"F5.dat") ?  nResult: - 1;
}



INT CKLineDataMgr::SaveF1KLine( VKL & VklF1_New,LPCTSTR szLabel )
{
	//按时间排序 
	sort(VklF1_New.begin(),VklF1_New.end(),Sort_Proc);
	//非法数据
	vector<KLINEDATA>::iterator it;
	for (it = VklF1_New.begin(); it!= VklF1_New.end(); ++it)
	{
		if( it+1 != VklF1_New.end())
		{
			if(it->tUnix == (it+1)->tUnix)
				it = VklF1_New.erase(it);
		}
		if(it == VklF1_New.end())
			break;
	}
	
	vector<KLINEDATA> VklF1_Old;
	if(!LoadFile(VklF1_Old,szLabel,"F1.dat"))
		return SaveFile(VklF1_New,szLabel,"F1.dat") ? 0 : -1;
	
	INT nResult = MergeKLine(VklF1_New,VklF1_Old);
	VklF1_New = VklF1_Old;
	return SaveFile(VklF1_Old,szLabel,"F1.dat") ?  nResult: - 1;
}

BOOL CKLineDataMgr::Select( VKL & vkl,LPCTSTR szLabel,LPCTSTR lpszFileName )
{
	return LoadFile(vkl,szLabel,lpszFileName);
}
