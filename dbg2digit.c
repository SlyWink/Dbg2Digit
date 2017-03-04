#define F_CPU 1000000UL

#include "dbg2digit.h"
#include <util/delay.h>


void Display_Digit(uint8_t p_digit, int8_t p_nibble) {
  uint8_t l_segments, l_segment, l_portpin, l_pin ;

  if (p_nibble<0) l_segments = DGT_X ;
    else l_segments = eeprom_read_byte(&TB_DIGITS[p_nibble]) ;
  for (l_segment=0 ; l_segment<7 ; l_segment++) {
    l_portpin = eeprom_read_byte(&TB_PORTS[p_digit][l_segment]) ;
    l_pin = _BV(l_portpin & 0x0F) ;
    switch (l_portpin & 0xF0) {
      case PRT_A :
        if (l_segments & _BV(l_segment)) PORTA &= ~l_pin ; else PORTA |= l_pin ;
        if (p_nibble<0) DDRA |= l_pin ;
        break ;
      case PRT_B :
        if (l_segments & _BV(l_segment)) PORTB &= ~l_pin ; else PORTB |= l_pin ;
        if (p_nibble<0) DDRB |= l_pin ;
        break ;
      case PRT_D :
        if (l_segments & _BV(l_segment)) PORTD &= ~l_pin ; else PORTD |= l_pin ;
        if (p_nibble<0) DDRD |= l_pin ;
        break ;
    }
  }
}


void Init_Display() {
  Display_Digit(LEFT,-1) ;
  Display_Digit(RIGHT,-1) ;
}


void Display_Byte(uint8_t p_byte) {
  Display_Digit(LEFT,(p_byte & 0xF0) >> 4) ;
  Display_Digit(RIGHT,p_byte & 0x0F) ;
}


int main(void) {
  uint8_t cpt ;

  cpt = 0 ;
  while(1) {
    if (!cpt) { Init_Display() ; _delay_ms(5000) ; }
    Display_Byte(cpt++) ; _delay_ms(500) ;
  }
}
