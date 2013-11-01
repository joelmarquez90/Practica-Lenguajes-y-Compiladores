Grupo123.exe prueba.txt
GraficarArbol.jar
copy "C:\Users\CASA\Dropbox\Compilador\Grupo123\final.asm" "C:\Users\CASA\Tasm4\final.asm"
cd "C:\Users\CASA\Tasm4"
tasm /zi final.asm
tlink /i final
final
pause