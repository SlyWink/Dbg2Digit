#define F_CPU 1000000UL

#include "dbg2digit.h"
#include <util/delay.h>


uint8_t __Get_Segments(int8_t p_nibble) {
  switch (p_nibble) {
    case NIBBLE_INI : return (DGT_INI) ;
    case NIBBLE_ERR : return (DGT_ERR) ;
    default : return (eeprom_read_byte(&TB_DIGITS[p_nibble])) ;
  }
}


void _Display_Digit(uint8_t p_digit, int8_t p_nibble) {
  uint8_t l_segments, l_segment, l_portpin, l_pin ;

  l_segments = __Get_Segments(p_nibble) ;
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


void Display_Init(void) {
  _Display_Digit(LEFT,NIBBLE_INI) ;
  _Display_Digit(RIGHT,NIBBLE_INI) ;
}


void Display_Error(void) {
  _Display_Digit(LEFT,NIBBLE_ERR) ;
  _Display_Digit(RIGHT,NIBBLE_ERR) ;
}


void Display_Byte(uint8_t p_byte) {
  _Display_Digit(LEFT,(p_byte & 0xF0) >> 4) ;
  _Display_Digit(RIGHT,p_byte & 0x0F) ;
}


int main(void) {
  uint8_t l_cpt, l_byte ;

  // RXD pin : input with pull-up enabled
  SERIAL_RX_DATA_DIR_REG &= ~_BV(SERIAL_RX_PIN_NUM) ;
  SERIAL_RX_DATA_REG |= _BV(SERIAL_RX_PIN_NUM) ;

  Display_Init() ;
  while(1) {
    l_byte = 0 ;
    _delay_ms(10) ;
    while(SERIAL_RX_INPUT_PIN_ADDR & _BV(SERIAL_RX_PIN_NUM)) ;
    _delay_us(SERIAL_BIT_DELAY_US / 4) ;
    for (l_cpt=0 ; l_cpt<8 ; l_cpt++) {
      _delay_us(SERIAL_BIT_DELAY_US) ;
      if (SERIAL_RX_INPUT_PIN_ADDR & _BV(SERIAL_RX_PIN_NUM)) l_byte |= 1 ;
      l_byte <<= 1 ;
    }
    _delay_us(SERIAL_BIT_DELAY_US) ;
    if (SERIAL_RX_INPUT_PIN_ADDR & _BV(SERIAL_RX_PIN_NUM)) Display_Byte(l_byte) ;
      else Display_Error() ;
  }
}
