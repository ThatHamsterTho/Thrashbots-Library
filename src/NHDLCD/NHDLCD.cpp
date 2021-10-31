/* 
 	* NHDLCD.cpp - Library for handling the NHDLCD on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#include <Arduino.h>
#include <SPI.h>
#include "NHDLCD.h"
#include "characterset.h"
#include "../Hardware/pins.h"
#include "../Hardware/Demux.h"

#define GLCD_DISPLAY_ON		0xAF
#define GLCD_DISPLAY_OFF	0xAE

#define DISP_STRT_ADDR		0x40
#define PAGE_STRT_ADDR		0xB0
#define CLMN_ADDR_UPPR		0x10
#define CLMN_ADDR_LWR		0x00

GLCD::GLCD() {}
GLCD::~GLCD() {}

void GLCD::begin(){
	SPI.begin();

	digitalWrite(NRST, LOW);           
  	delay(100);                     
  	digitalWrite(NRST, HIGH);           
  	delay(100);

	command(0xA0);   			// ADC select
	noDisplay();				// Display OFF
	command(0xC8);   			// COM direction scan
	command(0xA2);   			// LCD bias set
	command(0x2F);   			// Power Control set
	command(0x21);   			// Resistor Ratio Set
	command(0x81);   			// Electronic Volume Command (set contrast) Double Btye: 1 of 2
	command(0x20);   			// Electronic Volume value (contrast value) Double Byte: 2 of 2
	display();					// Display ON
	clear();

	delay(10);  				// display delay
}

/********** high level commands, for the user! */
void GLCD::clear() {
	for(unsigned int i = 0 ; i < GLCD_ROWS; i++){   // 32pixel display / 8 pixels per page = 4 pages
    setCursor(0,i);
    for(unsigned int j = 0 ; j < GLCD_COLS; j++){ 	// 128 columns wide
      write(0x00);                     				// send picture data
    }
  }
}

void GLCD::clearLine(uint8_t line){
	line &= 0x03; // make sure that y is between 0 and 3
	setCursor(0,line);
	for(unsigned int j = 0 ; j < GLCD_COLS; j++){ 	// 128 columns wide
      write(0x00);                     				// send picture data
    }
	setCursor(0, line);
}

void GLCD::home() {
	setCursor(0, 0);
}

void GLCD::setCursor(uint8_t col, uint8_t row, bool pixcol){
	col &= 0x7F; // make sure that x is between 0 and 127
	row &= 0x03; // make sure that y is between 0 and 3
	C_POS[C_POS_X] = col * (7*pixcol);
	C_POS[C_POS_Y] = row;
	unsigned char page = PAGE_STRT_ADDR;
	command(DISP_STRT_ADDR);                 			 		// Display start address + 0x40
	command(page+C_POS[1]);
	command(CLMN_ADDR_UPPR + (C_POS[C_POS_X] & 0xF0 >> 4));           // column address upper 4 bits 0x10 + upper_x_offset
	command(CLMN_ADDR_LWR  + (C_POS[C_POS_X] & 0x0F     ));           // column address lower 4 bits 0x00 + lower_x_offset
	
}

void GLCD::noDisplay() {
	command(GLCD_DISPLAY_OFF);
}

void GLCD::display() {
	command(GLCD_DISPLAY_ON);
}

int GLCD::print(String S){
	unsigned int i = 0;
	for(; i < S.length() && i < 18; i++){
		writeChar(S[i]);
	}
	return (int)i;
}

void GLCD::writeChar(char c){
	if(c < 32){
		HandleSpecialChar(c);
	}
  	c -= 32;
	write(0x00);
	for(int i = 0; i < GLCD_SYMBOL_WIDTH; i++){
		write((uint8_t)pgm_read_byte(&(CharacterSet[(unsigned int)c][i])));
	}
	write(0x00);
}

/*	
customcharacter = 7 bytes.
Every column is 1 byte => every bit is a pixel, 1 is black, 0 is white
bytes are shown from top to bottom. with MSB being the top.
*/

void GLCD::writeCustomChar(uint8_t c[GLCD_SYMBOL_WIDTH]){
	write(0x00);
  	for(int i = 0; i < GLCD_SYMBOL_WIDTH; i++){
   		write((unsigned char)c[i]);
 	}
  	write(0x00);
	C_POS[C_POS_X] += 7;
}

void GLCD::writeCustomBlock(uint8_t symbol[7]){
	for(int i = 0; i < 7; i++){
   		write((unsigned char)symbol[i]);
 	}
	C_POS[C_POS_X] += 7;
}

/*********** mid level commands, for sending data/cmds */

