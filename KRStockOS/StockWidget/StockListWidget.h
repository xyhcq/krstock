#if !defined(AFX_STOCKLISTWIDGET_H__94DF1D57_58F3_42B0_B0E6_2A4157846B82__INCLUDED_)
#define AFX_STOCKLISTWIDGET_H__94DF1D57_58F3_42B0_B0E6_2A4157846B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StockListWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStockListWidget window
#include "MSortListCtrl.h"
#include "../DataMgr/ZXGDataMgr.h"

class CStockListWidget : public CStatic
{
// Construction
public:
	CStockListWidget();
	BOOL m_bAutoUpdate;
	BOOL m_bRelevanceZXG; //关联自选股

	CMSortListCtrl m_List;
// Attributes
public:
	BOOL AddLabel(const char * szLabel);
	BOOL AddLabel(const REALTIMEQUOTES * pRTQ);
	BOOL InsertLabel(const REALTIMEQUOTES * pRTQ , INT nItem);
	INT  GetCount();

	void UpdateItem(INT nItem, const REALTIMEQUOTES * pRTQ);
	INT  Find(const char * szLabel);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockListWidget)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStockListWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStockListWidget)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
	void    OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	void    OnDbclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT DragMoveItem(WPARAM wParam, LPARAM lParam);  /*声明自定义的消息响应函数*/
	afx_msg LRESULT OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CFont m_Font,m_ListFont;
	CMenu m_Menu;
	CMenu m_Menu_Sub;
	INT   m_nCurlSel;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKLISTWIDGET_H__94DF1D57_58F3_42B0_B0E6_2A4157846B82__INCLUDED_)
