/*
  avr_setup.h
  part of ATmegaProg

  created 2026.03.03
  by Robert Kovaľ <http://www.toroproduction.sk>
  modified 2026.04.12
  by Robert Kovaľ

  This is private source code
  Copyright 2025-2026 Robert Kovaľ and TORO-Production s.r.o.
  This program is Free Software and has ABSOLUTELY NO WARRANTY
  Please report bugs to mail@rocob.biz
  See LICENSE.txt for details.
*/

#include "Arduino.h"
#include <avr/pgmspace.h>

struct AVRDevice {
  uint8_t sig2;   // second byte signature
  uint8_t sig3;   // third  byte signature
  uint8_t pack1;  // package type 1
  uint8_t pack2;  // not used, (future)
  char name[16];  // MCU name (max 15 characters + \0)
};

const AVRDevice signatureTable[] PROGMEM = {

  //***************************************************************************
  //* MCS-51               Old AT89 non AVR models                            *
  //***************************************************************************


  // AT89 (DIP40, TQFP44, PLCC44)
  {0x51, 0x06, 0 , 0 , "AT89S51"        },  // 0x000, 0x100, 0x200
  {0x51, 0x07, 0 , 0 , "AT89C51RC"      },  // 0x000, 0x100, 0x200
  {0x52, 0x06, 0 , 0 , "AT89S52"        },  // 0x000, 0x100, 0x200
  {0x55, 0x06, 0 , 0 , "AT89C55WD"      },  // 0x000, 0x100, 0x200
  {0x61, 0x06, 0 , 0 , "AT89LS51"       },  // 0x000, 0x100, 0x200
  {0x62, 0x06, 0 , 0 , "AT89LS52"       },  // 0x000, 0x100, 0x200
  {0x63, 0x00, 0 , 0 , "AT89LS53"       },  // 0x30, 0x31
  {0x53, 0x00, 0 , 0 , "AT89S53"        },  // 0x30, 0x31
  {0x72, 0x00, 0 , 0 , "AT89S8252"      },  // 0x30, 0x31
  {0x73, 0x00, 0 , 0 , "AT89S8253"      },  // 0x30, 0x31

  {0x51, 0xFF, 0 , 0 , "AT89C51"        },  // 0x30, 0x31, 0x32
  {0x51, 0x05, 0 , 0 , "AT89C51-5"      },  // 0x30, 0x31, 0x32
  {0x52, 0xFF, 0 , 0 , "AT89C52"        },  // 0x30, 0x31, 0x32
  {0x52, 0x05, 0 , 0 , "AT89C52-5"      },  // 0x30, 0x31, 0x32
  {0x55, 0xFF, 0 , 0 , "AT89C55"        },  // 0x30, 0x31, 0x32
  {0x55, 0x05, 0 , 0 , "AT89C55-5"      },  // 0x30, 0x31, 0x32

  {0x61, 0xFF, 0 , 0 , "AT89LV51"       },  // 0x30, 0x31, 0x32
  {0x62, 0xFF, 0 , 0 , "AT89LV52"       },  // 0x30, 0x31, 0x32
  {0x65, 0xFF, 0 , 0 , "AT89LV55"       },  // 0x30, 0x31, 0x32
  {0x54, 0x05, 0 , 0 , "AT89LP51"       },  // 0x00, 0x01, 0x02
  {0x54, 0x06, 0 , 0 , "AT89LP52"       },  // 0x00, 0x01, 0x02
  {0x32, 0xFC, 0 , 0 , "AT89LP3240"     },  // 0x00, 0x01, 0x02
  {0x64, 0xFF, 0 , 0 , "AT89LP6440"     },  // 0x00, 0x01, 0x02

  // AT89 (DIP14, TSSOP14)
  {0x27, 0xFF, 0 , 0 , "AT89LP213"      },  // 0x00, 0x01, 0x02
  {0x28, 0xFF, 0 , 0 , "AT89LP214"      },  // 0x00, 0x01, 0x02

  // AT89 (DIP16, SOIC16, TSSOP16)
  {0x29, 0xFF, 0 , 0 , "AT89LP216"      },  // 0x00, 0x01, 0x02

  // AT89 (DIP20, SOIC20)
  {0x11, 0x00, 0 , 0 , "AT89C1051"      },  // 0x00, 0x01
  {0x21, 0x00, 0 , 0 , "AT89C2051"      },  // 0x00, 0x01
  {0x41, 0x00, 0 , 0 , "AT89C4051"      },  // 0x00, 0x01
  {0x23, 0xFF, 0 , 0 , "AT89S2051"      },  // 0x00, 0x01, 0x02
  {0x43, 0xFF, 0 , 0 , "AT89S4051"      },  // 0x00, 0x01, 0x02
  {0x25, 0xFF, 0 , 0 , "AT89LP2052"     },  // 0x00, 0x01, 0x02
  {0x45, 0xFF, 0 , 0 , "AT89LP4052"     },  // 0x00, 0x01, 0x02

  // AT89 (DIP28, TQFP32, PLCC32, MLF32)
  {0x40, 0xFF, 0 , 0 , "AT89LP428"      },  // 0x00, 0x01, 0x02
  {0x42, 0xFF, 0 , 0 , "AT89LP828"      },  // 0x00, 0x01, 0x02





  //***************************************************************************
  //* AVR                       Old AT90 models                               *
  //***************************************************************************
  
  // AT90 (DIP20, SOIC20)
  {0x90, 0x01, 2 , 0 , "AT90S1200/A"    },  // chip erase no polling
  {0x91, 0x01, 2 , 0 , "AT90S2313"      },  // 
  
  // AT90 (DIP8, SOIC8)
  {0x91, 0x02, 5 , 0 , "AT90S2323"      },  // 
  {0x91, 0x03, 5 , 0 , "AT90S2343"      },  // 

  // AT90 (DIP28, TQFP32)
  {0x91, 0x05, 1 , 0 , "AT90S2333"      },  // 
  {0x92, 0x03, 1 , 0 , "AT90S4433"      },  // 

  // AT90 (DIP40, TQFP44, PLCC44)
  {0x92, 0x01, 7 , 0 , "AT90S4414"      },  // 
  {0x93, 0x01, 7 , 0 , "AT90S8515"      },  // 
  {0x92, 0x02, 6 , 0 , "AT90S4434"      },  // 
  {0x93, 0x03, 6 , 0 , "AT90S8535"      },  // 

  // AT90 (USB)
  {0x93, 0x82, 0 , 0 , "AT90USB82"      },  // QFN32
  {0x94, 0x82, 0 , 0 , "AT90USB162"     },  // QFN32, TQFP32
  {0x96, 0x82, 0 , 0 , "AT90USB646/7"   },  // QFN64, TQFP64
  {0x97, 0x82, 0 , 0 , "AT90USB1286/7"  },  // QFN64, TQFP64, VQFN64

  /* AT90 (CAN)
    0x1e 0x95 0x81; AT90CAN32 TQFP64
    0x1e 0x96 0x81; AT90CAN64 TQFP64
    0x1e 0x97 0x81; AT90CAN128 TQFP64
  */

  /* AT90 (PWM)
    0x1e 0x93 0x88; AT90PWM81 QFN32, SOIC20
    0x1e 0x94 0x8b; AT90PWM161 QFN32, SOIC20
    0x1e 0x93 0x83; AT90PWM1 QFN32, SOIC24
    0x1e 0x93 0x81; AT90PWM2 SOIC24
    0x1e 0x93 0x83; AT90PWM2B SOIC32, SOIC24
    0x1e 0x93 0x81; AT90PWM3 QFN32, SOIC32
    0x1e 0x93 0x83; AT90PWM3B QFN32, SOIC32
    0x1e 0x94 0x83; AT90PWM216 SOIC24
                    AT90PWM316 QFN32, SOIC32
  */


  //***************************************************************************
  //* AVR                       ATtiny models                                 *
  //***************************************************************************

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

  /* others models - unsorted
  {0x92, 0x0C, 0 , 0 , "ATtiny43U"      },  // SO20
  {0x93, 0x87, 0 , 0 , "ATtiny87"       },  // SO20
  {0x94, 0x87, 0 , 0 , "ATtiny167"      },  // SO20
  {0x92, 0x15, 0 , 0 , "ATtiny441"      },  // SO14
  {0x93, 0x15, 0 , 0 , "ATtiny841"      },  // SO14
  {0x93, 0x14, 0 , 0 , "ATtiny828"      },  // TQFP32
  {0x94, 0x12, 0 , 0 , "ATtiny1634"     },  // SO20
  */


  //***************************************************************************
  //* AVR                       ATmega models                                 *
  //***************************************************************************

  // ATmega 8 (DIP28, TQFP32)
  {0x93, 0x07, 1 , 0 , "ATmega8/L/A"    },  // DIP28, TQFP32, QFN32, VQFN32, MLF32

  // ATmega 48 / 88 / 168 / 328 (DIP28, TQFP32)
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

  // ATmega 161 / 163 / 323 (DIP40)
  {0x94, 0x01, 0 , 0 , "ATmega161/L"    },  // DIP40, TQFP44
  {0x94, 0x02, 0 , 0 , "ATmega163/L"    },  // DIP40, TQFP44
  {0x95, 0x01, 0 , 0 , "ATmega323/L"    },  // DIP40, TQFP44

  // ATmega 16 / 32 / 8535 (DIP40)
  {0x94, 0x03, 6 , 0 , "ATmega16/L/A"   },  // DIP40, TQFP44, VQFN44, MLF44
  {0x95, 0x02, 6 , 0 , "ATmega32/L/A"   },  // DIP40, TQFP44, VQFN44, MLF44
  {0x93, 0x08, 6 , 0 , "ATmega8535/L"   },  // DIP40, TQFP44, PLCC44, QFN44, VFQFN44, MLF44
  
  // ATmega 162 / 8515  (DIP40)
  {0x94, 0x04, 7 , 0 , "ATmega162/V"    },  // DIP40, TQFP44, VFQFN44
  {0x93, 0x06, 7 , 0 , "ATmega8515/L"   },  // DIP40, TQFP44, PLCC44, QFN44, VFQFN44, MLF44

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
  
  // special models
  // 0x93, 0x10, ATmega8HVA   WFLGA36, TSSOP28
  // 0x94, 0x0C, ATmega16HVA  WFLGA36, TSSOP28
  // 0x94, 0x0D, ATmega16HVB  TFSOP44
  // 0x95, 0x10, ATmega32HVB  TFSOP44
  // 0x95, 0x13, ATmega32HVE2 VFQFN48
  // 0x96, 0x10, ATmega64HVE2 VFQFN48

  // 0x1e 0x94 0x84;  ATmega16M1  TQFP32, QFN32
  // 0x1e 0x95 0x84;  ATmega32M1  TQFP32, QFN32
  // 0x1e 0x95 0x86;  ATmega32C1  TQFP32, QFN32, VQFN32
  // 0x1e 0x96 0x84;  ATmega64M1  TQFP32, QFN32, VQFN32
  // 0x1e 0x96 0x86;  ATmega64C1  TQFP32, QFN32, VQFN32
  // 

  //***************************************************************************
  //* AVR                     BIG ATmega models                               *
  //***************************************************************************

  // ATmega 406 (LQFP48) with 4 cells battery managment
  {0x95, 0x07, 0 , 0 , "ATmega406"      },  // atyp with 40kB flash

  // ATmega 64 / 128 (TQFP64)
  {0x97, 0x01, 0 , 0 , "ATmega103"      },

  {0x96, 0x02, 0 , 0 , "ATmega64/L"     },  // TQFP64, QFN64, VQFN64, MLF64
  {0x97, 0x02, 0 , 0 , "ATmega128/L"    },  // TQFP64, MLF64
  
  {0x97, 0x04, 0 , 0 , "ATmega1281"     }, // TQFP64, QFN64, MLF64
  {0x98, 0x02, 0 , 0 , "ATmega2561"     }, // TQFP64, QFN64, MLF64
  
  {0x94, 0x05, 0 , 0 , "ATmega169/L/V"  },
  {0x94, 0x11, 0 , 0 , "ATmega169A"     },
  {0x95, 0x03, 0 , 0 , "ATmega329/V"    },
  {0x95, 0x0B, 0 , 0 , "ATmega329P"     },
  {0x96, 0x03, 0 , 0 , "ATmega649/V"    },
  {0x96, 0x0B, 0 , 0 , "ATmega649P"     },
  
  {0x94, 0x07, 0 , 0 , "ATmega165/V"    },
  {0x94, 0x10, 0 , 0 , "ATmega165P"     },
  {0x95, 0x05, 0 , 0 , "ATmega325/V"    },
  {0x95, 0x0D, 0 , 0 , "ATmega325P"     },
  {0x96, 0x05, 0 , 0 , "ATmega645/V"    },
  {0x96, 0x0D, 0 , 0 , "ATmega645P"     },
  
  // ATmega 640 / 1280 / 2560 (TQFP100)
  {0x96, 0x08, 0 , 0 , "ATmega640"      },  // TQFP100, CBGA100, TFBGA100
  {0x97, 0x03, 0 , 0 , "ATmega1280"     },  // TQFP100, CBGA100
  {0x98, 0x01, 0 , 0 , "ATmega2560"     },  // TQFP100, CBGA100

  {0x95, 0x04, 0 , 0 , "ATmega3290/V"   },
  {0x95, 0x0C, 0 , 0 , "ATmega3290P"    },
  {0x96, 0x04, 0 , 0 , "ATmega6490/V"   },
  {0x96, 0x0C, 0 , 0 , "ATmega6490P"    },
  
  {0x95, 0x06, 0 , 0 , "ATmega3250/A"   },
  {0x95, 0x0E, 0 , 0 , "ATmega3250P"    },
  {0x96, 0x06, 0 , 0 , "ATmega6450/A"   },
  {0x96, 0x0E, 0 , 0 , "ATmega6450P"    },
  
  // USB models
  {0x93, 0x89, 0 , 0 , "ATmega8U2"      },  // TQFP32, QFN32, LQFP32, VQFN32
  {0x94, 0x89, 0 , 0 , "ATmega16U2"     },  // TQFP32, QFN32
  {0x95, 0x8A, 0 , 0 , "ATmega32U2"     },  // TQFP32, QFN32
  {0x94, 0x88, 0 , 0 , "ATmega16U4"     },  // TQFP44, QFN44, VQFN44
  {0x95, 0x87, 0 , 0 , "ATmega32U4"     },  // TQFP44, QFN44

  /* special models for in-vehicle LIN networks
    0x1e 0x93 0x0a; ATA6612C    VFQFN48 (mega88)
    0x1e 0x94 0x06; ATA6613C    VFQFN48 (mega168)
    0x1e 0x95 0x0F; ATA6614Q    VFQFN48 (mega328P)
    0x1e 0x93 0x87; ATA6616C    VFQFN38
    0x1e 0x94 0x87; ATA6617C    VFQFN38
    0x1e 0x94 0x87; ATA5505     VFQFN38
    0x1e 0x94 0x87; ATA664251   VFQFN38
    
  */

  //***************************************************************************
  //*                        ATtiny new TPI models                            *
  //***************************************************************************

  // ATtiny 102 / 104 (TPI)
  {0x90, 0x0C, 8 , 0 , "ATtiny102"      },  // SO8 - is being prepared
  {0x90, 0x0B, 9 , 0 , "ATtiny104"      },  // SO14
  // 0x1E 0x8F 0x0A ATtiny4   // SOT23-6
  // 0x1E 0x8F 0x09 ATtiny5 
  // 0x1E 0x90 0x08 ATtiny9 
  // 0x1E 0x90 0x03 ATtiny10 
  // 0x1E 0x91 0x0F ATtiny20  // SO14, TSSOP14
  // 0x1E 0x92 0x0E ATtiny40  // SO20, TSSOP20
  
  // Logic Green parts
  {0x93, 0x0F, 1 , 0 , "LGT8F88P"       },  // same as ATmega88P
  {0x94, 0x0B, 1 , 0 , "LGT8F168P"      },  // same as ATmega168P
  {0x95, 0x0F, 1 , 0 , "LGT8F328P"      }   // same as ATmega328P
};

