/* Name: bigapp.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Test big app for bootloader
 * Version: 0.1 tftp / flashing functional
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

prog_char dummy[28000] PROGMEM = {};

int main(void) {
  uint8_t a,b,c;
  DDRB = _BV(5);
  for(;;) {
    for (a=0; a<255; a++) {
      for (c=0; c<15; c++) {
        for (b=0; b<255; b++) {
          if (a<b) PORTB |= _BV(5);
          else PORTB &= ~_BV(5);
        }
      }
    }
    for (a=0; a<255; a++) {
      for (c=0; c<15; c++) {
        for (b=0; b<255; b++) {
          if (a>b) PORTB |= _BV(5);
          else PORTB &= ~_BV(5);
        }
      }
    }
  }
  return 0;   /* never reached */
}
