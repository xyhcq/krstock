#include "StdAfx.h"
#include "THSWebHQ.h"
#include "Http.h"
#include "cJSON.h"
#include "zlib.h"
#pragma comment(lib,"Zlib.lib")

static BOOL GetValue(LPCTSTR lpszContext ,LPCTSTR lpszFirstBegin, LPCTSTR lpszBegin,LPCTSTR lpszEnd,LPTSTR lpszRetBuf,INT nRetBufSize,LPTSTR * lpszNewOffset = NULL)
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


	ZeroMemory(lpszRetBuf,sizeof(lpszRetBuf));
	strncpy(lpszRetBuf,lpszFindBegin,lpszFindEnd - lpszFindBegin);

	if(lpszNewOffset) *lpszNewOffset = lpszFindEnd;

	return TRUE;
}



void  Iconv(char *valuestring,char * szDest,INT nSize)
{
	//UTF8 - ASCII
	wchar_t * lpszWideChar =  new wchar_t[  strlen(valuestring) + 1];
	MultiByteToWideChar(CP_UTF8,0,valuestring,-1,lpszWideChar,strlen(valuestring) + 1);
	
	WideCharToMultiByte(CP_ACP,0,lpszWideChar,-1,szDest,nSize,NULL,NULL);
	delete [] lpszWideChar;
}


BOOL GetTHSWebGSGY(LPCTSTR szCode,CString & szZYYW,CString & szSSHY,CString & szGN,CString & szSYLD,CString & szSYLJ,CString & szSJL,CString & szZXJJ)
{
	CHttp Http;
	Http.Init(5000,5000,5000);
	

	CString szHeaders;
	szHeaders.Format("Connection: keep-alive\r\n"\
					 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
					 "Referer: http://stockpage.10jqka.com.cn/%s/\r\n"\
					 "Accept-Encoding: gzip,deflate,sdch\r\n"\
					 "Accept-Language: zh-CN,zh;q=0.8\r\n",szCode);


	CString szURL;
	szURL.Format("http://basic.10jqka.com.cn/%s/",szCode,szHeaders);

	CString szContext;
	if(!Http.GetURLContext_String(szURL,szContext,10000))
		return FALSE;
	

	char szTemp[4096]={0};

	ZeroMemory(szTemp,sizeof(szTemp));
	GetValue(szContext,"主营业务：</span>","<a title=\"","\"",szTemp,sizeof(szTemp));
	szZYYW = szTemp;


	ZeroMemory(szTemp,sizeof(szTemp));
	GetValue(szContext,"所属行业：</span>",">","<",szTemp,sizeof(szTemp));
	szSSHY = szTemp;
	

	ZeroMemory(szTemp,sizeof(szTemp));
	GetValue(szContext,"市盈率(动态)：</span>",">","<",szTemp,sizeof(szTemp));
	szSYLD = szTemp;
	
	
	ZeroMemory(szTemp,sizeof(szTemp));
	GetValue(szContext,"市盈率(静态)：</span>",">","<",szTemp,sizeof(szTemp));
	szSYLJ = szTemp;


	ZeroMemory(szTemp,sizeof(szTemp));
	GetValue(szContext,"市净率：</span>",">","<",szTemp,sizeof(szTemp));
	szSJL = szTemp;
	
	ZeroMemory(szTemp,sizeof(szTemp));
	GetValue(szContext,"解禁</span>：</span>",">","<",szTemp,sizeof(szTemp));
	szZXJJ = szTemp;



	LPCTSTR lpszFind = strstr(szContext,"class=\"f14");
	if(lpszFind == NULL)
		return FALSE;
	
	lpszFind = strstr(lpszFind,">");
	if(lpszFind == NULL)
		return FALSE;
	
	lpszFind++;
	LPCTSTR lpszEnd = strstr(lpszFind,"</div>");
	if(lpszEnd == NULL)
		return FALSE;
	
	CString szText(lpszFind,lpszEnd - lpszFind);
	lpszFind = szText;

	szGN.Empty();

	while(TRUE)
	{
		lpszFind = strstr(lpszFind,"<a");
		if(lpszFind == NULL)
			break;
		
		lpszFind = strstr(lpszFind,">");
		if(lpszFind == NULL)
			break;
		
		lpszFind++;
		lpszEnd = strstr(lpszFind,"<");		
		CString szTemp(lpszFind,lpszEnd - lpszFind);
		if(szTemp.Find("详情")>=0)
			break;
		
		szGN+=szTemp;
		szGN+=" ";
		lpszFind=lpszEnd;
	}

	return szGN.IsEmpty()?FALSE : TRUE;
}


