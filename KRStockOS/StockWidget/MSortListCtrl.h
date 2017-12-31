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
	filename: 	E:\������\SortListCtrlDemo\SortListCtrl.h
	file path:	E:\������\SortListCtrlDemo
	file base:	SortListCtrl
	file ext:	h
	author:		Coding���� 
	purpose:	�����Ǽ�ͷ���Զ������CListCtrl. δ��CListCtrl�ڲ����κθĶ���Ҳδ��CListCtrl���κ��ػ����
	������������ֲ�����Ĺ�����Ŀ�У����Բ�Ӱ��Ч�ʣ�֧��ASCII,UNICODE
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
//�е���������
enum COL_TYPE
{
	TYPE_FLOAT,		//������
	TYPE_INTGER,	//����-����
	TYPE_STRING,	//�ַ���
	TYPE_DATE		//ʱ�� ʱ���ʽΪ:YYYY*MM*DD*HH*MM*SS  *Ϊ����ָ��� �� 2012/11/22 22:11::00 ,2012-11-22 22:11::00
};

typedef struct _COLUMN_DATA
{
	INT nCol;			//�к�
	LPCTSTR szText;		//�б���
	int nFormat;		//�ж��뷽ʽ
	BOOL bAutoSize;		//�Ƿ��Զ��������
	INT nLenght;		//���
	COL_TYPE ColType;	//����������
}COLUMN_DATA,*LPCOLUMN_DATA;


typedef struct _ITEMCOLOR 
{
	COLORREF colText[30];
	COLORREF colTextBk[30];
}ITEMCOLOR,*LPITEMCOLOR;


//˫���塣
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
	//��̬�����Ļ�����ôPreSubclassWindow ���޷�����б�ͷ�������޷��Ի棬
	BOOL CreateEx(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID);
	/*	
		InsertColumn:
		�������˶��˸�ColType ��������CListCtrlһ����
		ColType ��ʾ������ʲô��������,����ð����ض��������򡣿���ȡ����ֵ:
		 1 TYPE_FLOAT
		 2 TYPE_INTGER
		 3 TYPE_STRING
		 4 TYPE_DATE
		 Ĭ��ΪTYPE_STRING ���ַ���
		 ���������뿴����һ��㡣
	*/

	int InsertColumn(
		int nCol,
		const LVCOLUMN* pColumn,
		COL_TYPE ColType=TYPE_STRING/*���е��������� Ĭ��Ϊ�ַ���*/,
		BOOL bAutoSize=TRUE/*�Ƿ��Զ������еĴ�С Ĭ��Ϊ�Զ�*/);

	int InsertColumn( int nCol, 
		LPCTSTR lpszColumnHeading, 
		COL_TYPE ColType=TYPE_STRING/*���е��������� Ĭ��Ϊ�ַ���*/,
		int nFormat = LVCFMT_LEFT,/*���뷽ʽĬ���� �����*/
		int nWidth = -1,
		int nSubItem = -1,
		BOOL bAutoSize=TRUE/*�Ƿ��Զ������еĴ�С Ĭ��Ϊ�Զ�*/);

	int InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText,COLORREF colorBK);
	BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText,COLORREF colorText);
	VOID SetSubItemColor(int nItem, int nSubItem,COLORREF colorText,COLORREF colorBK);

	VOID SetAllItemBkColor(COLORREF colorBK);
	BOOL RedrawWindow();
	//////////////////////////////////////////////////////////////////////////
	

	//�Զ������еĿ�� �ǳ��ǳ���ʵ��
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

	//������ �����б�ͷ m_header �Ա��Ի�
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
