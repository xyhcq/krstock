// HPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HPClient.h"


static En_HP_HandleResult __stdcall HPClient_OnConnect(HP_Client pSender, HP_CONNID dwConnID);
static En_HP_HandleResult __stdcall HPClient_OnReceive(HP_Client pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
static En_HP_HandleResult __stdcall HPClient_OnClose(HP_Client pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);


static HP_TcpPackClient			m_pClient;
static HP_TcpPackClientListener	m_pListener;
static HWND						g_hWnd = NULL;


VOID HPClient_SetHWND(HWND hWnd)
{
	g_hWnd = hWnd;	
}



BOOL HPClient_Create()
{
	// 创建监听器对象
	m_pListener	= ::Create_HP_TcpPackClientListener();

	// 创建 Socket 对象
	m_pClient	= ::Create_HP_TcpPackClient(m_pListener);

	// 设置 Socket 监听器回调函数
	::HP_Set_FN_Client_OnConnect(m_pListener, HPClient_OnConnect);
	::HP_Set_FN_Client_OnReceive(m_pListener, HPClient_OnReceive);
	::HP_Set_FN_Client_OnClose(m_pListener, HPClient_OnClose);

	//设置正常心跳包间隔
	::HP_TcpClient_SetKeepAliveTime(m_pClient,10 * 1000);
	
	//设置异常心跳包间隔
	::HP_TcpClient_SetKeepAliveInterval(m_pClient,10 * 1000);

	::HP_TcpPackClient_SetMaxPackSize(m_pClient,   1 * 1024 * 1024);
	::HP_TcpPackClient_SetPackHeaderFlag(m_pClient, 911);

	return TRUE;
}






BOOL HPClient_Destory()
{
	Destroy_HP_TcpPullClient(m_pClient);
	Destroy_HP_TcpPullClientListener(m_pListener);
	return TRUE;
}

BOOL HPClient_Stop()
{
	return HP_Client_Stop(m_pClient);
}

BOOL HPClient_Start(LPCTSTR lpszAddress,SHORT usPort,BOOL bAsyncConnect)
{
	return HP_Client_Start(m_pClient, lpszAddress, usPort, bAsyncConnect);
}


LPCTSTR HPClient_GetErrorMsg()
{
	return HP_Client_GetLastErrorDesc(m_pClient);
}


DWORD HPClient_GetLastError()
{
	return HP_Client_GetLastError(m_pClient);
}


BOOL HPClient_SendData(BYTE* pBuffer, int iLength)
{
	return HP_Client_Send(m_pClient,pBuffer,iLength);
}


En_HP_HandleResult __stdcall HPClient_OnConnect(HP_Client pSender, HP_CONNID dwConnID)
{
	HPCLIENT_LISTENDATA  HPLD;
	int iAddressLen = sizeof(HPLD.szAddress) / sizeof(TCHAR);
	if (HP_Client_GetLocalAddress(pSender, HPLD.szAddress, &iAddressLen, &HPLD.usPort))
	{
		if(g_hWnd) SendMessage(g_hWnd,WM_HPCLIENT_CONNECT,(WPARAM)&HPLD,(LPARAM)TRUE);
	}else
	{
		if(g_hWnd) SendMessage(g_hWnd,WM_HPCLIENT_CONNECT,(WPARAM)&HPLD,(LPARAM)FALSE);
	}
	return HR_OK;
}



En_HP_HandleResult __stdcall HPClient_OnReceive(HP_Client pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength)
{
	BYTE * pNewData = new BYTE[ iLength ];
	if(pNewData == NULL)
		return HR_OK;
	
	CopyMemory(pNewData,pData,iLength);

	HPCLIENT_RECVDATA HR;
	HR.iLenght =  iLength;
	HR.pData   =  pNewData;

	if(g_hWnd) SendMessage(g_hWnd,WM_HPCLIENT_RECEIVE,(WPARAM)dwConnID,(LPARAM)&HR);
	delete [] HR.pData;
	return HR_OK;
}



En_HP_HandleResult __stdcall HPClient_OnClose(HP_Client pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode)
{
	if(g_hWnd)
	{
		LPCTSTR lpszError = HPClient_GetErrorMsg();
		SendMessage(g_hWnd,WM_HPCLIENT_CLOSE,(WPARAM)enOperation,(LPARAM)lpszError);
	}
	return HR_OK;
}






