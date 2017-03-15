#define F_CPU 1000000L
#define SERIAL_DEBUG_DDR DDRB
#define SERIAL_DEBUG_PORT PORTB
#define SERIAL_DEBUG_PIN PB4

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "dbginclude.c"


int main(void) {
  uint8_t l_count ;
  int l_rnd ;

  _delay_ms(2000) ;
  l_count = 0 ;
  Serial_Debug_Init() ;

  while (1) {
    Serial_Debug_Send(l_count) ;
    l_count++ ;
    l_rnd = rand()%500 ;
    if (l_rnd < 50) _delay_ms(200) ;
     else if (l_rnd < 150) _delay_ms(300) ;
       else if (l_rnd < 250) _delay_ms(400) ;
         else if (l_rnd < 350) _delay_ms(500) ;
           else if (l_rnd < 450) _delay_ms(600) ;
             else _delay_ms(700) ;
  }
}
