#include "StdAfx.h"
#include "Common.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <float.h>
#include <math.h>

#define  WM_SHOWLOG_BEGIN			(WM_USER + 0x100)
#define  WM_SHOWLOG_END 			(WM_USER + 0x101)

/*
#define zend_isinf(a)   ((_fpclass(a) == _FPCLASS_PINF) || (_fpclass(a) == _FPCLASS_NINF))
#define zend_isnan(x)   _isnan(x)
double KRRound(double val, int places) 
{
    double t;
    double f = pow(10.0, (double) places);
    double x = val * f;
	
    if (zend_isinf(x) || zend_isnan(x)) 
	{
        return val;
    }
	
    if (x >= 0.0) 
	{
        t = ceil(x);
        if ((t - x) > 0.50000000000) 
		{
            t -= 1.0;
        }
    } else 
	{
        t = ceil(-x);
        if ((t + x) > 0.50000000000) 
		{
            t -= 1.0;
        }
        t = -t; 
    }

    x = t / f;
    return !zend_isnan(x) ? x : t;
}
*/

float KRRound(float val, int places) 
{
	float f = fabs(val);
	f = f*pow(10,places);
    f += 0.5;
    f = ceil(f);
	f = f/pow(10,places);

	if(val > 0 ) 
		return f;
    return -f;
}







void CExtenLogDlg::ShowWindow(INT nCmd)
{
	::ShowWindow(m_hWnd,nCmd); 
}

BOOL CExtenLogDlg::IsWindowVisible()
{
	return ::IsWindowVisible(m_hWnd);
}

CExtenLogDlg::CExtenLogDlg()
{
	m_hWnd =::FindWindow("#32770","My Log Record");
	if(m_hWnd){
		::SendMessage(m_hWnd,WM_SHOWLOG_BEGIN,0,0);
	}else
	{
		STARTUPINFO si;
		ZeroMemory(&si,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.wShowWindow = SW_SHOW;
		si.dwFlags = STARTF_USESHOWWINDOW;
		
		PROCESS_INFORMATION pi={0};
		
		CHAR szModulePath[MAX_PATH]={0};
		GetModuleFileName(NULL,szModulePath,MAX_PATH);
		PathRemoveFileSpec(szModulePath);
		strcat(szModulePath,"\\ShowLog.exe");
		
		
		char szCommand[MAX_PATH]={0};
		BOOL bResult = CreateProcess(szModulePath,szCommand,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&si,&pi);
		if(bResult)
		{
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			Sleep(1000);
		}
		m_hWnd =::FindWindow("#32770","KRSDE - Run Log");
		if(m_hWnd)  ::SendMessage(m_hWnd,WM_SHOWLOG_BEGIN,0,0);
	}
}





CExtenLogDlg::~CExtenLogDlg()
{
	if(m_hWnd){
		::SendMessage(m_hWnd,WM_SHOWLOG_END,0,0);
	}
}


BOOL CExtenLogDlg::InsertLog(LPCSTR szFormat,...)
{
	if(m_hWnd == NULL) 
		return FALSE;
	
	va_list vagrc;
	va_start(vagrc,szFormat);
	
	char szBuffer[1024]={0};
	vsprintf(szBuffer,szFormat,vagrc);
	va_end(vagrc);
	
	return ::SendMessage(m_hWnd,WM_SETTEXT,0,(LPARAM)szBuffer);
}



void  MakeDataPath(LPCTSTR lpszFileName, LPTSTR lpszOutPath)
{
	CHAR szModuleFileName[MAX_PATH]={0};
	GetModuleFileName(NULL,szModuleFileName,MAX_PATH);
	PathRemoveFileSpec(szModuleFileName);
	
	_tcscpy(lpszOutPath,szModuleFileName);
	_tcscat(lpszOutPath,_T("\\Dat\\"));

	CreateDirectory(lpszOutPath,NULL);
	_tcscat(lpszOutPath,lpszFileName);
}



INT GetFileSize(LPCTSTR lpFileName)
{
	HANDLE hFile = CreateFile(
		lpFileName,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return -1;

	INT nFileSize = ::GetFileSize(hFile,NULL);
	CloseHandle(hFile);
	return nFileSize;
}





BOOL SaveFile(LPCTSTR lpszFilePath,void* pBuffer,INT & nInOutBufferSize)
{
	HANDLE hFile = CreateFileA(
		lpszFilePath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	DWORD nBytesWrites = 0;
	BOOL bOk = WriteFile(hFile,pBuffer,nInOutBufferSize,&nBytesWrites,NULL);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	

	nInOutBufferSize = nBytesWrites;
	return bOk;
}



BOOL LoadFile(LPCTSTR lpszFilePath,void* pBuffer,INT & nInBufferSize)
{
	HANDLE hFile = CreateFile(
		lpszFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	
	DWORD dwFileSizeHigh;

	INT nFileSize = GetFileSize(hFile,&dwFileSizeHigh);

	if(nFileSize > nInBufferSize )
	{
		CloseHandle(hFile);
		return FALSE;
	}
	
	DWORD dwNumberOfBytesRead;
	BOOL bOk = ReadFile(hFile,pBuffer,nFileSize,&dwNumberOfBytesRead,NULL);
	CloseHandle(hFile);
	nInBufferSize = dwNumberOfBytesRead;
	return TRUE;
}




