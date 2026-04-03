# ATmegaProg
Turn your Arduino Mega 2560 board into a universal programmer

The idea of ​​this project was to create the most universal and, above all, simple programmer for AVR microprocessors. To support as many processors as possible, my requirement was a ZIF40 socket and programming using a high voltage of 12V. I chose high voltage support so that it would be possible to save microprocessors that were disabled by incorrect fuse settings, or if you previously needed another pin by melting the RSTDISBL fuse and you need to reload a new program. This required programming software that supports the ISP (In-System Programming), HVPP (High Voltage Parallel Programming) and HVSP (High Voltage Serial Programming) protocols. My requirement was also PC software support (Avrdude, Atmel Studio, MPLAB IDE). For this compatibility, I chose the STK500 version 2 communication protocol, which is described in the AVR068 document on the Microchip website, and is supported by all of the aforementioned programs. The STK500 protocol communicates with a PC via a serial interface, which greatly simplifies the resulting programmer. You can use an Arduino board that has a built-in USB -> Serial converter for programming and monitoring. I chose the Arduino Mega 2560 board for a sufficient number of pins. The resulting programmer board contains only 4 IOs, a few diodes, resistors and capacitors. A somewhat experienced electronics technician can solder the board himself (SMT soldering is required). The entire programmer is then simply inserted into the Arduino Mega 2560 board, and after uploading the firmware to the mega2560, it can be used immediately. The versatility of the programmer board also predetermines it for the implementation of other protocols such as TPI, PDI and UPDI. These will be added gradually.

## Main features:
- support for Avrdude, Atmel Studio, MPLAB IDE and others that support the STK500v2 protocol.
- support for ISP, HVPP and HVSP protocols.
- ZIF40 socket for various case sizes from 8 pin to 40 pin.
- using the built-in 6 pin ISP connector, it is possible to program MCUs in larger packages.
- the built-in HV 12V power supply can be programmatically applied to one of the five pins of the ZIF socket (1, 4, 9, 10 and 31).
- connection of a small display to display the programmer status.
- autodetection of the package size.

## High voltage switch example:
<img src="https://github.com/rocob/ATmegaProg/blob/main/images/4053_Example.webp" width="350">
A CMOS 4053 switch is used to switch the pin between the mega2560 MCU and the High Voltage 12V. By default, the pin is connected to the mega2560, which is ensured by a logic level of 1 using a resistor R. After applying Log. 0 to the SEL input, the TARGET pin is switched to HV +12V. This is how pins 1, 4, 9, 10 and 31 of the ZIF socket are connected.

