/* 
 	* pins.h - Header containing all pin definitions of the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#pragma once
#include <Arduino.h>

/** @defgroup PINDEFS Pin definitions
 * This module contains all the pin definitions. Can be used to track hardware errors.
 * @{
 */
// IRQ
//---------------------------//

/** Interrupt pin of the nRF24 module. */
#define IRQ_RF  A5 
/** Interrupt pin of the MPU module. */
#define IRQ_MPU A4 

// SPI
//---------------------------//

/** SPI Serial Clock line. */
#define SCK  13 
/** SPI MISO line. */
#define MISO 12	
/** SPI MOSI line. */
#define MOSI 11	

// switches
//* 10TC605
//---------------------------//

/** Left  switch pin. */
#define SW_L  2		
/** Right switch pin. */
#define SW_R  3		

// bumpers
//* B3F-3150
//---------------------------//

/** Left  bumper pin. */
#define BMPL  4		
/** Right bumper pin. */
#define BMPR  5		

// pots
//* PTV09A-4215F-B103
//---------------------------//

/** Left  potentiometer analog pin. */
#define POTL A7		
/** Right potentiometer analog pin. */
#define POTR A6		

// joysticks
//* JoystickPTH
//---------------------------//

/** Left  joystick vertical   analog pin. */
#define J_VL A3		
/** Left  joystick horizontal analog pin. */
#define J_HL A2		
/** Right joystick horizontal analog pin. */
#define J_HR A1		
/** Left  joystick vertical   analog pin. */
#define J_VR A0		

// LCD
//* NHD-C12832A1Z-FSW-FBW-3V3
//---------------------------//

/** REGS: used to select instruction or data for LCD. A0 pin in datasheet. */
#define REGS  7

/** Pin for the Not reset pin on the LCD. */	
#define NRST  8		

// Chip Select Demux
//* CD4052BM
// used to select either MPU_NCS, RF_NCS, LCD_NCS
//---------------------------//

/** Pin for the demux A pin. */
#define CS_A 10		
/** Pin for the demux B pin. */
#define CS_B  9		

// RF
//* nRF24L01-PA-Ext
//---------------------------//

// RF Chip Enable pin

/** Pin for RF24 Chip enable. */
#define RFCE REGS	

// Buzzer
//* BUZZER-pTH
//---------------------------//

/** Pin for the PCB buzzer. */
#define BUZZ  6		

/** @} */

/**
 * @brief Initializes the pins on the arduino pro mini to input or output.
 */
void InitControllerPins();
