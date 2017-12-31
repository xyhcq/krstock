#if !defined(AFX_KRPROGRESSWIDGET_H__681DA080_F648_46DF_9B8E_67FECDBA84AA__INCLUDED_)
#define AFX_KRPROGRESSWIDGET_H__681DA080_F648_46DF_9B8E_67FECDBA84AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KRProgressWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKRProgressWidget window
#include <string>
using namespace std;

enum{TIMER_AUTO,TIMER_SPEED};

class CKRProgressWidget : public CStatic
{
// Construction
public:
	CKRProgressWidget();
// Attributes
public:
	void SetAutoPos(BOOL bSet,DWORD dwDelay=100){
		if(bSet) SetTimer(TIMER_AUTO,dwDelay,NULL);
		else KillTimer(TIMER_AUTO);
	}

	void SetRange(DWORD nLower, DWORD nUpper){
		m_nRangeLower = nLower;
		m_nRangeUpper = nUpper;
	}

	void GetRange(DWORD & nLower, DWORD & nUpper){
		nLower = m_nRangeLower;
		nUpper = m_nRangeUpper;
	}

	DWORD GetUpper(){return m_nRangeUpper; }
	DWORD GetLower(){return m_nRangeLower; }

	int GetPos(){
		return m_nPos;
	}


	int SetPos(int nPos){
		if(nPos <= m_nRangeUpper && nPos > m_nRangeLower)
		{	
			float fCurPos = (float)m_nPos / (float)(m_nRangeUpper - m_nRangeLower) * 100;
			float fNewPos = (float) nPos / (float)(m_nRangeUpper - m_nRangeLower) * 100;
			if( fNewPos - fCurPos > 10.0f){
				//在10次Timer后完成加速
				m_dwSpeedBase = (float)(m_nRangeUpper - m_nRangeLower) / 100 * (fNewPos - fCurPos) / 10;
				m_dwSpeedTarget = nPos;
				SetTimer(TIMER_SPEED,60,NULL);
				return m_nPos;
			}
			m_nPos = nPos;
			if(GetSafeHwnd())
				RedrawWindow();
		}
		return m_nPos;
	}

	void SetTitleText(const WCHAR * lpszTitleText){
		m_TitleText = lpszTitleText;
		if(GetSafeHwnd())
			RedrawWindow();
	}

	void SetInfoText(const WCHAR * lpszInfoText){
		m_InfoText = lpszInfoText;
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

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKRProgressWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKRProgressWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKRProgressWidget)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	DWORD m_nRangeLower;
	DWORD m_nRangeUpper;
	DWORD m_nPos;
	DWORD m_dwSpeedTarget;
	DWORD m_dwSpeedBase;

	wstring m_TitleText;
	wstring m_InfoText;

	INT m_BKR;
	INT m_BKG;
	INT m_BKB;
	BOOL m_bShowWindow;
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KRPROGRESSWIDGET_H__681DA080_F648_46DF_9B8E_67FECDBA84AA__INCLUDED_)
