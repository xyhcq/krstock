#include "StdAfx.h"
#include "KLineDataMgr.h"
#include <algorithm>

CKLineDataMgr::CKLineDataMgr()
{
	m_DirMgr.Init("KLineData");
	m_LoadKLineBuffer = new KLINEDATA[LOAD_BUFFER_SIZE];
}


BOOL CKLineDataMgr::Init()
{
	INT nLoadCount = 0;
	INT nDirCount = m_DirMgr.Size();
	
	CExtenLogDlg ExtenLogDlg;
	ExtenLogDlg.InsertLog("KLineDataMgr::Init SDir.size() = %d ",nDirCount);
	
	set<string> SDir = m_DirMgr.Get();
	for (set<string>::iterator  SDirIt = SDir.begin(); SDirIt != SDir.end(); ++SDirIt)
	{
		string s = *SDirIt;

/////////////////////////////BUG修复//////////////////////////////////////
// 		if(!CheckLabel(s.c_str() + 2))
// 		{
// 			static BOOL bFirst = TRUE;
// 			m_DirMgr.DeleteFile(s.c_str(),"F1.dat");
// 			m_DirMgr.DeleteFile(s.c_str(),"F5.dat");
// 			m_DirMgr.DeleteFile(s.c_str(),"Day.dat");
// 			m_DirMgr.DeleteDir(s.c_str());
// 			
// 			if(bFirst)
// 				MessageBox(NULL,s.c_str(),"目录被删除！",MB_OK);
// 			bFirst = FALSE;
// 			continue;
// 		}
///////////////////////////////////////////////////////////////////	
			
		VKL Vkl;
		pair< map< string, VKL >::iterator, bool> ip= m_DayKLine.insert( pair< string, VKL >(s.c_str(),Vkl) );
		if(ip.second == false)
			continue;
		
		LoadFile(ip.first->second,s.c_str(),"Day.dat");
		ExtenLogDlg.InsertLog("KLineDataMgr::LoadKLine %s 总:%d 剩余:%d",s.c_str(),nDirCount,nDirCount - nLoadCount++);
	}

	return m_DayKLine.size() ? TRUE : FALSE;
}

INT CKLineDataMgr::GetSize()
{
	return m_DayKLine.size();
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


// 返回 -1  文件系统失败  
// 返回 0   文件新建
// 返回 1   与旧的K线存在相同的 合并处理
// 返回 2   与旧的K线不存在相同 尾部追加
INT CKLineDataMgr::MakeDayKLine( VKL & VklDay_New,LPCTSTR szLabel )
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

//	SaveFile(VklDay_New,szLabel,"Day.dat");
	return 0;
}


typedef struct _MINUTETABLE
{
	WORD wHour;
	WORD wMinute;
}MINUTETABLE,*LPMINUTETABLE;

static MINUTETABLE F5Table[48]={
	{ 9,35},{ 9,40},{ 9,45},{ 9,50},{ 9,55},{10, 0},
	{10, 5},{10,10},{10,15},{10,20},{10,25},{10,30},
	{10,35},{10,40},{10,45},{10,50},{10,55},{11, 0},
	{11, 5},{11,10},{11,15},{11,20},{11,25},{11,30},
	{13, 5},{13,10},{13,15},{13,20},{13,25},{13,30},
	{13,35},{13,40},{13,45},{13,50},{13,55},{14, 0},
	{14, 5},{14,10},{14,15},{14,20},{14,25},{14,30},
	{14,35},{14,40},{14,45},{14,50},{14,55},{15, 0}
};


