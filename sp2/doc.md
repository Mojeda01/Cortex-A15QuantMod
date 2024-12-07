# Assembly Code Breakdown
```
mov r0, #10      @ Load 10 into r0
mov r1, #20      @ Load 20 into r1
add r2, r0, r1   @ Add r0 and r1, store the result in r2
b .              @ Infinite loop (stays here indefinitely)

```

1. `mov r0, #10`
    * The immediate value 10 (decimal) is loaded into register R0.
    * After exeuction, R0 = 0x0000000A

2. `mov r1, #20`
    * The immediate value 20 (decimal) is loaded into register R1.
    * After exeuction, R1 = 0x00000014

3. `add r2, r0, r1`
    * Adds the contents of R0 (10) and R1 (20) and stores the result in register R2. 
    * After execution, R2 = 0x0000001E (30 in decimal).

4. `b .`
    * This creates an infinite loop, preventing the CPU from executing further instructions.
    * The program counter (PC) stays in the same location indefinitely.

# CPU Register State 
```
R00=0000000a  R01=00000014  R02=0000001e
R03=00000000  R04=00000000  ...
```
* R0 = 0x0000000A
    * Value 10 (decimal) loaded via the first `mov` instruction.

* R1 = 0x00000014
    * Value 20 (decimal) loaded via the second `mov` instruction.

* R2 = 0x0000001E
    * Sum of R0 + R1 = 10 + 20 = 30, stored in R2.

* Remaining Registers (R3 to R15)
    * These remain zero or unchanged because no instructions interact with them in this program.

## Program Counter (R15/C)
* R15 (Program Counter): 0x8001000C
    * Points to the address of the indefinite loop (`b .` instruction).
    * The CPU continuously executes the branch instruction (b .), keeping the program counter locked at this address.

## Status Register (PSR)
* PSR (Program Status Register): `40001d3`
    * This reflects the processor's state, specifically:
        * Z flag(Z--): Zero flag is clear because the result of the last operation (R0 + R1) was not zero.
        * SVC mode (svc32): The CPU is operating in Supervisor Mode with 32-bit instructions.
