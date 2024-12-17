    .global main

main:
    MOV R0, #5      @   Load 5 into R0
    MOV R1, #3      @   Load 3 into R1
    ADD R2, R0, R1

    B   .           
