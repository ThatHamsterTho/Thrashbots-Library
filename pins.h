#pragma once
#include <Arduino.h>

// IRQ
//---------------------------//

#define IRQ_RF  A5
#define IRQ_MPU A4

// SPI
//---------------------------//

#define SCK  13
#define SDO  12
#define SDI  11

// switches
//* 10TC605
//---------------------------//

#define SW_L  2
#define SW_R  3

// bumpers
//* B3F-3150
//---------------------------//

#define BMPL  4
#define BMPR  5

// pots
//* PTV09A-4215F-B103
//---------------------------//

#define POTL A7
#define POTR A6

// joysticks
//* JoystickPTH
//---------------------------//

#define J_VL A3
#define J_HL A2
#define J_HR A1
#define J_VR A0

// LCD
//* NHD-C12832A1Z-FSW-FBW-3V3
//---------------------------//

// REGS: used to select instruction or data for LCD. A0 pin in datasheet
#define REGS  7	
// NRST: !RST pin on LCD	
#define NRST  8		

// Chip Select Demux
//* CD4052BM
// used to select either MPU_NCS, RF_NCS, LCD_NCS
//---------------------------//

#define CS_A 10		
#define CS_B  9

// RF
//* nRF24L01-PA-Ext
//---------------------------//

// RF Chip Enable pin
#define RFCE REGS	

// Buzzer
//* BUZZER-pTH
//---------------------------//

#define BUZZ  6

void InitControllerPins();
