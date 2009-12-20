# Microsoft Developer Studio Project File - Name="font_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=font_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "font_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "font_lib.mak" CFG="font_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "font_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "font_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "font_lib - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\freetype-2.3.8\include" /I "..\\" /I "..\..\..\..\inc" /I "..\libbmf" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "FT2_BUILD_LIBRARY" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"font.lib"
# Begin Custom Build
InputPath=.\font.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\..\..\vc-x86 
	copy ..\font.h ..\..\..\..\inc 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "font_lib - Win32 Debug"

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
# ADD CPP /nologo /Za /W3 /Gm /GX /ZI /Od /I "..\freetype-2.3.8\include" /I "..\\" /I "..\..\..\..\inc" /I "..\libbmf" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "FT2_BUILD_LIBRARY" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"font.lib"
# Begin Custom Build
InputPath=.\font.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\..\..\vc-x86 
	copy ..\font.h ..\..\..\..\inc 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "font_lib - Win32 Release"
# Name "font_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "freetype2"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\freetype-2.3.8\src\autofit\autofit.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\bdf\bdf.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\cff\cff.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftbase.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftbbox.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftbdf.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftbitmap.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\cache\ftcache.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftdebug.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftgasp.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftglyph.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftgxval.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\gzip\ftgzip.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftinit.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\lzw\ftlzw.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftmm.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftotval.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftpfr.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftstroke.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftsynth.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftsystem.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\fttype1.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftwinfnt.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\base\ftxf86.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\pcf\pcf.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\pfr\pfr.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\psaux\psaux.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\pshinter\pshinter.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\psnames\psmodule.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\raster\raster.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\sfnt\sfnt.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\smooth\smooth.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\truetype\truetype.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\type1\type1.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\cid\type1cid.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\type42\type42.c"
# End Source File
# Begin Source File

SOURCE="..\freetype-2.3.8\src\winfonts\winfnt.c"
# End Source File
# End Group
# Begin Group "hzk16"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Hzk16\font_hzk.c
# End Source File
# End Group
# Begin Group "libbmf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libbmf\ccbmf.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\font.c

!IF  "$(CFG)" == "font_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "font_lib - Win32 Debug"

# ADD CPP /I "..\..\..\..\inc\libbmf"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\font_ascii.c

!IF  "$(CFG)" == "font_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "font_lib - Win32 Debug"

# ADD CPP /I "..\..\..\..\inc\libbmf"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\font_cache.c

!IF  "$(CFG)" == "font_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "font_lib - Win32 Debug"

# ADD CPP /I "..\..\..\..\inc\libbmf"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\font_nls.c

!IF  "$(CFG)" == "font_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "font_lib - Win32 Debug"

# ADD CPP /I "..\..\..\..\inc\libbmf"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ft2.c

!IF  "$(CFG)" == "font_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "font_lib - Win32 Debug"

# ADD CPP /I "..\..\..\..\inc\libbmf"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
