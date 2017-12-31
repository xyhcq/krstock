// MSortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MSortListCtrl.h"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <strsafe.h>
#include "Widget/MemoryDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define HDF_SORTDOWN		0x0200
#define HDF_SORTUP			0x0400
#define HDS_FLAT            0x0200


IMPLEMENT_DYNAMIC(CKRHeaderCtrl, CHeaderCtrl)

CKRHeaderCtrl::CKRHeaderCtrl(){
	
}

CKRHeaderCtrl::~CKRHeaderCtrl(){
	
}

BEGIN_MESSAGE_MAP(CKRHeaderCtrl, CHeaderCtrl)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CKRHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
    return TRUE;
}


void CKRHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetClientRect(rc);
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK() )
	{
		CDC * pDc = MemDC.GetMemoryDC();

		CFont font;
		font.CreatePointFont(100,_T("Fixedsys"));
		pDc->SelectObject(font);

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

			if ((hditem.fmt & HDF_SORTUP) )
				StringCbCat(szText,sizeof(szText),"↓");

			if(hditem.fmt & HDF_SORTDOWN)
				StringCbCat(szText,sizeof(szText),"↑");

			pDc->DrawText(szText,rcItem,nFormat);
		}
	}
}



/////////////////////////////////////////////////////////////////////////////
// CMSortListCtrl

enum 
{
		SORT_NON,	 //当前尚未排序
		SORT_ASCEND, //升序
		SORT_DESCEND //降序
};

//2012-11-21 17:18:23
typedef struct DATA_TIME_FORMAT
{
	TCHAR szYear[5];	//2012-   -最终会被替换成结束标志
	TCHAR szMonth[3];	//11-   -最终会被替换成结束标志
	TCHAR szDay[3];		//21    空格最终会被替换成结束标志
	TCHAR szHour[3];	//17:	:最终会被替换成结束标志
	TCHAR szMinute[3]; //18:	:最终会被替换成结束标志
	TCHAR szSecond[3]; //23		最后一个字符会被替换成结束标志
}DATA_TIME_FORMAT,*LPDATA_TIME_FORMAT;

#define DATATIMEFORMAT(T) 		T.szYear[4]=(TCHAR)'\0';	\
								T.szMonth[2]=(TCHAR)'\0';	\
								T.szDay[2]=(TCHAR)'\0';		\
								T.szHour[2]=(TCHAR)'\0';	\
								T.szMinute[2]=(TCHAR)'\0';	\
								T.szSecond[2]=(TCHAR)'\0';	


CMSortListCtrl::CMSortListCtrl()
{
		m_bRedrawBkgnd    = TRUE;
		m_bLockEraseBkgnd = FALSE;
}

CMSortListCtrl::~CMSortListCtrl()
{

}


