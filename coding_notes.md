# Development Notes
*******************
# Taking Notes From Bare-metal programming for ARM - A hands-on guide - Daniels Umanovskis

## 1 The First Boot
```
qemu-system-arm -M vexpress-a15 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait
```
1. Machine Selection (`-M`):
    * The original command uses `vexpress-a9`, which is specific to the Cortex-A9 processor.
    * For the Cortex-A15, you need to replace `vexpress-a9` with `vexpress-a15`. This switch tells QEMU to emulate a **Versatile Express board** with a **Cortex-A15 CPU**.

2. Memory Size (`-m 32M`):
    * This part doesn't need to change; it simply sets the amount of RAM for the emulated machine to **32 megabytes**. This is sufficient for simple bare-metal programs.

3. No Reboot (`-no-reboot`):
    * This flag prevents the system from rebooting if it chrashes, which is useful during early development and debugging so that you can observe the behavior before QEMU exits.

4. No Graphics (`-nographic`):
    * This flag ensures that QEMU runs as a command-line applciation with no graphical output, and it redirects the serial port output to your terminal, which is where you will see the output from your bare-metal code.

5. Monitor (`-monitor telnet:127.0.0.1:1234,server,nowait`):
    * The monitor allows you to interact with QEMU while it's running. Using telnet on port 1234, you can connect to QEMU and issue commands.
    * The server and nowait options ensure that QEMU keeps running even if you don't connect to the monitor immediately.

This command starts an ARM system using Cortex-A15 on the Versatile Express platform. Since there's no executable to run, QEMU will start, seem to do nothing, and eventually crash as there is no valid code for it to execute.

### Writing some code 
This simple assembly to make it compatible for the Cortex-A15. The assembly instructions are general ARM instructions, so they should work well with the Cortex-A15 without much modification.

**Adapted Code for Cortex-A15** (startup.s):

```
ldr r2,str1 
b .
str1: .word 0xDEADBEEF 
```

This part of the book walks through the process of assembling, linking, and converting the assembly code into a raw binary for running on an ARM CPU in bare-metal environment.

Assemble the `startup.s` assembly file using the **GNU assembler** (`as`) provided by the ARM toolchain. The command: 

```
arm-none-eabi-as -o startup.o startup.s 
```

This command assembles startup.s and outputs an object file named startup.o. This object 
file contains the machine code that represents your assembly instructions but is not yet in a format ready for the CPU to run.

Link the object file to create an ELF executable:

```
arm-none-eabi-ld -o first-hang.elf startup.o
```
This command links `startup.o` and outputs `first-hang.elf` without any custom linker script. The linker uses its default configuration.

* You may get a warning about the missing `_start` symbol, but we can ignore this for now as it is only a basic example.

Convert the ELF file to a raw binary format:

```
arm-none-eabi-objcopy -O binary first-hang.elf first-hang.bin 
```

This command extracts the machine code into raw binary file (`first-hang.bin`), which can be 
used in a bare-metal environment.

Use the following command to load and run your first-hang.bin binary on QEMU:

```
qemu-system-arm -M vexpress-a15 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel first-hang.bin 
```
The command slightly adapted for Cortex-A15, but it follows the same structure:
* `-M vexpress-a15`: Uses the Versatile Express platform for the Cortex-A15 CPU.
* `-m 32M`: Allocates **32MB of RAM** for the emulated system. 
* `-no-reboot`: Prevents the system from rebooting after halting.
* `-nographic`: Runs QEMU without a graphical window; everything is output in the terminal.
* `-monitor telnet:127.0.0.1:1234,server,nowait`: Sets up a *telnet server* for QEMU's monitor on port 1234. This lets you access the QEMU monitor for debugging. 
* `-kernel first-hang.bin`: Loads your raw binary file into the emulated machine.

Open a new terminal and connect to the QEMU monitor using the following command:

```
telnet localhost 1234 
```

This command will allow you to interact with the QEMU monitor. You should see a prompt similar to:

```
QEMU 2.8.1 monitor - type 'help' for more information
(qemu)
```

At the QEMU monitor prompt (`(qemu)`), type: 

```
info registers
```

This command will show you the current state of the CPU registers. Look for R2 in the output:

```
R00=00000000 R01=000008e0 R02=deadbeef R03=00000000
```

If `R02=deadbeef`, it means your assembly code executed successfully, and the value 
`0xDEADBEEF` was loaded into register **R2**.

