// KeyDemonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "krstockos.h"
#include "KeyDemonDlg.h"
#include "Widget/MemoryDC.h"
#include "KRStockOSDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyDemonDlg dialog


CKeyDemonDlg::CKeyDemonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyDemonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyDemonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nLeft = 0;
	m_nTop  = 0;
	m_ListSelIndex = 0;
	m_szTitle = "猎鲸狂人按键精灵";
	m_bFirst  = TRUE;
}


void CKeyDemonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyDemonDlg)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CLOSE,	   m_BtnClose);
}


BEGIN_MESSAGE_MAP(CKeyDemonDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyDemonDlg)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_INPUT, OnChangeInput)
	ON_WM_CTLCOLOR()
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyDemonDlg message handlers
void FirstLetter(int nCode, std::string& strLetter)
{
	if(nCode >= 1601 && nCode < 1637) strLetter = "A";
	if(nCode >= 1637 && nCode < 1833) strLetter = "B";
	if(nCode >= 1833 && nCode < 2078) strLetter = "C";
	if(nCode >= 2078 && nCode < 2274) strLetter = "D";
	if(nCode >= 2274 && nCode < 2302) strLetter = "E";
	if(nCode >= 2302 && nCode < 2433) strLetter = "F";
	if(nCode >= 2433 && nCode < 2594) strLetter = "G";
	if(nCode >= 2594 && nCode < 2787) strLetter = "H";
	if(nCode >= 2787 && nCode < 3106) strLetter = "J";
	if(nCode >= 3106 && nCode < 3212) strLetter = "K";
	if(nCode >= 3212 && nCode < 3472) strLetter = "L";
	if(nCode >= 3472 && nCode < 3635) strLetter = "M";
	if(nCode >= 3635 && nCode < 3722) strLetter = "N";
	if(nCode >= 3722 && nCode < 3730) strLetter = "O";
	if(nCode >= 3730 && nCode < 3858) strLetter = "P";
	if(nCode >= 3858 && nCode < 4027) strLetter = "Q";
	if(nCode >= 4027 && nCode < 4086) strLetter = "R";
	if(nCode >= 4086 && nCode < 4390) strLetter = "S";
	if(nCode >= 4390 && nCode < 4558) strLetter = "T";
	if(nCode >= 4558 && nCode < 4684) strLetter = "W";
	if(nCode >= 4684 && nCode < 4925) strLetter = "X";
	if(nCode >= 4925 && nCode < 5249) strLetter = "Y";
	if(nCode >= 5249 && nCode < 5590) strLetter = "Z";
}

BOOL GenFirstPYLetter(std::string  strName, std::string & strResult )
{
	BOOL bStartByLetter = FALSE;
	
	unsigned char ucHigh, ucLow;
	int  nCode;
	std::string strRet;
	if (strName.empty())
		return FALSE;
	
	strResult="";
	if ((unsigned char)strName[0] < 0x80 )
		bStartByLetter = TRUE;
	
	for (size_t i=0; i < strName.size(); i++)
	{
		if ( (unsigned char)strName[i] < 0x80 )
		{
			strResult += strName[i];
			continue;
		}
		
		ucHigh = (unsigned char)strName[i];
		ucLow  = (unsigned char)strName[i+1];
		if ( ucHigh < 0xa1 || ucLow < 0xa1)
			continue;
		else
			// Treat code by section-position as an int type parameter,
			// so make following change to nCode.
			nCode = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;
		
		FirstLetter(nCode, strRet);
		strResult += strRet;
		i++;
	}
	return bStartByLetter;
}


string ToDBS(string str) 
{
    string result = "";
    unsigned char tmp; unsigned char tmp1;
    for (unsigned int i = 0; i < str.length(); i++) 
	{
        tmp = str[i];
        tmp1 = str[i + 1];
        if (tmp == 163) 
		{
			//第一个字节是163，标志着是全角字符
            result += (unsigned char) str[i + 1] - 128;
            i++;
            continue;
        } else if (tmp > 163) 
		{
			//汉字
            result += str.substr(i, 2);
            i++;
            continue;
        } else if (tmp == 161 && tmp1 == 161) 
		{
			//处理全角空格
            result += "";
            i++;
        } else 
		{
            result += str.substr(i, 1); 
		} 
	} 
	return result;
}