#define segment_size 1460//largest tcp data segment
char* ungzip(const char* source,int len)
{
	int err;
	z_stream d_stream;
	Byte compr[segment_size]={0}, uncompr[segment_size*4]={0};
	memcpy(compr,(Byte*)source,len);
	uLong comprLen, uncomprLen;
	comprLen = sizeof(compr) / sizeof(compr[0]);
	uncomprLen = 4*comprLen;
	strcpy((char*)uncompr, "garbage");
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
	d_stream.next_in = compr;
	d_stream.avail_in = 0;
	d_stream.next_out = uncompr;
	err = inflateInit2(&d_stream,47);
	if(err!=Z_OK)
	{
		printf("inflateInit2 error:%d",err);
		return NULL;
	}
	while (d_stream.total_out < uncomprLen && d_stream.total_in < comprLen) 
	{
		d_stream.avail_in = d_stream.avail_out = 1;
		err = inflate(&d_stream,Z_NO_FLUSH);
		if(err == Z_STREAM_END) break;
		if(err!=Z_OK)
		{
			printf("inflate error:%d",err);
			return NULL;
		}
	}

	err = inflateEnd(&d_stream);
	if(err!=Z_OK)
	{
		printf("inflateEnd error:%d",err);
		return NULL;
	}
	char* b = new char[d_stream.total_out+1];
	memset(b,0,d_stream.total_out+1);
	memcpy(b,(char*)uncompr,d_stream.total_out);
	return b;
}



BOOL THSWeb_GetZiJinLiuRuLiuChu(LPCTSTR szCode,LPTHSWEB_ZJLRLCINFO lpTHSWeb_ZJLRLCInfo)
{
	if(szCode == NULL || lpTHSWeb_ZJLRLCInfo == NULL) 
		return FALSE;

	CHttp Http;
	Http.Init(2000,2000,2000);

	CString szHeaders;
	szHeaders.Format("Connection: keep-alive\r\n"\
					 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
					 "Referer: http://stockpage.10jqka.com.cn/%s/\r\n"\
					 "Accept-Encoding: gzip,deflate,sdch\r\n"\
					 "Accept-Language: zh-CN,zh;q=0.8\r\n",szCode);

	char szContext[4096];
	DWORD dwRecvCount = 0;

	CString szURL;
	szURL.Format("http://stockpage.10jqka.com.cn/spService/%s/Funds/realFunds",szCode);
	if(!Http.GetURLContext_Bin(szURL,(LPBYTE)szContext,sizeof(szContext),dwRecvCount,10000,szHeaders))
		return FALSE;

	if(NULL == strstr(szContext,"flash"))
	{
		char * szNewConText = ungzip((LPCTSTR)szContext,dwRecvCount);
		if(szContext == NULL)
			return FALSE;

		ZeroMemory(szContext,sizeof(szContext));
		strcpy(szContext,szNewConText);
		free(szNewConText);
	}

	cJSON *root=cJSON_Parse(szContext);
	if (!root) 
		return FALSE;
	
	cJSON * flash = cJSON_GetObjectItem(root,"flash");
	if(flash == NULL)
		return FALSE;


	INT nArrayCount = cJSON_GetArraySize(flash);
	for(int i=0;i<nArrayCount;i++)
	{
		cJSON * Index = cJSON_GetArrayItem(flash,i);
		cJSON * format = cJSON_GetObjectItem(Index,"name"); 
		
		char szTemp[100]={0};
		Iconv(format->valuestring,szTemp,sizeof(szTemp));
		
		if(0 == strcmp(szTemp,"大单流出") )
		{
			cJSON * format = cJSON_GetObjectItem(Index,"sr"); 
			lpTHSWeb_ZJLRLCInfo->nLCDaDan = atoi(format->valuestring);
		}
		
		if(0 == strcmp(szTemp,"中单流出") )
		{
			cJSON * format = cJSON_GetObjectItem(Index,"sr"); 
			lpTHSWeb_ZJLRLCInfo->nLCZhongDan = atoi(format->valuestring);
		}
		
		if(0 == strcmp(szTemp,"小单流出") )
		{
			cJSON * format = cJSON_GetObjectItem(Index,"sr"); 
			lpTHSWeb_ZJLRLCInfo->nLCXiaoDan = atoi(format->valuestring);
		}


		if(0 == strcmp(szTemp,"大单流入") )
		{
			cJSON * format = cJSON_GetObjectItem(Index,"sr"); 
			lpTHSWeb_ZJLRLCInfo->nLRDaDan = atoi(format->valuestring);
		}
		
		if(0 == strcmp(szTemp,"中单流入") )
		{
			cJSON * format = cJSON_GetObjectItem(Index,"sr"); 
			lpTHSWeb_ZJLRLCInfo->nLRZhongDan = atoi(format->valuestring);
		}
		
		if(0 == strcmp(szTemp,"小单流入") )
		{
			cJSON * format = cJSON_GetObjectItem(Index,"sr"); 
			lpTHSWeb_ZJLRLCInfo->nLRXiaoDan = atoi(format->valuestring);
		}
	}
	cJSON_Delete(root);
	return TRUE;
}