## 3 Preparing a C environment
Preparing a bare-metal C environment involves initializing critical components in assembly and transferring 
control to C code. This process setting up the stack and relocating data from ROM to RAM. The initial C program will confirm functionality by sending stringts to a UART terminal. While not using a formal driver, UART output ensures proper system setup and serves as an effective debugging tool.

The startup code has been revised to include stack preparation, which is essential for executing C code. The stack is defined with a start and end address, and the stack pointer is initialized to the end address, conforming to the descending stack convention (e.g., `0x60020000` to `0x60021000` for a `0x1000` byte stack). The ARMv7-A architecture supports multiple stack pointers, specific to various processor modes (e.g., Supervisor, IRQ, FIQ). Registers like SP (`R13`) LR (`R14`) and PC (`R15`) may be referenced by different names, but their functionality remains consitent across tools and documentation.

The ARM registers `SP` and `R13` are interchangeable terms for the stack pointer. Similarly, `LR` refers to `R14` (link register), and `PC` refers to the `R15` (program counter). The naming depends on the documentation or tools being used, but their roles remain consistent. Avoid confunsion regarding these special registers, as they often have multiple names.

To prepare the startup code for ARM, we begin by defining processor modes (`MODE_FIQ`, `MODE_IRQ`, `MODE_SVC`) in the assembly file. The stack pointer (`SP`) is set up for each mode, with a separate memory region allocated to prevent overlap.

1. Switch to FIQ Mode:
    * Use the `msr cpsr_c, MODE_FIQ` instruction to set the processor to FIQ mode by writing the appropriate value (`0x11`) to the **CPSR** register.

2. Initialize Stack:
    * Load the stack's stard address into `R1` and the end address into `SP` using the `ldr` instruction. This ensures proper stack growth.

3. Garbage Fill for Debugging:
    * Fill the stack region with a repeating pattern (`0xFEFEFEFE`) using `movw` and `movt`. ARM assembly splits 32-bit constants into two parts for efficiency.

4. Validate Stack initialization:
    * Use a loop (`fiq_loop`) to iterate over the stack and fill it. The `cmp` and `strlt` instructions ensure the stack is completely filed.

This structured intialization is essential for handling exceptions and ensuring mode-specific stacks operate independently. The linker script will define stack addresses, which simplifies management.

The `strlt` and `blt` instructions in ARM assembly demonstrate **conditional execution**. These instructions execute only if specific condition flags are set. For instance:
* `strlr`: A conditional version of the `str` instruction, executing only if the `less-than` (`lt`) condition code is met, based on the result of a prior comparison.
* `blt`: A branch instruction that executes if the "less-than" condition code is met.

In the stack-filling loop:
1. **Compare**: `cmp R1, SP` checks if the current address in `R1` is less than the stack pointer (`SP`).
2. **Conditionally Store**: If `R1` is less than `SP`, `strlt R0, [R1], #4` writes the value `0xFEFEFEFE` (stored in `R0`) to the address in `R1` and increments `R1` by 4 bytes.
3. **branch**: The `blt` instruction repeats the loop as long as the condition holds.

Once the FIQ stack is initialized, the same process is repeated for the IRQ and Supervisor stacks, ensuring independent stack regions for each mode.

Page - 29 - Handling sections and data

## 4 Build & Debug System
## 5 UART driver development 
## 6 Interrupts
## 7 Simple scheduling
***********************
***********************
# Writing ARM assembly code -  
## Chapter 1 - An overview of computing systems
Modern devices, such as smartphones and tablets, rely on highly complex System-on-Chip (SoC) designs to perform their multitude of tasks. These SoCs integrate processors, memory, and graphic chips into a single package to save space, reduce power consumption, and improve efficiency. For example, SoCs found in handheld devices handle tasks such as video playback, GPS navigation, and wireless communication.
### System-on-Chip overview
1. Definition: An SoC is a combination of processors, memory, and peripherals fabricated into a single chip to perform diverse tasks.
2. Components: Modern SoCs often include:
    * Specialized microprocessors
    * Graphics engines
    * Floating-point units 
    * Energy management units
    * Other devices like USB controllers, timers, and communication interfaces.
#### Design Trends
* Reuse of Intellectual Property (IP): To save time and cost, companies often license tested IP blocks (e.g., Cortex-A or Cortex-M processors from ARM) rather than designing microprocessors from scratch.
* Standardization: Components are designed to meet inudstry standards so that the microprocessors can be swapped without re-engineering the entire system.
#### Example: Texas Instruments TMS320DM355 SoC:
* Combines ARM926EJ-S processors with peripherals like:
    * MPEG4/JPEG coprocessors
    * DMA controllers 
    * DDR2/mDDR memory contollers 
    * USB 2.0 and other I/O interfaces
