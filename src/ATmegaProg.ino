/*
  ATmegaProg
  HW v1.0 , FW v1.1.0

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

/******************************************************************************
 *                           ZIF 40 connection
 *
 *
 *       PB   /------- PA -------\    /------- PC -------\    / PL \
 *        0  7  6  5  4  3  2  1  0  7  6  5  4  3  2  1  0  7  6  5
 *     40 |  |  |  |  |  |  |  |  |  H  |  |  |  |  |  |  |  |  |  | 21
 *       +----------------------------------------------------------+
 *       |----------------------------------------+                 |
 *       |-------------------+                    |                 |
 *       |>      DIL 14      |   DIL 28           |   DIL 40        |
 *       |°------------------+                    |                 |
 *       |°---------------------------------------+                 |
 *       +°---------------------------------------------------------+
 *     1  H  |  |  H  |  |  |  |  H  H  |  |  |  |  |  |  |  |  |  | 20
 *        1  2  3  4  5  6  7  0  1  2  3  4  5  6  7  0  1  2  3  4
 *         \----- PF ------/    \------- PK -------/    \-- PL ---/
 *
 *
 *  o* pin 1 of ZIF40 is always pin 1 MCU package
 *  H* for this pins apply HV 12V
 ******************************************************************************
*/

#define PARAM_BUILD_NUMBER_HIGH_VAL 1
#define PARAM_BUILD_NUMBER_LOW_VAL  (16 * 1 + 0)  // max 15 + 15

#define HWVERSION     2
#define SWMAJOR       2   // 1
#define SWMINOR       10  // 18

//#define HWVERSION     0
//#define SWMAJOR       1
//#define SWMINOR       41


#include "Arduino.h"
#include <Wire.h>
#include <U8g2lib.h>

//U8G2_SSD1312_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1107_SEEED_128X128_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#if defined(ARDUINO_AVR_MEGA2560)
  #define SELA        6
  #define SELB        7
  #define SELC        8
  #define HVxP_ON_12V 9
  #define VPP_CSENSE  2
  #define LED_RUN     5
  #define LED_ERR     4
  #define LED_PROG    3
  #define KEY_UP      12  // PB6 / 47
  #define KEY_DOWN    11  // PB5 / 46
  #define KEY_OK      10  // PB4 / 45
  #define VTG_PIN     54  // A0
  #define VTG_REF     INTERNAL2V56 // DEFAULT

//  const uint8_t ZIF[41] = { 0, // pin 0 not exist
//    54, 55, 56, 57, 58, 59, 60, 61, // PF
//    62, 63, 64, 65, 66, 67, 68, 69, // PK
//    49, 48, 47, 46, 45, 44, 43, 42, // PL
//    37, 36, 35, 34, 33, 32, 31, 30, // PC
//    22, 23, 24, 25, 26, 27, 28, 29  // PA
//  };


  // PROGMODE_ISP External IDC6 Connector
  #define IDC06_PIN_RESET 41  // 53; // PB0
  #define IDC06_PIN_MOSI  43  // 51; // PB2
  #define IDC06_PIN_MISO  44  // 50; // PB3
  #define IDC06_PIN_SCK   42  // 52; // PB1

#else
  #error Only ATmega2560 MCU Support!!!
#endif

#undef SERIAL
#ifdef SERIAL_PORT_USBVIRTUAL
  #define SERIAL SERIAL_PORT_USBVIRTUAL
#else
  #define SERIAL Serial
#endif


uint8_t ZIF40_PIN_RESET;
uint8_t ZIF40_PIN_MOSI;
uint8_t ZIF40_PIN_MISO;
uint8_t ZIF40_PIN_SCK;

#define ZIF40 false
#define IDC06 true

bool targetISP = ZIF40;

uint8_t STKError = 0;
uint8_t progMode = 0;
uint8_t signature[3] = {0,0,0};
char deviceNameText[15];

bool newClear = true;
//bool enMCUview = true;
bool STKdump = false;
uint32_t detectTime = 0;

uint8_t viewMode = 0;
#define VIEW_MODE_NONE      0
#define VIEW_MODE_PACKAGE   1
#define VIEW_MODE_MCUINFO   2


void lcdClear() {
  display.clearDisplay();
  display.setCursor(0, 8);
  newClear = false;
  viewMode = VIEW_MODE_NONE;
}

void lcdPrintLogo() {
  display.clearBuffer();
  display.setFontMode(1);
  display.setFont(u8g2_font_lubB14_tr);  // u8g2_font_cu12_tr
  display.setCursor(0, 16);
  display.print("ATmegaProg");
  display.drawHLine(0, 18, 56);
  display.drawHLine(74, 18, 38);
  display.sendBuffer();
  display.setFont(u8g2_font_6x10_tr);   // u8g2_font_squeezed_b7_tr
}

