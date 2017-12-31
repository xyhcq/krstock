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


//�е���������
enum COL_TYPE
{
	TYPE_FLOAT,		//������
	TYPE_INTGER,	//����-����
	TYPE_STRING,	//�ַ���
	TYPE_DATE,      //���� ��ʽΪ:YYYY*MM*DD  *Ϊ����ָ��� 2012/11/22 2012-11-22
	TYPE_DATETIME	//ʱ�� ʱ���ʽΪ:YYYY*MM*DD*HH*MM*SS  *Ϊ����ָ��� �� 2012/11/22 22:11::00 ,2012-11-22 22:11::00
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
	_SLMAP m_ColSortMark;//������
	_SLMAP m_ColType;	//����������
public:
	CStockList();
	
	//��̬�����Ļ���������øú���
	BOOL CreateEx(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID);
		
	
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

	//�����ı����Ҵ���ɫ��Ϣ
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
