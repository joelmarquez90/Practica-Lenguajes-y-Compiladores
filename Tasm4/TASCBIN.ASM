          EXTRN ASCBIN:FAR
          .MODEL SMALL
          .STACK 100H
          .DATA
BUF       DB ' -2345'
          .CODE
TASCBIN:  MOV  AX,@DATA
          MOV  DS,AX
          MOV  CX,6     
          LEA  DX,BUF
          CALL ASCBIN
; AX now contains -2345 in binary
          MOV  AX,4C00H
          INT  21H
          END  TASCBIN
