#ifndef KLineDataMgr_h__
#define KLineDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "DirectoryMgr.h"

#include <set>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

typedef vector <KLINEDATA>  VKL;

typedef struct _ALLVKL
{
	VKL  F1;
	VKL  F5;
	VKL  Day;
}ALLVKL,*LPALLVKL;



#define  LOAD_BUFFER_SIZE	1000000	


class CKLineDataMgr{

public:
	CKLineDataMgr(){
		m_DirMgr.Init("KLineData");
		m_LoadKLineBuffer = NULL;
	}

	void Destory(){
		if(m_LoadKLineBuffer) 
			delete [] m_LoadKLineBuffer;
		Save();
	}

	BOOL LoadKLine(const char * lpszLabel , const char * szlpFileName , VKL & vkl)
	{
		DWORD dwSize  = sizeof(KLINEDATA) * LOAD_BUFFER_SIZE;
		if(m_DirMgr.LoadFile( lpszLabel ,szlpFileName,m_LoadKLineBuffer,dwSize))
		{
			if(dwSize == 0 || (INT)dwSize < 0  || dwSize % sizeof(KLINEDATA) != 0 || dwSize > sizeof(KLINEDATA) * LOAD_BUFFER_SIZE )
				::MessageBox(NULL,lpszLabel,szlpFileName,MB_OK);

			vkl.assign(&m_LoadKLineBuffer[0],&m_LoadKLineBuffer[ dwSize / sizeof(KLINEDATA) ]);
			return TRUE;
		}
		return FALSE;
	}

	BOOL SaveKLine(const char * lpszLabel , const char * szlpFileName , VKL & vkl)
	{
		if(vkl.size() == 0)
			return FALSE;

		DWORD dwSize = vkl.size() * sizeof(KLINEDATA);
		return m_DirMgr.SaveFile(lpszLabel,szlpFileName,&vkl[0] ,dwSize);	
	}
	

	BOOL Save()
	{ 
		map< string, ALLVKL>::iterator		MapBegin = m_MData.begin();
		map< string, ALLVKL >::iterator		MapEnd   = m_MData.end();	
		
		INT nMKLine = GetSize();
		CExtenLogDlg ExtenLogDlg;
		ExtenLogDlg.InsertLog("KLineDataMgr::Save  m_MData.size() = %d ",nMKLine);
		INT nSaveCount=0;
		for(MapBegin; MapBegin!= MapEnd; ++MapBegin )
		{
			SaveKLine(MapBegin->first.c_str(),"Day.dat",MapBegin->second.Day);
			SaveKLine(MapBegin->first.c_str(),"F5.dat",MapBegin->second.F5);
			SaveKLine(MapBegin->first.c_str(),"F1.dat",MapBegin->second.F1);
			ExtenLogDlg.InsertLog("KLineDataMgr::SaveKLine %s зм:%d ЪЃгр:%d",MapBegin->first.c_str(),nMKLine,nMKLine - nSaveCount++);
		}
		return TRUE;
	}

	
	BOOL Init()
	{
		m_LoadKLineBuffer  = new KLINEDATA[LOAD_BUFFER_SIZE];
		if(m_LoadKLineBuffer == NULL)
			return FALSE;

		INT nLoadCount = 0;
		INT nDirCount = m_DirMgr.Size();
		CExtenLogDlg ExtenLogDlg;
		ExtenLogDlg.InsertLog("KLineDataMgr::Init SDir.size() = %d ",nDirCount);


		set<string> SDir = m_DirMgr.Get();
		for (set<string>::iterator  SDirIt = SDir.begin(); SDirIt != SDir.end(); ++SDirIt)
		{
			string s = *SDirIt;

			ALLVKL AllVkl;
			pair< map< string, ALLVKL >::iterator, bool> ip= m_MData.insert( pair< string, ALLVKL >(s.c_str(),AllVkl) );
			if(ip.second == false)
				continue;
			
			LoadKLine(s.c_str(),"F1.dat",ip.first->second.F1);
			ip.first->second.F1.reserve( ip.first->second.F1.size() + 242);

			LoadKLine(s.c_str(),"F5.dat",ip.first->second.F5);
			ip.first->second.F5.reserve( ip.first->second.F5.size() + 50);

			LoadKLine(s.c_str(),"Day.dat",ip.first->second.Day);
			ip.first->second.Day.reserve( ip.first->second.Day.size() + 1);

			ExtenLogDlg.InsertLog("KLineDataMgr::LoadKLine %s зм:%d ЪЃгр:%d",s.c_str(),nDirCount,nDirCount - nLoadCount++);
		}
		return m_MData.size()? TRUE : FALSE;
	}
	
