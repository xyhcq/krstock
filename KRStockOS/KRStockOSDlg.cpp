// KRStockOSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KRStockOS.h"
#include "KRStockOSDlg.h"
#include "Widget/MemoryDC.h"
#include "KeyDemonDlg.h"
#include "FilterSettingDlg.h"
#include "BootDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSDlg dialog
     
#define		TITLE_HEIGHT		20	


HWND g_hKRStockOSDlg = NULL;

CKRStockOSDlg::CKRStockOSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKRStockOSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKRStockOSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pHQReportDlg = NULL;
}

void CKRStockOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKRStockOSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MINIMIZE, m_BtnMinimize);
	DDX_Control(pDX, IDC_MAXIMIZE, m_BtnMaximize);
	DDX_Control(pDX, IDC_CLOSE,	   m_BtnClose);
}


BEGIN_MESSAGE_MAP(CKRStockOSDlg, CDialog)
	//{{AFX_MSG_MAP(CKRStockOSDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KRSDE_INITSUCCESS,OnKRSDE_InitSuccess)
	ON_MESSAGE(WM_KRSDE_INITFAILURE,OnKRSDE_InitFalure)
	ON_MESSAGE(WM_KRSDE_DISCONNECT,OnKRSDE_Disconnect)
	ON_MESSAGE(WM_SETLABLE,OnSetLabel)
	ON_MESSAGE(WM_SHOWKEYDEMON,OnShowKeyDemonDlg)
	ON_MESSAGE(WM_TEXTBUTTON,OnTextButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSDlg message handlers

enum{TIMER_UPDATE_TIME};
enum{KRStockOSDlg_ID_IndexWidget = 1024};




HHOOK  hKeyHook = NULL;
LRESULT CALLBACK KeyboardProc(int code,       WPARAM wParam,  LPARAM lParam   )
{
	if(g_hKRStockOSDlg == GetForegroundWindow()) 
		::SendMessage(g_hKRStockOSDlg,WM_SHOWKEYDEMON,wParam,lParam);
// 	CString szText;
// 	szText.Format("%c %d",wParam,lParam);
// 	OutputDebugString(szText);
	return CallNextHookEx(hKeyHook,code,wParam,lParam);
}


BOOL CKRStockOSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//MoveWindow(0,0,1024,732,FALSE);
	//CenterWindow();


	m_TextButton.SetMessage(WM_TEXTBUTTON);

	m_Font.CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体"));
	SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,NULL,GetCurrentThreadId());


	m_BtnClose.SetStyle(KRSYSBTN_CLOSEBOX);
	m_BtnClose.SetHotColor(255,0,0);
	m_BtnClose.SetBkColor(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE);
	
	m_BtnMinimize.SetStyle(KRSYSBTN_MINIMIZEBOX);
	m_BtnMinimize.SetHotColor(200,0,0);
	m_BtnMinimize.SetBkColor(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE);
	
	m_BtnMaximize.SetStyle(KRSYSBTN_MAXIMIZEBOX);
	m_BtnMaximize.SetHotColor(200,0,0);
	m_BtnMaximize.SetBkColor(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE);
	
	SetTimer(TIMER_UPDATE_TIME,500,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}




// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
enum{ID_TB_ZXG,ID_TB_XTYJ,ID_TB_XTYJJG,ID_TB_HQYJ, ID_TB_HQYJJG,ID_TB_ZDDP,ID_TB_ZDJY, ID_TB_SJY , ID_TB_GY };

void CKRStockOSDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{
		CDC * pDc = MemDC.GetMemoryDC();
		m_RCTitle = rc;
		m_RCTitle.bottom = TITLE_HEIGHT;

		CBrush bgbs(  RGB(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE)  );
		pDc->FillRect(rc,&bgbs);

		pDc->SelectObject(m_Font);
		pDc->SetBkMode(TRANSPARENT);
		
		HICON hIcon  = (HICON)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON, 16,16,0);
		::DrawIconEx(pDc->GetSafeHdc(), 3, 3, hIcon, 16, 16, 0, NULL, DI_NORMAL | DI_COMPAT);
		
		pDc->SetTextColor(RGB( TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE) );

		CString szTitle;
		szTitle.LoadString(IDS_TITLE);
		pDc->TextOut(24, 4 , szTitle);
		INT nXBase = pDc->GetTextExtent(szTitle).cx + 24 + 10;


		CBrush RectBrush(RGB(TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE));
		m_RCKeyDemon.left   = nXBase;
		m_RCKeyDemon.right  = m_RCKeyDemon.left + 120;
		m_RCKeyDemon.top    = 3;
		m_RCKeyDemon.bottom = m_RCKeyDemon.top + 16;
		pDc->FrameRect(m_RCKeyDemon,&RectBrush);

		////////////////用GDI+画放大镜////////////////////////
		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );
		raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		Pen penEllipse(Color(255,255,255),2);
		raphics.DrawEllipse(&penEllipse,245,5,8,8);
		raphics.DrawLine(&penEllipse,247,11, 243,16 );
		//////////////////////////////////////////////////
		INT nYBase  = m_RCKeyDemon.top + 2;
		pDc->TextOut(m_RCKeyDemon.left + 20 , nYBase , "代码/名称/简称" );

		

		nXBase = m_RCKeyDemon.right + 10;
		m_TextButton.Add("自选股",ID_TB_ZXG,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("自选股").cx + 10;

		m_TextButton.Add("形态预警",ID_TB_XTYJ,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("形态预警").cx + 10;

		m_TextButton.Add("预警结果",ID_TB_XTYJJG,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("预警结果").cx + 10;

		m_TextButton.Add("行情预警",ID_TB_HQYJ,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("行情预警").cx + 10;
		
		m_TextButton.Add("预警结果",ID_TB_HQYJJG,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("预警结果").cx + 10;

		m_TextButton.Add("自动盯盘",ID_TB_ZDDP,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("自动盯盘").cx + 10;

		m_TextButton.Add("自动交易",ID_TB_ZDJY,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("自动交易").cx + 10;

		m_TextButton.Add("数据源",ID_TB_SJY,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("数据源").cx + 10;

		m_TextButton.Add("关于",ID_TB_GY,nXBase,nYBase,pDc);
		m_TextButton.ReDraw(pDc,&RectBrush,RGB(TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE),RGB(78,1,57));
	}
}


void CKRStockOSDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustWidget(0,0,cx,cy);
	RedrawWindow();
}



// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKRStockOSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKRStockOSDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = 1100;
	lpMMI->ptMinTrackSize.y = 732;
	lpMMI->ptMaxSize.x  = GetSystemMetrics(SM_CXFULLSCREEN);
	lpMMI->ptMaxSize.y  = GetSystemMetrics(SM_CYFULLSCREEN)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME);
	CDialog::OnGetMinMaxInfo(lpMMI);
}

BOOL CKRStockOSDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

UINT CKRStockOSDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rc;
    GetWindowRect(&rc);
	if (abs(point.y-rc.top+1) < 4 && abs(point.x-rc.left+1) < 4)
		return HTTOPLEFT;
	if (abs(point.y-rc.bottom+1) < 4 && abs(point.x-rc.left+1) < 4)
		return HTBOTTOMLEFT;
	if (abs(point.y-rc.top+1) < 4 && abs(point.x-rc.right+1) < 4)
		return HTTOPRIGHT;
	if (abs(point.y-rc.bottom+1) < 4 && abs(point.x-rc.right+1) < 4)
		return HTBOTTOMRIGHT;
	if (abs(point.y-rc.bottom+1) < 4)
		return HTBOTTOM;
	if (abs(point.y-rc.top+1) < 4)
		return HTTOP;
	if (abs(point.x-rc.right+1) < 4)
		return HTRIGHT;
	if (abs(point.x-rc.left+1) < 4)
		return HTLEFT;
	return CDialog::OnNcHitTest(point);
}

void CKRStockOSDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_TextButton.OnLButtonDown(this,nFlags,point))
		return;

	if(m_RCTitle.PtInRect(point))
		ShowWindow(IsZoomed()?SW_RESTORE:SW_MAXIMIZE);

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CKRStockOSDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonUp(nFlags, point);
}



void CKRStockOSDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_TextButton.OnLButtonDown(this,nFlags,point))
		return;
	
	if(m_RCKeyDemon.PtInRect(point))
	{
		CKeyDemonDlg KeyDemonDlg;
		if(::FindWindow(NULL,KeyDemonDlg.m_szTitle))
			return;
		
		CRect WindowRc = m_RCKeyDemon;
		ClientToScreen(WindowRc);
		
		KeyDemonDlg.m_nLeft =  WindowRc.left ;
		KeyDemonDlg.m_nTop  =  WindowRc.top  ;
		KeyDemonDlg.DoModal();
		return;
	}
	
	if(nFlags == MK_LBUTTON && m_RCTitle.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	
	CDialog::OnLButtonDown(nFlags, point);
}








void CKRStockOSDlg::AdjustWidget(int x, int y,int cx, int cy)
{
	if(m_BtnClose.GetSafeHwnd())
	{
		CRect RCClose;
		m_BtnClose.GetClientRect(RCClose);
		m_BtnClose.MoveWindow(cx-RCClose.Width(),2,RCClose.Width(),RCClose.Height());
		m_BtnMaximize.MoveWindow(cx-RCClose.Width()*2,2,RCClose.Width(),RCClose.Height());
		m_BtnMinimize.MoveWindow(cx-RCClose.Width()*3,2,RCClose.Width(),RCClose.Height());
	}

	if(m_IndexWidget.GetSafeHwnd())
	{
		CRect RCIndex;
		RCIndex.left   = 2;
		RCIndex.right  = cx - 2;
		RCIndex.top    = cy - m_IndexWidget.GetDefaultHeight();
		RCIndex.bottom = cy - 2;
		m_IndexWidget.MoveWindow(RCIndex);
	}

	m_KLineAnalyze.AdjuestWidget(x,TITLE_HEIGHT,cx,cy- m_IndexWidget.GetDefaultHeight());
}




int CKRStockOSDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	g_hKRStockOSDlg = m_hWnd;
	CString szTitle;
	szTitle.LoadString(IDS_TITLE);
	::SetWindowText(m_hWnd,szTitle);



	KRSDE.SetHWND(m_hWnd);
	m_KLineAnalyze.Create(this);
	m_IndexWidget.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,KLineAnalyze_ID_KLineWidget);
	return 0;
}








LRESULT CKRStockOSDlg::OnSetLabel( WPARAM wParam, LPARAM lParam )
{
	m_KLineAnalyze.SetLabel((LPCTSTR)lParam);
	return TRUE;
}

LRESULT CKRStockOSDlg::OnKRSDE_InitSuccess(WPARAM wParam, LPARAM lParam)
{
	if(m_pHQReportDlg == NULL )
	{
		m_pHQReportDlg = new CHQReportDlg;
		if(m_pHQReportDlg)
		{
			m_pHQReportDlg->Create(IDD_MYSTOCK_DIALOG,this);
			m_pHQReportDlg->ShowWindow(SW_SHOW);
		}
	}

	m_KLineAnalyze.SetLabel("SZ002702");
	KRSDE.ShowWindow(KRSDE_SW_AUTO);
	SetForegroundWindow();
	return TRUE;
}



LRESULT CKRStockOSDlg::OnKRSDE_InitFalure(WPARAM wParam, LPARAM lParam)
{
	KRSDE.ShowWindow(KRSDE_SW_SHOW);
	return TRUE;
}


LRESULT CKRStockOSDlg::OnKRSDE_Disconnect(WPARAM wParam, LPARAM lParam)
{
	KRSDE.ShowWindow(KRSDE_SW_SHOW);
	return TRUE;
}



void CKRStockOSDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMER_UPDATE_TIME:
		{
			CFont m_Font;
			m_Font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体"));

			CDC * pDC = GetDC();
			pDC->SetTextColor(RGB(255,255,255));
			pDC->SetBkMode(TRANSPARENT);
			pDC->SelectObject(m_Font);

			CString szText;
			CTime t = CTime::GetCurrentTime();
			char * pWeek[]={"","日","一","二","三","四","五","六"};

			szText.Format("%04u-%02u-%02u %s %02u:%02u:%02u",t.GetYear(),t.GetMonth(),t.GetDay(),pWeek[t.GetDayOfWeek()],t.GetHour(),t.GetMinute(),t.GetSecond());
			
			CRect rc;
			m_BtnMinimize.GetWindowRect(rc);
			ScreenToClient(rc);

			CRect TextRc = rc;
			TextRc.left = rc.left -  pDC->GetTextExtent(szText).cx - 20;
			TextRc.right -= 20;
			
			CBrush BGBrush(RGB(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE));
			pDC->FillRect(TextRc,&BGBrush);

			ScreenToClient(rc);
			pDC->TextOut(TextRc.left,6,szText);
			ReleaseDC(pDC);
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CKRStockOSDlg::OnShowKeyDemonDlg( WPARAM wParam, LPARAM lParam )
{
	char nChar = wParam; 
	if((nChar >= '0' && nChar <= '9')  || 
		(nChar >= 'A' && nChar <= 'Z') || 
		(nChar >= 'a' && nChar <= 'z') || 
		(nChar >= 96 && nChar <= 105) )
	{
		CKeyDemonDlg KeyDemonDlg;

// 		if(::FindWindow(NULL,KeyDemonDlg.m_szTitle))
// 			return FALSE;


		if(nChar >= 96) 
			nChar = nChar - 96 + '0';

		CRect WindowRc = m_RCKeyDemon;
		ClientToScreen(WindowRc);
		KeyDemonDlg.m_szInitText=nChar;
		KeyDemonDlg.m_nLeft =  WindowRc.left ;
		KeyDemonDlg.m_nTop  =  WindowRc.top  ;
		KeyDemonDlg.DoModal();
	} 	
	return TRUE;
}



void CKRStockOSDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_TextButton.OnMouseMove(this,nFlags,point);
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CKRStockOSDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_KLineAnalyze.PreTranslateMessage(this,pMsg))
		return TRUE;


	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}






LRESULT CKRStockOSDlg::OnTextButton( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case ID_TB_ZXG:
		if(m_pHQReportDlg) 
		{
			m_pHQReportDlg->ShowWindow(SW_RESTORE);
			::SetForegroundWindow(m_pHQReportDlg->m_hWnd);
			return FALSE;
		}
		break;

	case ID_TB_XTYJ:
		{
			CFilterSettingDlg FilterSettingDlg;
			FilterSettingDlg.DoModal();
		}
		break;
	case ID_TB_XTYJJG:
		{
			MessageBox("近期开放");
		}break;

	case ID_TB_HQYJ:
		{
			MessageBox("近期开放");
		}
		break;

	case ID_TB_HQYJJG:
		{
			MessageBox("近期开放");
		}
		break;


	case ID_TB_ZDDP:
		{
			MessageBox("近期开放");
		}
		break;

	case ID_TB_ZDJY:
		{
			MessageBox("近期开放");
		}
		break;

	case ID_TB_SJY:
		KRSDE.ShowWindow(KRSDE_SW_AUTO);
		return FALSE;

	case ID_TB_GY:
		CBootDlg BootDlg;
		BootDlg.DoModal();
		break;
	}
	return TRUE;
}



void CKRStockOSDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pHQReportDlg)
	{
		m_pHQReportDlg->DestroyWindow();
		delete m_pHQReportDlg;
	}
	CDialog::OnClose();
}

