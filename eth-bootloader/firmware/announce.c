/* Name: announce.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: announcement (Network broadcast)
 * Version: 0.1 tftp / flashing functional
 */

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "net.h"
#include "w5100_reg.h"
#include "tftp.h"
#include "main.h"
#include "validate.h"
#include "debug.h"

uint16_t readPointer;
 
#define ANNOUNCE_PORT (5555)
#define ANNOUNCE_DATA_SIZE    128

char *hex="0123456789ABCDEF";

char bootloaderIdentString[] PROGMEM = "Ethernet 1.0";
#define bootloaderIdentStringLength (12)

uint8_t readNextByte() {
  uint8_t returnValue;
  returnValue = readPointer++;
  if (readPointer == S2_RX_END) readPointer = S2_RX_START;
  return returnValue;
}

void announceReply() {
  uint8_t txBuffer[100];
  uint8_t *txPtr;
  uint8_t packetLength = 0;
  uint16_t writePointer;
  uint8_t value;
  uint8_t i;
  
  writePointer = netReadWord(REG_S2_TX_WR0) + S2_TX_START;  
  txPtr = txBuffer + 2;

  // Send IP address in hex
  //
  for (i=REG_SIPR0; i<=REG_SIPR3; i++) {
    value = netReadReg(i);
    *txPtr++ = hex[value>>4];
    *txPtr++ = hex[value & 0xf];
  }
  //
  // Tag on ident string
  //
  memcpy_P(txPtr,bootloaderIdentString,bootloaderIdentStringLength);
  packetLength = 8 + bootloaderIdentStringLength;
  
  // Write packet length at start of packet
  txBuffer[0] = 0;
  txBuffer[1] = packetLength;
  packetLength += 2;
  txPtr = txBuffer;
  while (packetLength--) {
    netWriteReg(writePointer++, *txPtr++);
    if (writePointer == S2_TX_END) writePointer = S2_TX_START;
  }
  netWriteWord(REG_S2_TX_WR0, writePointer - S2_TX_START);
  netWriteReg(REG_S2_CR,CR_SEND);
  while (netReadReg(REG_S2_CR));
}

void announcePacket(uint16_t packetSize) {
  uint8_t buffer[ANNOUNCE_DATA_SIZE];
  uint16_t packetLength;
  
  // Transfer entire packet to RAM
  uint8_t* bufPtr = buffer;
  uint16_t count;

  readPointer = netReadWord(REG_S2_RX_RD0) + S2_RX_START;

  // Read destination IP address
  for (count = 0; count<4; count++) {
    netWriteReg(REG_S2_DIPR0+count,readNextByte());
  }

  // Read destination port - but ignore it and respond on 5555 anyway.
  readNextByte();
  readNextByte();
  netWriteWord(REG_S2_DPORT0, ANNOUNCE_PORT);

  // Read packet length
  packetLength = readNextByte() | (readNextByte()<<8);

  // Trim overlong packets
  if (packetLength > ANNOUNCE_DATA_SIZE) packetLength = ANNOUNCE_DATA_SIZE;
  for (count=packetLength; --count;) {
    *bufPtr++ = readNextByte();
  }
  netWriteWord(REG_S2_RX_RD0,readPointer - S2_RX_START); // Write back new pointer
  netWriteWord(REG_S2_CR, CR_RECV); // Receive again
  
  // Dump packet
  bufPtr = buffer;
  
  // Parse packet
  if (memcmp(buffer,"arduino",7)==0) announceReply();
}

void announceInit() {
  // Open socket
  do {
    netWriteWord(REG_S2_PORT0,ANNOUNCE_PORT);
    netWriteReg(REG_S2_MR,MR_UDP);
    netWriteReg(REG_S2_CR,CR_OPEN);
    if (netReadReg(REG_S2_SR) != SOCK_UDP)
      netWriteReg(REG_S2_CR,CR_CLOSE);
  } while (netReadReg(REG_S2_SR) != SOCK_UDP);
}

void announcePoll() {
  uint16_t packetSize = netReadWord(REG_S2_RX_RSR0);
  if (packetSize) {
    announcePacket(packetSize);
    netWriteReg(REG_S2_CR,CR_CLOSE);
    announceInit();
  }
}