BEGIN_MESSAGE_MAP(CMSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMSortListCtrl message handlers


void CMSortListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
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
					lplvdr->clrTextBk = RGB(64,0,129);//GetSysColor(COLOR_HIGHLIGHT);
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




static double STOF(LPCTSTR s)
{
	double val,power;
	int i,sign;
	
	for(i=0;isspace(s[i]);i++);
	sign=(s[i]=='-')?-1:1;

	if(s[i]=='+'||s[i]=='-')
		i++;

	for(val=0.0;isdigit(s[i]);i++)
		val=10.0*val+(s[i]-'0');
	
	if(s[i]=='.')
		i++;
	
	for(power=1.0;isdigit(s[i]);i++)
	{
		val=10.0*val+(s[i]-'0');
		power*=10.0;
	}
	float f =  sign*val/power;
	if( StrStr(s,_T("万")) )
		f *= 10000;
	if( StrStr(s,_T("亿")) )
		f *= 100000000;
	return f;
}



static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMSortListCtrl * lpThis = (CMSortListCtrl *)lParamSort;
	if(lpThis->m_nCurCol == -1)
		return 0;

	CMMAP::iterator itMark,itType;
	itMark = lpThis->m_ColSortMark.find(lpThis->m_nCurCol);
	itType = lpThis->m_ColType.find(lpThis->m_nCurCol);
	if(itMark == lpThis->m_ColSortMark.end() || itType == lpThis->m_ColType.end())
		return 0;

	INT nItem1=-1,nItem2=-1;
	CString szItem1,szItem2;

	LVFINDINFO stFindInfo;
	stFindInfo.flags = LVFI_PARAM;
    stFindInfo.lParam = lParam1;
	nItem1 = lpThis->FindItem(&stFindInfo);

	stFindInfo.flags = LVFI_PARAM;
    stFindInfo.lParam = lParam2;
	nItem2 = lpThis->FindItem(&stFindInfo);

	szItem1 = lpThis->GetItemText(nItem1, lpThis->m_nCurCol);
	szItem2 = lpThis->GetItemText(nItem2, lpThis->m_nCurCol);

	INT nResult = 0;
	switch(itType->second)
	{
	case TYPE_FLOAT:
		if( (STOF(szItem1) == STOF(szItem2) ))
		{
			if(szItem1.GetLength() == szItem2.GetLength())
				nResult = 0;
			else
				nResult = (szItem1.GetLength() > szItem2.GetLength())?1:-1;
		}
		else
		{
			nResult = ( STOF(szItem1) > STOF(szItem2) )?1:-1;
		}
		break;
	case TYPE_INTGER:
		if((_ttoi(szItem1) == _ttoi(szItem2) ))
		{
			if(szItem1.GetLength() == szItem2.GetLength())
				nResult = 0;
			else
				nResult = (szItem1.GetLength() > szItem2.GetLength())?1:-1;
		}else
		{
			nResult = ( _ttoi(szItem1) > _ttoi(szItem2) )?1:-1;
		}
		break;
	case TYPE_STRING:
		nResult = StrCmpI(szItem1,szItem2);
		break;
	case TYPE_DATE:
		{
			DATA_TIME_FORMAT dt1,dt2;
			CopyMemory(&dt1,(LPCTSTR)szItem1,szItem1.GetLength() * sizeof(TCHAR));
			CopyMemory(&dt2,(LPCTSTR)szItem2,szItem2.GetLength() * sizeof(TCHAR));
			DATATIMEFORMAT(dt1);
			DATATIMEFORMAT(dt2);
			CTime t1(_ttoi(dt1.szYear),_ttoi(dt1.szMonth),_ttoi(dt1.szDay),
				_ttoi(dt1.szHour),_ttoi(dt1.szMinute),_ttoi(dt1.szSecond));
			
			CTime t2(_ttoi(dt2.szYear),_ttoi(dt2.szMonth),_ttoi(dt2.szDay),
				_ttoi(dt2.szHour),_ttoi(dt2.szMinute),_ttoi(dt2.szSecond));
			nResult = ( t1 == t2 )?0:(t1 > t2 )?1:-1;
		}
		break;
	}
	if(itMark->second == SORT_DESCEND && nResult)
		return (nResult==1)?-1:1;
	
	return nResult;
}



void CMSortListCtrl::SetHeaderSortImage(HWND listView, int columnIndex, BOOL isAscending)
{
    HWND header = ListView_GetHeader(listView);
    int columnCount = Header_GetItemCount(header);
    for (int i = 0; i<columnCount; i++)
    {
        HDITEM hi = {0};
        hi.mask = HDI_FORMAT;
        Header_GetItem(header, i, &hi);
        if(i == columnIndex)
        {
            hi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
            hi.fmt |= isAscending ? HDF_SORTUP : HDF_SORTDOWN;
        }
        else
        {
            hi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
        }
        Header_SetItem(header, i, &hi);
    }
}



void CMSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nCurCol = pNMListView->iSubItem;
	
	CMMAP::iterator it;
	it = m_ColSortMark.find(m_nCurCol);
	if(it  == m_ColSortMark.end())
		m_ColSortMark.insert(CMPAIR(m_nCurCol,SORT_NON));

	it = m_ColSortMark.find(m_nCurCol);
	
	//没有赋值
	switch(it->second)
	{
	case SORT_NON:
		it->second = SORT_ASCEND;	
		break;
	case SORT_ASCEND:
		it->second = SORT_DESCEND;
		break;
	case SORT_DESCEND:
		it->second = SORT_ASCEND;
		break;
	}

	SortItems(CompareProc,(DWORD)this);
	SetHeaderSortImage(GetSafeHwnd(), m_nCurCol,(it->second==SORT_ASCEND)?TRUE:FALSE);
	*pResult = 0;
}

