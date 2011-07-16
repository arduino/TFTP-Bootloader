#include <avr/io.h>
#include <avr/eeprom.h>

#define SCK_PIN 5
#define MISO_PIN 4
#define MOSI_PIN 3
#define SS_PIN 2

#define EEPROM_SIG_1 ((uint8_t*)0)
#define EEPROM_SIG_2 ((uint8_t*)1)
#define EEPROM_DATA ((uint8_t*)2)
#define EEPROM_SIG_1_VALUE (0x55)
#define EEPROM_SIG_2_VALUE (0xAA)

void netWriteReg(uint16_t address, uint8_t value);
uint8_t netReadReg(uint16_t address);
uint16_t netReadWord(uint16_t address);
void netWriteWord(uint16_t address, uint16_t value);

void netInit();

#define SS_LOW() PORTB &= ~_BV(SS_PIN)
#define SS_HIGH() PORTB |= _BV(SS_PIN)

#define SPI_WRITE (0xF0)
#define SPI_READ (0x0F)
