.MODEL LARGE
.386
.STACK 200h

.DATA

;Variables del sistema:
	MAX_TEXT equ 30
	MAX_LIST equ 100
	ENDL db 0Dh, 0Ah, '$'
	_zero dd 0.0 
	msgError1 db "Error: division por cero",'$' 
	msgError2 db "Error: desbordamiento",'$' 
	msgError3 db "Error: indice fuera de rango",'$' 
	_cotaEnteros dd 65535

;Variables del usuario:
	A dd 0
	B dd 0
	__1 dd 1

;Variables auxiliares:
	AUXSTR db MAX_TEXT dup (?), '$'
	$EXP dd 0
	$AUX_ELEM dd 0
	$EQU_AUX dd 0 
	AuxTrue db "TRUE",'$' 
	AuxFalse db "FALSE",'$' 
AUX0 dd 0

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
JNE _else0
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, A
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se suma __1 a A
FILD A
FILD __1
FADD
FISTP AUX0
;Se asigna AUX0 a A
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD AUX0
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD AUX0
FISTP A
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
