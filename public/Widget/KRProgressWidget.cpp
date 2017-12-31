// KRProgressWidget.cpp : implementation file
//

#include "stdafx.h"
#include "KRProgressWidget.h"
#include "MemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKRProgressWidget

CKRProgressWidget::CKRProgressWidget()
{
//	m_TitleText=L"这是大字显示的标题";
//	m_InfoText =L"这是小字显示的信息";
 	m_nRangeLower = 0;
 	m_nRangeUpper = 0;
 	m_nPos        = 0;

	m_BKR = 168;
	m_BKG = 0;
	m_BKB = 157;
	m_bShowWindow = TRUE;
}

CKRProgressWidget::~CKRProgressWidget()
{

}


BEGIN_MESSAGE_MAP(CKRProgressWidget, CStatic)
	//{{AFX_MSG_MAP(CKRProgressWidget)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKRProgressWidget message handlers



BOOL CKRProgressWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}


void CKRProgressWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_nRangeLower == 0 && m_nRangeUpper == 0)
		return;

	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	
	if (MemDC.IsOK())
	{
 		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );

 		SolidBrush SBBK(Color(m_BKR,m_BKG,m_BKB));
 		raphics.FillRectangle(&SBBK,0,0,rc.Width(),rc.Height());
		raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		raphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);


		//输出标题栏 居中对其
		if(m_TitleText.length())
		{
			FontFamily TitleFontFamily(L"Arial");
			Font TitleFont(&TitleFontFamily, 15);
			StringFormat TitleFormat;                         
			TitleFormat.SetAlignment(StringAlignmentCenter);
			TitleFormat.SetLineAlignment(StringAlignmentNear); 
			
			SolidBrush TitleBrush(Color(255, 255, 255));

			RectF TitleBound;
			raphics.MeasureString(m_TitleText.c_str(),m_TitleText.length(),&TitleFont,	RectF(0,0,(float)rc.Width(),(float)rc.Height()),&TitleBound);

			raphics.DrawString(m_TitleText.c_str(),
				m_TitleText.length(),
				&TitleFont,
				RectF(0,TitleBound.Height / 4,(float)rc.Width(),(float)rc.Height()),
				&TitleFormat,
				&TitleBrush);	
		}


		{
			FontFamily NumFontFamily(L"Arial");
			Font NumFont(&NumFontFamily, 40);
			
			StringFormat NumFormat;                         
			NumFormat.SetAlignment(StringAlignmentCenter);
			NumFormat.SetLineAlignment(StringAlignmentCenter); 
			
			//计算比例 
			float fCurPos = (float)m_nPos / (float)(m_nRangeUpper - m_nRangeLower);
			WCHAR szPos[5];
			wsprintfW(szPos,L"%02d%c",(INT)(fCurPos * 100),'%');

			SolidBrush NumBrush(Color(255, 255, 255));
			raphics.DrawString(	szPos,
				wcslen(szPos),
				&NumFont,
				RectF(0,0,(float)rc.Width(),(float)rc.Height()),
				&NumFormat,
				&NumBrush);
			
			RectF NumBound;
			raphics.MeasureString(szPos,wcslen(szPos),&NumFont,	RectF(0,0,(float)rc.Width(),(float)rc.Height()),&NumBound);
			
			Pen penEllipse(Color(100,255,255,255),4);
			raphics.DrawEllipse(&penEllipse, 
				(INT)(rc.Width() - NumBound.Width) / 2 ,
				(INT)(rc.Height() - NumBound.Height) / 2 - (INT)(NumBound.Width - NumBound.Height) / 2, 
				(INT)NumBound.Width,
				(INT)NumBound.Width);
			
			Pen penArc(Color(255,255,255,255),4);
			raphics.DrawArc(&penArc, 			
				(INT)(rc.Width() - NumBound.Width) / 2 ,
				(INT)(rc.Height() - NumBound.Height) / 2 -  (INT)(NumBound.Width - NumBound.Height) / 2, 
				(INT)NumBound.Width,
				(INT)NumBound.Width,
				0,  fCurPos * 360  );
			
		}

		//输出信息栏
		if(m_InfoText.length())
		{
			FontFamily InfoFontFamily(L"Arial");
			Font InfoFont(&InfoFontFamily, 10);
			StringFormat InfoFormat;                         
			InfoFormat.SetAlignment(StringAlignmentCenter);
			InfoFormat.SetLineAlignment(StringAlignmentFar); 

			SolidBrush InfoBrush(Color(255, 255, 255));
			RectF InfoBound;
			raphics.MeasureString(m_InfoText.c_str(),m_InfoText.length(),&InfoFont,RectF(0,0,(float)rc.Width(),(float)rc.Height()),&InfoBound);
			raphics.DrawString( m_InfoText.c_str(),
								m_InfoText.length(),
								&InfoFont,
								RectF(0,0,(float)rc.Width(),(float)rc.Height() - InfoBound.Height),
								&InfoFormat,
								&InfoBrush);	
		}
	}
	// Do not call CWnd::OnPaint() for painting messages
}



void CKRProgressWidget::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_AUTO){
		SetPos(GetPos()+1);
	}
	if(nIDEvent == TIMER_SPEED){
		SetPos(GetPos() + m_dwSpeedBase);
		if(GetPos() >= m_dwSpeedTarget)
			KillTimer(nIDEvent);
	}
	CWnd::OnTimer(nIDEvent);
}
