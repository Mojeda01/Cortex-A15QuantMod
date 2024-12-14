SECTIONS {
    /* Define the memory layout */
    . = 0x8000; /* Starting address for the program */

    .text : {
        *(.text*) /* Include all text (code) sections */
    }

    .data : {
        *(.data*) /* Include all initialized data sections */
    }

    .bss : {
        *(.bss*)  /* Include all uninitialized data sections */
    }
}

