# Microsoft Developer Studio Project File - Name="FFTW2dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FFTW2dll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FFTW2dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FFTW2dll.mak" CFG="FFTW2dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FFTW2dll - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FFTW2dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FFTW2dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\fftw" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "BUILD_FFTW_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Desc=Install
PostBuild_Cmds=copy Release\FFTW2dll.dll ..\Install	copy Release\FFTW2dll.lib\
 ..\Install	copy Release\FFTW2dll.exp ..\Install
# End Special Build Tool

!ELSEIF  "$(CFG)" == "FFTW2dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "..\..\fftw" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "BUILD_FFTW_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FFTW2dll - Win32 Release"
# Name "FFTW2dll - Win32 Debug"
# Begin Source File

SOURCE=..\..\fftw\config.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\executor.c
# End Source File
# Begin Source File

SOURCE=.\fftw2dll.def
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fftwnd.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_1.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_10.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_11.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_12.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_13.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_14.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_15.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_16.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_2.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_3.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_32.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_4.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_5.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_6.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_64.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_7.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_8.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fn_9.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_1.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_10.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_11.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_12.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_13.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_14.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_15.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_16.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_2.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_3.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_32.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_4.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_5.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_6.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_64.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_7.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_8.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\fni_9.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_10.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_16.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_2.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_3.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_32.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_4.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_5.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_6.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_64.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_7.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_8.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftw_9.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_10.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_16.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_2.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_3.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_32.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_4.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_5.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_6.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_64.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_7.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_8.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\ftwi_9.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\generic.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\malloc.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\planner.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\putils.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\rader.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\timer.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\twiddle.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\wisdom.c
# End Source File
# Begin Source File

SOURCE=..\..\fftw\wisdomio.c
# End Source File
# End Target
# End Project
