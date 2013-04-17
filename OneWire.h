#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include <util/delay.h>

#define   Pin                   PB5    // Set up pin 12 as the data pin
#define   DIRECT_MODE_OUTPUT    DDRB |= _BV(Pin)
#define   DIRECT_MODE_INPUT     DDRB &= ~_BV(Pin)
#define   DIRECT_WRITE_HIGH     PORTB |= _BV(Pin)
#define   DIRECT_WRITE_LOW      PORTB &= ~_BV(Pin)
#define   DIRECT_READ           PINB & _BV(Pin) ? 1 : 0

#define   READROM               0x33  // Read the ROM of a OneWire device; there must only be one OneWire device on the bus!
#define   STARTCONVO            0x44  // Tells device to take a temperature reading and put it on the scratchpad
#define   READSCRATCH           0xBE  // Read EEPROM
#define   SKIPROM               0xCC
#define   MATCHROM              0x55
#define   TEMP_LSB              0
#define   TEMP_MSB              1

#define clk_div 1
#define   DELAY_A               6/clk_div
#define   DELAY_B               64/clk_div
#define   DELAY_C               60/clk_div
#define   DELAY_D               10/clk_div
#define   DELAY_E               9/clk_div
#define   DELAY_F               55/clk_div
#define   DELAY_G               0/clk_div
#define   DELAY_H               480/clk_div
#define   DELAY_I               72/clk_div
#define   DELAY_J               410/clk_div

uint8_t read(void);
void write(uint8_t v);
uint8_t reset(void);

#endif /* OneWire.h */
