// F10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WebGet.h"
#include "GetMidString.h"


BOOL BaseInfo_GetMidString(
					  const char * pTextBufferPoint,
					  char* leftKeyWords,
					  char* RightKeyWords,
					  std::string& OutMidString)
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
	return TRUE;
}

// {sad[fsd[[ 123 ]]adfsa]fsdaf} 如果有这样一个字符串需要取中间
//leftKeyWords1 == {
//RightKeyWords1 == }
//leftKeyWords2 == [[
//RightKeyWords2 == ]]
BOOL BaseInfo_GetMidStringEx(
						const char *& pTextBufferPoint,
						char* leftKeyWords1,  
						char* RightKeyWords1,
						char* leftKeyWords2,
						char* RightKeyWords2,
						std::string& OutMidString
						)
{
	std::string buffer1;
	if (!BaseInfo_GetMidString(pTextBufferPoint,leftKeyWords1,RightKeyWords1,buffer1))
		return FALSE;
	
	const char* pp1 = buffer1.c_str();
	if (!BaseInfo_GetMidString(pp1,leftKeyWords2,RightKeyWords2,OutMidString))
		return FALSE;

	return TRUE;
}


void BaseInfo_string_replace(std::string&s1,const std::string s2,const std::string s3)
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


//主营业务: 582 所属行业：12 涉及概念：171
BOOL WebGet_BaseInfo(LPCTSTR szCode,HQBASE_INFO * pOutData)
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
	
	const char* lpText = szContext;
	
	
	//公司概要-------------------
	
	//-主营业务
	
	std::string strZYYW;
	if (!BaseInfo_GetMidStringEx(lpText,"主营业务：","</td>","title=\"","\"",strZYYW))
		return FALSE;
	strcpy(pOutData->szZYYW,strZYYW.c_str());

	
	//-所属行业
	std::string strSSHY;
	if (!BaseInfo_GetMidStringEx(lpText,"所属行业：","</td>","\">","<",strSSHY))
		return FALSE;
	strcpy(pOutData->szSSHY,strSSHY.c_str());
	

	//-涉及概念 || --概念强弱排名
	if (StrStr(lpText,"涉及概念"))
	{
		std::string buff1;
		if (!BaseInfo_GetMidString(lpText,"涉及概念","</td>",buff1))
			return FALSE;
		
		std::string strSJGN,s1;
		const char* p1 = buff1.c_str();
		while (BaseInfo_GetMidString(p1,"ifind\">","<",s1))
		{
			strSJGN += s1;
			strSJGN += ",";
			p1 = StrStr(p1,"ifind\">");
			p1 += strlen("ifind\">");
			p1 += strlen(s1.c_str());
			p1 += strlen("<");
		}
		BaseInfo_string_replace(strSJGN,",详情>>,","");
		BaseInfo_string_replace(strSJGN,"详情>>,","");
		strcpy(pOutData->szSJGN,strSJGN.c_str());
		
	}else if (StrStr(lpText,"概念强弱排名"))
	{
		std::string buff1;
		if (!BaseInfo_GetMidString(lpText,"概念强弱排名：","</td>",buff1))
			return FALSE;
		
		
		std::string strSJGN,s1;
		const char* p1 = buff1.c_str();
		while (BaseInfo_GetMidString(p1,"ifind\">","<",s1))
		{

			strSJGN += s1;
			strSJGN += ",";
			p1 = StrStr(p1,"ifind\">");
			p1 += strlen("ifind\">");
			p1 += strlen(s1.c_str());
			p1 += strlen("<");
		}
		BaseInfo_string_replace(strSJGN,",详情>>,","");
		BaseInfo_string_replace(strSJGN,"详情>>,","");
		strcpy(pOutData->szSJGN,strSJGN.c_str());
	}
	
	//-市盈率(动态)
	std::string strSYLDT;
	if (!BaseInfo_GetMidStringEx(lpText,"市盈率(动态)","</td>","dtsyl\">","<",strSYLDT))
		return FALSE;
	pOutData->fSYLDT_BL = atof(strSYLDT.c_str());
	
	//-市盈率(静态)
	std::string strSYLJT;
	if (!BaseInfo_GetMidStringEx(lpText,"市盈率(静态)","</td>","jtsyl\">","<",strSYLJT))
		return FALSE;
	pOutData->fSYLJT_BL = atof(strSYLJT.c_str());
	
	//-市净率
	std::string strSJL;
	if (!BaseInfo_GetMidStringEx(lpText,"市净率：","</td>","sjl\">","<",strSJL))
		return FALSE;
	pOutData->fSJL = atof(strSJL.c_str());
	
	//-最新解禁
	
	if (strstr(lpText,"client_f10_baike_1'>解禁"))
	{
		std::string strzxjj;
		if (!BaseInfo_GetMidStringEx(lpText,"client_f10_baike_1'>解禁","</table>","f12\">","<",strzxjj))
			return FALSE;
		strcpy(pOutData->szZXJJ,strzxjj.c_str());
	}
	
	//主力控盘 ---------------------------------
	
	
	if (StrStr(lpText,"<h2>主力控盘</h2>"))
	{
		
		//股东总数
		std::string strGDZS;
		if (!BaseInfo_GetMidStringEx(lpText,"股东总数","</tr>","\">","<",strGDZS))
			return FALSE;
		pOutData->dwGDZS = (DWORD)atoi(strGDZS.c_str());
		
		//-较上期变化
		
		std::string strJSQBH;
		std::string strTmp;
		if (BaseInfo_GetMidString(lpText,"较上期变化","</tr>",strTmp))
		{
			const char* pp = strTmp.c_str();
			//	MessageBox(NULL,pp,NULL,MB_OK);
			if (StrStr(strTmp.c_str(),"downcolor\">"))
			{
				if (!BaseInfo_GetMidString(pp,"downcolor\">","<",strJSQBH))
					return FALSE;
			}else if (StrStr(strTmp.c_str(),"color:#FB0000\">"))
			{
				if (!BaseInfo_GetMidString(pp,"color:#FB0000\">","<",strJSQBH))
					return FALSE;
			}else if (StrStr(strTmp.c_str(),"class=\"tr\">"))
			{
			} 
			else
			{
				return FALSE;
			}
			
			
			// 	if (!F10_GetMidStringEx(lpText,"较上期变化","</tr>","downcolor\">","<",strJSQBH))
			// 		return FALSE;
			pOutData->fJSQBH = atof(strJSQBH.c_str());
			
			
			//-机构成本估算
			std::string strJGCBGS;
			if (BaseInfo_GetMidStringEx(lpText,"前十大流通股东","</p>","<strong>","<",strJGCBGS))
			{
				pOutData->fQSDLTGDCC = atof(strJGCBGS.c_str());
				if(strstr(strJGCBGS.c_str(),"万"))
					pOutData->fQSDLTGDCC*= 10000;
				if(strstr(strJGCBGS.c_str(),"亿"))
				pOutData->fQSDLTGDCC*= 100000000;
			}	
		}
		
	}
	return TRUE;
}




