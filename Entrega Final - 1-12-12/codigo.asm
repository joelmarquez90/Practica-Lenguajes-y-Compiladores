
.CODE

include strings.asm
include reales.asm

MAIN:
MOV AX, @DATA
MOV DS,AX
;Se asigna __30 a C
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD __30
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD __30
FISTP C
_while0:
FILD A
FILD C
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JE _cuerpo_sent0
FILD A
FILD C
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB _fin_while0
_cuerpo_sent0:
;Se asigna __20 a B
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD __20
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD __20
FISTP B
_while1:
;Salta si A no es menor a B
FILD A
FILD B
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB _fin_while1
;Se suma __2 a A
FILD A
FILD __2
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
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, A
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
JMP _while1
_fin_while1:
;Se imprime fin de linea y luego lo de adentro del print
MOV DX, OFFSET _CTE_STRING_0
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, A
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se resta C a B
FILD B
FILD C
FSUB
FISTP AUX1
;Se asigna AUX1 a B
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD AUX1
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD AUX1
FISTP B
;Se suma __1 a A
FILD A
FILD __1
FADD
FISTP AUX2
;Se asigna AUX2 a B
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD AUX2
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD AUX2
FISTP B
;Se suma __2 a A
FILD A
FILD __2
FADD
FISTP AUX3
;Se asigna AUX3 a A
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD AUX3
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD AUX3
FISTP A
JMP _while0
_fin_while0:
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, A
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, B
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, C
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se resta __8 a B
FILD B
FILD __8
FSUB
FISTP AUX4
;Se suma __4 a C
FILD C
FILD __4
FADD
FISTP AUX5
;Se carga en el vector numero 0 el elemento AUX4
mov eax, INDICE_0 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_0 
mov eax, AUX4 
mov dword ptr[bx],eax 
add INDICE_0, 1 
;Se carga en el vector numero 0 el elemento AUX5
mov eax, INDICE_0 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_0 
mov eax, AUX5 
mov dword ptr[bx],eax 
add INDICE_0, 1 
;Comienzo del equmin 0
FILD _cotaEnteros
FISTP EQU_AUX 
FILD INDICE_0
FISTP MAX_INDICE_0
mov INDICE_0, 0 
FILD A 
FISTP EXP
;Se compara con el primer elemento de la lista y comienza el ciclo
mov eax, INDICE_0
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_0 
mov eax, dword ptr[bx] 
mov AUX_ELEM, eax 
ciclo_equmin_0: 
FILD INDICE_0 
FILD MAX_INDICE_0 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB fin_equmin_0
FILD AUX_ELEM 
FILD EQU_AUX 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB cargar_elem_0 
FILD AUX_ELEM 
FISTP EQU_AUX 
cargar_elem_0: 
add INDICE_0, 1 
mov eax, INDICE_0
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_0 
mov eax, dword ptr[bx] 
mov AUX_ELEM, eax 
JMP ciclo_equmin_0 
fin_equmin_0: 
FILD EXP 
FILD EQU_AUX 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE cont_list_0
mov RESUL_0, 1 
cont_list_0:
;Salta si A no es igual a AUX_ELEM
JE _fin_if0
;Se suma C a B
FILD B
FILD C
FADD
FISTP AUX6
;Se multiplica C a A
FILD A
FILD C
FMUL
FISTP AUX7
;Se carga en el vector numero 1 el elemento AUX6
mov eax, INDICE_1 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_1 
mov eax, AUX6 
mov dword ptr[bx],eax 
add INDICE_1, 1 
;Se carga en el vector numero 1 el elemento AUX7
mov eax, INDICE_1 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_1 
mov eax, AUX7 
mov dword ptr[bx],eax 
add INDICE_1, 1 
;Comienzo del inlist 1
FILD INDICE_1
FISTP MAX_INDICE_1
mov INDICE_1, 0 
FILD A 
FISTP EXP
;Se compara con el primer elemento de la lista y comienza el ciclo
mov eax, INDICE_1
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_1 
mov eax, dword ptr[bx] 
mov AUX_ELEM, eax 
add INDICE_1, 1 
ciclo_inlist_1: 
FILD INDICE_1 
FILD MAX_INDICE_1 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB fin_inlist_1 
FILD EXP 
FILD AUX_ELEM 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JE fin_inlist_1
mov eax, INDICE_1
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_1 
mov eax, dword ptr[bx] 
mov AUX_ELEM, eax 
add INDICE_1, 1 
JMP ciclo_inlist_1 
fin_inlist_1: 
FILD EXP 
FILD AUX_ELEM 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE cont_list_1
mov RESUL_1, 1 
cont_list_1:
;Se carga en el vector numero 2 el elemento __0
mov eax, INDICE_2 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_2 
mov eax, __0 
mov dword ptr[bx],eax 
add INDICE_2, 1 
;Se carga en el vector numero 2 el elemento __1
mov eax, INDICE_2 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_2 
mov eax, __1 
mov dword ptr[bx],eax 
add INDICE_2, 1 
;Se carga en el vector numero 2 el elemento __0
mov eax, INDICE_2 
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_2 
mov eax, __0 
mov dword ptr[bx],eax 
add INDICE_2, 1 
;Comienzo del equmax 2
mov EQU_AUX, 0 
FILD INDICE_2
FISTP MAX_INDICE_2
mov INDICE_2, 0 
FILD C 
FISTP EXP
;Se compara con el primer elemento de la lista y comienza el ciclo
mov eax, INDICE_2
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_2 
mov eax, dword ptr[bx] 
mov AUX_ELEM, eax 
ciclo_equmax_2: 
FILD INDICE_2 
FILD MAX_INDICE_2 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNB fin_equmax_2
FILD AUX_ELEM 
FILD EQU_AUX 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNA cargar_elem_2 
FILD AUX_ELEM 
FISTP EQU_AUX 
cargar_elem_2: 
add INDICE_2, 1 
mov eax, INDICE_2
mov bx,4 
mul bx 
mov bx, ax 
add bx, OFFSET VECAUX_2 
mov eax, dword ptr[bx] 
mov AUX_ELEM, eax 
JMP ciclo_equmax_2 
fin_equmax_2: 
FILD EXP 
FILD EQU_AUX 
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE cont_list_2
mov RESUL_2, 1 
cont_list_2:
FILD RESUL_1
FILD ___UNO___
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE _else1
FILD RESUL_2
FILD ___UNO___
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE _else1
;Se concatena _CTE_STRING_2 a _CTE_STRING_1
MOV AX, @DATA
MOV es, AX
MOV si, OFFSET _CTE_STRING_1
MOV di, OFFSET AUXSTR
CALL COPIAR
MOV si, OFFSET _CTE_STRING_2
MOV di, OFFSET AUXSTR
CALL CONCAT
;Se asigna AUXSTR a G
MOV AX, @DATA
MOV es, AX
MOV si, OFFSET AUXSTR
MOV di, OFFSET G
CALL COPIAR
;Se imprime fin de linea y luego lo de adentro del print
MOV DX, OFFSET G
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
JMP _fin_if1
_else1:
;Se concatena _CTE_STRING_4 a _CTE_STRING_3
MOV AX, @DATA
MOV es, AX
MOV si, OFFSET _CTE_STRING_3
MOV di, OFFSET AUXSTR
CALL COPIAR
MOV si, OFFSET _CTE_STRING_4
MOV di, OFFSET AUXSTR
CALL CONCAT
;Se concatena _CTE_STRING_5 a AUXSTR
MOV AX, @DATA
MOV es, AX
MOV si, OFFSET AUXSTR
MOV di, OFFSET AUXSTR
CALL COPIAR
MOV si, OFFSET _CTE_STRING_5
MOV di, OFFSET AUXSTR
CALL CONCAT
;Se concatena _CTE_STRING_6 a AUXSTR
MOV AX, @DATA
MOV es, AX
MOV si, OFFSET AUXSTR
MOV di, OFFSET AUXSTR
CALL COPIAR
MOV si, OFFSET _CTE_STRING_6
MOV di, OFFSET AUXSTR
CALL CONCAT
;Se asigna AUXSTR a G
MOV AX, @DATA
MOV es, AX
MOV si, OFFSET AUXSTR
MOV di, OFFSET G
CALL COPIAR
;Se imprime fin de linea y luego lo de adentro del print
MOV DX, OFFSET G
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
_fin_if1:
;Se asigna __10 a A
;Se comprueba si la asignacion supera la cota de enteros
FILD _cotaEnteros
FILD __10
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JBE errorOverflow
FILD __10
FISTP A
;Se imprime fin de linea y luego lo de adentro del print
MOV eax, A
CALL print_num
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
;Se resta D a __10_00
FLD __10_00
FLD D
FSUB
FSTP AUX8
;Se asigna AUX8 a F
FLD AUX8
FSTP F
;Salta si F no es igual a __10_00
FLD F
FLD __10_00
FXCH
FCOMP
FSTSW AX
FFREE ST(0)
SAHF
JNE _fin_if2
;Se imprime fin de linea y luego lo de adentro del print
MOV DX, OFFSET _CTE_STRING_7
MOV ah, 9
int 21h
MOV DX, OFFSET ENDL
MOV ah, 9
int 21h
_fin_if2:
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
