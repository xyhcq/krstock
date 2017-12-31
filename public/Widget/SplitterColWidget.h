#ifndef SplitterColWidget_h__
#define SplitterColWidget_h__



class CSplitterRowWidget;

enum{SCW_FixedMode_Left=1,SCW_FixedMode_Right};



//ÁÐ·Ö¸îÆ÷
class CSplitterColWidget
{
public:
	CSplitterColWidget();

	void SetRect(CRect rc);
	void SetLine(INT nLeft , INT nFixedMode);

	void BindLeft(CWnd * pWnd);
	void BindRight(CWnd * pWnd);

	void BindLeftRow(CSplitterRowWidget * pRow);
	void BindRightRow(CSplitterRowWidget * pRow);

	CRect GetLeftRect();
	CRect GetRightRect();	 
	BOOL PreTranslateMessage(CWnd * pWnd,MSG* pMsg);

private:
	void Adjuest();
	CRect				m_Rc;
	INT					m_nXOffset;
	INT					m_nFixedMode;
	BOOL				m_bLBDown;
	CWnd * m_pLeft, *	m_pRight;
	CSplitterRowWidget * m_pLeftRow, * m_pRightRow;
};



#endif // SplitterColWidget_h__