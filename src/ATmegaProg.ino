/*
  ATmegaProg
  HW v1.0 , FW v1.0.0

  created 2026.03.03
  by Robert Kovaľ <http://www.toroproduction.sk>
  modified 2026.03.26
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
 *         /------- PA -------\    /------- PC -------\    /- PL -\
 *        7  6  5  4  3  2  1  0  7  6  5  4  3  2  1  0  7  6  5  4
 *     40 |  |  |  |  |  |  |  |  |  H  |  |  |  |  |  |  |  |  |  | 21
 *       +----------------------------------------------------------+
 *       |----------------------------------------+                 |
 *       |-------------------+                    |                 |
 *       |>      DIL 14      |   DIL 28           |   DIL 40        |
 *       |°------------------+                    |                 |
 *       |°---------------------------------------+                 |
 *       +°---------------------------------------------------------+
 *     1  H  |  |  H  |  |  |  |  H  H  |  |  |  |  |  |  |  |  |  | 20
 *        0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7  0  1  2  3
 *         \------- PF -------/    \------- PK -------/    \- PL -/
 *
 *
 *  o* pin 1 of ZIF40 is always pin 1 MCU package
 *  H* for this pins apply HV 12V
 ******************************************************************************
*/

#define PARAM_BUILD_NUMBER_HIGH_VAL 1
#define PARAM_BUILD_NUMBER_LOW_VAL  (16 * 0 + 0)  // max 15 + 15


#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_BLACK   0 // OLED display black color
#define SCREEN_WHITE   1 // OLED display white color

#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 
                            // 0x3D for 128x64, 0x3C for 128x32
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#if defined(ARDUINO_AVR_MEGA2560)
  #define SELA        6
  #define SELB        7
  #define SELC        8
  #define HVxP_ON_12V 9
  #define LED_HB      13
  #define LED_ERR     5
  #define LED_PMODE   0
  #define KEY_UP      12
  #define KEY_DOWN    11
  #define KEY_OK      10

  const uint8_t ZIF[41] = { 0, // pin 0 not exist
    54, 55, 56, 57, 58, 59, 60, 61, // PF
    62, 63, 64, 65, 66, 67, 68, 69, // PK
    49, 48, 47, 46, 45, 44, 43, 42, // PL
    37, 36, 35, 34, 33, 32, 31, 30, // PC
    22, 23, 24, 25, 26, 27, 28, 29  // PA
  };


  // PROGMODE_ISP External IDC6 Connector
  const uint8_t IDC06_PIN_RESET = 53;
  const uint8_t IDC06_PIN_MOSI  = 51;
  const uint8_t IDC06_PIN_MISO  = 50;
  const uint8_t IDC06_PIN_SCK   = 52;

#else
  #error Only ATmega2560 MCU Support!!!
#endif

#define HWVERSION     2
#define SWMAJOR       2   // 1
#define SWMINOR       10  // 18

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
  display.setCursor(0, 0);
  newClear = false;
  viewMode = VIEW_MODE_NONE;
}

void lcdPrintLogo() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SCREEN_WHITE);
  display.setCursor(0, 0);
  display.println("ATmegaProg");
  display.display();
  display.setTextSize(1);
}

void lcdPrintHex(uint32_t c, bool disp = true) {
  if (c < 16) display.print(0);
  display.print(c, HEX);
  display.print(" ");
  if (disp) display.display();
}

void lcdPrintln(char* c) {
  display.println(c);
  display.display();
}

void lcdDump(uint8_t *buffer, uint16_t size) {
  for (uint16_t i = 0; i < size; i++) {
    if (buffer[i] < 16) display.print(0);
    display.print(buffer[i], HEX);
    display.print(" ");
  }
  display.display();
  display.println();
}

void lcdPrintMCUname() {
  int16_t x = display.getCursorX();
  int16_t y = display.getCursorY();
  display.fillRect(12, 12, 100, 8, SCREEN_BLACK);
  display.setCursor(24, 12);
  display.print(deviceNameText);
  display.setCursor(x, y);
  display.display();
}

