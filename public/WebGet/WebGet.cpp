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

// {sad[fsd[[ 123 ]]adfsa]fsdaf} ���������һ���ַ�����Ҫȡ�м�
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


//��Ӫҵ��: 582 ������ҵ��12 �漰���171
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
	
	
	//��˾��Ҫ-------------------
	
	//-��Ӫҵ��
	
	std::string strZYYW;
	if (!BaseInfo_GetMidStringEx(lpText,"��Ӫҵ��","</td>","title=\"","\"",strZYYW))
		return FALSE;
	strcpy(pOutData->szZYYW,strZYYW.c_str());

	
	//-������ҵ
	std::string strSSHY;
	if (!BaseInfo_GetMidStringEx(lpText,"������ҵ��","</td>","\">","<",strSSHY))
		return FALSE;
	strcpy(pOutData->szSSHY,strSSHY.c_str());
	

	//-�漰���� || --����ǿ������
	if (StrStr(lpText,"�漰����"))
	{
		std::string buff1;
		if (!BaseInfo_GetMidString(lpText,"�漰����","</td>",buff1))
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
		BaseInfo_string_replace(strSJGN,",����>>,","");
		BaseInfo_string_replace(strSJGN,"����>>,","");
		strcpy(pOutData->szSJGN,strSJGN.c_str());
		
	}else if (StrStr(lpText,"����ǿ������"))
	{
		std::string buff1;
		if (!BaseInfo_GetMidString(lpText,"����ǿ��������","</td>",buff1))
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
		BaseInfo_string_replace(strSJGN,",����>>,","");
		BaseInfo_string_replace(strSJGN,"����>>,","");
		strcpy(pOutData->szSJGN,strSJGN.c_str());
	}
	
	//-��ӯ��(��̬)
	std::string strSYLDT;
	if (!BaseInfo_GetMidStringEx(lpText,"��ӯ��(��̬)","</td>","dtsyl\">","<",strSYLDT))
		return FALSE;
	pOutData->fSYLDT_BL = atof(strSYLDT.c_str());
	
	//-��ӯ��(��̬)
	std::string strSYLJT;
	if (!BaseInfo_GetMidStringEx(lpText,"��ӯ��(��̬)","</td>","jtsyl\">","<",strSYLJT))
		return FALSE;
	pOutData->fSYLJT_BL = atof(strSYLJT.c_str());
	
	//-�о���
	std::string strSJL;
	if (!BaseInfo_GetMidStringEx(lpText,"�о��ʣ�","</td>","sjl\">","<",strSJL))
		return FALSE;
	pOutData->fSJL = atof(strSJL.c_str());
	
	//-���½��
	
	if (strstr(lpText,"client_f10_baike_1'>���"))
	{
		std::string strzxjj;
		if (!BaseInfo_GetMidStringEx(lpText,"client_f10_baike_1'>���","</table>","f12\">","<",strzxjj))
			return FALSE;
		strcpy(pOutData->szZXJJ,strzxjj.c_str());
	}
	
	//�������� ---------------------------------
	
	
	if (StrStr(lpText,"<h2>��������</h2>"))
	{
		
		//�ɶ�����
		std::string strGDZS;
		if (!BaseInfo_GetMidStringEx(lpText,"�ɶ�����","</tr>","\">","<",strGDZS))
			return FALSE;
		pOutData->dwGDZS = (DWORD)atoi(strGDZS.c_str());
		
		//-�����ڱ仯
		
		std::string strJSQBH;
		std::string strTmp;
		if (BaseInfo_GetMidString(lpText,"�����ڱ仯","</tr>",strTmp))
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
			
			
			// 	if (!F10_GetMidStringEx(lpText,"�����ڱ仯","</tr>","downcolor\">","<",strJSQBH))
			// 		return FALSE;
			pOutData->fJSQBH = atof(strJSQBH.c_str());
			
			
			//-�����ɱ�����
			std::string strJGCBGS;
			if (BaseInfo_GetMidStringEx(lpText,"ǰʮ����ͨ�ɶ�","</p>","<strong>","<",strJGCBGS))
			{
				pOutData->fQSDLTGDCC = atof(strJGCBGS.c_str());
				if(strstr(strJGCBGS.c_str(),"��"))
					pOutData->fQSDLTGDCC*= 10000;
				if(strstr(strJGCBGS.c_str(),"��"))
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
		AllocConsole();						// ���ٿ���̨
		SetConsoleTitleW(pText);			// ���ÿ���̨���ڱ���
		freopen("CONOUT$","w",stdout);		// �ض������
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
	printf("��Ӫҵ��: %d ������ҵ��%d �漰���%d \r\n",nPart1MiniSize,nPart2MiniSize,nPart3MiniSize);
}

void CodeTest(const char* lpCode)
{
	THSDATA thsData;
	ZeroMemory(&thsData,sizeof(thsData));
	F10_GetCodeInfo(lpCode,&thsData);
	printf("----------------------------\r\n");
	printf("��Ӫҵ��: %s  \r\n",thsData.szZYYW);
	printf("������ҵ: %s \r\n",thsData.szSSHY);
	printf("�漰����: %s \r\n",thsData.szSJGN);
	printf("��ӯ��(��̬): %f \r\n",thsData.fSYLDT);
	printf("��ӯ��(��̬): %f \r\n",thsData.fSYLJT);
	printf("�о���: %f \r\n",thsData.fSJL);
	printf("���½������: %s \r\n",thsData.strZXJJ);
	printf("�ɶ�����: %u \r\n",thsData.dwGDZS);
	printf("�����ڱ仯: %f \r\n",thsData.fJSQBH);
	printf("�����ɱ�����: %f \r\n",thsData.fJGCBGS);

// 	std::string strZYYW_;//-��Ӫҵ��
// 	std::string strSSHY_; //-������ҵ
// 	std::string strSJGN_;     //-�漰���� || --����ǿ������
// 	float fSYLDT;    //-��ӯ��(��̬)
// 	float fSYLJT;  //-��ӯ��(��̬)
// 	float fSJL;    //-�о���
// 	std::string strZXJJ_;//-���½������
// 	DWORD dwGDZS;	//�ɶ�����
// 	float fJSQBH;//-�����ڱ仯
// 	float fJGCBGS;//-�����ɱ�����
	

// 	if (!F10Parese(lpCode,strZYYW_,strSSHY_,strSJGN_,fSYLDT,fSYLJT,fSJL,strZXJJ_,dwGDZS,fJSQBH,fJGCBGS))
// 	{
// 		printf("�ɼ��ñ�ʧ�� %s \r\n",lpCode);
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
// 	printf("��Ӫҵ��: %s  \r\n",strZYYW_.c_str());
// 	printf("������ҵ: %s \r\n",strSSHY_.c_str());
// 	printf("�漰����: %s \r\n",strSJGN_.c_str());
// 	printf("��ӯ��(��̬): %f \r\n",fSYLDT);
// 	printf("��ӯ��(��̬): %f \r\n",fSYLJT);
// 	printf("�о���: %f \r\n",fSJL);
// 	printf("���½������: %s \r\n",strZXJJ_.c_str());
// 	printf("�ɶ�����: %u \r\n",dwGDZS);
// 	printf("�����ڱ仯: %f \r\n",fJSQBH);
// 	printf("�����ɱ�����: %f \r\n",fJGCBGS);
	
	return ;
}


#ifdef _CONSOLE




// �ɼ��ñ�ʧ�� 601128
// �ɼ��ñ�ʧ�� 601811
// �ɼ��ñ�ʧ�� 603160
// �ɼ��ñ�ʧ�� 603313
// �ɼ��ñ�ʧ�� 603421
// �ɼ��ñ�ʧ�� 603777
// �ɼ��ñ�ʧ�� 603816
// �ɼ��ñ�ʧ�� 603887
// �ɼ��ñ�ʧ�� 000553
// �ɼ��ñ�ʧ�� 002807
// �ɼ��ñ�ʧ�� 002813
// �ɼ��ñ�ʧ�� 002815
// �ɼ��ñ�ʧ�� 300547
// �ɼ��ñ�ʧ�� 300548
// �ɼ��ñ�ʧ�� 300549

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
