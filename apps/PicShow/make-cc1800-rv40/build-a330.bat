PATH E:\Program Files\ARM\RVCT\Programs\4.0\400\win_32-pentium;D:/CYGWIN/BIN;C:\windows\system32
set RVCT40INC=
set RVCT22INC=
set ARMINC=
make -f makefile shell=shell_app_a330 LinkScript=DLL.SCF dllcfg=dlmake_a330.ini
copy PicShow-A330.cc k:\
pause
