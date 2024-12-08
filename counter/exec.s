ldr r0, =0x80000000 @   Load address into r0 (memory location for counter)
mov r1, #0          @   Initialize counter to 0

loop:
    str r1, [r0]    @   Store the counter value at memory address in r0 
    add r1, r1, #1  @   Increment the counter by 1
    b   loop        @   Repeat indefinitely
