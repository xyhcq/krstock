#include "StdAfx.h"
#include "krscrollvbar.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


#define TIMER_LBUTTOMDOWN					0x911911

CKRScrollVBar::CKRScrollVBar()
{
	m_nBtnWidth		= GetSystemMetrics(SM_CXHSCROLL);
	m_nBtnHeight	= GetSystemMetrics(SM_CYHSCROLL);

	m_bDownBtnHot		= FALSE;
	m_bUpBtnHot			= FALSE;
	m_bBlockHot			= FALSE;
	m_bDragging			= FALSE;
	m_bTrackMouseLeave	= FALSE;
	m_bShowWindow		= FALSE;

	m_nPos          = 0;
	m_nMinPos		= 0;
	m_nMaxPos		= 0;

	m_nPos          = 0;
	m_nMinPos		= 0;
	m_nMaxPos		= 0;
	m_nTrackSize    = 2;
	m_nDragYOffset  = 0;
	m_nBlockHeight  = 0;
}



CKRScrollVBar::~CKRScrollVBar()
{

	
}



BOOL CKRScrollVBar::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message){
	case  WM_MOUSELEAVE:
		{
			return OnMouseLeave();
		}break;
	case  WM_MOUSEMOVE:
		{
			if(m_pWnd->m_hWnd != ::GetFocus()) ::SetFocus(m_pWnd->m_hWnd);
			return OnMouseMove(pMsg->wParam,CPoint(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam)));
		}break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(pMsg->wParam,CPoint(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam)));
		}break;
	case WM_LBUTTONUP:
		{	
			return OnLButtonUp(pMsg->wParam,CPoint(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam)));
		}break;
	case WM_MOUSEWHEEL:
		{
			return OnMouseWheel(pMsg->wParam,GET_WHEEL_DELTA_WPARAM(pMsg->wParam),CPoint(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam)));
		}break;
	case WM_TIMER:
		{
			return OnTimer(pMsg->wParam);
		}break;
	}
	return FALSE;
}



void CKRScrollVBar::InitDownBtnRc(CRect & rc){
	m_DownBtnRc.left   = rc.Width() - m_nBtnWidth;
	m_DownBtnRc.right  = rc.Width();

	m_DownBtnRc.top    = rc.bottom - m_nBtnHeight;
	m_DownBtnRc.bottom = rc.bottom;
}


void CKRScrollVBar::InitUPBtnRc(CRect & rc){

	m_UpBtnRc.left   = rc.Width() - m_nBtnWidth;
	m_UpBtnRc.right  = rc.Width();
	m_UpBtnRc.top    = 0;
	m_UpBtnRc.bottom = m_nBtnHeight;
}




//画滚动条上下翻页按钮
void CKRScrollVBar::DrawUp_DownBtn(Graphics & raphics,CRect & rc)
{
	InitUPBtnRc(rc);
	InitDownBtnRc(rc);

	if(m_bUpBtnHot){
		SolidBrush scBrush(Color(80,255,255,255));
		raphics.FillRectangle(&scBrush,m_UpBtnRc.left,m_UpBtnRc.top,m_UpBtnRc.Width(),m_UpBtnRc.Height());
	}

	if(m_bDownBtnHot){
		SolidBrush scBrush(Color(80,255,255,255));
		raphics.FillRectangle(&scBrush,m_DownBtnRc.left,m_DownBtnRc.top,m_DownBtnRc.Width(),m_DownBtnRc.Height());
	}
	
	Pen fsPen(Color(255,255,255),2.0);

	raphics.DrawLine(&fsPen,Point(m_UpBtnRc.left+5,m_UpBtnRc.bottom-5) , Point(m_UpBtnRc.left + m_UpBtnRc.Width()/2,m_UpBtnRc.top + 5));
	raphics.DrawLine(&fsPen,Point(m_UpBtnRc.right-6,m_UpBtnRc.bottom-5) , Point(m_UpBtnRc.left + m_UpBtnRc.Width()/2,m_UpBtnRc.top + 5));

	raphics.DrawLine(&fsPen,Point(m_DownBtnRc.left+5,m_DownBtnRc.top+5) , Point(m_DownBtnRc.left + m_UpBtnRc.Width()/2,m_DownBtnRc.bottom - 5));
	raphics.DrawLine(&fsPen,Point(m_DownBtnRc.right-6,m_DownBtnRc.top+5) , Point(m_DownBtnRc.left + m_UpBtnRc.Width()/2,m_DownBtnRc.bottom - 5));
}


