ldr r0, =0x80000000 @   Load address into r0
mov r1, #42         @   Load value 42 into r1 
str r1, [r0]        @   Store r1 value at the address in r0 
ldr r2, [r0]        @   Load the value from the address into r2
b   .
