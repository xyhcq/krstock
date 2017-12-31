# Microsoft Developer Studio Project File - Name="KRStockOS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KRStockOS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KRStockOS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KRStockOS.mak" CFG="KRStockOS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KRStockOS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KRStockOS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KRStockOS - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../public" /I "StockWidget" /I "THSWebHQ" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /pdb:"../ClinetBin/KRStockOS.pdb" /debug /machine:I386 /out:"../ClinetBin/KRStockOS.exe" /libpath:"../Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "KRStockOS - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../public" /I "StockWidget" /I "THSWebHQ" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../ClinetBin/KRStockOS.exe" /pdbtype:sept /libpath:"../Lib"

!ENDIF 

# Begin Target

# Name "KRStockOS - Win32 Release"
# Name "KRStockOS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BootDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HQReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyDemonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KRStockOS.cpp
# End Source File
# Begin Source File

SOURCE=.\KRStockOS.rc
# End Source File
# Begin Source File

SOURCE=.\KRStockOSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Style.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BootDlg.h
# End Source File
# Begin Source File

SOURCE=.\FilterSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\HQReportDlg.h
# End Source File
# Begin Source File

SOURCE=.\KeyDemonDlg.h
# End Source File
# Begin Source File

SOURCE=.\KRStockOS.h
# End Source File
# Begin Source File

SOURCE=.\KRStockOSDlg.h
# End Source File
# Begin Source File

SOURCE=.\Render.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Style.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\KRStockOS.ico
# End Source File
# Begin Source File

SOURCE=.\res\KRStockOS.rc2
# End Source File
# End Group
# Begin Group "Widget"

# PROP Default_Filter ""
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

SOURCE=..\public\Widget\SplitterColWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SplitterColWidget.h
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SplitterRowWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SplitterRowWidget.h
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SystemButton.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Widget\SystemButton.h
# End Source File
# End Group
# Begin Group "ComHeader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\public\ComHeader\ComHeader.h
# End Source File
# End Group
# Begin Group "StockWidget"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StockWidget\AmountAnalyzeWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\AmountAnalyzeWidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\BTWidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\FenBiWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\FenBiWidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\HQFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\HQFunction.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\HQWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\HQWidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\IndexWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\IndexWidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\KLineWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\klinewidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\KRWave.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\KRWave.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\MSortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\MSortListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\StockAlgorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\StockAlgorithm.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\StockListWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\StockListWidget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\StockToolsWiget.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\StockToolsWiget.h
# End Source File
# Begin Source File

SOURCE=.\StockWidget\TextButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StockWidget\TextButton.h
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
# Begin Source File

SOURCE=..\public\Common\TopLevelExceptionFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\public\Common\TopLevelExceptionFilter.h
# End Source File
# End Group
# Begin Group "DataMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataMgr\ZXGDataMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\5WaveDonw.PNG
# End Source File
# Begin Source File

SOURCE=.\res\5WaveDown.PNG
# End Source File
# Begin Source File

SOURCE=.\res\BootBG.png
# End Source File
# Begin Source File

SOURCE=.\res\OneBuy0.PNG
# End Source File
# Begin Source File

SOURCE=.\res\OneBuy1.PNG
# End Source File
# Begin Source File

SOURCE=.\res\OneBuy2.PNG
# End Source File
# Begin Source File

SOURCE=.\res\OneBuy3.PNG
# End Source File
# Begin Source File

SOURCE=.\res\OneBuy4.PNG
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section KRStockOS : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section KRStockOS : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
