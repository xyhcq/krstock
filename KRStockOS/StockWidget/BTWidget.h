#ifndef BTWidget_h__
#define BTWidget_h__
#pragma  warning(disable:4244)

#include <math.h>
class CBTWidget
{
public:
	CBTWidget()
	{
		m_hBrushLrDa	= NULL;
		m_hBrushLrZhong = NULL;
		m_hBrushLrXiao  = NULL;
		m_hBrushLcDa    = NULL;
		m_hBrushLcZhong = NULL;
		m_hBrushLcXiao  = NULL;
	}
	~CBTWidget()
	{
		Destory();
	}
	void Create(CRect& rt)
	{
		m_rt = rt;
		Create(m_rt.left,m_rt.top,m_rt.Width(),m_rt.Height());
	}

	void Destory()
	{
		if (m_hBrushLrDa)
		{
			::DeleteObject(m_hBrushLrDa);
		}
		if (m_hBrushLrZhong)
		{
			::DeleteObject(m_hBrushLrZhong);
		}
		if (m_hBrushLrXiao)
		{
			::DeleteObject(m_hBrushLrXiao);
		}
		if (m_hBrushLcDa)
		{
			::DeleteObject(m_hBrushLcDa);
		}
		if (m_hBrushLcZhong)
		{
			::DeleteObject(m_hBrushLcZhong);
		}
		if (m_hBrushLcXiao)
		{
			::DeleteObject(m_hBrushLcXiao);
		}

		m_hBrushLrDa	= NULL;
		m_hBrushLrZhong = NULL;
		m_hBrushLrXiao  = NULL;
		m_hBrushLcDa    = NULL;
		m_hBrushLcZhong = NULL;
		m_hBrushLcXiao  = NULL;
	}

	void Draw(CDC * pDC,
		INT nLRDaDan,INT nLRZhongDan,INT nLRXiaoDan,
		INT nLCDaDan,INT nLCZhongDan,INT nLCXiaoDan)
	{

		CFont Font;
		Font.CreatePointFont( 90, "ËÎÌו" );
		HGDIOBJ hOldFont = pDC->SelectObject(Font);

		pDC->SelectStockObject(NULL_PEN);

		double total = (nLRDaDan+nLRZhongDan+nLRXiaoDan+nLCDaDan+nLCZhongDan+nLCXiaoDan);
		double oneCellPrecent = 360/total;

 		INT nIndex = 0;
 		INT nLcDa = nLCDaDan*oneCellPrecent;
		pDC->SelectObject(m_hBrushLcDa);
		DrawPrecent(pDC,nIndex,nLcDa);
 		nIndex+=nLcDa;


		double nLcZhong = nLCZhongDan*oneCellPrecent;
		pDC->SelectObject(m_hBrushLcZhong);
		DrawPrecent(pDC,nIndex,nLcZhong);
		nIndex+=nLcZhong;
		
		double nLcXiao  = nLCXiaoDan*oneCellPrecent;
		pDC->SelectObject(m_hBrushLcXiao);
		DrawPrecent(pDC,nIndex,nLcXiao);
		nIndex+=nLcXiao;

		double nLrXiao = nLRXiaoDan*oneCellPrecent;
		pDC->SelectObject(m_hBrushLrXiao);
		DrawPrecent(pDC,nIndex,nLrXiao);
		nIndex+=nLrXiao;

		double nLrZhong = nLRZhongDan*oneCellPrecent;
		pDC->SelectObject(m_hBrushLrZhong);
		DrawPrecent(pDC,nIndex,nLrZhong);
		nIndex+=nLrZhong;

		double nLrDa   = nLRDaDan*oneCellPrecent;
		pDC->SelectObject(m_hBrushLrDa);
		DrawPrecent(pDC,nIndex,360-nIndex);


		nIndex = 0;
		nIndex+=nLcDa;
		DrawText(pDC,nIndex,nLcDa);

		nIndex+=nLcZhong;
		DrawText(pDC,nIndex,nLcZhong);

		nIndex+=nLcXiao;
		DrawText(pDC,nIndex,nLcXiao);

		nIndex+=nLrXiao;
		DrawText(pDC,nIndex,nLrXiao);

		nIndex+=nLrZhong;
		DrawText(pDC,nIndex,nLrZhong);

		nIndex+=nLrDa;
		DrawText(pDC,nIndex,nLrDa);

		pDC->SelectObject(hOldFont);
	}

	void DrawText(CDC * pDC,int nIndex,INT nCount)
	{
		if((double)(nCount)*100.0/360.0 <= 0.0)
			return;

		int n = nIndex - (nCount)/2;
		CPoint p;
		CountPoint(n, p, TRUE);
		CString str;
		str.Format("%.0f%%", (double)(nCount)*100.0/360.0);
		CSize sz = pDC->GetTextExtent(str);
		pDC->SetTextColor(RGB(255,255,255));
		pDC->TextOut(p.x-sz.cx/2, p.y-sz.cy/2, str);
	}


	void DrawPrecent(CDC* pDC,int nIndex,INT nCount)
	{
		CPoint ptStart ;
		CPoint ptEnd  ;
		CountPoint(nIndex,ptEnd,FALSE);
		CountPoint(nIndex+nCount,ptStart,FALSE);
		pDC->Pie(&m_rt,ptStart,ptEnd);
	}
private:

	void Create(int x,int y,int cx,int cy)
	{
		m_x = x;
		m_y = y;
		m_cx = cx;
		m_cy = cy;
		
		m_hBrushLrDa	= CreateSolidBrush(RGB(177,28,30));
		m_hBrushLrZhong = CreateSolidBrush(RGB(203,78,80));
		m_hBrushLrXiao  = CreateSolidBrush(RGB(221,118,118));
		m_hBrushLcDa    = CreateSolidBrush(RGB(80,114,20));
		m_hBrushLcZhong = CreateSolidBrush(RGB(112,161,32));
		m_hBrushLcXiao  = CreateSolidBrush(RGB(181,197,102));
	}

	void CountPoint(int nAngle, CPoint & pt, BOOL bPercent)
	{
		double pi = 3.1415926535;
		int nLeft = m_x;
		int nTop  = m_y;
		int nRight = m_x+m_cx;
		int nBottom = m_y+ m_cy;
		
		while (nAngle <0)
			nAngle += 360;
		
		while (nAngle>359)
			nAngle -= 360;
		
		double dAngle = ((double)nAngle)*pi/(double)180;
		
		double r;
		r = ((double)nBottom- nTop)/2.0;
		if (bPercent)
			r = r*3.0/5.0;
		
		double dOffX = (r*sin(dAngle));
		double dOffY = 0.0 - (r*cos(dAngle));
		
		double dX = ((double)(nRight+nLeft))/2.0;
		double dY = ((double)(nTop+nBottom))/2.0;
		
		pt.x = (int)(dX + dOffX);
		pt.y = (int)(dY + dOffY);
	}

private:
	int m_x;
	int m_y;
	int m_cx;
	int m_cy;
	CRect m_rt;

	HBRUSH m_hBrushLrDa;
	HBRUSH m_hBrushLrZhong;
	HBRUSH m_hBrushLrXiao;
	HBRUSH m_hBrushLcDa;
	HBRUSH m_hBrushLcZhong;
	HBRUSH m_hBrushLcXiao;
};


#endif // BTWidget_h__