BOOL CMSortListCtrl::SetItemTextEx( int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText)
{
	int nItemData = GetItemData(nItem);
	m_ItemColor[nItemData].colText[nSubItem] = colorText;
	return SetItemText(nItem,nSubItem,lpszText);
}

BOOL CMSortListCtrl::SetItemTextEx( int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText,COLORREF colorBK )
{
	int nItemData = GetItemData(nItem);
	m_ItemColor[nItemData].colText[nSubItem] = colorText;
	m_ItemColor[nItemData].colTextBk[nSubItem] = colorBK;
	return SetItemText(nItem,nSubItem,lpszText);
}

VOID CMSortListCtrl::SetSubItemColor( int nItem, int nSubItem,COLORREF colorText,COLORREF colorBK )
{
	int nItemData = GetItemData(nItem);
	m_ItemColor[nItemData].colText[nSubItem] = colorText;
	m_ItemColor[nItemData].colTextBk[nSubItem] = colorBK;
}

VOID CMSortListCtrl::SetAllItemBkColor(COLORREF colorBK)
{
	int nCount = m_Header.GetItemCount();
	map<DWORD,ITEMCOLOR>::iterator it = m_ItemColor.begin();
	for (it ; it != m_ItemColor.end(); ++it)
	{
		for(INT i =0; i< nCount; i++)
			it->second.colTextBk[i] =colorBK;
	}
}



BOOL CMSortListCtrl::CreateEx( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID )
{
	BOOL bOk = Create(dwStyle,rect,pParentWnd,nID);
	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	if (pHeaderCtrl!=NULL)
		m_Header.SubclassWindow(pHeaderCtrl->GetSafeHwnd());
	return bOk;
}





int CMSortListCtrl::InsertColumn( int nCol,
								 const LVCOLUMN* pColumn,
								 COL_TYPE ColType/*该列的数据类型*/,
								 BOOL bAutoSize/*是否自动调整列的大小*/)
{
	INT nResult = CListCtrl::InsertColumn(nCol,pColumn);
	m_ColType.insert(CMPAIR(nCol,ColType));
	if(bAutoSize) {
		CClientDC dc(this);
		CSize sz = dc.GetTextExtent(pColumn->pszText);
		SetColumnWidth(nCol,sz.cx+20);
	}
	return nResult;
}


int CMSortListCtrl::InsertColumn(int nCol, 
								 LPCTSTR lpszColumnHeading, 
								 COL_TYPE ColType/*该列的数据类型 默认为字符串*/,
								 int nFormat,/*对齐方式默认是 左对齐*/
								 int nWidth,
								 int nSubItem ,
								 BOOL bAutoSize/*是否自动调整列的大小 默认为自动*/)
{
	INT nResult = CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
	m_ColType.insert(CMPAIR(nCol,ColType));
	if(bAutoSize) {
		CClientDC dc(this);
		CSize sz = dc.GetTextExtent(lpszColumnHeading);
		SetColumnWidth(nCol,sz.cx+20);
	}
	return nResult;
}


int CMSortListCtrl::InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize)
{
	for (DWORD i = 0; i < dwSize;i++)
	{
		InsertColumn(lpColumnData[i].nCol,lpColumnData[i].szText,lpColumnData[i].ColType,
			lpColumnData[i].nFormat,lpColumnData[i].nLenght,-1,lpColumnData[i].bAutoSize);
	}
	return 1;
}

void CMSortListCtrl::PreSubclassWindow()
{
	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	if (pHeaderCtrl!=NULL)
		m_Header.SubclassWindow(pHeaderCtrl->GetSafeHwnd());
	CListCtrl::PreSubclassWindow();
}



BOOL CMSortListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect Rc;
	GetClientRect(Rc);
	CBrush bgBrush(GetBkColor());

	//强制刷新背景
	if(m_bRedrawBkgnd)
	{	
		pDC->FillRect(Rc,&bgBrush);
		m_bRedrawBkgnd = FALSE;
		return TRUE;
	}

// 	else
// 	{
// 		CRect temp = Rc;
//   		temp.top    = 20;
//   		temp.bottom = 22;
//  		pDC->FillRect(temp,&bgBrush);
// 
// 		temp = Rc;
// 		temp.right = 2;
// 		pDC->FillRect(temp,&bgBrush);
// 	}
	return TRUE;
}

