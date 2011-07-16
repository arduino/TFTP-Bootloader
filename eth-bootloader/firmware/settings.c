/* Name: settings.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Alter network EEPROM settings
 * Version: 0.1 tftp / flashing functional
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include "neteeprom.h"

/* Alter these to your network setting */
uint8_t ip_address[4]     = {192, 168, 1, 250};
uint8_t gateway_address[4]= {192, 168, 1, 1};
uint8_t subnet_mask[4]    = {255, 255, 255, 0};

uint8_t getRandom() {
  uint8_t result = 0;
  uint8_t i;
  for (i=8; i--;) {
    uint8_t bit = 0;
    uint8_t dummy;
    uint16_t j;
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
    ADCSRB = 0;
    for (j=1000; i--;) {
      ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1);
      while(ADCSRA & _BV(ADSC));
      bit ^= ADCL;
      dummy = ADCH; // ADC result is locked until ADCH is read
    }
    bit &= 1;
    result += result + bit;
  }
  return result;
}

int main(void) {
  uint8_t mac[6];
  uint8_t i;
  
  // Deliberately corrupt network settings
  eeprom_write_byte(EEPROM_SIG_2,EEPROM_SIG_1_VALUE);
  
  // Write gateway
  for (i=0; i<4; i++) eeprom_write_byte(EEPROM_GATEWAY+i,gateway_address[i]);

  // Write a random MAC address
  for (i=0; i<6; i++) eeprom_write_byte(EEPROM_MAC+i,getRandom());
  
  // Write IP address
  for (i=0; i<4; i++) eeprom_write_byte(EEPROM_IP+i,ip_address[i]);
  
  // Fix signature bytes
  eeprom_write_byte(EEPROM_SIG_1,EEPROM_SIG_1_VALUE);
  eeprom_write_byte(EEPROM_SIG_2,EEPROM_SIG_2_VALUE);
  
  // Stop
  while(1);
}
