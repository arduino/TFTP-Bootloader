#include <avr/pgmspace.h>
#include <avr/io.h>

//#define DEBUG

#ifdef DEBUG
#define TRACE(msg) trace(msg)
void trace(char* msg);
void puthex(uint8_t c);
void tracenum(uint16_t num);
#else
#define TRACE(msg)
#endif
void debugInit();
