/*
  avr_setup.h
  part of ATmegaProg

  created 2026.03.03
  by Robert Kovaľ <http://www.toroproduction.sk>
  modified 2026.04.03
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
  uint8_t pack2;  // package type 2
  char name[16];  // MCU name (max 15 characters + \0)
};

const AVRDevice signatureTable[] PROGMEM = {
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
};

const int deviceCount = sizeof(signatureTable) / sizeof(AVRDevice);



//*********************************************************************
//                        Power & ISP                                 *
//*********************************************************************
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
// size, GND1, GND2, VCC1, VCC2, HVPP, RST , MOSI, MISO, SCK , XTAL, UPDI,
  { 8 ,   8 ,   0 ,   1 ,   0 ,   4 ,   4 ,   0 ,   3 ,   2 ,   0 ,   0 },  //  8 ATtiny102
  { 14,   14,   0 ,   1 ,   0 ,   4 ,   4 ,   0 ,   3 ,   2 ,   0 ,   0 }   //  9 ATtiny104
};

const int packageCount = sizeof(packageTable) / sizeof(AVRPackage);



// Pracovné pole v RAM, kam skopírujeme riadok
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
      ptr[i] = ZIF[getOffsetPin(ptr[i])];
    }

    for (int i = 1; i < 41; i++) {    // All pins to GROUND
      digitalWrite(ZIF[i], LOW); }

      pinMode(currentMCU.GND1, OUTPUT);
    if (currentMCU.GND2) {
      pinMode(currentMCU.GND2, OUTPUT); }
      digitalWrite(currentMCU.VCC1, HIGH);
      pinMode(currentMCU.VCC1, OUTPUT);
    if (currentMCU.VCC2) {
      digitalWrite(currentMCU.VCC2, HIGH);
      pinMode(currentMCU.VCC2, OUTPUT); }

    // Setup for ISP pins
    ZIF40_PIN_RESET = currentMCU.RESET;
    ZIF40_PIN_MOSI  = currentMCU.MOSI;
    ZIF40_PIN_MISO  = currentMCU.MISO;
    ZIF40_PIN_SCK   = currentMCU.SCK;
  } else {
    // All pins OFF
    for (int i = 1; i < 41; i++) {
      pinMode(ZIF[i], INPUT);
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
      return 1;
    }
  }
  return 0;
}



//*********************************************************************
//                           HVPP / HVSP                              *
//*********************************************************************


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
  for(int i=0; i<8; i++) pinMode(HVPP.DATA[i], mode);
}

void dataWrite(uint8_t value) {
  for(int i=0; i<8; i++) {
    digitalWrite(HVPP.DATA[i], (value >> i) & 0x01);
  }
}

uint8_t dataRead() {
  uint8_t value = 0;
  for(int i=0; i<8; i++) {
    if(digitalRead(HVPP.DATA[i])) value |= (1 << i);
  }
  return value;
}

void toggleXTAL() {
  digitalWrite(currentMCU.XTAL, HIGH);
  delayMicroseconds(1);
  digitalWrite(currentMCU.XTAL, LOW);
  delayMicroseconds(1);
}

void toggleSCI() {
  digitalWrite(HVSP.SCI, HIGH);
  delayMicroseconds(1);
  digitalWrite(HVSP.SCI, LOW);
  delayMicroseconds(1);
}

void commitWrite(uint8_t pulseWidth = 0) {
  digitalWrite(HVPP.WR, LOW);
  if (pulseWidth) delay(pulseWidth);
  else delayMicroseconds(1);
  digitalWrite(HVPP.WR, HIGH);
  // Čakanie na RDY pin, kým čip dokončí zápis
  //while(digitalRead(HVPP.RDY) == LOW); 
}

uint8_t searchArduinoPin(uint8_t p) {
  for (int i = 0; i < 41; i++) {
    if (ZIF[i] == p) return i; }
  return 0;
}

void HV_apply(uint8_t ap) {
  digitalWrite(HVxP_ON_12V, HIGH);  // disable HV output

  uint8_t p = searchArduinoPin(ap);
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
  } else if (p == 41) {       // use for IDC06 Connector
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
      ptp[i] = ZIF[getOffsetPin(ptp[i])];
    }

    memcpy_P(&HVPP, &HVxPpinsTable[p], sizeof(HVPP_Pins));
    uint8_t* ptr = (uint8_t*)&HVPP;
    for (uint8_t i = 0; i < sizeof(HVPP_Pins); i++) {
      ptr[i] = ZIF[getOffsetPin(ptr[i])];
    }
    
    for (int i = 1; i < 41; i++) {    // All pins to GROUND
      digitalWrite(ZIF[i], LOW); }

    // Connect Ground pins
    pinMode(currentMCU.GND1, OUTPUT);
    if (currentMCU.GND2) {
       pinMode(currentMCU.GND2, OUTPUT); }
    // Connect VCC pins
    pinMode(currentMCU.VCC1, OUTPUT);
    if (currentMCU.VCC2) {
      pinMode(currentMCU.VCC2, OUTPUT); }
    // Connect Reset pin
    pinMode(currentMCU.RESET, OUTPUT);

    if (HVPP.DATA[0]) { 
      // HVPP - High Voltage Paralel programing
      if (HVPP.PAGEL) pinMode(HVPP.PAGEL, OUTPUT);
      if (HVPP.XA1)   pinMode(HVPP.XA1,   OUTPUT);
      if (HVPP.XA0)   pinMode(HVPP.XA0,   OUTPUT);
      if (HVPP.BS1)   pinMode(HVPP.BS1,   OUTPUT);
      if (HVPP.BS2)   pinMode(HVPP.BS2,   OUTPUT);
      if (HVPP.OE)    pinMode(HVPP.OE,    OUTPUT);
      if (HVPP.WR)    pinMode(HVPP.WR,    OUTPUT);

      pinMode(HVPP.RDY, INPUT);
      pinMode(currentMCU.XTAL, OUTPUT);
      dataMode(INPUT); // Default as Input
      return 1;

    } else {
      // HVSP - High Voltage Serial programing
      memcpy(&HVSP, &HVPP, sizeof(HVSP_Pins));
      if (HVSP.PEN0) pinMode(HVSP.PEN0, OUTPUT);
      if (HVSP.PEN1) pinMode(HVSP.PEN1, OUTPUT);
      if (HVSP.PEN2) pinMode(HVSP.PEN2, OUTPUT);
      if (HVSP.PEN3) pinMode(HVSP.PEN3, OUTPUT);
      if (HVSP.SCI)  pinMode(HVSP.SCI, OUTPUT);
      if (HVSP.SII)  pinMode(HVSP.SII, OUTPUT);
      if (HVSP.SDI)  pinMode(HVSP.SDI, OUTPUT);
      if (HVSP.SDO)  pinMode(HVSP.SDO, OUTPUT);
      return 2;
    }
  } else {
    // All pins OFF
    for (int i = 1; i < 41; i++) {
      pinMode(ZIF[i], INPUT);
    }
    return false;
  }
}



//*********************************************************************
//                            Detect MCU                              *
//*********************************************************************
bool detectedPins[41];



byte detectMCUsize() {
  bool pin;

  // Step 1 - all pins to ground
  for (uint8_t i = 1; i < 41; i++) {
    digitalWrite(ZIF[i], LOW);
    pinMode(ZIF[i], OUTPUT);
    detectedPins[i] = false;
  }

  // Step 2 - detect bottom row
  for (uint8_t i = 20; i > 0; i--) {
    pinMode(ZIF[i], INPUT_PULLUP);
    //delayMicroseconds(10);
    pin = digitalRead(ZIF[i]);
    detectedPins[i] = pin;
    pinMode(ZIF[i], OUTPUT);
    //delay(10);
  }

  // Step 3 - detect upper row
  for (uint8_t i = 21; i < 41; i++) {
    pinMode(ZIF[i], INPUT_PULLUP);
    //delayMicroseconds(10);
    pin = digitalRead(ZIF[i]);
    detectedPins[i] = pin;
    pinMode(ZIF[i], OUTPUT);
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
    pinMode(ZIF[i], INPUT);
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
