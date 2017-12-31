#ifndef trace_h__
#define trace_h__


/*==================trace ��������ַ���====================
 * (һ)���ܣ�
 * ������Ա���(������TRACE)
 * (��)�ص�:
 * 1.�Զ���Ӧ����������(char��char*��string��CString��HWND��POINT��RECT....)
 * 2.�Զ���Ӧ��������ĸ�����(�ر��˱������� #pragma warning(disable: 4003) )
 * 3.��������ı���ֵǰ���Զ���ӱ��������ƣ�����鿴
 * 4.�����Զ�����Edit�����������ת������ַ�����
 * 5.�����˳�ʱ�Ὣ����ַ������浽����Ŀ¼�µ�DebugData.txt������鿴
 * 6.���ֱ���ģʽ���������DEBUG��Release�汾����������Ա����������߶������
 * 7.֧��UNICODE,WIN32,Dll,MFC

 * (��)ʹ��˵����
 * 1.��trace.h���Ƶ�����Ŀ¼�£����Բ���ӵ����̣���
 * 2.���ļ�"stdafx.h"��ļ����·������ #include "trace.h"��֮��Ϳ���ʹ��trace()���ˡ�
 * 3.����������ַ����ᱣ���ڹ���Ŀ¼�µ�"DebugData.txt"�ļ����Է���鿴
 * 4.�Ұ����д��붼����һ��ͷ�ļ����Ȼ���Ϸ��淶��������ʹ�������ܷ��㡣
 * 5.trace(x,y,z,w)��ԭ����4������������������ʱ����������������棬
 * ����ʹ�� #pragma warning(disable: 4003) ��������뾯����ص��ˡ�

 * (��)����ʹ�õĺ꣺
 * trace(X,Y,Z,W) //����������ͱ�������int��double��short��POINT��RECT��string
 * //���Զ���Ӧ�����ĸ�����������Ϊ1-4����
 * tracef() //��ʽ���ַ���������sprintf
 * traceLastError()//���ϵͳ������룬������GetLastError()
 * traceRel(X,Y)  //��X=true�����"Y: successful" ; x=false�����"Y: failed"
 * CEasyTrace::clear();  //��մ���
 *

 * (��)����trace��ʹ�����ã�
 * 1.Ĭ������� NO_TRACE_WINDOW �� TRACE_WINDOW��û���壬��
 * DEBUG�汾����������ַ�������Release�汾����
 * 2.�����ͷ������#define NO_TRACE_WINDOW
 * DEBUG�汾��Release�汾�����������������ַ���
 * 3.�����ͷ������#define TRACE_WINDOW
 * DEBUG�汾��Release�汾���������������ַ���
 * 4.ÿ���޸�����2�������Ҫȫ�����±���Ż���Ч

 * (��)����ʾ����
 * void CDemoDlg::OnButton1()
 * {
 * int a=45; double b=67.45; double c=847.424;short d=324;
 * RECT rect;
 * ::GetWindowRect(m_hWnd,&rect);
 * HWND hWnd=::GetForegroundWindow();
 * POINT point;
 * GetCursorPos(&point);
 * bool ak=true;
 * CPoint cpoint(88,421);
 * CRect crect(19,423,56,522);
 * CString mfcstr=_T("mfc CString");
 * TCHAR buf[100]=_T("c style string");
 * #ifdef _UNICODE
 * wstring stlstr=_T("stl string");
 * #else
 * string stlstr=_T("stl string");
 * #endif
 * tracef(_T("tracef ������ %s"),_T("sprintf")); //��ʽ���ַ���������sprintf
 * trace(_T("Impossible is nothing"));
 * trace(stlstr);  //����ַ���
 * trace(mfcstr);  //����ַ���
 * trace(buf);  //����ַ���
 * trace(a); //���int
 * trace(a,b); //���int��double
 * trace(a,b,c);
 * trace(a,b,c,d);
 * trace(point);
 * trace(point,cpoint); //���POINT �� CPoint
 * trace(rect);
 * trace(rect,crect); //���RECT �� CRect
 * trace(hWnd);  //���������Ϣ
 * traceLastError(); //���ϵͳ�������
 * traceRel(ak==true,"func()");  //��X=true�����"Y: successful" ; x=false�����"Y: failed"
 * }

 * (��)���ɣ�
 * 1.��ʱ������ֻ��۲�ĳһ�������ı仯�����������кܶ�trace��������������ڵ�
 * ����Ӱ�������ǵĹ۲졣�������ǿ���������������������trace����Ч�ˡ�
 * BEGINTRACE; //��������
 * trace(_T("ֻ�� BEGINTRACE �� ENDTRACE ֮���trace����Ч"));
 * ENDTRACE; //��ֹ��������
 *
 * ��Ȩ����:
 * û�а�Ȩ������κε�λ,��������ʹ��,����,�޸ģ������߲��е���
 * �˴���������κ���ʧ����������ˮƽ���ޣ����������֮������
 * �����⣬�ɴ�Ϊ������Ĳ��㣬����������������κ�BUG������ϵ���ߣ�
 * ���һ�����������Ժ��и���ͳһ����������ַ�ϣ��������������
 * �����Լ���ϸ��ʹ��˵����
 * ��ǰ�������ڣ�2011-10-21
 * http://blog.csdn.net/jacky_qiu/archive/2010/11/04/5986089.aspx
 * ���:
 * ����˷ݴ���Ϊ������˰���������ʹ��������л֮�⡣���ҿɷ�����
 * ���Ĺ�������10�顰��(��:��na)��(��:Ħmo)�����ӷ𡱣�
 * Ը�÷����ӳ֣�ʹ�㹤��˳�����ϼ��Ҹ���
 *
 *    Jacky
 *    2011-11-7
 *    qiujiejia@gmail.com
 * ========================================================================*/


