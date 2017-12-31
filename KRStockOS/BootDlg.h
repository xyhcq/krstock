//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_BOOTDLG_H__55584AC7_CB62_4826_B75F_1AFA9173A5A7__INCLUDED_)
#define AFX_BOOTDLG_H__55584AC7_CB62_4826_B75F_1AFA9173A5A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BootDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBootDlg dialog

class CBootDlg : public CDialog
{
// Construction
public:
	CBootDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBootDlg)
	enum { IDD = IDD_BOOT_DLG };
	CWebBrowser2	m_IE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBootDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBootDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	Image * m_pImg;
	HICON  m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOTDLG_H__55584AC7_CB62_4826_B75F_1AFA9173A5A7__INCLUDED_)
