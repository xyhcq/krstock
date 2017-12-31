#if !defined(AFX_STOCKLIST_H__3BC114CD_A4D1_4100_8D02_F74FE658C331__INCLUDED_)
#define AFX_STOCKLIST_H__3BC114CD_A4D1_4100_8D02_F74FE658C331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StockList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStockList window
#pragma warning(disable:4786)
#include <iostream>
#include <map>
using namespace std;


//列的数据类型
enum COL_TYPE
{
	TYPE_FLOAT,		//浮点数
	TYPE_INTGER,	//数字-整型
	TYPE_STRING,	//字符串
	TYPE_DATE,      //日期 格式为:YYYY*MM*DD  *为任意分隔符 2012/11/22 2012-11-22
	TYPE_DATETIME	//时间 时间格式为:YYYY*MM*DD*HH*MM*SS  *为任意分隔符 如 2012/11/22 22:11::00 ,2012-11-22 22:11::00
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



#define	 LVS_EX_DOUBLEBUFFER				0x00010000

typedef map<INT,BOOL>  _SLMAP;
typedef pair<INT,BOOL> _SLPAIR;

class CMyHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CMyHeaderCtrl)	
public:
	CMyHeaderCtrl();
	virtual ~CMyHeaderCtrl();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


class CStockList : public CListCtrl
{
// Construction

public:
	_SLMAP m_ColSortMark;//排序标记
	_SLMAP m_ColType;	//列数据类型
public:
	CStockList();
	
	//动态创建的话，必须调用该函数
	BOOL CreateEx(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID);
		
	
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

	//设置文本，且带颜色信息
	BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText,COLORREF colorBK);

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStockList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStockList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	CMyHeaderCtrl m_MyHeader;
};






/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKLIST_H__3BC114CD_A4D1_4100_8D02_F74FE658C331__INCLUDED_)
