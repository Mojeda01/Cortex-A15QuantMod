# Development Notes
*******************
## Taking Notes From Bare-metal programming for ARM - A hands-on guide - Daniels Umanovskis

### 1 The First Boot
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

### 2 Adding a Bootloader 
### 3 Preparing a C environment 
### 4 Build & Debug System
### 5 UART driver development 
### 6 Interrupts
### 7 Simple scheduling
