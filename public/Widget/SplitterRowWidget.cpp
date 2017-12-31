#include "StdAfx.h"
#include "SplitterRowWidget.h"
#include "SplitterColWidget.h"

CSplitterRowWidget::CSplitterRowWidget()
{
	m_nYOffset		= 0;
	m_bLBDown		= FALSE;
	m_nFixedMode	= SRW_FixedMode_Bottom;
	m_pTop			= NULL;
	m_pBottom       = NULL;
	
	m_pTopCol       = NULL;
	m_pBottomCol    = NULL;
	m_pTopRow       = NULL;
	m_pBottomRow    = NULL;

	m_pTopRect      = NULL;
	m_pBottomRect   = NULL;
}



void CSplitterRowWidget::SetRect( CRect rc )
{
	if(m_nYOffset == 0)
	{
		m_Rc		= rc;
	}else
	{
		if(m_nFixedMode == SRW_FixedMode_Bottom)
		{
			INT nOffset = m_Rc.bottom - m_nYOffset;
			m_Rc = rc;
			m_nYOffset = m_Rc.bottom - nOffset;
			Adjuest();
		}else if(m_nFixedMode == SRW_FixedMode_Top)
		{
			m_Rc = rc;
			Adjuest();
		}
	}
}


void CSplitterRowWidget::SetLine( INT nTop , INT nFixedMode )
{
	m_nYOffset		= nTop;
	m_nFixedMode	= nFixedMode;
	Adjuest();
}

void CSplitterRowWidget::BindTop( CWnd * pWnd )
{
	m_pTop = pWnd;
}

void CSplitterRowWidget::BindBottom( CWnd * pWnd )
{
	m_pBottom = pWnd;
}

void CSplitterRowWidget::BindTopCol( CSplitterColWidget * pCol )
{
	m_pTopCol = pCol;
}


void CSplitterRowWidget::BindBottomCol( CSplitterColWidget * pCol )
{
	m_pBottomCol = pCol;
}

void CSplitterRowWidget::BindTopRow( CSplitterRowWidget * pRow )
{
	m_pTopRow = pRow;
}


void CSplitterRowWidget::BindBottomRow( CSplitterRowWidget * pRow )
{
	m_pBottomRow = pRow;
}

void CSplitterRowWidget::BindTopRect( CRect * pRect )
{
	m_pTopRect = pRect;
}

void CSplitterRowWidget::BindBottomRect( CRect * pRect )
{
	m_pBottomRect = pRect;
}


CRect CSplitterRowWidget::GetTopRect()
{
	CRect rc;
	rc = m_Rc;
	rc.bottom = m_nYOffset -1;
	return rc;
}

CRect CSplitterRowWidget::GetBottomRect()
{
	CRect rc;
	rc = m_Rc;
	rc.top = m_nYOffset +1;
	return rc;
}

BOOL CSplitterRowWidget::PreTranslateMessage( CWnd * pWnd,MSG* pMsg )
{
	if(m_nYOffset == 0)
		return FALSE;
	
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			CPoint point;
			point.x = LOWORD(pMsg->lParam); 
			point.y = HIWORD(pMsg->lParam);
			
			//不能越界
			if((point.y - 4) <= m_Rc.top || point.y + 4 >= m_Rc.bottom)
				return FALSE;
			
			CRect rc;
			rc.top		= m_nYOffset - 1;
			rc.bottom	= m_nYOffset + 1;
			rc.left		= m_Rc.left;
			rc.right	= m_Rc.right;
			
			
			if(m_bLBDown)
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZENS));
				m_nYOffset = point.y;
				pWnd->RedrawWindow(rc);
				CClientDC dc(pWnd);
				CPen RedPen(PS_DOT,1,RGB(192,192,192));
				dc.SelectObject(RedPen);
				dc.MoveTo( m_Rc.left, m_nYOffset);
				dc.LineTo( m_Rc.right , m_nYOffset);
				return TRUE;
			}
			
			if(!m_bLBDown && rc.PtInRect(point))
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZENS));
				return TRUE;
			}
		}break;
		
	case WM_LBUTTONDOWN:
		{
			CPoint point;
			point.x = LOWORD(pMsg->lParam); 
			point.y = HIWORD(pMsg->lParam);
			
			CRect rc;
			rc.top		= m_nYOffset - 1;
			rc.bottom	= m_nYOffset + 1;
			rc.left		= m_Rc.left;
			rc.right	= m_Rc.right;
			
			if(rc.PtInRect(point))
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZENS));
				m_bLBDown = TRUE;
				pWnd->SetCapture();
				return TRUE;
			}
		}break;
		
	case WM_LBUTTONUP:
		if(m_bLBDown)
		{
			m_bLBDown = FALSE;
			ReleaseCapture();
			CPoint point;
			point.x = LOWORD(pMsg->lParam); 
			point.y = HIWORD(pMsg->lParam);
			CRect rc;
			rc.left		= m_Rc.left;
			rc.right	= m_Rc.right;
			rc.top		= point.y - 1;
			rc.bottom	= point.y + 1;

			pWnd->RedrawWindow(rc);
			Adjuest();

			if(m_pTopRect || m_pBottomRect)
				pWnd->RedrawWindow();
			return TRUE;
		}break;
	}
	
	BOOL bResult = FALSE;
	if(m_pTopCol)
		bResult = m_pTopCol->PreTranslateMessage(pWnd,pMsg);
	
	if( m_pBottomCol)
		bResult = m_pBottomCol->PreTranslateMessage(pWnd,pMsg);

	if(m_pTopRow)
		bResult = m_pTopRow->PreTranslateMessage(pWnd,pMsg);
	
	if(m_pBottomRow)
		bResult = m_pBottomRow->PreTranslateMessage(pWnd,pMsg);
	

	return bResult;
}

void CSplitterRowWidget::Adjuest()
{
	CRect rcTop    = GetTopRect();
	CRect rcBottom = GetBottomRect();
	
	if(m_pTop)
		m_pTop->MoveWindow(rcTop,TRUE);
	
	if(m_pBottom)
		m_pBottom->MoveWindow(rcBottom,TRUE);
	
	if(m_pTopCol)
		m_pTopCol->SetRect(rcTop);

	if(m_pBottomCol)
		m_pBottomCol->SetRect(rcBottom);

	if(m_pTopRow)
		m_pTopRow->SetRect(rcTop);

	if(m_pBottomRow)
		m_pBottomRow->SetRect(rcBottom);

	if(m_pTopRect)
		*m_pTopRect = rcTop;

	if(m_pBottomRect)
		*m_pBottomRect = rcBottom;
}