#ifndef _EASY_TRACE_H_
#define _EASY_TRACE_H_

/*
 * trace����뿪�ة�����������������
 *  #define NO_TRACE_WINDOW //DEBUG�汾��Release�汾�����������������ַ���
 *  #define TRACE_WINDOW //DEBUG�汾��Release�汾���������������ַ���
*/

#ifdef NO_TRACE_WINDOW
#else
#ifdef _DEBUG
#define TRACE_WINDOW
#else
#ifdef RELEASE_TRACE
#define TRACE_WINDOW
#endif
#endif
#endif

/*
 * trace��Ĳ�����4��������������4������������������棬�����ֶ��ر����
 * ����(disable warning C4003: not enough actual parameters for macro)
*/

#pragma warning(disable: 4003)
#pragma warning(disable: 4002)

#define TRACE_WND_TEXT	_T( "TraceWnd" )        /* ���ڱ��� */
#define AUTO_HIDE_TIME	3000                    /* ������ٺ�����Զ����� */
#include <Windows.h>                            /* ��Ҫ��ͷ�ļ� */
#include <tchar.h>
#include <sstream>
#ifndef ASSERT
#include <crtdbg.h>
#define ASSERT( X ) _ASSERT( X );
#endif

/* Not #define TRACE_WINDOW */
#ifndef TRACE_WINDOW
#define trace( X )
#define tracef __noop
#define traceLastError()
#define traceRel( X )
#define traceClear( X )
#define BEGINTRACE
#define ENDTRACE
#endif