	BOOL UpdateMinuteKLine( const RCV_REPORT_STRUCTExV3 * pNewRRS ,const RCV_REPORT_STRUCTExV3 * pOldRRS)
	{
		map<string,ALLVKL>::iterator it = m_MData.find((const char *)&pNewRRS->m_wMarket);
		if(it == m_MData.end())
			return FALSE;

// 		if(stricmp((const char *)&pNewRRS->m_wMarket,"SZ002702") != 0)
// 			return FALSE;


		UpdateKLine(it->second.F1,pNewRRS,pOldRRS);
		UpdateKLine(it->second.F5,pNewRRS,pOldRRS);
		return TRUE;
	}

	void UpdateKLine(VKL & vkl ,const RCV_REPORT_STRUCTExV3 * pNewRRS ,const RCV_REPORT_STRUCTExV3 * pOldRRS)
	{
		LPKLINEDATA pEnd = &vkl[ vkl.size() - 1];
		if( Amount_Equality(pEnd->fVolume, 0.00f) )										
		{																		
			pEnd->fOpen   = pNewRRS->m_fNewPrice;																	
			pEnd->fHigh   = pNewRRS->m_fNewPrice;									
			pEnd->fLow    = pNewRRS->m_fNewPrice;
			pEnd->fClose   = pNewRRS->m_fNewPrice;	
			if(pOldRRS == NULL || Price_Equality(pOldRRS->m_fOpen,0.00f))
			{
				pEnd->fVolume = pNewRRS->m_fVolume;										
				pEnd->fAmount = pNewRRS->m_fAmount;
			}else
			{
				pEnd->fVolume  += pNewRRS->m_fVolume - pOldRRS->m_fVolume;										
				pEnd->fAmount  += pNewRRS->m_fAmount - pOldRRS->m_fAmount;
			}
			pEnd->tUnix    = pNewRRS->m_time;
		}else																	
		{
			if(pNewRRS->m_fNewPrice > pEnd->fHigh)									
				pEnd->fHigh = pNewRRS->m_fNewPrice;
			
			if(pNewRRS->m_fNewPrice < pEnd->fLow)									
				pEnd->fLow =  pNewRRS->m_fNewPrice;

			pEnd->fVolume  += pNewRRS->m_fVolume - pOldRRS->m_fVolume;										
			pEnd->fAmount  += pNewRRS->m_fAmount - pOldRRS->m_fAmount;
			pEnd->tUnix    = pNewRRS->m_time;
			pEnd->fClose   = pNewRRS->m_fNewPrice;	
		}
	}

	void NewRRSToKLine(const RCV_REPORT_STRUCTExV3 & NewRRS,KLINEDATA & KLineData)
	{
		KLineData.tUnix = NewRRS.m_time;
		KLineData.fOpen = NewRRS.m_fOpen;
		KLineData.fClose= NewRRS.m_fNewPrice;
		KLineData.fHigh = NewRRS.m_fHigh;
		KLineData.fLow  = NewRRS.m_fLow;
		KLineData.fAmount=NewRRS.m_fAmount;
		KLineData.fVolume=NewRRS.m_fVolume;
	}


	BOOL UpdateDayKLine(const RCV_REPORT_STRUCTExV3 & NewRRS)
	{
		map<string,ALLVKL>::iterator it = m_MData.find((const char *)&NewRRS.m_wMarket);
		if(it == m_MData.end())
		{
			ALLVKL AllVkl;
			pair< map< string, ALLVKL >::iterator, bool> ip= m_MData.insert( pair< string, ALLVKL >((const char *)&NewRRS.m_wMarket,AllVkl) );
			if(ip.second == true)
			{
				KLINEDATA  KLineData;
				NewRRSToKLine(NewRRS,KLineData);
				ip.first->second.Day.push_back(KLineData);
				ip.first->second.F1.reserve(242);
				ip.first->second.F5.reserve(50);
				return TRUE;
			}
		}else
		{
			ALLVKL * pVkl = &it->second;
			KLINEDATA  KLineData;
			NewRRSToKLine(NewRRS,KLineData);
			LPKLINEDATA pEnd = &pVkl->Day[ pVkl->Day.size() - 1];
			CTime t1(KLineData.tUnix),t2(pEnd->tUnix);
			if(t1.GetDay() != t2.GetDay())
			{
				pVkl->Day.push_back(KLineData);
			}else
			{
				*pEnd = KLineData;
			}
			return TRUE;
		}
		return FALSE;
	}


