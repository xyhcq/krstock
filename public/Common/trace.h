#ifndef trace_h__
#define trace_h__


/*==================trace 输出调试字符串====================
 * (一)功能：
 * 输出调试变量(类似于TRACE)
 * (二)特点:
 * 1.自动适应参数的类型(char、char*、string、CString、HWND、POINT、RECT....)
 * 2.自动适应输入参数的个数。(关闭了编译提醒 #pragma warning(disable: 4003) )
 * 3.会在输出的变量值前面自动添加变量的名称，方便查看
 * 4.程序自动创建Edit窗口用于输出转换后的字符串。
 * 5.程序退出时会将输出字符串保存到工程目录下的DebugData.txt。方便查看
 * 6.多种编译模式，比如可让DEBUG和Release版本都能输出调试变量或者两者都不输出
 * 7.支持UNICODE,WIN32,Dll,MFC

 * (三)使用说明：
 * 1.把trace.h复制到工程目录下（可以不添加到工程）。
 * 2.在文件"stdafx.h"里（文件的下方）添加 #include "trace.h"。之后就可以使用trace()宏了。
 * 3.所有输出的字符串会保存在工程目录下的"DebugData.txt"文件，以方便查看
 * 4.我把所有代码都放在一个头文件里，虽然不合符规范，但这样使用起来很方便。
 * 5.trace(x,y,z,w)宏原本有4个参数，当参数不如时，编译器会给出警告，
 * 所以使用 #pragma warning(disable: 4003) 把这个编译警告给关掉了。

 * (四)可以使用的宏：
 * trace(X,Y,Z,W) //输出常用类型变量，如int、double、short、POINT、RECT、string
 * //且自动适应变量的个数（变量数为1-4个）
 * tracef() //格式化字符串，类似sprintf
 * traceLastError()//输出系统错误代码，调用了GetLastError()
 * traceRel(X,Y)  //当X=true，输出"Y: successful" ; x=false，输出"Y: failed"
 * CEasyTrace::clear();  //清空窗口
 *

 * (五)关于trace宏使能设置：
 * 1.默认情况下 NO_TRACE_WINDOW 和 TRACE_WINDOW都没定义，则
 * DEBUG版本会输出调试字符串，而Release版本不会
 * 2.如果开头定义了#define NO_TRACE_WINDOW
 * DEBUG版本和Release版本都不会输出输出调试字符串
 * 3.如果开头定义了#define TRACE_WINDOW
 * DEBUG版本和Release版本都会输出输出调试字符串
 * 4.每次修改上面2个宏后需要全部重新编译才会生效

 * (六)调用示例：
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
 * tracef(_T("tracef 类似于 %s"),_T("sprintf")); //格式化字符串，类似sprintf
 * trace(_T("Impossible is nothing"));
 * trace(stlstr);  //输出字符串
 * trace(mfcstr);  //输出字符串
 * trace(buf);  //输出字符串
 * trace(a); //输出int
 * trace(a,b); //输出int和double
 * trace(a,b,c);
 * trace(a,b,c,d);
 * trace(point);
 * trace(point,cpoint); //输出POINT 和 CPoint
 * trace(rect);
 * trace(rect,crect); //输出RECT 和 CRect
 * trace(hWnd);  //输出窗口信息
 * traceLastError(); //输出系统错误代码
 * traceRel(ak==true,"func()");  //当X=true，输出"Y: successful" ; x=false，输出"Y: failed"
 * }

 * (七)技巧：
 * 1.有时候我们只想观察某一个变量的变化，但程序里有很多trace，所以输出到窗口的
 * 数据影响了我们的观察。这是我们可以这样做，就能令其他trace都无效了。
 * BEGINTRACE; //接收数据
 * trace(_T("只有 BEGINTRACE 和 ENDTRACE 之间的trace才有效"));
 * ENDTRACE; //禁止接收数据
 *
 * 版权声明:
 * 没有版权，许可任何单位,个人随意使用,拷贝,修改，但作者不承担由
 * 此代码带来的任何损失。由于作者水平有限，错误或不完善之处，在
 * 所难免，由此为你带来的不便，还望海涵。如果有任何BUG，请联系作者，
 * 大家一起完善它！以后有更新统一放在以下网址上，你可以浏览并获得
 * 更新以及详细的使用说明：
 * 当前更新日期：2011-10-21
 * http://blog.csdn.net/jacky_qiu/archive/2010/11/04/5986089.aspx
 * 最后:
 * 如果此份代码为你带来了帮助，并且使你心生感谢之意。那我可否请你
 * 诚心恭敬地念10遍“南(音:拿na)无(音:摩mo)阿弥陀佛”？
 * 愿得佛力加持，使你工作顺利，合家幸福！
 *
 *    Jacky
 *    2011-11-7
 *    qiujiejia@gmail.com
 * ========================================================================*/


