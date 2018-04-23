${ProjName}:
I2C_MasterSlave


Intended purpose:
* To demonstrate the configuration and use of the I2C-bus interface in both master and slave modes.
* User input is via the debug UART.
* Connect a terminal emulator (9600/8/N/1) to the debug UART COM port.
* See chip_setup.h and Serial.c to setup the debug com port.


Functional description:
* In this example we configure one of the I2C bus interfaces as both an I2C master and slave simultaneously.
* The I2C slave (called the self-slave) will be given an address in the SLAVEADDR3 register.
* In the main while(1) loop, the user is prompted to enter a string, and
  the result is stored in an array.
* The I2C master transmits the string to the self-slave, which stores the data in a second array.
* When the NUL string terminator is reached, the received data are printed, and the process repeats.



External connections:
* None (see note).
* note: This example uses I2C0 which is only available on the true open-drain pads P0.10 and P0.11.
        The MAX board has pullups on these pins. If you use another I2C instance, the addition of 
        external pullups on the pins you choose may be required.



Program Flow:
* This example runs at system_ahb_clk = 30 MHz. See SystemInit() and chip_setup.h.
* This example runs from Flash. 


To run this code:
* Build, Debug, Run.
or
* Download to flash, then reset.


Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      