const int deviceCount = sizeof(signatureTable) / sizeof(AVRDevice);



//*****************************************************************************
//                              Power & ISP                                   *
//*****************************************************************************
struct AVRPackage {
  uint8_t size;
  uint8_t GND1;
  uint8_t GND2;
  uint8_t VCC1;
  uint8_t VCC2;
  uint8_t HVPP;

  uint8_t RESET;
  uint8_t MOSI;
  uint8_t MISO;
  uint8_t SCK;
  uint8_t XTAL;
  uint8_t UPDI;
};

// Definícia pinov pre jednotivé púzdra
const AVRPackage packageTable[] PROGMEM = {
  { 0 ,   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,   0 },  //  0 none
// ISP
// size, GND1, GND2, VCC1, VCC2, HVPP, RST , MOSI, MISO, SCK , XTAL, UPDI,
  { 28,   8 ,   22,   7 ,   0 ,   1 ,   1 ,   17,   18,   19,   9 ,   0 },  //  1 ATmega48/88/168/328
  { 20,   10,   0 ,   20,   0 ,   1 ,   1 ,   17,   18,   19,   5 ,   0 },  //  2 ATtiny2313/4313
  { 20,   6 ,   16,   5 ,   0 ,   10,   10,   1 ,   2 ,   3 ,   7 ,   0 },  //  3 ATtiny26/261/461/861
  { 14,   14,   0 ,   1 ,   0 ,   4 ,   4 ,   7 ,   8 ,   9 ,   2 ,   0 },  //  4 ATtiny24/44/84
  { 8 ,   4 ,   0 ,   8 ,   0 ,   1 ,   1 ,   5 ,   6 ,   7 ,   2 ,   0 },  //  5 ATtiny25/45/85/13
  { 40,   11,   31,   10,   0 ,   9 ,   9 ,   6 ,   7 ,   8 ,   13,   0 },  //  6 ATmega16/32/8535
  { 40,   20,   0 ,   40,   0 ,   9 ,   9 ,   6 ,   7 ,   8 ,   19,   0 },  //  7 ATmega162/8515

// TPI
// size, GND1, GND2, VCC1, VCC2, HVPP, RST ,0=TPI, DATA, CLK , XTAL, UPDI,
  { 8 ,   8 ,   0 ,   1 ,   0 ,   4 ,   4 ,   0 ,   3 ,   2 ,   0 ,   0 },  //  8 ATtiny102
  { 14,   14,   0 ,   1 ,   0 ,   4 ,   4 ,   0 ,   3 ,   2 ,   0 ,   0 }   //  9 ATtiny104
};

