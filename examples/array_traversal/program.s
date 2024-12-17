    .data
array:
    .word   1,  2,  3,  4,  5

    .text
    .global main
main:
    
    ldr r0, =array          @   Load base address of 'array'
    mov r1, #5              @   Number of elements
    mov r2, #0              @   Index = 0
loop:
    ldr r3, [r0, r2, lsl #2]    @ Load array[R2] into R3 (4-byte aligned)
    add r2, r2, #1              @   Increment Index
    cmp r2, r1                  @   Compare index with size 
    bne loop                    @   Repeat until R2 == 5

    b   .                          @   Infinite loop (halts execution)
