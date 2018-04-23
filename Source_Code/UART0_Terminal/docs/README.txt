${ProjName}:
UART0_Terminal


Intended purpose:
To demonstrate the configuration and use of the USART module, in asynchronous
mode, while communicating with a terminal emulator.


Functional description:
* In this example UART0 communicates with a terminal emulator running on the PC, via a 
  USB-to-RS232 breakout cable or via the VCOM serial port.
* The user is prompted to enter a string (not to exceed 32 characters), terminated by [Enter].
* Each character that is entered is echoed back to the terminal by the UART0
  ISR, and the characters are stored in an array for future use.
* When the user types [Enter], a NUL character is appended to the array, and
  a handshake flag is set by the ISR for the main routine.
* Upon seeing the handshake flag, the main routine prints the stored string to
  the terminal, and the process repeats.


External connections:
* To use the VCOM Serial Port:
    1. No external connections are necessary.

* To use a USB-to-RS232 breakout cable:
    1. There are three external connections necessary: RXD and TXD based on the SWM
       settings, plus a ground connection. The RXD and TXD pins can be chosen in chip_setup.h.
       The defaults are as follows:
       P0.25 = U0_TXD = breakout cable RXD
       P0.24 = U0_RXD = breakout cable TXD
       Board GND = breakout cable GND



Notes on the baud rate and FRG:
* The BRG and FRG settings are calculated in two steps, as follows:
  // For asynchronous mode (UART mode) the formula is:
  // (BRG + 1) * (1 + (m/256)) * (16 * baudrate Hz.) = FRG_in Hz.
  // We proceed in 2 steps.
  // Step 1: Let m = 0, and round (down) to the nearest integer value of BRG for the desired baudrate.
  // Step 2: Plug in the BRG from step 1, and find the nearest integer value of m, (for the FRG fractional part).
  //
  // Step 1 (with m = 0)
  // BRG = ((FRG_in Hz.) / (16 * baudrate Hz.)) - 1
  //     = (30,000,000/(16 * 9600)) - 1
  //     = 194.3
  //     = 194 (rounded)
  //
  // Step 2.
  // m = 256 * [-1 + {(FRG_in Hz.) / (16 * baudrate Hz.)(BRG + 1)}]
  //   = 256 * [-1 + {(30,000,000) / (16*9600)(195)}]
  //   = 0.002
  //   = 0 (rounded)


Program Flow:
* This example runs with:
    system_ahb_clk = 30 MHz. (main_clk = 30 MHz. FRO direct output)
    UART0CLK = 30 MHz. (FRG0CLKSEL = main_clk, FCLKSEL0 = frg0clk)
    See chip_setup.h and SystemInit()

* main() routine
  1. Clocks to USART0 and the SWM are enabled. 

  2. The SWM is configured as per above.

  3. The UARTCLKDIV and the FRG (both in SYSCON) are configured as per above
     (for 9600 b.p.s.).

  4. USART0 is given a peripheral reset.

  5. USART0 BRG is configured as per above (for 9600 b.p.s.). 

  6. USART0 CFG register is configured for:
     8 data bits
     no parity
     one stop bit
     no flow control
     asynchronous mode
     no loop-back

  7. USART0 CTL register is configured for:
     no continuous break
     no address detect
     no Tx disable
     no CC
     no CLRCC

  8. USART0 Rx Ready interrupt is enabled, and code execution enters the main while(1)
     loop.
 
  9. The user is prompted to enter a string, and each character is echoed by the
     ISR back to the terminal until the NUL terminator is encountered.

  10. When the NUL terminator is encounterd, the ISR sets a handshake flag for
      main, which prints the entire received string to the terminal, and the process repeats.
  
 
This example runs from Flash. 


To run this code:
  1. Build
  2. Program memory, then reset target
  or
  2. Debug
  3. Run or Go

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.