static BOOL CheckF5KLine(VKL & VklF5_New)
{
	if(VklF5_New.size() == 0)
		return FALSE;
	
	CTime tKL;

	//必须是9:35开始的 
	INT nBegin = 0;
	vector<KLINEDATA>::iterator it;
	for (it = VklF5_New.begin(); it!= VklF5_New.end(); ++it)
	{
		tKL = it->tUnix;
		if(tKL.GetHour() == F5Table[0].wHour && tKL.GetMinute() == F5Table[0].wMinute)
			break;
		nBegin++;
	}
	if(nBegin != 0)  VklF5_New.erase(&VklF5_New[0], &VklF5_New[nBegin]);

	//删除掉非法数据
	DWORD dwMinute = 0;
	for (it = VklF5_New.begin(); it!= VklF5_New.end(); ++it)
	{
		tKL = it->tUnix;
		if(tKL.GetMinute() % 5 != 0)
		{
			it = VklF5_New.erase(it);
		}else
		{
			dwMinute = tKL.GetHour() * 60 + tKL.GetMinute();
			if( (dwMinute > 11 *60 + 30 && dwMinute < 13 * 60 + 5) || dwMinute < 9 *60 + 35 || dwMinute > 15 * 60 + 0 )
				it = VklF5_New.erase(it);
		}
		if(it == VklF5_New.end())
			break;
	}

	INT nEnd  = VklF5_New.size() - 1;
	while(TRUE)
	{
		for (INT i=sizeof(F5Table)/sizeof(MINUTETABLE) - 1 ; i >= 0; i--)
		{
			if(nEnd <= 0)
				return TRUE;
			tKL = VklF5_New[nEnd].tUnix;
			//CString szText;
			//szText.Format("%02u-%02u-%02u %02u:%02u:%02u",tKL.GetYear(),tKL.GetMonth(),tKL.GetDay(),tKL.GetHour(),tKL.GetMinute(),tKL.GetSecond());
			if(tKL.GetHour() != F5Table[i].wHour || tKL.GetMinute() != F5Table[i].wMinute)
			{
				//注意时间轴 这里无需判断 nEnd + 1
				if(tKL.GetHour() == 15 && tKL.GetMinute() == 0)
					tKL = VklF5_New[nEnd+1].tUnix;
				CTime Temp(tKL.GetYear(),tKL.GetMonth(),tKL.GetDay(),F5Table[i].wHour,F5Table[i].wMinute,0);
				KLINEDATA KLineData;
				KLineData.fVolume   = 0;
				KLineData.fAmount   = 0;
				KLineData.fOpen     = VklF5_New[nEnd-1].fClose;
				KLineData.fClose    = VklF5_New[nEnd-1].fClose;
				KLineData.fHigh     = VklF5_New[nEnd-1].fClose;
				KLineData.fLow      = VklF5_New[nEnd-1].fClose;
				KLineData.tUnix     = Temp.GetTime();
				VklF5_New.insert( &VklF5_New[nEnd+1],KLineData);
				continue;
			}
			nEnd--;
		}
	}
	return TRUE;
}



INT CKLineDataMgr::MakeF5KLine( VKL & VklF5_New , LPCTSTR szLabel )
{
//	CExtenLogDlg ExtenLogDlg;
//	ExtenLogDlg.InsertLog("%s 补5分钟数据！",szLabel);

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

	//ExtenLogDlg.InsertLog("%s 校验5分钟数据！",szLabel);

	//校验5分钟数据
	if(!CheckF5KLine(VklF5_New))
	{
//		ExtenLogDlg.InsertLog("%s 5 分钟数据不完整,放弃！",szLabel);
		return -2;
	}

//	ExtenLogDlg.InsertLog("%s 校验5分钟数据完成！",szLabel);

 	vector<KLINEDATA> VklF5_Old;
 	if(!LoadFile(VklF5_Old,szLabel,"F5.dat"))
 		return SaveFile(VklF5_New,szLabel,"F5.dat") ? 0 : -1;
 
 	INT nResult = MergeKLine(VklF5_New,VklF5_Old);
 	VklF5_New = VklF5_Old;
	return SaveFile(VklF5_Old,szLabel,"F5.dat") ?  nResult: - 1;
}






INT CKLineDataMgr::MakeF1KLine( VKL & VklF1_New,LPCTSTR szLabel )
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
	if(0 == stricmp(lpszFileName,"Day.dat"))
	{
		map< string, VKL >::iterator it = m_DayKLine.find(szLabel);
		if(it != m_DayKLine.end())
		{
			vkl = it->second;
			return TRUE;
		}
		return FALSE;
	}
	return LoadFile(vkl,szLabel,lpszFileName);
}

