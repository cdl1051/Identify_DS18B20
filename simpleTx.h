#ifndef _SIMPLETX_H
#define _SIMPLETX_H

#include <util/delay.h>

#define myubbr (16000000/16/9600-1)

void simpletx( char * string );
void txByteAsHex(uint8_t inp);
void txRawTempAsFloat(uint16_t raw);
void txInt(long inp);

#endif /* simpleTx.h */
