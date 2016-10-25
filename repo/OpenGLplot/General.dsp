# Microsoft Developer Studio Project File - Name="General" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=General - Win32 Deu Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "General.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "General.mak" CFG="General - Win32 Deu Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "General - Win32 Deu Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "General - Win32 Deu Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "General - Win32 Enu Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "General - Win32 Enu Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "General - Win32 Deu Release"

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
F90=df.exe
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "_GENERALDLL" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /fo"Release\GeneralDeu.res" /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_DEU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 version.lib opengl32.lib glu32.lib /nologo /subsystem:windows /dll /pdb:"Release\General32Deu.pdb" /machine:I386 /out:"..\..\Release Deu\General32.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "General - Win32 Deu Debug"

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
F90=df.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "_GENERALDLL" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /fo"Debug\GeneralDeu.res" /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_DEU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib opengl32.lib glu32.lib /nologo /subsystem:windows /dll /pdb:"Debug\General32Deu.pdb" /debug /machine:I386 /out:"..\..\Debug Deu\General32d.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "General - Win32 Enu Release"

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
F90=df.exe
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "_GENERALDLL" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Release\GeneralEnu.res" /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 version.lib opengl32.lib glu32.lib /nologo /subsystem:windows /dll /pdb:"Release\General32Enu.pdb" /machine:I386 /out:"..\..\Release Enu\General32.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "General - Win32 Enu Debug"

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
F90=df.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "_GENERALDLL" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Debug\GeneralEnu.res" /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib opengl32.lib glu32.lib /nologo /subsystem:windows /dll /pdb:"Debug\General32Enu.pdb" /debug /machine:I386 /out:"..\..\Debug Enu\General32d.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "General - Win32 Deu Release"
# Name "General - Win32 Deu Debug"
# Name "General - Win32 Enu Release"
# Name "General - Win32 Enu Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcInThreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorDialogEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandLineInfoEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DdxDdvSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTts.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFilePrintOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOpenGLInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DocMgrEx.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialogEx.cpp
# End Source File
# Begin Source File

SOURCE=.\FormPropertiesView.cpp
# End Source File
# Begin Source File

SOURCE=.\FormViewTts.cpp
# End Source File
# Begin Source File

SOURCE=.\General.cpp
# End Source File
# Begin Source File

SOURCE=.\General.rc
# End Source File
# Begin Source File

SOURCE=.\General32.def

!IF  "$(CFG)" == "General - Win32 Deu Release"

!ELSEIF  "$(CFG)" == "General - Win32 Deu Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "General - Win32 Enu Release"

!ELSEIF  "$(CFG)" == "General - Win32 Enu Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\General32d.def

!IF  "$(CFG)" == "General - Win32 Deu Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "General - Win32 Deu Debug"

!ELSEIF  "$(CFG)" == "General - Win32 Enu Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "General - Win32 Enu Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GlDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\GlView.cpp
# End Source File
# Begin Source File

SOURCE=.\GlWinApp.cpp
# End Source File
# Begin Source File

SOURCE=.\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IntMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ListViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MathAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageTts.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertySheetAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\PsRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Scale.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterWndEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticExtra.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Version.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchInThreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchInThreadExtra.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchThread.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\CalcInThreadBase.h
# End Source File
# Begin Source File

SOURCE=.\ClientFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColorDialogEx.h
# End Source File
# Begin Source File

SOURCE=.\CommandLineInfoEx.h
# End Source File
# Begin Source File

SOURCE=.\DdxDdvSupport.h
# End Source File
# Begin Source File

SOURCE=.\DialogTts.h
# End Source File
# Begin Source File

SOURCE=.\DlgAbout.h
# End Source File
# Begin Source File

SOURCE=.\DlgFilePrintOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgOpenGLInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgProperties.h
# End Source File
# Begin Source File

SOURCE=.\DocMgrEx.h
# End Source File
# Begin Source File

SOURCE=.\FileDialogEx.h
# End Source File
# Begin Source File

SOURCE=.\FormPropertiesView.h
# End Source File
# Begin Source File

SOURCE=.\FormViewTts.h
# End Source File
# Begin Source File

SOURCE=.\General.h
# End Source File
# Begin Source File

SOURCE=.\GlDocument.h
# End Source File
# Begin Source File

SOURCE=.\GlView.h
# End Source File
# Begin Source File

SOURCE=.\GlWinApp.h
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IntMatrix.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxEx.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\ListViewEx.h
# End Source File
# Begin Source File

SOURCE=.\MathAdd.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAdjust.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageTts.h
# End Source File
# Begin Source File

SOURCE=.\PropertySheetAdjust.h
# End Source File
# Begin Source File

SOURCE=.\PsRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.hm
# End Source File
# Begin Source File

SOURCE=.\Scale.h
# End Source File
# Begin Source File

SOURCE=.\SplitterWndEx.h
# End Source File
# Begin Source File

SOURCE=.\StaticExtra.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\WatchInThreadBase.h
# End Source File
# Begin Source File

SOURCE=.\WatchInThreadExtra.h
# End Source File
# End Group
# Begin Group "Resourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CursorDblCross.cur
# End Source File
# Begin Source File

SOURCE=.\res\CursorHand.cur
# End Source File
# Begin Source File

SOURCE=.\res\General.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=".\Was ist neu.txt"
# End Source File
# Begin Source File

SOURCE=".\What's new.txt"
# End Source File
# End Target
# End Project