BOOL CMSortListCtrl::RedrawWindow()
{
	m_bRedrawBkgnd = TRUE;
	return CListCtrl::RedrawWindow();
}

void CMSortListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OutputDebugString("CMSortListCtrl::OnLButtonDown");
	CListCtrl::OnLButtonDown(nFlags, point);
	CDragList::BeginDrag(m_hWnd,point); 
}

void CMSortListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDragList::EndDrag(m_hWnd,point);  
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CMSortListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( CDragList::m_SelectItem!=SELECT_NONE && nFlags&MK_LBUTTON )  
		CDragList::Dragging(m_hWnd,point);  
	CListCtrl::OnMouseMove(nFlags, point);
}


vector <RECT> CDragList::ItemRect;
int  CDragList::m_ActiveItem=SELECT_NONE;
bool CDragList::m_IsOut=true;
int  CDragList::m_SelectItem=SELECT_NONE;
bool CDragList::m_IsInFilstPart=false;
bool CDragList::m_IsInSecondPart=false;
POINT CDragList::MouseDownPoint={-10,-10};
 
void CDragList::BeginDrag(HWND ListWnd,POINT point) 
{
	//不可以设置焦点
	//::SetCapture(ListWnd);
	
	//设置焦点
	if (::GetFocus()!=ListWnd )
		::SetFocus(ListWnd);
	
	//enum to find out if select one
	int count=(int)::SendMessage(ListWnd, LVM_GETITEMCOUNT, 0, 0);
	int state;
	for (int i=0;i<count;i++)
	{
		state=(int)::SendMessage(ListWnd, LVM_GETITEMSTATE, i,(LPARAM) LVIS_SELECTED);
		if (state|LVIS_SELECTED==1)
		{
			m_SelectItem=i;
			m_ActiveItem=i;
			m_IsOut=true;
			MouseDownPoint=point;
		}
	}
}
void CDragList::EndDrag(HWND ListWnd,POINT point) 
{
//	::ReleaseCapture(); 
	if (m_ActiveItem!=SELECT_NONE )
	{
		::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
		if ( m_ActiveItem!=m_SelectItem && m_SelectItem!=SELECT_NONE)
		{
			//最后的Item的处理
			if (m_IsInSecondPart==true)
				m_ActiveItem+=1;
			
			::PostMessage(::GetParent(ListWnd),WM_SWAPITEM,m_SelectItem,m_ActiveItem);
		}
	}
	m_SelectItem=SELECT_NONE;
}



