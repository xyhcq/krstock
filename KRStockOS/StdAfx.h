// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7755AD77_D9C9_4C6F_A311_FF0A5E0F1840__INCLUDED_)
#define AFX_STDAFX_H__7755AD77_D9C9_4C6F_A311_FF0A5E0F1840__INCLUDED_
#define  _WIN32_WINNT  0x0400

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "resource.h"
#pragma warning(disable:4786)

#include <gdiplus.h>
using namespace Gdiplus; 
#pragma comment(lib,"gdiplus.lib")

#include "ComHeader/ComHeader.h"
#include "Common/Common.h"
#include "Style.h"
#include "DataMgr/ZXGDataMgr.h"

extern CKRStockDataEngine  KRSDE;
extern CKRStockDataEngine  KRSDE;
extern CZXGDataMgr		   ZXGDataMgr;

#define BGCOLOR_RED							229
#define BGCOLOR_BLUE						0
#define BGCOLOR_GREEN						0

#define KLWIDGET_BGCOLOR_RED				0
#define KLWIDGET_BGCOLOR_BLUE				0
#define KLWIDGET_BGCOLOR_GREEN				0

#define KLWIDGET_TEXTCOLOR_RED				255
#define KLWIDGET_TEXTCOLOR_BLUE				255
#define KLWIDGET_TEXTCOLOR_GREEN			255



#define TEXTCOLOR_RED						255
#define TEXTCOLOR_BLUE						255
#define TEXTCOLOR_GREEN						255


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7755AD77_D9C9_4C6F_A311_FF0A5E0F1840__INCLUDED_)
