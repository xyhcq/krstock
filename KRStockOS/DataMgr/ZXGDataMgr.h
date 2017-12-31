#ifndef ZXGDataMgr_h__
#define ZXGDataMgr_h__

#include <map>
#include <string>
using namespace std;


class CZXGDataMgr
{
public:
	CZXGDataMgr()
	{
		ZeroMemory(m_szFilePath,sizeof(m_szFilePath));
		TCHAR szFilePath[MAX_PATH]={0};
		MakeDataPath("ZXG.db", m_szFilePath);
	}

	const REALTIMEQUOTES * Select(const char * szLabel)
	{
		map<string, REALTIMEQUOTES >::iterator  it = m_MData.find(szLabel);
		if(it == m_MData.end())
			return NULL;

		return &it->second;
	}

	BOOL Delete(const char * szLabel)
	{
		map<string, REALTIMEQUOTES >::iterator  it = m_MData.find(szLabel);
		if(it == m_MData.end())
			return FALSE;
		
		m_MData.erase(it);
		return TRUE;
	}


	BOOL Insert(const REALTIMEQUOTES & RRS)
	{
		pair< map<string, REALTIMEQUOTES>::iterator , bool> ip = m_MData.insert( pair<string, REALTIMEQUOTES>((const char *)&RRS.m_wMarket ,RRS)  );
		return ip.second;
	}


	BOOL Save()
	{	
		vector<REALTIMEQUOTES> VRTQ;
		map<string, REALTIMEQUOTES > ::iterator  it = m_MData.begin();
		for (it; it!= m_MData.end(); ++it)
			VRTQ.push_back(it->second);
		
 		INT nSize = VRTQ.size() * sizeof(REALTIMEQUOTES);
 		return SaveFile(m_szFilePath,&VRTQ[0],nSize);
	}


	BOOL Load()
	{
		REALTIMEQUOTES * pRTQ = new REALTIMEQUOTES[5000];
		if(pRTQ == NULL)
			return FALSE;

		INT nSize = sizeof(REALTIMEQUOTES) * 5000;
		if(!LoadFile(m_szFilePath,pRTQ,nSize))
		{
			delete [] pRTQ;
			return FALSE;
		}
		
		nSize /= sizeof(REALTIMEQUOTES);
		if(nSize <= 0 )
		{
			delete [] pRTQ;
			return FALSE;
		}

		for(INT i=0; i < nSize ; i++)
			m_MData.insert( pair<string, REALTIMEQUOTES>((const char *)&pRTQ[i].m_wMarket ,pRTQ[i] ) );

		delete [] pRTQ;
		return TRUE;
	}
	map<string, REALTIMEQUOTES > & GetMap() { return m_MData; }

private:
	TCHAR m_szFilePath[MAX_PATH];
	map<string, REALTIMEQUOTES > m_MData;
};



#endif // ZXGDataMgr_h__