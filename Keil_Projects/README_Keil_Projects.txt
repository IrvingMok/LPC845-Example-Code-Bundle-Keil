Tool chain versions supported
=============================
- This package was built using Keil uVision V5.23.0.0



Steps for building this workspace when it's clean
=================================================
1. Launch Keil and open the .uvmpw file in this directory (or double-click the file from Windows).
2. Go to Project/Batch_Build
3. Select All Project Targets, and click Rebuild

- Otherwise, the library projects (*_lib) must be individually built before the example projects will build.



Note about using the debug COM port
=================================================================
Many projects in this package are interactive and communicate with the user
via a terminal emulator running on the host PC, connected either to the VCOM
serial port, or to a USB-to-UART cable connected to pins on the Arduino header.

It is recommended to run the project UART0_Terminal first, in order to
establish that communication between the terminal emulator (via the VCOM
serial port and/or the USB COM port w/ USB-to-UART cable) and the LPC8xx are
working.

* To use the VCOM Serial Port:
    1. No external connections are necessary.
    2. In the project's inc/chip_setup.h, near the bottom, make sure there is 
       #define USE_VCOM_PORT 1

* To use a USB-to-RS232 breakout cable:
    1. There are three external connections necessary: UART RXD and TXD based
       on the #defines near the bottom of the project's inc/chip_setup.h, plus
       a ground connection:
       DBGTXPIN = breakout cable RXD
       DBGRXPIN = breakout cable TXD
       Board GND = breakout cable GND


Note about the Device and Target in Project >  Options for target 'flash' > Device | Target
===========================================================================================
The project files in this workspace were built before Keil had built-in support
for the LPC84x device. 
Therefore, the Device has been set to ARM / ARM Cortex M0 plus / ARMCM0P
Target > IRAM1 size has been set to 0x4000
Target > IROM1 size has been set to 0x10000