	BOOL KLineNotUpdated(ALLVKL & AllVKL , CTime & Time)
	{
		CTime TDay = AllVKL.Day[ AllVKL.Day.size() - 1 ].tUnix;
		if(TDay.GetDay() != Time.GetDay())
			return FALSE;
		return TRUE;
	}

	INT	AddF1KLine()
	{
		map< string, ALLVKL>::iterator		MapBegin	=  m_MData.begin();
		map< string, ALLVKL>::iterator		MapEnd		=  m_MData.end();	
		
		KLINEDATA KLineData;
		INT nCount = 0;
		CTime Time = CTime::GetCurrentTime();
		for(MapBegin; MapBegin!= MapEnd; ++MapBegin )
		{
			if(!KLineNotUpdated(MapBegin->second,Time))
				continue;

			if(MapBegin->second.F1.size() == 0)
			{
				ZeroMemory(&KLineData,sizeof(KLINEDATA));
				KLineData.tUnix      = Time.GetTime();
				MapBegin->second.F1.push_back(KLineData);
			}else
			{
				LPKLINEDATA pEnd = &MapBegin->second.F1[ MapBegin->second.F1.size() - 1];
				KLineData.fOpen   = pEnd->fClose;
				KLineData.fClose  = pEnd->fClose;
				KLineData.fHigh   = pEnd->fClose;
				KLineData.fLow    = pEnd->fClose;
				KLineData.fVolume = 0.00f;
				KLineData.fAmount = 0.00f;
				KLineData.tUnix   = Time.GetTime();
				MapBegin->second.F1.push_back(KLineData);
			}
			nCount++;
		}
		return nCount;
	}

	INT AddF5KLine()
	{
		map< string, ALLVKL>::iterator		MapBegin	=  m_MData.begin();
		map< string, ALLVKL>::iterator		MapEnd		=  m_MData.end();	
		
		KLINEDATA KLineData;
		INT nCount = 0;
		CTime Time = CTime::GetCurrentTime();
		for(MapBegin; MapBegin!= MapEnd; ++MapBegin )
		{
			if(!KLineNotUpdated(MapBegin->second,Time))
				continue;

			if(MapBegin->second.F5.size() == 0)
			{
				ZeroMemory(&KLineData,sizeof(KLINEDATA));
				KLineData.tUnix      = Time.GetTime();
				MapBegin->second.F5.push_back(KLineData);
			}else
			{
				LPKLINEDATA pEnd = &MapBegin->second.F1[ MapBegin->second.F1.size() - 1];
				KLineData.fOpen   = pEnd->fClose;
				KLineData.fClose  = pEnd->fClose;
				KLineData.fHigh   = pEnd->fClose;
				KLineData.fLow    = pEnd->fClose;
				KLineData.fVolume = 0.00f;
				KLineData.fAmount = 0.00f;
				KLineData.tUnix   = Time.GetTime();
				MapBegin->second.F5.push_back(KLineData);
			}
			nCount++;
		}
		return nCount;
	}


	BOOL Select(const char * lpszFullLable , map< string, ALLVKL >::iterator & ItFind)
	{
		ItFind = m_MData.find(lpszFullLable);
		if(ItFind == m_MData.end())
			return FALSE;
	
		return TRUE;
	}

	map<string, ALLVKL >  * GetMapPoint() { return &m_MData; }
	DWORD GetSize() { return m_MData.size(); }

private:
	LPKLINEDATA m_LoadKLineBuffer;
	CDirectoryMgr m_DirMgr;
	map<string,ALLVKL> m_MData;
};


#endif // KLineDataMgr_h__