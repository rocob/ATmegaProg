/*
  xtal_clock.h
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

volatile uint8_t xtalPin = 9;
volatile uint8_t xtalPinMask = 0x01;

void setupTimer(uint8_t p) {
  for (int i = 1; i < 41; i++) {
    if (ZIF[i] == p) {
      xtalPin = i;
      xtalPinMask = 1 << ((i - 1) & 0x07);
      break;
    }
  }
  if (!p) { // ZERO disable timer
    pinMode(ZIF[xtalPin], INPUT);
    TIMSK2 = 0;
    return;
  }
  pinMode(p, OUTPUT);       // Enable output clock
  cli();
  TCCR2A = (1 << WGM21);    // CTC mód
  TCCR2B = (1 << CS20);     // Prescaler = 1 (16 MHz)  // PARAM_OSC_PSCALE_VAL
  // TCCR2B = (1 << CS21);     // Prescaler = 8
  OCR2A = 80;               // PARAM_OSC_CMATCH_VAL
                            // Skúste hodnotu 10 pre cca 720 kHz (na hrane stability)
                            // value 80 eq 10 us == 100 kHz
  TIMSK2 = (1 << OCIE2A);
  sei();
}

ISR(TIMER2_COMPA_vect) {
  xtalPin &= 0x7F;
  // uint8_t xtalPinMask = 1 << ((xtalPin - 1) & 0x07);
  if      (xtalPin <=  8) PINF = xtalPinMask;  // ZIF01-08
  else if (xtalPin <= 16) PINK = xtalPinMask;  // ZIF09-16
  else if (xtalPin <= 24) PINL = xtalPinMask;  // ZIF17-24
  else if (xtalPin <= 32) PINC = xtalPinMask;  // ZIF25-32
  else                    PINA = xtalPinMask;  // ZIF33-40
}

//ISR(TIMER2_COMPA_vect, ISR_NAKED) { // ISR_NAKED šetrí čas procesora
//  asm volatile(
//    "sbi %0, %1 \n\t"  // Zapni/Toggle pin cez register PIN
//    "reti       \n\t"  // Okamžitý návrat
//    :: "I" (_SFR_IO_ADDR(PINF)), "I" (PF0)
//  );
//}
