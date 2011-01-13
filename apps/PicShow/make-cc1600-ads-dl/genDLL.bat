PATH d:\cygwin\arm-linux\bin

cd PicShow_Data\DebugRel\
arm-linux-objcopy -O binary -R EXPORT_TABLE_EXEC  -R EXPORT_STRING_EXEC -R IMPORT_TABLE_EXEC -R IMPORT_STRING_EXEC -R SECTION_INFO_EXEC downenv.elf ..\..\downenv.bin
arm-linux-objcopy -O binary -j EXPORT_TABLE_EXEC downenv.elf ..\..\export.tab
arm-linux-objcopy -O binary -j EXPORT_STRING_EXEC downenv.elf ..\..\export.str
arm-linux-objcopy -O binary -j IMPORT_TABLE_EXEC downenv.elf ..\..\import.tab
arm-linux-objcopy -O binary -j IMPORT_STRING_EXEC downenv.elf ..\..\import.str
arm-linux-objcopy -O binary -j SECTION_INFO_EXEC downenv.elf ..\..\section.dsc
cd ..\..\

dlmake dlmake.ini

pause

copy  PicShow.CC j:
pause