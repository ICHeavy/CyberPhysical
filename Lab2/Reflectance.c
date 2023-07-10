
// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"

// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
void Reflectance_Init(void){

    // Initialize Port 5 0000 1000
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;   // configure 5.3 as GPIO
    P5->DIR |= 0x08;     // make P5.3 out
    P5->OUT &= ~0x08;


//    // Initialize Port 9 0000 0100
//    P9->SEL0 &= ~0x04;
//    P9->SEL1 &= ~0x04;   // configure P9.2 as GPIO
//    P9->DIR |= 0x04;     // make P9.2 out
//    P9->OUT &= ~0x04;

    // Initialize Port 7 1111 1111
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;   // configure P7 as GPIO
    P7->DIR &= ~0xFF;    // p7 in

}




uint8_t Reflectance_Read(uint32_t time){
uint8_t result;
    // write this as part of Lab 6

    P5->OUT |= 0x08;        // turn on even IR LEDs
    P9->OUT |= 0x04;        // turn on odd IR LEDs

    P7->DIR = 0xFF;         // change to output
    P7->OUT = 0xFF;         // set high to charge caps


    Clock_Delay1us(10);     // wait 10 us
    P7->DIR = 0x00;         // make P7.7-P7.0 inputs
    Clock_Delay1us(time);   // wait time us
    result = P7->IN;   // read binary line voltages

    P5->OUT &= ~0x08;       // turn off 4 even IR LEDs
    P9->OUT &= ~0x04;       // turn off odd IR LEDs


  return result;
}


// Input: time to wait in usec
// Output: 0 (off road), 1 off to left, 2 off to right, 3 on road
// (Left,Right) Sensors
// 1,1          both sensors   on line
// 0,1          just right     off to left
// 1,0          left left      off to right
// 0,0          neither        lost
// Assumes: Reflectance_Init() has been called
uint8_t Reflectance_Center(uint32_t time){

  return 0; // replace this line
}


// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line
int32_t Reflectance_Position(uint8_t data){
    int32_t wi[8]= {-33400,-23800,-14300,-4800,4800,14300,23800,33400};
        int32_t bi[8];
        int32_t di[8];
        int32_t d;

        bi[0]= data & 0x01;
        bi[1]= data & 0x02;
        bi[2]= data & 0x04;
        bi[3]= data & 0x08;
        bi[4]= data & 0x10;
        bi[5]= data & 0x20;
        bi[6]= data & 0x40;
        bi[7]= data & 0x80;
        int i;
        for(i=0; i<=7; i++){
            di[i] = (bi[i]*wi[i]) / (bi[i]);
        }

        d = di[0]+di[1]+di[2]+di[3]+di[4]+di[5]+di[6]+di[7];


     return d; // replace this line
     }


// ------------Reflectance_Start------------
// Begin the process of reading the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// Input: none
// Output: none
// Assumes: Reflectance_Init() has been called
 void Reflectance_Start(void){

     P5->OUT |= 0x08;      // turn on even IR LEDs
     P9->OUT |= 0x04;      // turn on odd IR LEDs

     P7->DIR = 0xFF;       // make P7.7-P7.0 out
     P7->OUT = 0xFF;       // make P7.7-P7.0 in
     Clock_Delay1us(10);
     P7->DIR = 0x00;       // make P7.7-P7.0 in

 }



// ------------Reflectance_End------------
// Finish reading the eight sensors
// Read sensors
// Turn off the 8 IR LEDs
// Input: none
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
// Assumes: Reflectance_Start() was called 1 ms ago
uint8_t Reflectance_End(void){
//    Clock_Delay1us(800); // wait time in us
    uint8_t x = P7->IN;      // 0 trigger

    P5->OUT &= ~0x08;
    P9->OUT &= ~0x04;
    return x;
}
