#ifndef FinanceDataMgr_h__
#define FinanceDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "StockMgr/StockCommon.h"


#include <map>
#include <string>
using namespace std;


class CFinancialDataMgr
{

public:
	CFinancialDataMgr(){
		MakeDataPath("FinanceData.db",m_szDbPath);
	}

	~CFinancialDataMgr(){}

	BOOL Init(){

		Fin_LJF_STRUCTEx * pFJS = new Fin_LJF_STRUCTEx[ 5000 ];
		if(pFJS == NULL)
			return FALSE;
		
		INT nSize = 5000 * sizeof(Fin_LJF_STRUCTEx);
		if(LoadFile(m_szDbPath,pFJS,nSize) <= 0)
		{
			delete []pFJS;
			return FALSE;
		}
		
		
	
		m_FinaceMap.clear();
		nSize = nSize/sizeof(Fin_LJF_STRUCTEx);

		char szFullLabel[100]={0};
		for (INT i=0;i<nSize;i++)
		{
			MakeFullLabel(szFullLabel,pFJS[i].m_szLabel,pFJS[i].m_wMarket);
			m_FinaceMap.insert( pair<string,Fin_LJF_STRUCTEx>(szFullLabel,pFJS[i]) );
		}

		delete []pFJS;
		return m_FinaceMap.size() ? TRUE : FALSE;
	}



	BOOL Init(Fin_LJF_STRUCTEx * pFJS,DWORD dwSize){
		m_FinaceMap.clear();
	
		dwSize = dwSize/sizeof(Fin_LJF_STRUCTEx);
		
		char szFullLabel[STKLABEL_LEN]={0};
		for (INT i=0;i<dwSize;i++)
		{
			MakeFullLabel(szFullLabel,pFJS[i].m_szLabel,pFJS[i].m_wMarket);
			m_FinaceMap.insert( pair<string,Fin_LJF_STRUCTEx>(szFullLabel,pFJS[i]) );
		}
		return m_FinaceMap.size() ? TRUE : FALSE;
	}


	BOOL Destory(){

		if(m_FinaceMap.size() == 0)
			return TRUE;

		Fin_LJF_STRUCTEx * pFJS = new Fin_LJF_STRUCTEx[ m_FinaceMap.size() ];
		if(pFJS == NULL)
			return FALSE;
		
		map<string,Fin_LJF_STRUCTEx>::iterator it;
		
		Fin_LJF_STRUCTEx * pOffset = pFJS;
		
		for (it = m_FinaceMap.begin(); it != m_FinaceMap.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(Fin_LJF_STRUCTEx));
			pOffset++;
		}
		
		INT nSize = m_FinaceMap.size() * sizeof(Fin_LJF_STRUCTEx);
		SaveFile(m_szDbPath,pFJS,nSize);
		delete []pFJS;
		return TRUE;
	}


	BOOL Insert(const Fin_LJF_STRUCTEx & fls){

		pair< map<string,Fin_LJF_STRUCTEx>::iterator, bool> Insert_Pair;

		char szFullLabel[100]={0};
		MakeFullLabel(szFullLabel,fls.m_szLabel,fls.m_wMarket);

		Insert_Pair = m_FinaceMap.insert( pair<string,Fin_LJF_STRUCTEx>(szFullLabel,fls));
		
		//插入失败，更新数据
		if(Insert_Pair.second == FALSE)
			Insert_Pair.first->second = fls;
		
		return TRUE;
	}
	
	
	PFin_LJF_STRUCTEx Select(const char * szFullLabel){
		map<string,Fin_LJF_STRUCTEx>::iterator it = m_FinaceMap.find(szFullLabel);
		if(it == m_FinaceMap.end())
			return NULL;
		return &it->second;
	}


	DWORD GetSize(){ return m_FinaceMap.size(); } 
	
	map<string,Fin_LJF_STRUCTEx> & GetMap(){ 
		return m_FinaceMap; 
	}

	BYTE * GetMemoryObject( DWORD & dwOBSize ){
		if(m_FinaceMap.size() == 0)
			return FALSE;
		
		INT n = m_FinaceMap.size();

		Fin_LJF_STRUCTEx * pFJS = new Fin_LJF_STRUCTEx[ m_FinaceMap.size() ];
		if(pFJS == NULL)
			return FALSE;
		
		map<string,Fin_LJF_STRUCTEx>::iterator it;
		
		Fin_LJF_STRUCTEx * pOffset = pFJS;
		
		for (it = m_FinaceMap.begin(); it != m_FinaceMap.end(); ++it)
		{
			CopyMemory(pOffset,&it->second,sizeof(Fin_LJF_STRUCTEx));
			pOffset++;
		}

		dwOBSize = m_FinaceMap.size() * sizeof(Fin_LJF_STRUCTEx);

		n= dwOBSize / sizeof(Fin_LJF_STRUCTEx);
		return (BYTE *)pFJS;
	}



	BOOL FreeMemoryObject(BYTE * MemoryObject, DWORD dwOBSize) {
		delete [] MemoryObject;
		return TRUE;
	}

private:
	map<string,Fin_LJF_STRUCTEx> m_FinaceMap;
	CHAR  m_szDbPath[MAX_PATH];
};









#endif // FinanceDataMgr_h__