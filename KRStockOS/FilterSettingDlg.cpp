// FilterSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "krstockos.h"
#include "FilterSettingDlg.h"
#include "Widget/MemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterSettingDlg dialog
#define		TITLE_HEIGHT		20	
#define		KRFILTER_TITLE		"ÁÔ¾¨¿ñÈË- Ô¤¾¯Æ÷ÉèÖÃ"

CFilterSettingDlg::CFilterSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFilterSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CLOSE,	   m_BtnClose);
}


BEGIN_MESSAGE_MAP(CFilterSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterSettingDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterSettingDlg message handlers
BOOL CFilterSettingDlg::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * & pImg)  
{  
    HINSTANCE hInst = AfxGetResourceHandle();  
    HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type  
    if (!hRsrc)  
        return FALSE;  
    // load resource into memory  
    DWORD len = SizeofResource(hInst, hRsrc);  
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
    if (!lpRsrc)  
        return FALSE;  
    // Allocate global memory on which to create stream  
    HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
    BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
    memcpy(pmem,lpRsrc,len);  
    IStream* pstm;  
    CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
    // load from stream  
    pImg=Gdiplus::Image::FromStream(pstm);  
    // free/release stuff  
    GlobalUnlock(m_hMem);  
    pstm->Release();  
    FreeResource(lpRsrc);  
    return TRUE;  
}

BOOL CFilterSettingDlg::DrawImage(UINT nID, LPCTSTR sTR,Graphics & raphics , INT x, INT y , INT & nImageWidth, INT & nImageHeight)
{
	Image * pImgPng;
	ImageFromIDResource(nID,sTR,(Image * &)pImgPng);
	if(pImgPng == NULL)
		return FALSE;

	raphics.DrawImage(pImgPng,x,y);
	nImageWidth = pImgPng->GetWidth();
	nImageHeight = pImgPng->GetHeight();
	return TRUE;
}


void CFilterSettingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{

		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );
		SolidBrush SBBK(Color(255,255,255));
		raphics.FillRectangle(&SBBK,0,0,rc.Width(),rc.Height());
		
		CDC * pDc = MemDC.GetMemoryDC();
		m_RCTitle= rc;
		m_RCTitle.bottom=  TITLE_HEIGHT;
		CBrush TitleBrush(RGB(255,0,0));
		pDc->FillRect(m_RCTitle,&TitleBrush);
		pDc->SetBkMode(TRANSPARENT);
		HICON hIcon  = (HICON)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON, 16,16,0);
		::DrawIconEx(pDc->GetSafeHdc(), 3, 3, hIcon, 16, 16, 0, NULL, DI_NORMAL | DI_COMPAT);
		pDc->SelectObject(m_Font);
		pDc->SetTextColor(RGB( TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE) );
		pDc->TextOut(24, 4 , KRFILTER_TITLE);


		INT nXBase = 8 , nYBase = TITLE_HEIGHT + 8 ,nImageWidth=0,  nImageHeight=0;
		DrawImage(IDR_OneBuy0,"png",raphics,nXBase,nYBase,nImageWidth,nImageHeight);

		nXBase += nImageWidth + 20;
		DrawImage(IDR_OneBuy1,"png",raphics,nXBase,nYBase,nImageWidth,nImageHeight);


		nYBase += nImageHeight + 30;
		nXBase = 8;
		DrawImage(IDR_OneBuy2 ,"png",raphics,nXBase,nYBase,nImageWidth,nImageHeight);
		nXBase += nImageWidth + 20;
		DrawImage(IDR_5WaveDown,"png",raphics,nXBase,nYBase,nImageWidth,nImageHeight);


		nYBase += nImageHeight + 30;
		nXBase = 8;
		DrawImage(IDR_OneBuy3,"png",raphics,nXBase,nYBase,nImageWidth,nImageHeight);
		nXBase += nImageWidth + 20;
		DrawImage(IDR_OneBuy4,"png",raphics,nXBase,nYBase,nImageWidth,nImageHeight);

	}
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CFilterSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BtnClose.SetStyle(KRSYSBTN_CLOSEBOX);
	m_BtnClose.SetHotColor(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE);
	m_BtnClose.SetBkColor(255,0,0);
	
	m_Font.CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("ËÎÌå"));
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	m_BKBrush.CreateSolidBrush(RGB(255,255,255));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CFilterSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));
	// TODO: Return a different brush if the default is not desired
	return m_BKBrush;
	//return hbr;
}

BOOL CFilterSettingDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CFilterSettingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags == MK_LBUTTON && m_RCTitle.PtInRect(point))
 		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

UINT CFilterSettingDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
    GetWindowRect(&rc);
	return rc.PtInRect(point)?HTCAPTION: CDialog::OnNcHitTest(point);
}
