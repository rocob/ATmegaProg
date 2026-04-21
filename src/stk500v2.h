/*
  stk500v2.h
  part of ATmegaProg

  created 2026.03.03
  by Robert Kovaľ <http://www.toroproduction.sk>
  modified 2026.04.21
  by Robert Kovaľ

  This is private source code
  Copyright 2025-2026 Robert Kovaľ and TORO-Production s.r.o.
  This program is Free Software and has ABSOLUTELY NO WARRANTY
  Please report bugs to mail@rocob.biz
  See LICENSE.txt for details.
*/


//******************************************************************
//                      STK500v2 protocol
//******************************************************************
#define STK_START           0
#define STK_GET_SEQ_NUM     1
#define STK_MSG_SIZE_1      2
#define STK_MSG_SIZE_2      3
#define STK_GET_TOKEN       4
#define STK_GET_DATA        5
#define STK_GET_CHECK       6
#define STK_PROCESS         7

#define STK_PROGMODE_NONE   0
#define STK_PROGMODE_ISP    1
#define STK_PROGMODE_HVPP   2
#define STK_PROGMODE_HVSP   3
#define STK_PROGMODE_JTAG   4
#define STK_PROGMODE_UPDI   5
#define STK_PROGMODE_TPI    6

#include "Arduino.h"
#include "V2commands.h"
#include "SPI.h"
#include "zif_io.h"
#include "avr_setup.h"


static BitBangedSPI VirtualSPI;


uint8_t seqNum = 0;
uint16_t msgLength = 0;
uint16_t msgCPoint = 0;
uint8_t msgBuffer[285];
uint8_t msgState = 0;
uint8_t msgChecksum = 0;

uint8_t STKP_timeout = 0;           // Command time-out (in ms)
uint8_t STKP_stabDelay = 0;         // Delay (in ms) used for pin stabilization
// uint8_t STKP_eraseDelay = 0;        // Delay (in ms) to ensure that the erase finished
uint8_t STKP_cmdexeDelay = 0;       // Delay (in ms) in connection with the 
                                    // EnterProgMode command execution
uint8_t STKP_synchLoops = 0;        // Number of synchronization loops
uint8_t STKP_byteDelay = 0;         // Delay (in ms) between each byte in the
                                    // EnterProgMode command.
uint8_t STKP_pollValue = 0;         // Poll value: 0x53 for AVR, 0x69 for AT89xx
uint8_t STKP_pollIndex = 0;         // Start address, received byte:
                                    // 0 = no polling, 3 = AVR, 4 = AT89xx
// uint8_t STKP_pollMethod = 0;        // Poll method, 0 = use delay 1= use RDY/BSY command
uint8_t STKP_progModeDelay= 0;      // Delay (in ms) in connection with the EnterProgMode
uint8_t STKP_latchCycles =  0;      // Number of xtal cycles used to latch OSCCAL
uint8_t STKP_toggleVtg =    0;      // Toggle Vtg when entering prog.mode (0=no, 1=yes).
                                    // For parts with RSTDSBL functionality 
uint8_t STKP_powerOffDelay= 0;      // Power-off delay. Additional delay (in ms) after Vtg is turned off
uint8_t STKP_resetDelayMs = 0;      // RSTDELAY #1 (in ms) Additional delay between Vtg is turned on and reset goes high.
uint8_t STKP_resetDelayUs = 0;      // RSTDELAY #2 (in us x 10) Additional delay between Vtg is turned on and reset goes
                                    // high. Total delay is RSTDELAY #1 (ms) + RSTDELAY #2 (us x 10) 

uint32_t STKP_address;              // Flashing address

// Writeable Parameter Values
uint8_t PARAM_VTARGET_VAL         = 50;
uint8_t PARAM_VADJUST_VAL         = 50;
uint8_t PARAM_OSC_PSCALE_VAL      = 1;
uint8_t PARAM_OSC_CMATCH_VAL      = 80;
uint8_t PARAM_SCK_DURATION_VAL    = 4;
uint8_t PARAM_RESET_POLARITY_VAL  = 1;
uint8_t PARAM_CONTROLLER_INIT_VAL = 0;  // RESET init value must by 0x00

// uint8_t PARAM_TOPCARD_DETECT_VAL  = 0; // read only
// uint8_t PARAM_DATA_VAL            = 0; // read only
// uint8_t PARAM_STATUS              = 0; // read only

//void stk500_commands(void);






void resetTarget(bool val = true) {
  if (PARAM_RESET_POLARITY_VAL) val = !val;
  if (targetISP == IDC06)
    zifWrite(IDC06_PIN_RESET, val);
  else
    zifWrite(ZIF40_PIN_RESET, val);
}

uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  if (targetISP == IDC06) {
    SPI.transfer(a);
    SPI.transfer(b);
    SPI.transfer(c);
    return SPI.transfer(d);
  } else {
    VirtualSPI.transfer(a);
    VirtualSPI.transfer(b);
    VirtualSPI.transfer(c);
    return VirtualSPI.transfer(d);
  }
}

uint8_t spi_transaction(uint8_t *cmd, uint16_t index) {
  if (targetISP == IDC06) {
    SPI.transfer(cmd[index++]);
    SPI.transfer(cmd[index++]);
    SPI.transfer(cmd[index++]);
    return SPI.transfer(cmd[index]);
  } else {
    VirtualSPI.transfer(cmd[index++]);
    VirtualSPI.transfer(cmd[index++]);
    VirtualSPI.transfer(cmd[index++]);
    return VirtualSPI.transfer(cmd[index]);
  }
}

uint8_t doubleTransfer(uint8_t d, uint8_t i) {
  uint8_t o;
  uint8_t pulseWidth = 4;
  zifWrite(HVSP.SCI, HIGH);
  delayMicroseconds(pulseWidth);
  for (byte j = 0; j < 8; j++) {
    o = (o << 1) | zifRead(HVSP.SDO);
    zifWrite(HVSP.SDI, (d & 0x80) ? HIGH : LOW);
    d = d << 1;
    zifWrite(HVSP.SII, (i & 0x80) ? HIGH : LOW);
    i = i << 1;
    zifWrite(HVSP.SCI, LOW);
    delayMicroseconds(pulseWidth);
    zifWrite(HVSP.SCI, HIGH);
    delayMicroseconds(pulseWidth);
  }
  zifWrite(HVSP.SDI, LOW);
  zifWrite(HVSP.SII, LOW);

  zifWrite(HVSP.SCI, LOW);
  delayMicroseconds(pulseWidth);

  zifWrite(HVSP.SCI, HIGH);
  delayMicroseconds(pulseWidth);
  zifWrite(HVSP.SCI, LOW);
  delayMicroseconds(pulseWidth);

  zifWrite(HVSP.SCI, HIGH);
  delayMicroseconds(pulseWidth);
  zifWrite(HVSP.SCI, LOW);
  delayMicroseconds(pulseWidth);
  return o;
}

static void sendchar(char c) {
  //lcdPrintHex(c);
  SERIAL.print((char)c);
}

