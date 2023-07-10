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
#include "../inc/Clock.h"
#include "../inc/UART0.h"
#include "../inc/LaunchPad.h"

#define BLUEOUT  (*((volatile uint8_t *)(0x42098068)))
#define GREENOUT (*((volatile uint8_t *)(0x42098064)))
#define REDLED (*((volatile uint8_t *)(0x42098040)))
#define SW2IN ((*((volatile uint8_t *)(0x42098010)))^1)
#define SW1IN ((*((volatile uint8_t *)(0x42098004)))^1)
//int Semaphore = 0;



void mapBits(char* x){
    char ch = x[0];
    int N = (int)(ch)+0;
    switch (N) {
    case 0x64: // dark
    LaunchPad_Output(0);
        break;
    case 0x72: // “red”:
        // bit 1 is low (GREEN)
        LaunchPad_Output(RED);
        break;
    case 0x62: //“blue”:
        // bit 2 is low (BLUE)
        LaunchPad_Output(BLUE);
        break;
    case 0x67: //“green”:
        // bit 3 is low (RED + GREEN)
        LaunchPad_Output(GREEN);
        break;
    case 0x79: //“yellow”:
        // bit 4 is low (RED + BLUE)
        LaunchPad_Output(RED + GREEN);
        break;
    case 0x73: //“sky blue”:
        // bit 5 is low (GREEN + BLUE)
        LaunchPad_Output(GREEN + BLUE);
        break;
    case 0x77: //“white”:
        // bit 6 is low (RED + GREEN + BLUE)
        LaunchPad_Output(RED+GREEN+BLUE);
        break;
    case 0x70: //“pink”:
        // bit 7 is low (all LEDs on)
        LaunchPad_Output(RED+BLUE);
        break;
    default:
        // all bits are high (no LEDs on)
        break;
    }
}



//uint8_t Data, Position, Bumper;
//void SysTick_Handler(void){ // every 1ms
//
//  Time = Time + 1;
//  if(Time%10==1){
//    Reflectance_Start(); // call this function every tenth time
//    Semaphore =1;
//  }
//  if(Time%10==2){
//    if (Semaphore ==1){
//        Data = Reflectance_End();
//        Position = Reflectance_Position(Data);
//        Bumper = Bump_Read();
//        mapBits(Data);
//        Semaphore = 0;
//    }
//  }
//}

/**
 * main.c
 */


void main(void){
//    uint32_t data=0;
    Clock_Init48MHz();                   // set system clock to 48 MHz
    LaunchPad_Init();       // P1.0 is red LED on LaunchPad
    UART0_Init();
//    uint8_t newMsg = 0;
    uint16_t max = 9;
    char command[10] = "000000000";
    UART0_OutString("--- RSLK 1.1 --------\n\r");
    UART0_OutString("ready to type: \n\r");

  while(1){
    UART0_InString(command, max);
    mapBits(command);
    Clock_Delay1ms(1000);
  }
}



