/* Name: debug.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Utility routines for bootloader debugging
 * Version: 0.1 tftp / flashing functional
 */

#include "debug.h"

#ifdef DEBUG
void debugInit() {
  UCSR0A = 0x22;
  UCSR0B = 0x08;
  //UCSR0C = 0x06; // = reset state
  UBRR0 = 16; // 115k2 baud 8N1
  DDRD = 0x92;
}
void trace(char* msg) {
  uint8_t c;
  while ((c = *msg++)) {
    UDR0 = c;
    while (!(UCSR0A & _BV(UDRE0)));
  }
}
void putchar(uint8_t c) {
  UDR0=c;
  while(!(UCSR0A & _BV(UDRE0)));
}
void puthex(uint8_t c) {
  c &= 0xf;
  if (c>9) c+=7;
  UDR0=c+'0';
  while(!(UCSR0A & _BV(UDRE0)));
}
void tracenum(uint16_t num) {
  putchar('0');
  putchar('x');
  puthex(num>>12);
  puthex(num>>8);
  puthex(num>>4);
  puthex(num);
}
#else
void debugInit() {
  ;
}
#endif
