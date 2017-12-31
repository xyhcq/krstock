#ifndef HQSnapshoot_h__
#define HQSnapshoot_h__


#include "Common/Common.h"
#include "ComHeader/ComHeader.h"


#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

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



	BOOL Insert(const RCV_REPORT_STRUCTExV3 & CurRRS,RCV_REPORT_STRUCTExV3 & OldRRS)
	{
		map<string,RCV_REPORT_STRUCTExV3>::iterator it = m_HQSS.find((const char *)&CurRRS.m_wMarket);

		//没有找到 则插入新的
		if(it == m_HQSS.end())
		{
			m_HQSS.insert( pair<string,RCV_REPORT_STRUCTExV3>( (const char *)&CurRRS.m_wMarket , CurRRS ) );
			return TRUE;
		}else
		{
			//数据一样 不更新
			if( Amount_Equality(CurRRS.m_fAmount,it->second.m_fAmount))
				return FALSE;

			//OldRRS 记录为前一个数据
			OldRRS  = it->second;

			//更新数据为CurRRS
			it->second = CurRRS;
		}
		return TRUE;
	}


	map<string , RCV_REPORT_STRUCTExV3 > & GetMap(){ return m_HQSS; }

	DWORD GetSize(){ return m_HQSS.size(); } 

	const RCV_REPORT_STRUCTExV3 * Select(const char * szFullLabel){
		map<string,RCV_REPORT_STRUCTExV3>::iterator it = m_HQSS.find(szFullLabel);
		if(it == m_HQSS.end())
			return NULL;

		return &it->second;
	}

private:
	map<string , RCV_REPORT_STRUCTExV3 > m_HQSS;
	CHAR  m_szDbPath[MAX_PATH];
};


#endif // HQSnapshoot_h__