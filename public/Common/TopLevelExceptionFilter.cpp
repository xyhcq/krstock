#include "StdAfx.h"
#include "TopLevelExceptionFilter.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")
#include <strsafe.h>
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp.lib")



CStackRecord g_StackRecord;

 
void _Record_StartC(const char* lpFuctionName , const char * szFileName , INT nLine)
{
	char szTemp[30]={0};

	string s;
	s.reserve(300);
	s = lpFuctionName;
	s+= " - ";
	s+= itoa(nLine,szTemp,10);
	s+= " - ";
	s+= szFileName;
	s+= "\r\n";
	g_StackRecord.Push(s.c_str());
}


void _Record_StartB(const char* lpFuctionName , const char * szFileName )
{
	string s;
	s.reserve(300);
	s = lpFuctionName;
	s+= " - ";
	s+= szFileName;
	s+= "\r\n";
	g_StackRecord.Push(s.c_str());
}


void _Record_StartA(const char* lpFuctionName)
{
	g_StackRecord.Push(lpFuctionName);
}



void _Record_End()
{
	g_StackRecord.Pop();
}




BOOL CTopLevelExceptionFilter::m_bShowMessageBox = TRUE;

void CTopLevelExceptionFilter::InstallExceptionFilter()
{
	SetUnhandledExceptionFilter(ExceptionFilter);
}

LONG WINAPI CTopLevelExceptionFilter::ExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo)
{	
	TCHAR szExceptInfoPath[MAX_PATH];
	GetModuleFileName(NULL,szExceptInfoPath,sizeof(szExceptInfoPath)/sizeof(TCHAR));


	
	SYSTEMTIME st;
	GetLocalTime(&st);

	//收集错误信息
	TCHAR szExceptInfo[1024 * 10 ]={0};
	StringCbPrintf(szExceptInfo,sizeof(szExceptInfo)/sizeof(TCHAR),
		_T("Date:%04u-%02u-%02u %02u:%02u:%02u\r\n")
		_T("Module:%s\r\nExceptionCode:%08X\r\nExceptionAddress:%08X\r\n")
        _T("EAX:%08X EBX:%08X ECX:%08X EDX:%08X\r\nESP:%08X EBP:%08X EDI:%08X ESI:%08X\r\n"),
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,
		szExceptInfoPath,
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		ExceptionInfo->ExceptionRecord->ExceptionAddress,
		ExceptionInfo->ContextRecord->Eax,
		ExceptionInfo->ContextRecord->Ebx,
		ExceptionInfo->ContextRecord->Ecx,
		ExceptionInfo->ContextRecord->Edx,
		ExceptionInfo->ContextRecord->Esp,
		ExceptionInfo->ContextRecord->Ebp,
		ExceptionInfo->ContextRecord->Edi,
		ExceptionInfo->ContextRecord->Esi);

	TCHAR szDumpFilePath[MAX_PATH]={0};
	PathRemoveFileSpec(szExceptInfoPath);
	StringCbCopy(szDumpFilePath,sizeof(szDumpFilePath)/sizeof(TCHAR),szExceptInfoPath);
	StringCbCat(szDumpFilePath,sizeof(szDumpFilePath)/sizeof(TCHAR),_T("\\Except.dmp"));
	StringCbCat(szExceptInfoPath,sizeof(szExceptInfoPath)/sizeof(TCHAR),_T("\\Except.txt"));


	TCHAR szMessageBox[2048]={0};
	StringCbPrintf(szMessageBox,sizeof(szMessageBox)/sizeof(TCHAR),
		_T("以下信息对作者非常重要！请一定要联系作者！\r\n%s\r\n详细信息已经保存到:\r\n%s\r\n%s\r\n\r\n")
		_T("联系作者时请告知是在什么情况下出现的异常\r\n并将以上两个文件发送给作者,万分感激!"),
		szExceptInfo,szExceptInfoPath,szDumpFilePath);


	//收集模块信息
	HMODULE hModule[1024];
	DWORD cbNeeded;
	if(EnumProcessModules(GetCurrentProcess(), hModule, sizeof( hModule ), &cbNeeded ) )
	{
		TCHAR szModName[MAX_PATH]={0};
		TCHAR szTemp[300]={0};
		for (int i = 0; i <= cbNeeded/sizeof(HMODULE); ++i )
		{
			if ( GetModuleFileNameEx(GetCurrentProcess(), hModule[i], szModName,sizeof(szModName)))
			{
				StringCbPrintf(szTemp,sizeof(szTemp)/sizeof(TCHAR),_T("%08X %s\r\n"),hModule[i],szModName);
				StringCbCat(szExceptInfo,sizeof(szExceptInfo)/sizeof(TCHAR),szTemp);
			}
		}
	}

	//收集调用堆栈
	StringCbCat(szExceptInfo,sizeof(szExceptInfo)/sizeof(TCHAR),_T("调用堆栈:\r\n"));
	for (size_t i = 0; i < g_StackRecord.size(); i++)
	{
		StringCbCat(szExceptInfo,sizeof(szExceptInfo)/sizeof(TCHAR),g_StackRecord.Get(i));
		StringCbCat(szExceptInfo,sizeof(szExceptInfo)/sizeof(TCHAR),_T(" "));
	}




	HANDLE hDumpFile = CreateFile(szDumpFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = ExceptionInfo;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);


	//记录到文件
	HANDLE hFile = CreateFile(szExceptInfoPath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile,0,0,FILE_END);
		DWORD dwWriteByte;
		WriteFile(hFile,(LPCTSTR)szExceptInfo,_tcslen(szExceptInfo) * sizeof(TCHAR),&dwWriteByte,NULL);
		CloseHandle(hFile);
	}

	if(m_bShowMessageBox == TRUE){
		MessageBox(NULL,szMessageBox,"非常抱歉！程序出现异常！",MB_OK);
	}


	//干掉进程，就不显示恶心的错误框了。
	TerminateProcess(GetCurrentProcess(),0);
	return EXCEPTION_CONTINUE_EXECUTION;
}