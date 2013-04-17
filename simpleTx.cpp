#include "simpleTx.h"
#include "Arduino.h"

void simpletx( char * string ) {
  if (UCSR0B != (1<<TXEN0)) { //do we need to init the uart?
    UBRR0H = (unsigned char)(myubbr>>8);
    UBRR0L = (unsigned char)myubbr;
    UCSR0A = 0;//Disable U2X mode
    UCSR0B = (1<<TXEN0);//Enable transmitter
    UCSR0C = (3<<UCSZ00);//N81
    _delay_ms(30);
  }
  while (*string) {
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = *string++; //send the data
  }
}

void txByteAsHex(uint8_t inp) {
  char snd[3];
  uint8_t tmp = inp>>4;
  //uint8_t tmp = inp/16;
  
  if (tmp<10) {
    snd[0]=48+tmp;
  } else {
    snd[0]=55+tmp;
  }
  
  tmp=inp%16;
  if (tmp<10) {
    snd[1]=48+tmp;
  } else {
    snd[1]=55+tmp;
  }
  snd[2]='\0';
  simpletx(snd);
}

// Converts the raw temperature from a DS18B20 directly to a string containing the temperature in °F with 1 decimal place
// avoids unnecessary floating point math, float variables, and casts, and 32-bit math
// TODO: May not work properly with temperatures below 32°F
void txRawTempAsFloat(uint16_t raw) {  
  char buffer[5];
  //uint32_t temp = (raw*1125ul+320000ul)/1000ul;  // Keeps all 4 decimal places but uses 32-bit math
  uint16_t temp = (9*raw)/8+320;                   // Keeps only 1 decimal place but uses 16-bit math
  
  uint8_t decimalPos = 2;  // default case of a temp between 0 and 99.9
  uint8_t nullPos = 4;     // default case of a temp between 0 and 99.9
  
  if(temp>1000) {  // We're looking at a positive number with three digits
    decimalPos = 3;
    nullPos = 5;
  }
  
  for(int8_t i=nullPos;i>-1;i--) {
    if (i==decimalPos) {
      buffer[i] = '.';
    }
    else if (i==nullPos) {
      buffer[i] = '\0';
    }
    else {
      buffer[i]= temp % 10 + '0';
      temp /= 10;
    }
  } 
  
  simpletx(buffer);
}

void txInt(long inp) {
  long temp = inp;
  uint8_t numChars=0;
  boolean isNegative=false;
  
  // Check to see if there is a negative sign
  if(temp<0){
    isNegative=true;
    numChars++;
    temp*=-1;
  }
  
  do {
    numChars++;
    temp /= 10;
  } while ( temp );
  char buf[numChars];
  
  // Write the negative sign if present and the terminating null character
  temp=inp;
  buf[numChars]=0;
  if(isNegative) {
    temp*=-1;
    buf[0]='-';
  }
  
  int i = numChars - 1;
  do {
      buf[i--] = temp%10 + '0';
      temp /= 10;
  } while (temp);
  
  simpletx(buf);
}

