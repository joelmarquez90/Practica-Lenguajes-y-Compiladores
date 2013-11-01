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
