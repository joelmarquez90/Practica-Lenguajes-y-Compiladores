          TITLE TBINASC.ASM
          EXTRN BINASC:FAR
          .MODEL SMALL
          .STACK 100H
          .DATA
MES       DB 0DH,0AH,'ANSWER IS: '
BUF       DB 6 DUP(?)
          DB 0DH,0AH,'$'
          .CODE
TBINASC:  MOV  AX,@DATA
          MOV  DS,AX
          MOV  AX,-32760   
          LEA  DX,BUF
          CALL BINASC
          LEA  DX,MES
          MOV  AH,9
          INT  21H
          MOV  AX,4C00H
          INT  21H
          END  TBINASC
