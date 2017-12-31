// StockListWidget.cpp : implementation file
//

#include "stdafx.h"
#include "StockListWidget.h"
#include "StockAlgorithm.h"
#include "HQFunction.h"
#include "../KRStockOSDlg.h"
#include <VECTOR>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStockListWidget

CStockListWidget::CStockListWidget()
{
	m_bAutoUpdate   = TRUE;
	m_bRelevanceZXG = FALSE;

	srand(GetTickCount());
}

CStockListWidget::~CStockListWidget()
{
}


BEGIN_MESSAGE_MAP(CStockListWidget, CStatic)
	//{{AFX_MSG_MAP(CStockListWidget)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, 911, OnRclickList)
	ON_NOTIFY(NM_DBLCLK, 911, OnDbclickList)
	ON_MESSAGE(WM_SWAPITEM, DragMoveItem)
	ON_MESSAGE(WM_KRSDE_REALTIMEQUOTES_REMOTE,OnKRSDE_RealTimeQuotes_Remote)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStockListWidget message handlers
enum{
	    RLIST_COL_ID,
		RLIST_COL_DAIMA,
		RLIST_COL_MINGCHENG,
		
		RLIST_COL_ZHANGFU,
		RLIST_COL_ZHANGSU,
		RLIST_COL_ZHENGFU,
		
		RLIST_COL_XIANJIA,
		RLIST_COL_KAIPAN,
		RLIST_COL_ZUOSHOU,
		
		RLIST_COL_ZONGSHOU,
		RLIST_COL_JINE,
		RLIST_COL_LIANGBI,
		RLIST_COL_HUANSHOU,
		
		RLIST_COL_5RIHUANSHOU,
		RLIST_COL_5RIJINE,
		RLIST_COL_ZUIXINJIEJIN,
		
		RLIST_COL_SHIYINDONG,
		RLIST_COL_SHIJINGLV,
		RLIST_COL_MEIGUSHOUYI,
		
		RLIST_COL_LIUTONGSHIZHI,
		RLIST_COL_ZONGSHIZHI,
		RLIST_COL_LIUTONGBILI,
		
		RLIST_COL_SUOSHUHANGYE,
		RLIST_COL_HANGYEZHANGFU,
		
		RLIST_COL_GUDONGSHU,
		RLIST_COL_GDJSQBH,
		RLIST_COL_RENJUNSHICHI,
		RLIST_COL_SHRENJUNSHICHI,
		RLIST_COL_ZHULIZANBI,
};





