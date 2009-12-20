# Microsoft Developer Studio Project File - Name="advlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=advlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "advlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "advlib.mak" CFG="advlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "advlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "advlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "advlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\inc" /I "..\regexp" /I "..\libini\iniparser3.0b\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Custom Build
InputPath=.\Release\advlib.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy         ..\advlib.h         ..\..\..\..\inc\  
	copy         $(InputPath)         ..\..\..\vc-x86\  
	
# End Custom Build

!ELSEIF  "$(CFG)" == "advlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\inc" /I "..\regexp" /I "..\libini\iniparser3.0b\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Custom Build
InputPath=.\Debug\advlib.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy         ..\advlib.h         ..\..\..\..\inc\  
	copy         $(InputPath)         ..\..\..\vc-x86\  
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "advlib - Win32 Release"
# Name "advlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "advkey"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\adv_key\adv_key.c
# End Source File
# Begin Source File

SOURCE=..\adv_key\adv_key.h
# End Source File
# End Group
# Begin Group "config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\config\platform_config.c
# End Source File
# Begin Source File

SOURCE=..\config\platform_config.H
# End Source File
# End Group
# Begin Group "regexp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\regexp\regerror.c
# End Source File
# Begin Source File

SOURCE=..\regexp\regexp.c
# End Source File
# Begin Source File

SOURCE=..\regexp\regexp.h
# End Source File
# Begin Source File

SOURCE=..\regexp\regmagic.h
# End Source File
# End Group
# Begin Group "file_list"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\file_list\file_list.c
# End Source File
# Begin Source File

SOURCE=..\file_list\file_list.h
# End Source File
# End Group
# Begin Group "file_explorer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\file_explorer\file_explorer.c
# End Source File
# Begin Source File

SOURCE=..\file_explorer\file_explorer.h
# End Source File
# End Group
# Begin Group "libini"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libini\iniparser3.0b\src\dictionary.c
# End Source File
# Begin Source File

SOURCE=..\libini\iniparser3.0b\src\dictionary.h
# End Source File
# Begin Source File

SOURCE=..\libini\iniparser3.0b\src\iniparser.c
# End Source File
# Begin Source File

SOURCE=..\libini\iniparser3.0b\src\iniparser.h
# End Source File
# Begin Source File

SOURCE=..\libini\platform_libini.c
# End Source File
# Begin Source File

SOURCE=..\libini\platform_libini.h
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
