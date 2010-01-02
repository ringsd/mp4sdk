PATH D:/cygwin/arm-linux/bin;D:/CYGWIN/BIN
rem make -f makefile clean
make -f makefile 2>&1 |tee err.txt
call genDLL.bat
copy PicShow.CC j:\
pause
