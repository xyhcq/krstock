#ifndef FenBiDataMgr_h__
#define FenBiDataMgr_h__


#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "DirectoryMgr.h"

#include <set>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

typedef vector<TRADINGRECORD> VFB;

class CFenBiDataMgr
{
public:
	CFenBiDataMgr(){
		m_DirMgr.Init("FenBiData");
		m_DirMgr.MakePath(NULL,"config.ini",m_szConfigPath,MAX_PATH);
	}

	BOOL Select(const char* lpszFullLabel, VFB & vfb, WORD wYear,WORD wMonth,WORD wDay)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		//当日数据 
		if( (st.wYear == wYear && st.wMonth ==  wMonth && st.wDay == wDay) || (wYear == 0 && wMonth == 0 && wDay==0) )
		{
			map<string,VFB >::iterator it = m_MData.find(lpszFullLabel);
			
			//没有找到数据
			if ( it == m_MData.end())
				return FALSE;

			vfb = it->second;
			return TRUE;
		}


		TRADINGRECORD rfs[6000];
		DWORD dwSize  = sizeof(rfs);
		
		CHAR szFileName[MAX_PATH]={0};
		sprintf(szFileName,"%04u-%02u-%02u.dat",wYear,wMonth,wDay);
		if(!m_DirMgr.LoadFile(  lpszFullLabel ,szFileName,rfs,dwSize))
			return FALSE;

