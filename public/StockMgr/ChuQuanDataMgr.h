#ifndef ChuQuanDataMgr_h__
#define ChuQuanDataMgr_h__




#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"


typedef struct  
{
	DWORD dwIndex;
	DWORD dwSize;
}INDEX;

typedef vector<RCV_POWER_STRUCTEx> VRCV_POWER_STRUCTEx;

class CChuQuanDataMgr
{
public:
	CChuQuanDataMgr()
	{
		MakeDataPath("ChuQuan.db",m_szDbPath);
	}

	BOOL Init(RCV_POWER_STRUCTEx* pRps, DWORD dwSize)
	{
		dwSize = dwSize/sizeof(RCV_POWER_STRUCTEx);
		for (int i = 0; i < dwSize; i++)
			Insert(pRps[i]);

		return m_Mdata.size()?TRUE:FALSE;
	}

	BOOL Init()
	{
		INT nFileSize = GetFileSize(m_szDbPath);
		if(nFileSize <=0   )
			return FALSE;

		DWORD dwSize = nFileSize/sizeof(RCV_POWER_STRUCTEx);
		RCV_POWER_STRUCTEx* pRps = new RCV_POWER_STRUCTEx[dwSize];
		if(pRps == NULL)
			return FALSE;

		LoadFile(m_szDbPath,pRps,nFileSize);
		for (int i = 0; i < dwSize; i++)
			Insert(pRps[i]);

		delete [] pRps;
		return m_Mdata.size()?TRUE:FALSE;
	}


	BOOL Destory()
	{
		DWORD dwPowerExCount = 0;

		map<string,VRCV_POWER_STRUCTEx>::iterator it = m_Mdata.begin();
		while (it != m_Mdata.end())
		{
			dwPowerExCount+=it->second.size();
			++it;
		}

		RCV_POWER_STRUCTEx* pRps = new RCV_POWER_STRUCTEx[dwPowerExCount];
		RCV_POWER_STRUCTEx* pRpsOffset = pRps;
		
		it = m_Mdata.begin();
		while (it != m_Mdata.end())
		{
			CopyMemory(pRpsOffset,&(it->second[0]),it->second.size()*sizeof(RCV_POWER_STRUCTEx) );
			pRpsOffset+=it->second.size();
			it++;
		}

		INT nBufferSize = dwPowerExCount*sizeof(RCV_POWER_STRUCTEx);
		SaveFile(m_szDbPath,pRps,nBufferSize);

		delete [] pRps;
		pRps = NULL;
		return TRUE;
	}


	BOOL Insert(RCV_POWER_STRUCTEx& rps)
	{
		if (rps.m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
			VRCV_POWER_STRUCTEx VRps;
			VRps.push_back(rps);
			m_Pair = m_Mdata.insert(pair<string,VRCV_POWER_STRUCTEx>( (const char *)&rps.m_head.m_wMarket, VRps));
			if (m_Pair.second == false)
			{
				m_Pair.first->second.clear();
				m_Pair.first->second.push_back(rps);
			}
		}else
		{
			m_Pair.first->second.push_back(rps);
		}
		return TRUE;
	}

	
	const RCV_POWER_STRUCTEx * Select(const char * szFullLabel , INT * lpnSize){
		map<string,VRCV_POWER_STRUCTEx>::iterator it = m_Mdata.find(szFullLabel);
		if(it == m_Mdata.end())
			return NULL;
		*lpnSize = it->second.size();
		return it->second.begin();
	}


	BYTE * GetMemoryObject( DWORD & dwOBSize ){
		if(m_Mdata.size() == 0)
			return FALSE;

		DWORD dwPowerExCount = 0;
		map<string,VRCV_POWER_STRUCTEx>::iterator it = m_Mdata.begin();
		while (it != m_Mdata.end())
		{
			dwPowerExCount+=it->second.size();
			++it;
		}
		
		RCV_POWER_STRUCTEx* pRps = new RCV_POWER_STRUCTEx[dwPowerExCount];
		RCV_POWER_STRUCTEx* pRpsOffset = pRps;
		it = m_Mdata.begin();
		while (it != m_Mdata.end())
		{
			CopyMemory(pRpsOffset,&(it->second[0]),it->second.size()*sizeof(RCV_POWER_STRUCTEx) );
			pRpsOffset+=it->second.size();
			it++;
		}
		dwOBSize = dwPowerExCount*sizeof(RCV_POWER_STRUCTEx);
		return (BYTE *)pRps;
	}

	BOOL FreeMemoryObject(BYTE * MemoryObject, DWORD dwOBSize) {
		delete [] MemoryObject;
		return TRUE;
	}

	map<string,VRCV_POWER_STRUCTEx> & GetMap(){ return m_Mdata;}
	DWORD GetSize() {  return m_Mdata.size(); }

private:
	CHAR  m_szDbPath[MAX_PATH];
	map<string,VRCV_POWER_STRUCTEx> m_Mdata;
	pair< map<string,VRCV_POWER_STRUCTEx>::iterator,bool> m_Pair;
};


#endif // ChuQuanDataMgr_h__