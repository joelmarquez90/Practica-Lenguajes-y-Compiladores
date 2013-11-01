copy Grupo123.y  "C:\Users\Joel"
cd "C:\Users\Joel"
PCYACC.EXE Grupo123.y
copy GRUPO123.C "C:\Users\Joel\Documents\Joel\Universidad\Lenguajes y Compiladores\Varios\Grupo123\main.c"
pause
copy "C:\Users\Joel\Documents\Joel\Universidad\Lenguajes y Compiladores\Varios\Grupo123\bin\Debug\Grupo123.exe" "C:\Users\Joel\Documents\Joel\Universidad\Lenguajes y Compiladores\Compilador\Grupo123\Entrega Final - 1-12-12\Grupo123.exe"
cd "C:\Users\Joel\Documents\Joel\Universidad\Lenguajes y Compiladores\Compilador\Grupo123\Entrega Final - 1-12-12"
Grupo123.exe prueba.jsm
GraficarArbol.jar
copy "C:\Users\Joel\Documents\Joel\Universidad\Lenguajes y Compiladores\Compilador\Grupo123\Entrega Final - 1-12-12\final.asm" "C:\Users\Joel\Tasm4\final.asm"
cd "C:\Users\Joel\Tasm4"
tasm /zi final.asm
tlink /i final
final
pause