// StockToolsWiget.cpp : implementation file
//

#include "stdafx.h"
#include "StockToolsWiget.h"
#include "Widget/MemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStockToolsWiget

CStockToolsWiget::CStockToolsWiget()
{

}


CStockToolsWiget::~CStockToolsWiget()
{

}


BEGIN_MESSAGE_MAP(CStockToolsWiget, CStatic)
	//{{AFX_MSG_MAP(CStockToolsWiget)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TEXTBUTTON,OnTextButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStockToolsWiget message handlers
enum{ID_TB_XWGG,ID_TB_GNTC,ID_TB_JYFX, ID_TB_LHB,ID_TB_DZJY,ID_TB_RZRQ,ID_TB_GDYJ,ID_TB_GBJG ,ID_TB_ZLCC ,ID_TB_CWGK,ID_TB_GSZL};



void CStockToolsWiget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK())
	{

		//��䱳��ɫ
		CDC * pDc = MemDC.GetMemoryDC();
		CBrush bgbs(  RGB(KLWIDGET_BGCOLOR_RED,KLWIDGET_BGCOLOR_GREEN,KLWIDGET_BGCOLOR_BLUE)  );
		pDc->FillRect(rc,&bgbs);
		pDc->SelectObject(m_Font);
		pDc->SetBkMode(TRANSPARENT);
		
		CBrush RedBrush(COLOR_RED );
		pDc->FrameRect(rc,&RedBrush);

		INT nYBase  = rc.top + 4;
		INT nXBase  = 4;
		m_TextButton.Add("���Ź���",ID_TB_XWGG,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("���Ź���").cx + 10;

		m_TextButton.Add("�������",ID_TB_GNTC,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("�������").cx + 10;

		m_TextButton.Add("��Ӫ����",ID_TB_JYFX,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("��Ӫ����").cx + 10;

		m_TextButton.Add("������",ID_TB_LHB,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("������").cx + 10;

		m_TextButton.Add("���ڽ���",ID_TB_DZJY,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("���ڽ���").cx + 10;

		m_TextButton.Add("������ȯ",ID_TB_RZRQ,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("������ȯ").cx + 10;

		m_TextButton.Add("�ɶ��о�",ID_TB_GDYJ,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("�ɶ��о�").cx + 10;

		m_TextButton.Add("�ɱ��ṹ",ID_TB_GBJG,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("�ɱ��ṹ").cx + 10;

		m_TextButton.Add("�����ֲ�",ID_TB_ZLCC,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("�����ֲ�").cx +10;
		
		m_TextButton.Add("����ſ�",ID_TB_CWGK,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("����ſ�").cx +10;

		m_TextButton.Add("��˾����",ID_TB_GSZL,nXBase,nYBase,pDc);
		nXBase += pDc->GetTextExtent("��˾����").cx +10;

		CBrush RectBrush(RGB(TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE));
		m_TextButton.ReDraw(pDc,&RectBrush,RGB(TEXTCOLOR_RED,TEXTCOLOR_GREEN,TEXTCOLOR_BLUE),RGB(255,0,0));
	}
	// Do not call CStatic::OnPaint() for painting messages
}



BOOL CStockToolsWiget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


int CStockToolsWiget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_Font.CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("����"));

	
	m_TextButton.SetMessage(WM_TEXTBUTTON);
	m_TextButton.SetIndex(0);
	m_TextButton.SetFiexdIndex();
	
	return 0;
}

void CStockToolsWiget::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_TextButton.OnLButtonDown(this,nFlags,point))
		return;
	CStatic::OnLButtonDown(nFlags, point);
}


LRESULT CStockToolsWiget::OnTextButton( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case ID_TB_XWGG:
	case ID_TB_GNTC:
	case ID_TB_JYFX:
	case ID_TB_LHB:
	case ID_TB_DZJY:
	case ID_TB_RZRQ:
	case ID_TB_GDYJ:
	case ID_TB_GBJG :
	case ID_TB_ZLCC :
	case ID_TB_CWGK:
	case ID_TB_GSZL:
		{
			MessageBox("���ڿ��ţ�");
		}break;
	}

	return TRUE;
}

void CStockToolsWiget::SetLabel( const char * szLabel )
{
	m_szLabel = szLabel;
}
