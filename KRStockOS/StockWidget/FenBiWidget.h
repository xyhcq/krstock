#if !defined(AFX_FENBIWIDGET_H__B9DD0320_4E39_47EC_872B_C932DFBDDA21__INCLUDED_)
#define AFX_FENBIWIDGET_H__B9DD0320_4E39_47EC_872B_C932DFBDDA21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FenBiWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFenBiWidget window
#include "HQFunction.h"
#include "Widget/KRScrollVbar.h"
#include <vector>
using namespace std;



class CFenBiWidget : public CStatic
{
// Construction
public:
	CFenBiWidget();
	void SetLabel(const char * szLabel);
// Attributes
private:
	INT			GetTextWidth(CDC * pDC,LPCTSTR lpszText="0");
	INT			GetTextHeight(CDC * pDC,LPCTSTR lpszText="0");
	void		DrawList(CDC * pDc);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFenBiWidget)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFenBiWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFenBiWidget)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	LRESULT OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnKRSDE_TradingRecord_Remote(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScrollChange(LPARAM lParam,WPARAM wParam);
	DECLARE_MESSAGE_MAP()
private:
	CFont m_Font;
	CRect m_RCTextList;
	CKRScrollVBar			m_KRScrollVBar;
	vector<TRADINGRECORD>	m_VTradingRecord;
	INT						m_VIndex;
	REALTIMEQUOTES			m_RTQ;
	BOOL					m_bSetPosEnd;
	const FINANCIAL	 *		m_pFinData;
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FENBIWIDGET_H__B9DD0320_4E39_47EC_872B_C932DFBDDA21__INCLUDED_)