COLUMN_DATA  g_RListColData[]=
{
	{RLIST_COL_ID,"ID",LVCFMT_RIGHT,FALSE,30,TYPE_INTGER},
	{RLIST_COL_DAIMA,"�� ��",LVCFMT_LEFT,FALSE,60,TYPE_STRING},
	{RLIST_COL_MINGCHENG," ���� ",LVCFMT_LEFT,FALSE,78,TYPE_STRING},
	
	{RLIST_COL_ZHANGFU,"�Ƿ�%",LVCFMT_RIGHT,FALSE,65,TYPE_FLOAT},
	{RLIST_COL_ZHANGSU,"����%",LVCFMT_RIGHT,FALSE,65,TYPE_FLOAT},
	{RLIST_COL_ZHENGFU,"���%",LVCFMT_RIGHT,FALSE,60,TYPE_FLOAT},
	
	{RLIST_COL_XIANJIA,"�ּ�",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	{RLIST_COL_KAIPAN,"����",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	{RLIST_COL_ZUOSHOU,"����",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	
	
	{RLIST_COL_ZONGSHOU,"����",LVCFMT_RIGHT,FALSE,74,TYPE_FLOAT},
	{RLIST_COL_JINE,"���",LVCFMT_RIGHT,FALSE,74,TYPE_FLOAT},
	{RLIST_COL_LIANGBI,"����",LVCFMT_CENTER,FALSE,50,TYPE_FLOAT},

	{RLIST_COL_HUANSHOU,"����%",LVCFMT_RIGHT,FALSE,55,TYPE_FLOAT},
	{RLIST_COL_5RIHUANSHOU,"5�վ���%",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	{RLIST_COL_5RIJINE,"5�վ���", LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	{RLIST_COL_ZUIXINJIEJIN,"���½��", LVCFMT_CENTER,FALSE,80,TYPE_STRING},
	
	{RLIST_COL_SHIYINDONG,"��ӯ��",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	{RLIST_COL_SHIJINGLV,"�о���",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	{RLIST_COL_MEIGUSHOUYI,"ÿ������",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	
	{RLIST_COL_LIUTONGSHIZHI,"��ͨ��ֵ",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	{RLIST_COL_ZONGSHIZHI,"����ֵ",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	{RLIST_COL_LIUTONGBILI,"��ͨ����",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	
	{RLIST_COL_SUOSHUHANGYE,"������ҵ",LVCFMT_LEFT,FALSE,80,TYPE_STRING},
	{RLIST_COL_HANGYEZHANGFU,"��ҵ�Ƿ�",LVCFMT_CENTER,FALSE,80,TYPE_FLOAT},
	
	{RLIST_COL_GUDONGSHU,"�ɶ���",LVCFMT_CENTER,FALSE,60,TYPE_INTGER},
	{RLIST_COL_GDJSQBH,"������",LVCFMT_CENTER,FALSE,60,TYPE_FLOAT},
	
	{RLIST_COL_RENJUNSHICHI,"�˾���ֵ",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	{RLIST_COL_SHRENJUNSHICHI,"ɢ���˾�",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	{RLIST_COL_ZHULIZANBI,"�����ֹ�",LVCFMT_RIGHT,FALSE,80,TYPE_FLOAT},
	
};



enum {

	LIST_MENU_TOTOP   = 911,
	LIST_MENU_TOBOTTOM,
	LIST_MENU_DELETE,
	LIST_MENU_ADD
};

enum{TIMER_RELEVANCEZXG = 911};



int CStockListWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	KRSDE.SetHWND(m_hWnd);

	m_List.CreateEx(WS_CHILD|WS_VISIBLE|LVS_REPORT | LVS_SINGLESEL /*| LVS_OWNERDRAWFIXED*/,CRect(0,0,0,0),this,911);

	m_List.SetExtendedStyle(m_List.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT|
		LVS_EX_DOUBLEBUFFER|
		LVS_EX_HEADERDRAGDROP);
	
	m_List.SetBkColor(COLOR_BLACK);
	m_List.SetTextBkColor(COLOR_BLACK);
	m_List.SetTextColor(COLOR_WHITE);

	m_ListFont.CreatePointFont(100,_T("System"),NULL);
	m_List.SetFont(&m_ListFont,FALSE);
	m_List.InsertColumn(g_RListColData,sizeof(g_RListColData)/sizeof(COLUMN_DATA));
	

	m_Menu.CreateMenu();	
	m_Menu_Sub.CreatePopupMenu();
	m_Menu.AppendMenu(MF_POPUP,(UINT)m_Menu_Sub.m_hMenu,"�Ҽ��˵�");
	
	m_Menu_Sub.AppendMenu(MF_STRING,LIST_MENU_DELETE,"ɾ����ѡ��(&D)" );
	m_Menu_Sub.AppendMenu(MF_STRING,LIST_MENU_TOTOP,"���ڶ���(&T)" );
	m_Menu_Sub.AppendMenu(MF_STRING,LIST_MENU_TOBOTTOM,"���ڵײ�(&B)" );

	if(m_bRelevanceZXG)	SetTimer(TIMER_RELEVANCEZXG,500,NULL);



// 	if(GetDlgCtrlID() == 3)
// 		SetTimer(0x100,500,NULL);
	return 0;
}







void CStockListWidget::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_List.GetSafeHwnd())
		m_List.MoveWindow(0,0,cx,cy);
}




INT CStockListWidget::Find( const char * szLabel )
{
	const REALTIMEQUOTES * pRTQ = KRSDE.GetRealTimeQuotes(szLabel);
	LVFINDINFO info;
	info.flags=LVFI_PARAM;
	info.lParam= (LPARAM)pRTQ;
	return m_List.FindItem(&info);
}



LRESULT CStockListWidget::OnKRSDE_RealTimeQuotes_Remote( WPARAM wParam,LPARAM lParam )
{
	if(m_List.GetItemCount() == 0)
		return FALSE;

	//���ڲ��ɼ���ʱ�� ����������
	if(m_bAutoUpdate && !IsWindowVisible())
		return FALSE;
	

	m_List.SetAllItemBkColor(COLOR_BLACK);

	//DWORD dwTickCount = GetTickCount();
	const REALTIMEQUOTES * pBegin = (const REALTIMEQUOTES *)wParam;
	INT nItem  = 0;
	LVFINDINFO info;
	const REALTIMEQUOTES * pRTQ = NULL;
	for (INT i=0; i< lParam;i++)
	{
		pRTQ = KRSDE.GetRealTimeQuotes((const char *)&pBegin->m_wMarket);
		if(pRTQ == NULL)
		{
			pBegin++;
			continue;
		}

		info.flags=LVFI_PARAM;
		info.lParam= (LPARAM)pRTQ;
		nItem = m_List.FindItem(&info);	
		//������  ��ʱȫ��UpdateItem ��for��ʱ����10ms
		if(nItem != -1) 
			UpdateItem(nItem,pBegin);
		pBegin++;
	}

//	CString szText;
//	szText.Format("%d",GetTickCount() - dwTickCount);
//	OutputDebugString(szText);

	m_List.Invalidate();
	m_List.UpdateWindow();
	return TRUE;
}





void CStockListWidget::UpdateItem(INT nItem, const REALTIMEQUOTES * pRTQ)
{
	//�������� ��ʱԭ���HQFunction
	
	CHQFunction HQFunction;
	HQFunction .Init(pRTQ);
	
	CString szText;
	float fZhangFu = HQFunction.ZhangFuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHANGFU,szText,GetColor(fZhangFu));

	float fZhangSu = HQFunction.ZhangSuText(5,szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHANGSU,szText,GetColor(fZhangSu));
	
	HQFunction.ZhengFuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHENGFU,szText,COLOR_GRAY);
	
	float fNewPrice = HQFunction.NewPriceText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_XIANJIA,szText,GetColor(pRTQ->m_fLastClose,fNewPrice) ,RGB(38,56,217));
	
	float fOpen = HQFunction.OpenPriceText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_KAIPAN,szText,GetColor(pRTQ->m_fLastClose,fOpen));

	float fAmount = HQFunction.AmountText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_JINE,szText,GetAmountColor(fAmount));
	
	float fVolume = HQFunction.VolumeText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZONGSHOU,szText,COLOR_TIANLAN);

	float fLiangBi = HQFunction.LiangBiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_LIANGBI,szText,GetLiangBiColor(fLiangBi));
	
	float fHuanShou = HQFunction.HuanShouText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_HUANSHOU,szText,GetHuanShouColor(fHuanShou));

	HQFunction.ShiYingDongText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_SHIYINDONG,szText,COLOR_GRAY);
	
	HQFunction.LiuTongZhiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_LIUTONGSHIZHI,szText,COLOR_TIANLAN);
	
	HQFunction.ZongShiZhiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZONGSHIZHI,szText,COLOR_TIANLAN);	
	
	float fHangYeZhangFu = HQFunction.HangYeZhangFuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_HANGYEZHANGFU,szText,GetColor(fHangYeZhangFu));
	
	HQFunction.RenJunChiGuSZText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_RENJUNSHICHI,szText,COLOR_TIANLAN);
	
	HQFunction.SHRenJunChiGuSZText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_SHRENJUNSHICHI,szText,COLOR_TIANLAN);
}