// A. PP Load Command
void loadCommand(byte command) {
  zifWrite(HVPP.BS1, LOW);
  zifWrite(HVPP.BS2, LOW);
  zifWrite(HVPP.XA1, HIGH);
  zifWrite(HVPP.XA0, LOW);
  dataMode(OUTPUT);
  dataWrite(command);
  toggleXTAL();
}
// B/G. PP Load Address byte
void loadAddress(byte address, bool high = false) {
  zifWrite(HVPP.XA1, LOW);
  zifWrite(HVPP.XA0, LOW);
  zifWrite(HVPP.BS1, high);
  dataWrite(address);
  toggleXTAL();
}
// C/D. Load data
void loadData(byte data, bool high = false) {
  zifWrite(HVPP.XA1, LOW);
  zifWrite(HVPP.XA0, HIGH);
  zifWrite(HVPP.BS1, high);
  dataWrite(data);
  toggleXTAL();
}
// E. Read byte
byte readByte(bool BS1) {
  dataMode(INPUT);
  zifWrite(HVPP.BS1, BS1);
  zifWrite(HVPP.OE,  LOW);
  byte val = dataRead();
  zifWrite(HVPP.OE,  HIGH);
  return val;
}

void messageSend() {
  sendchar(MESSAGE_START);
  byte checksum = MESSAGE_START^0;
  
  sendchar(seqNum);
  checksum ^= seqNum;
  
  char c = ((msgLength>>8)&0xFF);
  sendchar(c);
  checksum ^= c;
  
  c = msgLength&0x00FF;
  sendchar(c);
  checksum ^= c;
  
  sendchar(TOKEN);
  checksum ^= TOKEN;

  for (uint16_t i = 0; i < msgLength; i++) {
    c = msgBuffer[i];
    sendchar(c);
    checksum ^= c;
  }
  sendchar(checksum);
  seqNum++;
}




//******************************************************************
//                      STK500v2 commands
//******************************************************************

void stk500v2_commands() {
  uint8_t cmd = msgBuffer[0];
  uint8_t ins = msgBuffer[1];
  uint8_t val = 0;
                                                          if (STKdump && newClear) lcdClear();
                                                          if (STKdump) lcdPrintHex(cmd);
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);