#### Micrcontoller integration:
* Microcontrollers, a simplified form of SoCs, integrate additional useful components like:
    * Timers 
    * UARTs (Universal Asynchronous Receiver - Transmitters)
    * Analog-to-Digital Converters (A/D) These are widely used in cost-sensitive applications such as automotive, toys and industrial devices.

* Key advantages:
    1. Efficiency: Combines multiple functionalities into a compact, low-power chip.
    2. Flexibility: Standardized IP allows rapid prototyping and faster design cycles.
    3. Scalability: Designers can update microprocessors without re-engineering the entire system.

In summary, the rise of SoCs and microcontrollers designs have revolutionized electronics, enabling compact, efficient, and highly functionality devices across various industries.

### History of RISC (Reduced Instruction Set Computers)
#### Early Computing Challenge 
* In the 1980s, mainframes dominated computing, with large systems like the DEC VAX series and IBM System/38.
* Processor designs became increasingly complex, leading to challenges in improving frequency, adding features, and managing power consumption.
* Designers often relied on techniques like larger fans or external cooling to handle power demands.
#### Origins of RISC 
* RISC emerged from research by IBM, Control Data Corporation, and Data General in the late 1960s and early 1970s 
* In the 1980s, David Patterson and Carlo Sequin at UC Berkeley, and John Hennessey at Stanford, formalized the RISC concept.
* The goal was to simplify processor design by focusing on small, efficient instruction sets, diverging from the complex instruction set computing (CISC) approach.
#### Characteristic of RISC Architecture:
1. Single-Cycle Instructions:
    * Instructions execute in a single clock cycle.
    * Simpler hardware design compared to multi-cycle CISC instructions.
2. Fixed-Length Instructions:
    * Uniform instruction size simplifies decoding.
    * Contrasts with CISC designs like Motorola 68000, where instructions vary in size.
3. Simplified decoding
    * Fewer instructions mean simpler hardware for decoding.
    * Register numbers are encoded uniformly within instructions.
4. No microcode:
    * RISC eliminates the need for microcode, which in CISC Designs maps high-level instructions to hardware operations.
    * This avoid complexities like those in VAX instructions (e.g., the "POLY" instruction)
5. Load/Store Architecture:
    * Only load and store instructions access memory, with all other operations using registers.
    * Reduces hardware complexity compared to CISC's direct memory operations.
6. Optimized Validation:
    * Simple designs are easier to debug and validate, improving reliability.
#### Trade-offs and Impact:
* RISC processors often execute more instructions for the same task, leading to increased program size.
* However, faster clock speeds and simpler hardware offset these costs, making RISC highly efficient for modern applications.
* The Berkeley RISC architecture showed a 30% increase in code size but achieved faster execution due to simpler operations.
#### Evolution of RISC 
* Early RISC designs like DEC's Alpha and Motorola's 88000 family were powerful but faced limited commercial success.
* Over time, RISC evolved to incorporate more complex instructions ("feature creep"), blending with traditional CISC approaches.
* Modern RISC processors, like ARM, balance simplicity with advanced features, far exceeding the original RISC-1 instruction set of 31 operations.

### History of ARM Processors and RISC
#### Early Beginnings: Acorn Computers
* ARM traces its roots to Acorn Computers, a UK company known for its desktop PCs in the educational market.
* In the early 1980s, Acorn developed its own microprocessors to replace the 6502 used in the BBC Micro. This led to the ARM1 processor in 1985, notable for its simplicity (25,000 transistors) and efficiency, achieving functionality on its first attempt.
#### Transition to ARM Ltd.
* By 1990, Acorn, along with VLSI Technology and Apple, created ARM Ltd., marking a shift to licensing processor designs rather than manufacturing.
* The ARM6 processor was an early success, used in Apple's Newton PDA, and introduced features like 32-bit addressing, 4K caches, and MMUs.
* ARM7TDMI, launched in 1993, became one of ARM's most iconic processors, featuring a "Thumb" instruction set for improved code density and Debug Interface (ICE) for development ease. It saw widespread adoption in mobile devices.
#### Evolution Through the 1990s and beyond 
* The ARM8, ARM9, and ARM10 lines followed, improving performance and efficiency.
* Partnerships like one with DEC (Digital Equipment Corporation) led to custom ARM derivatives like StrongARM, later sold to Intel.
* By 1998, ARM became a publicly traded company, reflecting its growing prominence.
#### Architecture Overview 
* ARM's architecture evolved through various instruction set versions (e.g., ARMV4T, ARMv7-M).
* Different cores cater to diverse applications:
    * Application processors: Cortex-A series (e.g., Cortex-A15, A53).
    * Embedded processors: Cortex-M series. 
    * Classical processors: Legacy ARM cores like ARM7TDMI. 
