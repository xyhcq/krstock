#ifndef krscrollvbar_h__
#define krscrollvbar_h__

#define WM_SCROLLCHANGE			(WM_USER+0x911)

class CKRScrollVBar{
public:
	CKRScrollVBar();
	~CKRScrollVBar();

	BOOL PreTranslateMessage(MSG* pMsg);

	BOOL OnPaint(Graphics & raphics, CRect &  rc);
	
	void SetCWnd(CWnd * pWnd){	m_pWnd = pWnd;	}

	void SetTrackSize(INT nTrackSize);
	void SetPos(INT nPos);
	INT  GetPos();
	void SetRange(int nMinPos, int nMaxPos,  BOOL bRedraw = TRUE );
	void GetRange(int & nMinPos, int & nMaxPos);
	INT  GetRange();
	void SetPosEnd();

	INT GetWidth();

	BOOL ShowWindow(int nCmdShow );

private:
	void InitBlockRc(CRect & rc);
	void InitUPBtnRc(CRect & rc);
	void InitDownBtnRc(CRect & rc);
	void DrawUp_DownBtn(Graphics & raphics,CRect & rc);
	INT GetBlockRange();
	INT YToPos(INT Y);
	INT PosToY(INT nPos);
private:
	BOOL CKRScrollVBar::OnLButtonDown(UINT nFlags, CPoint point);
	BOOL CKRScrollVBar::OnMouseMove(UINT nFlags, CPoint point);
	BOOL CKRScrollVBar::OnLButtonUp(UINT nFlags, CPoint point);
	BOOL CKRScrollVBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	BOOL CKRScrollVBar::OnTimer(INT nEventID);
	BOOL CKRScrollVBar::OnMouseLeave();
private:
	INT   m_nBtnHeight;
	INT   m_nBtnWidth;
	CRect m_UpBtnRc;
	CRect m_DownBtnRc;

	BOOL  m_bUpBtnHot;
	BOOL  m_bDownBtnHot;
	
	CRect m_BlockRc;
	BOOL  m_bBlockHot;
	INT   m_nBlockHeight;

	CWnd * m_pWnd;

	INT   m_nPos;
	INT	  m_nMinPos;
	INT   m_nMaxPos;
	INT   m_nTrackSize;

	BOOL  m_bDragging;
	INT   m_nDragYOffset;

	BOOL  m_bTrackMouseLeave;

	BOOL  m_bShowWindow;


};



#endif // krscrollvbar_h__