/* #define TRACE_WINDOW */
#ifdef TRACE_WINDOW
/* ����������������������������������������������������������trace�ꩥ������ */
#define trace( X, Y, Z, W ) { CEasyTrace obj; \
			      obj.name( # X ); obj.add( X ); \
			      obj.name( # Y ); obj.add( Y ); \
			      obj.name( # Z ); obj.add( Z ); \
			      obj.name( # W ); obj.add( W ); }
#define tracef CEasyTrace::Tracef
#define traceLastError()	{ CEasyTrace().TraceSysError(); }
#define traceRel( X )		{ CEasyTrace().TraceResult( # X, X ); }
#define traceClear( X )		{ ::SendMessage( ::FindWindow( NULL, TRACE_WND_TEXT ), WM_COMMAND, 3, NULL ); }
#define BEGINTRACE	{ CTraceWnd::ConfigData().IsReceive = true; }
#define ENDTRACE	{ CTraceWnd::ConfigData().IsReceive = false; }
/* �������������������������������������������������������������������������������� */

class CTraceWnd
{
	struct TRACEWNDCONFIG
	{
		int	x;
		int	y;
		int	cx;
		int	cy;
		BOOL	IsContinue;     /* ������ַ���ʱ�Ƿ����֮ǰ������ */
		BOOL	IsAutoWidth;    /* �Ƿ�����ַ��������Զ��������ڿ�� */
		BOOL	IsAutoHide;     /* �Ƿ��Զ����� */
		BOOL	IsHide;         /* �Ƿ����� */
		BOOL	IsName;         /* �Ƿ��Զ����ϱ����� */
		BOOL	IsIndex;        /* ����к����� */
		BOOL	IsSplitter;     /* 2���ַ�������ʱ������1��ʱ��ӷָ��� */
		BOOL	IsDeleteBefore; /* 2���ַ�������ʱ������1��ʱɾ��ǰ�����������ַ��� */
		BOOL	IsReceive;      /* �Ƿ�������� */
	};
public:
	~CTraceWnd()
	{
		::DestroyWindow( ::FindWindow( NULL, TRACE_WND_TEXT ) );
	}


	static int & IndexNum()
	{
		static int num = 1;
		return(num);
	}


	static HWND & SaveEditWnd()
	{
		static HWND wnd = NULL;
		return(wnd);
	}


	static TRACEWNDCONFIG & ConfigData()
	{
		static TRACEWNDCONFIG config = { 0, 0, 400, 400, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE };
		return(config);
	}


	/****************************************************************************
	*  �������к�
	****************************************************************************/
	static TCHAR* GetIndex()
	{
#define _INDEX_NUM_MAX 4
		static TCHAR	SrcIndex[11]			= { 0 };
		static TCHAR	NewIndex[_INDEX_NUM_MAX + 1]	= { ' ', ' ', ' ', ' ', 0 };
		/* reset NewIndex buffer */
		if ( IndexNum() == 1 )
			for ( int i = 0; i < _INDEX_NUM_MAX; i++ )
				NewIndex[i] = ' ';
		_itot( IndexNum()++, SrcIndex, 10 );    /* int ת�ı� */
		int	len	= _tcslen( SrcIndex );
		int	count	= _INDEX_NUM_MAX - len; /* 4 - �����λ�� */
		if ( count > 0 )
		{
			for ( int i = count, j = 0; i < _INDEX_NUM_MAX; i++, j++ )
				NewIndex[i] = SrcIndex[j];
			return(NewIndex);
		}else
			return(SrcIndex);
	}


	/****************************************************************************
	*  ������ַ���������
	****************************************************************************/
	static void CTraceWnd::PrintString( const TCHAR* OutputStr )
	{
		if ( SaveEditWnd() == NULL )
		{
			/* Ѱ���Ѵ������������ */
			HWND MainWnd = ::FindWindow( NULL, TRACE_WND_TEXT );
			/* ��������ڣ��򴴽����ڲ���ʼ������ */
			if ( !MainWnd )
			{
				/*
				 * �������������Ϊ�˳����˳��ǵ�������������
				 * �����ļ��͹رմ��ڣ�����֮��û�������ô���
				 */
				static CTraceWnd ForGetCloseMessage;
				/* �������� */
				MainWnd		= CreateMainWnd();
				SaveEditWnd()	= CreateEditWnd( MainWnd );
				if ( !MainWnd )
				{
					::MessageBox( ::GetFocus(), _T( "�޷�����trace����" ), NULL, MB_OK );
					return;
				}
			}
		}
		if ( CTraceWnd::ConfigData().IsReceive == false )
			return;
		/* ���ַ������������ */
		::SendMessage( SaveEditWnd(), EM_REPLACESEL, FALSE, (LPARAM) OutputStr );
	}


	/****************************************************************************
	*  //���洰������
	****************************************************************************/
	static void SaveConfig( TRACEWNDCONFIG & config )
	{
		HKEY hKey;
		if ( ::RegOpenKeyEx( HKEY_CURRENT_USER, _T( "Software\\" ), 0, KEY_WRITE, &hKey ) == ERROR_SUCCESS )
		{
			::RegSetValueEx( hKey, _T( "config" ), 0, REG_BINARY, (LPBYTE) &config, sizeof(config) );
			::RegCloseKey( hKey );
		}
	}


	static BOOL GetConfig( TRACEWNDCONFIG & config )
	{
		HKEY	hKey;
		long	ret = ::RegOpenKeyEx( HKEY_CURRENT_USER, _T( "Software\\" ), 0, KEY_READ, &hKey );
		if ( ret == ERROR_SUCCESS )
		{
			DWORD	type	= REG_BINARY;
			DWORD	cbData	= sizeof(config);
			::RegQueryValueEx( hKey, _T( "config" ), 0, &type, (LPBYTE) &config, &cbData );
			::RegCloseKey( hKey );
			return(TRUE);
		}
		return(FALSE);
	}


	static void ShowTraceWnd()
	{
		::SetWindowPos( ::FindWindow( NULL, TRACE_WND_TEXT ), HWND_TOPMOST,
				ConfigData().x, ConfigData().y, ConfigData().cx, ConfigData().cy, SWP_NOACTIVATE );
		ConfigData().IsHide = false;
	}


	static void HideTraceWnd()
	{
		::SetWindowPos( ::FindWindow( NULL, TRACE_WND_TEXT ), HWND_TOPMOST, -100, 0, 104, GetSystemMetrics( SM_CYSCREEN ), NULL );
		ConfigData().IsHide = true;
	}


	static HWND CreateEditWnd( HWND ParentWnd )
	{
		RECT rect;
		::GetClientRect( ParentWnd, &rect );
		HWND EditWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, TEXT( "Edit" ), TRACE_WND_TEXT,
					       WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE |
					       ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE,
					       0, 0, rect.right, rect.bottom,
					       ParentWnd, NULL, NULL, NULL );
		/* ��hWnd��Ĭ�ϴ��ڹ����滻ΪWindowProc,����Ĭ�Ϻ������̵ĺ���ָ�� */
		WNDPROC OldWndProc = (WNDPROC) SetWindowLongPtr( EditWnd, GWLP_WNDPROC, (LONG_PTR) EditWindowProc );
		ASSERT( OldWndProc );
		/* ����OldWndProc */
		::SetWindowLongPtr( EditWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(OldWndProc) );

		/* �޸�edit��������ַ����� */
		::SendMessage( EditWnd, EM_SETLIMITTEXT, 1000000, 0 );
		/* ���Edit */
		::SendMessage( EditWnd, WM_SETTEXT, NULL, (LPARAM) _T( "" ) );
		/* ����������� */
		LOGFONT LogFont;
		::memset( &LogFont, 0, sizeof(LOGFONT) );
		lstrcpy( LogFont.lfFaceName, _T( "Fixedsys" ) );
		LogFont.lfHeight	= -12; /* �����С */
		LogFont.lfCharSet	= GB2312_CHARSET;
		/* �������� */
		HFONT hFont = CreateFontIndirect( &LogFont );
		/* �������� */
		::SendMessage( EditWnd, WM_SETFONT, (WPARAM) hFont, 0 );
		::PostMessage( EditWnd, WM_RBUTTONDOWN, NULL, MAKELPARAM( -10000, -10000 ) );
		return(EditWnd);
	}


	static HWND CreateMainWnd()
	{
		TRACEWNDCONFIG & config = ConfigData();
		GetConfig( config );
		config.IsReceive = true;

		/* ������Ļ���µ��� */
		int	SrcCx	= GetSystemMetrics( SM_CXSCREEN );
		int	SrcCy	= GetSystemMetrics( SM_CYSCREEN );
		if ( config.x < 0 || config.x >= SrcCx )
			config.x = 0;
		if ( config.y < 0 || config.y >= SrcCy )
			config.y = 0;
		if ( config.cx < 100 )
			config.cx = 100;
		if ( config.cy < 40 )
			config.cy = 40;
		if ( config.x + config.cx > SrcCx )
			config.x = SrcCx - config.cx;
		if ( config.y + config.cy > SrcCy )
			config.y = SrcCy - config.cy;

		/* ������������������������������������������������������������������ */
		HWND hWnd;
		if ( config.IsHide ) /* �������������״̬ */
		{
			hWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, _T( "#32770" ), TRACE_WND_TEXT,
					       WS_OVERLAPPEDWINDOW,
					       -100, 0, 104, GetSystemMetrics( SM_CYSCREEN ), NULL, NULL, NULL, NULL );
		}else  {
			hWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, _T( "#32770" ), TRACE_WND_TEXT,
					       WS_OVERLAPPEDWINDOW,
					       config.x, config.y, config.cx, config.cy, NULL, NULL, NULL, NULL );
		}
		::ShowWindow( hWnd, SW_SHOWNOACTIVATE ); /* ��ʾʱ�޽��� */
		/* ASSERT(hWnd); */
		if ( hWnd == NULL )
			return(NULL);
		/* ��hWnd��Ĭ�ϴ��ڹ����滻ΪWindowProc,����Ĭ�Ϻ������̵ĺ���ָ�� */
		WNDPROC OldWndProc = (WNDPROC) SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR) MainWindowProc );
		ASSERT( OldWndProc );
		/* ����OldWndProc */
		::SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(OldWndProc) );
		/* ������������������������������������������������������������������ */

		/* �����������������������������������������������������������������������˵� */
		HMENU hMenu = CreateMenu();
		AppendMenu( hMenu, MF_STRING, 5, _T( "���ص���Ļ���" ) );
		AppendMenu( hMenu, MF_STRING, 3, _T( "��մ���" ) );
		/* -----------------sub menu */
		HMENU hSubMenu = CreateMenu();
		AppendMenu( hSubMenu, MF_STRING, 6, _T( "�Զ�����" ) );
		AppendMenu( hSubMenu, MF_STRING, 7, _T( "��������" ) );
		AppendMenu( hSubMenu, MF_STRING, 4, _T( "�����ַ��������Զ��������ڿ��" ) );
		AppendMenu( hSubMenu, MF_STRING, 9, _T( "�����������" ) );
		AppendMenu( hSubMenu, MF_STRING, 10, _T( "���ʱ�Զ���ӱ�����" ) );
		AppendMenu( hSubMenu, MF_STRING, 11, _T( "���ʱ����к�����" ) );
		AppendMenu( hSubMenu, MF_STRING, 12, _T( "2���ַ�������ʱ������1��ʱ��ӷָ���" ) );
		AppendMenu( hSubMenu, MF_STRING, 13, _T( "2���ַ�������ʱ������1��ʱɾ��ǰ�����������ַ���" ) );
		AppendMenu( hSubMenu, MF_STRING, 8, _T( "����" ) );
		/* -----------------sub menu */
		AppendMenu( hMenu, MF_POPUP, UINT( hSubMenu ), _T( "ѡ��" ) );
		::SetMenu( hWnd, hMenu );
		/* ������������������������������������������������������������������ */
		return(hWnd);
	}


	static LRESULT CALLBACK MainWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		TRACEWNDCONFIG & config = ConfigData();
		switch ( message )
		{
		case WM_COMMAND:
			::SendMessage( ::GetWindow( hWnd, GW_CHILD ), WM_COMMAND, wParam, lParam );
			break;
		case WM_CLOSE:
			::DestroyWindow( hWnd );
			break;
		case WM_DESTROY:
			DestroyMenu( GetMenu( hWnd ) );
			break;
		case WM_SIZE:
		{
			int	cx	= LOWORD( lParam );
			int	cy	= HIWORD( lParam );
			::SetWindowPos( ::GetWindow( hWnd, GW_CHILD ), NULL, 0, 0, cx, cy, SWP_NOMOVE );
		}
		break;
		case WM_INITMENUPOPUP:
		{
			HMENU hSubMenu = (HMENU) wParam;
			CheckMenuItem( hSubMenu, 6, MF_BYCOMMAND | config.IsAutoHide  ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 7, MF_BYCOMMAND | config.IsReceive   ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 4, MF_BYCOMMAND | config.IsAutoWidth ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 9, MF_BYCOMMAND | config.IsContinue  ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 10, MF_BYCOMMAND | config.IsName     ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 11, MF_BYCOMMAND | config.IsIndex     ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 12, MF_BYCOMMAND | config.IsSplitter  ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( hSubMenu, 13, MF_BYCOMMAND | config.IsDeleteBefore ? MF_CHECKED : MF_UNCHECKED );
		}
		break;

		case WM_NCLBUTTONDOWN:
		{
			POINT	point		= { LOWORD( lParam ), HIWORD( lParam ) };
			UINT	nHitTest	= wParam;
			RECT	rect		= { -10000, 0, 0, 0 };
			if ( nHitTest == HTTOPLEFT || nHitTest == HTTOPRIGHT
			     || nHitTest == HTBOTTOMLEFT || nHitTest == HTBOTTOMRIGHT
			     || nHitTest == HTLEFT || nHitTest == HTRIGHT
			     || nHitTest == HTTOP || nHitTest == HTBOTTOM
			     || nHitTest == HTCAPTION )
			{
				::GetWindowRect( hWnd, &rect );
			}
			LRESULT ret = DefWindowProc( hWnd, message, wParam, lParam );
			if ( config.IsHide )
			{
				ShowTraceWnd(); /* ������ʾ���� */
			}
			if ( rect.left != -10000 )
			{
				RECT NewRect;
				::GetWindowRect( hWnd, &NewRect );
				if ( 0 != memcmp( &rect, &NewRect, sizeof(rect) ) )
				{
					config.x	= NewRect.left;
					config.y	= NewRect.top;
					config.cx	= NewRect.right - NewRect.left;
					config.cy	= NewRect.bottom - NewRect.top;
					SaveConfig( config );
				}
			}
			return(ret);
		}
		break;

		default:
		{
			WNDPROC OldWndProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
			ASSERT( OldWndProc != NULL );
			return(::CallWindowProc( OldWndProc, hWnd, message, wParam, lParam ) );
		}
		}
		return(0);
	}


	/****************************************************************************
	*  Edit Window�Ĵ��ڹ���
	****************************************************************************/
	static LRESULT CALLBACK EditWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		static HWND		StaticWnd	= NULL; /*  */
		TRACEWNDCONFIG &	config		= ConfigData();
		switch ( message )
		{
/*
 * case WM_ACTIVATE:
 * {
 * if (LOWORD(wParam)==WA_INACTIVE ) //����ʧȥ����
 * {
 * if (config.IsAutoHide)
 * {
 * ::SetTimer(hWnd,1,200,NULL); //�ӳ�200ms��hide
 * }
 * }
 * }
 * break;
*/
		case WM_RBUTTONDOWN:
		{
			if ( lParam == MAKELPARAM( -10000, -10000 ) )
			{
				/* ���û�п����Զ����ز����ִ�������״̬���򵯳���ʾ���� */
				if ( !config.IsAutoHide && config.IsHide )
				{
					StaticWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST & ~WS_EX_APPWINDOW, /* ȡ������������ */
								    _T( "Static" ),
								    _T( "Trace������������Ļ���\n�����Ļ��߿ɻָ�����" ),
								    WS_POPUP | WS_VISIBLE | WS_BORDER |
								    SS_CENTER,
								    0, 200, 200, 40, NULL, NULL, NULL, NULL );
					SetTimer( hWnd, 2, 2000, NULL );
				}
			}
		}
		break;

		case WM_TIMER:
		{
			if ( wParam == 2 ) /* �ر���ʾ���� */
			{
				::DestroyWindow( StaticWnd );
				::KillTimer( hWnd, 2 );
				break;
			}
			if ( !config.IsHide && ::GetForegroundWindow() != GetParent( hWnd ) )
			{
				HideTraceWnd(); /* ���ش��� */
			}
		}
		break;

		case WM_COMMAND:
		{
			switch ( wParam )
			{
			case 3:         /* ������� */
			{
				IndexNum() = 1;
				::SendMessage( hWnd, WM_SETTEXT, NULL, (LPARAM) _T( "" ) );
			}
			break;
			case 8:
				::SendMessage( hWnd, WM_SETTEXT, NULL, (LPARAM) _T( "http://blog.csdn.net/jacky_qiu/article/details/5986089" ) );
				break;
			case 5:                 /* hide window int the left of the screen */
			{
				HideTraceWnd(); /* ���ش��� */
				/* ���ش��ں�Ҫ��ֹ�Զ����� */
				config.IsAutoHide = false;
				KillTimer( hWnd, 1 );
				SaveConfig( config );
			}
			break;
			case 4:                 /* modify the Auto Adjust window width option */
			{
				config.IsAutoWidth = !config.IsAutoWidth;
				SaveConfig( config );
			}
			break;
			case 6:                 /* modify the Auto hide window option */
			{
				config.IsAutoHide = !config.IsAutoHide;
				if ( config.IsAutoHide )
					::SetTimer( hWnd, 1, AUTO_HIDE_TIME, NULL );
				else
					::KillTimer( hWnd, 1 );
				SaveConfig( config );
			}
			break;
			case 7:         /* modify the receivd date option */
			{
				if ( config.IsReceive )
					PrintString( _T( "\r\nע�⣺��ǰ���ڽ�ֹ��������״̬\r\n" ) );
				config.IsReceive = !config.IsReceive;
				SaveConfig( config );
			}
			break;
			case 9:         /* modify the receivd date option */
			{
				config.IsContinue = !config.IsContinue;
				SaveConfig( config );
			}
			break;
			case 10:
			{
				config.IsName = !config.IsName;
				SaveConfig( config );
			}
			break;
			case 11:
			{
				config.IsIndex = !config.IsIndex;
				SaveConfig( config );
			}
			break;
			case 12:
			{
				config.IsSplitter = !config.IsSplitter;
				if ( config.IsSplitter )
					config.IsDeleteBefore = FALSE;
				SaveConfig( config );
			}
			break;
			case 13:
			{
				config.IsDeleteBefore = !config.IsDeleteBefore;
				if ( config.IsDeleteBefore )
					config.IsSplitter = FALSE;
				SaveConfig( config );
			}
			break;
			}
		}
		break;
		case EM_REPLACESEL:
		{
			if ( !config.IsReceive )
				return(0);
			if ( config.IsContinue )
			{
				/* ���ָ����� */
				::SendMessage( hWnd, EM_SETSEL, 4294967290, 4294967290 );
				WNDPROC OldWndProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
				if ( config.IsSplitter )
				{
					static DWORD	begin;
					DWORD		end = GetTickCount();
					if ( end - begin > 1000 )
					{
						TCHAR	fen[]		= _T( "����������������������������������������������������������������\r\n" );
						TCHAR	* outputstr	= (TCHAR *) lParam;
						int	len		= _tcslen( outputstr );
						TCHAR	* p		= new TCHAR[len + sizeof(fen) / sizeof(TCHAR)];
						_tcscpy( p, fen );
						_tcscpy( p + sizeof(fen) / sizeof(TCHAR) - 1, outputstr );
						/* ����Ĭ�Ϲ�������ַ�����edit ���� */
						::CallWindowProc( OldWndProc, hWnd, message, wParam, (LPARAM) p );
						delete p;
					}else
						::CallWindowProc( OldWndProc, hWnd, message, wParam, lParam );
					begin = end;
				}else if ( config.IsDeleteBefore )
				{
					static DWORD	begin;
					DWORD		end = GetTickCount();
					if ( end - begin > 1000 )
					{
						::SetWindowText( hWnd, (TCHAR *) lParam );
					}else
						::CallWindowProc( OldWndProc, hWnd, message, wParam, lParam );
					begin = end;
				}else  {
					::CallWindowProc( OldWndProc, hWnd, message, wParam, lParam );
				}
			}else  {
				::SendMessage( hWnd, WM_SETTEXT, NULL, lParam );
			}


			/* �������Զ����ع��� */
			if ( config.IsAutoHide )
			{
				if ( config.IsHide )
				{
					ShowTraceWnd(); /* ������ʾ���� */
				}
				/* ����AUTO_HIDE_TIME�����ش��� */
				::SetTimer( hWnd, 1, AUTO_HIDE_TIME, NULL );
			}

			/* �������Զ��������ڵĿ�ȹ��� */
			if ( config.IsAutoWidth )
			{
				if ( !config.IsHide )
				{
					static int	m_WindowWidth = 200; /* ���ڿ�� */
					int		MinPos, MaxPos;
					GetScrollRange( hWnd, SB_HORZ, &MinPos, &MaxPos );
					if ( m_WindowWidth < MaxPos + 40 )
					{
						HWND ParentWnd = GetParent( hWnd );
						m_WindowWidth = MaxPos + 40;
						RECT rect;
						::GetWindowRect( ParentWnd, &rect );
						::SetWindowPos( ParentWnd, NULL, 0, 0, m_WindowWidth, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOACTIVATE );
					}
				}
			}
		}
		break;
		case WM_DESTROY:
		{
			IndexNum()	= 1;
			SaveEditWnd()	= NULL;
			/*ɾ������ */
			DeleteObject( (HFONT) ::SendMessage( hWnd, WM_GETFONT, 0, 0 ) );
			/*
			 * ���ó������е�Ŀ¼�������ļ�����Ŀ¼��ͬ
			 * �����ǵĳ������������ʱ�����Ĺ���Ŀ¼�ʹ������ǡ�
			 * �������Ŀ¼����ͬ�ġ�����������Ҫ��Ŀ¼�Ļ���
			 */
			TCHAR	AppPath[MAX_PATH];
			int	nlen = GetModuleFileName( NULL, AppPath, MAX_PATH );
			while ( AppPath[--nlen] != '\\' )
				;
			AppPath[nlen] = '\0';
			SetCurrentDirectory( AppPath );

			/* ��ȡ���ڵ��ַ��� */
			int	len		= ::SendMessage( hWnd, WM_GETTEXTLENGTH, 0, 0 );
			TCHAR	* WindowText	= new TCHAR[(len + 1) * sizeof(TCHAR)];
			::GetWindowText( hWnd, WindowText, len + 1 );   /* GetWindowText���Զ����NULL������ */

			/* �����ڱ��������ļ�DebugData.txt, */
			FILE* fp = fopen( "DebugData.txt", "wb" );

#ifdef _UNICODE    /* ���������unicode������תΪansi�ٱ��� */
			/*
			 * len��Դ�ַ����ĳ��� len=_tcslen(WindowText)
			 * (len+1)*2��Դ�ַ���ռ�õ��ڴ�ռ䣬����NULL������
			 * nChars����ת�����ַ�������������NULL����nChars=_tcslen(buffer)
			 * wcstombs���Զ����NULL������
			 */
			char* buffer = new char[(len + 1) * 2];
			setlocale( LC_ALL, ".936" );
			int nChars = wcstombs( buffer, WindowText, (len + 1) * 2 );
			setlocale( LC_ALL, "C" );
			fwrite( buffer, 1, nChars, fp );
			delete buffer;
#else
			fwrite( WindowText, 1, len, fp );
#endif
			delete[] WindowText;
			fclose( fp );
		}
		break;

		case WM_KEYDOWN:
		{
			/* select all */
			if ( wParam == 'A' && GetKeyState( VK_CONTROL ) < 0 )
				::SendMessage( hWnd, EM_SETSEL, 0, -1 );
		}
		break;

		default:
		{
			WNDPROC OldWndProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
			ASSERT( OldWndProc != NULL );
			return(::CallWindowProc( OldWndProc, hWnd, message, wParam, lParam ) );
		}
		}
		return(0);
	}
};