void viewMCU(uint8_t s, uint8_t type, uint8_t model) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SCREEN_WHITE);
  display.setCursor(4, 0);
  uint8_t half = s / 2;
  // display.print("12345678901234567890");
  for (uint8_t i = 40; i > 20; i--) {
    display.print((half > 0) ? char((i + s - 40) % 10 + 48) : char(124));
    if (half) half--;
  }
  display.drawRect(0, 8, 128, 15, SCREEN_WHITE);
  display.fillCircle(4, 18, 2, SCREEN_WHITE);
  display.setCursor(4, 24);
  half = s / 2;
  // display.print("12345678901234567890");
  for (uint8_t i = 1; i < 21; i++) {
    display.print((half > 0) ? char(i % 10 + 48) : char(124));
    if (half) half--;
  }
  if (signature[0] == 0x1E && progMode == 0) {
    if (model) {
      // lcdPrintMCUname();
      display.setCursor(24, 12);
      display.print(deviceNameText);
    } else {
      display.setCursor(12, 12),
      display.print("Unknown ");
      lcdPrintHex(signature[0]);
      lcdPrintHex(signature[1]);
      lcdPrintHex(signature[2]);
    }
  } else {
    display.setCursor(48, 12);
    display.print("DIL ");
    display.print(s);
    display.print(" - ");
    display.print(type);
  }
  display.display();
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
  display.begin(SCREEN_ADDRESS, true); // Address 0x3C default
  lcdPrintLogo();

  // High Voltage Selector setup
  digitalWrite(HVxP_ON_12V, HIGH);  // Disable HIGH VOLTAGE
  pinMode(SELA, OUTPUT);
  pinMode(SELB, OUTPUT);
  pinMode(SELC, OUTPUT);
  pinMode(HVxP_ON_12V, OUTPUT);

  // LEDs setup
  digitalWrite(LED_ERR, LOW);
  pinMode(LED_HB, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
  pinMode(LED_PMODE, OUTPUT);

  // Keys setup
  pinMode(KEY_UP  , INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_OK  , INPUT_PULLUP);

  //SPI.begin(PIN_MOSI, PIN_MISO, PIN_SCK, PIN_RESET);

  // ZIF40 setup
  for (uint8_t i = 1; i < 41; i++) {
    digitalWrite(ZIF[i], LOW);
    pinMode(ZIF[i], INPUT);
  }

  // setup ATmega328
  currentMCUpackage = 0;


  // setup Target
  bool targetISP = ZIF40;

  //setupTimer(ZIF[39]);
}



void loop() {
  uint32_t now = millis();
  // is pmode active?
  if (progMode) {
    digitalWrite(LED_PMODE, HIGH);
  } else {
    digitalWrite(LED_PMODE, LOW);

    if (digitalRead(KEY_UP) == LOW) {
      // key up
      if (viewMode == VIEW_MODE_NONE)
        viewMode = VIEW_MODE_PACKAGE;
      else if (viewMode == VIEW_MODE_PACKAGE)
        viewMode = VIEW_MODE_MCUINFO;
      else
        viewMode = VIEW_MODE_NONE;
      delay(300);

    } else if (viewMode == VIEW_MODE_MCUINFO) {
      uint8_t model = findDevice(signature[1], signature[2]);
      display.clearDisplay();
      display.setCursor(0, 0);
      if (model) {
        display.print("Model: ");
        display.println(deviceNameText);
      } else if (signature[0] == 0x1E) {
        display.println("Model: ---");
      } 
      display.print("Flash: ");
      display.println(calcSize(signature[1]));
      display.print("Sign : ");
      lcdPrintHex(signature[0], false);
      lcdPrintHex(signature[1], false);
      lcdPrintHex(signature[2], false);
      display.display();

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
        uint8_t model = findDevice(signature[1], signature[2]);
        viewMCU(s, currentMCUpackage, model);
      } else {
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
  heartbeat(LED_HB);

  if (SERIAL.available()) {
    stk500v2_process();

  }

}
