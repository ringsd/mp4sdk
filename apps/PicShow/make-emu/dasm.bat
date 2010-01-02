PATH d:\cygwin\bin;d:\cygwin\mipseltools\bin
mipsel-linux-objdump -d downenv.elf > env.ss
mipsel-linux-readelf -a downenv.elf > readelf
pause