/****************************************************************************
*  CEasyTrace
****************************************************************************/
class CEasyTrace
{
private:
#ifdef _UNICODE
	std::wostringstream m_str;
#else
	std::ostringstream m_str;
#endif
public:
	void add()
	{
	}       /* ��trace���������ʱ����ô˺��� */


	void name()
	{
	}       /* ��trace���������ʱ����ô˺��� */


	/* ��ʼ��һ��ostringstream������������к� */
	CEasyTrace()
	{
		if ( CTraceWnd::ConfigData().IsIndex )
			m_str << CTraceWnd::GetIndex() << _T( "  " );
		else
			m_str << _T( "  " );
	}


	/* ����ʱ���ַ��������Edit���� */
	~CEasyTrace()
	{
		m_str << _T( "\r\n" ); /* ��������С� */
#ifdef _UNICODE
		CTraceWnd::PrintString( std::wstring( m_str.str() ).c_str() );
#else
		CTraceWnd::PrintString( std::string( m_str.str() ).c_str() );
#endif
	}


	/* ��������� */
	void name( char* pStr )
	{
		if ( !CTraceWnd::ConfigData().IsName )
			return;
		if ( strchr( pStr, '\"' ) != NULL )
			return;
#ifdef _UNICODE
		wchar_t* UnicodeStr = AnsiToUnicode( pStr );
		m_str << UnicodeStr << _T( "=" );
		delete[] UnicodeStr;
#else
		m_str << pStr << _T( "=" );
#endif
	}


