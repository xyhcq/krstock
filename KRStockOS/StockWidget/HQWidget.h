#if !defined(AFX_HQWIDGET_H__1797B83B_C4CB_4E83_B93F_5805824D6389__INCLUDED_)
#define AFX_HQWIDGET_H__1797B83B_C4CB_4E83_B93F_5805824D6389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HQWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHQWidget window
#include "HQFunction.h"


class CHQWidget : public CStatic
{
// Construction
public:
	CHQWidget();
	void SetLabel(const char * szLabel);
	INT GetDefaultWidth(){ return 230; }
	INT GetDefaultHeight(){return 534; }
private:
	INT GetTextWidth(CDC * pDC,LPCTSTR lpszText="Äã");
	INT GetTextHeight(CDC * pDC,LPCTSTR lpszText="ºÃ");
	void AdjustRC(CRect & rc);
	void DrawAllRC(CDC * pDC);
	void DrawTitle(CDC * pDC);
	void Draw5DPK(CDC * pDC);
	void DrawHQInfo(CDC * pDC);

	CRect SetListText(CDC * pDc, CRect & rc , int nColumnAve, 
		int nColumn, int nLine, LPCTSTR lpszText , int nTextHeight, int nTextLineFill , COLORREF nRGB ,UINT uFormat,int nColspan = 0);
	void DrawLine(CDC * pDc, CPen & pen ,CPoint & a, CPoint & b);
// Attributes
public:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHQWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHQWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHQWidget)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CRect m_RC5DPK;
	CRect m_RCHQInfo;
	CRect m_RCTitle;
	CFont m_Font;
	CFont m_FontSmall;
public:
	REALTIMEQUOTES		m_RTQ,m_OldRTQ;
	CHQFunction			m_HQFunction;
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HQWIDGET_H__1797B83B_C4CB_4E83_B93F_5805824D6389__INCLUDED_)
