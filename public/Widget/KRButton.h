#ifndef KRButton_h__
#define KRButton_h__
#include <VECTOR>
using namespace std;

typedef struct _KRCHECKBOX_INFO
{
	CString szText;
	CRect   rc;
	INT     nID;
	BOOL    bCheck;

	COLORREF cTextColor;
	COLORREF cBgColor;
	COLORREF cFrameColor;

	COLORREF cCheckedTextColor;
	COLORREF cCheckedBgColor;
	COLORREF cCheckedFrameColor;
}KRCHECKBOX_INFO,*LPKRCHECKBOX_INFO;




class CKRCheckBox
{
public:
	void AddButton(LPCTSTR lpszText,INT nID,BOOL  bCheck,CRect  rc ,	
		COLORREF cTextColor,	COLORREF cBgColor,	COLORREF cFrameColor,
		COLORREF cCheckedTextColor,	COLORREF cCheckedBgColor,	COLORREF cCheckedFrameColor)
	{
		KRCHECKBOX_INFO KRCheckBoxInfo;
		KRCheckBoxInfo.szText	= lpszText;
		KRCheckBoxInfo.nID		= nID;
		KRCheckBoxInfo.rc		= rc;
		KRCheckBoxInfo.bCheck	= bCheck;
		
		KRCheckBoxInfo.cTextColor			=cTextColor;
		KRCheckBoxInfo.cBgColor				=cBgColor;
		KRCheckBoxInfo.cFrameColor			=cFrameColor;

		KRCheckBoxInfo.cCheckedTextColor	=cCheckedTextColor;
		KRCheckBoxInfo.cCheckedBgColor		=cCheckedBgColor;
		KRCheckBoxInfo.cCheckedFrameColor	=cCheckedFrameColor;
		m_KRCheckBoxInfo.push_back(KRCheckBoxInfo);
	}


	void Redraw(CDC * pDc)
	{

		for(INT i=0;i<m_KRCheckBoxInfo.size(); i++)
		{
			if(m_KRCheckBoxInfo[i].bCheck)
			{
				CBrush BgBrush(m_KRCheckBoxInfo[i].cCheckedBgColor);
				pDc->FillRect(m_KRCheckBoxInfo[i].rc,&BgBrush);
			
				CBrush FrameBrush(m_KRCheckBoxInfo[i].cCheckedFrameColor);
				pDc->FrameRect(m_KRCheckBoxInfo[i].rc,&FrameBrush);

				pDc->SetTextColor(m_KRCheckBoxInfo[i].cCheckedTextColor);
				pDc->TextOut(m_KRCheckBoxInfo[i].rc.left + 2 ,m_KRCheckBoxInfo[i].rc.top + 2,m_KRCheckBoxInfo[i].szText);
			}
			else
			{
				CBrush BgBrush(m_KRCheckBoxInfo[i].cBgColor);
				pDc->FillRect(m_KRCheckBoxInfo[i].rc,&BgBrush);
				
				CBrush FrameBrush(m_KRCheckBoxInfo[i].cFrameColor);
				pDc->FrameRect(m_KRCheckBoxInfo[i].rc,&FrameBrush);
				
				pDc->SetTextColor(m_KRCheckBoxInfo[i].cTextColor);
				pDc->TextOut(m_KRCheckBoxInfo[i].rc.left + 2 ,m_KRCheckBoxInfo[i].rc.top + 2,m_KRCheckBoxInfo[i].szText);
			}
		}
	}
private:
	vector<KRCHECKBOX_INFO> m_KRCheckBoxInfo;
};

#endif // KRButton_h__