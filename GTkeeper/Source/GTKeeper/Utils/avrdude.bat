REM echo %1.hex
REM exit

REM matar proceso
taskkill /F /FI "WINDOWTITLE eq COM2 - PuTTy"
REM Cargar avrdude

C:\WinAVR-20100110\bin\avrdude -Uflash:w:%1.hex:i -v -V -pm2560  -cwiring -PCOM2 -b115200 -D 
REM D:\arduino-1.8.1\hardware\tools\avr/bin/avrdude.exe -Uflash:w:%1.hex:i -CD:\arduino-1.8.1\hardware\tools\avr/etc/avrdude.conf -v -patmega328p  -carduino -PCOM2 -b115200 -D 
REM lanza putty
ping localhost -n 2 -w 500
start C:\Users\Gabi\Desktop\putty.exe -load arduino
