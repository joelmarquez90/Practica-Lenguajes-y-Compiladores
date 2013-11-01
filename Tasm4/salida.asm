.MODEL SMALL

.STACK 200h

.DATA

msgerr DB "La operacion exedio el rango, overflow$"
A DW 0
AUX1 DW 0


.CODE
MAIN:

  MOV AX,1
  MOV A,AX
  MOV AX,A
  SUB AX,2
  JC ERRSUMRES
  MOV AUX1,AX
  MOV AX,AUX1
  MOV A,AX


JMP FINAL

ERRSUMRES:
  MOV AX, SEG msgerr
  MOV DS, AX
  MOV DX, OFFSET msgerr
  MOV AH,9h
  INT 21h

FINAL:
  MOV AX, 4C00h  ; Sale al DOS
  INT 21h        ; Genera la interrupcion 21h
END MAIN