	/* ���һ�����͵ı��� */
	template <class T> void add( T t )
	{
		m_str << t << _T( "\t" );
	}


	/* ���point */
	void add( POINT point )
	{
		m_str << (_T( "(" ) ) << point.x << (_T( "," ) ) << point.y << (_T( ")\t" ) );
	}


	/* ���rect */
	void add( RECT rect )
	{
		m_str << (_T( "(" ) ) << rect.left << (_T( "," ) ) << rect.top << (_T( "," ) ) << rect.right << (_T( "," ) ) << rect.bottom << (_T( ")-(" ) ) << (rect.right - rect.left) << (_T( "," ) ) << (rect.bottom - rect.top) << (_T( ")\t" ) );
	}


	/* ���������Ϣ */
	void add( HWND hWnd )
	{
		if ( !::IsWindow( hWnd ) )
		{
			m_str << _T( "Invalid Window" ); return;
		}

		TCHAR	WindowText[40]; ::SendMessage( hWnd, WM_GETTEXT, (WPARAM) sizeof(WindowText) / sizeof(TCHAR), (LPARAM) WindowText );
		TCHAR	ClassName[40]; ::GetClassName( hWnd, ClassName, sizeof(ClassName) / sizeof(TCHAR) );
		RECT	rect;  ::GetWindowRect( hWnd, &rect );
		m_str	<< _T( "��������������������������" )
			<< _T( "\r\n\tWindow Text:  \"" ) << WindowText
			<< _T( "\"\r\n\tClass Name:  \"" ) << ClassName
			<< _T( "\"\r\n\tHandle:  0x" ) << HWND( hWnd )
			<< _T( "\r\n\trect:\t" );
		add( rect );
		m_str << _T( "\r\n\t��������������������������������" );
	}


