#include "StdAfx.h"
#include "MemoryDC.h"

CMemoryDC::CMemoryDC(CDC *dc, RECT * rect, bool autoRender)
{    
	m_bAutoRender = autoRender;
	m_pOriginalDC = dc;
	if (dc==NULL || rect==NULL)
		return;

	if (!m_MemoryDC.CreateCompatibleDC(dc))
		return;

	m_DCRect.SetRect(rect->left, rect->top, rect->right, rect->bottom);   


	if (!m_MemoryBmp.CreateCompatibleBitmap(dc, m_DCRect.Width(), m_DCRect.Height()))
		return;

	m_MemoryDC.SelectObject(m_MemoryBmp);
}




CMemoryDC::~CMemoryDC(void)
{
	if (m_bAutoRender)
		Render();

	if (m_MemoryDC.m_hDC!=NULL)
		m_MemoryDC.DeleteDC();
	if (m_MemoryBmp.m_hObject!=NULL)
		m_MemoryBmp.DeleteObject();
}


bool CMemoryDC::IsOK()
{
	return m_MemoryDC.m_hDC!=NULL && m_MemoryBmp.m_hObject != NULL;
	
}


void CMemoryDC::Render(CDC * p_objectDC)
{
	if (!IsOK())
		return;
	
	CDC * pDC = (p_objectDC==NULL ? m_pOriginalDC : p_objectDC);

	pDC->BitBlt(
		m_DCRect.left, 
		m_DCRect.top,
		m_DCRect.Width(),
		m_DCRect.Height(),
		&m_MemoryDC, 
		0,0,
		SRCCOPY);
}


CDC* CMemoryDC::GetMemoryDC()
{
	return & m_MemoryDC;
}


CMemoryDC::operator CDC * ()
{
	return & m_MemoryDC;
}
