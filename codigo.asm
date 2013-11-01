
.CODE

include strings.asm
include reales.asm

MAIN:
MOV AX, @DATA
MOV DS,AX
;Salta si A no es igual a B
FILD A
FILD B
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE _fin_if0
_while0:
;Salta si B no es menor a C
FILD B
FILD C
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB _fin_while0
;Se asigna C a B
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD C
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD C
FISTP B
;Se asigna C a A
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD C
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD C
FISTP A
;Salta si C no es mayor a A
FILD C
FILD A
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE _fin_if1
;Se asigna __3 a B
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD __3
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD __3
FISTP B
_fin_if1:
JMP _while0
_fin_while0:
_fin_if0:
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
