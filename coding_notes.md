# Development Notes
*******************
Taking Notes From Bare-metal programming for ARM - A hands-on guide - Daniels Umanovskis

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