const int packageCount = sizeof(packageTable) / sizeof(AVRPackage);



AVRPackage currentMCU;
uint8_t currentMCUpackage = 0;



uint8_t getOffsetPin(uint8_t pin) {
  if (pin == 0 || currentMCU.size == 40) return pin;
  
  // upper side add offset
  if (pin > (currentMCU.size / 2)) {
    return pin + (40 - currentMCU.size);
  }
  return pin;
}



void setupISP(uint8_t p, bool connect = true) {
  if (p >= packageCount) return;
  
  if (connect) {
    //currentMCUpackage = p;
    memcpy_P(&currentMCU, &packageTable[p], sizeof(AVRPackage));
    uint8_t* ptr = (uint8_t*)&currentMCU;
    for (uint8_t i = 1; i < sizeof(AVRPackage); i++) {
      ptr[i] = getOffsetPin(ptr[i]);
    }

    for (int i = 1; i < 41; i++) {    // All pins to GROUND
      zifWrite(i, LOW); }

      zifMode(currentMCU.GND1, OUTPUT);
    if (currentMCU.GND2) {
      zifMode(currentMCU.GND2, OUTPUT); }
      zifWrite(currentMCU.VCC1, HIGH);
      zifMode(currentMCU.VCC1, OUTPUT);
    if (currentMCU.VCC2) {
      zifWrite(currentMCU.VCC2, HIGH);
      zifMode(currentMCU.VCC2, OUTPUT); }

    // Setup for ISP pins
    ZIF40_PIN_RESET = currentMCU.RESET;
    ZIF40_PIN_MOSI  = currentMCU.MOSI;
    ZIF40_PIN_MISO  = currentMCU.MISO;
    ZIF40_PIN_SCK   = currentMCU.SCK;
  } else {
    // All pins OFF
    for (int i = 1; i < 41; i++) {
      zifMode(i, INPUT);
    }
  }
}



