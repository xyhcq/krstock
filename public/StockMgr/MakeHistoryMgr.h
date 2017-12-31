
#ifndef MakeHistoryMgr_h__
#define MakeHistoryMgr_h__



#include <map>
#include <string>
using namespace std;


typedef struct _MHDATE
{
	WORD   wMarket;
    char   szLabel[8];
	time_t  tLHB[100];
	time_t  tGGCG[50];
	time_t  tCWBG[20];
	time_t  tYJBG[50];
}MHDATE,*LPMHDATE;


class CMakeHistoryMgr
{
public:
	CMakeHistoryMgr()
	{
		MakeDataPath("MHDate.db",m_szDbPath);
	}

	BOOL Init()
	{
		MHDATE * pMHDate = new MHDATE[ 5000 ];
		if(pMHDate == NULL)
			return FALSE;
		
		INT nSize = 5000 * sizeof(MHDATE);
		if(LoadFile(m_szDbPath,pMHDate,nSize) <=0 )
		{
			delete []pMHDate;
			return FALSE;
		}
		
		nSize = nSize/sizeof(MHDATE);
		for (INT i=0;i<nSize;i++)
			m_MData.insert( pair<string,MHDATE>( (const char *)&pMHDate[i].wMarket,pMHDate[i]) );


		delete []pMHDate;

		return m_MData.size() ? TRUE : FALSE;
	}


	BOOL Init(MHDATE * pMHDate,DWORD dwSize)
	{
		m_MData.clear();
		dwSize = dwSize/sizeof(MHDATE);
		for (INT i=0;i<dwSize;i++)
			m_MData.insert( pair<string,MHDATE>((const char *)&pMHDate[i].wMarket,pMHDate[i]) );

		return m_MData.size() ? TRUE : FALSE;
	}



	BOOL Destory()
	{
		if(m_MData.size() == 0)
			return TRUE;
		
		MHDATE * pMHDate = new MHDATE[ 5000 ];
		if(pMHDate == NULL)
			return FALSE;
		
		map<string,MHDATE>::iterator it;
		MHDATE * pOffset = pMHDate;
		
		for (it = m_MData.begin(); it != m_MData.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(MHDATE));
			pOffset++;
		}

		INT nSize = m_MData.size() * sizeof(MHDATE);
		SaveFile(m_szDbPath,pMHDate,nSize);
		
		delete [] pMHDate;
		return TRUE;
	}


	BOOL Insert(const MHDATE & MHDate)
	{
		map<string,MHDATE>::iterator it = m_MData.find((const char *)&MHDate.wMarket);
		
		//没有找到 则插入新的
		if(it == m_MData.end())
		{
			m_MData.insert( pair<string,MHDATE>( (const char *)&MHDate.wMarket , MHDate ) );
			return TRUE;
		}else
		{
			//更新数据为CurRRS
			it->second = MHDate;
		}
		return TRUE;
	}


	map<string, MHDATE> * GetMapPoint(){return &m_MData; }

	LPMHDATE Select(const char * szFullLabel){

		map<string,MHDATE>::iterator it = m_MData.find(szFullLabel);
		if(it == m_MData.end())
			return NULL;
		return &it->second;
	}


	BYTE * GetMemoryObject( DWORD & dwOBSize )
	{
		if(m_MData.size() == 0)
			return NULL;
		
		INT n = m_MData.size();
		MHDATE * pMHDate = new MHDATE[ m_MData.size() ];
		if(pMHDate == NULL)
			return FALSE;
		
		MHDATE * pOffset = pMHDate;

		map<string,MHDATE>::iterator it;
		for (it = m_MData.begin(); it != m_MData.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(MHDATE));
			pOffset++;
		}
		
		dwOBSize = m_MData.size() * sizeof(MHDATE);
		return (BYTE *)pMHDate;
	}
	
	BOOL FreeMemoryObject(BYTE * MemoryObject, DWORD dwOBSize)
	{
		delete [] MemoryObject;
		return TRUE;
	}

	INT GetSize(){ return m_MData.size(); }

private:
	CHAR  m_szDbPath[MAX_PATH];
	map<string, MHDATE> m_MData;
};





#endif // MakeHistoryMgr_h__