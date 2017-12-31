#ifndef Common_h__
#define Common_h__



class CFormatMessage
{
public:
	CFormatMessage() : m_nLastError(::GetLastError()),szErrorText(NULL){ FormatMessage(); }
	CFormatMessage(INT nLaseError) : m_nLastError(nLaseError),szErrorText(NULL){ FormatMessage();}
	~CFormatMessage(){ if(szErrorText) LocalFree((LPVOID)szErrorText);}	
	DWORD GetLastError(){return m_nLastError;}
	LPCTSTR operator() (){return szErrorText;}
public:
	LPCTSTR szErrorText;
private:
	void FormatMessage()
	{
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			m_nLastError, //接收错误码
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //默认语言
			(LPTSTR)&szErrorText, //接收错误信息
			0,
			NULL);
	}
	INT m_nLastError;
};


BOOL SaveFile(LPCTSTR lpszFilePath,void* pBuffer,INT & nInOutBufferSize);
BOOL LoadFile(LPCTSTR lpszFilePath,void* pBuffer,INT & nInBufferSize);

INT   GetFileSize(LPCTSTR lpFileName);
void  MakeDataPath(LPCTSTR lpszFileName, LPTSTR lpszOutPath);

class CExtenLogDlg
{
public:
	CExtenLogDlg();
	~CExtenLogDlg();
	BOOL InsertLog(LPCSTR szFormat,...);
	void ShowWindow(INT nCmd);
	BOOL IsWindowVisible();
private:
	HWND m_hWnd;
};


class CTRString
{
public:
	CTRString(){
		m_szPWChar = NULL;
		m_szPAChar = NULL;
	}
	~CTRString(){
		if(m_szPWChar){
			delete [] m_szPWChar;
			m_szPWChar = NULL;
		}
		if(m_szPAChar){
			delete [] m_szPAChar;
			m_szPAChar= NULL;
		}
	}
	
	WCHAR *  ANSIToUnicode(const char * szText){
		INT nLen = MultiByteToWideChar(CP_ACP,0,szText,-1,NULL,NULL); 
		if(m_szPWChar) 
			delete [] m_szPWChar;
		m_szPWChar = new WCHAR[nLen];
		if(m_szPWChar == NULL)
			return NULL;
		if(0 == MultiByteToWideChar(CP_ACP,0,szText,-1,m_szPWChar,nLen))
			return NULL;
		return m_szPWChar;
	}
	
	BOOL ANSIToUnicode(const char * szText, WCHAR * pWChar , INT nWChar){
		if(0 == MultiByteToWideChar(CP_ACP,0,szText,-1,pWChar,nWChar))
			return FALSE;
		return TRUE;
	}
	
	char * UnicodeToANSI(const WCHAR * szText){
		INT nLen =	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,szText, -1, NULL,NULL, NULL,NULL);   
		if(m_szPAChar) 
			delete []m_szPAChar;
		m_szPAChar = new char[nLen];
		if(m_szPAChar == NULL)
			return NULL;
		if(0 == WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,szText, -1, m_szPAChar,nLen, NULL,NULL))
			return NULL;
		return m_szPAChar;
	}
	
	BOOL UnicodeToANSI(const WCHAR * szText , char * pAChar, INT nAChar){
		if(0 == WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,szText, -1, pAChar,nAChar, NULL,NULL) )
			return FALSE;
		return TRUE;
	}
	
private:
	char  * m_szPAChar;
	WCHAR * m_szPWChar;
};

#endif // Common_h__