BOOL GetTHSWebHQ_FormatJSON(LPCTSTR lpszContext,LPTHSWEB_HQDATA lpTHSWeb_HQData)
{

	cJSON *root=cJSON_Parse(lpszContext);
	if (!root) return FALSE;

	__try
	{
		cJSON * format = cJSON_GetObjectItem(root,"fieldname"); 
		Iconv(format->valuestring,lpTHSWeb_HQData->szSuoShuBanKuai,sizeof(lpTHSWeb_HQData->szSuoShuBanKuai));
		
		format = cJSON_GetObjectItem(root,"field");
		lpTHSWeb_HQData->fBanKuaiZhangDieFu = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"xj");
		lpTHSWeb_HQData->fXianJia = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"jj");
		lpTHSWeb_HQData->fJunJia = (float)atof(format->valuestring);

		format = cJSON_GetObjectItem(root,"zdf");
		lpTHSWeb_HQData->fZhangDieFu = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"zde");
		lpTHSWeb_HQData->fZhangDieE = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"cjl");
		lpTHSWeb_HQData->dChengJiaoLiang = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"cje");
		lpTHSWeb_HQData->dChengJiaoJinE = (float)atof(format->valuestring);

		char szTemp[100]={0};
		Iconv(format->valuestring,szTemp,sizeof(szTemp));
		if(strstr(szTemp,"万元"))
			lpTHSWeb_HQData->dChengJiaoJinE*= 10000;
		if(strstr(szTemp,"亿元"))
			lpTHSWeb_HQData->dChengJiaoJinE*= 100000000;


		format = cJSON_GetObjectItem(root,"kp");
		lpTHSWeb_HQData->fKaiPan = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"zs");
		lpTHSWeb_HQData->fZuoShou = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"zg");
		lpTHSWeb_HQData->fZuiGao = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"zd");
		lpTHSWeb_HQData->fZuiDi = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"wp");
		lpTHSWeb_HQData->nWaiPan = format->valueint;
		format = cJSON_GetObjectItem(root,"np");
		lpTHSWeb_HQData->nNeiPan = format->valueint;
 
		format = cJSON_GetObjectItem(root,"wb");
		lpTHSWeb_HQData->fWeibi = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"wc");
		lpTHSWeb_HQData->nWeiCha = format->valueint;

		

		format = cJSON_GetObjectItem(root,"hs");
		lpTHSWeb_HQData->fHuanShou = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"syl");
		lpTHSWeb_HQData->fShiYinLv = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"zt");
		lpTHSWeb_HQData->fZhangTingJia = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"dt");
		lpTHSWeb_HQData->fDieTingJia = (float)atof(format->valuestring);
		
		format = cJSON_GetObjectItem(root,"zf");
		lpTHSWeb_HQData->fZhengFu = (float)atof(format->valuestring);
	

		format = cJSON_GetObjectItem(root,"buy1");
		lpTHSWeb_HQData->fBP1 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"buy2");
		lpTHSWeb_HQData->fBP2 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"buy3");
		lpTHSWeb_HQData->fBP3 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"buy4");
		lpTHSWeb_HQData->fBP4 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"buy5");
		lpTHSWeb_HQData->fBP5 = (float)atof(format->valuestring);


		format = cJSON_GetObjectItem(root,"buy1data");
		lpTHSWeb_HQData->dwBN1 = format->valueint;
		format = cJSON_GetObjectItem(root,"buy2data");
		lpTHSWeb_HQData->dwBN2 = format->valueint;
		format = cJSON_GetObjectItem(root,"buy3data");
		lpTHSWeb_HQData->dwBN3 = format->valueint;
		format = cJSON_GetObjectItem(root,"buy4data");
		lpTHSWeb_HQData->dwBN4 = format->valueint;
		format = cJSON_GetObjectItem(root,"buy5data");
		lpTHSWeb_HQData->dwBN5 = format->valueint;


		format = cJSON_GetObjectItem(root,"sell1");
		lpTHSWeb_HQData->fSP1 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"sell2");
		lpTHSWeb_HQData->fSP2 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"sell3");
		lpTHSWeb_HQData->fSP3 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"sell4");
		lpTHSWeb_HQData->fSP4 = (float)atof(format->valuestring);
		format = cJSON_GetObjectItem(root,"sell5");
		lpTHSWeb_HQData->fSP5 = (float)atof(format->valuestring);
		

		format = cJSON_GetObjectItem(root,"sell1data");
		lpTHSWeb_HQData->dwSN1 = format->valueint;
		format = cJSON_GetObjectItem(root,"sell2data");
		lpTHSWeb_HQData->dwSN2 = format->valueint;
		format = cJSON_GetObjectItem(root,"sell3data");
		lpTHSWeb_HQData->dwSN3 = format->valueint;
		format = cJSON_GetObjectItem(root,"sell4data");
		lpTHSWeb_HQData->dwSN4 = format->valueint;
		format = cJSON_GetObjectItem(root,"sell5data");
		lpTHSWeb_HQData->dwSN5 = format->valueint;
	}__except(1)
	{

	}

	cJSON_Delete(root);
	return TRUE;
}


