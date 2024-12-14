/*
 * main.c file is a simple bare-metal program designed to run on the ARM Cortex-A15
 * processor. It demonstrates basic hardware initialization by configuring the PLO11 UART
 * peripheral for serial communication. The program sends a "Hello, Cortex-A15!" message
 * over the UART interface then enters an infinite loop to prevent the CPU from halting.
 *
 * This code serves as a foundation for bare-metal development on the Cortex-A15, 
 * showcasing how to interact with memory-mapped peripherals without relying on an
 * operating system or bootloader.
 *
 * */

#define UART0_BASE 0x1c090000           // PL011 UART base address for vexpress-a15
#define UARTFR (UART0_BASE + 0x18)      // UART Flag Register offset 
#define UARTDR (UART0_BASE + 0x00)      // UART Data Register offset 

// Forward declaration of main()
void main(void);

static inline void uart_putc(char c) {
    // Wait until UART is ready to transmit
    while (*(volatile unsigned int *)UARTFR & (1 << 5)) {
        // Busy-wait
    }
    *(volatile unsigned int *)UARTDR = c; // Write the character
}

static inline void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// The entry point
void _start(void) {
    main(); // Call your main function
    while (1) {
        // Infinite loop to prevent returning
    }
}

// Your main function
void main(void) {
    uart_puts("Hello, Cortex-A15!\n");
    while (1) {
        // Infinite loop to prevent exiting
    }
}
