#if !defined(AFX_SYSTEMBUTTOM_H__D6A18722_9426_49A0_85A6_37B5C1629FC1__INCLUDED_)
#define AFX_SYSTEMBUTTOM_H__D6A18722_9426_49A0_85A6_37B5C1629FC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemButtom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemButtom window
enum	{ KRSYSBTN_CLOSEBOX=1,KRSYSBTN_MINIMIZEBOX,KRSYSBTN_MAXIMIZEBOX};


class CSystemButton : public CStatic
{
// Construction
public:
	CSystemButton();

// Attributes
public:
	void SetStyle(INT nStyle){
		m_nStyle = nStyle;
		if(GetSafeHwnd())
			RedrawWindow();
	}
	
	void SetBkColor(INT R,INT G,INT B){
		m_BKR = R;
		m_BKG = G;
		m_BKB = B;
		if(GetSafeHwnd())
			RedrawWindow();
	}
	
	void SetHotColor(INT R,INT G,INT B){
		m_HOTR = R;
		m_HOTG = G;
		m_HOTB = B;
		if(GetSafeHwnd())
			RedrawWindow();
	}
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemButtom)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSystemButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSystemButtom)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM ,LPARAM );
	DECLARE_MESSAGE_MAP()
private:
	INT m_nStyle;
	
	INT m_BKR;
	INT m_BKG;
	INT m_BKB;
	
	INT m_HOTR;
	INT m_HOTG;
	INT m_HOTB;
	
	BOOL m_bHot;
	BOOL m_bOver;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMBUTTOM_H__D6A18722_9426_49A0_85A6_37B5C1629FC1__INCLUDED_)
