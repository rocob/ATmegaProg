# ATmegaProg
Turn your Arduino Mega 2560 board into a universal programmer

The idea of ​​this project was to create the most universal and, above all, simple programmer for AVR microprocessors. To support as many processors as possible, my requirement was a ZIF40 socket and programming using a high voltage of 12V. I chose high voltage support so that it would be possible to save microprocessors that were disabled by incorrect fuse settings, or if you previously needed another pin by melting the RSTDISBL fuse and you need to reload a new program. This required programming software that supports the ISP (In-System Programming), HVPP (High Voltage Parallel Programming) and HVSP (High Voltage Serial Programming) protocols. My requirement was also PC software support (Avrdude, Atmel Studio). For this compatibility, I chose the STK500 version 2 communication protocol, which is described in the AVR068 document on the Microchip website, and is supported by all of the aforementioned programs. The STK500 protocol communicates with a PC via a serial interface, which greatly simplifies the resulting programmer. You can use an Arduino board that has a built-in USB -> Serial converter for programming and monitoring. I chose the Arduino Mega 2560 board for a sufficient number of pins. The resulting programmer board contains only 4 IOs, a few diodes, resistors and capacitors. A somewhat experienced electronics technician can solder the board himself (SMT soldering is required). The entire programmer is then simply inserted into the Arduino Mega 2560 board, and after uploading the firmware to the mega2560, it can be used immediately. The versatility of the programmer board also predetermines it for the implementation of other protocols such as TPI, PDI and UPDI. These will be added gradually.

## Main features:
- support for Avrdude, Atmel Studio and others that support the STK500v2 protocol.
- support for ISP, HVPP and HVSP protocols.
- ZIF40 socket for various package sizes from 8 pin to 40 pin.
- using the built-in 6 pin ISP connector, it is possible to program MCUs in larger packages.
- the built-in HV 12V power supply can be programmatically applied to one of the five pins of the ZIF socket (1, 4, 9, 10 and 31).
- connection of a small display to display the programmer status.
- autodetection of the package size.

## High voltage switch example:
<img src="https://github.com/rocob/ATmegaProg/blob/main/images/4053_Example.webp" width="350">
A CMOS 4053 switch is used to switch the pin between the mega2560 MCU and the High Voltage 12V. By default, the pin is connected to the mega2560, which is ensured by a logic level of 1 using a resistor R. After applying Log. 0 to the SEL input, the TARGET pin is switched to HV +12V. This is how pins 1, 4, 9, 10 and 31 of the ZIF socket are connected.

## Currently supported microprocessors:
```
ATmega168/A     tested ISP, HVPP
ATmega168P      tested ISP, HVPP
ATmega168PB
ATmega328       tested ISP, HVPP
ATmega328P      tested ISP, HVPP
ATmega328PB
ATmega48/A      tested ISP, HVPP
ATmega48P       tested ISP, HVPP
ATmega48PB
ATmega8/L/A     tested ISP, HVPP
ATmega88/A      tested ISP, HVPP
ATmega88P       tested ISP, HVPP
ATmega88PB

ATtiny11/L      only HVSP, not yet tested
ATtiny12/L/V
ATtiny13/A
ATtiny15/L
ATtiny22/L
ATtiny2313/A    tested ISP, HVPP
ATtiny24/A/V    tested ISP, HVPP
ATtiny25/V"     tested ISP, HVSP
ATtiny26/L      tested ISP, HVPP
ATtiny261/A
ATtiny28L/V     only HVPP, not yet tested
ATtiny4313      tested ISP, HVPP
ATtiny44/A/V    tested ISP, HVPP
ATtiny45/V      tested ISP, HVPP
ATtiny461/A
ATtiny48
ATtiny84/A/V    tested ISP, HVSP
ATtiny85/V      tested ISP, HVPP
ATtiny88
ATtiny861/A

// big mega, not yet tested
ATmega16/A
ATmega32/A
ATmega8535/L
ATmega164P/PA
ATmega164A
ATmega324P
ATmega324A
ATmega324PA
ATmega324PB
ATmega644P/PA
ATmega644A
ATmega1284P
ATmega1284
ATmega162/V
ATmega8515/L



// other tiny, not yet tested
ATtiny43U         // SO20
ATtiny87          // SO20
ATtiny167         // SO20
ATtiny441         // SO14
ATtiny841         // SO14
ATtiny828         // TQFP32
ATtiny1634        // SO20

// ATtiny 102 / 104 (TPI) - being prepared
ATtiny4          // SOT23-6
ATtiny5
ATtiny9
ATtiny10
ATtiny102        // SO8
ATtiny104        // SO14
ATtiny20         // SO14, TSSOP14
ATtiny40         // SO20, TSSOP20

```
## Bugs

Users are encouraged to post their bug reports using git issues or opening an issue in git:

https://github.com/rocob/ATmegaProg/issues

All problem reports must be accompanied by a proper problem definition.
This should include how the problem occurred, the command line and
output from the programmer showing the startup messages and any errors.
A history is also useful, ie did it work before.

## Donations

ATmegaProg has no fees of any kind but donations are accepted.

BTC: 16JvqnECPTZM9kr7VhNH2iUmBxb7WmnUsR

Happy programming!