BOOL CStockListWidget::AddLabel( const REALTIMEQUOTES * pRTQ )
{
	if(pRTQ == NULL)
		return FALSE;

	INT nItem = m_List.GetItemCount();
	m_List.InsertItem(nItem,"");
	m_List.SetItemData(nItem,(DWORD)pRTQ);	
	return InsertLabel(pRTQ,nItem);
}


BOOL CStockListWidget::AddLabel( const char * szLabel )
{
	if(szLabel == NULL)
		return FALSE;

	const REALTIMEQUOTES * pRTQ = KRSDE.GetRealTimeQuotes(szLabel);
	if(pRTQ == NULL)
		return FALSE;

	INT nItem = m_List.GetItemCount();
	m_List.InsertItem(nItem,"");
	m_List.SetItemData(nItem,(DWORD)pRTQ);	
	return InsertLabel(pRTQ,nItem);
}



BOOL CStockListWidget::InsertLabel( const REALTIMEQUOTES * pRTQ ,INT nItem )
{
	CHQFunction HQFunction;
	HQFunction .Init(pRTQ);

	CString szText('\0',100);

	szText.Format("%4d",nItem);
	m_List.SetItemTextEx(nItem,RLIST_COL_ID,szText,COLOR_WHITE,COLOR_BLACK);
	m_List.SetItemTextEx(nItem,RLIST_COL_DAIMA,pRTQ->m_szLabel,COLOR_WHITE,COLOR_BLACK);
	m_List.SetItemTextEx(nItem,RLIST_COL_MINGCHENG,pRTQ->m_szName,COLOR_WHITE,COLOR_BLACK);


	float fZhangFu = HQFunction.ZhangFuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHANGFU,szText,GetColor(fZhangFu),COLOR_BLACK);

	float fZhangSu = HQFunction.ZhangSuText(5,szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHANGSU,szText,GetColor(fZhangSu),COLOR_BLACK);

	HQFunction.ZhengFuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHENGFU,szText,COLOR_GRAY,COLOR_BLACK);

	float fNewPrice = HQFunction.NewPriceText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_XIANJIA,szText,GetColor(pRTQ->m_fLastClose,fNewPrice),COLOR_BLACK);
	
	float fOpen = HQFunction.OpenPriceText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_KAIPAN,szText,GetColor(pRTQ->m_fLastClose,fOpen),COLOR_BLACK);

	float fLastClose = HQFunction.LastCloseText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZUOSHOU,szText,RGB(255,255,255),COLOR_BLACK);
	 	
	float fAmount = HQFunction.AmountText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_JINE,szText,GetAmountColor(fAmount),COLOR_BLACK);
	
	float fVolume = HQFunction.VolumeText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZONGSHOU,szText,COLOR_TIANLAN,COLOR_BLACK);

	float fLiangBi = HQFunction.LiangBiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_LIANGBI,szText,GetLiangBiColor(fLiangBi),COLOR_BLACK);

	float fHuanShou = HQFunction.HuanShouText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_HUANSHOU,szText,GetHuanShouColor(fHuanShou),COLOR_BLACK);

	float fDay5HuanShou = HQFunction.Day5HuanShouText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_5RIHUANSHOU,szText,GetHuanShouColor(fDay5HuanShou),COLOR_BLACK);
	
	float fDay5Amount = HQFunction.Day5AmountText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_5RIJINE,szText,GetAmountColor(fDay5Amount),COLOR_BLACK);

	HQFunction.ZuiXinJieJin(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZUIXINJIEJIN,szText,COLOR_GRAY,COLOR_BLACK);

	HQFunction.ShiYingDongText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_SHIYINDONG,szText,COLOR_GRAY,COLOR_BLACK);

	HQFunction.ShiJingText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_SHIJINGLV,szText,COLOR_GRAY,COLOR_BLACK);
	
	HQFunction.MeiGuShowYiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_MEIGUSHOUYI,szText,COLOR_GRAY,COLOR_BLACK);


	HQFunction.LiuTongZhiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_LIUTONGSHIZHI,szText,COLOR_TIANLAN,COLOR_BLACK);

	HQFunction.ZongShiZhiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZONGSHIZHI,szText,COLOR_TIANLAN,COLOR_BLACK);	

	HQFunction.LiuTongBiLiText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_LIUTONGBILI,szText,COLOR_TIANLAN,COLOR_BLACK);	

	HQFunction.SuoShuHangYeText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_SUOSHUHANGYE,szText,COLOR_GRAY,COLOR_BLACK);

	float fHangYeZhangFu = HQFunction.HangYeZhangFuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_HANGYEZHANGFU,szText,GetColor(fHangYeZhangFu),COLOR_BLACK);

	HQFunction.GuDongShuText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_GUDONGSHU,szText,COLOR_GRAY,COLOR_BLACK);

	float fGDJSQBH = HQFunction.GDJSQBHText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_GDJSQBH,szText,GetColor(fGDJSQBH),COLOR_BLACK);

	HQFunction.RenJunChiGuSZText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_RENJUNSHICHI,szText,COLOR_TIANLAN,COLOR_BLACK);
 
 	HQFunction.SHRenJunChiGuSZText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_SHRENJUNSHICHI,szText,COLOR_TIANLAN,COLOR_BLACK);

	HQFunction.QSDLTGDZBText(szText,"-");
	m_List.SetItemTextEx(nItem,RLIST_COL_ZHULIZANBI,szText,COLOR_TIANLAN,COLOR_BLACK);
	return TRUE;
}






