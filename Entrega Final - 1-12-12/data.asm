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
	A dd 0
	B dd 0
	C dd 0
	D dd 0
	E dd 0
	F dd 0
	G db MAX_TEXT dup (?),'$'
	__30 dd 30
	__20 dd 20
	__2 dd 2
	_CTE_STRING_0 db "HOLA",'$'
	__1 dd 1
	__8 dd 8
	__4 dd 4
	__0 dd 0
	_CTE_STRING_1 db "INLIST",'$'
	_CTE_STRING_2 db " AND EQUMAX",'$'
	_CTE_STRING_3 db "NO",'$'
	_CTE_STRING_4 db " ENTRE",'$'
	_CTE_STRING_5 db " AL ",'$'
	_CTE_STRING_6 db "IF INLIST",'$'
	__10 dd 10
	__10_00 dd 10.00
	_CTE_STRING_7 db "F == 10.0",'$'

;Variables auxiliares:
	AUXSTR db MAX_TEXT dup (?), '$'
	EXP dd 0
	AUX_ELEM dd 0
	EQU_AUX dd 0 
	AuxTrue db "TRUE",'$' 
	AuxFalse db "FALSE",'$' 
	AUX0 dd 0
	AUX1 dd 0
	AUX2 dd 0
	AUX3 dd 0
	AUX4 dd 0
	AUX5 dd 0
	;Variables auxiliares para el Inlist/Equmax/Equmin 0
	VECAUX_0 dd MAX_LIST dup (0)
	INDICE_0 dd 0 
	MAX_INDICE_0 dd 0 
	RESUL_0 dd 0 
	AUX6 dd 0
	AUX7 dd 0
	;Variables auxiliares para el Inlist/Equmax/Equmin 1
	VECAUX_1 dd MAX_LIST dup (0)
	INDICE_1 dd 0 
	MAX_INDICE_1 dd 0 
	RESUL_1 dd 0 
	;Variables auxiliares para el Inlist/Equmax/Equmin 2
	VECAUX_2 dd MAX_LIST dup (0)
	INDICE_2 dd 0 
	MAX_INDICE_2 dd 0 
	RESUL_2 dd 0 
	AUX8 dd 0
