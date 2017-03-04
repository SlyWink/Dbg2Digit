#include <avr/eeprom.h>

#define SEG_A _BV(0)
#define SEG_B _BV(1) //    a
#define SEG_C _BV(2) //  +---+
#define SEG_D _BV(3) // f| g |b
#define SEG_E _BV(4) //  +---+
#define SEG_F _BV(5) // e| d |c
#define SEG_G _BV(6) //  +---+.
#define SEG_P _BV(7) //    d  p

#define DGT_0 (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)
#define DGT_1 (SEG_B|SEG_C)
#define DGT_2 (SEG_A|SEG_B|SEG_D|SEG_E|SEG_G)
#define DGT_3 (SEG_A|SEG_B|SEG_C|SEG_D|SEG_G)
#define DGT_4 (SEG_B|SEG_C|SEG_F|SEG_G)
#define DGT_5 (SEG_A|SEG_C|SEG_D|SEG_F|SEG_G)
#define DGT_6 (SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DGT_7 (SEG_A|SEG_B|SEG_C)
#define DGT_8 (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DGT_9 (SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G)
#define DGT_A (SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G)
#define DGT_B (SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DGT_C (SEG_A|SEG_D|SEG_E|SEG_F)
#define DGT_D (SEG_B|SEG_C|SEG_D|SEG_E|SEG_G)
#define DGT_E (SEG_A|SEG_D|SEG_E|SEG_F|SEG_G)
#define DGT_F (SEG_A|SEG_E|SEG_F|SEG_G)
#define DGT_X (SEG_G)

const uint8_t EEMEM TB_DIGITS[16] = {
  DGT_0, DGT_1, DGT_2, DGT_3, DGT_4, DGT_5, DGT_6, DGT_7,
  DGT_8, DGT_9, DGT_A, DGT_B, DGT_C, DGT_D, DGT_E, DGT_F
} ;

#define PRT_A (1<<4)
#define PRT_B (1<<5)
#define PRT_D (1<<6)

// 2 matrices de 7 segments : a1 - g1 et a2 - g2
// pnnnn avec nnnn = n°de port, p = 0 si port B ou p = 1 si port D
const uint8_t EEMEM TB_PORTS[2][7] = {
// { (PRT_D|1), (PRT_B|4), (PRT_B|5), (PRT_B|6), (PRT_B|7), (PRT_D|3), (PRT_D|0) },
 { (PRT_D|PD3), (PRT_B|PB4), (PRT_B|PB5), (PRT_B|PB6), (PRT_B|PB7), (PRT_D|PD1), (PRT_A|PA1) },
 { (PRT_D|PD4), (PRT_D|PD5), (PRT_B|PB0), (PRT_B|PB2), (PRT_B|PB3), (PRT_D|PD6), (PRT_B|PB1) }
} ;

#define LEFT 0
#define RIGHT 1

#define FALSE 0
#define TRUE 1
