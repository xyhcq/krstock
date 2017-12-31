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
	
	CHQBaseInfoDataMgr(){}

	
	BOOL Get5DayInfo(const VKL & KLDay , float & f5DayVolume , float & f5DayAmount);
	LPHQBASE_INFO Select(const char * szFullLabel);


	BOOL Init(const map< string , VKL > * pMap );


	BOOL Init(HQBASE_INFO * pBaseInfo,DWORD dwSize)
	{
		m_MData.clear();
		dwSize = dwSize/sizeof(HQBASE_INFO);
		for (INT i=0;i<dwSize;i++)
			m_MData.insert( pair<string,HQBASE_INFO>(pBaseInfo[i].szLabel,pBaseInfo[i]) );

		return m_MData.size() ? TRUE : FALSE;
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
	
	BOOL FreeMemoryObject(BYTE * MemoryObject, DWORD dwOBSize) {
		delete [] MemoryObject;
		return TRUE;
	}

	INT GetSize(){ return m_MData.size(); }

private:
	map<string, HQBASE_INFO> m_MData;
};




#endif // HQBaseInfo_h__