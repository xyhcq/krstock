#include "StdAfx.h"
#include "Http.h"



CHttp::CHttp(LPCTSTR lpszUserAgent  )
{
	m_szUserAgent = 	lpszUserAgent;
	m_hInternetOpen = NULL;
}



CHttp::~CHttp()
{
	if(m_hInternetOpen != NULL)
		InternetCloseHandle(m_hInternetOpen);
}



BOOL CHttp::Init(DWORD dwConnectTimeOut, DWORD dwRecvTimeOut , DWORD dwSendTimeOut)
{
	m_hInternetOpen  = InternetOpen(m_szUserAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if(m_hInternetOpen == NULL)
		return FALSE;

	//�������ӳ�ʱ
	DWORD dwTimeOut = dwConnectTimeOut;
	InternetSetOption(m_hInternetOpen,INTERNET_OPTION_CONNECT_TIMEOUT , (LPVOID)&dwTimeOut,sizeof(DWORD));
	
	//���ý��ճ�ʱ
	dwTimeOut = dwRecvTimeOut;
	InternetSetOption(m_hInternetOpen,INTERNET_OPTION_RECEIVE_TIMEOUT , (LPVOID)&dwTimeOut,sizeof(DWORD));
	
	//���÷��ͳ�ʱ
	dwTimeOut = dwSendTimeOut;
	InternetSetOption(m_hInternetOpen,INTERNET_OPTION_SEND_TIMEOUT , (LPVOID)&dwTimeOut,sizeof(DWORD));

	return TRUE;
}




BOOL CHttp::GetURLContext_String(LPCTSTR lpszURL,CString & strData,DWORD dwTimeOut,LPCSTR lpszHeaders)
{
	HINTERNET hInternetUrl=NULL;
	BOOL  bResult    = FALSE;
	strData.Empty();

	try{
		hInternetUrl   = InternetOpenUrl(m_hInternetOpen,lpszURL,lpszHeaders,-1,INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE,NULL);
		if(hInternetUrl == NULL)
			throw "hInternetUrl is null!";

		BYTE  Buffer[200*1024];
		DWORD dwReadByteSize = 0;
		BOOL  bReadResult = FALSE;
		DWORD dwTickCount = GetTickCount();

		while(TRUE)
		{
			if(GetTickCount() - dwTickCount > dwTimeOut)
				break;

			dwReadByteSize=0;

			ZeroMemory(Buffer,sizeof(Buffer));
			bReadResult = InternetReadFile(hInternetUrl,Buffer,sizeof(Buffer ) - 1,&dwReadByteSize);

			//��������Ƿ������
			if(bReadResult == TRUE && dwReadByteSize == 0)
			{
				bResult= TRUE;
				break;	
			}

			//��������
			Buffer[dwReadByteSize] = 0x00;
			strData += (char *)Buffer;
		}

	}catch(...)
	{
		

	}

	if(hInternetUrl != NULL)
		InternetCloseHandle(hInternetUrl);

	return bResult;
}



BOOL CHttp::GetURLContext_Bin(LPCTSTR lpszURL,LPBYTE lpRecvBuffer,DWORD dwRecvBufferSize,DWORD & dwRecvCount,DWORD dwTimeOut,LPCSTR lpszHeaders)
{

	HINTERNET hInternetUrl=NULL;
	BOOL  bResult    = FALSE;


	try{
		hInternetUrl   = InternetOpenUrl(m_hInternetOpen,lpszURL,lpszHeaders,-1,INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE,NULL);
		if(hInternetUrl == NULL)
			throw "hInternetUrl is null!";

		BYTE  Buffer[200*1024];

		DWORD dwReadByteSize = 0;
		BOOL  bReadResult = FALSE;
		DWORD dwTickCount = GetTickCount();

		while(TRUE)
		{
			if(GetTickCount() - dwTickCount > dwTimeOut)
				break;

			dwReadByteSize=0;
			bReadResult = InternetReadFile(hInternetUrl,Buffer,sizeof(Buffer),&dwReadByteSize);

			
			//��������Ƿ������
			if(bReadResult == TRUE && dwReadByteSize == 0)
			{
				bResult= TRUE;
				break;	
			}

			if(dwReadByteSize + dwRecvCount > dwRecvBufferSize)
				break;

			CopyMemory(&lpRecvBuffer[dwRecvCount],Buffer,dwReadByteSize);
			dwRecvCount += dwReadByteSize;
		}

	}catch(...)
	{
		

	}

	if(hInternetUrl != NULL)
		InternetCloseHandle(hInternetUrl);

	return bResult;
}
