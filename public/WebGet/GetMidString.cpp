#include "StdAfx.h"
#include "GetMidString.h"




BOOL _dzjy_GetMidString(
				  const char * pTextBufferPoint,
				  char* leftKeyWords,
				  char* RightKeyWords,
				  std::string& OutMidString,
				  const char** lpNewOffset)
{
	
	pTextBufferPoint = StrStr(pTextBufferPoint,leftKeyWords);
	if (pTextBufferPoint == NULL)
		return FALSE;
	
	pTextBufferPoint+= strlen(leftKeyWords);
	char* pNextFlag = StrStr(pTextBufferPoint,RightKeyWords);
	if (pNextFlag == NULL)
		return FALSE;
	

	std::string strGDRS(pTextBufferPoint,pNextFlag-pTextBufferPoint);
	OutMidString = strGDRS;
	

	if (lpNewOffset != NULL)
		*lpNewOffset = pNextFlag;

	return TRUE;
}


void _dzjy_replace(std::string&s1,const std::string s2,const std::string s3)
{
	std::string::size_type pos=0;
	std::string::size_type a=s2.size();
	std::string::size_type b=s3.size();
	while((pos=s1.find(s2,pos))!=std::string::npos)
	{
		s1.replace(pos,a,s3);
		pos+=b;
	}
}



// {sad[fsd[[ 123 ]]adfsa]fsdaf} 如果有这样一个字符串需要取中间
//leftKeyWords1 == {
//RightKeyWords1 == }
//leftKeyWords2 == [[
//RightKeyWords2 == ]]
BOOL _dzjy_GetMidStringEx(
const char * pTextBufferPoint,
char* leftKeyWords1,  
char* RightKeyWords1,
char* leftKeyWords2,
char* RightKeyWords2,
std::string& OutMidString,
const char** lpNewOffset
)
{
	std::string buffer1;
	if (!_dzjy_GetMidString(pTextBufferPoint,leftKeyWords1,RightKeyWords1,buffer1))
		return FALSE;
	
	const char* pp1 = buffer1.c_str();
	if (!_dzjy_GetMidString(pp1,leftKeyWords2,RightKeyWords2,OutMidString))
		return FALSE;
	
	return TRUE;
}


BOOL _dzjy_GetValue(
					 LPCTSTR lpszContext ,
					 LPCTSTR lpszFirstBegin, 
					 LPCTSTR lpszBegin,
					 LPCTSTR lpszEnd,
					 string& OutMidString,const char** lpszNewOffset)
{
	if(lpszBegin ==NULL || lpszEnd == NULL)
		return FALSE;
	
	LPCTSTR lpszFindBegin = lpszContext;
	if(lpszFirstBegin)	
	{
		lpszFindBegin = strstr(lpszContext,lpszFirstBegin);
		if(lpszFindBegin == NULL)
			return FALSE;
		
		lpszFindBegin += strlen(lpszFirstBegin);
	}
	
	
	lpszFindBegin=strstr(lpszFindBegin,lpszBegin);
	if(lpszFindBegin == NULL)
		return FALSE;
	
	
	lpszFindBegin+= strlen(lpszBegin);
	LPTSTR lpszFindEnd  = strstr(lpszFindBegin,lpszEnd);
	if(lpszFindEnd == NULL)
		return FALSE;
	
	

	std::string strGDRS(lpszFindBegin,lpszFindEnd - lpszFindBegin);
	OutMidString = strGDRS;

	if(lpszNewOffset) *lpszNewOffset = lpszFindEnd;
	
	return TRUE;
}
