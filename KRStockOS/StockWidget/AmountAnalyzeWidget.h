#if !defined(AFX_AMOUNTANALYZEWIDGET_H__76491DB9_AEC4_46AD_9561_2F2F31126B29__INCLUDED_)
#define AFX_AMOUNTANALYZEWIDGET_H__76491DB9_AEC4_46AD_9561_2F2F31126B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AmountAnalyzeWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAmountAnalyzeWidget window

typedef struct  _ZJLRLCINFO
{
	float  fLRDaDan;
	float  fLRZhongDan;
	float  fLRXiaoDan;
	float  fLCDaDan;
	float  fLCZhongDan;
	float  fLCXiaoDan;
}ZJLRLCINFO,*LPZJLRLCINFO;



class CAmountAnalyzeWidget : public CStatic
{
// Construction
public:
	CAmountAnalyzeWidget();
	void SetLabel(const char * szLabel);
	INT GetDefaultWidth(){ return 230; }
	INT GetDefaultHeight(){return 220; }
	REALTIMEQUOTES			m_RTQ;
private:
//	static	UINT THSWeb_UpdateZJInfoThread(LPVOID lpParameter);
	void DrawZJInfo(CDC * pDC);
	void DrawSmall(CDC * pDC);

	INT GetTextWidth(CDC * pDC,LPCTSTR lpszText="Äã");
	INT GetTextHeight(CDC * pDC,LPCTSTR lpszText="ºÃ");
// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmountAnalyzeWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAmountAnalyzeWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAmountAnalyzeWidget)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnKRSDE_TradingRecord_Remote(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CFont m_Font;
	ZJLRLCINFO m_ZJLRLCInfo;
	CRect m_RCZJInfo;
	const FINANCIAL	 *		m_pFinData;
	vector<TRADINGRECORD>	m_VTradingRecord;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMOUNTANALYZEWIDGET_H__76491DB9_AEC4_46AD_9561_2F2F31126B29__INCLUDED_)
