#if !defined(AFX_FILTERSETTINGDLG_H__E2E0A269_E1C8_4FA1_80FC_326AF6E93C81__INCLUDED_)
#define AFX_FILTERSETTINGDLG_H__E2E0A269_E1C8_4FA1_80FC_326AF6E93C81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterSettingDlg dialog
#include "Widget/SystemButton.h"
class CFilterSettingDlg : public CDialog
{
// Construction
public:
	CFilterSettingDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * & pImg)  ;
	BOOL DrawImage(UINT nID, LPCTSTR sTR,Graphics & raphics , INT x, INT y , INT & nImageWidth, INT & nImageHeight);
// Dialog Data
	//{{AFX_DATA(CFilterSettingDlg)
	enum { IDD = IDD_FILTERSETTING_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

		CSystemButton	m_BtnClose;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterSettingDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg UINT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	CBrush m_BKBrush;
	CFont				m_Font;
	CRect				m_RCTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSETTINGDLG_H__E2E0A269_E1C8_4FA1_80FC_326AF6E93C81__INCLUDED_)