#ifndef _EASY_TRACE_H_
#define _EASY_TRACE_H_

/*
 * trace宏编译开关━━━━━━━━━
 *  #define NO_TRACE_WINDOW //DEBUG版本和Release版本都不会输出输出调试字符串
 *  #define TRACE_WINDOW //DEBUG版本和Release版本都会输出输出调试字符串
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
 * trace宏的参数是4个，当参数少于4个，编译器会给出警告，我们手动关闭这个
 * 警告(disable warning C4003: not enough actual parameters for macro)
*/

#pragma warning(disable: 4003)
#pragma warning(disable: 4002)

#define TRACE_WND_TEXT	_T( "TraceWnd" )        /* 窗口标题 */
#define AUTO_HIDE_TIME	3000                    /* 定义多少毫秒后自动窗口 */
#include <Windows.h>                            /* 需要的头文件 */
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
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━trace宏━━━━ */
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
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

class CTraceWnd
{
	struct TRACEWNDCONFIG
	{
		int	x;
		int	y;
		int	cx;
		int	cy;
		BOOL	IsContinue;     /* 当输出字符串时是否清空之前的数据 */
		BOOL	IsAutoWidth;    /* 是否根据字符串长度自动调整窗口宽度 */
		BOOL	IsAutoHide;     /* 是否自动隐藏 */
		BOOL	IsHide;         /* 是否隐藏 */
		BOOL	IsName;         /* 是否自动补上变量名 */
		BOOL	IsIndex;        /* 添加行号索引 */
		BOOL	IsSplitter;     /* 2条字符串输入时间差大于1秒时添加分隔符 */
		BOOL	IsDeleteBefore; /* 2条字符串输入时间差大于1秒时删除前面的所有输出字符串 */
		BOOL	IsReceive;      /* 是否接受数据 */
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
	*  行首序列号
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
		_itot( IndexNum()++, SrcIndex, 10 );    /* int 转文本 */
		int	len	= _tcslen( SrcIndex );
		int	count	= _INDEX_NUM_MAX - len; /* 4 - 对齐的位数 */
		if ( count > 0 )
		{
			for ( int i = count, j = 0; i < _INDEX_NUM_MAX; i++, j++ )
				NewIndex[i] = SrcIndex[j];
			return(NewIndex);
		}else
			return(SrcIndex);
	}


	/****************************************************************************
	*  输出的字符串到窗口
	****************************************************************************/
	static void CTraceWnd::PrintString( const TCHAR* OutputStr )
	{
		if ( SaveEditWnd() == NULL )
		{
			/* 寻找已创建的输出窗口 */
			HWND MainWnd = ::FindWindow( NULL, TRACE_WND_TEXT );
			/* 如果不存在，则创建窗口并初始化设置 */
			if ( !MainWnd )
			{
				/*
				 * 定义这个对象是为了程序退出是调用析构函数来
				 * 保存文件和关闭窗口，除此之外没有其他用处了
				 */
				static CTraceWnd ForGetCloseMessage;
				/* 创建窗口 */
				MainWnd		= CreateMainWnd();
				SaveEditWnd()	= CreateEditWnd( MainWnd );
				if ( !MainWnd )
				{
					::MessageBox( ::GetFocus(), _T( "无法创建trace窗口" ), NULL, MB_OK );
					return;
				}
			}
		}
		if ( CTraceWnd::ConfigData().IsReceive == false )
			return;
		/* 将字符串输出到窗口 */
		::SendMessage( SaveEditWnd(), EM_REPLACESEL, FALSE, (LPARAM) OutputStr );
	}


