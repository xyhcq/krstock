// KRStockOS.h : main header file for the KRSTOCKOS application
//

#if !defined(AFX_KRSTOCKOS_H__DE0D88A1_219E_4FDC_A5AD_13FC3E18CF2C__INCLUDED_)
#define AFX_KRSTOCKOS_H__DE0D88A1_219E_4FDC_A5AD_13FC3E18CF2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKRStockOSApp:
// See KRStockOS.cpp for the implementation of this class
//

class CKRStockOSApp : public CWinApp
{
public:
	CKRStockOSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKRStockOSApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKRStockOSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	GdiplusStartupInput		m_Gdistart; 
	ULONG_PTR				m_GdiplusToken;
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KRSTOCKOS_H__DE0D88A1_219E_4FDC_A5AD_13FC3E18CF2C__INCLUDED_)