void CStockListWidget::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	
	m_nCurlSel = m_List.GetNextSelectedItem(pos);
	const REALTIMEQUOTES * pRRS = (const REALTIMEQUOTES *) m_List.GetItemData(m_nCurlSel);

	if(ZXGDataMgr.Select((const char *)&pRRS->m_wMarket))
	{
		m_Menu_Sub.ModifyMenu(0,MF_BYPOSITION,LIST_MENU_DELETE, "ɾ����ѡ��(&D)");
	}else
	{
		m_Menu_Sub.ModifyMenu(0,MF_BYPOSITION,LIST_MENU_ADD, "������ѡ��(&D)");
	}

	if(m_nCurlSel == 0)
		m_Menu_Sub.EnableMenuItem(LIST_MENU_TOTOP,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	else
		m_Menu_Sub.EnableMenuItem(LIST_MENU_TOTOP,MF_BYCOMMAND|MF_ENABLED);


	if(m_nCurlSel == m_List.GetItemCount() - 1)
		m_Menu_Sub.EnableMenuItem(LIST_MENU_TOBOTTOM,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	else
		m_Menu_Sub.EnableMenuItem(LIST_MENU_TOBOTTOM,MF_BYCOMMAND|MF_ENABLED);


	CPoint point;
	GetCursorPos(&point);
	CMenu * pSubMenu = m_Menu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN| TPM_LEFTBUTTON| TPM_RIGHTBUTTON,point.x,point.y,this); 

	*pResult = 0;
}


