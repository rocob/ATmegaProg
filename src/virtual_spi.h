/*
  virtual_spi.h
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

#define BAUDRATE      115200 //19200
#define SPI_CLOCK     (1000000/6)       // PARAM_SCK_DURATION_VAL
#define SPI_MODE0     0x00

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
      digitalWrite(pin_sck, LOW);
      digitalWrite(pin_mosi, LOW);
      pinMode(pin_sck, OUTPUT);
      pinMode(pin_mosi, OUTPUT);
      pinMode(pin_miso, INPUT);
    }

    void beginTransaction(vSPISettings settings) {
      pulseWidth = (500000 + settings.getClockFreq() - 1) / settings.getClockFreq();
      if (pulseWidth == 0) {
        pulseWidth = 1;
      }
      // pulseWidth = 4;
    }

    void end() {
      pinMode(pin_sck, INPUT);
      pinMode(pin_mosi, INPUT);
    }

    uint8_t transfer(uint8_t b) {
      for (byte i = 0; i < 8; ++i) {
        digitalWrite(pin_mosi, (b & 0x80) ? HIGH : LOW);
        digitalWrite(pin_sck, HIGH);
        delayMicroseconds(pulseWidth);
        b = (b << 1) | digitalRead(pin_miso);
        digitalWrite(pin_sck, LOW); // slow pulse
        delayMicroseconds(pulseWidth);
        // whithout delay clock is 18.4 us
        // whith delay 4 us clock is 26 us = 38 kHz
      }
      return b;
    }

};
