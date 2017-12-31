#if !defined(AFX_INDEXWIDGET_H__FFDC69BF_7834_47BC_87FC_C8824F7FC3ED__INCLUDED_)
#define AFX_INDEXWIDGET_H__FFDC69BF_7834_47BC_87FC_C8824F7FC3ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndexWidget window

#include "HQFunction.h"
class CIndexWidget : public CStatic
{
// Construction
public:
	CIndexWidget();
	INT GetDefaultWidth(){ return 1020; }
	INT GetDefaultHeight(){return 24; }
	INT GetTextWidth(CDC * pDC,LPCTSTR lpszText="Äã");
	INT GetTextHeight(CDC * pDC,LPCTSTR lpszText="ºÃ");
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndexWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIndexWidget)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL DrawIndex(CDC * pDc, CRect rc,const char * szTitle,const REALTIMEQUOTES * pRTQ, DWORD dwRedCount ,DWORD dwGreenCount  ,DWORD  dwWhiteCount);
	CFont m_Font;
	BOOL  m_bRealTimeQuotesOK;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXWIDGET_H__FFDC69BF_7834_47BC_87FC_C8824F7FC3ED__INCLUDED_)
