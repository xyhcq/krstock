#if !defined(AFX_HQReportDlg_H__11B2C1C0_6988_4192_BDE5_D55773113C7F__INCLUDED_)
#define AFX_HQReportDlg_H__11B2C1C0_6988_4192_BDE5_D55773113C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HQReportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHQReportDlg dialog
#include "resource.h"
#include "Widget/SystemButton.h"
#include "StockListWidget.h"
#include "TextButton.h"



#define WM_TEXTBUTTON		(WM_USER + 0x913)

class CHQReportDlg : public CDialog
{
// Construction
public:
	CHQReportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHQReportDlg)
	enum { IDD = IDD_MYSTOCK_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CSystemButton	m_BtnMinimize;
	CSystemButton	m_BtnMaximize;
	CSystemButton	m_BtnClose;

	void   SwitchList(INT nIndex);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHQReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHQReportDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnTextButton(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CRect m_RCTitle;
	CFont m_Font;
	CTextButton m_TextButton;
	CStockListWidget  m_StockListWidget[9];
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HQReportDlg_H__11B2C1C0_6988_4192_BDE5_D55773113C7F__INCLUDED_)