	/* ���ϵͳ������� */
	void TraceSysError()
	{
		m_str << _T( "System Error Codes:" ) << GetLastError();
	}


	/* �������ִ�еĽ�� */
	void TraceResult( char* pStr, BOOL IsOk )
	{
#ifdef _UNICODE
		wchar_t* UnicodeStr = AnsiToUnicode( pStr );
		m_str << UnicodeStr;
		delete[] UnicodeStr;
#else
		m_str << pStr;
#endif
		m_str << _T( " : " ) << (IsOk == TRUE ? _T( " Successful(true)" ) : _T( "Failed(false)" ) );
	}


	/* ���MFC�� CPoint CRect CString */
#ifdef __AFXWIN_H__
	void add( CPoint point )
	{
		add( (POINT) point );
	}


	void add( CRect rect )
	{
		add( (RECT) rect );
	}


	void add( CString str )
	{
		add( (LPTSTR) (LPCTSTR) str );
	}


#endif
	/* ansi ת unicode ������Ҫɾ��ָ�� */
	wchar_t* AnsiToUnicode( char* AnsiStr )
	{
		UINT	len		= strlen( AnsiStr );
		wchar_t * UnicodeStr	= new wchar_t[len + 1];
		setlocale( LC_ALL, ".936" );
		int num = mbstowcs( UnicodeStr, AnsiStr, len + 1 );
		setlocale( LC_ALL, "C" );
		if ( -1 == num )
			*UnicodeStr = NULL;
		return(UnicodeStr);
	}


	/* �����ʽ���ַ���������sprintf */
	static void Tracef( LPCTSTR lpszFormat, ... )
	{
		va_list args; va_start( args, lpszFormat );     /* begin */
		TCHAR	szBuffer[1024];
		int	rel = _vsntprintf( szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1, lpszFormat, args );
		if ( rel == -1 )
			szBuffer[sizeof(szBuffer) / sizeof(TCHAR) - 1] = NULL;
		va_end( args );                                 /* end */
		CEasyTrace().add( szBuffer );                   /* ����ַ��� */
	}
};

#endif /* #define TRACE_WINDOW */
#endif /* #define _EASY_TRACE_H_ */

#endif // trace_h__