//*****************************************************  
//                  General commands 
//*****************************************************  
  if (cmd == CMD_SIGN_ON) {                     // 0x01
    STKError = 0;
    
    msgLength = 11;
    msgBuffer[1]  = STATUS_CMD_OK;

    // msgBuffer[2]  = 6;
    // msgBuffer[3]  = 'S';
    // msgBuffer[4]  = 'T';
    // msgBuffer[5]  = 'K';
    // msgBuffer[6]  = '6';
    // msgBuffer[7]  = '0';
    // msgBuffer[8]  = '0';
    // msgBuffer[9]  = ' ';
    // msgBuffer[10] = ' ';

    // msgBuffer[2]  = 8;
    // msgBuffer[3]  = 'A';
    // msgBuffer[4]  = 'V';
    // msgBuffer[5]  = 'R';
    // msgBuffer[6]  = 'I';
    // msgBuffer[7]  = 'S';
    // msgBuffer[8]  = 'P';
    // msgBuffer[9]  = '_';
    // msgBuffer[10] = '2';

    msgBuffer[2]  = 8;
    msgBuffer[3]  = 'S';
    msgBuffer[4]  = 'T';
    msgBuffer[5]  = 'K';
    msgBuffer[6]  = '5';
    msgBuffer[7]  = '0';
    msgBuffer[8]  = '0';
    msgBuffer[9]  = '_';
    msgBuffer[10] = '2';

    // msgLength = 13;
    // msgBuffer[2]  = 10;
    // msgBuffer[3]  = 'A';
    // msgBuffer[4]  = 'V';
    // msgBuffer[5]  = 'R';
    // msgBuffer[6]  = 'I';
    // msgBuffer[7]  = 'S';
    // msgBuffer[8]  = 'P';
    // msgBuffer[9]  = '_';
    // msgBuffer[10] = 'M';
    // msgBuffer[11] = 'K';
    // msgBuffer[12] = '2';
    
//*****************************************************  
  } else if (cmd == CMD_SET_PARAMETER) {        // 0x02
                                                          if (STKdump) lcdPrintHex(ins, 'p');
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
    uint8_t result = STATUS_CMD_OK;
    val = msgBuffer[2];
    switch (ins) {
      case PARAM_VTARGET:
        PARAM_VTARGET_VAL = val;
        break;
      case PARAM_VADJUST:
        PARAM_VADJUST_VAL = val;
        break;
      case PARAM_OSC_PSCALE:
        PARAM_OSC_PSCALE_VAL = val;
        break;
      case PARAM_OSC_CMATCH:
        PARAM_OSC_CMATCH_VAL = val;
        break;
      case PARAM_SCK_DURATION:
        PARAM_SCK_DURATION_VAL = val;
        break;
      case PARAM_RESET_POLARITY:
        PARAM_RESET_POLARITY_VAL = val;
        break;
      case PARAM_CONTROLLER_INIT:
        PARAM_CONTROLLER_INIT_VAL = val;
        break;
      default:
        result = STATUS_SET_PARAM_MISSING;
        STKError = msgBuffer[0];
    }

     msgLength = 2;
     msgBuffer[1] = result;
        
//*****************************************************  
  } else if (cmd == CMD_GET_PARAMETER) {        // 0x03
                                                          if (STKdump) lcdPrintHex(ins, 'p');
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
    switch (ins) {
      case PARAM_BUILD_NUMBER_LOW:  // 0x80
        val = PARAM_BUILD_NUMBER_LOW_VAL;
        break;
      case PARAM_BUILD_NUMBER_HIGH: // 0x81
        val = PARAM_BUILD_NUMBER_HIGH_VAL;
        break;
      case PARAM_HW_VER:            // 0x90
        val = HWVERSION;
        break;
      case PARAM_SW_MAJOR:          // 0x91
        val = SWMAJOR;
        break;
      case PARAM_SW_MINOR:          // 0x92
        val = SWMINOR;
        break;
      case PARAM_VTARGET:           // 0x94
        val = PARAM_VTARGET_VAL;    // target VCC voltage x100 mV
        break;
      case PARAM_VADJUST:           // 0x95
        val = PARAM_VADJUST_VAL;    // target AREF voltage x100 mV
        break;
      case PARAM_OSC_PSCALE:        // 0x96
        val = PARAM_OSC_PSCALE_VAL;
        break;
      case PARAM_OSC_CMATCH:        // 0x97
        val = PARAM_OSC_CMATCH_VAL;
        break;
      case PARAM_SCK_DURATION:      // 0x98
        val = PARAM_SCK_DURATION_VAL;
        break;
      // case PARAM_TOPCARD_DETECT:    // 0x9A
      //   val = PARAM_TOPCARD_DETECT_VAL;
      //   break;
      // case PARAM_STATUS:            // 0x9C
      //   val = _VAL;
      //   break;
      // case PARAM_DATA:              // 0x9D
      //   val = PARAM_DATA_VAL;
      //   break;
      case PARAM_RESET_POLARITY:    // 0x9E
        val = PARAM_RESET_POLARITY_VAL;
        break;
      case PARAM_CONTROLLER_INIT:   // 0x9F
        val = PARAM_CONTROLLER_INIT_VAL;
        break;
      default:
        val = 0;
    }

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;
    //msgBuffer[3] = 0;

//*****************************************************  
  } else if (cmd == CMD_SET_DEVICE_PARAMETERS) {  // 0x04

/* 5.1.4. Device Parameter Setting Command (CMD_SET_DEVICE_PARAMETERS) [$04]
(Translator's Note) Although not described in the original text, it is defined in the 
command.h file. It is not actually used and is treated as an undefined command.

 *  Štruktúra dát pre CMD_SET_DEVICE_PARAMETERS (20 bajtov)
Index  Názov param. Dĺžka Popis
0     deviceCode      1B  ID čipu (napr. 0x41 pre ATmega2560).
1     revision        1B  Revízia hardvéru (zvyčajne 0x00).
2     progType        1B  0 = Parallel, 1 = Serial (ISP).
3     parMode         1B  Príznak pre paralelný režim (zvyčajne 0x01).
4     polling         1B  Čakanie na koniec zápisu (0 = nie, 1 = áno).
5     selfTimed       1B  Použitie interného časovača pre zápis.
6     lockDie         1B  Počet bajtov Lock bits (zvyčajne 1).
7     fuseDie         1B  Počet bajtov Fuse bits (zvyčajne 3).
8     flashPollVal1   1B  Hodnota 1 pre polling Flash pamäte.
9     flashPollVal2   1B  Hodnota 2 pre polling Flash pamäte.
10    eepromPollVal1  1B  Hodnota 1 pre polling EEPROM.
11    eepromPollVal2  1B  Hodnota 2 pre polling EEPROM.
12-13 pageSize        2B  Veľkosť stránky vo Flash (napr. 0x0100 = 256B).
14-15 eepromSize      2B  Celková veľkosť EEPROM v bajtoch.
16-19 flashSize       4B  Celková veľkosť Flash pamäte v bajtoch (napr. 0x00040000 pre 256KB).
*/
    // Currently nothing work

    // answer unknown
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_UNKNOWN;

//*****************************************************  
  } else if (cmd == CMD_OSCCAL) {               // 0x05
    
    // Read documentation first !!! AVR053, AVR054, AVR055

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_UNKNOWN;

//*****************************************************  
  } else if (cmd == CMD_LOAD_ADDRESS) {         // 0x06
    STKP_address = ((uint32_t)msgBuffer[1] << 24) | \
                   ((uint32_t)msgBuffer[2] << 16) | \
                   ((uint32_t)msgBuffer[3]  << 8) | \
                    (uint32_t)msgBuffer[4];

    // answer
    msgBuffer[1] = STATUS_CMD_OK;
    if (msgLength != 5) {
      msgBuffer[1] = STATUS_CMD_FAILED;
      STKError = msgBuffer[0];
    }
    msgLength = 2;

//*****************************************************  
  } else if (cmd == CMD_FIRMWARE_UPGRADE) {     // 0x07

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_UNKNOWN;

//*****************************************************  
//                     ISP commands 
//*****************************************************  
  } else if (cmd == CMD_ENTER_PROGMODE_ISP) {   // 0x10

    // If MCU package not inserted to ZIF, change ISP target to IDC06 connector
    if (currentMCUpackage) {
      targetISP = ZIF40;
    } else {
      targetISP = IDC06;
    }

    STKP_timeout =      msgBuffer[1];   // 200 ms
    STKP_stabDelay =    msgBuffer[2];   // 100 ms used for pin stabilization
    STKP_cmdexeDelay =  msgBuffer[3];   // 25 ms
    STKP_synchLoops =   msgBuffer[4];   // 32
    STKP_byteDelay =    msgBuffer[5];   // 0 ms
    STKP_pollValue =    msgBuffer[6];   // 0x53 for AVR (0x69 for AT89xx)
    STKP_pollIndex =    msgBuffer[7];   // 3 (0 no polling, 3 AVR, 4 AT89xx)
    // msgBuffer[8] - cmd1 - eq 0xAC
    // msgBuffer[9] - cmd2 - eq 0x53
    // msgBuffer[10]- cmd3 - eq 0x00
    // msgBuffer[11]- cmd4 - eq 0x00

    setupISP(currentMCUpackage);

    //digitalWrite(PIN_RESET, LOW);
    resetTarget();
 
    if (targetISP == IDC06) {
      zifMode(IDC06_PIN_RESET, OUTPUT);
      SPI.begin();
      SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));
      zifWrite(IDC06_PIN_SCK, LOW);
    } else {
      zifMode(ZIF40_PIN_RESET, OUTPUT);
      VirtualSPI.begin(ZIF40_PIN_MOSI, ZIF40_PIN_MISO, ZIF40_PIN_SCK); // ZIF40
      VirtualSPI.beginTransaction(vSPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));
      zifWrite(ZIF40_PIN_SCK, LOW);
    }

    delay(20);                          // Wait for at least 20ms and enable serial programming
    
    uint8_t loops = STKP_synchLoops;
    if (!loops) loops = 1;
    do {
      resetTarget(false);                 // Pulse RESET
      delayMicroseconds(100);
      resetTarget();
      delayMicroseconds(100);
      
      // delay(50);
      // spi_transaction(msgBuffer, 8);

      if (targetISP == IDC06) {
        SPI.transfer(msgBuffer[8]);
        SPI.transfer(msgBuffer[9]);
        val = SPI.transfer(msgBuffer[10]);
        ins = SPI.transfer(msgBuffer[11]);
      } else {
        VirtualSPI.transfer(msgBuffer[8]);
        VirtualSPI.transfer(msgBuffer[9]);
        val = VirtualSPI.transfer(msgBuffer[10]);
        ins = VirtualSPI.transfer(msgBuffer[11]);
      }
      if (STKP_pollIndex == 4) val = ins;
      else if (STKP_pollIndex != 3) break;
      loops--;
    } while (STKP_pollValue != val && loops > 0);
  
    progMode = STK_PROGMODE_ISP;
                                                          if (STKdump) lcdPrintHex(loops, '#');
    // send answer
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!loops) {
      msgBuffer[1] = STATUS_CMD_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_LEAVE_PROGMODE_ISP) {   // 0x11
                                                          if (STKdump) lcdPrintln("EndISP");
    // msgBuffer[1];   // preDelay  (in ms)
    // msgBuffer[2];   // postDelay (in ms)

    resetTarget(false);

    if (targetISP == IDC06) {
      SPI.end();
      zifMode(IDC06_PIN_MOSI, INPUT);
      zifMode(IDC06_PIN_SCK, INPUT);
      zifMode(IDC06_PIN_RESET, INPUT);
    } else {
      VirtualSPI.end();
      // zifMode(ZIF40_PIN_MOSI, INPUT); // Impleted in virtual_spi
      // zifMode(ZIF40_PIN_SCK, INPUT);
      zifMode(ZIF40_PIN_RESET, INPUT);
    }

    setupISP(currentMCUpackage, false);   // Disconnet MCU
    
    progMode = STK_PROGMODE_NONE;
    newClear = true;
    
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_CHIP_ERASE_ISP) {       // 0x12
    uint8_t eDelay = msgBuffer[1];              // STKP_eraseDelay
    uint8_t method = msgBuffer[2];              // STKP_pollMethod, 0 = use delay, 1= use RDY/BSY

    //val = spi_transaction(0xAC, 0x80, 0x00, 0x00);
    spi_transaction(msgBuffer, 3);

    uint16_t timeout = 100 * eDelay;
    if (method == 1) {
      do {
        delayMicroseconds(10);
        val = spi_transaction(0xF0, 0x00, 0x00, 0x00);
        timeout--;
      } while(val == 1 && timeout > 0);
    } else {
      delay(eDelay);
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_FLASH_ISP) {    // 0x13
    uint16_t size = ((uint16_t)msgBuffer[1]<<8) | msgBuffer[2];
    uint8_t  mode = msgBuffer[3];  // 0xC1
    /*  Bit Function in the mode byte
        0   0 = Word mode, 1 = Page mode
        1   Timed delay word mode
        2   Value polling word mode
        3   RDY/BSY polling word mode
        4   Timed delay page mode
        5   Value polling page mode
        6   RDY/BSY polling page mode
        7   0 = Don't write page, 1 = Write page
    */
    uint8_t pDelay = msgBuffer[4];  // 5 ms
    uint8_t cmd1   = msgBuffer[5];  // 0x40
    // msgBuffer[6] - cmd2 - eq 0x4C
    // msgBuffer[7] - cmd3 - eq 0x20
    // msgBuffer[8] - poll1 - 0xFF
    // msgBuffer[9] - poll2 - 0xFF
    // msgBuffer[10] - first data
    uint16_t p = 10;
    uint16_t timeout = 100 * pDelay;
    bool is_high = false;

    if (mode & 0x01) {              // PAGE MODE
      // Load FLASH memory page
      for (uint16_t i = 0; i < size; i++) {
        uint8_t cmdx = (i % 2 == 0) ? cmd1 : (cmd1 | 0x08);
        
        spi_transaction(cmdx, 0x00,
          (STKP_address + (i/2)) & 0xFF, msgBuffer[p++]);
      }

      if (mode & 0x80) {            // COMMIT (Write page)
        spi_transaction(msgBuffer[6],
          (STKP_address >> 8) & 0xFF,
          STKP_address & 0xFF, 0x00);
      
        if (mode & 0x40) {          // RDY/BSY polling
          do {
            delayMicroseconds(10);
            val = spi_transaction(0xF0, 0x00, 0x00, 0x00);
            timeout--;
          } while((val & 0x01) && timeout > 0);
        } else {
          delay(pDelay);            // Timed delay
        }
      }

      STKP_address += (size / 2);

    } else {                        // WORD MODE
      for (uint16_t i = 0; i < size; i++) {
        spi_transaction(cmd1, (STKP_address >> 8) & 0xFF, 
                        STKP_address & 0xFF, 
                        msgBuffer[p++]);
        STKP_address++;

        timeout = 100 * pDelay;
        if (mode & 0x08) {          // RDY/BSY polling
          do {
            delayMicroseconds(10);
            val = spi_transaction(0xF0, 0x00, 0x00, 0x00);
            timeout--;
          } while((val & 0x01) && timeout > 0);
        } else {
          delay(pDelay);            // Timed delay
        }

        if (!timeout) break;
      }
    }
    
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_FLASH_ISP) {       // 0x14
    uint16_t size = ((uint16_t)msgBuffer[1]<<8) | msgBuffer[2];
    val = msgBuffer[3]; // standard 0x20 for FLASH
    uint16_t p = 2;
    bool is_high = false;

    for (uint16_t i = 0; i < size; i++) {
      msgBuffer[p++] = spi_transaction(val | (is_high * 0x08),
                        (STKP_address >> 8) & 0xFF,
                        STKP_address & 0xFF, 0x00);
      if (is_high) STKP_address++;
      is_high = !is_high;
    }

    msgLength = size + 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[p] = STATUS_CMD_OK;
    
//*****************************************************  
  } else if (cmd == CMD_PROGRAM_EEPROM_ISP) {   // 0x15
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t  mode = msgBuffer[3];  // 0xC1
//                                                          if (STKdump) lcdPrintHex(mode, 'm');
    /*  Bit Function in the mode byte
        0   0 = Word mode, 1 = Page mode
        1   Timed delay word mode
        2   Value polling word mode
        3   RDY/BSY polling word mode
        4   Timed delay page mode
        5   Value polling page mode
        6   RDY/BSY polling page mode
        7   0 = Don't write page, 1 = Write page
    */
    uint8_t pDelay = msgBuffer[4];  // 20 ms Delay, used for different types
    uint8_t cmd1   = msgBuffer[5];  // 0xC1 standard 0xC0-word, 0xC1-page
    // msgBuffer[6] - cmd2 - eq 0xC2
    // msgBuffer[7] - cmd3 - eq 0xA0
    // msgBuffer[8] - poll1 - 0xFF
    // msgBuffer[9] - poll2 - 0xFF
    // msgBuffer[10] - first data
    uint16_t p = 10; 
    uint16_t timeout = 100 * pDelay;

    if (mode & 0x01) {              // PAGE MODE
      // Load EEPROM memory page
      for (uint16_t i = 0; i < size; i++) {
        spi_transaction(cmd1, 0x00, i, msgBuffer[p++]);
      }
      if (mode & 0x80) {            // COMMIT (Write page)
        spi_transaction(msgBuffer[6],
          (STKP_address >> 8) & 0xFF,
          STKP_address & 0xFF, 0x00);
      
        if (mode & 0x40) {          // RDY/BSY polling
        //if (mode & 0x40 && false) {          // RDY/BSY polling - currently still disabled
          do {
            delayMicroseconds(10);
            val = spi_transaction(0xF0, 0x00, 0x00, 0x00);
            timeout--;
          } while((val & 0x01) && timeout > 0);
        } else {
          delay(pDelay);            // Timed delay
        }
//                                                          if (STKdump) lcdPrintHex(val, 'v');
//                                                          if (STKdump) lcdPrintHex(pDelay, 'd');
//                                                          if (STKdump) lcdPrintHex(pDelay*100 - timeout, 't');
      }
      
      STKP_address += size;

    } else {                        // WORD MODE
      for (uint16_t i = 0; i < size; i++) {
        spi_transaction(cmd1, (STKP_address >> 8) & 0xFF, 
                        STKP_address & 0xFF, 
                        msgBuffer[p++]);
        STKP_address++;

        timeout = 100 * pDelay;
        if (mode & 0x08) {          // RDY/BSY polling
          do {
            delayMicroseconds(10);
            val = spi_transaction(0xF0, 0x00, 0x00, 0x00);
            timeout--;
          } while((val & 0x01) && timeout > 0);
        } else {
          delay(pDelay);            // Timed delay
        }

        if (!timeout) break;
      }
    }

    msgBuffer[1] = STATUS_CMD_OK;
    msgLength = 2;
//                                                          if (STKdump) lcdPrintHex(timeout, 't');
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_EEPROM_ISP) {      // 0x16
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    val = msgBuffer[3]; // standard 0xA0 for EEPROM
    uint16_t p = 2; 
    for (uint16_t i = 0; i < size; i++) {
        msgBuffer[p++] = spi_transaction(val,
                          (STKP_address >> 8) & 0xFF, 
                          STKP_address & 0xFF, 0x00);
        STKP_address++;
    }

    msgLength = size + 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[p] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_FUSE_ISP) {     // 0x17
    spi_transaction(msgBuffer, 1);
    delay(20);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_READ_FUSE_ISP) {        // 0x18
    val = spi_transaction(msgBuffer, 2);

    msgLength = 4;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;
    msgBuffer[3] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_LOCK_ISP) {     // 0x19
    spi_transaction(msgBuffer, 1);
    delay(20);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_READ_LOCK_ISP) {        // 0x1A
    val = spi_transaction(msgBuffer, 2);

    msgLength = 4;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;
    msgBuffer[3] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_READ_SIGNATURE_ISP) {   // 0x1B
    val = spi_transaction(msgBuffer, 2);
    // cmd3 = msgBuffer[4] // 0-high, 1-middle, 2-low
    if (msgBuffer[4] <= 2) {
      signature[msgBuffer[4]] = val;      // Store signature
    }

    msgLength = 4;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;
    msgBuffer[3] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_READ_OSCCAL_ISP) {      // 0x1C
    val = spi_transaction(msgBuffer, 2);

    msgLength  = 4;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;
    msgBuffer[3] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_SPI_MULTI) {            // 0x1D
    uint8_t numTX   = msgBuffer[1];
    uint8_t numRX   = msgBuffer[2];
    uint8_t rxStart = msgBuffer[3];             // Index
            // data = msgBuffer[4];
    uint8_t rxCount = 0;
    
    for (uint8_t i = 0; i < numTX; i++) {
      if (targetISP == IDC06) {
        val = SPI.transfer(msgBuffer[4 + i]);
      } else {
        val = VirtualSPI.transfer(msgBuffer[4 + i]);
      }
      if (i >= rxStart && rxCount < numRX) {
        msgBuffer[2 + rxCount] = val;
        rxCount++;
      }
    }

    // Answer
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2 + rxCount] = STATUS_CMD_OK;
    msgLength = 3 + rxCount;

//***************************************************** 
//                    HVPP commands 
//*****************************************************  
  } else if (cmd == CMD_ENTER_PROGMODE_PP) {    // 0x20
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);

    // If current MCU package is none, disable progmode and answer timeout
    if (!currentMCUpackage) {
      STKError = msgBuffer[0];
      msgLength = 2;
      msgBuffer[1] = STATUS_CMD_TOUT;
      return;
    }

    STKP_stabDelay =    msgBuffer[1];   // 100 ms used for pin stabilization
    STKP_progModeDelay= msgBuffer[2];   // 0 ms Delay in connection with the EnterProgMode
    STKP_latchCycles =  msgBuffer[3];   // 5 Number of xtal cycles used to latch OSCCAL
    STKP_toggleVtg =    msgBuffer[4];   // 1 Toggle Vtg when entering prog.mode (0=no, 1=yes).
                                        // For parts with RSTDSBL functionality 
    STKP_powerOffDelay= msgBuffer[5];   // 15 ms Power-off delay. Additional delay after Vtg is turned off
    STKP_resetDelayMs = msgBuffer[6];   // 1 ms Additional delay between Vtg is turned on and reset goes high.
    STKP_resetDelayUs = msgBuffer[7];   // 0 x10us Additional delay between Vtg is turned on and
                          // reset goes high. Total delay is RSTDELAY #1 (ms) + RSTDELAY #2 (us x 10) 

    // 1. Príprava pinov
    setupHVxP(currentMCUpackage);
    
    dataMode(OUTPUT);
    dataWrite(0x00);

    // 2. Sekvencia zapínania napätí
    zifWrite(currentMCU.VCC1, HIGH);          // Zapni VCC (5V)
    if (currentMCU.VCC2)
      zifWrite(currentMCU.VCC2, HIGH);
    delayMicroseconds(100);                       // Stabilizácia

    // 3. Activate 12V to RESET pin
    HV_apply(currentMCU.HVPP);
    digitalWrite(HVxP_ON_12V, LOW);               // LOW value Power switch ON 12V
    delayMicroseconds(10);                        // Short delay on apply 12V
    if (digitalRead(VPP_CSENSE)) {                // Check if 12V not high currency
      digitalWrite(HVxP_ON_12V, HIGH);            // and disable 12V if high currency
      setupHVxP(currentMCUpackage, false);
      STKError = msgBuffer[0];
      msgLength = 2;
      msgBuffer[1] = STATUS_CMD_FAILED;
      return;
    }

    // 4. Dokončenie sekvencie vstupu
    // Niektoré čipy vyžadujú počas držania 12V krátky pulz na XTAL alebo WR
    zifWrite(HVPP.WR, HIGH);
    zifWrite(HVPP.OE, HIGH);
    
    // 5. Počkáme, kým čip potvrdí pripravenosť (ak RDY pin existuje)
    uint16_t timeout = 100;  // 1 ms
    while(zifRead(HVPP.RDY) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    progMode = STK_PROGMODE_HVPP;

    // Answer
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_LEAVE_PROGMODE_PP) {    // 0x21
                                                          if (STKdump) lcdPrintln("EndPP");
    digitalWrite(HVxP_ON_12V, HIGH);              // HIGH value Power switch OFF 12V
    setupHVxP(currentMCUpackage, false);

    progMode = STK_PROGMODE_NONE;

    newClear = true;
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_CHIP_ERASE_PP) {        // 0x22
    uint8_t  pulseWidth    = msgBuffer[1]; //  0 ms
    uint16_t timeout = 100 * msgBuffer[2]; // 10 ms

    loadCommand(0x80);
    zifWrite(HVPP.WR, LOW);
    if (pulseWidth) delay(pulseWidth);
    else delayMicroseconds(1);
    zifWrite(HVPP.WR, HIGH);

    while(zifRead(HVPP.RDY) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_FLASH_PP) {     // 0x23
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
//                                                          if (STKdump) if (msgBuffer[3] != 0xCF) lcdDump(msgBuffer, 5);
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t  mode = msgBuffer[3]; // 0xCF
    /*  Bit Function
        0   0 = Word mode, 1 = Page mode
        1-3 Page size Bit 3 2 1
                          0 0 0   256
                          0 0 1   2
                          0 1 0   4
                          0 1 1   8
                          1 0 0   16
                          1 0 1   32
                          1 1 0   64
                          1 1 1   128
        4-5 no function
        6   1 = Last page sent
        7   0 = Don't write page, 1 = Write page
    */
    uint16_t timeout = 100 * msgBuffer[4]; // 20 ms
    uint16_t p = 5;

    if (mode & 0x01) {              // PAGE MODE
      loadCommand(0x10);
      for (uint16_t i = 0; i < (size >> 1); i++) {
        loadAddress((STKP_address + i) & 0xFF, LOW);
        loadData(msgBuffer[p++], LOW);
        loadData(msgBuffer[p++], HIGH);
        //zifWrite(HVPP.BS1, HIGH);
        zifWrite(HVPP.PAGEL, HIGH); // LATCH Data
        delayMicroseconds(1);
        zifWrite(HVPP.PAGEL, LOW);
      }

      if (mode & 0x80) {           // WRITE PAGE
        loadAddress((STKP_address >> 8) & 0xFF, HIGH);
        commitWrite();                  // negative pulse WR starts programming

        while(zifRead(HVPP.RDY) == LOW && timeout > 0) {
          delayMicroseconds(10);
          timeout--;
        }
      }

      STKP_address += (size >> 1);

      if (mode & 0x40) {               // Last page programmed
        loadCommand(0x00);
      }
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_FLASH_PP) {        // 0x24
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint16_t p = 2;

    for (uint16_t i = 0; i < (size >> 1); i++) {
      loadCommand(0x02);
      loadAddress((STKP_address >> 8) & 0xFF, HIGH);
      loadAddress( STKP_address       & 0xFF, LOW);
      msgBuffer[p++] = readByte(LOW);
      msgBuffer[p++] = readByte(HIGH);
      STKP_address++;
    }
    
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[p++] = STATUS_CMD_OK;
    msgLength = p;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_EEPROM_PP) {    // 0x25
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t  mode = msgBuffer[3]; // 0xC5
                                                                  // bug t26 sends 0xC0
    /*  Bit Function
        0   0 = Word mode, 1 = Page mode
        1-3 Page size Bit 3 2 1
                          0 0 0   256
                          0 0 1   2
                          0 1 0   4
                          0 1 1   8
                          1 0 0   16
                          1 0 1   32
                          1 1 0   64
                          1 1 1   128
        4-5 no function
        6   1 = Last page sent
        7   0 = Don't write page, 1 = Write page
    */

    uint16_t timeout = 100 * msgBuffer[4]; // 20 ms
    uint16_t p = 5;

    if (mode & 0x01 || mode & 0x80) {              // PAGE MODE
      loadCommand(0x11);
      loadAddress((STKP_address >> 8) & 0xFF, HIGH);
      for (uint16_t i = 0; i < size; i++) {
        loadAddress( STKP_address & 0xFF, LOW);
        loadData(msgBuffer[p++]);
        zifWrite(HVPP.PAGEL, HIGH); // LATCH Data
        delayMicroseconds(1);
        zifWrite(HVPP.PAGEL, LOW);
        STKP_address++;
      }
      if (mode & 0x80) {           // WRITE PAGE
        zifWrite(HVPP.BS1, LOW);
        commitWrite();                  // negative pulse WR starts programming

        while(zifRead(HVPP.RDY) == LOW && timeout > 0) {
          delayMicroseconds(10);
          timeout--;
        }
      }
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_EEPROM_PP) {       // 0x26
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint16_t p = 2;

    for (uint16_t i = 0; i < size; i++) {
      loadCommand(0x03);
      loadAddress((STKP_address >> 8) & 0xFF, HIGH);
      loadAddress( STKP_address       & 0xFF, LOW);
      msgBuffer[p++] = readByte(LOW);
      STKP_address++;
    }
    
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[p++] = STATUS_CMD_OK;
    msgLength = p;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_FUSE_PP) {      // 0x27
    uint8_t fuse           = msgBuffer[1];
    // BS1=0, BS2=0 - low
    // BS1=1, BS2=0 - high
    // BS1=0, BS2=1 - extended
    // uint8_t data        = msgBuffer[2];
    uint8_t  pulseWidth    = msgBuffer[3]; // 0 ms
    uint16_t timeout = 100 * msgBuffer[4]; // 5 ms
    
    loadCommand(0x40);
    loadData(msgBuffer[2]);
    zifWrite(HVPP.BS1, (fuse & 0x01) == 1);
    zifWrite(HVPP.BS2, (fuse & 0x02) == 2);
    // zifWrite(HVPP.BS1, bitRead(msgBuffer[1], 0));
    // zifWrite(HVPP.BS2, bitRead(msgBuffer[1], 1));
    commitWrite(pulseWidth);

    while(zifRead(HVPP.RDY) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_FUSE_PP) {         // 0x28
    uint8_t fuse = msgBuffer[1];    // Address of fuse byte (low, high, ext, ext2)
    // bool bs1, bs2;
    // BS1=0, BS2=0 - low
    // BS1=1, BS2=1 - high
    // BS1=0, BS2=1 - extended
    // BS1=1, BS2=0 - lock
    // if (fuse == 0) {bs1 = 0; bs2 = 0;}
    // else
    // if (fuse == 1) {bs1 = 1; bs2 = 1;}
    // else           {bs1 = 0; bs2 = 1;}

    loadCommand(0x04);
    zifWrite(HVPP.BS2, fuse >= 1);
    val = readByte((fuse & 0x01) == 1);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_LOCK_PP) {      // 0x29
    // uint8_t Address     = msgBuffer[1];
    // uint8_t data        = msgBuffer[2];
    uint8_t  pulseWidth    = msgBuffer[3]; // 0 ms
    uint16_t timeout = 100 * msgBuffer[4]; // 5 ms
    
    loadCommand(0x20);
    loadData(msgBuffer[2]);
    commitWrite(pulseWidth);

    while(zifRead(HVPP.RDY) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_LOCK_PP) {         // 0x2A
    uint8_t fuse = msgBuffer[1];    // Address of fuse byte (low, high, ext, ext2)

    loadCommand(0x04);
    zifWrite(HVPP.BS2, LOW);    // requered if XA1/BS2 same pin
    val = readByte(HIGH);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_READ_SIGNATURE_PP) {    // 0x2B
    loadCommand(0x08);
    loadAddress(msgBuffer[1]);
    val = readByte(LOW);

    if (msgBuffer[1] <= 2) {
      signature[msgBuffer[1]] = val;      // Store signature
    }

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_READ_OSCCAL_PP) {       // 0x2C
    loadCommand(0x08);
    loadAddress(msgBuffer[1]);
    val = readByte(HIGH);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_SET_CONTROL_STACK) {    // 0x2D
    // Data = msgBuffer[1] - msgBuffer[33] // Control stack Data 32 bytes
    // Dump avrdude m328
    // 0e 1e 0f 1f 2e 3e 2f 3f
    // 4e 5e 4f 5f 6e 7e 6f 7f
    // 66 76 67 77 6a 7a 6b 7b
    // be fd 00 01 00 00 00 00

    /*
    Bit	Signál	  Význam (pre ATmega328P)

    Bit 0	PAGEL	  Program Memory Page Load
    Bit 1	XA1	    Action Bit 1 (určuje typ operácie)
    Bit 2	XA0	    Action Bit 0 (určuje typ operácie)
    Bit 3	BS1	    Byte Select 1 (výber horného/dolného bajtu)

    Bit 4	WR	    Write Pulse (aktívny v log. 0)
    Bit 5	OE	    Output Enable (aktívny v log. 0)
    Bit 6	RDY/BSY	Sledovanie stavu Ready/Busy
    Bit 7	BS2	    Byte Select 2 (len pre čipy s >8KB Flash)
    */

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;

//***************************************************** 
//                    HVSP commands 
//*****************************************************  
  } else if (cmd == CMD_ENTER_PROGMODE_HVSP) {  // 0x30
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);

    // If current MCU package is none, disable progmode and answer timeout
    if (!currentMCUpackage) {
      STKError = msgBuffer[0];
      msgLength = 2;
      msgBuffer[1] = STATUS_CMD_TOUT;
      return;
    }

    STKP_stabDelay    = msgBuffer[1];   // 100 ms used for pin stabilization
    STKP_cmdexeDelay  = msgBuffer[2];   // 0 ms Delay in connection with the EnterProgMode
    byte synchCycles  = msgBuffer[3];   // 6 Number of synchronization clock cycles 
    STKP_latchCycles  = msgBuffer[4];   // 1 Number of PulseXtal1_HVSP cycles 
    STKP_toggleVtg    = msgBuffer[5];   // 1 Toggle Vtg when entering prog.mode (0=no, 1=yes).
                                        // For parts with RSTDSBL functionality 
    STKP_powerOffDelay= msgBuffer[6];   // 25 ms Power-off delay. Additional delay after Vtg is turned off
    STKP_resetDelayMs = msgBuffer[7];   // 0 ms Additional delay between Vtg is turned on and reset goes high.
    STKP_resetDelayUs = msgBuffer[8];   // 70 x10us Additional delay between Vtg is turned on and
                          // reset goes high. Total delay is RSTDELAY #1 (ms) + RSTDELAY #2 (us x 10) 

    // 0. Load pins
    setupHVxP(currentMCUpackage);

    // 1. Set the Prog_enable pins to “000” and wait at least 100ns.
    if (HVSP.PEN0) zifWrite(HVSP.PEN0, LOW);
    if (HVSP.PEN1) zifWrite(HVSP.PEN1, LOW);
    if (HVSP.PEN2) zifWrite(HVSP.PEN2, LOW);
    if (HVSP.PEN3) zifWrite(HVSP.PEN3, LOW);
    zifWrite(currentMCU.RESET, LOW);

    // 2. Apply 4.5 - 5.5V between VCC and GND. Ensure that VCC reaches at least 1.8V within the next 20 µs
    zifWrite(currentMCU.VCC1, HIGH);          // Zapni VCC (5V)
    delayMicroseconds(100);                       // Stabilizácia

    // if (HVSP.SDO == HVSP.PEN2)                    // Small tiny25 requered toggle SCI at least six times
    //   for (byte i = 0; i < synchCycles; i++) toggleSCI();

    // 3. Apply HV to RESET
    HV_apply(currentMCU.HVPP);
    digitalWrite(HVxP_ON_12V, LOW);               // LOW value Power switch ON 12V
    delayMicroseconds(10);
    if (digitalRead(VPP_CSENSE)) {                // Check if 12V not high currency
      digitalWrite(HVxP_ON_12V, HIGH);            // and disable 12V if high currency
      setupHVxP(currentMCUpackage, false);
      STKError = msgBuffer[0];
      msgLength = 2;
      msgBuffer[1] = STATUS_CMD_FAILED;
      return;
    }

    // 4. Release the Prog_enable[2]/SDO pin after tHVRST has elapsed.
    zifMode(HVSP.PEN2, INPUT);
    zifMode(HVSP.SDO, INPUT);

    // 5. Wait at least 50µs before giving any serial instructions on SDI/SII.
    delayMicroseconds(300);                        // Short pause after 12V apply

    progMode = STK_PROGMODE_HVSP;

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_LEAVE_PROGMODE_HVSP) {  // 0x31
                                                          if (STKdump) lcdPrintln("EndSP");
    zifWrite(HVSP.SCI, LOW);
    zifWrite(currentMCU.RESET, LOW);
    digitalWrite(HVxP_ON_12V, HIGH);              // HIGH value Power switch OFF 12V
    setupHVxP(currentMCUpackage, false);

    progMode = STK_PROGMODE_NONE;

    newClear = true;
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_CHIP_ERASE_HVSP) {      // 0x32
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
    uint16_t timeout = 100 * msgBuffer[1];  // 0x28 eq 40 ms - Poll timeout
    uint8_t eraseTime = msgBuffer[2];       // 0x00 Delay (in ms) to ensure that the erase of the 
                                            // device is finished. If 0, polling will be used. 

    doubleTransfer(0x80, 0x4C);
    doubleTransfer(0x00, 0x64);
    doubleTransfer(0x00, 0x6C);

    if (eraseTime) delay(eraseTime);
    else while(zifRead(HVSP.SDO) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_FLASH_HVSP) {   // 0x33
//                                                          if (STKdump) lcdDump(msgBuffer, 7);
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t  mode = msgBuffer[3]; // 0xCD
                                                                  // bug t26 sends 0xC0
    /*  Bit Function
        0   0 = Word mode, 1 = Page mode
        1-3 Page size Bit 3 2 1
                          0 0 0   256
                          0 0 1   2
                          0 1 0   4
                          0 1 1   8
                          1 0 0   16
                          1 0 1   32
                          1 1 0   64
                          1 1 1   128
        4-5 no function
        6   1 = Last page sent
        7   0 = Don't write page, 1 = Write page
    */

    uint16_t timeout = 100 * msgBuffer[4]; // 6 ms
    uint16_t p = 5;

    doubleTransfer(0x10, 0x4C);     // Load “Write FLASH Command

    if (mode & 0x01) {                // PAGE MODE
      for (uint16_t i = 0; i < (size >> 1); i++) {
        doubleTransfer((STKP_address + i) & 0xFF, 0x0C);
        doubleTransfer(msgBuffer[p++]     , 0x2C);  // LOW byte
        doubleTransfer(0x00, 0x6D);
        doubleTransfer(0x00, 0x6C);
        doubleTransfer(msgBuffer[p++]     , 0x3C);  // HIGH byte
        doubleTransfer(0x00, 0x7D);
        doubleTransfer(0x00, 0x7C);
      }
      if (mode & 0x80) {              // WRITE PAGE
        doubleTransfer((STKP_address >> 8) & 0xFF, 0x1C);
        doubleTransfer(0x00, 0x64);
        doubleTransfer(0x00, 0x6C);

        while(zifRead(HVSP.SDO) == LOW && timeout > 0) {
          delayMicroseconds(10);
          timeout--;
        }
      }
      STKP_address += (size >> 1);
    
    // } else {                          // BYTE MODE
    //   for (uint16_t i = 0; i < size; i++) {
    //     doubleTransfer( STKP_address       & 0xFF, 0x0C);
    //     doubleTransfer((STKP_address >> 8) & 0xFF, 0x1C);
    //     doubleTransfer(msgBuffer[p++]            , 0x2C);
    //     doubleTransfer(0x00, 0x6D);
    //     doubleTransfer(0x00, 0x6C);
    //     doubleTransfer(msgBuffer[p++]            , 0x3C);
    //     doubleTransfer(0x00, 0x7D);
    //     doubleTransfer(0x00, 0x7C);
    //     doubleTransfer(0x00, 0x64);
    //     doubleTransfer(0x00, 0x6C);
    //     STKP_address++;
    //   }
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_FLASH_HVSP) {      // 0x34
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t p = 2;

    doubleTransfer(0x02, 0x4C);     // Load “Read FLASH" Command

    for (uint16_t i = 0; i < (size >> 1); i++) {
      doubleTransfer( STKP_address       & 0xFF, 0x0C);
      doubleTransfer((STKP_address >> 8) & 0xFF, 0x1C);
      doubleTransfer(0x00, 0x68);
      msgBuffer[p++] = doubleTransfer(0x00, 0x6C);  // LOW byte
      doubleTransfer(0x00, 0x78);
      msgBuffer[p++] = doubleTransfer(0x00, 0x7C);  // HIGH byte
      STKP_address++;
    }

    msgLength = 3 + size;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[p] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_EEPROM_HVSP) {  // 0x35
//                                                          if (STKdump) lcdDump(msgBuffer, msgLength);
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t  mode = msgBuffer[3]; // 0xC5
                                                                  // bug t26 sends 0xC0
    /*  Bit Function
        0   0 = Word mode, 1 = Page mode
        1-3 Page size Bit 3 2 1
                          0 0 0   256
                          0 0 1   2
                          0 1 0   4
                          0 1 1   8
                          1 0 0   16
                          1 0 1   32
                          1 1 0   64
                          1 1 1   128
        4-5 no function
        6   1 = Last page sent
        7   0 = Don't write page, 1 = Write page
    */

    uint16_t timeout = 100 * msgBuffer[4];     // 20 ms
    uint16_t p = 5;

    doubleTransfer(0x11, 0x4C);             // Load “Write EEPROM” Command

    if (mode & 0x01) {                // PAGE MODE
      for (uint16_t i = 0; i < size; i++) {
        doubleTransfer( STKP_address       & 0xFF, 0x0C);
        doubleTransfer((STKP_address >> 8) & 0xFF, 0x1C);
        doubleTransfer(msgBuffer[p++]            , 0x2C);
        doubleTransfer(0x00, 0x6D);
        doubleTransfer(0x00, 0x6C);
        STKP_address++;
      }
      if (mode & 0x80) {              // WRITE PAGE
        doubleTransfer(0x00, 0x64);
        doubleTransfer(0x00, 0x6C);

        while(zifRead(HVSP.SDO) == LOW && timeout > 0) {
          delayMicroseconds(10);
          timeout--;
        }
      }
    
    } else {                          // BYTE MODE
      for (uint16_t i = 0; i < size; i++) {
        doubleTransfer( STKP_address       & 0xFF, 0x0C);
        doubleTransfer((STKP_address >> 8) & 0xFF, 0x1C);
        doubleTransfer(msgBuffer[p++]            , 0x2C);
        doubleTransfer(0x00, 0x6D);
        doubleTransfer(0x00, 0x64);
        doubleTransfer(0x00, 0x6C);
        STKP_address++;
      }
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_EEPROM_HVSP) {     // 0x36
    uint16_t size = ((uint16_t)msgBuffer[1] << 8) | msgBuffer[2];
    uint8_t p = 2;

    doubleTransfer(0x03, 0x4C);             // Load “Read EEPROM” Command

    for (uint16_t i = 0; i < size; i++) {
      doubleTransfer( STKP_address       & 0xFF, 0x0C);
      doubleTransfer((STKP_address >> 8) & 0xFF, 0x1C);
      doubleTransfer(0x00, 0x68);
      msgBuffer[p++] = doubleTransfer(0x00, 0x6C);
      STKP_address++;
    }

    msgLength = 3 + size;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[p] = STATUS_CMD_OK;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_FUSE_HVSP) {    // 0x37
    uint8_t addr = msgBuffer[1];            // Address of fuse byte (low, high, ext, ext2)
    uint8_t fuse = msgBuffer[2];            // Fuse data
    uint16_t timeout = 100 * msgBuffer[3];  // 0x19 eq 25 ms - Poll timeout

    doubleTransfer(0x40, 0x4C);
    doubleTransfer(fuse, 0x2C);

    if (addr == 0) {
      doubleTransfer(0x00, 0x64);
      doubleTransfer(0x00, 0x6C);
    } else
    if (addr == 1) {
      doubleTransfer(0x00, 0x74);
      doubleTransfer(0x00, 0x7C);
    } else
    if (addr == 2) {
      doubleTransfer(0x00, 0x66);
      doubleTransfer(0x00, 0x6E);
    }

    while(zifRead(HVSP.SDO) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_FUSE_HVSP) {       // 0x38
    uint8_t addr = msgBuffer[1];            // Address of fuse byte (low, high, ext, ext2)

    doubleTransfer(0x04, 0x4C);

    if (addr == 0) {
      doubleTransfer(0x00, 0x68);
      val = doubleTransfer(0x00, 0x6C);
    } else
    if (addr == 1) {
      doubleTransfer(0x00, 0x7A);
      val = doubleTransfer(0x00, 0x7C);
    } else
    if (addr == 2) {
      doubleTransfer(0x00, 0x6A);
      val = doubleTransfer(0x00, 0x6E);
    }
    
    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_PROGRAM_LOCK_HVSP) {    // 0x39
    // uint8_t addr = msgBuffer[1];            // Address
    uint8_t fuse = msgBuffer[2];           // Fuse data
    uint16_t timeout = 100 * msgBuffer[3];  // 0x19 eq 25 ms - Poll timeout

    doubleTransfer(0x20, 0x4C);
    doubleTransfer(fuse, 0x2C);
    doubleTransfer(0x00, 0x64);
    doubleTransfer(0x00, 0x6C);

    while(zifRead(HVSP.SDO) == LOW && timeout > 0) {
      delayMicroseconds(10);
      timeout--;
    }

    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_OK;
    if (!timeout) {
      msgBuffer[1] = STATUS_RDY_BSY_TOUT;
      STKError = msgBuffer[0];
    }

//*****************************************************  
  } else if (cmd == CMD_READ_LOCK_HVSP) {       // 0x3A
    // uint8_t addr = msgBuffer[1];            // Address

    doubleTransfer(0x04, 0x4C);
    doubleTransfer(0x00, 0x78);
    // doubleTransfer(0x00, 0x68);
    val = doubleTransfer(0x00, 0x6C);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_READ_SIGNATURE_HVSP) {  // 0x3B
    uint8_t addr = msgBuffer[1];            // Address of signature byte

    doubleTransfer(0x08, 0x4C);
    doubleTransfer(addr, 0x0C);
    doubleTransfer(0x00, 0x68);
    val = doubleTransfer(0x00, 0x6C);

    if (msgBuffer[1] <= 2) {
      signature[msgBuffer[1]] = val;        // Store signature
    }

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else if (cmd == CMD_READ_OSCCAL_HVSP) {     // 0x3C
    // uint8_t addr = msgBuffer[1];            // Address

    doubleTransfer(0x08, 0x4C);
    doubleTransfer(0x00, 0x0C);
    doubleTransfer(0x00, 0x78);
    val = doubleTransfer(0x00, 0x7C);

    msgLength = 3;
    msgBuffer[1] = STATUS_CMD_OK;
    msgBuffer[2] = val;

//*****************************************************  
  } else {
                                                          if (STKdump) lcdPrintln("FAILED");
    STKError = msgBuffer[0];
    
    msgLength = 2;
    msgBuffer[1] = STATUS_CMD_FAILED;
  }
}



void stk500v2_process(uint8_t ch) {
//                                                          if (STKdump) lcdPrintHex(ch);
  
  if (msgState != STK_START && msgState != STK_GET_CHECK) {
    msgChecksum ^= ch;
  }

  switch (msgState) {
    case STK_START:
      if (ch != MESSAGE_START) break; // no start sign
      msgState = STK_GET_SEQ_NUM;
      msgChecksum = ch ^ 0;
      break;
      
    case STK_GET_SEQ_NUM:
      // if (ch != seqNum +1 && progMode) {
      // msgState = STK_START;         // seqNum not increment +1 -> DROP
      // } else {
        seqNum = ch;
        msgState = STK_MSG_SIZE_1;
      // }
      break;

    case STK_MSG_SIZE_1:
      msgLength = ch * 256;
      msgState = STK_MSG_SIZE_2;
      break;
      
    case STK_MSG_SIZE_2:
      msgLength += ch;
      if (msgLength > 285) {
        msgState = STK_START;         // big packet -> DROP
      } else {
        msgState = STK_GET_TOKEN;
      }
      break;
      
    case STK_GET_TOKEN:
      if (ch != TOKEN) {              // valid token -> DROP
        msgState = STK_START;
      } else {
        msgState = STK_GET_DATA;
        msgCPoint = 0;
      }
      break;
      
    case STK_GET_DATA:
      msgBuffer[msgCPoint] = ch;
      msgCPoint++;
      if (msgCPoint == msgLength) {
        msgState = STK_GET_CHECK;
      }
      break;
      
    case STK_GET_CHECK:
      if (msgChecksum != ch) {        // valid checksum
        msgState = STK_START;
      } else {
        msgState = STK_PROCESS;
        stk500v2_commands();          // execute command
        messageSend();
        msgState = STK_START;
      }
      break;
      
    default:
      msgState = STK_START;
      break;
  }
}

void signatureReadISP() {
  uint8_t buffer[] = { 0x10, 200, 100, 25, 32, 0, 0x53, 3, 0xAC, 0x53, 0, 0 };
  memcpy(&msgBuffer, &buffer, sizeof(buffer));
  stk500v2_commands();    // Enter Progmode
  msgBuffer[0] = 0x30;
  msgBuffer[1] = 0;
  msgBuffer[3] = 0;
  for (byte i = 0; i < 3; i++) {
    msgBuffer[2] = i;
    signature[i] = spi_transaction(msgBuffer, 0);
  }
  msgBuffer[0] = 0x11;
  stk500v2_commands();    // Leave Progmode
}

void signatureReadOLD() {
  uint8_t buffer[] = { 0x10, 200, 100, 25, 32, 0, 0x53, 3, 0xAC, 0x53, 0, 0 };
  memcpy(&msgBuffer, &buffer, sizeof(buffer));
  stk500v2_commands();
  msgBuffer[0] = 0x1B;
  for (byte i = 0; i < 3; i++) {
    msgBuffer[1] = 0;
    msgBuffer[2] = 0x30;
    msgBuffer[3] = 0;
    msgBuffer[4] = i;
    msgBuffer[5] = 0;
    stk500v2_commands();
  }
  msgBuffer[0] = 0x11;
  stk500v2_commands();
}