bool findDevice(uint8_t s2, uint8_t s3) {
  for (int i = 0; i < deviceCount; i++) {
    // Čítame bajty priamo z Flash
    uint8_t b2 = pgm_read_byte(&(signatureTable[i].sig2));
    uint8_t b3 = pgm_read_byte(&(signatureTable[i].sig3));
    
    if (b2 == s2 && b3 == s3) {
      //char buffer[15];
      // Skopírujeme reťazec z Flash do RAM pre výpis
      strcpy_P(deviceNameText, signatureTable[i].name);
      return true;
    }
  }
  return false;
}



//*****************************************************************************
//                               HVPP / HVSP                                  *
//*****************************************************************************


struct HVSP_Pins {
  uint8_t PEN3;
  uint8_t PEN2;
  uint8_t PEN1;
  uint8_t PEN0;
  // Controls
  uint8_t SCI;
  uint8_t SDI;
  uint8_t SII;
  uint8_t SDO;
};

struct HVPP_Pins {
  // Controls
  uint8_t PAGEL;
  uint8_t XA1;
  uint8_t XA0;
  uint8_t BS1;
  uint8_t BS2;
  
  // State pins
  uint8_t OE;
  uint8_t WR;
  uint8_t RDY;
  //uint8_t XTAL1;

  // Data
  uint8_t DATA[8]; 
};