	/****************************************************************************
	*  //保存窗口配置
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
		/* 把hWnd的默认窗口过程替换为WindowProc,返回默认函数过程的函数指针 */
		WNDPROC OldWndProc = (WNDPROC) SetWindowLongPtr( EditWnd, GWLP_WNDPROC, (LONG_PTR) EditWindowProc );
		ASSERT( OldWndProc );
		/* 保存OldWndProc */
		::SetWindowLongPtr( EditWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(OldWndProc) );

		/* 修改edit框的限制字符个数 */
		::SendMessage( EditWnd, EM_SETLIMITTEXT, 1000000, 0 );
		/* 清空Edit */
		::SendMessage( EditWnd, WM_SETTEXT, NULL, (LPARAM) _T( "" ) );
		/* 设置字体参数 */
		LOGFONT LogFont;
		::memset( &LogFont, 0, sizeof(LOGFONT) );
		lstrcpy( LogFont.lfFaceName, _T( "Fixedsys" ) );
		LogFont.lfHeight	= -12; /* 字体大小 */
		LogFont.lfCharSet	= GB2312_CHARSET;
		/* 创建字体 */
		HFONT hFont = CreateFontIndirect( &LogFont );
		/* 设置字体 */
		::SendMessage( EditWnd, WM_SETFONT, (WPARAM) hFont, 0 );
		::PostMessage( EditWnd, WM_RBUTTONDOWN, NULL, MAKELPARAM( -10000, -10000 ) );
		return(EditWnd);
	}


	static HWND CreateMainWnd()
	{
		TRACEWNDCONFIG & config = ConfigData();
		GetConfig( config );
		config.IsReceive = true;

		/* 超出屏幕重新调整 */
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

		/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
		HWND hWnd;
		if ( config.IsHide ) /* 如果窗口是隐藏状态 */
		{
			hWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, _T( "#32770" ), TRACE_WND_TEXT,
					       WS_OVERLAPPEDWINDOW,
					       -100, 0, 104, GetSystemMetrics( SM_CYSCREEN ), NULL, NULL, NULL, NULL );
		}else  {
			hWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, _T( "#32770" ), TRACE_WND_TEXT,
					       WS_OVERLAPPEDWINDOW,
					       config.x, config.y, config.cx, config.cy, NULL, NULL, NULL, NULL );
		}
		::ShowWindow( hWnd, SW_SHOWNOACTIVATE ); /* 显示时无焦点 */
		/* ASSERT(hWnd); */
		if ( hWnd == NULL )
			return(NULL);
		/* 把hWnd的默认窗口过程替换为WindowProc,返回默认函数过程的函数指针 */
		WNDPROC OldWndProc = (WNDPROC) SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR) MainWindowProc );
		ASSERT( OldWndProc );
		/* 保存OldWndProc */
		::SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(OldWndProc) );
		/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

		/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━创建菜单 */
		HMENU hMenu = CreateMenu();
		AppendMenu( hMenu, MF_STRING, 5, _T( "隐藏到屏幕左边" ) );
		AppendMenu( hMenu, MF_STRING, 3, _T( "清空窗口" ) );
		/* -----------------sub menu */
		HMENU hSubMenu = CreateMenu();
		AppendMenu( hSubMenu, MF_STRING, 6, _T( "自动隐藏" ) );
		AppendMenu( hSubMenu, MF_STRING, 7, _T( "接受数据" ) );
		AppendMenu( hSubMenu, MF_STRING, 4, _T( "根据字符串长度自动调整窗口宽度" ) );
		AppendMenu( hSubMenu, MF_STRING, 9, _T( "连续输出数据" ) );
		AppendMenu( hSubMenu, MF_STRING, 10, _T( "输出时自动添加变量名" ) );
		AppendMenu( hSubMenu, MF_STRING, 11, _T( "输出时添加行号索引" ) );
		AppendMenu( hSubMenu, MF_STRING, 12, _T( "2条字符串输入时间差大于1秒时添加分隔符" ) );
		AppendMenu( hSubMenu, MF_STRING, 13, _T( "2条字符串输入时间差大于1秒时删除前面的所有输出字符串" ) );
		AppendMenu( hSubMenu, MF_STRING, 8, _T( "帮助" ) );
		/* -----------------sub menu */
		AppendMenu( hMenu, MF_POPUP, UINT( hSubMenu ), _T( "选项" ) );
		::SetMenu( hWnd, hMenu );
		/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
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
				ShowTraceWnd(); /* 重新显示窗口 */
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
	*  Edit Window的窗口过程
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
 * if (LOWORD(wParam)==WA_INACTIVE ) //窗口失去焦点
 * {
 * if (config.IsAutoHide)
 * {
 * ::SetTimer(hWnd,1,200,NULL); //延长200ms后hide
 * }
 * }
 * }
 * break;
