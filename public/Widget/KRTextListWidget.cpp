// KRTextListWidget.cpp : implementation file
//

#include "stdafx.h"
#include "krtextlistwidget.h"
#include "MemoryDC.h"
#include "Common/Common.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKRTextListWidget





CKRTextListWidget::CKRTextListWidget()
{
	m_BKR = 168;
	m_BKG = 0;
	m_BKB = 157;
	m_VTextIndex = 0;
	m_KRScrollVBar.SetCWnd(this);
}



CKRTextListWidget::~CKRTextListWidget()
{

}


BEGIN_MESSAGE_MAP(CKRTextListWidget, CStatic)
	//{{AFX_MSG_MAP(CKRTextListWidget)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SCROLLCHANGE,OnScrollChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKRTextListWidget message handlers


void CKRTextListWidget::SetBkColor(INT R,INT G,INT B)
{
	m_BKR = R;
	m_BKG = G;
	m_BKB = B;
	if(GetSafeHwnd()) 
		RedrawWindow();
}

void CKRTextListWidget::AddString(const char * szText)
{
	CTRString TRString;
	AddString(TRString.ANSIToUnicode(szText));
}


void CKRTextListWidget::AddString(const wchar_t * szText)
{
	m_VText.push_back(szText);
	if(GetSafeHwnd())
		RedrawWindow();
}


void CKRTextListWidget::InsertString(INT nIndex,const char * szText)
{
	CTRString TRString;
	TRString.ANSIToUnicode(szText);
	InsertString(nIndex,(const wchar_t *)TRString.ANSIToUnicode(szText));
}

void CKRTextListWidget::InsertString(INT nIndex,const wchar_t * szText)
{
	m_VText.insert(&m_VText[nIndex],szText);
	if(GetSafeHwnd()) 
		RedrawWindow();
}


INT CKRTextListWidget::GetCount()
{
	return m_VText.size();
}


BOOL CKRTextListWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return  TRUE;
}


void CKRTextListWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{
		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );

		SolidBrush SBBK(Color(m_BKR,m_BKG,m_BKB));
		raphics.FillRectangle(&SBBK,0,0,rc.Width(),rc.Height());
		if(m_VText.size())
		{
			raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
			raphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

			FontFamily TextFontFamily(L"Arial");
			Font TextFont(&TextFontFamily, 9);
			StringFormat TextFormat;                         
			TextFormat.SetAlignment(StringAlignmentNear);
			TextFormat.SetTrimming(StringTrimmingEllipsisPath);
			SolidBrush TextBrush(Color(255, 255, 255));

			wchar_t * pMeasureText = L"≤‚ ‘ABCabc123!@#$%^";
			RectF StringBound;
			raphics.MeasureString(pMeasureText,wcslen(pMeasureText),&TextFont,RectF(0,0,rc.Width(),rc.Height()),&StringBound);
			INT nRowCount = (float)rc.Height() / (float)StringBound.Height;

			INT nDrawStringCount = 0;
			for (INT i= m_VTextIndex; i< m_VText.size(); i++)
			{
				RectF TextRc(0,StringBound.Height * nDrawStringCount , rc.Width() - m_KRScrollVBar.GetWidth(),StringBound.Height);
				raphics.DrawString(	m_VText[i].c_str(),m_VText[i].length(),&TextFont,TextRc,&TextFormat,&TextBrush);

				if(nDrawStringCount > nRowCount)
					break;

				nDrawStringCount++;
			}

			m_KRScrollVBar.SetRange(0,m_VText.size() - nRowCount,FALSE);

			if(m_VText.size() > nRowCount)
				m_KRScrollVBar.ShowWindow(SW_SHOW);
			else
				m_KRScrollVBar.ShowWindow(SW_HIDE);
			
			m_KRScrollVBar.OnPaint(raphics,rc);
		}
	}
	// Do not call CWnd::OnPaint() for painting messages
}



LRESULT CKRTextListWidget::OnScrollChange(LPARAM lParam,WPARAM wParam)
{
	m_VTextIndex = m_KRScrollVBar.GetPos();
	RedrawWindow();
	return TRUE;
}

BOOL CKRTextListWidget::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_KRScrollVBar.PreTranslateMessage(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}
