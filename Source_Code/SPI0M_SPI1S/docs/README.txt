${ProjName}
SPI0M_SPI1S


Intended purpose:
* To demonstrate the configuration and use of the SPI module, in both master and slave modes.
* User input/output is via the debug UART.
* Connect a terminal emulator (9600/8/N/1) to the appropriate VCOM or USB COM port.
* See Serial.c to setup the debug com port.



Functional description:
* In this example we configure SPI0 as master and SPI1 as slave.
* The user is prompted to enter a string (not to exceed 31 characters) with no whitespace, terminated by [Enter].
* One character at a time, that string is transmitted by the master to the slave, and simultaneously by the
  slave to the master.
* Both master and slave receive data is handled by their respective interrupt service routines.
* When the NUL string terminator is received, handshake flags are set by the ISRs, the received strings are both
  printed to the console/terminal, and the process repeats.


External connections:
* There are four external connections necessary based on the SWM settings:

*  SWM settings for SPI0 (master):
*  P0.20 = SPI0_SCK   (requires external connection with P0.6)
*  P0.21 = SPI0_MISO  (requires external connection with P0.7)
*  P0.25 = SPI0_MOSI  (requires external connection with P1.19)
*  P0.24 = SPI0_SSEL0 (requires external connection with P1.18)
* 
*  SWM settings for SPI1 (slave):
*  P0.6  = SPI1_SCK   (requires external connection with P0.20)
*  P0.7  = SPI1_MISO  (requires external connection with P0.21)
*  P1.19 = SPI1_MOSI  (requires external connection with P0.25)
*  P1.18 = SPI1_SSEL0 (requires external connection with P0.24)



Program Flow:
* This example runs at 30 MHz.
  See funtion SystemInit() and chip_setup.h

* main() routine
  1. Clocks to both SPI modules and the SWM are enabled. 

  2. The SWM is configured as per above.

  3. The SPIs are each given a peripheral reset.

  4. SPI0 (the master) clock divider is configured for 15 MHz (div-by-2) based on 30 MHz system clock.

  5. Both SPI config registers are configured (Enable=true, master/slave, no LSB first, CPHA=0, CPOL=0, no loop-back, SSEL active low).

  6. The master SPI0 delay register is configured (Pre-delay = 0 clocks, post-delay = 0 clocks, frame-delay = 0 clocks, transfer-delay = 0 clocks).

  7. Both SPI control registers are configured (Master: End-of-frame true, LEN = 8 bits. Slave: LEN = 8 bits).

  8. Interrupts are enabled (Master: Rx Ready. Slave: Rx Ready and slave select deassert (end of transmission)),
     and code execution enters the main while(1) loop.

  9. The user is prompted to enter a string, and that string is transmitted one character at a time from both master to slave
     and slave to master until the NUL terminator is received.

  10. The received strings are both printed, and the process repeats.
  
 
* This example runs from Flash. 


* To run this code: Connect a terminal emulator (9600/8/N/1) to the appropriate COM port.
  1. Build
  2. Debug
  3. Run or Go
or:
  1. Build
  2. Program to flash
  3. Reset

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
           



