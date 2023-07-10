#include <stdint.h>
#include "msp.h"

/**
 * main.c
 **/


#define RED      0x02
#define YELLOW   0x10



void SW_Init(void){
//    0011 -> 0x03 P4.4 (0x02) P4.1(0x10)
     P5->SEL0 = 0x00;
     P5->SEL1 = 0x00;                        // configure P1.4 and P1.1 as GPIO
     P5->DIR &= ~0x12;    // 2) make P1.4 and P1.1 in
     P5->REN |= 0x12;     // 3) enable pull resistors on P1.4 and P1.1
     P5->OUT |= 0x12;     //    P1.4 and P1.1 are pull-up
}

void LED_Init(void){
//    00010001 -> 0x18 P6.1 (0x02) P6.4(0x10)
    P6->SEL0 = 0x00;
    P6->SEL1 = 0x00;                        // configure P1.4 and P1.1 as GPIO
    P6->DIR |= 0x12;                        // make P1.4 and P1.1 in, P1.0 output
    P6->DS &= ~0x12;
    P6->OUT &= ~0x12;     //    all LEDs off
//   REN and DIR DC ON OUTPUT

}


uint8_t Port4_Input(void){
  return (P5->IN&0x12);                   // read P1.4,P1.1 inputs
}


void Port6_Output(uint8_t data){        // write all of P1.0 outputs
  P6->OUT = (P6->OUT&~ 0x12)|data;        // (0001 0010-> 1110 1101 -> 0xED)
}


void main(uint8_t status )
{
    SW_Init();
    LED_Init();

	while(1){

	    status = Port4_Input();
	    switch(status){
	      case 0x10:                    // SW1 pressed
	          Port6_Output(YELLOW);
	          break;
	      case 0x02:                    // SW2 pressed
	          Port6_Output(RED);
	          break;
	      case 0x12:                    // both pressed
	          Port6_Output(RED+YELLOW);
 	          break;
	      case 0x00:                    // both not pressed
	          Port6_Output(0);
	          break;
	    }
	}
}
