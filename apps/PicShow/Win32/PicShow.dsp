# Microsoft Developer Studio Project File - Name="PicShow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PicShow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PicShow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PicShow.mak" CFG="PicShow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PicShow - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PicShow - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PicShow - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "\regexp" /I "..\regexp" /I "..\...\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../../lib/vc-x86/xgui.lib ../../lib/vc-x86/zlib.lib ../../lib/vc-x86/MultiMedia.lib ../../lib/vc-x86/Graphics.lib ../../lib/vc-x86/advlib.lib ../../lib/vc-x86/shell.res ../../lib/vc-x86/miniio.lib ../../lib/vc-x86/font.lib ../../lib/vc-x86/shell.lib Winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "PicShow - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\regexp" /I "..\...\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../lib/vc-x86/xgui.lib ../../lib/vc-x86/zlib.lib ../../lib/vc-x86/MultiMedia.lib ../../lib/vc-x86/Graphics.lib ../../lib/vc-x86/advlib.lib ../../lib/vc-x86/shell.res ../../lib/vc-x86/miniio.lib ../../lib/vc-x86/font.lib ../../lib/vc-x86/shell.lib Winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PicShow - Win32 Release"
# Name "PicShow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\bitmap_zoom.c
# End Source File
# Begin Source File

SOURCE=..\image_cache.c
# End Source File
# Begin Source File

SOURCE=..\image_zoom.c
# End Source File
# Begin Source File

SOURCE=..\PicShow.c
# End Source File
# Begin Source File

SOURCE=..\ps_config.c
# End Source File
# Begin Source File

SOURCE=..\ps_menu.c
# End Source File
# Begin Source File

SOURCE=..\xgui_keyset.c
# End Source File
# Begin Source File

SOURCE=..\xgui_menu.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE="..\..\lib\vc-x86\shell.res"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\advlib.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\font.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\Graphics.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\miniio.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\MultiMedia.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\shell.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\shell_noarg.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\xgui.lib"
# End Source File
# Begin Source File

SOURCE="..\..\lib\vc-x86\zlib.lib"
# End Source File
# End Group
# End Target
# End Project
