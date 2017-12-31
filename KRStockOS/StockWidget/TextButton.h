#ifndef TextButton_h__
#define TextButton_h__

#include <VECTOR>

typedef struct _MTBINFO
{
	CRect    rc;
	CString  szText;
	INT      nCommandID;
}MTBINFO,*LPMTBINFO;

 

class CTextButton
{
public:
	CTextButton()
	{
		m_nIndex = -1;
		m_bFiexdIndex = FALSE;
	}


	void SetIndex(INT n)
	{
		m_nIndex=n;
	}


	void SetFiexdIndex()
	{
		m_bFiexdIndex = TRUE;
	}

	void SetMessage(INT nMsg)
	{
		m_nMsg = nMsg;
	}

	BOOL Add(LPCTSTR lpszText, INT nCommandID, INT x , INT y , CDC * pDC )
	{
		for (INT i=0; i< m_VTBInfo.size(); i++)
		{
			if(m_VTBInfo[i].nCommandID == nCommandID)
				return TRUE;
		}

		MTBINFO TBInfo;
		TBInfo.rc.left = x;
		TBInfo.rc.right= x+ pDC->GetTextExtent(lpszText).cx;
		TBInfo.rc.top  = y;
		TBInfo.rc.bottom = y+ pDC->GetTextExtent(lpszText).cy;

		TBInfo.szText = lpszText;
		TBInfo.nCommandID = nCommandID;
		m_VTBInfo.push_back(TBInfo);
		return TRUE;
	}

	void ReDraw(CDC * pDC , CBrush * RectBrush, COLORREF cNor , COLORREF cHot)
	{
		for (INT i=0; i< m_VTBInfo.size(); i++)
		{
			CRect rc = m_VTBInfo[i].rc;
			rc.top -= 2;
			rc.bottom += 1;
			rc.left   -= 2;
			rc.right  += 2;
			pDC->FrameRect(rc,RectBrush);

			pDC->SetTextColor( i == m_nIndex ? cHot : cNor);
			pDC->DrawText(m_VTBInfo[i].szText,m_VTBInfo[i].rc,DT_CENTER);
		}
	}


	BOOL OnMouseMove(CWnd * pWnd, UINT nFlags, CPoint point)
	{
		if(nFlags == MK_LBUTTON || m_bFiexdIndex)
			return FALSE;


		INT nIndex = -1;
		for (INT i=0; i< m_VTBInfo.size(); i++)
		{
			if( m_VTBInfo[i].rc.PtInRect(point) )
			{
				nIndex = i;
				break;
			}
		}

		//索引有变化了 需要重绘
		if(m_nIndex != nIndex )
		{
			m_nOldInde = m_nIndex;
			m_nIndex   = nIndex;
			pWnd->RedrawWindow( m_VTBInfo[m_nOldInde].rc );
			pWnd->RedrawWindow( m_VTBInfo[i].rc );
			return TRUE;
		}
		return FALSE;
	}

	BOOL OnLButtonDown(CWnd * pWnd,UINT nFlags, CPoint point) 
	{
		INT nIndex = -1;
		for (INT i=0; i< m_VTBInfo.size(); i++)
		{
			if( m_VTBInfo[i].rc.PtInRect(point) )
			{
				if(m_bFiexdIndex == FALSE)
				{
					if(pWnd->SendMessage(m_nMsg,m_VTBInfo[i].nCommandID,NULL))
					{
						m_nIndex = -1;
						pWnd->RedrawWindow( m_VTBInfo[i].rc );
					}
				}else 
				{
					if(m_nIndex != i)
					{
						m_nOldInde = m_nIndex;
						m_nIndex   = -1;
						pWnd->RedrawWindow( m_VTBInfo[m_nOldInde].rc );
						m_nIndex = i;
						pWnd->RedrawWindow( m_VTBInfo[m_nIndex].rc );
						pWnd->SendMessage(m_nMsg,m_VTBInfo[i].nCommandID,NULL);
					}
				}
				return TRUE;
			}
		}
		return FALSE;
	}



private:
	vector<MTBINFO> m_VTBInfo;
	INT m_nIndex;
	INT m_nOldInde;
	INT m_nMsg;
	BOOL m_bFiexdIndex;
};

#endif // TextButton_h__