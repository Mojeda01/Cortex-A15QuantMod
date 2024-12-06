# ARM A15x2 TRF Notes
**********************************************
# Chapter 1 - Introduction
## 1.1 About the CoreTileExpress A15x2 daughterboard
The CoreTile CoreTile Express A15x2 daughterboard is designed as a platform for developing systems based on the *Advanced Microcontroller Bus Architecture* (AMBA) that use the *Advanced eXtensible Interface* (AXI) or custom logic for use with ARM cores.

You can use the CoreTile Express A15x2 daughterboard to create prototype systems.

The daughterboard includes the following features:

* Cortex-A15 MPCore test chip, with NEON, the advanced Single Instruction Multiple Data (SIMD) extension and FPU, that contains a dual-core A15 cluster operating at 1GHz.
    1. **Cortex-A15MPCore**: This is the multi-core variant of the Cortex-A15 CPU architecture, allowing for multiple cores (processors) to work together, which is useful for parallel computing and increased performance. The A15 was designed for high-performance use cases, typically in mobile devices and embedded systems.

    2. **NEON**: NEON is an advanced SIMD (Single Instruction Multiple Data) extension. It allows for parallel processing of multiple data points in a single instruction, which makes it very effective for operations involving multimedia like audio processing, image manipulation, or complex mathematical calculations (like in machine learning). Essentially, NEON can handle large blocks of data more efficiently by processing them in parallel.

    3. **Advanced SIMD Extension**: This refersion to NEON's ability to perform SIMD operations. SIMD means that a single instruction can be applied to multiple pieces of data simultaneously, which is why it's called "vector processing". This is especially useful for tasks that involve repeated calculations on large sets of data, such as digital signal processing or graphics manipulation.

    4. **FPU (Floating-Point-Unit)**: The FPU is a specialized unit for performing arithmetic operations on floating-point numbers (numbers with decimals). Havind a dedicated FPU allows for efficient exeuction of math-heavy calculations, which is essential in fields like scientific computing, 3D graphics, and engineering simulations.

    5. **Dual-core A15 Cluster operating at 1GHz**: This means that the chip contains two Cortex A15 cores working together as a "cluster". Each core operates at a clock speed of 1GHz, which refers to how many cycles the CPU can execute per second (1GHz=1 billion cycles per second). This allows for parallelism - meaning multiple instructions can be processed simultaneously, improving performance and responsiveness.

    - *In summary*, the test chip we're working with is quite powerful for its intended purposes, featuring dual-core processing, advanced SIMD capabilities through NEON, and an efficient FPU for floating-point operations. This combination makes it suitable for a wide variety of high-performance applications. from multimedia to scientific calculations.

* Cortex-A15 MPCore test chip internal AXI subsystem operating at 600MHz.

    1. Cortex-A15 MPCore test chip: This is the specific chip being used, containing one or more Cortex-A15 cores. The term MPCore indicates that this is a multi-core version of the Cortex-A15, meaning it has multiple CPU cores working in tandem.

    2. Internal AXI Subsystem:
        * AXI stands for Advanced eXtensible Interface, which is part of the AMBA (Advanced Microcontroller Bus Architecture) specification. AXI is the bus system used for communication between various components within the SoC (System on Chip).
        * The internal AXI subsystem connects different parts of the chip, such as the CPU cores, memory controllers, and peripherals. It acts as the communication backbone that allows data and control signals to be efficiently transferred between components.
        * The AXI protocol supports high performance and low latency, which is why it's often used in high-speed processors like the Cortex-A15. It provides features like burst transactions, multiple outstanding addresses, and out-of-order transactions to keep the data flow optimal within the chip.

    3. Operating at 600MHz:
        * This means that the internal AXI bus is clocked at 600MHz (600 million cycles per second). This frequency dictates how fast data can move across the internal AXI subsystem.
        * Note that this clock speed is different from the CPU clock speed (e.g., 1GHz for the CPU cores). The AXI bus often operates at a lower frequency compared to the CPU cores because it's focused on ensuring reliable communication between components, rather than directly executing instructions.
        - The AXI subsystem operating at 600MHz is the internal communication infrastructure that links different parts of the Cortex-A15 MPCore chip. It handles data flow between CPU cores, memory, and other components at a clock rate of 600 million cycles per second. This ensures that all the parts of the SoC can communicate efficiently, supporting the processor's overall performance.