const HVPP_Pins HVxPpinsTable[] PROGMEM = {
  {  0,   0,   0,   0,   0,   0,   0,   0, { 0, 0, 0, 0, 0, 0, 0, 0} }, //  0 none
//PAGEL,XA1, XA0, BS1, BS2,  OE,  WR, RDY,          DATA D0-D7
  { 13,  12,  11,   6,  25,   4,   5,   3, {14,15,16,17,18,19,23,24} }, //  1 ATmega48/88/168/328
  {  8,  11,   9,   8,  11,   6,   7,   3, {12,13,14,15,16,17,18,19} }, //  2 ATtiny2313/4313
  {  4,   3,   2,   4,   3,   8,   1,   9, {20,19,18,17,14,13,12,11} }, //  3 ATtiny26/261/461/861
// PE3, PE2, PE1, PE0, SCI, SDI, SII, SDO   // for HVSP DATA is zero
  {  0,  11,  12,  13,   2,   7,   8,   9, { 0, 0, 0, 0, 0, 0, 0, 0} }, //  4 ATtiny24/44/84
  {  0,   7,   6,   5,   2,   5,   6,   7, { 0, 0, 0, 0, 0, 0, 0, 0} }, //  5 ATtiny25/45/85/13
//PAGEL,XA1, XA0, BS1, BS2,  OE,  WR, RDY,          DATA D0-D7
  { 21,  20,  19,  18,  40,  16,  17,  15, { 1, 2, 3, 4, 5, 6, 7, 8} }, //  6 ATmega16/32/8535
  { 17,  16,  15,  14,  39,  12,  13,  11, { 1, 2, 3, 4, 5, 6, 7, 8} }, //  7 ATmega162/8515
  // TPI models
  {  0,   0,   0,   0,   0,   0,   0,   0, { 0, 0, 0, 0, 0, 0, 0, 0} }, //  8 none
  {  0,   0,   0,   0,   0,   0,   0,   0, { 0, 0, 0, 0, 0, 0, 0, 0} }  //  9 none
};

