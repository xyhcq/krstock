// KRStockOSDlg.h : header file
//

#if !defined(AFX_KRSTOCKOSDLG_H__813305B7_B190_409B_BC9C_FE171A8E7434__INCLUDED_)
#define AFX_KRSTOCKOSDLG_H__813305B7_B190_409B_BC9C_FE171A8E7434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSDlg dialog

#define WM_SETLABLE			(WM_USER + 0x911)
#define WM_SHOWKEYDEMON		(WM_USER + 0x912)
#define WM_TEXTBUTTON		(WM_USER + 0x913)


#include "Widget/SystemButton.h"
#include "Render.h"
#include "IndexWidget.h"
#include "TextButton.h"
#include "HQReportDlg.h"

class CKRStockOSDlg : public CDialog
{
// Construction
public:
	CKRStockOSDlg(CWnd* pParent = NULL);	// standard constructor
	void AdjustWidget(int x, int y,int cx, int cy);
// Dialog Data
	//{{AFX_DATA(CKRStockOSDlg)
	enum { IDD = IDD_KRSTOCKOS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CSystemButton	m_BtnMinimize;
	CSystemButton	m_BtnMaximize;
	CSystemButton	m_BtnClose;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKRStockOSDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKRStockOSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnTextButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowKeyDemonDlg(WPARAM wParam, LPARAM	lParam);
	afx_msg LRESULT OnSetLabel(WPARAM wParam, LPARAM	lParam);
	afx_msg LRESULT OnKRSDE_InitSuccess(WPARAM wParam, LPARAM	lParam);
	afx_msg LRESULT OnKRSDE_InitFalure(WPARAM wParam,	LPARAM	lParam);
	afx_msg LRESULT OnKRSDE_Disconnect(WPARAM wParam, LPARAM	lParam);
	DECLARE_MESSAGE_MAP()
private:
	CRect				m_RCTitle;
	CRect               m_RCKeyDemon;
	CKLineAnalyze		m_KLineAnalyze;
	CTextButton			m_TextButton;
	CFont				m_Font;
	CHQReportDlg	*   m_pHQReportDlg;
	CIndexWidget        m_IndexWidget;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KRSTOCKOSDLG_H__813305B7_B190_409B_BC9C_FE171A8E7434__INCLUDED_)