*/
		case WM_RBUTTONDOWN:
		{
			if ( lParam == MAKELPARAM( -10000, -10000 ) )
			{
				/* 如果没有开启自动隐藏并且又处于隐藏状态，则弹出提示窗口 */
				if ( !config.IsAutoHide && config.IsHide )
				{
					StaticWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST & ~WS_EX_APPWINDOW, /* 取消任务栏标题 */
								    _T( "Static" ),
								    _T( "Trace窗口隐藏在屏幕左边\n点击屏幕左边可恢复窗口" ),
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
			if ( wParam == 2 ) /* 关闭提示窗口 */
			{
				::DestroyWindow( StaticWnd );
				::KillTimer( hWnd, 2 );
				break;
			}
			if ( !config.IsHide && ::GetForegroundWindow() != GetParent( hWnd ) )
			{
				HideTraceWnd(); /* 隐藏窗口 */
			}
		}
		break;

		case WM_COMMAND:
		{
			switch ( wParam )
			{
			case 3:         /* 清空内容 */
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
				HideTraceWnd(); /* 隐藏窗口 */
				/* 隐藏窗口后要禁止自动隐藏 */
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
					PrintString( _T( "\r\n注意：当前处于禁止接收数据状态\r\n" ) );
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
				/* 光标指向最后 */
				::SendMessage( hWnd, EM_SETSEL, 4294967290, 4294967290 );
				WNDPROC OldWndProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
				if ( config.IsSplitter )
				{
					static DWORD	begin;
					DWORD		end = GetTickCount();
					if ( end - begin > 1000 )
					{
						TCHAR	fen[]		= _T( "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\r\n" );
						TCHAR	* outputstr	= (TCHAR *) lParam;
						int	len		= _tcslen( outputstr );
						TCHAR	* p		= new TCHAR[len + sizeof(fen) / sizeof(TCHAR)];
						_tcscpy( p, fen );
						_tcscpy( p + sizeof(fen) / sizeof(TCHAR) - 1, outputstr );
						/* 调用默认过程添加字符串到edit 窗口 */
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


			/* 开启了自动隐藏功能 */
			if ( config.IsAutoHide )
			{
				if ( config.IsHide )
				{
					ShowTraceWnd(); /* 重新显示窗口 */
				}
				/* 设置AUTO_HIDE_TIME后隐藏窗口 */
				::SetTimer( hWnd, 1, AUTO_HIDE_TIME, NULL );
			}

			/* 开启了自动调整窗口的宽度功能 */
			if ( config.IsAutoWidth )
			{
				if ( !config.IsHide )
				{
					static int	m_WindowWidth = 200; /* 窗口宽度 */
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
			/*删除字体 */
			DeleteObject( (HFONT) ::SendMessage( hWnd, WM_GETFONT, 0, 0 ) );
			/*
			 * 设置程序运行的目录跟程序文件所在目录相同
			 * 当我们的程序被其他程序打开时，它的工作目录和打开它的那。
			 * 个程序的目录是相同的。所以我们需要把目录改回来
			 */
			TCHAR	AppPath[MAX_PATH];
			int	nlen = GetModuleFileName( NULL, AppPath, MAX_PATH );
			while ( AppPath[--nlen] != '\\' )
				;
			AppPath[nlen] = '\0';
			SetCurrentDirectory( AppPath );

			/* 获取窗口的字符串 */
			int	len		= ::SendMessage( hWnd, WM_GETTEXTLENGTH, 0, 0 );
			TCHAR	* WindowText	= new TCHAR[(len + 1) * sizeof(TCHAR)];
			::GetWindowText( hWnd, WindowText, len + 1 );   /* GetWindowText会自动添加NULL结束符 */

			/* 打开由于保存数据文件DebugData.txt, */
			FILE* fp = fopen( "DebugData.txt", "wb" );

#ifdef _UNICODE    /* 如果定义了unicode，则先转为ansi再保存 */
			/*
			 * len是源字符串的长度 len=_tcslen(WindowText)
			 * (len+1)*2是源字符串占用的内存空间，包括NULL结束符
			 * nChars返回转换的字符个数，不包括NULL，既nChars=_tcslen(buffer)
			 * wcstombs会自动添加NULL结束符
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
	}       /* 当trace宏参数不足时会调用此函数 */


	void name()
	{
	}       /* 当trace宏参数不足时会调用此函数 */


	/* 初始化一个ostringstream对象并且添加序列号 */
	CEasyTrace()
	{
		if ( CTraceWnd::ConfigData().IsIndex )
			m_str << CTraceWnd::GetIndex() << _T( "  " );
		else
			m_str << _T( "  " );
	}


	/* 析构时将字符串输出到Edit窗口 */
	~CEasyTrace()
	{
		m_str << _T( "\r\n" ); /* 输出“换行” */
#ifdef _UNICODE
		CTraceWnd::PrintString( std::wstring( m_str.str() ).c_str() );
#else
		CTraceWnd::PrintString( std::string( m_str.str() ).c_str() );
#endif
	}


	/* 输出变量名 */
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


	/* 输出一般类型的变量 */
	template <class T> void add( T t )
	{
		m_str << t << _T( "\t" );
	}


	/* 输出point */
	void add( POINT point )
	{
		m_str << (_T( "(" ) ) << point.x << (_T( "," ) ) << point.y << (_T( ")\t" ) );
	}


	/* 输出rect */
	void add( RECT rect )
	{
		m_str << (_T( "(" ) ) << rect.left << (_T( "," ) ) << rect.top << (_T( "," ) ) << rect.right << (_T( "," ) ) << rect.bottom << (_T( ")-(" ) ) << (rect.right - rect.left) << (_T( "," ) ) << (rect.bottom - rect.top) << (_T( ")\t" ) );
	}


	/* 输出窗口信息 */
	void add( HWND hWnd )
	{
		if ( !::IsWindow( hWnd ) )
		{
			m_str << _T( "Invalid Window" ); return;
		}

		TCHAR	WindowText[40]; ::SendMessage( hWnd, WM_GETTEXT, (WPARAM) sizeof(WindowText) / sizeof(TCHAR), (LPARAM) WindowText );
		TCHAR	ClassName[40]; ::GetClassName( hWnd, ClassName, sizeof(ClassName) / sizeof(TCHAR) );
		RECT	rect;  ::GetWindowRect( hWnd, &rect );
		m_str	<< _T( "━━━━━━━━━━━━━" )
			<< _T( "\r\n\tWindow Text:  \"" ) << WindowText
			<< _T( "\"\r\n\tClass Name:  \"" ) << ClassName
			<< _T( "\"\r\n\tHandle:  0x" ) << HWND( hWnd )
			<< _T( "\r\n\trect:\t" );
		add( rect );
		m_str << _T( "\r\n\t━━━━━━━━━━━━━━━━" );
	}


	/* 输出系统错误代码 */
	void TraceSysError()
	{
		m_str << _T( "System Error Codes:" ) << GetLastError();
	}


	/* 输出函数执行的结果 */
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


	/* 输出MFC的 CPoint CRect CString */
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
	/* ansi 转 unicode ，用完要删除指针 */
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


	/* 输出格式化字符串，类似sprintf */
	static void Tracef( LPCTSTR lpszFormat, ... )
	{
		va_list args; va_start( args, lpszFormat );     /* begin */
		TCHAR	szBuffer[1024];
		int	rel = _vsntprintf( szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1, lpszFormat, args );
		if ( rel == -1 )
			szBuffer[sizeof(szBuffer) / sizeof(TCHAR) - 1] = NULL;
		va_end( args );                                 /* end */
		CEasyTrace().add( szBuffer );                   /* 输出字符串 */
	}
};

#endif /* #define TRACE_WINDOW */
#endif /* #define _EASY_TRACE_H_ */

#endif // trace_h__