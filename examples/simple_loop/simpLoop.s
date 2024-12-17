    .global main

main:
    mov r0,#0           @   Counter (initialize to 0)
    mov r1,#10          @   End value (10)

loop:
    add r0, r0, #1      @   R0 = R0 + 1
    cmp r0, r1          @   Compare r0 to r1 (10)
    bne loop            @   If not equal, repeat loop


    b   .               @   Infinite loop, halts execution
