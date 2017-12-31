#ifndef ClientKLineDataMgr_h__
#define ClientKLineDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "DirectoryMgr.h"

#include <set>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;


typedef struct _ALLKL
{
	vector <KLINEDATA> VKLF1;
	vector <KLINEDATA> VKLF5;
	vector <KLINEDATA> VKLF15;
	vector <KLINEDATA> VKLF30;
	vector <KLINEDATA> VKLF60;
	vector <KLINEDATA> VKLDay;
}ALLKL,*LPALLKL;



class CClientKLineDataMgr
{
public:
	BOOL Insert(const char * szLabel, LPKLINEDATA pKLineData,INT nSize, INT nPeriod)
	{
		map<string,ALLKL>::iterator it  = m_Map.find(szLabel);
		if(it == m_Map.end())
		{
			ALLKL AllKL;
			pair< map<string,ALLKL>::iterator , bool>  pi = m_Map.insert(pair<string,ALLKL>(szLabel,AllKL));
			it = pi.first;
		}
		
		vector<KLINEDATA> * pVkl = NULL;
		switch(nPeriod)
		{
			case  KLINE_PERIOD_F1:    pVkl = &it->second.VKLF1;		  break;
			case  KLINE_PERIOD_F5:    pVkl = &it->second.VKLF5;		  break;
			case  KLINE_PERIOD_F15:   pVkl = &it->second.VKLF15;      break;
			case  KLINE_PERIOD_F30:   pVkl = &it->second.VKLF30;      break;
			case  KLINE_PERIOD_F60:   pVkl = &it->second.VKLF60;      break;
			case  KLINE_PERIOD_DAY:   pVkl = &it->second.VKLDay;      break;
		}

		pVkl->assign(&pKLineData[0],&pKLineData[nSize]);
		return TRUE;
	}	
	

	BOOL Check(const char * szLabel, INT nPeriod)
	{
		if(szLabel == NULL)
			return FALSE;

		map<string,ALLKL>::iterator it  = m_Map.find(szLabel);
		if(it == m_Map.end())
			return FALSE;
		
		vector<KLINEDATA> * pVKL = NULL;

		switch(nPeriod)
		{
			case  KLINE_PERIOD_F1:    pVKL = &it->second.VKLF1;		  break;
			case  KLINE_PERIOD_F5:    pVKL = &it->second.VKLF5;		  break;
			case  KLINE_PERIOD_F15:   pVKL = &it->second.VKLF15;      break;
			case  KLINE_PERIOD_F30:   pVKL = &it->second.VKLF30;      break;
			case  KLINE_PERIOD_F60:   pVKL = &it->second.VKLF60;      break;
			case  KLINE_PERIOD_DAY:   pVKL = &it->second.VKLDay;      break;
		}

		if(pVKL == NULL || pVKL->size() == 0)
			return FALSE;

		return TRUE;
	}


	BOOL Select(const char * szLabel, INT nPeriod , vector<KLINEDATA> & VKLResult)
	{
		if(szLabel == NULL)
			return FALSE;

		map<string,ALLKL>::iterator it  = m_Map.find(szLabel);
		if(it == m_Map.end())
			return NULL;
		
		switch(nPeriod)
		{
		case  KLINE_PERIOD_F1:    VKLResult = it->second.VKLF1;		  break;
		case  KLINE_PERIOD_F5:    VKLResult = it->second.VKLF5;		  break;
		case  KLINE_PERIOD_F15:   VKLResult = it->second.VKLF15;      break;
		case  KLINE_PERIOD_F30:   VKLResult = it->second.VKLF30;      break;
		case  KLINE_PERIOD_F60:   VKLResult = it->second.VKLF60;      break;
		case  KLINE_PERIOD_DAY:   VKLResult = it->second.VKLDay;      break;
		}
		
		
		return VKLResult.size() ? TRUE : FALSE;
	}

private:
	map<string,ALLKL> m_Map;
};



#endif // ClientKLineDataMgr_h__