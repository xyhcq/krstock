#if !defined(AFX_KRSDECLIENTDLG_H__5DB2081D_427B_44F7_9F4E_A5D442877E00__INCLUDED_)
#define AFX_KRSDECLIENTDLG_H__5DB2081D_427B_44F7_9F4E_A5D442877E00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KRSDEClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKRSDEClientDlg dialog
#include "Widget/SystemButton.h"
#include "Widget/KRProgressWidget.h"
#include "Widget/KRTextListWidget.h"
#include "Widget/MemoryDC.h"
class CKRSDEClientDlg : public CDialog
{
// Construction
public:
	CKRSDEClientDlg(CWnd* pParent = NULL);   // standard constructor
	static 	UINT CKRSDEClientDlg::ReConnect(LPVOID lpParameter);
	void CKRSDEClientDlg::InsertRunLog(LPCTSTR szFormat,... );
// Dialog Data
	//{{AFX_DATA(CKRSDEClientDlg)
	enum { IDD = IDD_KRSDE_DIALOG };
	CKRTextListWidget	m_TextListWidget;
	CKRProgressWidget	m_ProgressWidget;
	CSystemButton		m_MinimizeBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKRSDEClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKRSDEClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LRESULT OnHPClient_Connect(WPARAM wParam,LPARAM lParam);
	LRESULT OnHPClient_Receive(WPARAM wParam,LPARAM lParam);
	LRESULT OnHPClient_Close(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bConnected;
	DWORD m_dwRequstCount;
	DWORD m_dwRequstSize;
	CString m_szTitle;
	CTime   m_tServer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KRSDECLIENTDLG_H__5DB2081D_427B_44F7_9F4E_A5D442877E00__INCLUDED_)
