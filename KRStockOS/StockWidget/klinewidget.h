#if !defined(AFX_KLINEWIDGET_H__40B50BFB_CE6B_4F39_B029_09FE4176E4CB__INCLUDED_)
#define AFX_KLINEWIDGET_H__40B50BFB_CE6B_4F39_B029_09FE4176E4CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KLineWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKLineWidget window
#include "KRWave.h"
#include "HQFunction.h"
#include "Widget/SplitterColWidget.h"
#include "Widget/SplitterRowWidget.h"


#include <vector>
#include <set>
using namespace std;

typedef struct _STICKLINE
{
	float fValue;
	INT nOffset;
}STICKLINE,*LPSTICKLINE;

/*
typedef struct _MAKEHISTORY
{
	vector<CTime>  tVER;	//除权日
	set<CTime>  tVLHB;		//龙虎榜
	set<CTime>  tGGCGBD;	//高管持股变动
	set<CTime>  tCWJB;		//财务季报
}MAKEHISTORY,*LPMAKEHISTORY;
*/

/*
typedef struct _MAKEHISTORY
{	
	time_t			tLHB[100];		//龙虎榜	
	time_t		    tGGCGBD[100];	//高管持股变动
	time_t			tCWJB[100];		//财务季报	
}MAKEHISTORY,*LPMAKEHISTORY;
*/





class CKLineWidget : public CStatic
{
// Construction
public:
	CKLineWidget();
	void SetLabel(const char * szLabel);
private:
	void	InitMacd();
	void	InitKRMacd();
	void	InitKLine();
	void    InitKLineMA();
	void    InitVolumeMA();

	INT		GetTextWidth(CDC * pDC,LPCTSTR lpszText="你");
	INT		GetTextHeight(CDC * pDC,LPCTSTR lpszText="好");

	const char *	FormatPeriod(DWORD dwPreiod);
	void			FormatTime(CTime t , CString & szText);
	const char *	FormatFuQuan(INT nFuQuan);

	void	AdjustRC(CRect & rc);
	INT		PriceToY(float fValue);
	float   YToPrice(INT Y);

	INT		MakeYCoord(CRect & rc,double fValue , double fHH,double fLL);
	INT     OffsetToX(INT nOffset);
	INT     OffsetToX(LPKLINEDATA  pOffset);
	INT		IndexToX(INT nIndex);
	INT		XToIndex(INT X);
	INT     GetBeginIndex();
	INT     GetEndIndex();
	INT		GetUsableDrawLineCount();
	void    KLineTextOut(CDC * pDc,int x, int y,LPCTSTR lpszText,COLORREF crColor );
	void    MacdTextOut(CDC * pDc,int x, int y, int z,const CString & str,COLORREF crColor);
	void    KRMacdTextOut(CDC * pDc,int x, int y, int z,const CString & str,COLORREF crColor);
	void    DrawGap(CDC* pDC);
	void    DrawTitle(CDC * pDc,INT nIndex);
	void    DrawVolTitle(CDC * pDc,INT nIndex);
	void    DrawYCoord(CDC * pDc);
	void	DrawAllRC(CDC * pDc);
	void	DrawVol(CDC * pDc);
	void    DrawMacd(CDC * pDc);
	void    DrawMacdTitle(CDC * pDc,INT nIndex);
	void    DrawKRMacd(CDC * pDc);
	void	DrawKRMacdlTitle(CDC * pDc,INT nIndex);
	void	DrawKLine(CDC * pDc);

	void    DrawKLineMakeHistory(CDC * pDC);


	void	DrawMALine(CDC * pDc);
	void	DrawMALine(CDC * pDC,vector<float> & MA, COLORREF color);

	void	DrawVolumeMALine(CDC * pDc);
	void	DrawVolumeMALine(CDC * pDC,vector<float> & MA, COLORREF color);

	void    DrawWave(CDC * pDc);
	void	DrawCross(CPoint & point);
	void    SafeAdjustBegin(INT nJust);
	void    SafeAdjustEnd(INT nJust);
// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKLineWidget)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKLineWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKLineWidget)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnRButtonDown( UINT, CPoint );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnKRSDE_KLine_Remote(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKRSDE_RealTimeQuotes_Remote(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMyCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CFont m_Font;
	
	CRect m_RCYInfo;
	CRect m_RCTopInfo;
	CRect m_RCBottomInfo;
	CRect m_RCKLine;
	CRect m_RCVol;
	CRect m_RCKRMacd;
	CRect m_RCCommon;
	CRect m_RC;

	INT m_nKLineWidth;
	float m_fHH;
	float m_fLL;

	vector<KLINEDATA> m_VKLine;
	KLINEDATA * m_pKLineBegin;
	KLINEDATA * m_pKLineEnd;
	
	INT  m_nSTextHeight;
	INT  m_nSTextWidth;

	BOOL m_bShowCross;
	INT  m_nCrossIndex;

	REALTIMEQUOTES    m_RTQ;
	CHQFunction       m_HQFunction;
private:
	vector<float> m_DIFF;
	vector<float> m_MACD;
	vector<float> m_DEA;

	float * m_fMA10;
	float * m_fMA5;
	vector<float>		m_KRMACD;
	vector<WAVEDATA>	m_VKRWave;
	DWORD m_dwPeriod;
	INT   m_nFuQuan;
	char  m_szLabel[30];

	CMenu m_KLineMenu;
	CMenu m_KLineMenu_Sub;
	CMenu m_KLineMenu_Period;
	CMenu m_KLineMenu_MA;

	CMenu m_VolMenu;
	CMenu m_VolMenu_Sub;
	CMenu m_VolMenu_MA;

	CMenu m_FuQuanMenu;
	CMenu m_FuQuanMenu_Sub;
	CMenu m_FuQuanMenu_Context;

	vector<float> m_MA5,m_MA10,m_MA20,m_MA30,m_MA60,m_MA120,m_MA250;
	vector<float> m_VolMA5,m_VolMA10,m_VolMA20,m_VolMA30,m_VolMA60,m_VolMA120,m_VolMA250;

	CSplitterRowWidget m_SplitterRowWidget;
	CSplitterRowWidget m_SplitterRowSubWidget;
	CSplitterRowWidget m_SplitterRowSubSubWidget;
private:
	set<time_t>  m_SER;
	set<time_t>  m_STP;
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KLINEWIDGET_H__40B50BFB_CE6B_4F39_B029_09FE4176E4CB__INCLUDED_)