const int HVxPpackageCount = sizeof(HVxPpinsTable) / sizeof(HVPP_Pins);

HVPP_Pins HVPP;
HVSP_Pins HVSP;

// Rýchle nastavenie celej 8-bitovej zbernice
void dataMode(uint8_t mode) {
  if (HVPP.DATA[0] == 0) return;
  for(int i=0; i<8; i++) zifMode(HVPP.DATA[i], mode);
}

void dataWrite(uint8_t value) {
  for(int i=0; i<8; i++) {
    zifWrite(HVPP.DATA[i], (value >> i) & 0x01);
  }
}

uint8_t dataRead() {
  uint8_t value = 0;
  for(int i=0; i<8; i++) {
    if(zifRead(HVPP.DATA[i])) value |= (1 << i);
  }
  return value;
}

void toggleXTAL() {
  zifWrite(currentMCU.XTAL, HIGH);
  delayMicroseconds(1);
  zifWrite(currentMCU.XTAL, LOW);
  delayMicroseconds(1);
}

void toggleSCI() {
  zifWrite(HVSP.SCI, HIGH);
  delayMicroseconds(1);
  zifWrite(HVSP.SCI, LOW);
  delayMicroseconds(1);
}

void commitWrite(uint8_t pulseWidth = 0) {
  zifWrite(HVPP.WR, LOW);
  if (pulseWidth) delay(pulseWidth);
  else delayMicroseconds(1);
  zifWrite(HVPP.WR, HIGH);
  // Čakanie na RDY pin, kým čip dokončí zápis
  //while(zifRead(HVPP.RDY) == LOW); 
}

