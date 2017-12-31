// HQReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "krstockos.h"
#include "HQReportDlg.h"
#include "Widget/MemoryDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHQReportDlg dialog
#define		TITLE_HEIGHT		20	
#define		KRSTOCKHQ_TITLE		"猎鲸狂人- 行情报表"


CHQReportDlg::CHQReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHQReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHQReportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHQReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHQReportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MINIMIZE, m_BtnMinimize);
	DDX_Control(pDX, IDC_MAXIMIZE, m_BtnMaximize);
	DDX_Control(pDX, IDC_CLOSE,	   m_BtnClose);
	
}


BEGIN_MESSAGE_MAP(CHQReportDlg, CDialog)
	//{{AFX_MSG_MAP(CHQReportDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TEXTBUTTON,OnTextButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHQReportDlg message handlers
enum{ID_TB_ZXG,ID_TB_SHAG,ID_TB_SZAG, ID_TB_CYB,ID_TB_ZXB,ID_TB_SHBG,ID_TB_HSAG,ID_TB_CNJJ ,ID_TB_HSZS };



BOOL CHQReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BtnClose.SetStyle(KRSYSBTN_CLOSEBOX);
	m_BtnClose.SetHotColor(200,0,0);
	m_BtnClose.SetBkColor(255,0,0);
	
	m_BtnMinimize.SetStyle(KRSYSBTN_MINIMIZEBOX);
	m_BtnMinimize.SetHotColor(200,0,0);
	m_BtnMinimize.SetBkColor(255,0,0);
	
	m_BtnMaximize.SetStyle(KRSYSBTN_MAXIMIZEBOX);
	m_BtnMaximize.SetHotColor(200,0,0);
	m_BtnMaximize.SetBkColor(255,0,0);
	
	m_Font.CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("宋体"));
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	
	m_TextButton.SetMessage(WM_TEXTBUTTON);
	m_TextButton.SetIndex(0);
	m_TextButton.SetFiexdIndex();
	
	

	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHQReportDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{
		
		CDC * pDc = MemDC.GetMemoryDC();
		CBrush bgbs(  RGB(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE)  );
		pDc->FillRect(rc,&bgbs);
		pDc->SetBkMode(TRANSPARENT);

		m_RCTitle= rc;
		m_RCTitle.bottom=  TITLE_HEIGHT;

		CBrush TitleBrush(RGB(255,0,0));
		pDc->FillRect(m_RCTitle,&TitleBrush);
	

		HICON hIcon  = (HICON)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON, 16,16,0);
		::DrawIconEx(pDc->GetSafeHdc(), 3, 3, hIcon, 16, 16, 0, NULL, DI_NORMAL | DI_COMPAT);
		pDc->SelectObject(m_Font);
		pDc->SetTextColor(RGB( TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE) );
		pDc->TextOut(24, 4 , KRSTOCKHQ_TITLE);

		CBrush RectBrush(RGB(TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE));


		INT nYBase  = 4;
		INT nXBase  = pDc->GetTextExtent(KRSTOCKHQ_TITLE).cx + 60;
		m_TextButton.Add("自选股",ID_TB_ZXG,nXBase,nYBase,pDc);
		
		nXBase += pDc->GetTextExtent("自选股").cx + 10;
		m_TextButton.Add("上海A股",ID_TB_SHAG,nXBase,nYBase,pDc);
		
		nXBase += pDc->GetTextExtent("上海A股").cx + 10;
		m_TextButton.Add("深圳A股",ID_TB_SZAG,nXBase,nYBase,pDc);
		
		nXBase += pDc->GetTextExtent("深圳A股").cx + 10;
		m_TextButton.Add("中小板",ID_TB_ZXB,nXBase,nYBase,pDc);
		
		
		nXBase += pDc->GetTextExtent("中小板").cx + 10;
		m_TextButton.Add("创业板",ID_TB_CYB,nXBase,nYBase,pDc);
		
		nXBase += pDc->GetTextExtent("创业板").cx + 10;
		m_TextButton.Add("上海B股",ID_TB_SHBG,nXBase,nYBase,pDc);
		
		nXBase += pDc->GetTextExtent("上海B股").cx + 10;
		m_TextButton.Add("沪深A股",ID_TB_HSAG,nXBase,nYBase,pDc);
		
		nXBase += pDc->GetTextExtent("沪深A股").cx + 10;
		m_TextButton.Add("场内基金",ID_TB_CNJJ,nXBase,nYBase,pDc);

		nXBase += pDc->GetTextExtent("沪深指数").cx + 10;
		m_TextButton.Add("沪深指数",ID_TB_HSZS,nXBase,nYBase,pDc);

		m_TextButton.ReDraw(pDc,&RectBrush,RGB(TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE),RGB(78,1,57));
	}
	// Do not call CDialog::OnPaint() for painting messages
}








void CHQReportDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_BtnClose.GetSafeHwnd())
	{
		CRect RCClose;
		m_BtnClose.GetClientRect(RCClose);
		m_BtnClose.MoveWindow(cx-RCClose.Width(),2,RCClose.Width(),RCClose.Height());
		m_BtnMaximize.MoveWindow(cx-RCClose.Width()*2,2,RCClose.Width(),RCClose.Height());
		m_BtnMinimize.MoveWindow(cx-RCClose.Width()*3,2,RCClose.Width(),RCClose.Height());
	}


	for(INT i=0;i< sizeof(m_StockListWidget) / sizeof(CStockListWidget ); i++ )
	{
		if(m_StockListWidget[i].GetSafeHwnd())
			m_StockListWidget[i].MoveWindow(2,TITLE_HEIGHT,cx-4,cy-TITLE_HEIGHT-2);
	}
}






UINT CHQReportDlg::OnNcHitTest(CPoint point) 
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


int CHQReportDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_StockListWidget[ID_TB_ZXG].m_bAutoUpdate   = FALSE;
	m_StockListWidget[ID_TB_ZXG].m_bRelevanceZXG = TRUE;
	m_StockListWidget[ID_TB_ZXG].Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_ZXG);


	m_StockListWidget[ID_TB_SHAG].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_SHAG);
	m_StockListWidget[ID_TB_SZAG].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_SZAG);
	m_StockListWidget[ID_TB_CYB].Create(_T(""),WS_CHILD |SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_CYB);
	m_StockListWidget[ID_TB_ZXB].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_ZXB);

	m_StockListWidget[ID_TB_SHBG].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_SHBG);
	m_StockListWidget[ID_TB_HSAG].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_HSAG);
	m_StockListWidget[ID_TB_CNJJ].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_CNJJ);
	m_StockListWidget[ID_TB_HSZS].Create(_T(""),WS_CHILD|SS_NOTIFY|MF_OWNERDRAW,CRect(0,0,0,0),this,ID_TB_HSZS);



	
	return 0;
}



void CHQReportDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}


void CHQReportDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OutputDebugString("CHQReportDlg::OnLButtonDown");

	if(m_TextButton.OnLButtonDown(this,nFlags,point))
		return;

	if(nFlags == MK_LBUTTON && m_RCTitle.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}



void CHQReportDlg::SwitchList( INT nIndex )
{
	for(INT i=0;i< sizeof(m_StockListWidget) / sizeof(CStockListWidget ); i++ )
	{
		if(nIndex == i)
			m_StockListWidget[i].ShowWindow(SW_SHOW);
		else
			m_StockListWidget[i].ShowWindow(SW_HIDE);
	}	
}



