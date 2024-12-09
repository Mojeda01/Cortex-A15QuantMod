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
## 2 Adding a Bootloader 
## 3 Preparing a C environment 
## 4 Build & Debug System
## 5 UART driver development 
## 6 Interrupts
## 7 Simple scheduling
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
## Chapter 3 - Introduction to Instruction Sets: v4Y and v7-M
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