void lcdPrintHex(uint32_t c, char e = ' ') {
  uint16_t x = display.getCursorX();
  uint16_t y = display.getCursorY();
  if (x > 120) {
    if (y < 64) display.setCursor(0, y + 8);
    else {
      // display.clearBuffer();
      // display.setCursor(0, 8);
      return;
    }
  }
  if (c < 16) display.print(0);
  display.print(c, HEX);
  if (e == ' ')
    display.setCursor(display.getCursorX() + 4, display.getCursorY());
  else display.print(e);
  display.sendBuffer();
}

void lcdPrintln(char* c) {
  display.setCursor(display.getCursorX(), display.getCursorY() + 8);
  display.print(c);
  display.sendBuffer();
}

void lcdDump(uint8_t *buffer, uint16_t size) {
  uint16_t x, y;
  for (uint16_t i = 0; i < size; i++) {
    x = display.getCursorX();
    y = display.getCursorY();
    if (x > 120) {
      if (y < 64) display.setCursor(0, y + 8);
      else {
        // display.clearBuffer();
        // display.setCursor(0, 8);
        return;
      }
    }
    if (buffer[i] < 16) display.print(0);
    display.print(buffer[i], HEX);
    display.print(" ");
  }
  display.sendBuffer();
  display.setCursor(0, display.getCursorY() + 8);
}

void viewMCU(uint8_t s, uint8_t type, bool model) {
  display.clearBuffer();
  display.setCursor(4, 7);
  uint8_t half = s / 2;
  // display.print("12345678901234567890");
  for (uint8_t i = 40; i > 20; i--) {
    display.print((half > 0) ? char((i + s - 40) % 10 + 48) : char(124));
    if (half) half--;
  }
  display.drawFrame(0, 8, 128, 15);
  display.drawCircle(4, 18, 2);
  display.setCursor(4, 32);
  half = s / 2;
  // display.print("12345678901234567890");
  for (uint8_t i = 1; i < 21; i++) {
    display.print((half > 0) ? char(i % 10 + 48) : char(124));
    if (half) half--;
  }
  if (signature[0] == 0x1E && progMode == 0) {
    if (model) {
      display.setCursor(24, 19);
      display.print(deviceNameText);
    } else {
      display.setCursor(12, 19),
      display.print("Unknown ");
      for (uint8_t i = 0; i < 3; i++) {
        if (signature[i] < 16) display.print(0);
        display.print(signature[i], HEX);
      }
    }
  } else {
    display.setCursor(24, 19);
    display.print("DIL ");
    display.print(s);
    display.print(" - ");
    display.print(type);
  }
  display.setCursor(108, 19);
  if (STKError) {
    display.print("E");
    display.print(STKError, HEX);
  }
  display.sendBuffer();
}

void lcdPrintInfo(bool model) {
  display.clearBuffer();
  display.setCursor(0, 8);
  if (model) {
    display.print("Model: ");
    display.print(deviceNameText);
  } else { //if (signature[0] == 0x1E) {
    display.print("Model: ---");
  } 
  display.setCursor(0, display.getCursorY() + 8);
  display.print("Flash: ");
  display.print(calcSize(signature[1]));
  display.setCursor(0, display.getCursorY() + 8);
  display.print("Sign : ");
  for (uint8_t i = 0; i < 3; i++) {
    if (signature[i] < 16) display.print(0);
    display.print(signature[i], HEX);
  }
  display.setCursor(0, display.getCursorY() + 8);
  if (STKError) {
    display.print("Error: ");
    display.print(STKError, HEX);
  }
  display.sendBuffer();
}

char* calcSize(uint8_t s) {
  static char buffer[10]; 

  if (s < 0x8F) {
    return (char*)"-";
  } else if (s == 0x8F) {
    return (char*)"512 B";
  } else if (s < 0x9A) {
    uint16_t size = 1 << (s & 0x0F);
    sprintf(buffer, "%u kB", size);
    return buffer;
  } else {
    return (char*)"big";
  }
}

uint8_t voltageSense() {
  pinMode(VTG_PIN, INPUT);
  delayMicroseconds(10);
  uint16_t v = analogRead(VTG_PIN);
  return v/20;
}



#include "stk500v2.h"



// this provides a heartbeat on LED, so you can tell the software is running.
uint8_t hbval = 128;
int8_t hbdelta = 8;
void heartbeat(uint8_t led) {
  static unsigned long last_time = 0;
  unsigned long now = millis();
  if ((now - last_time) < 40) {
    return;
  }
  last_time = now;
  if (hbval > 192) {
    hbdelta = -hbdelta;
  }
  if (hbval < 32) {
    hbdelta = -hbdelta;
  }
  hbval += hbdelta;
  analogWrite(led, hbval);
}



