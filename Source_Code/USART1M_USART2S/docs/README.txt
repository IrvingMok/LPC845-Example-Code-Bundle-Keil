${ProjName}:
USART1M_USART2S


Intended purpose:
* To demonstrate the configuration and use of the USART module, in both synchronous master and slave modes.
* User input/output is via the debug UART.
* See Serial.c and chip_setup.h to setup the debug com port.
* Connect a terminal emulator to the appropriate VCOM or USB COM port.



Functional description:
* In this example we configure USART1 as master and USART2 as slave.
* The user is prompted to enter a string (not to exceed 31 characters) with no whitespace, terminated by [Enter].
* One character at a time, that string is transmitted by the master to the slave.
* Slave receive data is handled by the USART2 interrupt service routine.
* When the NUL string terminator is received, a handshake flag is set by the
  ISR, the received string is printed to the console, and the process repeats.


// Configure USART1 as master, USART2 as slave
// Bit rate = 115,200 b.p.s
// 8 data bits
// No parity
// 1 stop bit

// FRG and BRG calculation
// For synchronous mode, the formula is
// BRG + 1 = MainClk * (1/baud) * (1/(1+(frgmult/frgdiv)))
// main clock = 30 MHz want baudrate = 115,200 Hz
// Use FRG = 1 (frgmult = 0)
// Then the formula is (BRG + 1) = (MainClk / baudrate) = 260.4
// So use BRG = 259

// SWM settings for USART1 (master):
// P0.24 = U1_SCLK (requires external connection with P0.25)
// P0.13 = U1_TXD
// P0.14 = U1_RXD
//
// SWM settings for USART2 (slave):
// P0.25 = U2_SCLK (requires external connection with P0.24)
// P0.13 = U2_RXD
// P0.14 = U2_TXD

// Function clocks to USARTs 1 and 2:
// Both USART1 and USART2 are clocked by the output of FRG1, and FRG1 is clocked by main_clk.



External connections:
* There is one external connection necessary based on the SWM settings:
* P0.24 (U1_SCLK) requires external connection with P0.25 (U2_SCLK)



Program Flow:
* This example runs at 30 MHz. See funtion SystemInit() and chip_setup.h

* main() routine
  1. Clocks to both USART1,2, and the SWM are enabled. 

  2. The SWM is configured as per above.

  3. The FRG and BRG are configured as per above.

  4. The USARTs are each given a peripheral reset.

  5. USART1 CFG register is configured for:
     8 data bits
     no parity
     one stop bit
     no flow control
     synchronous mode
     sample on falling edges
     master mode
     no loop-back

  6. USART1 CTL register is configured for:
     no continuous break
     no address detect
     no Tx disable
     no CC
     no CLRCC

  7. USART2 BRG, CFG, and CTL registers are configured the same as for USART1:

  9. USART2 Rx Ready interrupt is enabled, and code execution enters the main while(1)
     loop.
 
  9. The user is prompted to enter a string, and that string is transmitted one character at a time from master to slave
     until the NUL terminator is received.

  10. The received string is printed, and the process repeats.
  
 
This example runs from Flash. 


To run this code:
  1. Build
  2. Debug
  3. Run
  OR:
  1. Program to flash
  2. Reset								

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
                      



