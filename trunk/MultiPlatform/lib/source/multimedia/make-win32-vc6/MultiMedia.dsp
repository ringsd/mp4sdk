# Microsoft Developer Studio Project File - Name="MultiMedia" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MultiMedia - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MultiMedia.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MultiMedia.mak" CFG="MultiMedia - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MultiMedia - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MultiMedia - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MultiMedia - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\inc" /I "..\common\imager" /I "..\\" /I "..\jpg\jpeg-6b" /I "..\bmp" /I "..\jpg" /I "..\ico" /I "..\png" /I "..\png\libpng-1.2.35-src" /I "..\png\zlib-1.2.3" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
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
InputPath=.\Release\MultiMedia.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\..\..\..\lib\vc-x86 
	copy ..\ico\MM_ico.h ..\..\..\..\inc 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MultiMedia - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\inc" /I "..\common\imager" /I "..\\" /I "..\jpg\jpeg-6b" /I "..\bmp" /I "..\jpg" /I "..\ico" /I "..\png" /I "..\png\libpng-1.2.35-src" /I "..\png\zlib-1.2.3" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
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
InputPath=.\Debug\MultiMedia.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\..\..\..\lib\vc-x86 
	copy ..\MM_image.h ..\..\..\..\inc 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "MultiMedia - Win32 Release"
# Name "MultiMedia - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "ICO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ico\MM_ico.c
# End Source File
# Begin Source File

SOURCE=..\ico\MM_ico.h
# End Source File
# Begin Source File

SOURCE=..\ico\MM_imager.c
# End Source File
# Begin Source File

SOURCE=..\ico\msicon.c
# End Source File
# End Group
# Begin Group "BMP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\bmp\bmp.c
# End Source File
# Begin Source File

SOURCE=..\bmp\MM_bmp.c
# End Source File
# End Group
# Begin Group "jpg"

# PROP Default_Filter ""
# Begin Group "jpeg-6b"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\jpg\jpeg-6b\cderror.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\cdjpeg.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\cdjpeg.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcapimin.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcapistd.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jccoefct.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jccolor.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcdctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jchuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jchuff.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcinit.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcmainct.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcmarker.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcmaster.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcomapi.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcparam.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcphuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcprepct.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jcsample.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jctrans.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdapimin.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdapistd.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdatadst.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdatasrc.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdcoefct.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdcolor.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdct.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jddctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdhuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdhuff.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdinput.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdmainct.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdmarker.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdmaster.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdmerge.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdphuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdpostct.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdsample.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jdtrans.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jerror.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jerror.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jfdctflt.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jfdctfst.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jfdctint.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jidctflt.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jidctfst.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jidctint.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jidctred.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jinclude.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jmemansi.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jmemmgr.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jmemsys.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jpegint.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jpeglib.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jquant1.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jquant2.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jutils.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\jversion.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdbmp.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdcolmap.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdgif.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdppm.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdrle.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdswitch.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\rdtarga.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\transupp.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\transupp.h"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\wrbmp.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\wrgif.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\wrppm.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\wrrle.c"
# End Source File
# Begin Source File

SOURCE="..\jpg\jpeg-6b\wrtarga.c"
# End Source File
# End Group
# Begin Source File

SOURCE=..\jpg\MM_jpg.c
# End Source File
# Begin Source File

SOURCE=..\jpg\MM_jpg.h
# End Source File
# End Group
# Begin Group "png"

# PROP Default_Filter ""
# Begin Group "libpng"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\png.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngerror.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pnggccrd.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngget.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngmem.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngpread.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngread.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngrio.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngrtran.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngrutil.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngset.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngtest.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngtrans.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngvcrd.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngwio.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngwrite.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngwtran.c"
# End Source File
# Begin Source File

SOURCE="..\png\libpng-1.2.35-src\pngwutil.c"
# End Source File
# End Group
# Begin Source File

SOURCE=..\png\MM_png.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\MM_Image.c
# End Source File
# Begin Source File

SOURCE=..\MM_Image.H
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
