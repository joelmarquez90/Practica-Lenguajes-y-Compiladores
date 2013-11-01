TITLE ASSIGN4 : ASCII TO BINARY
.MODEL SMALL
.STACK 100H

.DATA
MSG1    DB      'TYPE A CHARACTER: $'
MSG2    DB      0DH, 0AH, 'THE ASCII CODE OF $'
MSG3    DB      ' IN BINARY IS $'
MSG4    DB      0DH, 0AH, 'THE NUMBER OF 1 BITS IS $'

.CODE
MAIN PROC
;initialize DS
        MOV AX, @DATA
        MOV DS, AX

;print user prompt
        LEA DX, MSG1    ;load MSG1
        MOV AH, 9
        INT 21H         ;display it

;read user input
        MOV AH, 1       ;initialize AH to read input
        INT 21H         ;read input
        MOV BL, AL      ;move input into BL

;print result MSG
        LEA DX, MSG2    ;load MSG2
        MOV AH, 9
        INT 21H         ;display it

;display user input and result MSG
        MOV DL, BL      ;move input into DL
        MOV AH, 2
        INT 21H         ;display input
        LEA DX, MSG3    ;load MSG3
        MOV AH, 9
        INT 21H         ;display it

;clear registers
        XOR CX, CX      ;clear CX
        MOV CX, 8       ;set counter to 8

TOP:
;print result
        MOV AH, 2       ;prepare to display result
        ROL BL, 1       ;rotate one bit to left
        JNC CCF1        ;if CF=0 then jump to CCF1
        MOV DL, '1'     ;CF=1
        JMP CCF2        ;jump to CCF2
CCF1:   MOV DL, '0'     ;CF=0
CCF2:   INT 21H         ;display result of CF
        LOOP TOP        ;loop 8 times

;print MSG3
        LEA DX, MSG4    ;load MSG4
        MOV AH, 9
        INT 21H         ;display it

;count the number of 1 bits
        XOR DI, DI
        XOR AX, AX      ;clear AX
        MOV CX, 8       ;set counter to 8
TOP1:   SHL BL, 1       ;rotate to left
        JNC NEXT        ;CF=1
        INC DI          ;increment AX
NEXT:   LOOP TOP1       ;CF=0

;print out number of 1 bits
        MOV DX, DI      ;move # of 1 bits to DL
        MOV AH, 2       ;initialize AH
        INT 21H         ;display # of 1 bits

;exit to DOS
        MOV AH, 4CH
        INT 21H

MAIN    ENDP
        END MAIN        ;end of program