/*
int nPart1MiniSize = 0;
int nPart2MiniSize = 0;
int nPart3MiniSize = 0;

class CConsole
{
public:
	CConsole(const wchar_t* pText = L"Debug Output")
	{
		AllocConsole();						// 开辟控制台
		SetConsoleTitleW(pText);			// 设置控制台窗口标题
		freopen("CONOUT$","w",stdout);		// 重定向输出
	}
	~CConsole()
	{
		FreeConsole();
	}
private:
};

CConsole con;


void FinishedTest()
{
	printf("主营业务: %d 所属行业：%d 涉及概念：%d \r\n",nPart1MiniSize,nPart2MiniSize,nPart3MiniSize);
}

void CodeTest(const char* lpCode)
{
	THSDATA thsData;
	ZeroMemory(&thsData,sizeof(thsData));
	F10_GetCodeInfo(lpCode,&thsData);
	printf("----------------------------\r\n");
	printf("主营业务: %s  \r\n",thsData.szZYYW);
	printf("所属行业: %s \r\n",thsData.szSSHY);
	printf("涉及概念: %s \r\n",thsData.szSJGN);
	printf("市盈率(动态): %f \r\n",thsData.fSYLDT);
	printf("市盈率(静态): %f \r\n",thsData.fSYLJT);
	printf("市净率: %f \r\n",thsData.fSJL);
	printf("最新解禁日期: %s \r\n",thsData.strZXJJ);
	printf("股东总数: %u \r\n",thsData.dwGDZS);
	printf("较上期变化: %f \r\n",thsData.fJSQBH);
	printf("机构成本估算: %f \r\n",thsData.fJGCBGS);

// 	std::string strZYYW_;//-主营业务
// 	std::string strSSHY_; //-所属行业
// 	std::string strSJGN_;     //-涉及概念 || --概念强弱排名
// 	float fSYLDT;    //-市盈率(动态)
// 	float fSYLJT;  //-市盈率(静态)
// 	float fSJL;    //-市净率
// 	std::string strZXJJ_;//-最新解禁日期
// 	DWORD dwGDZS;	//股东总数
// 	float fJSQBH;//-较上期变化
// 	float fJGCBGS;//-机构成本估算
	

// 	if (!F10Parese(lpCode,strZYYW_,strSSHY_,strSJGN_,fSYLDT,fSYLJT,fSJL,strZXJJ_,dwGDZS,fJSQBH,fJGCBGS))
// 	{
// 		printf("采集姑表失败 %s \r\n",lpCode);
// 	}	
// 	if (strZYYW_.size() > nPart1MiniSize)
// 	{
// 		nPart1MiniSize = strZYYW_.size();
// 	}
// 	if (strSSHY_.size() > nPart2MiniSize)
// 	{
// 		nPart2MiniSize = strSSHY_.size();
// 	}
// 	if ( strSJGN_.size() > nPart3MiniSize)
// 	{
// 		nPart3MiniSize = strSJGN_.size();
// 	}
// 	
// 	
// 
// 	printf("----------------------------\r\n");
// 	printf("主营业务: %s  \r\n",strZYYW_.c_str());
// 	printf("所属行业: %s \r\n",strSSHY_.c_str());
// 	printf("涉及概念: %s \r\n",strSJGN_.c_str());
// 	printf("市盈率(动态): %f \r\n",fSYLDT);
// 	printf("市盈率(静态): %f \r\n",fSYLJT);
// 	printf("市净率: %f \r\n",fSJL);
// 	printf("最新解禁日期: %s \r\n",strZXJJ_.c_str());
// 	printf("股东总数: %u \r\n",dwGDZS);
// 	printf("较上期变化: %f \r\n",fJSQBH);
// 	printf("机构成本估算: %f \r\n",fJGCBGS);
	
	return ;
}


#ifdef _CONSOLE




// 采集姑表失败 601128
// 采集姑表失败 601811
// 采集姑表失败 603160
// 采集姑表失败 603313
// 采集姑表失败 603421
// 采集姑表失败 603777
// 采集姑表失败 603816
// 采集姑表失败 603887
// 采集姑表失败 000553
// 采集姑表失败 002807
// 采集姑表失败 002813
// 采集姑表失败 002815
// 采集姑表失败 300547
// 采集姑表失败 300548
// 采集姑表失败 300549

int main(int argc, char* argv[])
{
	CodeTest("603160");
	// 	CodeTest("002506");
	// 	CodeTest("002568");
	// 	CodeTest("002702");
	// 	CodeTest("600871");
	// 	CodeTest("300431");
	// 	CodeTest("300458");
	// 	CodeTest("600868");
	// 	CodeTest("002043");
	// 	CodeTest("601558");
	//	http://basic.10jqka.com.cn/32/603160/
	return 0;
}
#endif
*/