* ARM's model of licensing intellectual property rather than manufacturing processors has made it a dominant force in the microprocessor industry, powering devices from smartphones to embedded systems.
### ARM Today 
* By 2002, over 1.3 billion ARM-based devices were in use, largely in mobile phones. Nokia dominated the mobile handset market, powered by ARM processors. TI provided a significant portion of silicon for these devices, alongside other ARM partners like Phillips, Analog Devices, LSI Logic, PrarieComm, and Qualcomm, with ARM7 as the primary processor.
* ARM's focus extended beyond processor cores to encompass Application Specific Integrated Circuits (ASICs), requiring components like timers, USB interfaces, DSPS, and cell libraries. To meet these needs, ARM acquired several companies: 
    * 2003: Adelante Technologies (DSP processors).
    * 2004: Axys Design Automation (hardware tools).
    * 2005: Keil Software (microcontoller tools).
    * 2006: Falanx (3D graphics accelerators) and SOISIC (silicon-on-insulator technology).
* These acquisitions aimed to streamline SoC (System-on-Chip) development, facilitating innovation and broad adoption.
* By 2012, ARM partner shipped around 8.7 billion ARM-based chips annually, spanning applications like cameras, smart power meters, and home devices. While consumers rarely notice ARM's role, its efficiency and adaptability have cemented its dominance in the tech industry.
### The Cortex family
#### Overview of Cortex Families 
* ARM has divided its processors cores into three families based on applications requriements: Cortex-A, Cortex-R, and Cortex-M. Each family is tailored for specific performance, power, and functionality needs. 
    * *Cortex-A*: Designed for high-performance applications like smartphones, tablets, servers, and desktops. These cores support complex operating systems like Linux and Android and often include large caches, arithmetic blocks, and memory management units (MMUs). Examples include the Cortex-A5, A9, and A15 cores. 
    * *Cortex-R*: Optimized for real-time and safety-critical applications such as automoative systems (e.g., anti-lock braking) and medical devices. These cores ensure deterministic behavior, essential for systems requiring precise timing and safety redundancy. Exaples include the Cortex-R4, R5 and R7.
    * *Cortex-M*: Focused on deeply embedded microcontroller applications like remote sensors, displays, and industrial robotics. These cores are cost-efficient, low-power and simple to program. Popular cores include the Cortex-M0, M0+, M3, and M4, with the latter supporting signal processing and floating-point arithmetic.
#### Design Highlights:
* The Cortex-M series caters to low-power devices, featuring minimal gate counts and compact instruction sets (e.g., 56 instructions for Cortex-M0).
* The Cortex-A series prioritizes computational horsepower for modern, feature-rich devices.
* The Cortex-R series bridges the gap by providing real-time reliability for embedded systems needing robust error handling and precise execution.
#### Industry adoption
* ARM's Cortex family is hihgly versatile, enabling tailored SoC designs across industries. For example:
    * Cortex-A cores are widely used in smartphones and tablets.
    * Cortex-R cores appear in automotive and medical safety systems.
    * Cortex-M cores dominate the microcontroller market for industrial and IoT applications. 
* By addressing diverse needs, ARM's Cortex family ensures scalability, efficiency, and widespread adoption across computing and embedded markets.
### The Computing Device 
* At its core, a computing device executes specific instructions or commands using mechanisms like paper tape, switches, or magnetic materials. Machines do not need to be electronic to compute. For example, in 1804, Joseph Marie Jacquard used punched cards to control a silk loom, a concept later adapted in computers. By the 1960s, punched cards fed programs into computers, often requiring precise numbering to avoid errors.
* A stored program computer fetches instructions from memory to execute tasks. This model divideds the system into three main components:
    1. **Processor**: Performs computations and processing tasks.
    2. **Memory**: Stores instructions and data.
    3. **Busses**: Transfers data and instructions between components.