BOOL THSWeb_GetWebHQData(LPCTSTR szCode,LPTHSWEB_HQDATA lpTHSWeb_HQData)
{
	if(szCode == NULL || lpTHSWeb_HQData == NULL) 
		return FALSE;

	CHttp Http;
	Http.Init(2000,2000,2000);
	
	CString szHeaders;
	szHeaders.Format("Connection: keep-alive\r\n"\
					 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
					 "Referer: http://stockpage.10jqka.com.cn/%s/\r\n"\
					 "Accept-Encoding: gzip,deflate,sdch\r\n"\
					 "Accept-Language: zh-CN,zh;q=0.8\r\n",szCode);


	
	char szContext[4096];
	DWORD dwRecvCount = 0;
	CString szURL;
	szURL.Format("http://stockpage.10jqka.com.cn/spService/%s/Header/realHeader",szCode,szHeaders);
	if(!Http.GetURLContext_Bin(szURL,(LPBYTE)szContext,sizeof(szContext),dwRecvCount,3000,szHeaders))
		return FALSE;

	if(NULL == strstr(szContext,"stockcode"))
	{
		char * szNewConText = ungzip((LPCTSTR)szContext,dwRecvCount);
		if(szContext == NULL)
			return FALSE;
		
		ZeroMemory(szContext,sizeof(szContext));
		strcpy(szContext,szNewConText);
		free(szNewConText);
	}

	return GetTHSWebHQ_FormatJSON(szContext,lpTHSWeb_HQData);
}
