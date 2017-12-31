#if !defined(AFX_MSORTLISTCTRL_H__1888833F_72A3_4628_B984_593563E3FAA7__INCLUDED_)
#define AFX_MSORTLISTCTRL_H__1888833F_72A3_4628_B984_593563E3FAA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSortListCtrl.h : header file
//

#if _MSC_VER ==1200 
#pragma warning(disable:4786)
#endif
#include <iostream>
#include <map>
using namespace std;



/////////////////////////////////////////////////////////////////////////////
// CMSortListCtrl window
/********************************************************************
	created:	2012/11/22
	created:	22:11:2012   0:04
	filename: 	E:\界面编程\SortListCtrlDemo\SortListCtrl.h
	file path:	E:\界面编程\SortListCtrlDemo
	file base:	SortListCtrl
	file ext:	h
	author:		Coding狂人 
	purpose:	带三角箭头可自动排序的CListCtrl. 未对CListCtrl内部做任何改动，也未对CListCtrl做任何重绘操作
	，可以随意移植到您的工程项目中，绝对不影响效率，支持ASCII,UNICODE
*********************************************************************/


class CKRHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CKRHeaderCtrl)	
public:
	CKRHeaderCtrl();
	virtual ~CKRHeaderCtrl();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



//////////////////////////////////////////////////////////////////////////
//列的数据类型
enum COL_TYPE
{
	TYPE_FLOAT,		//浮点数
	TYPE_INTGER,	//数字-整型
	TYPE_STRING,	//字符串
	TYPE_DATE		//时间 时间格式为:YYYY*MM*DD*HH*MM*SS  *为任意分隔符 如 2012/11/22 22:11::00 ,2012-11-22 22:11::00
};

typedef struct _COLUMN_DATA
{
	INT nCol;			//列号
	LPCTSTR szText;		//列标题
	int nFormat;		//列对齐方式
	BOOL bAutoSize;		//是否自动调整宽度
	INT nLenght;		//宽度
	COL_TYPE ColType;	//列数据类型
}COLUMN_DATA,*LPCOLUMN_DATA;


typedef struct _ITEMCOLOR 
{
	COLORREF colText[30];
	COLORREF colTextBk[30];
}ITEMCOLOR,*LPITEMCOLOR;


//双缓冲。
#define	 LVS_EX_DOUBLEBUFFER				0x00010000
#define LVS_EX_SINGLEROW					0x00040000

typedef map<INT,BOOL> CMMAP;
typedef pair<INT,BOOL> CMPAIR;

class CMSortListCtrl : public CListCtrl
{
// Construction
public:
	CMSortListCtrl();
// Attributes
public:
	CMMAP m_ColSortMark;
	CMMAP m_ColType;
	map<DWORD,ITEMCOLOR> m_ItemColor;
	INT m_nCurCol;
// Operations
public:
	//////////////////////////////////////////////////////////////////////////
	//动态创建的话，那么PreSubclassWindow 中无法获得列表头，所以无法自绘，
	BOOL CreateEx(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID);
	/*	
		InsertColumn:
		参数除了多了个ColType 其他均与CListCtrl一样。
		ColType 表示该列是什么数据类型,排序好按照特定类型排序。可以取以下值:
		 1 TYPE_FLOAT
		 2 TYPE_INTGER
		 3 TYPE_STRING
		 4 TYPE_DATE
		 默认为TYPE_STRING 即字符串
		 参数含义请看上面一点点。
	*/

	int InsertColumn(
		int nCol,
		const LVCOLUMN* pColumn,
		COL_TYPE ColType=TYPE_STRING/*该列的数据类型 默认为字符串*/,
		BOOL bAutoSize=TRUE/*是否自动调整列的大小 默认为自动*/);

	int InsertColumn( int nCol, 
		LPCTSTR lpszColumnHeading, 
		COL_TYPE ColType=TYPE_STRING/*该列的数据类型 默认为字符串*/,
		int nFormat = LVCFMT_LEFT,/*对齐方式默认是 左对齐*/
		int nWidth = -1,
		int nSubItem = -1,
		BOOL bAutoSize=TRUE/*是否自动调整列的大小 默认为自动*/);

	int InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText,COLORREF colorBK);
	BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText);
	VOID SetSubItemColor(int nItem, int nSubItem,COLORREF colorText,COLORREF colorBK);

	VOID SetAllItemBkColor(COLORREF colorBK);
	BOOL RedrawWindow();
	//////////////////////////////////////////////////////////////////////////
	

	//自动调整列的宽度 非常非常的实用
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSortListCtrl)
	//}}AFX_VIRTUAL

protected:
	void SetHeaderSortImage(HWND listView, int columnIndex, BOOL isAscending);
// Implementation
public:
	virtual ~CMSortListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMSortListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

	//重载中 关联列表头 m_header 以便自绘
	virtual void PreSubclassWindow();
private:
	CHAR m_szFormatBufferA[2048];
	TCHAR m_szFormatBuffer[2048];
	CKRHeaderCtrl m_Header;
	BOOL          m_bRedrawBkgnd;
	BOOL	      m_bLockEraseBkgnd;
};

/////////////////////////////////////////////////////////////////////////////

#include <vector>
using namespace std;
#define SELECT_NONE -1
#define BUTTON_UP -1
#define WM_SWAPITEM WM_USER+435

class CDragList  
{
private:
	static vector <RECT> ItemRect;
	static int  m_ActiveItem;
	static bool  m_IsOut;
	static bool  m_IsInFilstPart;
	static bool  m_IsInSecondPart;
	static POINT MouseDownPoint;
public:
	static int  m_SelectItem;
public:
	static void BeginDrag(HWND ListWnd,POINT point) ;
	static void EndDrag(HWND ListWnd,POINT point) ;
	static void Dragging(HWND ListWnd,POINT point) ;
	static void DrawLine(HWND hWnd,int BeginX,int BeginY,int EndX,int EndY);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSORTLISTCTRL_H__1888833F_72A3_4628_B984_593563E3FAA7__INCLUDED_)