extern HWND g_hKRStockOSDlg;
void CStockListWidget::OnDbclickList( NMHDR* pNMHDR, LRESULT* pResult )
{

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	
	m_nCurlSel = m_List.GetNextSelectedItem(pos);

	const REALTIMEQUOTES * pRRS = (const REALTIMEQUOTES *) m_List.GetItemData(m_nCurlSel);
	::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)&pRRS->m_wMarket);
	*pResult = 0;	
}


BOOL CStockListWidget::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_COMMAND)
	{
		if(HIWORD(pMsg->wParam) == 0 )
		{
			switch(LOWORD(pMsg->wParam))
			{
			case LIST_MENU_DELETE: 
				{
 					const REALTIMEQUOTES * pRRS = (const REALTIMEQUOTES *) m_List.GetItemData(m_nCurlSel);	
 					ZXGDataMgr.Delete((const char *)&pRRS->m_wMarket);
				}break;

			case LIST_MENU_ADD:
				{
 					const REALTIMEQUOTES * pRRS = (const REALTIMEQUOTES *) m_List.GetItemData(m_nCurlSel);	
 					ZXGDataMgr.Insert(*pRRS);
				}break;

			case LIST_MENU_TOTOP: 
				{
					const REALTIMEQUOTES * pCurSelRRS = (const REALTIMEQUOTES *) m_List.GetItemData(m_nCurlSel);
					m_List.DeleteItem(m_nCurlSel);
					m_List.InsertItem(0,"");
					m_List.SetItemData(0,(DWORD)pCurSelRRS);	
					InsertLabel(pCurSelRRS,0);
				}
				break;
			case LIST_MENU_TOBOTTOM: 
				{
					const REALTIMEQUOTES * pCurSelRRS = (const REALTIMEQUOTES *) m_List.GetItemData(m_nCurlSel);
					m_List.DeleteItem(m_nCurlSel);

					INT nItem = m_List.GetItemCount();
					m_List.InsertItem(nItem,"");
					m_List.SetItemData(nItem,(DWORD)pCurSelRRS);	
					InsertLabel(pCurSelRRS,nItem);
				}
				break;
			}
		}
	}
	return CStatic::PreTranslateMessage(pMsg);
}