## Currently supported microprocessors:
```
  // ATmega 8 (DIP28)
  {0x93, 0x07, 1 , 0 , "ATmega8/L/A"    },

  // ATmega 48 / 88 / 168 / 328 (DIP28)
  {0x92, 0x05, 1 , 0 , "ATmega48/A"     },
  {0x92, 0x0A, 1 , 0 , "ATmega48P"      },
  {0x92, 0x10, 1 , 0 , "ATmega48PB"     },
  {0x93, 0x0A, 1 , 0 , "ATmega88/A"     },
  {0x93, 0x0F, 1 , 0 , "ATmega88P"      },
  {0x93, 0x16, 1 , 0 , "ATmega88PB"     },
  {0x94, 0x06, 1 , 0 , "ATmega168/A"    },
  {0x94, 0x0B, 1 , 0 , "ATmega168P"     },
  {0x94, 0x15, 1 , 0 , "ATmega168PB"    },
  {0x95, 0x14, 1 , 0 , "ATmega328"      },
  {0x95, 0x0F, 1 , 0 , "ATmega328P"     },  // 328P tested ISP, HVPP
  {0x95, 0x16, 1 , 0 , "ATmega328PB"    },

  // ATtiny 28 (DIP28)
  {0x91, 0x07, 1 , 0 , "ATtiny28L/V"    },  // only HVPP, not yet tested

  // ATtiny 48 / 88 (DIP28)
  {0x92, 0x09, 1 , 0 , "ATtiny48"       },  // not yet tested
  {0x93, 0x11, 1 , 0 , "ATtiny88"       },
  
  // ATtiny 2313 / 4313 (DIP20)
  {0x91, 0x0A, 2 , 0 , "ATtiny2313/A"   },
  {0x92, 0x0D, 2 , 0 , "ATtiny4313"     },  // 4313 tested ISP, HVPP

  // ATtiny 26 / 261 / 461 / 861 (DIP20)
  {0x91, 0x09, 3 , 0 , "ATtiny26/L"     },  // 26L tested ISP, HVPP
  {0x91, 0x0C, 3 , 0 , "ATtiny261/A"    },
  {0x92, 0x08, 3 , 0 , "ATtiny461/A"    },
  {0x93, 0x0D, 3 , 0 , "ATtiny861/A"    },

  // ATtiny 24 / 44 / 84 (DIP14)
  {0x91, 0x0B, 4 , 0 , "ATtiny24/A/V"   },
  {0x92, 0x07, 4 , 0 , "ATtiny44/A/V"   },
  {0x93, 0x0C, 4 , 0 , "ATtiny84/A/V"   },  // 84A tested ISP, HVSP

  // ATtiny 11 / 12 (DIP8)
  {0x90, 0x04, 5 , 0 , "ATtiny11/L"     },  // only HVSP, not yet tested
  {0x90, 0x05, 5 , 0 , "ATtiny12/L/V"   },  // not yet tested

  // ATtiny 15 (DIP8)
  // for HVSP: SCI is pin 3 and requires 16 pulses per 1 SCI clock
  {0x90, 0x06, 5 , 0 , "ATtiny15/L"     },  // not yet tested

  // ATtiny 13A (DIP8)
  {0x90, 0x07, 5 , 0 , "ATtiny13/A"     },  // not yet tested

  // ATtiny 22 (DIP8)
  {0x91, 0x06, 5 , 0 , "ATtiny22/L"     },  // not yet tested

  // ATtiny 25 / 45 / 85 (DIP8)
  {0x91, 0x08, 5 , 0 , "ATtiny25/V"     },  // 25 tested ISP, HVSP
  {0x92, 0x06, 5 , 0 , "ATtiny45/V"     },
  {0x93, 0x0B, 5 , 0 , "ATtiny85/V"     },

  // ATtiny 43U         // SO20
  // ATtiny 87 / 167    // SO20
  // ATtiny 441 / 841   // SO14
  // ATtiny 828         // TQFP32
  // ATtiny 1634        // SO20

  // ATmega 16 / 32 / 8535 (DIP40)
  {0x94, 0x03, 6 , 0 , "ATmega16/A"     },  // not yet tested
  {0x95, 0x02, 6 , 0 , "ATmega32/A"     },
  {0x93, 0x08, 6 , 0 , "ATmega8535/L"   },

  // ATmega 164 / 324 / 644 / 1284 (DIP40)
  {0x94, 0x0A, 6 , 0 , "ATmega164P/PA"  },  // not yet tested
  {0x94, 0x0F, 6 , 0 , "ATmega164A"     },
  {0x95, 0x08, 6 , 0 , "ATmega324P"     },
  {0x95, 0x15, 6 , 0 , "ATmega324A"     },
  {0x95, 0x11, 6 , 0 , "ATmega324PA"    },
  {0x95, 0x17, 6 , 0 , "ATmega324PB"    },
  {0x96, 0x0A, 6 , 0 , "ATmega644P/PA"  },
  {0x96, 0x09, 6 , 0 , "ATmega644A"     },
  {0x97, 0x05, 6 , 0 , "ATmega1284P"    },
  {0x97, 0x06, 6 , 0 , "ATmega1284"     },

  // ATmega 162 / 8515  (DIP40)
  {0x94, 0x04, 7 , 0 , "ATmega162/V"    },  // not yet tested
  {0x93, 0x06, 7 , 0 , "ATmega8515/L"   },

  // ATtiny 102 / 104 (TPI)
  {0x90, 0x0C, 8 , 0 , "ATtiny102"      },  // SO8 - is being prepared
  {0x90, 0x0B, 9 , 0 , "ATtiny104"      }   // SO14
  // ATtiny4 0x1E 0x8F 0x0A   // SOT23-6
  // ATtiny5 0x1E 0x8F 0x09
  // ATtiny9 0x1E 0x90 0x08
  // ATtiny10 0x1E 0x90 0x03
  // ATtiny20 0x1E 0x91 0x0F  // SO14, TSSOP14
  // ATtiny40 0x1E 0x92 0x0E  // SO20, TSSOP20

```
