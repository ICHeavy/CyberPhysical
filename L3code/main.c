
#include <stdint.h>
#include "msp.h"
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Clock.h"
#include "..\inc\Bump.h"
#include "..\inc\Reflectance.h"



volatile uint32_t Time,MainCount;
int Semaphore = 0;
void mapBits(uint8_t x){
    uint8_t y = ~x;
    switch (y) {
        case 0xFE:
            // bit 0 is low (RED)
            LaunchPad_Output(RED);
            break;
        case 0xFD:
            // bit 1 is low (GREEN)
            LaunchPad_Output(GREEN);
            break;
        case 0xFB:
            // bit 2 is low (BLUE)
            LaunchPad_Output(BLUE);
            break;
        case 0xF7:
            // bit 3 is low (RED + GREEN)
            LaunchPad_Output(RED + GREEN);
            break;
        case 0xEF:
            // bit 4 is low (RED + BLUE)
            LaunchPad_Output(RED + BLUE);
            break;
        case 0xDF:
            // bit 5 is low (GREEN + BLUE)
            LaunchPad_Output(GREEN + BLUE);
            break;
        case 0xBF:
            // bit 6 is low (RED + GREEN + BLUE)
            LaunchPad_Output(RED+GREEN+BLUE);
            break;
        case 0x7F:
            // bit 7 is low (all LEDs on)
            LaunchPad_Output(RED+GREEN+BLUE);
            break;
        default:
            // all bits are high (no LEDs on)
            break;
    }
}

uint8_t Data, Position, Bumper;
void SysTick_Handler(void){
  Time = Time + 1;
  if(Time%10==1){ //call this function every tenth time in the SysTick ISR
    Reflectance_Start();
    Semaphore =1;
  }
  if(Time%10==2){
    if (Semaphore ==1){
        Data = Reflectance_End();
        Position = Reflectance_Position(Data);
        Bumper = Bump_Read();
        mapBits(Data);
//        mapBits(Bump_Read());
        Semaphore = 0;
    }
  }
}




int main(void){
  Clock_Init48MHz();
  Time = MainCount = 0;
  SysTick_Init(48000,2); //1kHz
  LaunchPad_Init();
  EnableInterrupts();
  Bump_Init();
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

                       }
                   }

      WaitForInterrupt();
      MainCount++;}
  }
}

