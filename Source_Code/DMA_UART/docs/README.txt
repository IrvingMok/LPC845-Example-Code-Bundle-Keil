Project_Name
============
- DMA_UART



Intended purpose
================
- To demonstrate the usage of the DMA controller with linked descriptors, 
  peripheral DMA requests, and triggers.
- This example provides a clear (I hope) framework upon which more complex
  configurations of the DMA controller can be built.



Hardware requirements
=====================
- Micro USB cable
- LPC8xx Xpresso Max board
- Personal Computer



Board settings / external connections
=====================================
- None



Functional description
======================
- User input is via the debug UART.
- Connect a terminal emulator (9600/8/N/1) to the debug UART COM port.
- A board modification may be required to use the debug COM port. Please refer
  to the board schematic.
- See chip_setup.h and Serial.c to setup the debug UART.

- Because this example uses the USART0_TX_DMA request input, we must use DMA
  channel #1.
- The triggers for this example are launched when software writes '1' to the
  SETTRIG0 register bit for the channel. The effect is exactly the same as if 
  an enabled hardware trigger (which would hypothetically have been enabled
  in the CFG register for the channel) had occurred.
- Since this example does not currently use hardware triggers, we will not 
  configure the DMA trigger multiplexer (DMA_ITRIG_INMUX) for the channel.

- Two descriptors are configured for the DMA channel: The initial channel
  descriptor (which we refer to as descriptor A or sequence A) and
  one reload descriptor (which we refer to as descriptor B or sequence B).
- Channel Descriptor (A) 
  - Configured to fetch successive bytes from the source buffer in
    data memory and write them to the destination register (the TXDAT register) in
    the UART0 peripheral. The USART0_TX_DMA request will throttle the DMA writes
    to the TXDAT holding register, preventing overrun. A variable-length string
    is entered by the user and gets placed in the data memory buffer for
    retrieval by Descriptor A.
  - Configured so that the trigger is not cleared when A is
    exhausted (CLRTRIG = 0). Also, Descriptor A is 'linked' to Descriptor B, so
    that each time the A sequence is exhausted, the B sequence begins without
    the need for another trigger.
  - Configured to cause an 'A' interrupt when exhausted.
- Reload Descriptor (B)  
  - Configured to fetch successive bytes from the source address in flash
    (a string constant), and write them to the destination TXDAT register
    in the UART0 peripheral (of course, paced by the dma request).
  - Configured to clear the trigger when it is exhausted (CLRTRIG = 1), and
    the next trigger will start a new sequence. 
  - The next trigger will be generated only after the user enters a new string, and 
    Descriptor A has been adjusted for the new transfer length, and source data
    end address, of the new string.
  - Configured to cause a 'B' interrupt when exhausted.



Program Flow
============
- The UART is configured.
- The user is prompted to enter a string, which gets stored in the seqa_string
  array in SRAM.
- The length of the seqa_string and seqb_string (which is constant) are determined.
- Clocks are enabled and the DMA gets reset.
- The DMA channel's CFG register is initialized
- The transfer configuration for the A sequence is constructed, and the initial
  Channel Descriptor (A) is configured.
- The transfer configuration for the B sequence is constructed, and the Reload
  Descriptor (B) is configured.
- The SRAMBASE register is pointed to the beginning of the channel descriptor
  SRAM table.
- The Valid bit for the channel is set, the channel and it's interrupt are enabled,
  the DMA is globally enabled, and code enters the main while(1) loop. 
- Handshake flags for the A and B interrupts are cleared, and software causes
  a trigger on the channel by writing to the SETTRIG0 register.
- Code then waits for both A and B interrupts to complete.
- Upon triggering, the A sequence DMAs the user string, from the seqa_string
  buffer, to the UART. Then the B sequence DMAs the constant string from flash
  to the UART.
- Each sequence, when complete, triggers the DMA interrupt, and the
  appropriate handskae flag is set by the ISR.
- When both sequences have completed, the user is prompted to enter a new
  string.
- The new length is calculated, the channel's XFERCFG register and the A
  descriptor source field are updated, and the loop repeats.

- This example runs at system_ahb_clk = 30 MHz. (main_clk = 30 MHz. FRO direct output). 
  See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, Debug, Run.
- or
- Download to flash, then reset.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