time_t  ToDate( const char * pszData /*2016-11-11*/)
{
	char * pszFind = strchr(pszData,'-');
	if(pszFind == NULL)
		return -1;
	
	char szYear[5]={0};
	strncpy(szYear,pszData,pszFind - pszData); 
	
	pszData = pszFind+1;
	
	pszFind = strchr(pszData,'-');
	if(pszFind == NULL)
		return -1;
	
	char szMonth[3]={0};
	strncpy(szMonth,pszData,pszFind - pszData); 
	
	pszData = pszFind+1;
	
	return CTime(atoi(szYear),atoi(szMonth),atoi(pszData),0,0,0).GetTime();
}




BOOL WebGet_MHDATE_CWBG( const char * lpStockCode,vector<time_t>& VCwbbsj)
{
	CHttp Http;
	Http.Init(20000,20000,20000);
	
	CString szURL;
	szURL.Format("http://news.stockstar.com/info/dstock.aspx?code=%s&id=2835",lpStockCode);


	CString szContext;
	if(!Http.GetURLContext_String(szURL,szContext,20000))
		return FALSE;
	
	const char* pWebData = szContext;

	string outDate;
	while (_dzjy_GetMidString(pWebData,"li_time\">","<",outDate,&pWebData))
	{
		VCwbbsj.push_back(ToDate(outDate.c_str()));
	}
	return TRUE;
}