		vfb.assign( &rfs[0], &rfs[dwSize/sizeof(TRADINGRECORD)] );
		return TRUE;
	}

	BOOL Init()
	{
		CHAR szLastDate[30]={0};
		if(!GetPrivateProfileString("config","lastdate","",szLastDate,30,m_szConfigPath))
			return FALSE;

		char szFileName[MAX_PATH]={NULL};
		sprintf(szFileName,"%s.dat",szLastDate);

		INT nLoadCount = 0;
		INT nDirCount = m_DirMgr.Size();
		CExtenLogDlg ExtenLogDlg;
		ExtenLogDlg.InsertLog("FenBiDataMgr::Init SDir.size() = %d ",nDirCount);


		TRADINGRECORD rfs[6000];
		set<string> SDir = m_DirMgr.Get();
		for (set<string>::iterator  SDirIt = SDir.begin(); SDirIt != SDir.end(); ++SDirIt)
		{
			DWORD dwSize  = sizeof(rfs);
			string s = *SDirIt;
			if(m_DirMgr.LoadFile(  s.c_str() ,szFileName,rfs,dwSize))
			{

				VFB vfb;
				pair<map<string,VFB>::iterator, bool> Insert_Pair;
				Insert_Pair = m_MData.insert( pair<string,VFB>( *SDirIt , vfb));
				if(Insert_Pair.second == TRUE)
				{
					Insert_Pair.first->second.reserve(6000);
					dwSize /= sizeof(TRADINGRECORD);

					if((INT)dwSize < 0  || dwSize > 6000)
					{
						CHAR szTip[200]={0};
						sprintf(szTip,"Label:%s dwSize:%d",s.c_str(),dwSize);
						::MessageBox(NULL,szTip,szFileName,MB_OK);
					}
					else
					{
						if(dwSize > 0) Insert_Pair.first->second.assign(&rfs[0],&rfs[dwSize]);
					}
				}
			}
			ExtenLogDlg.InsertLog("FenBiDataMgr::Load %s 总:%d 剩余:%d",s.c_str(),nDirCount,nDirCount - nLoadCount++);
		}
		return m_MData.size()? TRUE : FALSE;
		return FALSE;
	}

	BOOL Save()
	{

		INT nMKLine = GetSize();
		CExtenLogDlg ExtenLogDlg;
		ExtenLogDlg.InsertLog("FenBiDataMgr::Save  m_MData.size() = %d ",nMKLine);
		INT nSaveCount=0;

		char szFileName[MAX_PATH]={NULL};
		map<string,VFB>::iterator it;
		for (it = m_MData.begin(); it != m_MData.end(); ++it)
		{
			//日期以第一笔数据为准
			CTime t(it->second[0].m_lTime);
			sprintf(szFileName,"%04u-%02u-%02u.dat",t.GetYear(),t.GetMonth(),t.GetDay());
			DWORD dwSaveSize = it->second.size() * sizeof(TRADINGRECORD);
			BOOL bResult =  m_DirMgr.SaveFile(it->first.c_str(),szFileName,&it->second[0],dwSaveSize);
			ExtenLogDlg.InsertLog("FenBiDataMgr::Save %s 总:%d 剩余:%d",it->first.c_str(),nMKLine,nMKLine - nSaveCount++);
		}

		it = m_MData.begin();
		CTime t(it->second[ it->second.size() -1  ].m_lTime);
		CHAR szLastDate[30]={0};
		sprintf(szLastDate,"%04u-%02u-%02u",t.GetYear(),t.GetMonth(),t.GetDay());
		WritePrivateProfileString("config","lastdate",szLastDate,m_szConfigPath);
		return TRUE;
	}



	/*
	BOOL Save()
	{
		
		INT nMKLine = GetSize();
		CExtenLogDlg ExtenLogDlg;
		ExtenLogDlg.InsertLog("FenBiDataMgr::Save  m_MData.size() = %d ",nMKLine);
		INT nSaveCount=0;
		
		char szFileName[MAX_PATH]={NULL};
		map<string,VFB>::iterator it;
		for (it = m_MData.begin(); it != m_MData.end(); ++it)
		{
			//日期以第一笔数据为准
			CTime t(it->second[0].m_lTime);
			sprintf(szFileName,"%04u-%02u-%02u.dat",t.GetYear(),t.GetMonth(),t.GetDay());
			DWORD dwSaveSize = it->second.size() * sizeof(TRADINGRECORD);
			
			// 1 号跟 2号重叠了
			VFB & vfb = it->second;
			for (INT i=0; i< vfb.size(); i++)
			{
				if(CTime(vfb[i].m_lTime).GetDay() == 8)
					break;
			}
			
			dwSaveSize = i *  sizeof(TRADINGRECORD);
			m_DirMgr.SaveFile(it->first.c_str(),"2016-12-07.dat",&vfb[0],dwSaveSize);
			
			dwSaveSize = ( vfb.size() - i ) *  sizeof(TRADINGRECORD);
			m_DirMgr.SaveFile(it->first.c_str(),"2016-12-08.dat",&vfb[i],dwSaveSize);
			
			//BOOL bResult =  m_DirMgr.SaveFile(it->first.c_str(),szFileName,&it->second[0],dwSaveSize);
			ExtenLogDlg.InsertLog("FenBiDataMgr::Save %s 总:%d 剩余:%d",it->first.c_str(),nMKLine,nMKLine - nSaveCount++);
		}
		
		it = m_MData.begin();
		CTime t(it->second[ it->second.size() -1  ].m_lTime);
		CHAR szLastDate[30]={0};
		sprintf(szLastDate,"%04u-%02u-%02u",t.GetYear(),t.GetMonth(),t.GetDay());
		WritePrivateProfileString("config","lastdate",szLastDate,m_szConfigPath);
		return TRUE;
		}


	BOOL Init()
	{
		CHAR szLastDate[30]={0};
		if(!GetPrivateProfileString("config","lastdate","",szLastDate,30,m_szConfigPath))
			return FALSE;

		char szFileName[MAX_PATH]={NULL};
		sprintf(szFileName,"%s.dat",szLastDate);

		INT nLoadCount = 0;
		INT nDirCount = m_DirMgr.Size();
		CExtenLogDlg ExtenLogDlg;
		ExtenLogDlg.InsertLog("FenBiDataMgr::Init SDir.size() = %d ",nDirCount);

		TRADINGRECORD * rfs = new TRADINGRECORD[20000];
		set<string> SDir = m_DirMgr.Get();
		for (set<string>::iterator  SDirIt = SDir.begin(); SDirIt != SDir.end(); ++SDirIt)
		{
			DWORD dwSize  = 20000 * sizeof(TRADINGRECORD);
			string s = *SDirIt;
			if(m_DirMgr.LoadFile(  s.c_str() ,szFileName,rfs,dwSize))
			{
				VFB vfb;
				pair<map<string,VFB>::iterator, bool> Insert_Pair;
				Insert_Pair = m_MData.insert( pair<string,VFB>( *SDirIt , vfb));
				if(Insert_Pair.second == TRUE)
				{
					//Insert_Pair.first->second.reserve(1);
					dwSize /= sizeof(TRADINGRECORD);
					if(dwSize > 0) Insert_Pair.first->second.assign(&rfs[0],&rfs[dwSize]);
				}
			}
			ExtenLogDlg.InsertLog("FenBiDataMgr::Load %s 总:%d 剩余:%d",s.c_str(),nDirCount,nDirCount - nLoadCount++);
		}
		delete [] rfs;
	}*/

	BOOL Destory()
	{
		return Save();
	}

	void RRSToFB(const RCV_REPORT_STRUCTExV3 * pNewRRS ,const RCV_REPORT_STRUCTExV3 * pOldRRS,TRADINGRECORD & FB)
	{
		FB.m_lTime		= pNewRRS->m_time;
		FB.m_fHigh		= pNewRRS->m_fHigh;
		FB.m_fLow		= pNewRRS->m_fLow;
		FB.m_fNewPrice	= pNewRRS->m_fNewPrice;
		FB.m_fVolume	= pNewRRS->m_fVolume;
		FB.m_fAmount	= pNewRRS->m_fAmount;
		FB.m_bMark      = TRADINGRECORD_MARK_ZDBUY;

		//老数据没有开盘
		if(pOldRRS == NULL || Price_Equality(pOldRRS->m_fOpen , 0.00f))
		{
			if(pNewRRS->m_fNewPrice > pNewRRS->m_fLastClose)
				FB.m_bMark = TRADINGRECORD_MARK_ZDBUY;
			else if( pNewRRS->m_fNewPrice < pNewRRS->m_fLastClose )
				FB.m_bMark = TRADINGRECORD_MARK_ZDSELL;
			return;
		}

		if(pNewRRS->m_fNewPrice >= pOldRRS->m_fSellPrice[0])
		{
			FB.m_bMark = TRADINGRECORD_MARK_ZDBUY;
			return;
		}

		if(pNewRRS->m_fNewPrice <= pOldRRS->m_fBuyPrice[0] )
		{
			FB.m_bMark = TRADINGRECORD_MARK_ZDSELL;
			return;
		}
		

		if(pNewRRS->m_fNewPrice == pOldRRS->m_fSellPrice[0])
		{
			FB.m_bMark = TRADINGRECORD_MARK_ZDSELL;
			return;
		}
		
		if(pNewRRS->m_fNewPrice == pOldRRS->m_fBuyPrice[0])
		{
			FB.m_bMark = TRADINGRECORD_MARK_ZDBUY;
			return;
		}
	}

	BOOL Insert( const RCV_REPORT_STRUCTExV3 * pNewRRS ,const RCV_REPORT_STRUCTExV3 * pOldRRS , BOOL bNewInsert = TRUE)
	{
		map<string,VFB>::iterator it = m_MData.find((const char *)&pNewRRS->m_wMarket);
		TRADINGRECORD  FB;
		RRSToFB(pNewRRS,pOldRRS,FB);

		//没有找到数据
		if(it == m_MData.end())
		{
			if(bNewInsert == FALSE)
				return FALSE;

			VFB vfb;
			pair< map<string,VFB>::iterator, bool> Insert_Pair= m_MData.insert( pair< string,VFB >( (const char *)&pNewRRS->m_wMarket, vfb ) );
			Insert_Pair.first->second.reserve(5000);
			Insert_Pair.first->second.push_back(FB);
			return TRUE;
		}

		//数据一样 放弃
		if( Amount_Equality(pNewRRS->m_fAmount , it->second[ it->second.size() - 1 ].m_fAmount ) )
			return FALSE;

		//保存分笔数据
		it->second.push_back(FB);
		return TRUE;
	}



	INT Insert(const char* lpszFullLabel ,const LPTRADINGRECORD pTTRHead,  INT nSize)
	{
		if(pTTRHead == NULL || nSize == 0)
			return 0;

		VFB vfb;
		pair< map<string,VFB>::iterator, bool> Insert_Pair= m_MData.insert( pair< string,VFB >( lpszFullLabel, vfb ) );
		Insert_Pair.first->second.assign(&pTTRHead[0],&pTTRHead[nSize]);
		return Insert_Pair.second ? 1 : 2;
	}
	


	const LPTRADINGRECORD  Select(const char* lpszFullLabel , INT * lpnSize){
		map<string,VFB >::iterator it = m_MData.find(lpszFullLabel);
		if ( it == m_MData.end())
			return NULL;

		*lpnSize = it->second.size();
		return (LPTRADINGRECORD)it->second.begin();
	}

	void Clear(){ m_MData.clear(); }
	map<string,VFB>  * GetMapPoint(){ return &m_MData; } 
	DWORD GetSize(){ return m_MData.size(); }
private:
	map<string,VFB> m_MData;
	CDirectoryMgr	m_DirMgr;
	CHAR			m_szConfigPath[MAX_PATH];
};



#endif // FenBiDataMgr_h__