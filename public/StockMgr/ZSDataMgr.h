#ifndef ZSDataMgr_h__
#define ZSDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"


#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

#pragma pack(push,1)
typedef struct tagZSDATA
{
	time_t t;
	float  p;
} ZSDATA,*LPZSDATA;
#pragma pack(pop)


class CZSDataMgr
{
public:
	BOOL Insert(time_t t , float p , const char * szKey)
	{
		map<string, vector< ZSDATA > >::iterator it = 	m_MData.find(szKey);
		if(it == m_MData.end())
		{
			vector<ZSDATA> vzsd;
			pair< map<string, vector< ZSDATA > >::iterator , bool>	 ip =  m_MData.insert(pair<string, vector< ZSDATA > >(szKey,vzsd) );
			if(ip.second == false)
				return FALSE;

			it = ip.first;
			it->second.reserve(2400);
		}

		ZSDATA ZSData;
		ZSData.t = t;
		ZSData.p = p;
		it->second.push_back(ZSData);


// 		vector<ZSDATA> & vzsd = it->second;
// 		if(vzsd.size() == 0)
// 		{
// 			vzsd.push_back(ZSData);
// 			return TRUE;
// 		}
// 		CTime tCur = t;
// 		CTime tOld = vzsd[ vzsd.size() - 1] .t;
// 		if(tOld.GetMinute() != tCur.GetMinute())
// 		{
// 			vzsd.push_back(ZSData);
// 			return TRUE;
// 		} 
// 		ZSDATA & ZSDataOld = vzsd[ vzsd.size() - 1];
// 		ZSDataOld.t = t;
// 		ZSDataOld.p = p;

		return TRUE;
	}

	float Select(const char * szKey,  INT nMinute)
	{
		map<string, vector< ZSDATA > >::iterator it = 	m_MData.find(szKey);
		if(it == m_MData.end())
			return 0.00f;

		vector< ZSDATA > & VZSD = it->second;
		vector< ZSDATA >::reverse_iterator rit = VZSD.rbegin();
		CTime tCur = rit->t;
		CTime tOld = rit->t;
		BOOL bFirst = TRUE;

		for (rit; rit != VZSD.rend(); ++rit)
		{
			if(bFirst == TRUE)
			{
				bFirst = FALSE;
				continue;
			}
			
			tOld = rit->t;
			CTimeSpan ts = tCur - tOld;
			if(ts.GetMinutes() >= nMinute)
				return rit->p;
		}
		return VZSD[0].p;
	}


private:
	map<string, vector< ZSDATA > > m_MData;
};


#endif // ZSDataMgr_h__