* Assembly language programming focuses on fundamental processor operations, making it easier for humans to write machine-level instructions.
#### Embedded System and Modern Interfaces
* Unlike traditional systems with peripherals like keyboards or monitors, embedded systems often interface directly with sensors and system-on-chip designs. For example, an engine controller gathers data from sensors and processes it internally without user interaction. 
#### Hierarchy of computing 
* Understanding computing involves moving through a hierarchy:
    * Transistors: Basic switches controlling electrical flow.
    * Gates: Combinations of transistors performing logical operations.
    * Microarchitecture: Specific processor behavior and control.
    * Instruction Set Architecture (ISA): Defines commands the processor understands.
    * Language: Higher-level tools like C++ or Java, built upon the ISA. 
    * Applications/OS: Systems like Linux that run programs on hardware. 
* Assembly language resides at the ISA level, bridging the gap between raw hardware and high-level programming languages.
### Number Systems and Binary Basics 
#### Binary Number System 
* Computers operate using the binary system (base 2), as transistors only understand two states: 1 or 0.
* Binary numbers represent powers of 2 instead of 10. For example:
    * Decimal: 0, 1, 2, 3, ...
    * Binary: 0, 1, 10, 11, ... (where 10 represents 2 in decimal).
* Example Conversion: 1101012 to decimal:
    * 25 + 24 + 22 + 20 = 32 + 16 + 4 + 1 = 5310
#### Hexadecimal System (Base 16)
* Hexadecimal is commonly used for its compact representation of binary data.
* Digits range from 0-9 and A-F (A=10, B=11, ..., F=15).
* Example Conversion: A5E916 to decimal:
    * (10 x 163) + (5 x 162) + (14 x 161) + (9 x 160) = **42,47310**
* Decimal to Hexadecimal Conversion:
    * Steps:
        1. Divide the decimal number by 16.
        2. Record the remainder.
        3. Continue dividing until the quotient is 0.
        4. Writer remainders in reverse order.
    * Example: Convert 86210 to hexadecimal:
        * 862 / 16 = 53 R14 (E)
        * 53 / 15 = 3 R5 
        * 3 / 16 = 0 R3 
        * Result: 35E16 
* Binary and Hexadecimal Relationship 
    * Binary groups of 4 bits directly map to one hexadecimal digit: 
        * Binary: 1101 1111 0000 1010 1111 
        * Hexadecimal: DF0AF16 
    * Table of Equivalents: 
        * Decimal: 0, 1, ..., 15.
        * Binary: 0000, 0001, ..., 1111 
        * Hexadecimal: 0, 1, ..., F.
* By understanding these relationship, programming tasks involving memory and low-level data manipulation become much clearer. 
## Chapter 2 - The Programmer's Model
### 2.1 Introduction 
Programmers usually don't need to know the physical construction of a microprocessor, such as its wires or transistors. Instead, they rely on the "programmer's model", which provides a high-level view of the processor's functionality. This includes details like data storage locations, behavior during invalid instructions, and how registers are managed during exceptions. The text introduces the programmer's models for ARM7TDMI and Cortex-M4, with plans to revisit related topics like branching, stacks, and exceptions in later chapters. For now, the focus is on covering the basics needed to start programming.

* *Personal Note:*
    * If someone wanted to incorporate blackjack mathematics into their programming, they could model the game's logic by leveraging the processor's features. This would involve defining the rules of the game, tracking the deck of cards using arrays or similar data structures, calculating probabilities of drawing specific cards, and implementing decision-making algorithms. The programmer could also optimize these computations by using low-level operations available in the processor, such as efficient branching for decision trees or specialized registers for probability calculations.

### 2.2 Data Types 
Data in computers is represented as binary digits, or bits, where each bit can either be a 0 or a 1. These bits are grouped into units of eight, known as bytes, or into larger units, depending on the device's architecture. For example, a 16-bit data value is referred to as a "word" in processors like the Intel 8086 or MC68040, while in ARM cores, a 32-bit data value is called a word. Both instructions and data are often described with their bit length, such as 16-bit instructions or 32-bit data.

Specifically, the ARM7TDMI and Cortex-M4 processors support data types in sizes 0, 4, 8, or C (hexadecimal). Notably, the Cortex-M4 allows unaligned memory access under specific conditions, enabling the reading or writing data from odd memory addresses. Memory access details will be explored further in addressing modes in Chapter 5. Most operations, such as additions (ADD), are performed on word-sized data, but smaller 16-bit values are also supported in later contexts.

### 2.3 ARM7TDMI 
The purpose of examining the ARM7TDMI programmer's model is to highlight its similarities to more advanced cores like the Cortex-A and Cortex-R processors, which build on the ARM7TDMI's foundation while adding numerous features and modes. Despite its simplicity - featuring only a three-stage pipeline compated to the Cortex-A15's complex fifteen-stage out-of-order pipeline - the ARM7TDMI provides sufficient depth to introduce concepts like modes and exceptions without overwhelming detail.

