
;labels errors

1@$@#@: hlt
A: mov r3 ,LENGTH
A: inc K
B:
C:  mov r3 ,LENGTH
.extern C
.entry h
inc: mov r3 ,LENGTH
r3: mov r3 ,LENGTH
*&^%$#: .data 1	 	 ,4 	 	,  	342 , 		234
;string errors
.string za wardo
    .string za wardo"
    .string "za wardo
    .string za "wardo".
    .string za ."wardo"
    .string ",
;data errors
L1:  .data l, 4, 8, 15, 16, 23, 42
    .data --433, 653, 30
    .data 763, 599, ++875, 5
    .data 4, 8, 1  5 , 16, 23, 42
    .data 4,8,15,16,23,4 +2
    .data  4, 8, 15, x, 16, 23, 42
    .data a, b, c, d
    .data 3, 4, 6.5, 9 

;two operands command
mov r2
cmp r3 ,
cmp #88
cmp r5,#23
;one operands command
red #3

jsr A(r3, )
jmp    A
bne A(#333333,r3)
sub #, !
add r3,,r4
jmp r0
jmp   r7  
jmp   r7  r5
jmp #45
B1: rts r1
B2: stop A
