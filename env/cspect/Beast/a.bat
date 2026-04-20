..\snasm.exe -map beast.asm beast_tmp.nex
if ERRORLEVEL 1 goto doexit

..\CSpect.exe -debug -brk -sound -60 -w3 -vsync -s28 -map=beast_tmp.nex.map -zxnext -mmc=.\ beast.nex

:doexit