It is also important to note that certain features are consistent across all ARM processors, through they may vary in number, purpose, and limitations. For example, the integer register file in the Cortex-M4 functions similarly to that of the ARM7TDMI, but with fewer registers. This exploration of the programmer's model begins with an overview of the processor modes.

#### 2.3.1 Processor Modes 
Version 4T cores operates in seven processor modes: User, FIQ, IRQ, Supervisor, Abort, Undefined and System, as outlined in Figure 2.1. Mode changes can be triggered by software control but are typically caused by external events or exceptions. Most application programs run in User mode. The other modes, known as priveliged modes, allow the processor to handle exceptions or access restricted resources. These resources include features like disabling certain sections of the processor, such as the branch predictor or caches, if they are available.

* *Personal notes Figure 2.1 - Processor modes:*
    1. Modes overview:
        * ARM processors operate in different modes to handle various tasks and exceptions.
        * These modes are categorized into **privileged modes** (for exceptions and system-level tasks) and an **unprivileged mode** (for regular application tasks).
    2. Privileged Modes:
        * Supervisor (SVC):
            * Entered during system resets or when a Software Interrupt (SWI) instruction is executed.
            * Used for system-level operations like kernel execution.
        * FIQ (Fast Interrupt):
            * Entered when a high-priority, fast interrupt is triggered.
            * Used for time-critical tasks, such as real-time signal handling.
        * IRQ (Interrupt):
            * Entered during a low-priority, standard interrupt.
            * Handles general-purpose interrupt-driven tasks.
        * Abort:
            * Activated when memory access violation occur.
            * Used to manage errors such as accessing invalid or restricted memory regions.
        * Undefined (Undef):
            * Triggered by the execution of undefined instructions.
            * Allows the system to manage unsupported or invalid operations.
        * System:
            * A privileged mode that uses the same register set as User mode.
            * Typically used for background system-level operations without switching to exception handling.
    3. Unprivileged Mode:
        * User:
            * Default mode for most application and operating system tasks.
            * Lacks access to certain system-level resources and instructions, ensuring safety and isoliation.
    4. Key Notes:
        * Mode transitions are generally triggered by exceptions or software instructions.
        * Privileged modes provide access to critical system resources, such as disabling caches or the branch predictor.
        * Exception handling is tightly integrated into mode management, allowing robust error handling and system control.
    * This structure ensures efficient and secure management of system resources, interrupts, and errors in ARM-based processors.

A mode can be viewed as an indicator of what the processor is currently doing. Under normal  conditions, the processor typically operates in either User Mode or Supervisor mode, exeucting code as expected. For instance, in devices like cell phones, where minimal activity occurs until an event such as a signal or user interaction happens, the processor may power down partially, awaiting an interrupt to wake it. Interrupts, triggered by external events, prompt the processor to act. The ARM7TDMI handles two types of interrupts: a high-priority, fast interrupt (FIQ) and a lower-priority interrupt (IRQ). FIQ mode is used for critical tasks, like handling power loss within milliseconds, while IRQ mode handles less urgent tasks, such as servicing peripherals or respknding to user input.

Abort mode helps the processor recover from exceptional situations, such as attempts to access invalid memory locations for instructions or data. This mode is also essential for supporting features like virtual memory, commonly used in operating systems like Linux.

Undefined mode is triggered when the processor encounters an instruction it cannot recognize in the pipeline. At this point, it becomes the responsibility of the programmer or operating system to handle the error. Historically, this mode was also used for supporting floating-point instructions on hardware without built-in floating-point capabilities, though modern systems rarely rely on it.

For most scenarios, work focuses on User and Supervisor modes, with interrupts and exceptions discussed in greater detail in Chapter 14.

