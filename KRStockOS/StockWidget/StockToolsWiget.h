#if !defined(AFX_STOCKTOOLSWIGET_H__33B765BF_4A3A_47C5_89D2_9993D6D11494__INCLUDED_)
#define AFX_STOCKTOOLSWIGET_H__33B765BF_4A3A_47C5_89D2_9993D6D11494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StockToolsWiget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStockToolsWiget window
#include "TextButton.h"

#define WM_TEXTBUTTON		(WM_USER + 0x913)


class CStockToolsWiget : public CStatic
{
// Construction
public:
	CStockToolsWiget();
	INT GetDefaultWidth(){ return 800; }
	INT GetDefaultHeight(){return 20; }
	void SetLabel(const char * szLabel);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockToolsWiget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStockToolsWiget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStockToolsWiget)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnTextButton(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CFont		m_Font;
	CTextButton m_TextButton;
	CString     m_szLabel;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKTOOLSWIGET_H__33B765BF_4A3A_47C5_89D2_9993D6D11494__INCLUDED_)
