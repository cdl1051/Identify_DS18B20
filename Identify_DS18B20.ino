// Solder-free method of detecting the ROM of a DS18B20 - spread the 3 legs of the sensor wide enough to fit into GND, 13, and 12 
// and place the sensor in these pins with the flat side facing the LED on the Uno and the round side facing away from the Uno.  
// Then place a 4.7 KΩ resistor in pin 13 and 12.  Upload and open a serial monitor with a baud rate of 9600.
#include "OneWire.h"    // An extremely space-efficient implementation of selected portions of the OneWire protocol
#include "simpleTx.h"   // An extremely space-efficient serial debug tool

// ====================================================== Pre-Compiler Definitions ====================================================== 
#define   PowerPin              PB4                       // Pin 12 - we will be using this pin to supply Vcc to the DS18B20
#define   POWER_TEMP_PROBE      PORTB |= _BV(PowerPin)    // Define method for powering the DB18B20
#define   DEPOWER_TEMP_PROBE    PORTB &= ~_BV(PowerPin)   // Define method for depowering the DB18B20

void Tiny_RawTempToString(uint32_t raw, char * buffer) {  
  uint8_t decimalPos=2;  // default case of a temp between 0 and 99.9
  uint8_t nullPos=7;     // default case of a temp between 0 and 99.9
  
  raw=raw*1125ul+320000ul;    // Hold the temperature
  
  if(raw>1000000ul) {  // We're looking at a positive number with three digits
    decimalPos=3;
    nullPos=8;
  }
  
  for(int8_t i=nullPos;i>-1;i--) {
    if (i==decimalPos) {
      buffer[i]='.';
    }
    else if (i==nullPos) {
      buffer[i]='\0';
    }
    else {
      buffer[i]=raw%10 + '0';
      raw/=10;
    }
  } 
}

int main() {  
  //init();
  char temp[10];
  
  uint8_t ROM[8] = { 0x28, 0xA, 0xA3, 0x83, 0x4, 0x0, 0x0, 0x63 };
  uint8_t scratchPad[2];

  DDRB = B00010000;
  
  for(;;) {  // Loop forever
    // Perform a OneWire reset pulse
    POWER_TEMP_PROBE;      // This isn't neccesary but I show it to demonstrate how to make the project conserve energy
    
    simpletx("Presence pulse: ");
    if(reset()) {
      simpletx("Detected\t");
    } else {
      simpletx("Not Detected\t");
    }
    
    // Attempt to read the ROM, if nothing is present this will return 0x00 for each of the eight bytes
    simpletx("ROM is: ");
    write(READROM);
    for(uint8_t i=0;i<8;i++) {
      ROM[i]=read();
      simpletx("0x");
      txByteAsHex(ROM[i]);
      if (i!=7) simpletx(",");
    }
    simpletx("\t\t");
    DEPOWER_TEMP_PROBE;    // This isn't neccesary but I show it to demonstrate how to make the project conserve energy
    
    // If we detected a Dallas family sensor, let's go ahead and take a temperature reading
    if (ROM[0]=0x28) {  // The first byte of all dallas sensors is always 0x28
      POWER_TEMP_PROBE;
      reset();
      write(SKIPROM);
      write(STARTCONVO);
      _delay_ms(750);
      
      reset();
      write(MATCHROM);
      for(uint8_t i = 0; i < 8; i++) write(ROM[i]);
      write(READSCRATCH);
      scratchPad[TEMP_LSB] = read();    
      scratchPad[TEMP_MSB] = read();
      reset();
      DEPOWER_TEMP_PROBE;
      uint16_t rawTemperature = (((int16_t)scratchPad[TEMP_MSB]) << 8) | scratchPad[TEMP_LSB];
      Tiny_RawTempToString(rawTemperature,temp);
      
      simpletx("Temperature: ");
      simpletx(temp);
      //txRawTempAsFloat(rawTemperature);
      simpletx("F\n");           
    }
    _delay_ms(5000);
  }  // End for
}  // End main