INT CKRScrollVBar::YToPos(INT Y){
	//Y 坐标点 在  Block范围里所占的比例 
	double a =  (double)(Y- m_UpBtnRc.bottom) / (double) GetBlockRange();
	//转化成对比的 Pos 偏移
	double p = a * (double)GetRange();
	if(p > m_nMaxPos) 
		p = m_nMaxPos;
	return (INT )p;
}



INT CKRScrollVBar::PosToY(INT nPos){
	double a  = (double)m_nPos / (double)GetRange();
	double y  = a * (double)GetBlockRange();
	return (INT )y + m_UpBtnRc.bottom;
}



BOOL CKRScrollVBar::OnPaint(Graphics & raphics, CRect & rc)
{
	if(m_bShowWindow == FALSE)
		return TRUE;


	DrawUp_DownBtn(raphics,rc);

	SolidBrush scBrush(Color(50,255,255,255));
 	raphics.FillRectangle(&scBrush,rc.Width() - m_UpBtnRc.Width(),0,m_UpBtnRc.Width(),rc.Height());

	if(m_nMinPos == 0 && m_nMaxPos == 0)
		return TRUE;

	m_nBlockHeight = m_DownBtnRc.top - m_UpBtnRc.bottom  - GetRange();
	if(m_nBlockHeight < 8) m_nBlockHeight = 8;

	m_BlockRc.left   = rc.Width() - m_nBtnWidth;
	m_BlockRc.right  = rc.Width();
	m_BlockRc.top    = PosToY(m_nPos);
	m_BlockRc.bottom = m_BlockRc.top + m_nBlockHeight;

	if(!m_bBlockHot)
	{
		SolidBrush blBrush(Color(100,255,255,255));
 		raphics.FillRectangle(&blBrush,m_BlockRc.left,m_BlockRc.top,m_BlockRc.Width(),m_BlockRc.Height());
	}else
	{
		SolidBrush blBrush(Color(200,255,255,255));
 		raphics.FillRectangle(&blBrush,m_BlockRc.left,m_BlockRc.top,m_BlockRc.Width(),m_BlockRc.Height());
	}
	return TRUE;
}


void CKRScrollVBar::SetTrackSize(INT nTrackSize){
	m_nTrackSize = nTrackSize;
}

void CKRScrollVBar::SetPos(INT nPos){
	m_nPos = nPos;
}


INT CKRScrollVBar::GetPos(){
	return m_nPos;	
}

void CKRScrollVBar::SetRange(int nMinPos, int nMaxPos, BOOL bRedraw /* = TRUE */ ){
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;
	if(m_pWnd  && bRedraw)
		m_pWnd->RedrawWindow();
}

void CKRScrollVBar::GetRange(int & nMinPos, int & nMaxPos){
	nMinPos = m_nMinPos;
	nMaxPos = m_nMaxPos;
}

INT CKRScrollVBar::GetRange(){
	return m_nMaxPos - m_nMinPos;
}


INT CKRScrollVBar::GetBlockRange(){
	return m_DownBtnRc.top - m_UpBtnRc.bottom - m_nBlockHeight;
}

INT CKRScrollVBar::GetWidth(){
	return m_nBtnWidth;
}

BOOL CKRScrollVBar::ShowWindow(int nCmdShow ){
	m_bShowWindow = (nCmdShow == SW_HIDE) ? FALSE:TRUE;
	return m_bShowWindow;
}


BOOL CKRScrollVBar::OnMouseLeave(){
	KillTimer(m_pWnd->m_hWnd,TIMER_LBUTTOMDOWN);
	m_bTrackMouseLeave = FALSE;
	m_bUpBtnHot = FALSE;
	m_bDownBtnHot = FALSE;
	m_bBlockHot  = FALSE;
	m_pWnd->RedrawWindow();
	return TRUE;
}

void CKRScrollVBar::SetPosEnd()
{
	m_nPos = m_nMaxPos;
	m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
}



