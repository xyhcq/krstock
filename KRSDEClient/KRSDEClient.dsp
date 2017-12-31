# Microsoft Developer Studio Project File - Name="KRSDEClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=KRSDEClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KRSDEClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KRSDEClient.mak" CFG="KRSDEClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KRSDEClient - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "KRSDEClient - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KRSDEClient - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../public" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:"../ClinetBin/KRSDEClient.pdb" /debug /machine:I386 /out:"../ClinetBin/KRSDEClient.dll" /libpath:"../Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "KRSDEClient - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../public" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../ClinetBin/KRSDEClient.dll" /pdbtype:sept /libpath:"../Lib"

!ENDIF 

# Begin Target

# Name "KRSDEClient - Win32 Release"
# Name "KRSDEClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\KRSDEClient.cpp
# End Source File
# Begin Source File

SOURCE=.\KRSDEClient.def
# End Source File
# Begin Source File

SOURCE=.\KRSDEClient.rc
# End Source File
# Begin Source File

SOURCE=.\KRSDEClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\KRSDEClient.h
# End Source File
# Begin Source File

SOURCE=.\KRSDEClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\KRSDEClient.rc2
# End Source File
# End Group
# Begin Group "Widget"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\Widget\KRProgressWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\KRProgressWidget.h
# End Source File
# Begin Source File

SOURCE=..\public\Widget\KRScrollVbar.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\KRScrollVbar.h
# End Source File
# Begin Source File

SOURCE=..\public\Widget\KRTextListWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\KRTextListWidget.h
# End Source File
# Begin Source File

SOURCE=..\public\Widget\MemoryDC.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\MemoryDC.h
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SystemButton.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SystemButton.h
# End Source File
# End Group
# Begin Group "HPClient"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\HPClient\HPClient.cpp
# End Source File
# Begin Source File

SOURCE=..\public\HPClient\HPClient.h
# End Source File
# End Group
# Begin Group "HPSocket"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\HPSocket\HPSocket4C.h
# End Source File
# Begin Source File

SOURCE=..\public\HPSocket\SocketDataMgr.h
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\public\zlib\zlib.h
# End Source File
# End Group
# Begin Group "StockMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\StockMgr\ChuQuanDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\ClientKLineDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\DirectoryMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\DirectoryMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\FenBiDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\FinancialDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\HangYeDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\HQBaseInfoMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\KLineDataMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\KLineDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\RTHQDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\StockCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\StockCommon.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\TickerSymbolDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\public\StockMgr\ZSDataMgr.h
# End Source File
# End Group
# Begin Group "ComHeader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\ComHeader\ComHeader.h
# End Source File
# Begin Source File

SOURCE=..\public\STKDRV\Stockdrv.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\Common\Common.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Common\Common.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
