// BootDlg.cpp : implementation file
//

#include "stdafx.h"
#include "krstockos.h"
#include "BootDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBootDlg dialog


CBootDlg::CBootDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBootDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBootDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBootDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_IE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBootDlg, CDialog)
	//{{AFX_MSG_MAP(CBootDlg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBootDlg message handlers

static BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * & pImg)  
{  
    HINSTANCE hInst = GetModuleHandle(NULL);  
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


DWORD WINAPI ThreadProc(LPVOID lpVoid)
{

	CBootDlg* pDlg = (CBootDlg*)lpVoid;

	CString szURL;
	szURL.Format("http://music.163.com/outchain/player?type=2&id=31654455&auto=1&height=66");
	VARIANT   varStr; 
	varStr.vt   =   VT_BSTR; 
	varStr.bstrVal   =   szURL.AllocSysString();   
	COleVariant   var; 
	pDlg->m_IE.Navigate2(&varStr,&var,&var,&var,&var);


	return 0;
}



BOOL CBootDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CreateThread(NULL,0,ThreadProc,this,0,NULL);



	CString szTitle;
	szTitle.LoadString(IDS_TITLE);
	::SetWindowText(m_hWnd,szTitle);


	MoveWindow(0,0,m_pImg->GetWidth(),m_pImg->GetHeight() + GetSystemMetrics(SM_CYCAPTION)  );


	m_IE.MoveWindow(0,m_pImg->GetHeight() + GetSystemMetrics(SM_CYCAPTION),m_pImg->GetWidth(),120);
	

	CenterWindow();
	SetForegroundWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



#define ID_TIMER	1
#define STRMAXLEN	25                              
#define STRMINLEN	8                             

typedef struct tagCharChain                             
{
	struct tagCharChain	*prev;                 
	TCHAR			ch;                     
	struct tagCharChain	*next;
}CharChain, *pCharChain;


typedef struct tagCharColumn
{
	CharChain	*head, *current, *point;
	int		x, y, iStrLen;                 
	int		iStopTimes, iMustStopTimes;     
}CharColumn, *pCharColumn;


TCHAR inline randomChar(){
	return( (TCHAR) (rand() % (126 - 33) + 33) );                                                 
}


int init( CharColumn *cc, int cyScreen, int x ) 
{
	int j;
	cc->iStrLen		= rand() % (STRMAXLEN - STRMINLEN) + STRMINLEN; 
	cc->x			= x + 3;
	cc->y			= rand() % 3 ?  rand() % cyScreen : 0;
	cc->iMustStopTimes	= rand() % 6;
	cc->iStopTimes		= 0;
	cc->head		= cc->current =
		(pCharChain) calloc( cc->iStrLen, sizeof(CharChain) );
	for ( j = 0; j < cc->iStrLen - 1; j++ )
	{
		cc->current->prev	= cc->point;
		cc->current->ch		= ' ';
		cc->current->next	= cc->current + 1; 
		cc->point		= cc->current++;
	}
	cc->current->prev	= cc->point;
	cc->current->ch		= ' ';
	cc->current->next	= cc->head;
	cc->head->prev = cc->current;
	cc->current	= cc->point = cc->head;
	cc->head->ch	= randomChar();
	return(0);
}


static HBITMAP		hBitmap;                                  
static int			iFontWidth = 10, iFontHeight = 15, iColumnCount; 
static CharColumn	*ccChain;
static HDC			hdcMem;


int CBootDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
		// TODO: Add your specialized creation code here
	
	HDC hdc;
	HWND hwnd = m_hWnd;

	HFONT				hFont;
	ImageFromIDResource(IDR_BOOTBG,"png",(Image * &)m_pImg);
	int			cxScreen, cyScreen;  
	cxScreen	= m_pImg->GetWidth();             
	cyScreen	= m_pImg->GetHeight() + GetSystemMetrics(SM_CYCAPTION); 


	::SetTimer( hwnd, ID_TIMER, 10, NULL );
	
	hdc	= ::GetDC( hwnd );
	hdcMem	= ::CreateCompatibleDC( hdc );
	hBitmap = ::CreateCompatibleBitmap( hdc, cxScreen, cyScreen );
	::SelectObject( hdcMem, hBitmap );
	::ReleaseDC( hwnd, hdc );
	

	hFont = ::CreateFont( iFontHeight, iFontWidth - 5, 0, 0, 
		FW_BOLD, 0, 0, 0,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,DRAFT_QUALITY, FIXED_PITCH | FF_SWISS, TEXT( "System" ) );
	
	::SelectObject( hdcMem, hFont );
	::DeleteObject( hFont );
	::SetBkMode( hdcMem, TRANSPARENT );               
	iColumnCount = cxScreen / (iFontWidth * 3 / 2); 
	
	ccChain = (pCharColumn) calloc( iColumnCount, sizeof(CharColumn) );
	for ( INT i = 0; i < iColumnCount; i++ )
		init( ccChain + i, cyScreen, (iFontWidth * 3 / 2) * i );
	
	return 0;
}



void CBootDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == ID_TIMER)
	{
		HDC hdc;
		HWND hwnd = m_hWnd;
		hdc = ::GetDC( hwnd );

		RECT rc;
		::GetWindowRect(hwnd,&rc);
		int			cxScreen, cyScreen;  
		cxScreen	= rc.right - rc.left;             
		cyScreen	= rc.bottom - rc.top; 

		::PatBlt( hdcMem, 0, 0, cxScreen, cyScreen, BLACKNESS );  
		
		//GDI+ 效率真感人
		Graphics raphics( hdcMem ); 			
		raphics.DrawImage(m_pImg,0,0,800,600);
		
		int			i, j, temp, ctn;  
		for ( i = 0; i < iColumnCount; i++ )
		{
			ctn = (ccChain + i)->iStopTimes++ > (ccChain + i)->iMustStopTimes;
			(ccChain + i)->point = (ccChain + i)->head; 
			
			::SetTextColor( hdcMem, RGB( 255, 255, 255 ) );
			::TextOut( hdcMem, (ccChain + i)->x, (ccChain + i)->y, &( (ccChain + i)->point->ch), 1 );
			j			= (ccChain + i)->y;
			(ccChain + i)->point	= (ccChain + i)->point->next;
			
			temp = 0; 
			while ( (ccChain + i)->point != (ccChain + i)->head && (ccChain + i)->point->ch )
			{
				SetTextColor( hdcMem, RGB( 0, 255 - (255 * (temp++) / (ccChain + i)->iStrLen), 0 ) );
				TextOut( hdcMem, (ccChain + i)->x, j -= iFontHeight, &( (ccChain + i)->point->ch), 1 );
				(ccChain + i)->point = (ccChain + i)->point->next;
			}
			
			if ( ctn ) (ccChain + i)->iStopTimes = 0;
			else continue;
			
			(ccChain + i)->y += iFontHeight; 
		
			if ( (ccChain + i)->y - (ccChain + i)->iStrLen * iFontHeight > cyScreen )
			{
				free( (ccChain + i)->current );
				init( ccChain + i, cyScreen, (iFontWidth * 3 / 2) * i );
			}
			(ccChain + i)->head	= (ccChain + i)->head->prev;
			(ccChain + i)->head->ch = randomChar();
		}
		::BitBlt( hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY );
		::ReleaseDC( hwnd, hdc );
	}
	CDialog::OnTimer(nIDEvent);
}

void CBootDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	HWND hwnd = m_hWnd;
	::KillTimer( hwnd, ID_TIMER );
	::DeleteObject( hBitmap );
	::DeleteDC( hdcMem );

	for (INT i = 0; i < iColumnCount; i++ )
		free( (ccChain + i)->current );
	
	free( ccChain );
	delete m_pImg;
	CDialog::OnClose();
}

