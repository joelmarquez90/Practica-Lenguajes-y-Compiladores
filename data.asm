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
	C dd 0
	D dd 0
	E dd 0
	F db MAX_TEXT dup (?),'$'
	G db MAX_TEXT dup (?),'$'
	__3 dd 3

;Variables auxiliares:
	AUXSTR db MAX_TEXT dup (?), '$'
	EXP dd 0
	AUX_ELEM dd 0
	EQU_AUX dd 0 
	AuxTrue db "TRUE",'$' 
	AuxFalse db "FALSE",'$' 
