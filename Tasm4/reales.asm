
ten dw 10 

putc macro char
push ax
mov al, char
mov ah, 0eh
int 10h
pop ax
endm 

print_num proc near

push dx
push ax
cmp ax, 0
jnz not_zero
putc '0'
jmp printed

not_zero:
; the check sign of ax,
; make absolute if it's negative:
cmp ax, 0
jns positive
neg ax
putc '-'

positive:
call print_num_uns
printed:
pop ax
pop dx
ret
print_num endp

; this procedure prints out an unsigned
; number in ax (not just a single digit)
; allowed values are from 0 to 65535 (ffff)
print_num_uns proc near
push ax
push bx
push cx
push dx

; flag to prevent printing zeros before number:
mov cx, 1
; (result of "/ 10000" is always less or equal to 9).
mov bx, 10000 ; 2710h - divider.
; ax is zero?
cmp ax, 0
jz print_zero
begin_print:
; check divider (if zero go to end_print):
cmp bx,0
jz end_print
; avoid printing zeros before number:
cmp cx, 0
je calc
; if ax<bx then result of div will be zero:
cmp ax, bx
jb skip
calc:
mov cx, 0 ; set flag.
mov dx, 0
div bx ; ax = dx:ax / bx (dx=remainder).
; print last digit
; ah is always zero, so it's ignored
add al, 30h ; convert to ascii code.
putc al
mov ax, dx ; get remainder from last div.

skip:
; calculate bx=bx/10
push ax
mov dx, 0
mov ax, bx
div cs: ten ; ax = dx:ax / 10 (dx=remainder).
mov bx, ax
pop ax
jmp begin_print

print_zero:
putc '0'
end_print:
pop dx
pop cx
pop bx
pop ax
ret
print_num_uns endp 