BOOL WebGet_MHDATE_YJBG (const char* lpStockCode,vector<time_t>& VDate)
{
	CHttp Http;
	Http.Init(20000,20000,20000);
	
	CString szURL;
	
	szURL.Format("http://news.stockstar.com/info/dstock.aspx?code=%s&id=3491",lpStockCode);
	
	CString szContext;
	if(!Http.GetURLContext_String(szURL,szContext,20000))
		return FALSE;
	
	const char* pWebData = szContext;
	
	string outDate;
	while (_dzjy_GetMidString(pWebData,"li_time\">","<",outDate,&pWebData))
	{
		OutputDebugString(outDate.c_str());
		OutputDebugString("\r\n");
		VDate.push_back(ToDate(outDate.c_str()));
	}
	return TRUE;
}




BOOL WebGet_MHDATE_GGCG ( const char * lpStockCode,vector<time_t>& VGntc)
{
	
	CHttp Http;
	Http.Init(20000,20000,20000);
	
	CString szURL;
	szURL.Format("http://data.10jqka.com.cn/financial/ggjy/op/code/code/%s",lpStockCode);
	
	
	CString szData;
	if(!Http.GetURLContext_String(szURL,szData,20000))
		return FALSE;
	
	const char* pData = szData;
	
	string outData;
	if (!_dzjy_GetMidString(pData,"tbody>","</tbody>",outData))
		return FALSE;
	
	
	pData = outData.c_str();
	
	string tempData;
	while (_dzjy_GetMidString(pData,"<tr","</tr>",tempData,&pData))
	{

		string tmp;
		const char* pTemp = tempData.c_str();
		_dzjy_GetValue(pTemp,"tc","tc\">","<",tmp);
		
		VGntc.push_back(ToDate(tmp.c_str()));
	}
	
	return TRUE;
}



BOOL WebGet_MHDATE_LHB (const char* lpStockCode,vector<time_t>& VDate)
{


	CHttp Http;
	Http.Init(20000,20000,20000);
	
	CString szURL;
	szURL.Format("http://data.10jqka.com.cn/market/lhbcjmx/code/%s/",lpStockCode);
	
	CString szContext;
	if(!Http.GetURLContext_String(szURL,szContext,20000))
		return FALSE;
	
	const char* pWebData = szContext;
	string strDateInfo;
	if (!_dzjy_GetMidString(pWebData,"jj_ggcjmx_select","</div>",strDateInfo))
		return FALSE;
	const char* pDataInfo = strDateInfo.c_str();
	string strDate;
	while (_dzjy_GetMidString(pDataInfo,"<option >","</option>",strDate,&pDataInfo))
	{
		VDate.push_back(ToDate(strDate.c_str()));
	}
	return TRUE;
}





// 
// void TestPrint(int i, time_t& Dzjy)
// {
// 	printf("%d - %d\r\n",i,Dzjy);
// }
// 
// 
// int main(int argc, char* argv[])
// {
// 
// 	//http://basic.10jqka.com.cn/32/002702/concept.html
// 	//600868
// 	//600068
// 	//600031
// 	//600168
// 
// 	vector<time_t> VDzjy;
// 	GetCWBBSJContext("002702",VDzjy);
// 	int j;
// 	for (j = 0; j < VDzjy.size(); j++)
// 	{
// 		TestPrint(j,VDzjy[j]);
// 	}
// 
// 	printf("-----------------------\r\n");
// 	vector<time_t> VCwbb;
// 	GetCWBBSJContext("002702",VCwbb);
// 	for (j = 0; j < VCwbb.size(); j++)
// 	{
// 		TestPrint(j,VCwbb[j]);
// 	}
// 
// 	printf("-----------------------\r\n");
// 	
// 	vector<time_t> VLhb;
// 	GetLongHuBangDate("002702",VLhb);
// 	for (j = 0; j < VLhb.size(); j++)
// 	{
// 		TestPrint(j,VLhb[j]);
// 	}
// 	return 0;
// }
