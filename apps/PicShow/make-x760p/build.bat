PATH D:\cygwin\bin;D:\cygwin\mipseltools\bin;D:\cygwin\sde\bin
rem make -f makefile clean
make -f makefile
call gen_dl.bat
copy PicShow.SIM j:\game\
pause
