#ifndef HPClient_h__
#define HPClient_h__

#include <TCHAR.H>
#include "HPSocket/HPSocket4C.h"

#ifdef _DEBUG
	#ifdef _UNICODE
	#pragma comment(lib,"HPSocket4C_UD.lib")
	#else
	#pragma comment(lib,"HPSocket4C_D.lib")
#endif

#else
	#ifdef _UNICODE
	#pragma comment(lib,"HPSocket4C_U.lib")
	#else
	#pragma comment(lib,"HPSocket4C.lib")
#endif
#endif



enum
{ 
	WM_HPSERVER_LISTEN = WM_USER + 0x222, 
	WM_HPCLIENT_CONNECT,
	WM_HPCLIENT_RECEIVE,
	WM_HPCLIENT_CLOSE
};

typedef struct _HPCLIENT_RECVDATA
{
	BYTE * pData;
	INT    iLenght;
}HPCLIENT_RECVDATA,*LPHPCLIENT_RECVDATA;

typedef struct _HPCLIENT_LISTENDATA
{
	TCHAR	szAddress[40];
	USHORT	usPort;
}HPCLIENT_LISTENDATA,*LPHPCLIENT_LISTENDATA;


void HPClient_SetHWND(HWND hWnd);

//先Create 才能 Start
BOOL HPClient_Create();
BOOL HPClient_Start(LPCTSTR lpszAddress,SHORT usPort,BOOL bAsyncConnect);


//必须 HPClient_Stop 才能HPClient_Destory
BOOL HPClient_Stop();
BOOL HPClient_Destory();


BOOL    HPClient_SendData(BYTE* pBuffer, int iLength);
LPCTSTR HPClient_GetErrorMsg();
DWORD   HPClient_GetLastError();


#endif // HPClient_h__