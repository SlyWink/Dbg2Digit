#define F_CPU 1000000L
#define SERIAL_DEBUG_DDR DDRB
#define SERIAL_DEBUG_PORT PORTB
#define SERIAL_DEBUG_PIN PB4

#include <avr/io.h>
#include <util/delay.h>

#include "dbginclude.c"


int main(void) {
  uint8_t l_count ;

  _delay_ms(2000) ;
  l_count = 0 ;
  Serial_Debug_Init() ;

  while (1) {
    Serial_Debug_Send(l_count) ;
    l_count++ ;
    _delay_ms(500) ;
  }
}
