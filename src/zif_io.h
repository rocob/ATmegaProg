/*
  zif_io.h
  part of ATmegaProg

  created 2026.04.16
  by Robert Kovaľ <http://www.toroproduction.sk>
  modified 2026.04.21
  by Robert Kovaľ

  This is private source code
  Copyright 2025-2026 Robert Kovaľ and TORO-Production s.r.o.
  This program is Free Software and has ABSOLUTELY NO WARRANTY
  Please report bugs to mail@rocob.biz
  See LICENSE.txt for details.
*/

#define BAUDRATE      115200 //19200
#define SPI_CLOCK     (1000000/6)       // PARAM_SCK_DURATION_VAL
#define SPI_MODE0     0x00

#include "Arduino.h"

volatile uint8_t* const DDR_REGS[]  = { &DDRF, &DDRK, &DDRL, &DDRC, &DDRA, &DDRB };
volatile uint8_t* const PORT_REGS[] = { &PORTF, &PORTK, &PORTL, &PORTC, &PORTA, &PORTB };
volatile uint8_t* const PIN_REGS[]  = { &PINF, &PINK, &PINL, &PINC, &PINA, &PINB };

volatile uint8_t  xtalPin     = 0;
volatile uint8_t* xtalPinPort = 0;
volatile uint8_t  xtalPinMask = 1;

// Direction setup (mode: 1 = OUTPUT, 0 = INPUT, 2 = INPUT_PULLUP)
void zifMode(uint8_t pinIdx, uint8_t mode) {
  uint8_t port = pinIdx >> 3;
  uint8_t mask = (1 << (pinIdx & 7));
  
  if (mode == OUTPUT)
    *DDR_REGS[port] |= mask;
  else {
    *DDR_REGS[port] &= ~mask;

    if (mode == INPUT_PULLUP)
      *PORT_REGS[port] |= mask;
    else
      *PORT_REGS[port] &= ~mask;
  }
}

// Write HIGH/LOW
void zifWrite(uint8_t pinIdx, bool state) {
  uint8_t port = pinIdx >> 3;
  uint8_t mask = (1 << (pinIdx & 7));
  
  if (state) *PORT_REGS[port] |= mask;
  else       *PORT_REGS[port] &= ~mask;
}

// Read state of pin
bool zifRead(uint8_t pinIdx) {
  uint8_t port = pinIdx >> 3;
  uint8_t mask = (1 << (pinIdx & 7));

  return (*PIN_REGS[port] & mask);
//  if (*PIN_REGS[port] & mask) return true;
//  return false;
}

void xtalSetup(uint8_t p) {
  if (!p || p > 48) { // ZERO disable timer
    TIMSK2 = 0;
    // Change xtalPin direction back to input
    // *(xtalPinPort + 2) &= ~xtalPinMask;   // PORT is PIN + 2
    // *(xtalPinPort + 1) &= ~xtalPinMask;   // DDR  is PIN + 1
    zifMode(xtalPin, INPUT);
    xtalPin = 0;
    return;
  }
  zifMode(p, OUTPUT);       // Enable output clock
  xtalPin = p;
  xtalPinPort = PIN_REGS[(p >> 3)];
  xtalPinMask = 1 << (p & 7);

  cli();
  TCCR2A = (1 << WGM21);    // CTC mode
  TCCR2B = (1 << CS20);     // Prescaler = 1 (16 MHz)  // PARAM_OSC_PSCALE_VAL?
  // TCCR2B = (1 << CS21);     // Prescaler = 8
  OCR2A = 80;               // PARAM_OSC_CMATCH_VAL?
                            // value 10 for 720 kHz (na hrane stability)
                            // value 80 eq 10 us == 100 kHz
  TIMSK2 = (1 << OCIE2A);
  sei();
}

ISR(TIMER2_COMPA_vect) {
  *xtalPinPort = xtalPinMask;
  *xtalPinPort = xtalPinMask;
}

class vSPISettings {
  public:
    // clock is in Hz
    vSPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) : clockFreq(clock) {
      (void) bitOrder;
      (void) dataMode;
    };

    uint32_t getClockFreq() const {
      return clockFreq;
    }

  private:
    uint32_t clockFreq;
};



class BitBangedSPI {
  private:
    uint8_t pin_sck;
    uint8_t pin_mosi;
    uint8_t pin_miso;
    unsigned long pulseWidth; // in microseconds

  public:
    void begin(uint8_t mosi, uint8_t miso, uint8_t sck) {
      pin_mosi = mosi;
      pin_miso = miso;
      pin_sck  = sck;
      zifWrite(pin_sck, LOW);
      zifWrite(pin_mosi, LOW);
      zifMode(pin_sck, OUTPUT);
      zifMode(pin_mosi, OUTPUT);
      zifMode(pin_miso, INPUT);
    }

    void beginTransaction(vSPISettings settings) {
      pulseWidth = (500000 + settings.getClockFreq() - 1) / settings.getClockFreq();
      if (pulseWidth == 0) {
        pulseWidth = 1;
      }
      // pulseWidth = 4;
    }

    void end() {
      zifMode(pin_sck, INPUT);
      zifMode(pin_mosi, INPUT);
    }

    uint8_t transfer(uint8_t b) {
      for (byte i = 0; i < 8; ++i) {
        zifWrite(pin_mosi, (b & 0x80) ? HIGH : LOW);
        zifWrite(pin_sck, HIGH);
        delayMicroseconds(pulseWidth);
        b = (b << 1) | zifRead(pin_miso);
        zifWrite(pin_sck, LOW); // slow pulse
        delayMicroseconds(pulseWidth);
        // whithout delay clock is 18.4 us
        // whith delay 4 us clock is 26 us = 38 kHz
        // whith zifWrite() 28 us = 36 kHz ???
      }
      return b;
    }

};
