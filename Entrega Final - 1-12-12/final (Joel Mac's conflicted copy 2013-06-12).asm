.MODEL LARGE
.386
.STACK 200h

.DATA

;Variables del sistema:
	MAX_TEXT equ 30
	MAX_LIST equ 100
	___UNO___ dd 1
	ENDL db 0Dh, 0Ah, '$'
	_zero dd 0.0 
	msgError1 db "Error: division por cero",'$' 
	msgError2 db "Error: desbordamiento",'$' 
	msgError3 db "Error: indice fuera de rango",'$' 
	_cotaEnteros dd 65535

;Variables del usuario:
	INT1 dd 0
	__9 dd 9

;Variables auxiliares:
	AUXSTR db MAX_TEXT dup (?), '$'
	EXP dd 0
	AUX_ELEM dd 0
	EQU_AUX dd 0 
	AuxTrue db "TRUE",'$' 
	AuxFalse db "FALSE",'$' 

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
