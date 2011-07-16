/* Name: main.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Bootloader core
 * Version: 0.1 tftp / flashing functional
 */


#include "main.h"
#include "net.h"
#include "tftp.h"
#include <avr/pgmspace.h>
//#include "debug.h"

uint16_t lastTimer1;
uint16_t tick = 0;

#define TIMEOUT 3

int main(void) __attribute__ ((naked)) __attribute__ ((section (".init9")));


// Function
void (*app_start)(void) = 0x0000;

void updateLed() {
  uint16_t nextTimer1 = TCNT1;
  if (nextTimer1 & 0x400) PORTB |= _BV(LED_PIN);
  else PORTB &= ~_BV(LED_PIN);
  if (nextTimer1 < lastTimer1) tick++;
  lastTimer1 = nextTimer1;
}

uint8_t timedOut() {
  // Never timeout if there is no code in Flash
  if (pgm_read_word(0x0000) == 0xFFFF) return 0;
  if (tick > TIMEOUT) return 1;
  return 0;
}

int main(void) {
  // cli();
  // SP=RAMEND;  // This is done by hardware reset
  // asm volatile ("clr __zero_reg__");
  
  // Set up outputs to communicate with W5100 chip
  DDRB = _BV(SCK_PIN) | _BV(MOSI_PIN) | _BV(SS_PIN);
  PORTB = _BV(SCK_PIN) | _BV(MISO_PIN) | _BV(MOSI_PIN) | _BV(SS_PIN);

  // Set up Timer 1 as timekeeper for LED flashing
  TCCR1B = 0x05;
  SPSR = (1<<SPI2X);

  // Initialise W5100 chip
  netInit();

  // Attempt tftp download
  tftpInit();
  announceInit();
  for(;;) {
    if (tftpPoll()==0) break;
    announcePoll();
    updateLed();
  }
  
  // Exit to foreground application
  app_start();
  return 0;   /* never reached */
}