LRESULT CHQReportDlg::OnTextButton( WPARAM wParam, LPARAM lParam )
{
	SwitchList(wParam);

	vector<REALTIMEQUOTES> VRTQ;
	KRSDE.GetAllRealTimeQuotes(VRTQ);
	if( VRTQ.size() == 0)
		return FALSE;

	switch(wParam)
	{
	case ID_TB_SHAG:
		{
			if(m_StockListWidget[ID_TB_SHAG].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_SHAG].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if(it->m_wMarket ==  0x4853 && it->m_szLabel[0] == '6')	
						m_StockListWidget[ID_TB_SHAG].AddLabel((const char *)&it->m_wMarket);
				}
				m_StockListWidget[ID_TB_SHAG].m_List.SetRedraw(TRUE);
			}
		}break;

	case ID_TB_SZAG:
		{
			if(m_StockListWidget[ID_TB_SZAG].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_SZAG].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if(it->m_wMarket !=  0x5A53)
						continue;

					if( (it->m_szLabel[0]=='3' && it->m_szLabel[1]=='0') || 
						(it->m_szLabel[0]=='0' && it->m_szLabel[1]=='0') || 
						(it->m_szLabel[1]=='0' && it->m_szLabel[2]=='2'))
					{
						m_StockListWidget[ID_TB_SZAG].AddLabel((const char *)&it->m_wMarket);
					}
				}

				m_StockListWidget[ID_TB_SZAG].m_List.SetRedraw(TRUE);
			}
		}break;
	case ID_TB_CYB:
		{
			if(m_StockListWidget[ID_TB_CYB].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_CYB].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if( it->m_wMarket ==  0x5A53 && (it->m_szLabel[0]=='3' && it->m_szLabel[1]=='0'))
					{
						m_StockListWidget[ID_TB_CYB].AddLabel((const char *)&it->m_wMarket);
					}
				}
				m_StockListWidget[ID_TB_CYB].m_List.SetRedraw(TRUE);
			}
		}break;
			
	case ID_TB_ZXB:
		{
			if(m_StockListWidget[ID_TB_ZXB].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_ZXB].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if( it->m_wMarket == 0x5A53 && (it->m_szLabel[1]=='0' && it->m_szLabel[2]=='2'))
						m_StockListWidget[ID_TB_ZXB].AddLabel((const char *)&it->m_wMarket);
				}

				m_StockListWidget[ID_TB_ZXB].m_List.SetRedraw(TRUE);
			}
		}break;

	case ID_TB_SHBG:
		{
			
			if(m_StockListWidget[ID_TB_SHBG].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_SHBG].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if(it->m_wMarket ==  0x4853 && it->m_szLabel[0] == '9')	
					  m_StockListWidget[ID_TB_SHBG].AddLabel((const char *)&it->m_wMarket);
				}
				m_StockListWidget[ID_TB_SHBG].m_List.SetRedraw(TRUE);
			}
		}break;


	case ID_TB_CNJJ:
		{
			if(m_StockListWidget[ID_TB_CNJJ].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_CNJJ].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if(it->m_szLabel[0] == '1' && it->m_szLabel[1] == '5')	
						m_StockListWidget[ID_TB_CNJJ].AddLabel((const char *)&it->m_wMarket);
				}
				m_StockListWidget[ID_TB_CNJJ].m_List.SetRedraw(TRUE);
			}
		}break;

	case ID_TB_HSAG:
		{
			
//			DWORD dwTickCount = GetTickCount();
			if(m_StockListWidget[ID_TB_HSAG].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_HSAG].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if(it->m_wMarket == 0x5A53)
					{
						if( (it->m_szLabel[0]=='3' && it->m_szLabel[1]=='0') || 
						(it->m_szLabel[0]=='0' && it->m_szLabel[1]=='0') || 
						(it->m_szLabel[1]=='0' && it->m_szLabel[2]=='2') )
						{
							m_StockListWidget[ID_TB_HSAG].AddLabel((const char *)&it->m_wMarket);
						}
					}else if(it->m_wMarket ==  0x4853 && it->m_szLabel[0] == '6')	
						m_StockListWidget[ID_TB_HSAG].AddLabel((const char *)&it->m_wMarket);
				}
				m_StockListWidget[ID_TB_HSAG].m_List.SetRedraw(TRUE);
// 				CString szText;
// 				szText.Format("%d",GetTickCount()-dwTickCount);
// 				MessageBox(szText);
			}
		}break;

	case ID_TB_HSZS:
		{
			
			if(m_StockListWidget[ID_TB_HSZS].GetCount() == 0)
			{
				m_StockListWidget[ID_TB_HSZS].m_List.SetRedraw(FALSE);
				for (vector<REALTIMEQUOTES>::const_iterator it = VRTQ.begin(); it != VRTQ.end(); ++it)
				{
					if(it->m_wMarket == 0x5A53 && it->m_szLabel[0] == '3' && it->m_szLabel[1] == '9')
						m_StockListWidget[ID_TB_HSZS].AddLabel((const char *)&it->m_wMarket);

					else if(it->m_wMarket ==  0x4853 && it->m_szLabel[0] == '0' && it->m_szLabel[1] == '0')	
						m_StockListWidget[ID_TB_HSZS].AddLabel((const char *)&it->m_wMarket);
				}
				m_StockListWidget[ID_TB_HSZS].m_List.SetRedraw(TRUE);
			}	

		}break;
	}

	return TRUE;
}


BOOL CHQReportDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CHQReportDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	if(m_RCTitle.PtInRect(point))
//		ShowWindow(IsZoomed()?SW_RESTORE:SW_MAXIMIZE);

	m_StockListWidget[ID_TB_HSAG].Find("SZ300431");
	CDialog::OnLButtonDblClk(nFlags, point);
}
