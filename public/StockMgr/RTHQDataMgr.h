#ifndef HQSnapshoot_h__
#define HQSnapshoot_h__


#include "Common/Common.h"
#include "ComHeader/ComHeader.h"


#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

#pragma pack(push,1)
typedef struct tagSMALLRRS
{
	time_t	  m_time;										// 成交时间
	WORD      nIndex;									    // 索引
	float	  m_fNewPrice;									// 最新
	float	  m_fVolume;									// 成交量
	float	  m_fAmount;									// 成交额 
	float	m_fBuyPrice[5];									// 申买价1,2,3,4,5
	INT		m_fBuyVolume[5];								// 申买量1,2,3,4,5
	float	m_fSellPrice[5];								// 申卖价1,2,3,4,5
	INT		m_fSellVolume[5];								// 申卖量1,2,3,4,5
} SMALLRRS,*LPSMALLRRS;
#pragma pack(pop)



class CRTHQDataMgr
{
public:
	CRTHQDataMgr(){
		MakeDataPath("RTHQ.db",m_szDbPath);
	}

	~CRTHQDataMgr(){}


	BOOL Init(){
		RCV_REPORT_STRUCTExV3 * pRRS = new RCV_REPORT_STRUCTExV3[ 5000 ];
		if(pRRS == NULL)
			return FALSE;
		
		INT nSize = 5000 * sizeof(RCV_REPORT_STRUCTExV3);
		if(LoadFile(m_szDbPath,pRRS,nSize) <=0 )
		{
			delete []pRRS;
			return FALSE;
		}
		
		nSize = nSize/sizeof(RCV_REPORT_STRUCTExV3);

		for (INT i=0;i<nSize;i++)
			m_HQSS.insert( pair<string,RCV_REPORT_STRUCTExV3>((const char *)&pRRS[i].m_wMarket,pRRS[i]) );
	
		
		delete []pRRS;
		return m_HQSS.size() ? TRUE : FALSE;
	}


	BOOL Init(RCV_REPORT_STRUCTExV3 * pRRS,DWORD dwSize)
	{
		m_HQSS.clear();
		dwSize = dwSize/sizeof(RCV_REPORT_STRUCTExV3);

		for (INT i=0;i<dwSize;i++)
			m_HQSS.insert( pair<string,RCV_REPORT_STRUCTExV3>((const char *)&pRRS[i].m_wMarket,pRRS[i]) );
		
		return m_HQSS.size() ? TRUE : FALSE;
	}

	BOOL Destory(){
		if(m_HQSS.size() == 0)
			return TRUE;
		
		RCV_REPORT_STRUCTExV3 * pRRS = new RCV_REPORT_STRUCTExV3[ 5000 ];
		if(pRRS == NULL)
			return FALSE;
		
		map<string,RCV_REPORT_STRUCTExV3>::iterator it;
		
		RCV_REPORT_STRUCTExV3 * pOffset = pRRS;
		
		for (it = m_HQSS.begin(); it != m_HQSS.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(RCV_REPORT_STRUCTExV3));
			pOffset++;
		}
		
		INT nSize = m_HQSS.size() * sizeof(RCV_REPORT_STRUCTExV3);
		SaveFile(m_szDbPath,pRRS,nSize);

		delete [] pRRS;
		return TRUE;
	}



	BOOL Insert(const RCV_REPORT_STRUCTExV3 & CurRRS)
	{
		map<string,RCV_REPORT_STRUCTExV3>::iterator it = m_HQSS.find((const char *)&CurRRS.m_wMarket);

		//没有找到 则插入新的
		if(it == m_HQSS.end())
		{
			m_HQSS.insert( pair<string,RCV_REPORT_STRUCTExV3>( (const char *)&CurRRS.m_wMarket , CurRRS ) );
			return TRUE;
		}else
		{
			//更新数据为CurRRS
			it->second = CurRRS;
		}
		return TRUE;
	}



	map<string , RCV_REPORT_STRUCTExV3 > & GetMap(){ 
		return m_HQSS; 
	}

	map<string , RCV_REPORT_STRUCTExV3 > * GetMapPoint(){ 
		return &m_HQSS; 
	}	
	


	DWORD GetSize(){ return m_HQSS.size(); } 

	RCV_REPORT_STRUCTExV3 * Select(const char * szFullLabel){

		map<string,RCV_REPORT_STRUCTExV3>::iterator it = m_HQSS.find(szFullLabel);
		if(it == m_HQSS.end())
			return NULL;

		return &it->second;
	}


	BYTE * GetMemoryObject( DWORD & dwOBSize )
	{
		if(m_HQSS.size() == 0)
			return NULL;

		RCV_REPORT_STRUCTExV3 * pRRS = new RCV_REPORT_STRUCTExV3[ m_HQSS.size() ];
		if(pRRS == NULL)
			return NULL;

		RCV_REPORT_STRUCTExV3 * pOffset = pRRS;
		map<string,RCV_REPORT_STRUCTExV3>::const_iterator it;
		for (it = m_HQSS.begin(); it != m_HQSS.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(RCV_REPORT_STRUCTExV3));
			pOffset++;
		}
		
		dwOBSize = m_HQSS.size()  * sizeof(RCV_REPORT_STRUCTExV3);

		return (BYTE * )pRRS;
	}
	
	BOOL FreeMemoryObject(BYTE * MemoryObject, DWORD dwOBSize) {
		delete [] MemoryObject;
		return TRUE;
	}



private:
	map<string , RCV_REPORT_STRUCTExV3 > m_HQSS;
	CHAR  m_szDbPath[MAX_PATH];
};





#endif // HQSnapshoot_h__