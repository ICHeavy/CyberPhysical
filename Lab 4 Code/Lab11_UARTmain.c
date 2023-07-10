// Lab11_UARTmain.c
// Runs on MSP432
// Test the functions in UART0.c by printing various
// Busy-wait device driver for the UART UCA0.
// Daniel Valvano
// May 24, 2019

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

#include "msp.h"
#include <stdio.h>
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Clock.h"
#include "..\inc\Bump.h"
#include "..\inc\UART0.h"

#define BLUEOUT  (*((volatile uint8_t *)(0x42098068)))
#define GREENOUT (*((volatile uint8_t *)(0x42098064)))
#define REDLED (*((volatile uint8_t *)(0x42098040)))
#define SW2IN ((*((volatile uint8_t *)(0x42098010)))^1)
#define SW1IN ((*((volatile uint8_t *)(0x42098004)))^1)

void mapBits(uint8_t x){
    switch (x) {
        case 0xFE:
            // bit 0 is low (RED)
            BLUEOUT^=1;
            UART0_OutUHex(0xB0);
            break;
        case 0xFD:
            // bit 1 is low (GREEN)
            LaunchPad_Output(GREEN);
            UART0_OutUHex(0xB1);
            break;
        case 0xFB:
            // bit 2 is low (BLUE)
            LaunchPad_Output(BLUE);
            UART0_OutUDec4(2343432205);
            UART0_OutUHex(0xB2);
            break;
        case 0xF7:
            // bit 3 is low (RED + GREEN)
            LaunchPad_Output(RED + GREEN);
            UART0_OutUHex(0xB3);
            break;
        case 0xEF:
            // bit 4 is low (RED + BLUE)
            LaunchPad_Output(RED + BLUE);
            UART0_OutUHex(0xB4);
            break;
        case 0xDF:
            // bit 5 is low (GREEN + BLUE)
            LaunchPad_Output(GREEN + BLUE);
            UART0_OutUHex(0xB5);
            break;
        case 0xBF:
            // bit 6 is low (RED + GREEN + BLUE)
            LaunchPad_Output(RED+GREEN+BLUE);
            UART0_OutUHex(0xB6);
            break;
        case 0x7F:
            // bit 7 is low (all LEDs on)
            LaunchPad_Output(RED+GREEN+BLUE);

            UART0_OutUHex(0xB7);
            break;
        default:
            // all bits are high (no LEDs on)
            break;
    }
}
volatile uint32_t Time,MainCount;
uint8_t Data, Position, Bumper;
int Semaphore = 0;
void SysTick_Handler(void){
    Time = Time + 1; // every 1ms
    if(Time%10==1){
        Bumper = Bump_Read();
        Semaphore =1;
      }
      if(Time%10==2){
        if (Semaphore ==1){

            mapBits(Bumper);
            Semaphore = 0;
        }
      }
  }


/**
 * main.c
 */
void main(void){
  Clock_Init48MHz();
  Time = MainCount = 0;
  SysTick_Init(600,2);  // set up SysTick for 8 Hz interrupts
  LaunchPad_Init();       // P1.0 is red LED on LaunchPad
  UART0_Init();
  Bump_Init();
  EnableInterrupts(); //cortexM lib// set system clock to 48 MHz

  while(1){
    Clock_Delay1ms(1000);
    WaitForInterrupt();
    MainCount++;
  }
}
