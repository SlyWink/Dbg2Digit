#define F_CPU 1000000UL
#define BAUD 9600

#include "dbg2digit.h"
#include <util/delay.h>
#include <util/setbaud.h>


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



void Init_Usart(void) {
  // RXD pin : input with pull-up enabled
  USART_DDR &= ~_BV(USART_RXD) ; // Input
  USART_PORT |= _BV(USART_RXD) ; // Pull-up
  // Set baut rate
  UBRRH = UBRRH_VALUE ;
  UBRRL = UBRRL_VALUE ;
  // 8 bits, no parity, 1 stop bit
  UCSRC = _BV(UCSZ1)|_BV(UCSZ0) ;
#if USE_2X
  UCSRA |= _BV(U2X);
#else
  UCSRA &= ~_BV(U2X);
#endif
  // Receive only
  UCSRB = _BV(RXEN) ;
}


#define Byte_Received() (UCSRA & _BV(RXC))

#define Error_Status() (UCSRA & (_BV(FE)|_BV(DOR)))


int main(void) {
  uint8_t l_error, l_byte ;
  int8_t l_delay ;

  Init_Usart() ;
  l_delay = 0 ;
  while(1) {
    while(!Byte_Received()) {
      // Display init after 5 seconds
      if (!l_delay) Display_Init() ;
        else if (l_delay > 0) l_delay-- ;
      _delay_ms(WAIT_UNIT) ;
    }
    l_error = Error_Status() ;
    l_byte = UDR ;
    if (l_error) Display_Error() ; else Display_Byte(l_byte);
    l_delay = WAIT_5SEC ;
  }
}