void GLCD::command(uint8_t value){
	this->send(value, LOW);
}

void GLCD::write(uint8_t value) {
	this->send(value, HIGH);
}

void GLCD::getCursorPos(uint8_t* C_X, uint8_t* C_Y, bool pixcol){
	if(pixcol){
		*C_X = C_POS[C_POS_X] / 7;
		*C_Y = C_POS[C_POS_Y];
	}
	else{
		*C_X = C_POS[C_POS_X];
		*C_Y = C_POS[C_POS_Y];
	}
}


void GLCD::HandleSpecialChar(char c) {
	switch (c)
	{
	case 0:	 // NUL ( Null )
		/* code */
		break;
	case 1:  // SOH ( Start of Header)
		/* code */
		break;
	case 2:  // STX ( Start of text)
		/* code */
		break;
	case 3:  // ETX ( End of text)
		/* code */
		break;
	case 4:  // EOT ( End of transmission)
		/* code */
		break;
	case 5:  // ENQ ( Enquiry )
		/* code */
		break;
	case 6:  // ACK ( Acknowledge )
		/* code */
		break;
	case 7:  // BEL ( Bell )
		/* code */
		break;
	case 8:  // BS  ( Backspace )
		/* code */
		break;
	case 9:  // HT  ( Horizontal Tab )
		/* code */
		break;
	case 10: // LF  ( Line Feed )
		/* code */
		break;
	case 11: // VT  ( Vertical Tab )
		/* code */
		break;
	case 12: // FF  ( Form Feed )
		/* code */
		break;
	case 13: // CR  ( Carriage Return ) 
		/* code */
		break;
	case 14: // SO  ( Shift Out )  
		/* code */
		break;
	case 15: // SI  ( Shift in )
		/* code */
		break;
	case 16: // DLE ( Data Link Escape )
		/* code */
		break;
	case 17: // DC1 ( Device Control 1 )
		/* code */
		break;
	case 18: // DC2 ( Device Control 2 )
		/* code */
		break;
	case 19: // DC3 ( Device Control 3 )
		/* code */
		break;
	case 20: // DC4 ( Device Control 4 )
		/* code */
		break;
	case 21: // NAK ( Negatice Acknowledge)
		/* code */
		break;
	case 22: // SYN ( Synchronize )
		/* code */
		break;
	case 23: // ETB ( End of Transmission Block )
		/* code */
		break;
	case 24: // CAN ( Cancel )
		/* code */
		break;
	case 25: // EM  ( End of Medium )
		/* code */
		break;
	case 26: // SUB ( Substitute )
		/* code */
		break;
	case 27: // ESC ( Escape )
		/* code */
		break;
	case 28: // FS  ( File Separator )
		/* code */
		break;
	case 29: // GS  ( Group Seperator )
		/* code */
		break;
	case 30: // RS  ( Record Separator )
		/* code */
		break;
	case 31: // US  ( Unit Seperator )
		/* code */
		break;
	default:
		return;
		break;
	}
}

/************ low level data pushing commands **********/

// Source: https://nhdforum.newhavendisplay.com/index.php?topic=11609.msg21076#msg21076
void GLCD::send(uint8_t d, uint8_t mode) {
	//CSPI.waitAvailable("GLCD");
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));
	Demux::Set(CSN::LCD);
	digitalWrite(REGS, mode);
	SPI.transfer(d);
	Demux::Set(CSN::NONE);
	SPI.endTransaction();
}	


/************ graphical functions **********/

void GLCD::DispPic(unsigned char *lcd_string)
{
  unsigned char page = PAGE_STRT_ADDR;
  command(DISP_STRT_ADDR);                   	 // Display start address + 0x40
  for(unsigned int i = 0; i < GLCD_ROWS; i++){   // 32pixel display / 8 pixels per page = 4 pages
    command(page);                       		 // send page address
    command(CLMN_ADDR_UPPR);                	 // column address upper 4 bits + 0x10
    command(CLMN_ADDR_LWR);                 	 // column address lower 4 bits + 0x00
    for(unsigned int j = 0; j < GLCD_COLS; j++){ // 128 columns wide
      command(*lcd_string);              		 // send picture data
      lcd_string++;
    }
    page++;                                 	 // after 128 columns, go to next page
  } 
}

// debug to view all characters
void GLCD::DispCharSet(){
  for(int k = 0; k < 4; k++){
      for(int j = 0; j < 4; j++){
        setCursor(0, j);
        for(int i = 0; i < 18; i++){
          if((' ' + i + (18 * (j+k)) < 128))
            write(' ' + i + (18 * (j+k)));
          else
            write(' ');
        }
      }
      delay(1000);
    }
}
