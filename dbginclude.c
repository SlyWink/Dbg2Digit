#include <avr/io.h>
#include <util/delay.h>

#define SERIAL_BIT_DELAY_US (1000000L / 1200)

#ifndef SERIAL_DEBUG_DDR
# warning "SERIAL_DEBUG_DDR not defined -> choosing DDRB"
# define SERIAL_DEBUG_DDR DDRB
#endif

#ifndef SERIAL_DEBUG_PORT
# warning "SERIAL_DEBUG_PORT not defined -> choosing PORTB"
# define SERIAL_DEBUG_PORT PORTB
#endif

#ifndef SERIAL_DEBUG_PIN
# warning "SERIAL_DEBUG_PIN not defined -> choosing PB0"
# define SERIAL_DEBUG_PIN PB0
#endif


void Serial_Debug_Init(void) {
  // Output
  SERIAL_DEBUG_DDR |= _BV(SERIAL_DEBUG_PIN) ;
  // High level
  SERIAL_DEBUG_PORT |= _BV(SERIAL_DEBUG_PIN) ;
}

void Serial_Debug_Send(uint8_t l_byte) {
  uint8_t l_count ;

  // Start bit : low level
  SERIAL_DEBUG_PORT  &= ~_BV(SERIAL_DEBUG_PIN) ;
  _delay_us(SERIAL_BIT_DELAY_US) ;
  // LSB to MSB
  for (l_count=0 ; l_count<8 ; l_count++) {
    if (l_byte & 1) SERIAL_DEBUG_PORT |=_BV(SERIAL_DEBUG_PIN) ;
      else SERIAL_DEBUG_PORT &= ~_BV(SERIAL_DEBUG_PIN) ;
    l_byte >>= 1 ;
    _delay_us(SERIAL_BIT_DELAY_US) ;
  }
  // Stop bit : high level
  SERIAL_DEBUG_PORT |= _BV(SERIAL_DEBUG_PIN) ;
  _delay_us(SERIAL_BIT_DELAY_US) ;
}
