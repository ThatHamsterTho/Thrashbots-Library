/* 
 	* pins.cpp - Library for initializing the pins on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#include "pins.h"
#include <Arduino.h>

void InitControllerPins(){
    noInterrupts();
    pinMode(IRQ_RF, INPUT);
    pinMode(IRQ_MPU, INPUT);
  
    pinMode(SCK, OUTPUT);
    pinMode(MISO, OUTPUT);
    pinMode(MOSI, OUTPUT);
  
    // Inputs
    pinMode(SW_L, INPUT);
    pinMode(SW_R, INPUT);
    
    pinMode(BMPL, INPUT);
    pinMode(BMPR, INPUT);
  
    pinMode(POTL, INPUT);
    pinMode(POTR, INPUT);
  
    pinMode(J_VL, INPUT);
    pinMode(J_HL, INPUT);
    pinMode(J_VR, INPUT);
    pinMode(J_HR, INPUT);
  
    // Component Control
    pinMode(REGS, OUTPUT);
    pinMode(NRST, OUTPUT);
    pinMode(CS_A, OUTPUT);
    pinMode(CS_B, OUTPUT);
  
    pinMode(BUZZ, OUTPUT);
    interrupts();
}
