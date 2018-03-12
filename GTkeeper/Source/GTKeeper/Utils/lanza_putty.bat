@echo off


taskkill /F /IM putty.exe /T

REM cd "D:\Users\gabi\Google Drive\Proyectos\AVR\GTkeeper\Release"

REM C:\WinAVR-20090306rc1\bin\avrdude -pm1280 -V -carduino -PCOM4 -b57600 -Uflash:w:GTkeeper.hex:a

start /B C:\Users\Gabi\Desktop\putty.exe -load "arduino" 
REM start /B "C:\Users\Gabi\Desktop\putty.exe" -load "GSMSHIELD_RESPONSE"
