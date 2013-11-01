.MODEL LARGE
.386
.STACK 200h

.DATA

;Variables del sistema:
	 MAX_TEXT equ 30
	 ENDL db 0Dh, 0Ah, '$'
	 _zero dd 0.0 
	 msgError1 db "Error: division por cero",'$' 
	 msgError2 db "Error: desbordamiento",'$' 
	 msgError3 db "Error: indice fuera de rango",'$' 
	 _cotaEnteros dd 65535

;Variables del usuario:
__8 dd 8
__10 dd 10
__9 dd 9
__11 dd 11
__ESTA_EN_LA_LISTA_ db "ESTA EN LA LISTA",'$'

;Variables auxiliares:
AUXSTR db MAX_TEXT dup (?), '$'