#### 2.3.2 registers
* *Personal Notes on Figure 2.2 Register organization*
    1. General Overview:
        * ARM processors, including the ARM7TDMI, utilize a set of registers for data storage and operations.
        * Registers are organized into **User/System mode registers** and **banked registers**a, which are mode-specific.
        * The banked registers are highlighted in gray, indicating they are unique to specific processor modes (e.g., Supervisor, Abort, Undefined, IRQ, FIQ).
    2. User/System Mode Registers:
        * Registers R0 to R12 are shared across all modes.
        * These registers are general-purpose and available in user mode as well as privileged modes.
    3. Banked Registers:
        * Registers R13 (Stack Pointer) and R14 (Link Register) are banked for each mode (e.g., R13_SVC for Supervisor mode).
        * Banked registers allow each mode to maintain its own context, such as a unique stack or return address, preventing interferene between modes.
        * This is critical for handling exceptions for interrupts efficiently, as each mode can operator independently.
    4. Program Counter (PC):
        * The PC is shared across all modes and holds the address of the current instruction being executed.
    5. Status Registers:
        * The CPSR (Current Program Status Register) is shared and reflects the processor's current, including mode, interrypt status, and condition flags.
        * Each mode has a SPSR (Saved Program Status Register) to save the CPSR during exceptions or interrupts, allowing the processor to restore the original state when returning to the previous mode.
    6. Mode-Specific Highlights:
        * FIQ Mode:
            * Has the most banked registers (R8_FIQ to R14_FIQ), optimized for fast interrupt handling and reducing context-switching overhead.
        * IRQ Mode:
            * Uses its own R13_IRQ and R14_IRQ to handler lower-priority interrupts without affecting other modes.
        * Supervisor, Abort, and Undefined Modes:
            * Each mode has unique R13 and R14 registers, ensuring isolated stack management and return addresses during their specific operations.
    7. Why Banked Registers Are Important:
        * Banked registers minimize the overhead of saving/restoring context during mode switches, especially for interrupts and exceptions.
        * This design enhances performance and ensures reliability in real-time and multi-tasking systems.
    * Summary:
        * This register organization allows efficient mode transitions and context management, making ARM processors well-suited for embedded systems and applications requiring real-time responsiveness.

* *Personal Notes on Figure 2.3 ARM7TDMI pipeline diagram*
    1. Overview:
        * The ARM7TDMI processor uses a three-stage pipeline: Fetch, Decode, and Execute.
        * The pipeline improves performance by overlapping the execution of multiple instructions, allowing different stages of multiple instructions to occur simultaneously.
    2. Pipeline Stages:
        * Fetch:
            * The instruction is retrieved from memory.
            * The Program Counter (PC) holds the address of the current instruction in ARM and Thumb modes.
        * Decode:
            * The fetched instruction is paralyzed, and the required registers for the operation are identified.
            * In this stage, the processor interprets the instruction to determine the operation and operands.
        * Execute:
            * The operation is performed using the Arithmetic Logic Unit (ALU) or other processor resources.
            * Data is read from or written back to the Register Bank as needed.
    3. PC Adjustments:
        * In ARM mode:
            * The PC adjusts in increments of 4 bytes (as ARM instructions are 4 bytes long).
        * In Thumb mode:
            * The PC adjusts in increments of 2 bytes (As Thumb instructions are 2 bytes long, making them more compact).
    4. Pipeline Behavior:
        * During execution, the pipeline ensures that while one instruction is being executed, the next is being decoded, and a third is being fetched. This overlapping process significantly increases throughput.
        * Example:
            * When the instruction in Fetch moves to Decode, a new instruction is fetched, maintaining a steady flow.
    5. Key Points:
        * The pipeline design reduces instruction latency by handling multiple instructions in parallel.
        * The adjustments in PC for ARM and Thumb modes ensure compatibility with both instruction sets.
    6. Applications:
        * The pipeline structure is fundamental to achieving higher efficiency in embedded systems where ARM7TDMI processors are used, such as in real-time applications and low-power devices.
    * This diagram showcases how ARM7TDMI optimizes instruction execution through a streamlined pipeline process, balancing simplicity and performance.

* *Personal Notes on Figure 2.4 Format of the program status registers*
    * The Program Status Register (PSR) in ARM architecture holds critical information about the processor's state, including condition flags, interrupt settings, and mode control.
    1. Condition Flags (Bits 31-28):
        * N (Negative): Indicates whether the result of the last arithmetic operation was negative.
        * Z (Zero): Set if the result of the last operation was zero.
        * C (Carry): Indicates a carry out from the most significant bit in an addition or a borrow in a subtraction.
        * V (Overflow): Set if the result of the last arithmetic operation caused an Overflow.
    2. Reserved Bits (Bits 27-8):
        * These bits are mared as "Do not modify/Read as zero".
        * Reserved for future use or specific hardware implementations. Altering these can lead to undefined behavior.
    3. Control Bits:
        * I (Bit 7): Interrupt Disable Flag.
            * When set, normal interrupts (IRQ) are disabled.
        * F (Bit 6): Fast Interrupt Disable Flag.
            * When set, FIQ are disabled.
        * T (Bit 5): Thumb State Flag.
            * Indicates whether the processor is executing in Thumb State (1 = Thumb mode, 0 = ARM mode).
    4. Processor Mode Bits (Bits 4-0):
        * M[4:0]: Defines the current processor mode.
            * 00000: User mode.
            * 10000: IRQ mode.
            * 10001: IRQ mode.
            * 10011: Abort mode.
            * 10111: Undefined mode.
            * 11111: System mode.
        * These bits determine access permissions and available features, such as privileged instructions.
    * Key Points:
        * The PSR plays a crucial role in managing the processor's operation and exception handling.
        * Condition flags are updated after most arithmetic and logical instructions, enabling conditional exeuction of subsequent instructions.
        * The I, F, and T bits provide fine-grained control over interrupts and instruction set states.
        * Changing the M[4:0] bits allow mode switching, critical for handling exceptions and system-level operations.
    * Understanding PSR is virtal for low-level programming, especially when working on exception handling, interrypt service routines, or mode transitions. 

