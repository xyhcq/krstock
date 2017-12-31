#ifndef HangYeDataMgr_h__
#define HangYeDataMgr_h__

#include <VECTOR>
#include <map>
#include <string>
using namespace std;
#include "Common/Common.h"
#include "ComHeader/ComHeader.h"




typedef vector< PRCV_REPORT_STRUCTExV3 >	VPRRS;
typedef map< string, VPRRS >				HYMAP;




class CHangYeDataMgr
{

public:
	BOOL Init(map<string , RCV_REPORT_STRUCTExV3 > * pMRRS , map<string, HQBASE_INFO> * pMBaseInfo)
	{
		map<string , HQBASE_INFO >::iterator  itBaseInfo = pMBaseInfo->begin();
		for (itBaseInfo ; itBaseInfo !=  pMBaseInfo->end(); ++itBaseInfo)
		{
			string szLabel = itBaseInfo->first;

			//先在行情数据里面查找
			map<string , RCV_REPORT_STRUCTExV3 >::iterator itRRS = pMRRS->find(szLabel);
			if( itRRS == pMRRS->end())
				continue;

			string szHY = (const char *)&itBaseInfo->second.szSSHY;
			if(szHY.length() == 0)
				continue;

			HYMAP::iterator itMData =  m_MData.find( szHY );

			if(itMData == m_MData.end())
			{
				VPRRS  VR;
				pair< HYMAP::iterator, bool> ip = m_MData.insert( pair< string,  VPRRS >(szHY,VR));
				if(ip.second==false)
					continue;

				itMData = ip.first;
			}

		    itMData->second.push_back(&itRRS->second);
		}
		return TRUE;
	}
	


	const VPRRS * Select(const char * szHY)
	{
		HYMAP::iterator it =  m_MData.find( szHY );
		if(it == m_MData.end())
			return NULL;

		return &it->second;
	}	


private:
	map< string,  VPRRS   > m_MData;
};




#endif // HangYeDataMgr_h__