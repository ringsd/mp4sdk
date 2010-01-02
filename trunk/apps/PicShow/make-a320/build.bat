PATH D:\cygwin\bin;D:\cygwin\mipseltools\bin;D:\cygwin\sde\bin
rem make -f makefile clean
make -f makefile 2>&1 |tee err.txt
call gen_dl.bat
copy PicShow.SIM j:\game\
pause
