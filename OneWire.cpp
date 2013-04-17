#include "OneWire.h"
#include "Arduino.h"

uint8_t read() {
  uint8_t r=0;
      
  noInterrupts();
  for (uint8_t bitMask = 0x01; bitMask; bitMask <<= 1) {
    DIRECT_MODE_OUTPUT;
    DIRECT_WRITE_LOW;
    _delay_us(DELAY_A);
    DIRECT_MODE_INPUT;
    _delay_us(DELAY_E);
    if (DIRECT_READ) r |= bitMask;
    _delay_us(DELAY_F);
  }
  interrupts();
  return r;
}

void write(uint8_t v) {
  noInterrupts();
  for (uint8_t bitMask = 0x01; bitMask; bitMask <<= 1) {
    DIRECT_WRITE_LOW;
    DIRECT_MODE_OUTPUT;
    if (bitMask & v) {	
       _delay_us(DELAY_A);
       DIRECT_WRITE_HIGH;
      _delay_us(DELAY_B);
    } else {
      _delay_us(DELAY_C);
      DIRECT_WRITE_HIGH;
      _delay_us(DELAY_D);
    }
  }
  DIRECT_MODE_INPUT;
  DIRECT_WRITE_LOW;
  interrupts();
}

uint8_t reset(void) {
  noInterrupts();
  DIRECT_MODE_INPUT;      
  DIRECT_WRITE_LOW;
  DIRECT_MODE_OUTPUT;
  _delay_us(DELAY_H);
  DIRECT_MODE_INPUT;
  _delay_us(DELAY_I);
  uint8_t ret = !(DIRECT_READ);
  interrupts();
  _delay_us(DELAY_J);
  return ret;
}
