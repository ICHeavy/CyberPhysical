
#include <stdint.h>
#include "msp.h"
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Clock.h"



volatile uint32_t Time,MainCount;
#define BLUEOUT  (*((volatile uint8_t *)(0x42098068)))
#define GREENOUT (*((volatile uint8_t *)(0x42098064)))
#define REDLED (*((volatile uint8_t *)(0x42098040)))
#define SW2IN ((*((volatile uint8_t *)(0x42098010)))^1)
#define SW1IN ((*((volatile uint8_t *)(0x42098004)))^1)

//void SysTick_Handler(void){
//  LEDOUT ^= 0x01;       // toggle P1.0
//  LEDOUT ^= 0x01;       // toggle P1.0
//  Time = Time + 1;
//  LEDOUT ^= 0x01;       // toggle P1.0
//}

uint32_t getCurrentSystickValue(void){
    uint32_t systick_ctrl = SysTick->CTRL;
    uint32_t systick_load = SysTick->LOAD;
    uint32_t systick_val = SysTick->VAL;

    // Calculate the current value of the systick timer
    uint32_t current_value = systick_load - systick_val;
    // Check if the systick timer has wrapped around
    if (systick_ctrl & SysTick->CTRL&0x00000007)
    {
        // If so, add the reload value to the current value
        current_value += systick_load;
    }

    return current_value;
}

int main(void){
  Clock_Init48MHz();      // running on crystal
  Time = MainCount = 0;
  SysTick_Init(6000000,2);  // set up SysTick for 8 Hz interrupts
  LaunchPad_Init();       // P1.0 is red LED on LaunchPad
  EnableInterrupts(); //cortexM lib
  while(1){
      uint32_t i, j, temp, length;
              uint32_t a[100]={5000,5308,5614,5918,6219,6514,
              6804,7086,7361,7626,7880,8123,8354,8572,8776,8964,9137,
              9294,9434,9556,9660,9746,9813,9861,9890,9900,9890,9861,
              9813,9746,9660,9556,9434,9294,9137,8964,8776,8572,8354,
              8123,7880,7626, 7361,7086,6804,6514,6219,5918,5614,
              5308,5000,4692,4386,4082,3781,3486,3196,2914,2639,2374,
              2120,1877,1646,1428,1224,1036,863,706,566,444,340,254,
              187,139,110,100,110,139,187,254,340,444,566,706,863,
              1036, 1224, 1428, 1646,1877,2120,2374,2639,2914,
              3196,3486,3781,4082,4386,4692};
              length = 100;
               for (i = 0; i < length; i++)
               {
                   for (j = 0; j < length - i - 1; j++)
                   {
                       if (a[j + 1] < a[j])
                       {
                           temp = a[j];
                           a[j] = a[j + 1];
                           a[j + 1] = temp;
                           printf( "%d\n", i );
                           WaitForInterrupt();
                           MainCount++;
                       }
                   }

      }
  }
}

uint32_t High=6000000,Low=750000;
void SysTick_Handler(void){ // PWM
//    uint32_t current_value = getCurrentSystickValue();
    if(!(SW1IN||SW2IN)){
        BLUEOUT = 0;
        REDLED = 0;
    }
   if ((SW1IN^SW2IN)&&!(SW1IN&&SW2IN)){
        if(SW1IN){ // Color=RED if SW1 is pressed
            REDLED ^= 1;
            BLUEOUT = 0;
            SysTick->LOAD = High-1; // time while high
        }else{
            REDLED = 0;
        }

        if(SW2IN){ // Color=BLUE if SW2 is pressed
            BLUEOUT ^= 1;
            REDLED = 0;
            SysTick->LOAD = Low-1;   // time while low

        }else{
            BLUEOUT = 0;
        }

    }
   else{
        if((SW1IN&&SW2IN)&&!(SW1IN^SW2IN)){
            BLUEOUT ^= 1;
            SysTick->LOAD = Low-1; // time while high
            if (Time%8 == 0){
                REDLED ^= 1;
            }else{
                REDLED = 0;
            }

        }else{
            BLUEOUT = 0;
        }
    }






//
//  if(SW1IN && SW2IN){  // if high
//      BLUEOUT ^= 1;
//      SysTick->LOAD = High-1; // time while high
//      if(Time%8==0){
//          REDLED ^= 1;
//      }
//  }else{
//      BLUEOUT = 0;
//      REDLED = 0;
//  }

  SysTick->VAL = 0;        // any value written to counter clears
  Time = Time + 1;
}







//
//
//
//void (*TimerA2Task) (void) // ptr to f(x) named TimerA2Task that has both void fields
//void TimerA2Task(void (*task) (void)), uint16_t T){
//    TimerA2Task = task; // goes to specified handler
//    // b[9-10]= 10, SMCLK
//    // b[7-6] = 10, /4
//    // b[5-4] = 00, stop mode
//    // b[2] = 0, clear
//    // b[1] = 0, no int on timer
//    // 0bxx10 1000 x00x -> 0b0010 1000 0000 -> 0x280
//    TIMER_A2 ->CTL = 0x280; // set control reg for timer, SLIDE 13
//
//    // B[15-14] = 00, NO CAPTURE
//    // B[8]= 0, NO CAPTURE
//    // B[4]= 1, COMPARE
//    // B[2]= 0, EN CAPT/COMP INT ON CCIFG
//    // B[0]= 0, CLR CAPT/COMP INT PENDING
//    // 0b xxx0 xxx1 0xx0 -> 0b 0000 0001 0000 -> 0x010
//    TIMER_A2->CCTL[0] = 0x0010;  // SETS CTRL REG FOR CAPTURE/COMPARE REG
//    TIMER_A2->CCR[0] = (T-1);    //COMPARE TO CORRESPONDING PERIOD
//    TIMER_A2->EX0 = 0x0005;      // CONFIG, /6 , /24 TOTAL, 2kHZ
//    NVIC->IP[3] = (NVIC->IP[3] & 0xFFFFFF00) | 0x00000040; // CLEAR LOWEST BYTE IN INT PRIORITY ARR, SET TO 0x40
//    NVIC->ISER[0] = 0x00001000;  // EN INT12 IN NVIC, IRQ
//    TIMER_A2->CTL |= 0x0014;     // RST AND STRT TIMER_A IN UP_MODE
//
//    // TODO
//    // SET UP OTHER COUNTER
//
//
//}
//    // TODO
//// MAKE BOTH HANDLERS FOR TURNING LEDS ON
//// INIT SWITCHES
//// IN MAIN
//// CALL TIMER TASK WITH POINTER TO LED ON AND FREQ
