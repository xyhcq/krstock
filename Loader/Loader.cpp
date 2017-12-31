// Loader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <TCHAR.H>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

typedef BOOL(WINAPI * KRSDEStartT)();
typedef BOOL(WINAPI * KRSDEStopT)();

class  CKRSDEClient
{
public:
	CKRSDEClient()
	{
		m_hModule = NULL;
	}

	~CKRSDEClient()
	{
		if(m_hModule) 
			FreeLibraryAndExitThread(m_hModule,0);
	}

	BOOL Init()
	{
		GetModuleFileName(NULL,m_szModulePath,MAX_PATH);
		PathRemoveFileSpec(m_szModulePath);

		_tcscat(m_szModulePath,TEXT("\\KRSDEClient.dll"));
		m_hModule = LoadLibrary(m_szModulePath);
		if(m_hModule == NULL)
			return FALSE;

		KRSDEStartProc = (KRSDEStartT)GetProcAddress(m_hModule,"KRSDE_Start");
		KRSDEStopProc = (KRSDEStopT)GetProcAddress(m_hModule,"KRSDE_Stop");
		return TRUE;
	}


	BOOL Start()
	{ 
		return KRSDEStartProc();
	}

	
	BOOL Stop()
	{
		return KRSDEStopProc();  
	}

private:
	TCHAR m_szModulePath[MAX_PATH];
	HMODULE m_hModule;
	KRSDEStartT  KRSDEStartProc;
	KRSDEStopT   KRSDEStopProc;
};



int main(int argc, char* argv[])
{
	CTime t(2016,11,11,10,00,0);
	for (INT i=1; i< 20; i++)
	{
		CTime tt = t;
		tt+=CTimeSpan(0,0,30,0);

		printf("{%2d,%2d,%2d,%2d},",t.GetHour(),t.GetMinute(),tt.GetHour(),tt.GetMinute());

		if( i % 5 ==0)
			printf("\r\n");

		t+=CTimeSpan(0,0,30,0);
	}
	printf("\r\n");


// 	printf("请记得按任意键正常退出！");
// 
// 	CKRSDEClient	KRSDEClient;
// 	KRSDEClient.Init();
// 
// 	KRSDEClient.Start();
// 
// 	getchar();
// 
// 	KRSDEClient.Stop();
// 
// 	printf("退出成功！");
	return 0;
}