const char * GetLabelExInfo(const char * pszLabel)
{
	if(pszLabel[0] == 'S' && pszLabel[1] == 'H')
	{
		switch(pszLabel[2])
		{
		case '6':
			return "上海A股";
		case '9':
			return "上海B股";
		case '0':
			return "上海指数";
		}
	}

	if(pszLabel[0] == 'S' && pszLabel[1] == 'Z')
	{
		if(pszLabel[2] == '0' && pszLabel[3] == '0')
			return "深圳A股";

		if(pszLabel[2] == '3' && pszLabel[3] == '0')
			return "深圳A股";

		if(pszLabel[2] == '1' && pszLabel[3] == '5')
			return "深圳A股";

		if(pszLabel[2] == '3' && pszLabel[3] == '9')
			return "深圳A股";
	}
	return "";
}


BOOL CKeyDemonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetWindowText(m_szTitle);

	CRect rc;
	GetClientRect(rc);

	MoveWindow(m_nLeft,m_nTop,rc.Width(),rc.Height());

	m_BtnClose.SetStyle(KRSYSBTN_CLOSEBOX);
	m_BtnClose.SetHotColor(255,0,0);
	m_BtnClose.SetBkColor(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE);

	m_Font.CreatePointFont( 100, "System" );
	GetDlgItem(IDC_INPUT)->SetFont(&m_Font);
	GetDlgItem(IDC_LIST)->SetFont(&m_Font);

	INT nSize  = 0;
	const STOCK_TICKER_SYMBOL * pSTS = KRSDE.GetTickerSymbol(&nSize);
	if(pSTS == NULL)
		return FALSE;

	string  szName,szPY;
	CString szText;
	for (INT i=0; i< nSize; i++)
	{
		string szBj = ToDBS(pSTS->m_szName);
		GenFirstPYLetter(szBj,szPY);

		szText.Format("%-5s %-8s %-6s %s",szPY.c_str(),pSTS->m_szName,pSTS->m_szLabel,GetLabelExInfo((const char *)&pSTS->m_wMarket));
		m_PYDB.insert(pair<string,string>( string((const char *)&pSTS->m_wMarket),string((const char *)szText) ));

		szText.Format("%6s %-8s %s",pSTS->m_szLabel,pSTS->m_szName,GetLabelExInfo((const char *)&pSTS->m_wMarket));
		m_SZDB.insert(pair<string,string>( string((const char *)&pSTS->m_wMarket),string((const char *)szText) ));

		pSTS++;
	}

	if(!m_szInitText.IsEmpty())
	{
		SetDlgItemText(IDC_INPUT,m_szInitText);
		UpdateList(m_szInitText);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CKeyDemonDlg::UpdateList(CString & szKey)
{
	m_List.ResetContent();
	
	if(szKey.IsEmpty())
		return;
	
	if((szKey[0] >= 'A' && szKey[0] <= 'Z' ) || (szKey[0] >= 'a' && szKey[0] <= 'z' ) )
	{
		map<string , string >::const_iterator it ;
		for (it = m_PYDB.begin(); it != m_PYDB.end(); ++it)
		{
			CString szHead(it->second.c_str(), szKey.GetLength());
			if(StrStrI(szHead,szKey))
			{
				INT nIndex = m_List.AddString(it->second.c_str());
				m_List.SetItemData(nIndex,(DWORD)it->first.c_str());
				if( nIndex > 100)
					break;
			}
		}
	}else if( szKey[0] >= '0' && szKey[0] <= '9'  )
	{
		map<string , string >::const_iterator it ;
		for (it = m_SZDB.begin(); it != m_SZDB.end(); ++it)
		{
			CString szHead(it->second.c_str(), szKey.GetLength());
			if(StrStrI(szHead,szKey))
			{
				INT nIndex = m_List.AddString(it->second.c_str());
				m_List.SetItemData(nIndex,(DWORD)it->first.c_str());
				if( nIndex > 100)
					break;
			}
		}
	}else
	{		
		map<string , string >::const_iterator it ;
		for (it = m_PYDB.begin(); it != m_PYDB.end(); ++it)
		{
			if(strstr(it->second.c_str(),szKey))
			{
				INT nIndex = m_List.AddString(it->second.c_str());
				m_List.SetItemData(nIndex,(DWORD)it->first.c_str());
				if( nIndex > 100)
					break;
			}
		}
	}
	m_List.SetCurSel(0);
}



void CKeyDemonDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	
	CMemoryDC MemDC(&dc, rc, true);
	if (MemDC.IsOK())
	{
		Graphics raphics( MemDC.GetMemoryDC()->GetSafeHdc() );
		SolidBrush SBBK(Color(BGCOLOR_RED,BGCOLOR_GREEN,BGCOLOR_BLUE));
		
		raphics.FillRectangle(&SBBK,0,0,rc.Width(),rc.Height());
		raphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		raphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
		
		Font TitleFont(L"Arial", 9);
		StringFormat TitleFormat;                         
		TitleFormat.SetAlignment(StringAlignmentNear);
		TitleFormat.SetLineAlignment(StringAlignmentNear); 
		
		SolidBrush TitleBrush(Color(255, 255, 255));
		
		CTRString TRString;
		WCHAR * pszTitle = TRString.ANSIToUnicode("请输入股票代码或简拼");
		
		raphics.DrawString(pszTitle,wcslen(pszTitle),
			&TitleFont,
			RectF(2,2,(float)rc.Width(),(float)rc.Height()),
			&TitleFormat,
			&TitleBrush);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

extern HWND g_hKRStockOSDlg;

BOOL CKeyDemonDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == 280 && pMsg->wParam == 65528)
		PostMessage(WM_CLOSE);

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		INT nIndex = m_List.GetCurSel();

		CString szKey;
		GetDlgItemText(IDC_INPUT,szKey);
		if(nIndex >= 0 && szKey.GetLength() )
		{
			const char * p = (const char *)m_List.GetItemData(nIndex);

			::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)m_List.GetItemData(nIndex));
		}
	}
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(m_ListSelIndex >0 && pMsg->wParam == VK_UP)
		{
			m_List.SetCurSel(--m_ListSelIndex);
			m_List.SetFocus();
		}
		
		if(pMsg->wParam == VK_DOWN)
		{
			m_List.SetCurSel(++m_ListSelIndex);
			m_List.SetFocus();
		}
	}

	if(pMsg->message == WM_KEYUP)
	{
		GetDlgItem(IDC_INPUT)->SetFocus();
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CKeyDemonDlg::OnChangeInput() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString szKey;
	GetDlgItemText(IDC_INPUT,szKey);
	if(szKey.GetLength() == 0)
	{
		PostMessage(WM_CLOSE,0,0); 
	}else
	{
		UpdateList(szKey);
		m_ListSelIndex = 0;
	}
}


HBRUSH CKeyDemonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_LISTBOX)
		pDC->SetTextColor(RGB(38,56,217));
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CKeyDemonDlg::OnDblclkList() 
{
	// TODO: Add your control notification handler code here
	INT nIndex = m_List.GetCurSel();
	CString szKey;
	GetDlgItemText(IDC_INPUT,szKey);
	if(nIndex >= 0 && szKey.GetLength() )
	  ::SendMessage(g_hKRStockOSDlg,WM_SETLABLE,NULL,(LPARAM)m_List.GetItemData(nIndex));
}

void CKeyDemonDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(m_bFirst)
		((CEdit *)GetDlgItem(IDC_INPUT))->SetSel(-1, 0, false);

	m_bFirst = FALSE;
}