void setup() {
  // Setup programmer
  SERIAL.begin(BAUDRATE);

  delay(250); // wait for the OLED to power up
  display.begin();
  lcdPrintLogo();

  // High Voltage Selector setup
  digitalWrite(HVxP_ON_12V, HIGH);  // Disable HIGH VOLTAGE
  pinMode(SELA, OUTPUT);
  pinMode(SELB, OUTPUT);
  pinMode(SELC, OUTPUT);
  pinMode(HVxP_ON_12V, OUTPUT);

  // Voltage & Currency Sense setup
  pinMode(VTG_PIN, INPUT);
  pinMode(VPP_CSENSE, INPUT);
  analogReference(VTG_REF);
  // analogReadResolution(10);

  // LEDs setup
  digitalWrite(LED_ERR, LOW);
  digitalWrite(LED_PROG, LOW);
  pinMode(LED_RUN, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
  pinMode(LED_PROG, OUTPUT);

  // Keys setup
  pinMode(KEY_UP  , INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_OK  , INPUT_PULLUP);

  // ZIF40 setup
  for (uint8_t i = 1; i < 41; i++) {
    zifWrite(i, LOW);
    zifMode(i, INPUT);
  }

  // setup ATmega328
  currentMCUpackage = 0;


  // setup Target
  bool targetISP = ZIF40;

  // run xtal clock
  //xtalSetup(48);
  //delay(2000);
  //xtalSetup(0);

  // read & display VTARGET
  display.setCursor(0, 32);
  uint8_t vtg = voltageSense();
  display.print("ISP VTARGET: ");
  display.print(vtg/10);
  display.print(".");
  display.print(vtg%10);
  display.print("V");
  display.sendBuffer();
  
}



void loop() {
  uint32_t now = millis();
  // is pmode active?
  if (progMode) {
    digitalWrite(LED_PROG, HIGH);
  } else {
    if (viewMode == VIEW_MODE_NONE)
      digitalWrite(LED_PROG, LOW);

    if (digitalRead(KEY_UP) == LOW) {
      // key up
      if (viewMode == VIEW_MODE_NONE)
        viewMode = VIEW_MODE_PACKAGE;
      else if (viewMode == VIEW_MODE_PACKAGE)
        viewMode = VIEW_MODE_MCUINFO;
      else {
        viewMode = VIEW_MODE_PACKAGE; //VIEW_MODE_NONE;
        if (xtalPin) xtalSetup(0);
        else xtalSetup(47);
      }
      delay(300);

    } else if (viewMode == VIEW_MODE_MCUINFO && \
      detectTime + 1000 < now) {
      detectTime = now;
      bool model = findDevice(signature[1], signature[2]);
      lcdPrintInfo(model);
      digitalWrite(LED_PROG, LOW);

    } else if (viewMode == VIEW_MODE_PACKAGE && \
      detectTime + 1000 < now) { // detection every 1 second
      detectTime = now;
      uint8_t s = detectMCUsize();
      if (s) {
        if (s == 28) {
          currentMCUpackage = 1;
        } else if (s == 20) {
          if (detectedPins[5] == LOW && detectedPins[36] == LOW) {
            currentMCUpackage = 3;
          } else {
            currentMCUpackage = 2;
          }
        } else if (s == 14) {
          currentMCUpackage = 4;
        } else if (s == 8) {
          currentMCUpackage = 5;
        }
        if (signature[0] != 0x1E) {
          STKError = 0;
          digitalWrite(13, HIGH);
          signatureReadISP();
          viewMode = VIEW_MODE_PACKAGE;
          digitalWrite(13, LOW);
        }
        bool model = findDevice(signature[1], signature[2]);
        viewMCU(s, currentMCUpackage, model);
        if (!STKError)
          digitalWrite(LED_PROG, digitalRead(LED_PROG) == LOW ? HIGH : LOW);
      } else {
        digitalWrite(LED_PROG, LOW);
        currentMCUpackage = 0;
        signature[0] = 0;
        signature[1] = 0;
        signature[2] = 0;
        //viewMCU(0, 0);
        //delay(1000);
        lcdPrintLogo();
      }
      //delay(300);
    }
  } // end if progMode

  // is there an error?
  if (STKError) {
    digitalWrite(LED_ERR, HIGH);
  } else {
    digitalWrite(LED_ERR, LOW);
  }

  // light the heartbeat LED
  heartbeat(LED_RUN);

  if (SERIAL.available()) {
    stk500v2_process(SERIAL.read());

  }

}