* Simple configuration with V2M-P1 motherboard:
    * Configuration EEPROM
    * Daughterboard Configuration Controller
    
    1. Simple Configuration with V2M-P1 motherboard:
        * The V2M-P1 is a specific ARM motherboard designed for use in development systems. This motherboard is often used with different daughterboard to create a flexible testing platform for ARM-based CPUs like Cortex-A15.
        * The term "simple configuration" here implies a basic setup that uses this motherboard along with other components, which are detailed below.

    2. Configuration EEPROM:
        * EEPROM stands for Electrically Erasable Programmable Read-Only Memory. It's a type of non-volatile memory tha retains its contents even when power is turned off.
        * The Configuration EEPROM in this context likely stores configuration settings for the motherboard and daughterboard. These settings could include things like boot configurations, clock settings, or peripheal setup information.
        * During boot-up, the system might read from the EEPROM to determine how to configure various aspects of the hardware.

    3. Daughterboard Configuration Controller:
        * The Daughterboard Configuration Controller is a component responsible for managing the settings and operation of the daughterboard.
        * It communicates with both the motherboard and the daughterboard to handle things like power managemment, clock distribution, and system initialization. Essentially, it acts as the "manager" for ensuring the daughterboard is correctly set up and communicating with the rest of the system.
        - The V2M-P1 motherboard works together with a Configuration EEPROM and a Daughterboard Configuration Controller to ensure proper initialization and management of the daughterboard(which might contain the Cortex-A15 cores). This setup helps in controlling various hardware aspects, ensuring all components work smoothly as inteded.  

* Nine programmable oscillators

    1. Oscillators:
        * An oscillator is a curcuit that generates a consistent periodic signal, usually a sine or square wave. These signals are crucial for setting up clock frequencies, which are used to synchronize different parts of the system.
        * In the context of CPUs like the Cortex-A15, oscillators are used to provide the clock signals that control the timing of operations. A stable clock signal is critical for the correct functioning of the processor and its components.

    2. Programmable Oscillators:
        * Programmable oscillators are oscillators that can be configured to operate at a different frequencies. This means that the frequency they produce can be adjusted according to the needs of the system.
        * By having programmable oscillators, the hardware provides flexibility in setting different clock rates, which is useful for optimizing performance or power consumption. For example, during testing, you may want to experiment with different clock speeds to see how the system performs under various conditions.

    3. Nine Programmable Oscillators:
        * The fact that there are *nine* programmable oscillators indicates that the system can generate multiple different clock signals, potentially for different components. this allows for a high level of control and tuning across various parts of the motherboard and daugtherboard.
        * Each oscillator can be used for different purposes, such as setting different clock frequencies for the CPU cores, the memory, or various peripherals. This is particularly useful in a development environment where you need to test or tweak specific parts of the system independently.
        - nine programmable oscillators means that there are nine independent clock-generating circuits that can be adjusted to different frequencies. This feature gives you a lot of flexibility when experimenting with the hardware, allowing you to configure different components of the system to run at their optimal clock speeds for different testing and debugging scenarios.

* 2GB of daughterboard DDR2 32-bit memory operating at 400MHz.

    1. 2GB of Memory: This refers to the total capacity of the memory available on the daughterboard, which is 2 gigabytes. This is the amount of data that can be stored and accessed quickly by the processor.

    2. DDR2 Memory:
        * DDR2 stands for Double Data Rate 2. It is a type of SDRAM (Synchronous Dynamic Random-Access Memory) that is widely used in computers and embedded systems for main memory.
        * Compared to its predecessor, DDR1, DDR2 memory is faster and more efficient, capable of achieving higher data rates. It uses a 4-bit prefetch buffer and operates at lower voltage compared to DDR1, making it more efficient in terms of power consumption.
        * The "Double Data Rate" means that the memory can transfer data on both the rising and falling edges of the clock signal, effectively doubling the data rate compared to single data rate memory.

    3. 32-bit Memory:
        * The 32-bit description refers to the width of the data bus used to transfer data between the memory and the processor. A 32-bit data bus means that 32 bits of data can be transferred at once, which affects the overall throughput.
        * The memory bus width is important because it impacts how quickly the system can move data to and from memory. In this case, a 32-bit width is a moderate size for embedded systems, providing a balance between speed and hardware complexity.

    4. Operating at 400MHz:
        * The memory is clocked at 400MHz, which means it performs 400 million clock cycles per second.
        * Because DDR memory transfers data on both edges of the clock signal, the effective data rate for DDR2 running at 400MHz is 800 MT/s (megatransfers per second). This clock frequency affects how quickly the CPU can read from and write to memory, and higher clock speeds typically result in better performance.
        - The 2GB of daughterboard DDR2 32-bit memory operating at 400MHz means youhave a 2GB capacity of DDR2 memory, which transfers data in 32-bit chunks and runs at a clock speed of 400MHz.

