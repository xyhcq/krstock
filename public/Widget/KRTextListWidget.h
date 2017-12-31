#if !defined(AFX_KRTEXTLISTWIDGET_H__7246F314_540E_48CA_97AE_4956AD765A9D__INCLUDED_)
#define AFX_KRTEXTLISTWIDGET_H__7246F314_540E_48CA_97AE_4956AD765A9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// krtextlistwidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKRTextListWidget window
#include "krscrollvbar.h"
#include <vector>
#include <string>
using namespace std;


class CKRTextListWidget : public CStatic
{
// Construction
public:
	CKRTextListWidget();

	void SetBkColor(INT R,INT G,INT B);
	void AddString(const char * szText);
	void AddString(const wchar_t * szText);
	void InsertString(INT nIndex,const char * szText);
	void InsertString(INT nIndex,const wchar_t * szText);
	INT GetCount();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKRTextListWidget)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKRTextListWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKRTextListWidget)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnScrollChange(LPARAM lParam,WPARAM wParam);
	DECLARE_MESSAGE_MAP()
private:
	
	INT m_BKR;
	INT m_BKG;
	INT m_BKB;
	CKRScrollVBar	m_KRScrollVBar;
	vector<wstring> m_VText;
	INT             m_VTextIndex;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KRTEXTLISTWIDGET_H__7246F314_540E_48CA_97AE_4956AD765A9D__INCLUDED_)
