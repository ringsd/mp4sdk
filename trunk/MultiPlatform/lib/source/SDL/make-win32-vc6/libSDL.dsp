# Microsoft Developer Studio Project File - Name="libSDL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libSDL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libSDL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libSDL.mak" CFG="libSDL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libSDL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libSDL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libSDL - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\inc" /I "..\..\..\..\inc" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
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
InputPath=.\Release\libSDL.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy         ..\libSDL.h         ..\..\..\..\inc\  
	copy         $(InputPath)         ..\..\..\vc-x86\  
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libSDL - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\inc" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
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
InputPath=.\Debug\libSDL.lib
SOURCE="$(InputPath)"

"no" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy         ..\libSDL.h         ..\..\..\..\inc\  
	copy         $(InputPath)         ..\..\..\vc-x86\  
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "libSDL - Win32 Release"
# Name "libSDL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\audio\SDL_audio.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_audio_c.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_audiocvt.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_audiodev.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_audiodev_c.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_audiomem.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer_m68k.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer_m68k.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer_MMX.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer_MMX.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer_MMX_VC.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_mixer_MMX_VC.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\mpf\SDL_mpf_audio.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\mpf\SDL_mpf_audio.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_sysaudio.h
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_wave.c
# End Source File
# Begin Source File

SOURCE=..\src\audio\SDL_wave.h
# End Source File
# End Group
# Begin Group "cdrom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\cdrom\SDL_cdrom.c
# End Source File
# Begin Source File

SOURCE=..\src\cdrom\dummy\SDL_syscdrom.c
# End Source File
# Begin Source File

SOURCE=..\src\cdrom\SDL_syscdrom.h
# End Source File
# End Group
# Begin Group "cpuinfo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\cpuinfo\SDL_cpuinfo.c
# End Source File
# End Group
# Begin Group "events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\events\SDL_active.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_events.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_events_c.h
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_expose.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_keyboard.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_mouse.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_quit.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_resize.c
# End Source File
# Begin Source File

SOURCE=..\src\events\SDL_sysevents.h
# End Source File
# End Group
# Begin Group "file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\file\SDL_rwops.c
# End Source File
# End Group
# Begin Group "joystick"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\joystick\SDL_joystick.c
# End Source File
# Begin Source File

SOURCE=..\src\joystick\SDL_joystick_c.h
# End Source File
# Begin Source File

SOURCE=..\src\joystick\dummy\SDL_sysjoystick.c
# End Source File
# Begin Source File

SOURCE=..\src\joystick\SDL_sysjoystick.h
# End Source File
# End Group
# Begin Group "loadso"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\loadso\dummy\SDL_sysloadso.c
# End Source File
# End Group
# Begin Group "main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\main\dummy\SDL_dummy_main.c
# End Source File
# End Group
# Begin Group "stdlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\stdlib\SDL_getenv.c
# End Source File
# Begin Source File

SOURCE=..\src\stdlib\SDL_iconv.c
# End Source File
# Begin Source File

SOURCE=..\src\stdlib\SDL_malloc.c
# End Source File
# Begin Source File

SOURCE=..\src\stdlib\SDL_qsort.c
# End Source File
# Begin Source File

SOURCE=..\src\stdlib\SDL_stdlib.c
# End Source File
# Begin Source File

SOURCE=..\src\stdlib\SDL_string.c
# End Source File
# End Group
# Begin Group "thread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\thread\mpf\SDL_sysmutex.c
# End Source File
# Begin Source File

SOURCE=..\src\thread\mpf\SDL_sysmutex_c.h
# End Source File
# Begin Source File

SOURCE=..\src\thread\mpf\SDL_syssem.c
# End Source File
# Begin Source File

SOURCE=..\src\thread\mpf\SDL_systhread.c
# End Source File
# Begin Source File

SOURCE=..\src\thread\SDL_systhread.h
# End Source File
# Begin Source File

SOURCE=..\src\thread\mpf\SDL_systhread_c.h
# End Source File
# Begin Source File

SOURCE=..\src\thread\SDL_thread.c
# End Source File
# Begin Source File

SOURCE=..\src\thread\SDL_thread_c.h
# End Source File
# End Group
# Begin Group "timer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\timer\mpf\SDL_systimer.c
# End Source File
# Begin Source File

SOURCE=..\src\timer\SDL_systimer.h
# End Source File
# Begin Source File

SOURCE=..\src\timer\SDL_timer.c
# End Source File
# Begin Source File

SOURCE=..\src\timer\SDL_timer_c.h
# End Source File
# End Group
# Begin Group "video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\video\blank_cursor.h
# End Source File
# Begin Source File

SOURCE=..\src\video\default_cursor.h
# End Source File
# Begin Source File

SOURCE=..\src\video\e_log.h
# End Source File
# Begin Source File

SOURCE=..\src\video\e_pow.h
# End Source File
# Begin Source File

SOURCE=..\src\video\e_sqrt.h
# End Source File
# Begin Source File

SOURCE=..\src\video\math_private.h
# End Source File
# Begin Source File

SOURCE=..\src\video\mmx.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_blit.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_blit.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_blit_0.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_blit_1.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_blit_A.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_blit_N.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_bmp.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_cursor.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_cursor_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_gamma.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_glfuncs.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_leaks.h
# End Source File
# Begin Source File

SOURCE=..\src\video\mpf\SDL_mpf_events.c
# End Source File
# Begin Source File

SOURCE=..\src\video\mpf\SDL_mpf_events_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\mpf\SDL_mpf_mouse.c
# End Source File
# Begin Source File

SOURCE=..\src\video\mpf\SDL_mpf_mouse_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\mpf\SDL_mpf_video.c
# End Source File
# Begin Source File

SOURCE=..\src\video\mpf\SDL_mpf_video.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_pixels.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_pixels_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_RLEaccel.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_RLEaccel_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_stretch.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_stretch_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_surface.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_sysvideo.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_video.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_yuv.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_yuv_mmx.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_yuv_sw.c
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_yuv_sw_c.h
# End Source File
# Begin Source File

SOURCE=..\src\video\SDL_yuvfuncs.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\SDL.c
# End Source File
# Begin Source File

SOURCE=..\src\SDL_error.c
# End Source File
# Begin Source File

SOURCE=..\src\SDL_error_c.h
# End Source File
# Begin Source File

SOURCE=..\src\SDL_fatal.c
# End Source File
# Begin Source File

SOURCE=..\src\SDL_fatal.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