BOOL CKRScrollVBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nMinPos == 0 && m_nMaxPos == 0)
		return TRUE;

	if ( m_bTrackMouseLeave == FALSE )
	{
		m_bTrackMouseLeave = TRUE;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof( TRACKMOUSEEVENT );
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_pWnd->m_hWnd;
		tme.dwHoverTime = 40;
		::TrackMouseEvent( &tme );
	}

	if(m_bDragging && nFlags == MK_LBUTTON){
		
		m_nPos = YToPos(point.y - m_nDragYOffset);

		if(m_nPos > m_nMaxPos)
 			m_nPos = m_nMaxPos;

		if(m_nPos < m_nMinPos)
			m_nPos = m_nMinPos;

		m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
		return TRUE;
	}


	if( m_UpBtnRc.PtInRect(point) && m_bUpBtnHot == FALSE){
		m_bUpBtnHot = TRUE;
		m_pWnd->RedrawWindow();
		return TRUE;
	}
	
	if( !m_UpBtnRc.PtInRect(point) && m_bUpBtnHot == TRUE){
		KillTimer(m_pWnd->m_hWnd,TIMER_LBUTTOMDOWN);
		m_bUpBtnHot = FALSE;
		m_pWnd->RedrawWindow();
		return TRUE;
	}
	
	if(m_DownBtnRc.PtInRect(point) && m_bDownBtnHot == FALSE){
		m_bDownBtnHot = TRUE;
		m_pWnd->RedrawWindow();
		return TRUE;
	}
	
	if( !m_DownBtnRc.PtInRect(point) && m_bDownBtnHot == TRUE){
		KillTimer(m_pWnd->m_hWnd,TIMER_LBUTTOMDOWN);
		m_bDownBtnHot = FALSE;
		m_pWnd->RedrawWindow();
		return TRUE;
	}

	if(m_BlockRc.PtInRect(point) && m_bBlockHot == FALSE){
		m_bBlockHot = TRUE;
		m_pWnd->RedrawWindow();
		return TRUE;
	}
	
	if( !m_BlockRc.PtInRect(point) && m_bBlockHot == TRUE){
		m_bBlockHot = FALSE;
		m_pWnd->RedrawWindow();
		return TRUE;
	}
	return FALSE;
}



BOOL CKRScrollVBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nMinPos == 0 && m_nMaxPos == 0)
		return TRUE;

	if(m_DownBtnRc.PtInRect(point))
	{
		::SetTimer(m_pWnd->m_hWnd,TIMER_LBUTTOMDOWN,100,NULL);
		if(m_nPos + 1 <= m_nMaxPos)
		{
			m_nPos++;
			m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
		}
		return TRUE;
	}

	if(m_UpBtnRc.PtInRect(point))
	{
		::SetTimer(m_pWnd->m_hWnd,TIMER_LBUTTOMDOWN,100,NULL);
		if(m_nPos - 1 >= m_nMinPos)
		{
			m_nPos--;
			m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
		}
		return TRUE;
	}


	if(!m_BlockRc.PtInRect(point))
	{
		CRect TopRc, BottomRc;
		TopRc.top		= m_UpBtnRc.bottom;
		TopRc.bottom	= m_BlockRc.top;
		TopRc.left		= m_UpBtnRc.left;
		TopRc.right		= m_UpBtnRc.right;
		
		BottomRc.top    = m_BlockRc.bottom;
		BottomRc.bottom = m_DownBtnRc.top;
		BottomRc.left   = m_DownBtnRc.left;
		BottomRc.right  = m_DownBtnRc.right;
		
		//调整滑动块
		if(TopRc.PtInRect(point)){
			m_nPos = YToPos(point.y);
			m_bBlockHot = TRUE;
			m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
			return TRUE;
		}else if(BottomRc.PtInRect(point)){
			m_nPos = YToPos(point.y);	
			m_bBlockHot = TRUE;
			m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
			return TRUE;
		}
	}else{
		m_bDragging = TRUE;
		m_nDragYOffset  = point.y - m_BlockRc.top;
	}
	return FALSE;
}




BOOL CKRScrollVBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(m_pWnd->m_hWnd,TIMER_LBUTTOMDOWN);
	m_bDragging =  FALSE;
	return FALSE;
}



BOOL CKRScrollVBar::OnTimer(INT nEventID){
	if(nEventID == TIMER_LBUTTOMDOWN){
		POINT point;
		::GetCursorPos(&point);
		ScreenToClient(m_pWnd->m_hWnd,&point);
		OnLButtonDown(0,CPoint(point.x,point.y));
	}
	return TRUE;
}


BOOL CKRScrollVBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0 ){
			m_nPos += m_nTrackSize;
			if(m_nPos > m_nMaxPos)
				m_nPos = m_nMaxPos;
		
			m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
			return TRUE;
	}else{
		m_nPos -= m_nTrackSize;
		if(m_nPos < m_nMinPos)
			m_nPos = m_nMinPos;

		m_pWnd->PostMessage(WM_SCROLLCHANGE,0,0);
		return TRUE;
	}
	return  FALSE;
}

