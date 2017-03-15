#define F_CPU 1000000UL
#define BAUD 9600

#include "dbg2digit.h"
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


uint8_t __Get_Segments(int8_t p_nibble) {
  switch (p_nibble) {
    case NIBBLE_CLR : return (DGT_CLR) ;
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


void Clear_Display(void) {
  _Display_Digit(LEFT,NIBBLE_CLR) ;
  _Display_Digit(RIGHT,NIBBLE_CLR) ;
}


void Display_Error(void) {
  _Display_Digit(LEFT,NIBBLE_ERR) ;
  _Display_Digit(RIGHT,NIBBLE_ERR) ;
}


void Display_Byte(uint8_t p_byte) {
  _Display_Digit(LEFT,(p_byte & 0xF0) >> 4) ;
  _Display_Digit(RIGHT,p_byte & 0x0F) ;
}


ISR(USART_RX_vect) {
  g_rx_error = (UCSRA & (_BV(FE)|_BV(DOR))) ;
  g_rx_byte = UDR ;
  g_event |= EVT_SERIALRX ;
}


ISR(INT0_vect) {
  g_event |= EVT_STARTBIT ;
}


EMPTY_INTERRUPT(WDT_OVERFLOW_vect) ;


void Init_Serial(void) {
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
  // Receive only + interrupt driven
  UCSRB = _BV(RXEN)|_BV(RXCIE) ;
  // INT0 pin connected to RXD pin to wake up from sleep mode
  MCUCR |= _BV(ISC01) ; // Falling edge = start bit
  GIMSK |= _BV(INT0) ; 
}


void Set_Other_Pins (void) {
  // Input
  DDRA &= ~(_BV(PA0)|_BV(PA2)) ;
  DDRD &= ~_BV(PD2) ; // Used (INT0) to detect start bit
  // Pull-Up enabled
  PORTA |= _BV(PA0)|_BV(PA2) ;
  PORTD |= _BV(PD2) ;
}


void Init_Watchdog(void) {
  WDTCSR = _BV(WDP1)|_BV(WDP0) ;
}


void Sleep_125ms(int8_t p_tics) {
  WDTCSR |= _BV(WDIE) ;
  while(!(g_event & EVT_STARTBIT)) {
    sleep_mode() ;
    if (p_tics > 0 && !--p_tics) break ;
  }
  WDTCSR &= ~_BV(WDIE) ;
}


void Start_Display(void) {
  uint8_t l_cpt ;
  for (l_cpt=0 ; l_cpt<3 ; l_cpt++) {
    Display_Byte(0x88) ; Sleep_125ms(3) ;
    Clear_Display() ; Sleep_125ms(1) ;
  }
}


int main(void) {
  Init_Serial() ;
  Init_Watchdog() ;
  Set_Other_Pins() ;
  set_sleep_mode(SLEEP_MODE_IDLE) ;
  sei() ;
  Start_Display() ;
  while(1) {
    Sleep_125ms(-1) ;
    while (!(g_event & EVT_SERIALRX)) ;
    g_event = 0 ;
    if (g_rx_error) Display_Error() ; else Display_Byte(g_rx_byte);
    Sleep_125ms(40) ;
    Clear_Display() ;
  }
}
