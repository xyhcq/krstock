#ifndef THSDataMgr_h__
#define THSDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"

#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

#pragma pack(push,1)

typedef struct _THSDATA
{	
	char  szLabel[10];
	char  szHangYe[20];
	float fSYLD_BL;
}THSDATA,*LPTHSDATA;

#pragma pack(pop)




class CTHSDataMgr
{
public:
	CTHSDataMgr(){
		MakeDataPath("THSData.db",m_szDbPath);
	}
	
	BOOL InsertWebHQ(const char *  szLabel,const char *  szHangYe , float & fSYLD_BL )
	{
		if(szLabel == NULL)
			return  FALSE;

		THSDATA THSData;
		ZeroMemory(&THSData,sizeof(THSData));
		if(szLabel) strcpy(THSData.szLabel,szLabel);
		if(szHangYe) strcpy(THSData.szHangYe,szHangYe);
		THSData.fSYLD_BL = fSYLD_BL;

		map<string,THSDATA>::iterator it = m_THSData.find(szLabel);
		
		//没有找到 则插入新的
		if(it == m_THSData.end())
		{
			m_THSData.insert( pair<string,THSDATA>( szLabel , THSData ) );
			return TRUE;
		}else
		{
			it->second = THSData;
		}
		return TRUE;
	}

	const LPTHSDATA Select(const char *  szLabel)
	{
		map<string,THSDATA>::iterator it = m_THSData.find(szLabel);
		if(it == m_THSData.end())
			return NULL;
		
		return &it->second;
	}

	BOOL Init()
	{
		LPTHSDATA pTHSData = new THSDATA[ 5000 ];
		if(pTHSData == NULL)
			return FALSE;
		
		INT nSize = 5000 * sizeof(THSDATA);
		if(LoadFile(m_szDbPath,pTHSData,nSize) <=0 )
		{
			delete [] pTHSData;
			return FALSE;
		}
		
		nSize = nSize/sizeof(THSDATA);
		for (INT i=0;i<nSize;i++)
			m_THSData.insert( pair<string,THSDATA>(pTHSData[i].szLabel,pTHSData[i]) );
		

		delete []pTHSData;
		return m_THSData.size() ? TRUE : FALSE;
	}

	BOOL Destory()
	{
		if(m_THSData.size() == 0)
			return TRUE;
		
		LPTHSDATA  pTHSData = new THSDATA[ 5000 ];
		if(pTHSData == NULL)
			return FALSE;
		
		map<string,THSDATA>::iterator it;
		
		LPTHSDATA pOffset = pTHSData;
		
		for (it = m_THSData.begin(); it != m_THSData.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(THSDATA));
			pOffset++;
		}
		

		INT nSize = m_THSData.size() * sizeof(THSDATA);
		SaveFile(m_szDbPath,pTHSData,nSize);
		
		delete [] pTHSData;
		return TRUE;
	}
	DWORD GetSize() { return m_THSData.size(); }
private:
	map<string , THSDATA > m_THSData;
	CHAR  m_szDbPath[MAX_PATH];	
};

#endif // THSDataMgr_h__