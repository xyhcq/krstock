#ifndef TickerSymbol_h__
#define TickerSymbol_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "StockMgr/StockCommon.h"

#include <map>
#include <vector>
#include <string>
using namespace std;


class CTickerSymbolDataMgr
{
public:
	CTickerSymbolDataMgr(){
		MakeDataPath("TickerSymbol.db",m_szDbPath);
	}

	BOOL Init(STOCK_TICKER_SYMBOL * Memory , DWORD dwSize)
	{
		dwSize = dwSize/sizeof(STOCK_TICKER_SYMBOL);

		m_VTbale.clear();
		m_VTbale.reserve(5000);
		m_VTbale.assign(&Memory[0],&Memory[dwSize]);
		
		for (INT i=0;i<dwSize;i++)
			m_MTable.insert( pair<string,INT>((const char *)&m_VTbale[i].m_wMarket,i));
	
		return m_MTable.size() ? TRUE : FALSE;
	}


	BOOL Init()
	{
		STOCK_TICKER_SYMBOL Temp[5000];
		INT nSize = 5000 * sizeof(STOCK_TICKER_SYMBOL);
		if(LoadFile(m_szDbPath,Temp,nSize) <= 0 )
		{
			m_VTbale.resize(0);
			return FALSE;
		}
		
		nSize = nSize/sizeof(STOCK_TICKER_SYMBOL);
	
		m_VTbale.clear();
		m_VTbale.reserve(5000);
		m_VTbale.assign(&Temp[0],&Temp[nSize]);
	
		for (INT i=0;i<nSize;i++)
			m_MTable.insert( pair<string,INT>((const char *)&m_VTbale[i].m_wMarket,i));

		return m_MTable.size() ? TRUE : FALSE;
	}

	
	BOOL Destory()
	{
		INT nSize = m_VTbale.size() * sizeof(STOCK_TICKER_SYMBOL);
		SaveFile(m_szDbPath,m_VTbale.begin(),nSize);
		return TRUE;
	}
	


	BOOL Insert(const RCV_TABLE_STRUCT & rts , WORD wMarket)
	{
		char szFullLabel[STKLABEL_LEN]={0};
		MakeFullLabel(szFullLabel,rts.m_szLabel,wMarket);
		map<string,INT>::iterator  it = m_MTable.find(szFullLabel);

		//查找到了，更新数据
		if(it != m_MTable.end())	
		{	
			m_VTbale [ it->second ].m_wMarket = wMarket;
			strcpy( m_VTbale [ it->second ].m_szLabel , rts.m_szLabel);
			strcpy( m_VTbale [ it->second ].m_szName , rts.m_szName);
			m_VTbale [ it->second ].m_cProperty = rts.m_cProperty;
			return FALSE;
		}else
		{
			STOCK_TICKER_SYMBOL  sts;
			sts.m_wMarket = wMarket;
			strcpy( sts.m_szLabel , rts.m_szLabel);
			strcpy( sts.m_szName , rts.m_szName);
			sts.m_cProperty = rts.m_cProperty;
			
			m_MTable.insert(pair<string,INT>(szFullLabel, m_VTbale.size()));
			m_VTbale.push_back(sts);
		}
		return TRUE;
	}

	INT Index(const char * szFullLabel)
	{
		map<string,INT>::iterator  it = m_MTable.find(szFullLabel);
		if(it == m_MTable.end())
			return -1;

		return it->second;
	}

	STOCK_TICKER_SYMBOL * Select(INT nIndex)
	{
		if(nIndex<0 || nIndex > m_VTbale.size() )
			return NULL;

		return &m_VTbale[nIndex];
	}

	const char * GetName(const char * szFullLabel)
	{
		map<string,INT>::iterator  it = m_MTable.find(szFullLabel);
		if(it == m_MTable.end())
			return "";
		return m_VTbale [ it->second ].m_szName;
	}

	
	BYTE * GetMemoryObject( DWORD & dwOBSize ){
		dwOBSize = m_VTbale.size() * sizeof(STOCK_TICKER_SYMBOL);
		return (BYTE *)&m_VTbale[0];
	}
	
	BOOL FreeMemoryObject( BYTE * MemoryObject, DWORD dwOBSize) {
		return TRUE;
	}

	vector< STOCK_TICKER_SYMBOL >  & GetVector()    { return m_VTbale;  }
	const LPSTOCK_TICKER_SYMBOL    GetTable(){    return m_VTbale.begin(); }
	DWORD GetSize(){ return m_VTbale.size(); }
private:
	map<string,INT>	   m_MTable;
	vector< STOCK_TICKER_SYMBOL > m_VTbale;
	CHAR  m_szDbPath[MAX_PATH];
};



#endif // TickerSymbol_h__