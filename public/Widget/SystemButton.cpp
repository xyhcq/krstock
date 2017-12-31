// SystemButtom.cpp : implementation file
//

#include "stdafx.h"
#include "SystemButton.h"
#include "MemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemButtom

CSystemButton::CSystemButton()
{
	m_nStyle = 0;
	m_BKR = 168;
	m_BKG = 0;
	m_BKB = 157;
	
	m_HOTR = 255;
	m_HOTG = 255;
	m_HOTB = 255;
	
	m_bHot = FALSE;
	m_bOver = FALSE;
}



CSystemButton::~CSystemButton()
{
}


BEGIN_MESSAGE_MAP(CSystemButton, CStatic)
	//{{AFX_MSG_MAP(CSystemButtom)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemButtom message handlers
LRESULT CSystemButton::OnMouseLeave(WPARAM ,LPARAM )
{
	m_bHot  = FALSE;
	m_bOver = FALSE;
	RedrawWindow();//鼠标离开,强制重绘
	return TRUE;
}

void CSystemButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bHot == FALSE)
	{
		m_bHot = TRUE;
		RedrawWindow();
	}
	
	//默认情况下 子窗口是不接受鼠标离开消息的
	if ( m_bOver == FALSE )
	{
		m_bOver = TRUE;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof( TRACKMOUSEEVENT );
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 60;
		::TrackMouseEvent( &tme );
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CSystemButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nStyle == KRSYSBTN_CLOSEBOX)
	{
		GetParent()->PostMessage(WM_CLOSE,0,0);
		
	}else if(m_nStyle == KRSYSBTN_MINIMIZEBOX){
		GetParent()->PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
	}else if(m_nStyle == KRSYSBTN_MAXIMIZEBOX){
		GetParent()->PostMessage(WM_SYSCOMMAND, GetParent()->IsZoomed()? SC_RESTORE :SC_MAXIMIZE ,0);
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CSystemButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD dwStyle = GetStyle();  
    SetWindowLong(GetSafeHwnd(),GWL_STYLE,dwStyle | SS_OWNERDRAW  );  
    CStatic::PreSubclassWindow();  
}



void CSystemButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK())
	{
		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );
		
		if(m_bHot){
			SolidBrush brush(Color(m_HOTR,m_HOTG,m_HOTB));
			raphics.FillRectangle(&brush,0,0,rc.Width(),rc.Height());
		}else{
			SolidBrush SBBK(Color(m_BKR,m_BKG,m_BKB));
			raphics.FillRectangle(&SBBK,0,0,rc.Width(),rc.Height());
		}
		
		Pen penLine(Color(255,255,255),2);
		
		if(m_nStyle == KRSYSBTN_CLOSEBOX)
		{
			penLine.SetStartCap(LineCapTriangle);  
			penLine.SetEndCap(LineCapTriangle);  
			raphics.DrawLine(&penLine,Point(4,4),Point(12,12));
			raphics.DrawLine(&penLine,Point(4,12),Point(12,4));
			
		}else if(m_nStyle == KRSYSBTN_MINIMIZEBOX){
			raphics.DrawLine(&penLine,Point(4,10),Point(14,10));
		} else if(m_nStyle == KRSYSBTN_MAXIMIZEBOX){
			if(!GetParent()->IsZoomed())
			{
				raphics.DrawRectangle(&penLine,6,6,6,6);
			}else{
				raphics.DrawRectangle(&penLine,7,5,6,6);
				
				if(m_bHot){
					SolidBrush brush(Color(m_HOTR,m_HOTG,m_HOTB));
					raphics.FillRectangle(&brush,4,8,6,6);
				}else{
					SolidBrush SBBK(Color(m_BKR,m_BKG,m_BKB));
					raphics.FillRectangle(&SBBK,4,8,6,6);
				}
				raphics.DrawRectangle(&penLine,4,8,6,6);
			}
		}
	}
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CSystemButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