* High Definition LCD (HDLCD) controller that supports up to 1920x1080p video at 60Hz, 165MHz pixel clock.
    1. High Definition LCD (HDLCD) Controller:
        * An HDLCD controller is a component that handles the output of video signals to an LCD display. It manages the data that needs to be sent to the screen so that images and video can be displayed correctly.
        * The term **High Definition (HD)** suggests that it is capable of handling higher resolution outputs, which is important for modern displays.

    2. Supports up to 1920x1080p Video:
        * This means the maximum resolution the controller can handle is 1920 pixels horizontally and 1080 pixels vertically, which is commonly referred to as 1080p or Full HD.
        * 1080p is a standard high-definition resolution, suitable for modern LCD screens, ensuring sharp and detailed video and image quality.

    3. 60Hz:
        * The 60Hz here refers to the refresh rate of the display. It indicates that the screen can refresh 60 times per second.
        * A refresh rate of 60Hz is standard for most displays, ensuring smooth motion in video playback and animations. Higher refresh rates would make fast-moving visuals appear smoother, but 60Hz is generally sufficient for typical applications.

    4. 165MHz Pixel Clock:
        * The pixel clock is the frequency at which pixels are sent to the display. In this case, it is 165MHz, meaning 165 million pixels are processed per second.
        * This clock determines how quickly each pxiel can be drawn on the screen. A higher pixel clock allows higher resolutions or refresh rates to be supported.
        * For a resolution of 1920x1080 at 60Hz, a 165MHz pixel clock is enough to ensure the entire screen can be updated smoothly at 60 times per second without losing detail or experiencing flicker.
        - The HDLCD controller in the system is responsible for driving a Full HD LCD display at a 60Hz refresh rate, and it operates with a 165MHz pixel clock to ensure smooth and high-quality video output. This makes it suitable for displaying crisp and fluid visuals, ideal for embedded systems that require graphical output, such as control interfaces or multimedia applications.

* CoreSight software debug and 32-bit trace ports.
    1. CoreSight:
        * CoreSight is an ARM-developed technology that provides debug and trace capabilities for embedded processors. It is used to gain insight into the internal behavior of ARM-based systems during development.
        * With CoreSight, developers can trace program execution, debug software, and diagnose issues such as crashes or unexpected behavior without affecting the normal execution of the CPU.
        * CoreSight supports a variety of features like tracing program flow, capturing memory access, and monitoring system performance, making it a comprehensive solution for debugging and performance analysis.
    2. Software Debug
        * Software debug using CoreSight allows developers to pause, step through, and inspect their code while it is running on the processor. This is helpful in finding bugs or understanding how software interacts with the hardware.
        * CoreSight can integrate with software development tools (like debuggers and IDEs) to provide real-time access to the internal state of the processor, allowing for more efficient and effective debugging.
    3. 32-bit Trace Ports:
        * The 32-bit trace ports refer to a physical interface that outputs trace data from the processor. **Trace data** is detailed inforation about what the CPU is doing, such as instructions being executed or memory accesses.
        * The 32-bit description indicates that the trace port has a 32-bit wide bus for outputting this information, meaning it can send 32 bits of trace data in each clock cycle. The width of the port affects how much trace information can be output at a given time.
        * Trace ports are particularly useful in high-performance debugging, where understanding exactly how code is being executed (in terms of timing and sequence) is crucial.
        - CoreSight software debug and 32-bit trace ports provide a powerful infrastructure for observing and debugging the operation of the Cortex-A15. CoreSight allows for non-intrusive debugging and tracing, while the 32-bit trace ports enable detailed output of the processor's internal activities, making it much easier for developers to troubleshoot issues and optimize software performance. This feature is particularly useful in embedded systems where visibility into low-level operations is often necessary to understand and solve complex bugs.

* HDRX header with one AMBA AXI master bus port that connects to the other daughterboard site on the V2M-P1 motherboard.
    1. HDRX Header

* HDRY header with four buses to the motherboard:
    * Static Memory Bus (SMB).
    * MultiMedia Bus (MMB).
    * Configuration BUS (CB).
    * System Bus (SB).

* Power Supply Units (PSUs) for the Cortex-A15 test chip and DDR2 memory.

* CPU voltage control and current, temperature, and power monitoring.

## 1.2 Precautions.

# Chapter 2 - Hardware Description
## 2.1 CoreTile Express A15x2 daughterboard architecture.
## 2.2 Cortex-A15 MPCore test chip
## 2.3 System interconnect signals
## 2.4 Power-up configuration and resets
## 2.5 Serial Configuration Controller 
## 2.7 Clocks 
## 2.8 Interrupts
## 2.9 HDLCD
## 2.10 DDR2 memory interface 
## 2.11 Debug 

# Chapter 3 - Programmers Model
## 3.1 About this programmers model 
## 3.2 Daughterboard memory map 
## 3.3 Test chip SCC registers
## 3.4 Programmable peripherals and interfaces

