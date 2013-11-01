
.CODE

include strings.asm
include reales.asm

MAIN:
MOV AX, @DATA
MOV DS,AX
;Se asigna __9 a INT1
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD __9
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD __9
FISTP INT1
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, INT1
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
JMP finProg
errorDivision:
MOV DX, OFFSET msgError1
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
JMP finProg
errorOverflow:
MOV DX, OFFSET msgError2
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
JMP finProg
errorLimite:
MOV DX, OFFSET msgError3
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Fin del programa
finProg: 
MOV AX,4C00h
int 21h

END MAIN
