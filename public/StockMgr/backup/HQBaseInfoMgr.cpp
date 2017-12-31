#include "StdAfx.h"
#include "HQBaseInfoMgr.h"
#include "THSDataMgr.h"

BOOL CHQBaseInfoDataMgr::Get5DayInfo( const VKL & KLDay , float & f5DayVolume , float & f5DayAmount )
{
	if(KLDay.size() == 0 )
		return FALSE;
	
	INT nCount = 0;
	for (INT i = KLDay.size() - 2; i >= 0 ; i--)
	{
		f5DayVolume	 += KLDay[i].fVolume;
		f5DayAmount	 += KLDay[i].fAmount;
		if(++nCount == 5)
			break;
	}
	return TRUE;
}


LPHQBASE_INFO CHQBaseInfoDataMgr::Select( const char * szFullLabel )
{
	map<string,HQBASE_INFO>::iterator it = m_MData.find(szFullLabel);
	if(it == m_MData.end())
		return NULL;
	
	return &it->second;
}



BOOL FindFileKeyDataDWORD(DWORD& dwOutData,const char * pTextBufferPoint,char* leftKeyWords,char* RightKeyWords)
{
	pTextBufferPoint = StrStr(pTextBufferPoint,leftKeyWords);
	if (pTextBufferPoint == NULL)
		return FALSE;
	
	pTextBufferPoint+= strlen(leftKeyWords);
	char* pNextFlag = StrStr(pTextBufferPoint,RightKeyWords);
	if (pNextFlag == NULL)
		return FALSE;
	
	while(*pTextBufferPoint == ' ')
		pTextBufferPoint++;
	std::string strGDRS(pTextBufferPoint,pNextFlag-pTextBufferPoint);
	dwOutData = atoi(strGDRS.c_str());

	return TRUE;
}



BOOL FindFileKeyDataFloat(float& dwOutData,const char * pTextBufferPoint,char* leftKeyWords,char* RightKeyWords)
{
	pTextBufferPoint = StrStr(pTextBufferPoint,leftKeyWords);
	if (pTextBufferPoint == NULL)
		return FALSE;
	
	pTextBufferPoint+= strlen(leftKeyWords);
	char* pNextFlag = StrStr(pTextBufferPoint,RightKeyWords);
	if (pNextFlag == NULL)
		return FALSE;
	
	while(*pTextBufferPoint == ' ')
		pTextBufferPoint++;

	std::string strGDRS(pTextBufferPoint,pNextFlag-pTextBufferPoint);
	dwOutData = atof(strGDRS.c_str());
	return TRUE;
}



BOOL GetF10DataInfo(DWORD & dwGDRS, DWORD & dwHJCG ,float& fJSQBH,float& fQSDLTGD,const char * pTextBuffer)
{
	char* pFind =StrStr(pTextBuffer,"股东户数变化");
	if (pFind == NULL)
		return FALSE;
	pFind += _tclen("股东户数变化");

	pFind = StrStr(pFind,"股东户数变化");
	if (pFind == NULL)
		return FALSE;
	pFind += strlen("股东户数变化");


	if (!FindFileKeyDataDWORD(dwGDRS,pFind,"股东人数(户)  ｜","｜"))
		return FALSE;

	if (!FindFileKeyDataDWORD(dwHJCG,pFind,"户均持股(股)  ｜","｜"))
		return FALSE;
	
	if (!FindFileKeyDataFloat(fJSQBH,pFind,"较上期变化(%) ｜","｜"))
		return FALSE;
	
	if (!FindFileKeyDataFloat(fQSDLTGD,pFind,"前十大流通股东｜","｜"))
		return FALSE;

	return TRUE;
}







BOOL CHQBaseInfoDataMgr::Init( const map< string , VKL > * pMap )
{
	DWORD dwF10DataBufferSize  = 1024*1024*5;
	char* pF10DataBuffer = new char[dwF10DataBufferSize];
	if(pF10DataBuffer == NULL)
		return FALSE;

	if(pMap == NULL || pMap->size() == 0)
		return FALSE;
	

	HQBASE_INFO  BaseInfo;
	CTHSDataMgr THSDataMgr;
	THSDataMgr.Init();


	CFileMgr FileMgr;
	FileMgr.Init("F10Data");
	char szFileName[MAX_PATH]={0};

	for (map< string , VKL >::const_iterator it= pMap->begin(); it != pMap->end(); ++it)
	{
		ZeroMemory(&BaseInfo,sizeof(HQBASE_INFO));

		LPTHSDATA pTHSData    = THSDataMgr.Select(it->first.c_str());
		if(pTHSData)
		{
			BaseInfo.fSYLD_BL = pTHSData->fSYLD_BL;
			strcpy(BaseInfo.szHangYe,pTHSData->szHangYe);
		}
		
		strcpy(szFileName,it->first.c_str());
		strcat(szFileName,".TXT");

		dwF10DataBufferSize = 1024*1024*5;
		ZeroMemory(pF10DataBuffer,dwF10DataBufferSize);
		if(FileMgr.LoadFile(szFileName,pF10DataBuffer,dwF10DataBufferSize))
			GetF10DataInfo(BaseInfo.dwGDS,BaseInfo.dwRJCG,BaseInfo.fJSQBH,BaseInfo.fQSDLTGD,pF10DataBuffer);


		if(Get5DayInfo(it->second,BaseInfo.f5DayVolume,BaseInfo.f5DayAmount))
		{
			strcpy(BaseInfo.szLabel,it->first.c_str());
			m_MData.insert( pair< string, HQBASE_INFO >(it->first.c_str(),BaseInfo) );
		}
	}


	delete [] pF10DataBuffer;
	return m_MData.size() ? TRUE : FALSE;
}

