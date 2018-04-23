${ProjName}:
ADC_Seqa_Seqb


Intended purpose:
* To demonstrate the configuration and use of the ADC controller.
* User input is via the debug UART.
* Connect a terminal emulator (9600/8/N/1) to the debug UART COM port.
* A board modification may be required to use the debug COM port. Please refer to the board schematic.
* See chip_setup.h and Serial.c to setup the debug UART.


Functional description:
* This example demonstrates how to configure the A and B sequences, assigning
  different channels to each, and allows either hardware or software
  triggering to be used based on a #define SOFTWARE_TRIGGER in the beginning of the main code.

* Two GPIO pins are configured as outputs, and two other GPIO pins are
  configured as the ADC_PINTRIG0 and ADC_PINTRIG1 inputs. The two outputs must be
  connected to the two inputs externally for hardware triggering to work.
  See the #defines in main.c
  Obviously, if pins are OUTPORTs/INPORTs (or any other digital function),
  they can't be ADC inputs, and vice-versa.

* Before configuration of the ADC begins, the ADC is put through a
  self-calibration cycle.

* Next, the switch matrix is configured, making ADC channels 1 - 7, and 9 - 11
  the active functions on their respective pins (remember, these are fixed-pin
  functions). 
  
* The pin interrupts are configured for active-high, level sensitive on
  PINTSEL1 and PINTSEL0, responding to the two input pins INPORT_B and INPORT_A.
  These interrupts will be assigned to ADC_PINTRIG1 and ADC_PINTRIG0.

* The TRM register is configured for the appropriate analog voltage and the
  CLKDIV register is configured for the desired sample rate.

* After ADC channels are assigned to each of the sequences, the triggers are
  chosen. Based on the #define SOFTWARE_TRIGGER, both channels will be either software
  triggered, or hardware triggered. If hardware triggering is chosen, the
  ADC_PINTRIG0 input will trigger sequence A and the ADC_PINTRIG1 input will
  triggger sequence B.

* Next the polarities are chosen for the hardware triggers (both sequences
  falling-edge triggered).

* SYNC_BYPASS is enabled since our external inputs are system-clock based
  (software driven GPIO) and active for longer than 1 ADC_CLOCK.

* Neither Burst nor Single-step is enabled.

* Sequence A is given high priority.

* Both sequences are configured for end-of-sequence mode, not
  end-of-conversion mode.

* Both sets of threshold registers are initialised and the channels are
  assigned to threshold register set 0 or 1. Note that
  this is for instructional purposes, as the threshold compare functions and
  interrupts are not currently part of this example.

* Finally, the SEQA and SEQB interrupts are enabled, the sequences themselves
  are enabled, and code enters the main while (1) loop.

* In the main while(1) loop, the user is prompted to enter 'a' or 'b' to launch a conversion
  on either sequence A or B.

* If hardware triggering is chosen, then a GPIO write occurs, causing a 1-to-0
  transition on the appropriate ADC_PINTRIG input, which launches the
  conversion sequence.

* If software triggering is chosen, then a '1' is written to the start bit in
  the SEQ_CTRL register, which launches the conversion sequence.

* Code then waits for a handshake flag that will be set by the interrupt
  service routine. 

* In each sequence's interrupt service routine, the converted result for each channel
  assigned to that sequence is stored in an array for main.

* A handshake flag is set, the interrupt flag is cleared, and the ISR returns.

* The 12-bit converted results for each channel assigned to the sequence are printed to the terminal, 
  and the main loop repeats.



External connections:
* OUTPORT_B must be connected externally to INPORT_B. See the #defines in main.c
* OUTPORT_A must be connected externally to INPORT_A. See the #defines in main.c
* Pins associated with ADC_1 - ADC_7, ADC_9 - ADC_11 can be tied high, or tied low,
  or have voltage dividers applied to them, or driven by an external voltage source. 
  Otherwise, these ADC inputs will be floating and the converted values will be variable.



Program Flow:
* This example runs with main_clk = 30 MHz. See chip_setup.h and SystemInit()
* This example runs from Flash. 


To run this code:
* Build, Debug, Run.
or
* Download to flash, then reset.


Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.