void CDragList::Dragging(HWND ListWnd,POINT point) 
{
	//当鼠标按下后没有移动就弹起时，ClistCtrl会发送一个click消息，
	//并且会有一个WM_MOUSEMOVE的消息（却没有WM_LBUTTONDUP的消息）
	//我们这里简单的返回
	if (point.x==MouseDownPoint.x && point.y==MouseDownPoint.y)
	{
		::ReleaseCapture(); 
		m_SelectItem=SELECT_NONE;
		return;
	}
	//get the rect of ListCtrl
	RECT rect;
	::GetWindowRect(ListWnd,&rect);
	rect.top+=24; //去掉头表的高度
	POINT MousePoint;
	::GetCursorPos(&MousePoint);
	
	//mouse is not in the ListCtrl
	if(!::PtInRect(&rect,MousePoint))
	{
		m_IsOut=true;
		if (m_ActiveItem!=SELECT_NONE)
		{
			::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
			m_ActiveItem=SELECT_NONE;
		}
		
		::SetCursor(::LoadCursor(NULL,IDC_CROSS)); 
		
		//超过窗体上边，向上滚动
		if (MousePoint.y<rect.top)
		{
			::SendMessage(ListWnd,WM_KEYDOWN,VK_UP,0); 
			::SendMessage(ListWnd,WM_KEYUP,VK_UP,0); 
		}
		//超过窗体下边，向下滚动
		else if (MousePoint.y>rect.bottom )
		{
			::SendMessage(ListWnd,WM_KEYDOWN,VK_DOWN,0);
			::SendMessage(ListWnd,WM_KEYUP,VK_DOWN,0); 
		}
		return;
	}
	
	//if is outside the widnow,re receive all the item rect again
	if (m_IsOut)
	{
		m_IsOut=false;
		m_IsInFilstPart=false;
		m_IsInSecondPart=false;
		//get the count of listctrl
		int count=(int)::SendMessage(ListWnd, LVM_GETITEMCOUNT, 0, 0);
		//initial data
		ItemRect.clear();
		ItemRect.resize(count);
		
		for (int i=0;i<count;i++)
		{
			//save item rect
			ItemRect[i].left=LVIR_SELECTBOUNDS ;
			::SendMessage(ListWnd,  LVM_GETITEMRECT , i,(LPARAM)&ItemRect[i]); 
			
		}
	}
	
	
	//begin find item
	int i;
	for(i=0; i<ItemRect.size() && !::PtInRect(&ItemRect[i],point) ; i++) ; //注意这行的分号
	
	//if found item(即鼠标在List里面)
	if(i!=ItemRect.size())  
	{
		//改变鼠标形状
		::SetCursor(::LoadCursor(NULL,IDC_CROSS)); 
		
		//在最后一项移动
		if (i==ItemRect.size()-1)
		{
			//是否第一次移入此Item
			if (m_ActiveItem!=i) 
			{
				//假如需要,更新原来的ItemRect
				if (m_ActiveItem!=SELECT_NONE)  
					::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
				
				//保存当前焦点的按钮索引
				m_ActiveItem=i;
			}    
			
			//在最后一个item的上半部分上移动
			if ( point.y<(ItemRect[m_ActiveItem].top+ItemRect[m_ActiveItem].bottom)/2 )
			{
				//第一次进入最后一个item的前半部分
				if (m_IsInFilstPart==false)
				{
					m_IsInFilstPart=true;
					m_IsInSecondPart=false;
					::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
					::UpdateWindow(ListWnd);
					DrawLine(ListWnd,ItemRect[m_ActiveItem].left+2,ItemRect[m_ActiveItem].top+2,
						ItemRect[m_ActiveItem].right-2,ItemRect[m_ActiveItem].top+2);
				}
			}
			
			//在最后一个item的后半部分上移动
			else
			{
				//第一次进入最后一个item的后半部分
				if (m_IsInSecondPart==false)
				{
					m_IsInSecondPart=true;
					m_IsInFilstPart=false;
					::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
					::UpdateWindow(ListWnd);
					DrawLine(ListWnd,ItemRect[m_ActiveItem].left+2,ItemRect[m_ActiveItem].bottom-2,
						ItemRect[m_ActiveItem].right-2,ItemRect[m_ActiveItem].bottom-2);
				}
			}
			return;
		}
		
		//是否第一次移入此Item
		if (m_ActiveItem!=i) 
		{
			m_IsInFilstPart=false;
			
			//假如需要,更新原来的ItemRect
			if (m_ActiveItem!=SELECT_NONE)  
				::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
			
			//保存当前焦点的按钮索引
			m_ActiveItem=i;
			
			DrawLine(ListWnd,ItemRect[m_ActiveItem].left+4,ItemRect[m_ActiveItem].top+2,
				ItemRect[m_ActiveItem].right-4,ItemRect[m_ActiveItem].top+2);
			
			return;
		}  
	}
	
	//if not found item(鼠标不在List里面)
	else
	{
		//假如第一次移出List
		if (m_ActiveItem!=SELECT_NONE)
		{
			//重绘原来的按钮
			::InvalidateRect(ListWnd,&ItemRect[m_ActiveItem],false);
			
			//表示鼠标在List外
			m_ActiveItem=SELECT_NONE; 
		}
		
		m_IsInFilstPart=false;
		m_IsInSecondPart=false;
		
		//改变鼠标形状
		::SetCursor(::LoadCursor(NULL,IDC_CROSS)); 
	} 
} 


void CDragList::DrawLine(HWND hWnd,int BeginX,int BeginY,int EndX,int EndY)
{
	HDC hdc=::GetDC(hWnd);
	HPEN Pen=::CreatePen(PS_SOLID,1,RGB(255,255,255));
	HPEN OldPen=(HPEN)SelectObject(hdc,Pen) ;
	HBRUSH OldBrush=(HBRUSH)::SelectObject (hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
	
	MoveToEx(hdc,BeginX,BeginY-2,NULL);
	LineTo(hdc,EndX,EndY-2);
	
	::SelectObject (hdc,OldBrush) ;
	::SelectObject (hdc,OldPen) ;
	::DeleteObject(Pen);
	::ReleaseDC(hWnd,hdc) ; 
} 