void HV_apply(uint8_t p) {
  digitalWrite(HVxP_ON_12V, HIGH);  // disable HV output

  if (p == 9) {
    digitalWrite(SELA, LOW);
    digitalWrite(SELB, LOW);
    digitalWrite(SELC, LOW);
  } else if (p == 31) {
    digitalWrite(SELA, HIGH);
    digitalWrite(SELB, LOW);
    digitalWrite(SELC, LOW);
  } else if (p == 10) {
    digitalWrite(SELA, LOW);
    digitalWrite(SELB, HIGH);
    digitalWrite(SELC, LOW);
  } else if (p == 1) {
    digitalWrite(SELA, HIGH);
    digitalWrite(SELB, HIGH);
    digitalWrite(SELC, LOW);
  } else if (p == 41) {       // used for IDC06 Connector
    digitalWrite(SELA, LOW);
    digitalWrite(SELB, LOW);
    digitalWrite(SELC, HIGH);
  } else if (p == 4) {
    digitalWrite(SELA, HIGH);
    digitalWrite(SELB, LOW);
    digitalWrite(SELC, HIGH);
  } else {
    digitalWrite(SELA, HIGH);
    digitalWrite(SELB, HIGH);
    digitalWrite(SELC, HIGH);
  }
}

byte setupHVxP(uint8_t p, bool connect = true) {
  if (p >= HVxPpackageCount) return false;

  if (connect) {
    memcpy_P(&currentMCU, &packageTable[p], sizeof(AVRPackage));
    if (p == 0) return false;
    uint8_t* ptp = (uint8_t*)&currentMCU;
    for (uint8_t i = 1; i < sizeof(AVRPackage); i++) {
      ptp[i] = getOffsetPin(ptp[i]);
    }

    memcpy_P(&HVPP, &HVxPpinsTable[p], sizeof(HVPP_Pins));
    uint8_t* ptr = (uint8_t*)&HVPP;
    for (uint8_t i = 0; i < sizeof(HVPP_Pins); i++) {
      ptr[i] = getOffsetPin(ptr[i]);
    }
    
    for (int i = 1; i < 41; i++) {    // All pins to GROUND
      zifWrite(i, LOW); }

    // Connect Ground pins
    zifMode(currentMCU.GND1, OUTPUT);
    if (currentMCU.GND2) {
       zifMode(currentMCU.GND2, OUTPUT); }
    // Connect VCC pins
    zifMode(currentMCU.VCC1, OUTPUT);
    if (currentMCU.VCC2) {
      zifMode(currentMCU.VCC2, OUTPUT); }
    // Connect Reset pin
    zifMode(currentMCU.RESET, OUTPUT);

    if (HVPP.DATA[0]) { 
      // HVPP - High Voltage Paralel programing
      if (HVPP.PAGEL) zifMode(HVPP.PAGEL, OUTPUT);
      if (HVPP.XA1)   zifMode(HVPP.XA1,   OUTPUT);
      if (HVPP.XA0)   zifMode(HVPP.XA0,   OUTPUT);
      if (HVPP.BS1)   zifMode(HVPP.BS1,   OUTPUT);
      if (HVPP.BS2)   zifMode(HVPP.BS2,   OUTPUT);
      if (HVPP.OE)    zifMode(HVPP.OE,    OUTPUT);
      if (HVPP.WR)    zifMode(HVPP.WR,    OUTPUT);

      zifMode(HVPP.RDY, INPUT);
      zifMode(currentMCU.XTAL, OUTPUT);
      dataMode(INPUT); // Default as Input
      return 1;

    } else {
      // HVSP - High Voltage Serial programing
      memcpy(&HVSP, &HVPP, sizeof(HVSP_Pins));
      if (HVSP.PEN0) zifMode(HVSP.PEN0, OUTPUT);
      if (HVSP.PEN1) zifMode(HVSP.PEN1, OUTPUT);
      if (HVSP.PEN2) zifMode(HVSP.PEN2, OUTPUT);
      if (HVSP.PEN3) zifMode(HVSP.PEN3, OUTPUT);
      if (HVSP.SCI)  zifMode(HVSP.SCI, OUTPUT);
      if (HVSP.SII)  zifMode(HVSP.SII, OUTPUT);
      if (HVSP.SDI)  zifMode(HVSP.SDI, OUTPUT);
      if (HVSP.SDO)  zifMode(HVSP.SDO, OUTPUT);
      return 2;
    }
  } else {
    // All pins OFF
    for (int i = 1; i < 41; i++) {
      zifMode(i, INPUT);
    }
    return false;
  }
}



//*****************************************************************************
//                                Detect MCU                                  *
//*****************************************************************************
bool detectedPins[41];



