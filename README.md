# ATmegaProg
AVR Programmer for Arduino Mega 2560 boards

The idea of ​​this project was to create the most universal and, above all, simple programmer for AVR microprocessors. To support as many processors as possible, my requirement was a ZIF40 socket and programming using a high voltage of 12V. I chose high voltage support so that it would be possible to save microprocessors that were disabled by incorrect fuse settings, or if you previously needed another pin by melting the RSTDISBL fuse and you need to reload a new program. This required programming software that supports the ISP (In-System Programming), HVPP (High Voltage Parallel Programming) and HVSP (High Voltage Serial Programming) protocols. My requirement was also PC software support (Avrdude, Atmel Studio, MPLAB IDE). For this compatibility, I chose the STK500 version 2 communication protocol, which is described in the AVR068 document on the Microchip website, and is supported by all of the aforementioned programs. The STK500 protocol communicates with a PC via a serial interface, which greatly simplifies the resulting programmer. You can use an Arduino board that has a built-in USB -> Serial converter for programming and monitoring. I chose the Arduino Mega 2560 board for a sufficient number of pins. The resulting programmer board contains only 4 IOs, a few diodes, resistors and capacitors. A somewhat experienced electronics technician can solder the board himself (SMT soldering is required). The entire programmer is then simply inserted into the Arduino Mega 2560 board, and after uploading the firmware to the mega2560, it can be used immediately. The versatility of the programmer board also predetermines it for the implementation of other protocols such as TPI, PDI and UPDI. These will be added gradually.

## Main features:
- support for Avrdude, Atmel Studio, MPLAB IDE and others that support the STK500v2 protocol.
- support for ISP, HVPP and HVSP protocols.
- ZIF40 socket for various case sizes from 8 pin to 40 pin.
- using the built-in 6 pin ISP connector, it is possible to program MCUs in larger packages.
- the built-in HV 12V power supply can be programmatically applied to one of the five pins of the ZIF socket (1, 4, 9, 10 and 31).
- connection of a small display to display the programmer status.
- autodetection of the package size.

## Currently supported microprocessors:
```
  // ATmega 8 (DIP28)
  {0x93, 0x07, 0 , 0 , "ATmega8/L/A"    },

  // ATmega 48 / 88 / 168 / 328 (DIP28)
  {0x92, 0x05, 0 , 0 , "ATmega48/A"     },
  {0x92, 0x0A, 0 , 0 , "ATmega48P"      },
  {0x92, 0x10, 0 , 0 , "ATmega48PB"     },
  {0x93, 0x0A, 0 , 0 , "ATmega88/A"     },
  {0x93, 0x0F, 0 , 0 , "ATmega88P"      },
  {0x93, 0x16, 0 , 0 , "ATmega88PB"     },
  {0x94, 0x06, 0 , 0 , "ATmega168/A"    },
  {0x94, 0x0B, 0 , 0 , "ATmega168P"     },
  {0x94, 0x15, 0 , 0 , "ATmega168PB"    },
  {0x95, 0x14, 0 , 0 , "ATmega328"      },
  {0x95, 0x0F, 0 , 0 , "ATmega328P"     },  // 328P tested ISP, HVPP
  {0x95, 0x16, 0 , 0 , "ATmega328PB"    },

  // ATtiny 48 / 88 (DIP28)
  {0x92, 0x09, 0 , 0 , "ATtiny48"       },  // not yet tested
  {0x93, 0x11, 0 , 0 , "ATtiny88"       },
  
  // ATtiny 2313 / 4313 (DIP20)
  {0x91, 0x0A, 1 , 0 , "ATtiny2313/A"   },
  {0x92, 0x0D, 1 , 0 , "ATtiny4313"     },  // 4313 tested ISP, HVPP

  // ATtiny 26 / 261 / 461 / 861 (DIP20)
  {0x91, 0x09, 2 , 0 , "ATtiny26/L"     },  // 26L tested ISP, HVPP
  {0x91, 0x0C, 2 , 0 , "ATtiny261/A"    },
  {0x92, 0x08, 2 , 0 , "ATtiny461/A"    },
  {0x93, 0x0D, 2 , 0 , "ATtiny861/A"    },

  // ATtiny 24 / 44 / 84 (DIP14)
  {0x91, 0x0B, 3 , 0 , "ATtiny24/A"     },
  {0x92, 0x07, 3 , 0 , "ATtiny44/A"     },
  {0x93, 0x0C, 3 , 0 , "ATtiny84/A"     },  // 84A tested ISP, HVSP

  // ATtiny 25 / 45 / 85 (DIP8)
  {0x91, 0x08, 4 , 0 , "ATtiny25/V"     },  // 25 tested ISP, HVSP
  {0x92, 0x06, 4 , 0 , "ATtiny45/V"     },
  {0x93, 0x0B, 4 , 0 , "ATtiny85/V"     },

  // ATmega 16 / 32 / 8535 (DIP40)
  {0x94, 0x03, 5 , 0 , "ATmega16/A"     }, // not yet tested
  {0x95, 0x02, 5 , 0 , "ATmega32/A"     },
  {0x93, 0x08, 5 , 0 , "ATmega8535/L"   },

  // ATmega 164 / 324 / 644 / 1284 (DIP40)
  {0x94, 0x0A, 5 , 0 , "ATmega164P/PA"  }, // not yet tested
  {0x94, 0x0F, 5 , 0 , "ATmega164A"     },
  {0x95, 0x08, 5 , 0 , "ATmega324P"     },
  {0x95, 0x15, 5 , 0 , "ATmega324A"     },
  {0x95, 0x11, 5 , 0 , "ATmega324PA"    },
  {0x95, 0x17, 5 , 0 , "ATmega324PB"    },
  {0x96, 0x0A, 5 , 0 , "ATmega644P/PA"  },
  {0x96, 0x09, 5 , 0 , "ATmega644A"     },
  {0x97, 0x05, 5 , 0 , "ATmega1284P"    },
  {0x97, 0x06, 5 , 0 , "ATmega1284"     },

  // ATmega 162 / 8515  (DIP40)
  {0x94, 0x04, 6 , 0 , "ATmega162/V"    }, // not yet tested
  {0x93, 0x06, 6 , 0 , "ATmega8515/L"   },

  // ATtiny 13A (DIP8)
  {0x90, 0x07, 4 , 0 , "ATtiny13/A"     }, // not yet tested

  // ATtiny 102 / 104 (TPI)
  {0x90, 0x0C, 8 , 0 , "ATtiny102"      },  // DIL8 - is being prepared
  {0x90, 0x0B, 9 , 0 , "ATtiny104"      }   // DIL14
```
