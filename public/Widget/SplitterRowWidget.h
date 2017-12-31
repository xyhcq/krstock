#ifndef SplitterRowWidget_h__
#define SplitterRowWidget_h__


class CSplitterColWidget;

enum{SRW_FixedMode_Top=11,SRW_FixedMode_Bottom};



//ÐÐ·Ö¸îÆ÷
class CSplitterRowWidget
{
public:
	CSplitterRowWidget();

	void SetRect(CRect rc);
	void SetLine(INT nTop , INT nFixedMode);

	void BindTop(CWnd * pWnd);
	void BindBottom(CWnd * pWnd);
	
	void BindTopCol(CSplitterColWidget * pCol);
	void BindBottomCol(CSplitterColWidget * pCol);

	void BindTopRow( CSplitterRowWidget * pRow );
	void BindBottomRow( CSplitterRowWidget * pRow );

	void BindTopRect( CRect * pRect );
	void BindBottomRect( CRect * pRect );


	CRect GetTopRect();
	CRect GetBottomRect();	 

	BOOL PreTranslateMessage(CWnd * pWnd,MSG* pMsg);
private:
	void Adjuest();
	CRect				m_Rc;
	INT					m_nYOffset;
	INT					m_nFixedMode;
	BOOL				m_bLBDown;
	CWnd *  m_pTop, *	m_pBottom;
	CRect * m_pTopRect, *	m_pBottomRect;

	CSplitterRowWidget * m_pTopRow, * m_pBottomRow;
	CSplitterColWidget * m_pTopCol, * m_pBottomCol;
};




#endif // SplitterRowWidget_h__