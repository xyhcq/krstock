#include "StdAfx.h"
#include "SplitterColWidget.h"
#include "SplitterRowWidget.h"

CSplitterColWidget::CSplitterColWidget()
{
	m_nXOffset		= 0;
	m_bLBDown		= FALSE;
	m_nFixedMode	= SCW_FixedMode_Left;
	m_pLeft			= NULL;
	m_pRight        = NULL;
	m_pLeftRow      = NULL;
	m_pRightRow     = NULL;
}

void CSplitterColWidget::SetRect( CRect rc )
{
	if(m_nXOffset == 0)
	{
		m_Rc		= rc;
	}else
	{
		if(m_nFixedMode == SCW_FixedMode_Right)
		{
			INT nOffset = m_Rc.right - m_nXOffset;
			m_Rc = rc;
			m_nXOffset = m_Rc.right - nOffset;
			Adjuest();
		}else if(m_nFixedMode == SCW_FixedMode_Left)
		{
			m_Rc = rc;
			Adjuest();
		}
	}
}

void CSplitterColWidget::SetLine( INT nLeft , INT nFixedMode )
{
	m_nXOffset		= nLeft;
	m_nFixedMode	= nFixedMode;
	Adjuest();
}

void CSplitterColWidget::BindLeft( CWnd * pWnd )
{
	m_pLeft = pWnd;
}

void CSplitterColWidget::BindRight( CWnd * pWnd )
{
	m_pRight = pWnd;
}

void CSplitterColWidget::BindLeftRow( CSplitterRowWidget * pRow )
{
	m_pLeftRow = pRow;
}

void CSplitterColWidget::BindRightRow( CSplitterRowWidget * pRow )
{
	m_pRightRow = pRow;
}

CRect CSplitterColWidget::GetLeftRect()
{
	CRect rc;
	rc = m_Rc;
	rc.right = m_nXOffset -1;
	return rc;
}

CRect CSplitterColWidget::GetRightRect()
{
	CRect rc;
	rc = m_Rc;
	rc.left = m_nXOffset +1;
	return rc;
}

BOOL CSplitterColWidget::PreTranslateMessage( CWnd * pWnd,MSG* pMsg )
{
	if(m_nXOffset == 0)
		return FALSE;
	
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			CPoint point;
			point.x = LOWORD(pMsg->lParam); 
			point.y = HIWORD(pMsg->lParam);
			
			//不能越界
			if((point.x - 4) <= m_Rc.left || point.x + 4 >= m_Rc.right)
				return FALSE;
			
			CRect rc;
			rc.left		= m_nXOffset - 1;
			rc.right	= m_nXOffset + 1;
			rc.top		= m_Rc.top;
			rc.bottom	= m_Rc.bottom;
			if(m_bLBDown)
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
				m_nXOffset = point.x;
				pWnd->RedrawWindow(rc);
				CClientDC dc(pWnd);
				CPen RedPen(PS_DOT,1,RGB(192,192,192));
				dc.SelectObject(RedPen);
				dc.MoveTo(m_nXOffset , m_Rc.top);
				dc.LineTo(m_nXOffset , m_Rc.bottom);
				return TRUE;
			}
			
			if(!m_bLBDown && rc.PtInRect(point))
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
				return TRUE;
			}
		}break;
		
	case WM_LBUTTONDOWN:
		{
			CPoint point;
			point.x = LOWORD(pMsg->lParam); 
			point.y = HIWORD(pMsg->lParam);
			
			CRect rc;
			rc.left		= m_nXOffset - 1;
			rc.right	= m_nXOffset + 1;
			rc.top		= m_Rc.top;
			rc.bottom	= m_Rc.bottom;
			if(rc.PtInRect(point))
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
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
			rc.left		= point.x - 1;
			rc.right	= point.x + 1;
			rc.top		= m_Rc.top;
			rc.bottom	= m_Rc.bottom;
			pWnd->RedrawWindow(rc);
			Adjuest();
			return TRUE;
		}break;
	}
	
	BOOL bResult = FALSE;
	if(m_pLeftRow)
		bResult = m_pLeftRow->PreTranslateMessage(pWnd,pMsg);
	
	if( m_pRightRow)
		bResult = m_pRightRow->PreTranslateMessage(pWnd,pMsg);
	
	return bResult;
}

void CSplitterColWidget::Adjuest()
{
	CRect rcLeft  = GetLeftRect();
	CRect rcRight = GetRightRect();
	
	if(m_pLeft)
		m_pLeft->MoveWindow(rcLeft,TRUE);
	
	if(m_pRight)
		m_pRight->MoveWindow(rcRight,TRUE);
	
	if(m_pLeftRow)
		m_pLeftRow->SetRect(rcLeft);
	
	if(m_pRightRow)
		m_pRightRow->SetRect(rcRight);
}
