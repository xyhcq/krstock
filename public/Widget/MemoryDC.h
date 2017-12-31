#pragma once
#include "Afxwin.h"

class CMemoryDC
{
public:
	CMemoryDC(CDC *dc, RECT * rect,bool autoRender = false);
	~CMemoryDC(void);
	
	bool IsOK();
	void Render(CDC * p_objectDC = NULL);
	CDC* GetMemoryDC();
	operator CDC * ();
private:
	bool m_bAutoRender;
	CRect m_DCRect;
	CDC* m_pOriginalDC;
	CDC m_MemoryDC;
	CBitmap m_MemoryBmp;
};