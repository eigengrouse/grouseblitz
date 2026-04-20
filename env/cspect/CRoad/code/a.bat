rem del *.o
rem del *.nex
rem del *.bin

cls
make

if ERRORLEVEL 1 goto doexit

r.bat

:doexit