#ifndef HQBaseInfo_h__
#define HQBaseInfo_h__

#include <map>
#include <string>
using namespace std;
#include "KLineDataMgr.h"
#include "Common/Common.h"
#include "ComHeader/ComHeader.h"


class CHQBaseInfoDataMgr
{
public:
	CHQBaseInfoDataMgr()
	{
		MakeDataPath("BaseInfo.db",m_szDbPath);
	}

	BOOL Get5DayInfo( const VKL & KLDay , float & f5DayVolume , float & f5DayAmount )
	{
		if(KLDay.size() == 0 )
			return FALSE;
		
		INT nCount = 0;
		for (INT i = KLDay.size() - 2; i >= 0 ; i--)
		{
			f5DayVolume	 += KLDay[i].fVolume;
			f5DayAmount	 += KLDay[i].fAmount;
			if(++nCount == 5)
				break;
		}
		return TRUE;
	}


	INT Init5DayInfo(const map< string , VKL > * KMap)
	{
		INT nCount = 0;
		map< string , VKL >::const_iterator it = KMap->begin();

		for (it; it != KMap->end(); ++it)
		{
			LPHQBASE_INFO lpBaseInfo =  Select(it->first.c_str());
			if(lpBaseInfo == NULL)
			{
				HQBASE_INFO BaseInfo;
				ZeroMemory(&BaseInfo,sizeof(HQBASE_INFO));
				strcpy((char *)&BaseInfo.wMarket,it->first.c_str());

				pair< map<string, HQBASE_INFO>::iterator, bool > ip= m_MData.insert(pair<string, HQBASE_INFO>(it->first,BaseInfo));

				if(ip.second == false)
					continue;

				lpBaseInfo = &ip.first->second;
			}
			Get5DayInfo(it->second,lpBaseInfo->f5DayVolume,lpBaseInfo->f5DayAmount);
			nCount++;
		}
		return nCount;
	}




	BOOL Init()
	{
		HQBASE_INFO * pHQBaseInfo = new HQBASE_INFO[ 5000 ];
		if(pHQBaseInfo == NULL)
			return FALSE;
		
		INT nSize = 5000 * sizeof(HQBASE_INFO);
		if(LoadFile(m_szDbPath,pHQBaseInfo,nSize) <=0 )
		{
			delete []pHQBaseInfo;
			return FALSE;
		}
		
		nSize = nSize/sizeof(HQBASE_INFO);
		for (INT i=0;i<nSize;i++)
			m_MData.insert( pair<string,HQBASE_INFO>( (const char *)&pHQBaseInfo[i].wMarket,pHQBaseInfo[i]) );


		delete []pHQBaseInfo;

		return m_MData.size() ? TRUE : FALSE;
	}

	BOOL Init(HQBASE_INFO * pBaseInfo,DWORD dwSize)
	{
		m_MData.clear();
		dwSize = dwSize/sizeof(HQBASE_INFO);
		for (INT i=0;i<dwSize;i++)
			m_MData.insert( pair<string,HQBASE_INFO>((const char *)&pBaseInfo[i].wMarket,pBaseInfo[i]) );

		return m_MData.size() ? TRUE : FALSE;
	}


	BOOL Destory()
	{
		if(m_MData.size() == 0)
			return TRUE;
		
		HQBASE_INFO * pHQBaseInfo = new HQBASE_INFO[ 5000 ];
		if(pHQBaseInfo == NULL)
			return FALSE;
		
		map<string,HQBASE_INFO>::iterator it;
		HQBASE_INFO * pOffset = pHQBaseInfo;
		
		for (it = m_MData.begin(); it != m_MData.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(HQBASE_INFO));
			pOffset++;
		}

		INT nSize = m_MData.size() * sizeof(HQBASE_INFO);
		SaveFile(m_szDbPath,pHQBaseInfo,nSize);
		
		delete [] pHQBaseInfo;
		return TRUE;
	}


	BOOL Insert(const HQBASE_INFO & HQBaseInfo)
	{
		map<string,HQBASE_INFO>::iterator it = m_MData.find((const char *)&HQBaseInfo.wMarket);
		
		//没有找到 则插入新的
		if(it == m_MData.end())
		{
			m_MData.insert( pair<string,HQBASE_INFO>( (const char *)&HQBaseInfo.wMarket , HQBaseInfo ) );
			return TRUE;
		}else
		{
			//更新数据为CurRRS
			it->second = HQBaseInfo;
		}
		return TRUE;
	}

	map<string, HQBASE_INFO> * GetMapPoint(){return &m_MData; }

	LPHQBASE_INFO Select(const char * szFullLabel){

		map<string,HQBASE_INFO>::iterator it = m_MData.find(szFullLabel);
		if(it == m_MData.end())
			return NULL;
		return &it->second;
	}

	BYTE * GetMemoryObject( DWORD & dwOBSize )
	{
		if(m_MData.size() == 0)
			return NULL;
		
		INT n = m_MData.size();
		HQBASE_INFO * pBaseInfo = new HQBASE_INFO[ m_MData.size() ];
		if(pBaseInfo == NULL)
			return FALSE;
		
		HQBASE_INFO * pOffset = pBaseInfo;

		map<string,HQBASE_INFO>::iterator it;
		for (it = m_MData.begin(); it != m_MData.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(HQBASE_INFO));
			pOffset++;
		}
		
		dwOBSize = m_MData.size() * sizeof(HQBASE_INFO);
		return (BYTE *)pBaseInfo;
	}
	
	BOOL FreeMemoryObject(BYTE * MemoryObject, DWORD dwOBSize)
	{
		delete [] MemoryObject;
		return TRUE;
	}

	INT GetSize(){ return m_MData.size(); }

private:
	CHAR  m_szDbPath[MAX_PATH];
	map<string, HQBASE_INFO> m_MData;
};





#endif // HQBaseInfo_h__