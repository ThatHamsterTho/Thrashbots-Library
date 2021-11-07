/* 
 	* pins.h - Header containing all pin definitions of the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#pragma once
#include <Arduino.h>

// IRQ
//---------------------------//

#define IRQ_RF  A5 /* << Interrupt pin of the nRF24 module. */
#define IRQ_MPU A4 /* << Interrupt pin of the MPU module. */

// SPI
//---------------------------//

#define SCK  13 /* << SPI Serial Clock line. */
#define MISO 12	/* << SPI MISO line. */
#define MOSI 11	/* << SPI MOSI line. */

// switches
//* 10TC605
//---------------------------//

#define SW_L  2		/* << Left  switch pin. */
#define SW_R  3		/* << Right switch pin. */

// bumpers
//* B3F-3150
//---------------------------//

#define BMPL  4		/* << Left  bumper pin. */
#define BMPR  5		/* << Right bumper pin. */

// pots
//* PTV09A-4215F-B103
//---------------------------//

#define POTL A7		/* << Left  potentiometer analog pin. */
#define POTR A6		/* << Right potentiometer analog pin. */

// joysticks
//* JoystickPTH
//---------------------------//

#define J_VL A3		/* << Left  joystick vertical   analog pin. */
#define J_HL A2		/* << Left  joystick horizontal analog pin. */
#define J_HR A1		/* << Right joystick horizontal analog pin. */
#define J_VR A0		/* << Left  joystick vertical   analog pin. */

// LCD
//* NHD-C12832A1Z-FSW-FBW-3V3
//---------------------------//

/**
 * @brief Pin to determine if LCD byte is command or data.
 * REGS: used to select instruction or data for LCD. A0 pin in datasheet.
 */
#define REGS  7

#define NRST  8		/* << Pin for the Not reset pin on the LCD. */	

// Chip Select Demux
//* CD4052BM
// used to select either MPU_NCS, RF_NCS, LCD_NCS
//---------------------------//

#define CS_A 10		/* << Pin for the demux A pin. */
#define CS_B  9		/* << Pin for the demux B pin. */

// RF
//* nRF24L01-PA-Ext
//---------------------------//

// RF Chip Enable pin

#define RFCE REGS	/* << Pin for RF24 Chip enable. */

// Buzzer
//* BUZZER-pTH
//---------------------------//

#define BUZZ  6		/* << Pin for the PCB buzzer. */

/**
 * @brief Initializes the pins on the arduino pro mini to input or output.
 */
void InitControllerPins();