byte detectMCUsize() {
  bool pin;

  // Step 1 - all pins to ground
  for (uint8_t i = 1; i < 41; i++) {
    zifWrite(i, LOW);
    zifMode(i, OUTPUT);
    detectedPins[i] = false;
  }

  // Step 2 - detect bottom row
  for (uint8_t i = 20; i > 0; i--) {
    zifMode(i, INPUT_PULLUP);
    //delayMicroseconds(10);
    pin = zifRead(i);
    detectedPins[i] = pin;
    zifMode(i, OUTPUT);
    //delay(10);
  }

  // Step 3 - detect upper row
  for (uint8_t i = 21; i < 41; i++) {
    zifMode(i, INPUT_PULLUP);
    //delayMicroseconds(10);
    pin = zifRead(i);
    detectedPins[i] = pin;
    zifMode(i, OUTPUT);
    //delay(10);
  }

  // Step 4 - calculate DIL package
  uint8_t dpin = 0;
  for (uint8_t i = 21; i < 41; i++) {
    pin = detectedPins[i];
    if (!pin && !dpin) dpin = (41 - i) * 2;
    pin = detectedPins[41 - i];
    if (!pin && !dpin) dpin = (41 - i) * 2;
  }

  // Step 5 - all pins OFF
  for (uint8_t i = 1; i < 41; i++) {
    zifMode(i, INPUT);
  }
  return dpin;
}




/*
#------------------------------------------------------------
# avrispv2
#------------------------------------------------------------

programmer # avrispv2
    id                     = "avrispv2";
    desc                   = "Serial Atmel AVR ISP using STK500v2";
    type                   = "stk500v2";
    prog_modes             = PM_TPI | PM_ISP;
    connection_type        = serial;
;

#------------------------------------------------------------
# avrispmkII
#------------------------------------------------------------

# Atmel AVR ISP mkII programmer. See
# https://www.microchip.com/en-us/development-tool/atavrisp2
# for details.

programmer # avrispmkII
    id                     = "avrispmkII", "avrisp2";
    desc                   = "USB Atmel AVR ISP mkII";
    type                   = "stk500v2";
    prog_modes             = PM_TPI | PM_ISP | PM_PDI;
    extra_features         = HAS_VTARG_READ;
    connection_type        = usb;
    usbvid                 = 0x03eb;
    usbpid                 = 0x2104;
;

#------------------------------------------------------------
# Common values for reduced core tinys (4/5/9/10/20/40)
#------------------------------------------------------------
# prog_modes             = PM_TPI;

#------------------------------------------------------------
# AVR8X family common values
#------------------------------------------------------------
# prog_modes             = PM_SPM | PM_UPDI;


# The following table lists the devices in the original AVR910
# appnote:
# |Device |Signature | Code |
# +-------+----------+------+
# |tiny12 | 1E 90 05 | 0x55 |
# |tiny15 | 1E 90 06 | 0x56 |
# |       |          |      |
# | S1200 | 1E 90 01 | 0x13 |
# |       |          |      |
# | S2313 | 1E 91 01 | 0x20 |
# | S2323 | 1E 91 02 | 0x48 |
# | S2333 | 1E 91 05 | 0x34 |
# | S2343 | 1E 91 03 | 0x4C |
# |       |          |      |
# | S4414 | 1E 92 01 | 0x28 |
# | S4433 | 1E 92 03 | 0x30 |
# | S4434 | 1E 92 02 | 0x6C |
# |       |          |      |
# | S8515 | 1E 93 01 | 0x38 |
# | S8535 | 1E 93 03 | 0x68 |
# |       |          |      |
# |mega32 | 1E 95 01 | 0x72 |
# |mega83 | 1E 93 05 | 0x65 |
# |mega103| 1E 97 01 | 0x41 |
# |mega161| 1E 94 01 | 0x60 |
# |mega163| 1E 94 02 | 0x64 |

# Appnote AVR109 also has a table of AVR910 device codes, which
# lists:
# dev         avr910   signature
# ATmega8     0x77     0x1E 0x93 0x07
# ATmega8515  0x3B     0x1E 0x93 0x06
# ATmega8535  0x6A     0x1E 0x93 0x08
# ATmega16    0x75     0x1E 0x94 0x03
# ATmega162   0x63     0x1E 0x94 0x04
# ATmega163   0x66     0x1E 0x94 0x02
# ATmega169   0x79     0x1E 0x94 0x05
# ATmega32    0x7F     0x1E 0x95 0x02
# ATmega323   0x73     0x1E 0x95 0x01
# ATmega64    0x46     0x1E 0x96 0x02
# ATmega128   0x44     0x1E 0x97 0x02
*/