Registers are the core data storage areas within a processor, used to hold variables, counters, memory addresses, or coefficients. The ARM7TDMI processor has a total of 37 registers, organized as follows:
* 30 General-Purpose Registers (R0–R14): Can store any data needed during execution.
* 6 Status Registers: Contain inforamtion about the processor's state, including flags, modes, and interrupt settings.
* 1 Program Counter (R15): Tracks the address of the current instruction.

General-purpose registers are 32 bits wide, and the set of available registers changes depending on the current processor mode (e.g., User, Supervisor, IRQ). This arrangement, called "banked registers", allows certain registers to be swapped out during mode changes (e.g., in interrupts) for efficiency and safety.

banked registers R13 and R14 are unqiue for each mode (e.g., R13_SVC, R14_IRQ), ensuring each mode has its own stack pointer and return address:
* R13 (Stack Pointer, SP): Points to the stack in memory and is mode-specific.
* R14 (Link Register, LR): Holds the return address for subroutine or exception handling.
When an interrupt or exception occurs, the processor switches to a different mode (e.g., FIQ, IRQ) and swaps in the relevant R13 and R14 registers without needing to save the entire context to memory. This speeds up handling by minimizing overhead.

The Program Counter (PC) tracks the address of the current instruction being executed. While the processor fetches instructions from memory in a pipeline architecture, the PC always points to the instruction currently being fetched, not executed.

The CPSR represents the "state" of the processor, providing essential information for managing operations:
* Condition Flags (N, Z, C, V): Indicate the results of operations (e.g., negative, zero, carry, overflow).
* Control Bits (I, F, T):
    * I (IRQ Disable): Disables standard interrupts.
    * F (FIQ Disable): Disables fast interrupts:
    * T (Thumb State): Indicates whether the processor is executing in Thumb (16-bit) or ARM (32-bit) instruction.
* Mode Bits (M[4:0]): Define the processor mode, such as User, Supervisor, or IRQ.

Saved Program Status Register - Each privileged mode (e.g., FIQ, IRQ) has its own SPSR to save the CPSR during exceptions or interrupts. This allows the processor to restore the original state when returning to the previous task. User and System modes lack an SPSR since they do not handle exceptions directly.

Important Notes on Usage:
* The CPSR's condition flags can be modified by arithmetic and logical instructions, enabling features like conditional execution.
* The T bit determines the instruction set (ARM or Thumb) and can only be changed in privileged modes.
* Invalid configurations of the mode bits (M[4:0]) result in undefined behavior, as the processor may not recognize the state.
This organization ensures efficient context management and enables robust handling of exceptions and interrupts, crucial for real-time and embedded systems.

#### 2.3.3 The Vector Table - Page 65


## Chapter 3 - Introduction to Instruction Sets: v4T and v7-M 
## Chapter 4 - Assembler Rules and Directives
## Chapter 5 - Loads, Stores, and Addressing 
## Chapter 6 - Constants and Literal Pools 
## Chapter 7 - Integer Logic and Arithmetic 
## Chapter 8 - Branches and Loops 
## Chapter 9 - Introduction to Floating-Point: Basics, Data Types, and Data Transfer
## Chapter 10 - Introduction to Floating-Point: Rounding and Exceptions
## Chapter 11 - Floating-Point Data-Processing Instructions 
## Chapter 12 - Tables 
## Chapter 13 - Subroutines 
## Chapter 14 - Exception Handling: ARM7TDMI
## Chapter 15 - Excepttion Handling: v7-M 
## Chapter 16 - Memory-Mapped Peripherals 
## Chapter 17 - ARM, Thumb and Thumb-2 Instructions 
## Chapter 18 - Mixing C and Assembly
