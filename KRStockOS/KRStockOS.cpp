// KRStockOS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KRStockOS.h"
#include "KRStockOSDlg.h"
#include "Common/TopLevelExceptionFilter.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSApp

BEGIN_MESSAGE_MAP(CKRStockOSApp, CWinApp)
	//{{AFX_MSG_MAP(CKRStockOSApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSApp construction


CKRStockOSApp::CKRStockOSApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	GdiplusStartup(&m_GdiplusToken,&m_Gdistart,NULL);

//	_CrtSetBreakAlloc(244347);
//	_CrtSetBreakAlloc(65809);
}




/////////////////////////////////////////////////////////////////////////////
// The one and only CKRStockOSApp object

CKRStockOSApp				theApp;
CKRStockDataEngine			KRSDE;
CZXGDataMgr					ZXGDataMgr;
CTopLevelExceptionFilter	ExceptionFilter;

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSApp initialization

BOOL CKRStockOSApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor(RGB(0,0,0),RGB(0,0,0));  

	ExceptionFilter.InstallExceptionFilter();

// 	__asm{
// 		xor eax,eax
// 		mov [eax],0
// 	}

	
	if(!KRSDE.Init()){
		CFormatMessage FormatMessage;
		CString szText;
		szText.Format("猎鲸狂人股票数据引擎初始化失败!\r\b错误代码:%d \r\n%s",FormatMessage.GetLastError(),FormatMessage.szErrorText);
		::MessageBox(NULL,szText,TEXT("错误！"),MB_OK|MB_ICONERROR);
		return FALSE;
	}

	KRSDE.Start();
	
	//加载自选股
	ZXGDataMgr.Load();




	CKRStockOSDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


int CKRStockOSApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	ZXGDataMgr.Save();
	GdiplusShutdown(m_GdiplusToken);
	KRSDE.Stop();
	return CWinApp::ExitInstance();
}
