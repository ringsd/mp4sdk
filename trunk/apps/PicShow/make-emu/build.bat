PATH d:\cygwin\bin;d:\cygwin\mipseltools\bin;d:\cygwin\sde\bin
rem make -f makefile clean
make -f makefile 2>&1 |tee err.txt
copy txtreader.emu i:\EMU\
copy txtreader.emu i:\plug\
pause
builddl.exe txtreader.emu load.dl
