#if !defined(AFX_KEYDEMONDLG_H__CCA378FC_4B30_4C15_B7A7_9E8566DBC217__INCLUDED_)
#define AFX_KEYDEMONDLG_H__CCA378FC_4B30_4C15_B7A7_9E8566DBC217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyDemonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyDemonDlg dialog
#include <VECTOR>
#include <map>
using namespace std;
#include "Widget/SystemButton.h"

class CKeyDemonDlg : public CDialog
{
// Construction
public:
	CKeyDemonDlg(CWnd* pParent = NULL);   // standard constructor
	INT  m_nLeft,m_nTop;
	CString m_szInitText;
	CString m_szTitle;
	void CKeyDemonDlg::UpdateList(CString & szKey);	
// Dialog Data
	//{{AFX_DATA(CKeyDemonDlg)
	enum { IDD = IDD_KEYDEMOM_DIALOG };
	CListBox	m_List;
	//}}AFX_DATA
	CSystemButton	m_BtnClose;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyDemonDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyDemonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangeInput();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDblclkList();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFont m_Font;

	map<string , string > m_PYDB;
	map<string , string > m_SZDB;

	INT   m_ListSelIndex;

	BOOL  m_bFirst;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYDEMONDLG_H__CCA378FC_4B30_4C15_B7A7_9E8566DBC217__INCLUDED_)
