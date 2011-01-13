PATH D:/cygwin/arm-linux/bin;D:/CYGWIN/BIN
make -f makefile debug=enable
call genDLL.bat
dlmake dlmake.ini
copy PicShow.CC k:\
pause
