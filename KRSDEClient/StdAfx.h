// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9A1C09F0_48D8_4739_80F3_2FCF8A6979CC__INCLUDED_)
#define AFX_STDAFX_H__9A1C09F0_48D8_4739_80F3_2FCF8A6979CC__INCLUDED_
#define  _WIN32_WINNT  0x0400
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define  nullptr  NULL


#pragma warning(disable:4786)
#include <gdiplus.h>
using namespace Gdiplus; 
#pragma comment(lib,"gdiplus.lib")

//#include "STKDRV/STKDRV.h"
#include "STKDRV/Stockdrv.h"
#include "ComHeader/ComHeader.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9A1C09F0_48D8_4739_80F3_2FCF8A6979CC__INCLUDED_)
