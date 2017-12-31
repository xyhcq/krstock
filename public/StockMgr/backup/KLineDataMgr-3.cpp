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
		//У������
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
 �ܴ���2������µ�K��
 
 1 2 3 4 5 6 7 8 9        -- �Ѵ��ڵľ��ļ�����
         5 6 7 8 9 10 11  -- ���յ��������� 
 1 2 3 4 5 6 7 8 9 10 11  -- �����������

  
  1   2  3  4  5  6  7  8  9              -- �Ѵ��ڵľ��ļ�����
  10 11 12 13 14 15 16		              -- ���յ��������� 
  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16  -- �������������
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
		//����K��û�жԱ��� ��β��׷��
		VklOld.insert(&VklOld[VklOld.size()],VklNew.begin(),VklNew.end());
		return 1;
	}
	
	//����K�߶Ա����� �Ѿ�K�ߵĵ�ǰ����֮���ȫ��ɾ�� 
	VklOld.erase(&VklOld[i],VklOld.end());
	//������K��
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



// ���� -1  �ļ�ϵͳʧ��  
// ���� 0   �ļ��½�
// ���� 1   ��ɵ�K�ߴ�����ͬ�� �ϲ�����
// ���� 2   ��ɵ�K�߲�������ͬ β��׷��
INT CKLineDataMgr::SaveDayKLine( VKL & VklDay_New,LPCTSTR szLabel )
{
	//��ʱ������ 
	sort(VklDay_New.begin(),VklDay_New.end(),Sort_Proc);

	//�Ƿ�����
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

	//1 ��ʾβ��׷��   0 ��ʾ�������ݴ���һ���ϲ�����
	INT nResult = MergeKLine(VklDay_New,VklDay_Old);

	VklDay_New = VklDay_Old;
	return SaveFile(VklDay_Old,szLabel,"Day.dat") ?  nResult: - 1;
}

INT CKLineDataMgr::SaveF5KLine( VKL & VklF5_New , LPCTSTR szLabel )
{
	//��ʱ������ 
	sort(VklF5_New.begin(),VklF5_New.end(),Sort_Proc);
	
	//�Ƿ�����
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
	
	//5F ���뱣֤�������� ��Ҫ�޸��۶�ʱ���ֵ����� 
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
	//��ʱ������ 
	sort(VklF1_New.begin(),VklF1_New.end(),Sort_Proc);
	//�Ƿ�����
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
