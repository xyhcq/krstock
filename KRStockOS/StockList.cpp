// StockList.cpp : implementation file
//

#include "stdafx.h"
#include "krstockos.h"
#include "StockList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStockList




CStockList::CStockList()
{

}

CStockList::~CStockList()
{
}


BEGIN_MESSAGE_MAP(CStockList, CListCtrl)
	//{{AFX_MSG_MAP(CStockList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStockList message handlers
void CStockList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//	static BOOL s_bThisItemSelect = FALSE;
    // TODO: Add your control notification handler code here
    *pResult = CDRF_DODEFAULT;
    NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
    NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
    switch(lplvdr->nmcd.dwDrawStage)
    {
	case CDDS_PREPAINT:
        { 
            *pResult = CDRF_NOTIFYITEMDRAW;
            break;
        }
	case CDDS_ITEMPREPAINT:
        {
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
        }
		break;
	case CDDS_SUBITEM |CDDS_ITEMPREPAINT:
		{
			DWORD dwData = GetItemData(nmcd.dwItemSpec);
			map<DWORD,ITEMCOLOR>::iterator  it = m_ItemColor.find(dwData); 
			if(it != m_ItemColor.end())
			{
				if (nmcd.uItemState & CDIS_SELECTED)
				{
					lplvdr->clrText = it->second.colText[lplvdr->iSubItem];
					lplvdr->clrTextBk = RGB(64,0,129);
					lplvdr->nmcd.uItemState ^= CDIS_SELECTED;
				}else
				{
					lplvdr->clrText = it->second.colText[lplvdr->iSubItem];
					lplvdr->clrTextBk = it->second.colTextBk[lplvdr->iSubItem];
				}
			}
			*pResult = CDRF_DODEFAULT;
		}
        break;	
    }
}



int CStockList::InsertColumn(	 int nCol,
								 const LVCOLUMN* pColumn,
								 COL_TYPE ColType/*该列的数据类型*/,
								 BOOL bAutoSize/*是否自动调整列的大小*/)
{
	INT nResult = CListCtrl::InsertColumn(nCol,pColumn);
	m_ColType.insert(_SLPAIR(nCol,ColType));

	if(bAutoSize) 
	{
		CClientDC dc(this);
		CSize sz = dc.GetTextExtent(pColumn->pszText);
		SetColumnWidth(nCol,sz.cx+20);
	}
	return nResult;
}




int CStockList::InsertColumn(int nCol, 
								 LPCTSTR lpszColumnHeading, 
								 COL_TYPE ColType/*该列的数据类型 默认为字符串*/,
								 int nFormat,/*对齐方式默认是 左对齐*/
								 int nWidth,
								 int nSubItem ,
								 BOOL bAutoSize/*是否自动调整列的大小 默认为自动*/)
{

	INT nResult = CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
	m_ColType.insert(_SLPAIR(nCol,ColType));

	if(bAutoSize) 
	{
		CClientDC dc(this);
		CSize sz = dc.GetTextExtent(lpszColumnHeading);
		SetColumnWidth(nCol,sz.cx+20);
	}
	return nResult;
}




int CStockList::InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize)
{
	for (DWORD i = 0; i < dwSize;i++)
	{
		InsertColumn(lpColumnData[i].nCol,lpColumnData[i].szText,lpColumnData[i].ColType,
			lpColumnData[i].nFormat,lpColumnData[i].nLenght,-1,lpColumnData[i].bAutoSize);
	}
	return 1;
}



BOOL CStockList::SetItemTextEx( int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText,COLORREF colorBK )
{

	return CListCtrl::SetItemText(nItem,nSubItem,lpszText);
}




BOOL CStockList::CreateEx( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID )
{
	BOOL bResult = CListCtrl::Create(dwStyle,rect,pParentWnd,nID);
	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	if (pHeaderCtrl!=NULL)
		m_MyHeader.SubclassWindow(pHeaderCtrl->GetSafeHwnd());

	return bResult;
}





#include "Widget/MemoryDC.h"

IMPLEMENT_DYNAMIC(CMyHeaderCtrl, CHeaderCtrl)

CMyHeaderCtrl::CMyHeaderCtrl(){
	
}

CMyHeaderCtrl::~CMyHeaderCtrl(){
	
}

BEGIN_MESSAGE_MAP(CMyHeaderCtrl, CHeaderCtrl)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CMyHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
    return TRUE;
}

void CMyHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{
		CDC * pDc = MemDC.GetMemoryDC();
		CRect rcItem;
		int nItems = GetItemCount();
		for (int i = 0; i < nItems; ++i)
		{
			TCHAR szText[256];
			HDITEM hditem;
			hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
			hditem.pszText = szText;
			hditem.cchTextMax = 255;
			GetItem(i, &hditem);
			
			GetItemRect(i, &rcItem);
			if (rcItem.IsRectEmpty())
				continue;
			
			pDc->SetTextColor(RGB(255,255,255));
			pDc->SetBkMode(TRANSPARENT);
			
			UINT nFormat = HDF_LEFT;
			if (hditem.fmt & HDF_RIGHT)
				nFormat = DT_RIGHT;
			if (hditem.fmt & HDF_CENTER)
				nFormat = DT_CENTER;
			
			pDc->DrawText(szText,rcItem,nFormat);
		}
	}
}
