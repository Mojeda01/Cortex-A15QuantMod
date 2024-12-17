    .global main
main:
    mov r0, #5                  @   Initialize r0 to 5
    mov r1, #3                  @   Initialize r1 to 3 

    cmp r0, r1                  @   Compare r0 and r1
    beq equal                   @   Branch if r0 == r1
    bne notequal                @   Branch if r0 != r1

equal:
    mov r2, #1                  @   Set r2 to 1 (equal flag)
    b   end

notequal:
    mov r2, #0                  @   Set R2 to 0 (not equal flag)

end:
    b   .
