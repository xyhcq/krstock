// KRSDEClient.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "KRSDEClient.h"
#include "KRSDEClientDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CKRSDEClientApp

BEGIN_MESSAGE_MAP(CKRSDEClientApp, CWinApp)
	//{{AFX_MSG_MAP(CKRSDEClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKRSDEClientApp construction

CKRSDEClientApp::CKRSDEClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	GdiplusStartup(&m_GdiplusToken,&m_Gdistart,NULL);

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKRSDEClientApp object

CKRSDEClientApp theApp;

int CKRSDEClientApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWinApp::ExitInstance();
}


CWinThread  * g_lpGuiThread = NULL;


UINT AFX_CDECL WorkThread(LPVOID lpParameter)
{
	CKRSDEClientDlg KRSDEClientDlg;
	KRSDEClientDlg.DoModal();
	return 0;
}


BOOL WINAPI KRSDE_Start()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_lpGuiThread == NULL)
		g_lpGuiThread = AfxBeginThread((AFX_THREADPROC)WorkThread,NULL);
	return TRUE;
}


BOOL WINAPI KRSDE_Stop()
{
	if(g_lpGuiThread == NULL) 
		return FALSE;
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	while(TRUE)
	{
		CString szTitle;
		szTitle.LoadString(IDS_TITLE);
		HWND hWnd = FindWindow(NULL,szTitle);
		if(hWnd == NULL)
			break;
		if(hWnd) SendMessage(hWnd,WM_CLOSE,200,200);
		Sleep(200);
	}

	if(WAIT_TIMEOUT ==WaitForSingleObject(g_lpGuiThread->m_hThread,100))
	{
		TerminateThread(g_lpGuiThread->m_hThread,0);
		return TRUE;
	}

	GdiplusShutdown(theApp.m_GdiplusToken);
	return TRUE;
}
