    .global main 

main:
    mov r0, #0x0F                   @   Load R0 with 0x0F 
    mov r1, #0xF0                   @   Load R1 with 0xF0 
    
    and r2, r0, r1                  @   r2 = r0 and r1 (0x00)
    orr r3, r0, r1                  @   r3 = r0 or r1 (0xFF)
    eor r4, r0, r1                  @   R4 = R0 XOR R1 (0xFF)


    b   .                           @   Infinite loop (halts execution)