INT CStockListWidget::GetCount()
{
	return m_List.GetItemCount();
}


BOOL CStockListWidget::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}



void CStockListWidget::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0x100)
	{
		vector<REALTIMEQUOTES>  VRTQ;
		KRSDE.GetAllRealTimeQuotes(VRTQ);
		for (INT i=0; i< VRTQ.size(); i++)
		{
			VRTQ[i].m_fNewPrice += rand() % 10;
		}
		OnKRSDE_RealTimeQuotes_Remote((WPARAM)VRTQ.begin(), (LPARAM)VRTQ.size()); 
	}
	

	//ͬ����ѡ��
	if(nIDEvent == TIMER_RELEVANCEZXG)
	{
		map<string, REALTIMEQUOTES > MData              = ZXGDataMgr.GetMap();
		if(MData.size() != m_List.GetItemCount())
		{
			//���жϲ���
			map<string, REALTIMEQUOTES >::const_iterator it = MData.begin();
			for(it; it != MData.end(); ++it)
			{
				if(-1 == Find((const char *)&it->second.m_wMarket))
					AddLabel((const char *)&it->second.m_wMarket);
			}

			//���ж�ɾ��
			const REALTIMEQUOTES * pRTQ = NULL;
			INT nItem = m_List.GetItemCount();
			for (INT i=0; i< nItem; i++)
			{
				pRTQ = (const REALTIMEQUOTES *)m_List.GetItemData(i);
				if(MData.end() == MData.find((const char *)&pRTQ->m_wMarket))
				{
					m_List.DeleteItem(i);
					break;
				}
			}
			m_List.RedrawWindow();
		}
	}
	CStatic::OnTimer(nIDEvent);
}

LRESULT CStockListWidget::DragMoveItem(WPARAM wParam, LPARAM lParam)  
{
	int nSrcItem=wParam;
	int nDestItem=lParam;
	if( abs(nDestItem - nSrcItem ) == 1)
		return FALSE;


 	const REALTIMEQUOTES * pSrcRRS = (const REALTIMEQUOTES *) m_List.GetItemData(nSrcItem);

	if(nDestItem > nSrcItem)
	{
		m_List.InsertItem(nDestItem,"");
		m_List.SetItemData(nDestItem,(DWORD)pSrcRRS);	
 		InsertLabel(pSrcRRS,nDestItem);
 		m_List.DeleteItem(nSrcItem);
	}else
	{
		m_List.DeleteItem(nSrcItem);
		m_List.InsertItem(nDestItem,"");
		m_List.SetItemData(nDestItem,(DWORD)pSrcRRS);	
 		InsertLabel(pSrcRRS,nDestItem);
	}
	return TRUE;
}

void CStockListWidget::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_List.RedrawWindow();
	// Do not call CStatic::OnPaint() for painting messages
}
