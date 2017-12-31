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
					Insert_Pair.first->second.reserve(5000);
					dwSize /= sizeof(TRADINGRECORD); 
					Insert_Pair.first->second.assign(&rfs[0],&rfs[dwSize]);
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

		//保存日期 以上证指数为准
		it = m_MData.find("SH000001");
		CTime t(it->second[ it->second.size() -1  ].m_lTime);
		CHAR szLastDate[30]={0};
		sprintf(szLastDate,"%04u-%02u-%02u",t.GetYear(),t.GetMonth(),t.GetDay());
		WritePrivateProfileString("config","lastdate",szLastDate,m_szConfigPath);
		return TRUE;
	}
	
	BOOL Destory()
	{
		return Save();
	}

	void RRSToFB(RCV_REPORT_STRUCTExV3 & NewRRS ,RCV_REPORT_STRUCTExV3 & OldRRS,TRADINGRECORD & FB)
	{
		FB.m_lTime		= NewRRS.m_time;
		FB.m_fHigh		= NewRRS.m_fHigh;
		FB.m_fLow		= NewRRS.m_fLow;
		FB.m_fNewPrice	= NewRRS.m_fNewPrice;
		FB.m_fVolume	= NewRRS.m_fVolume;
		FB.m_fAmount	= NewRRS.m_fAmount;

		if(Price_Equality(OldRRS.m_fOpen , 0.00f))
		{
			FB.m_bMark = TRADINGRECORD_MARK_NORMAL;
		}
		else
		{
			if(NewRRS.m_fNewPrice >= OldRRS.m_fSellPrice[0])
				FB.m_bMark = TRADINGRECORD_MARK_ZDBUY;
			
			if(NewRRS.m_fNewPrice <= OldRRS.m_fBuyPrice[0])
				FB.m_bMark = TRADINGRECORD_MARK_ZDSELL;
		}
	}
	
	BOOL Insert( RCV_REPORT_STRUCTExV3 & NewRRS ,RCV_REPORT_STRUCTExV3 & OldRRS)
	{

		map<string,VFB>::iterator it = m_MData.find((const char *)&NewRRS.m_wMarket);

		//查找到了数据
		if (it != m_MData.end())
		{
			//数据一样 放弃
			if( Amount_Equality(NewRRS.m_fAmount , it->second[ it->second.size() - 1 ].m_fAmount ) )
				return FALSE;

			//保存数据
			TRADINGRECORD  FB;
			RRSToFB(NewRRS,OldRRS,FB);
			it->second.push_back(FB);
		}else
		{
			//插入新数据
			VFB vfb;
			pair< map<string,VFB>::iterator, bool> Insert_Pair= m_MData.insert( pair< string,VFB >( (const char *)&NewRRS.m_wMarket, vfb ) );

			Insert_Pair.first->second.reserve(5000);
			TRADINGRECORD  FB;
			RRSToFB(NewRRS,OldRRS,FB);
			Insert_Pair.first->second.push_back(FB);
		}
		return TRUE;
	}
	
	const LPTRADINGRECORD  Select(const char* lpszFullLabel , INT * lpnSize){
		map<string,VFB >::iterator it = m_MData.find(lpszFullLabel);
		if ( it == m_MData.end())
			return NULL;

		*lpnSize = it->second.size();
		return (LPTRADINGRECORD)it->second.begin();
	}

	
	map<string,VFB>  * GetMapPoint(){ return &m_MData; } 
	DWORD GetSize(){ return m_MData.size(); }
private:
	map<string,VFB> m_MData;
	CDirectoryMgr	m_DirMgr;
	CHAR			m_szConfigPath[MAX_PATH];
};



#endif // FenBiDataMgr_h__