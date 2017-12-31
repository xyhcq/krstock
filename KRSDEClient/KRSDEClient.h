// KRSDEClient.h : main header file for the KRSDECLIENT DLL
//

#if !defined(AFX_KRSDECLIENT_H__A94FB7A8_99EF_4140_9028_50748D384DAD__INCLUDED_)
#define AFX_KRSDECLIENT_H__A94FB7A8_99EF_4140_9028_50748D384DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKRSDEClientApp
// See KRSDEClient.cpp for the implementation of this class
//

class CKRSDEClientApp : public CWinApp
{
public:
	CKRSDEClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKRSDEClientApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CKRSDEClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	GdiplusStartupInput		m_Gdistart; 
	ULONG_PTR				m_GdiplusToken;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KRSDECLIENT_H__A94FB7A8_99EF_4140_9028_50748D384DAD__INCLUDED_)
