#ifndef Http_h__
#define Http_h__

#include <wininet.h>
#pragma comment(lib,"wininet.lib")

class  CHttp
{
public:
	CHttp(LPCTSTR lpszUserAgent= TEXT("Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.125 Safari/537.36"));
	~CHttp();
	BOOL GetURLContext_String(LPCTSTR lpszURL,CString & strData,DWORD dwTimeOut=10000,LPCSTR lpszHeaders=_T("Accept: */*"));
	BOOL GetURLContext_Bin(LPCTSTR lpszURL,LPBYTE lpRecvBuffer,DWORD dwRecvBufferSize,DWORD & dwRecvCount,DWORD dwTimeOut=10000,LPCSTR lpszHeaders=_T("Accept: */*"));
	BOOL Init(DWORD dwConnectTimeOut=3000, DWORD dwRecvTimeOut=3000 , DWORD dwSendTimeOut=3000);
private:
	HINTERNET m_hInternetOpen;
	CString   m_szUserAgent;
};




#endif // Http_h__