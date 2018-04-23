Project_Name
============
- Ctimer_PWM



Intended purpose
================
- To demonstrate the configuration and use of the Standard Counter/Timer (CTIMER)
  to generate three PWM outputs, with independent duty-cycles, while being clocked
  by an external capture input pin.



Hardware requirements
=====================
- Micro USB cable
- LPC8xx Xpresso Max board
- Personal Computer
- Oscilloscope (optional)
- Tequila (optional)



Board settings / external connections
=====================================
- OUT_PORT.OUT_BIT connected to IN_PORT (defined in Ctimer_PWM.h)



Functional description
======================
- This example uses the Self-wake-up timer (WKT), which is configured to be clocked by the Low Power Oscillator,
  to interrupt on every LP Osc. clock (10,000 times/second +- 40%).
- The WKT interrupt handler toggles a GPIO output pin, OUT_PORT.OUT_BIT.
- The output pin is connected externally to another pin, IN_PORT, which is assigned to Capture 0 via the switch matrix.
- The CTIMER is configured for external count mode, incrementing on both edges of Capture 0.
- Match register 3 is used to determine the PWM period (reset the TC, and interrupt, on MR3 match).
- Match registers 0 - 2 are put into PWM mode, and the CTIMER ISR assigns new duty cycles to each, on every MR3 interrupt.
- The duty-cycles of the three PMW outputs cycle from 0% to 100% to 0% roughly every two seconds.



Program Flow
============
- This example runs at system_ahb_clk = 30 MHz. See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, Debug, Run.
- or
- Download to flash, then reset.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


