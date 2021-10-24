#include <Arduino.h>
#include "NHDLCD.h"
#include "pins.h"

const char CharacterSet[96][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00}, // Space
	{0x00, 0x00, 0x4F, 0x00, 0x00}, // !
	{0x00, 0x07, 0x00, 0x07, 0x00}, // "
	{0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
	{0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
	{0x23, 0x13, 0x08, 0x64, 0x62}, // %
	{0x36, 0x49, 0x55, 0x22, 0x50}, // &
	{0x00, 0x05, 0x03, 0x00, 0x00}, // '
	{0x00, 0x1C, 0x22, 0x41, 0x00}, // (
	{0x00, 0x41, 0x22, 0x1C, 0x00}, // )
	{0x14, 0x08, 0x3E, 0x08, 0x14}, // *
	{0x08, 0x08, 0x3E, 0x08, 0x08}, // +
	{0x00, 0x50, 0x30, 0x00, 0x00}, // ,
	{0x08, 0x08, 0x08, 0x08, 0x08}, // -
	{0x00, 0x60, 0x60, 0x00, 0x00}, // .
	{0x20, 0x10, 0x08, 0x04, 0x02}, // / 
	{0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
	{0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
	{0x42, 0x61, 0x51, 0x49, 0x46}, // 2
	{0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
	{0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
	{0x27, 0x45, 0x45, 0x45, 0x39}, // 5
	{0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
	{0x01, 0x71, 0x09, 0x05, 0x03}, // 7
	{0x36, 0x49, 0x49, 0x49, 0x36}, // 8
	{0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
	{0x00, 0x36, 0x36, 0x00, 0x00}, // :
	{0x00, 0x56, 0x36, 0x00, 0x00}, // ;
	{0x08, 0x14, 0x22, 0x41, 0x00}, // <
	{0x14, 0x14, 0x14, 0x14, 0x14}, // =
	{0x00, 0x41, 0x22, 0x14, 0x08}, // >
	{0x02, 0x01, 0x51, 0x09, 0x06}, // ?
	{0x32, 0x49, 0x79, 0x41, 0x3E}, // @
	{0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
	{0x7F, 0x49, 0x49, 0x49, 0x7F}, // B
	{0x3E, 0x41, 0x41, 0x41, 0x22}, // C
	{0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
	{0x7F, 0x49, 0x49, 0x49, 0x41}, // E
	{0x7F, 0x09, 0x09, 0x09, 0x01}, // F
	{0x7E, 0x41, 0x49, 0x49, 0x7A}, // G
	{0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
	{0x00, 0x41, 0x7F, 0x41, 0x00}, // I
	{0x20, 0x40, 0x41, 0x3F, 0x01}, // J
	{0x7F, 0x08, 0x14, 0x22, 0x41}, // K
	{0x7F, 0x40, 0x40, 0x40, 0x40}, // L
	{0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
	{0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
	{0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
	{0x7F, 0x09, 0x09, 0x09, 0x06}, // P
	{0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
	{0x7F, 0x09, 0x19, 0x29, 0x46}, // R
	{0x46, 0x49, 0x49, 0x49, 0x31}, // S
	{0x01, 0x01, 0x7F, 0x01, 0x01}, // T
	{0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
	{0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
	{0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
	{0x63, 0x14, 0x08, 0x14, 0x63}, // X
	{0x07, 0x08, 0x70, 0x08, 0x07}, // Y
	{0x61, 0x51, 0x49, 0x45, 0x43}, // Z
	{0x7F, 0x41, 0x41, 0x00, 0x00}, // [
  {0x20, 0x10, 0x08, 0x04, 0x02}, // backslash
	{0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
	{0x04, 0x02, 0x01, 0x02, 0x04}, // ^
	{0x40, 0x40, 0x40, 0x40, 0x40}, // _
	{0x00, 0x01, 0x02, 0x04, 0x00}, // `
	{0x20, 0x54, 0x54, 0x54, 0x78}, // a
	{0x7F, 0x48, 0x44, 0x44, 0x38}, // b
	{0x38, 0x44, 0x44, 0x44, 0x20}, // c
	{0x38, 0x44, 0x44, 0x48, 0x7F}, // d
	{0x38, 0x54, 0x54, 0x54, 0x18}, // e
	{0x08, 0x3E, 0x09, 0x01, 0x02}, // f
	{0x0C, 0x2A, 0x2A, 0x2A, 0x3E}, // g
	{0x7F, 0x08, 0x04, 0x04, 0x78}, // h
	{0x00, 0x44, 0x7D, 0x40, 0x00}, // i
	{0x20, 0x40, 0x44, 0x3D, 0x00}, // j
	{0x7F, 0x10, 0x28, 0x44, 0x00}, // k
	{0x00, 0x41, 0x7F, 0x40, 0x00}, // l
	{0x7C, 0x04, 0x18, 0x04, 0x78}, // m
	{0x7C, 0x08, 0x04, 0x04, 0x78}, // n
	{0x38, 0x44, 0x44, 0x44, 0x38}, // o
	{0x7C, 0x14, 0x14, 0x14, 0x08}, // p
	{0x08, 0x14, 0x14, 0x18, 0x7C}, // q
	{0x7C, 0x08, 0x04, 0x04, 0x08}, // r
	{0x48, 0x54, 0x54, 0x54, 0x20}, // s
	{0x04, 0x3F, 0x44, 0x40, 0x20}, // t
	{0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
	{0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
	{0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
	{0x44, 0x28, 0x10, 0x28, 0x44}, // x
	{0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
	{0x44, 0x64, 0x54, 0x4C, 0x44}, // z
	{0x00, 0x08, 0x36, 0x41, 0x00}, // {
	{0x00, 0x00, 0x7F, 0x00, 0x00}, // |
	{0x00, 0x41, 0x36, 0x08, 0x00}, // }
	{0x08, 0x08, 0x2A, 0x1C, 0x08}, // ->
	{0x08, 0x1C, 0x2A, 0x08, 0x08}  // <-
};

GLCD::GLCD() {}
GLCD::~GLCD() {}

void GLCD::init(){
	digitalWrite(NRST, LOW);           
  delay(100);                     
  digitalWrite(NRST, HIGH);           
  delay(100);

	comm_write(0xA0);   // ADC select
	comm_write(0xAE);   // Display OFF
	comm_write(0xC8);   // COM direction scan
	comm_write(0xA2);   // LCD bias set
	comm_write(0x2F);   // Power Control set
	comm_write(0x21);   // Resistor Ratio Set
	comm_write(0x81);   // Electronic Volume Command (set contrast) Double Btye: 1 of 2
	comm_write(0x20);   // Electronic Volume value (contrast value) Double Byte: 2 of 2
	comm_write(0xAF);   // Display ON
}

// Source: https://nhdforum.newhavendisplay.com/index.php?topic=11609.msg21076#msg21076
void GLCD::write(unsigned char d) {	
	for(unsigned int n=0; n<8; n++){
		if((d&0x80)==0x80)
			digitalWrite(SDI, HIGH);
		else
			digitalWrite(SDI, LOW);
		while(0); // nop
		d=(d<<1);
		digitalWrite(SCK, LOW);	while(0); // nop
		digitalWrite(SCK, HIGH);while(0); // nop
		digitalWrite(SCK, LOW);
	}
}

void GLCD::data_write(unsigned char d){
  Demux::Set(CSN::LCD);
	digitalWrite(REGS, HIGH);
	this->write(d);
  Demux::Clear();
}	

void GLCD::comm_write(unsigned char d){
  Demux::Set(CSN::LCD);
	digitalWrite(REGS, LOW);
	this->write(d);
  Demux::Clear();
}

void GLCD::write_char(char c){
  c -= 32;
  data_write(0x00);
  for(int i = 0; i < 5; i++){
    data_write((unsigned char)CharacterSet[(unsigned int)c][i]);
  }
  data_write(0x00);
}

void GLCD::SetCursorPos(unsigned char x, unsigned char y){
  y &= 0x03; // make sure that y is between 0 and 3
  x &= 0x7F; // make sure that x is between 0 and 127
  unsigned char page = 0xB0;
  comm_write(0x40);                           // Display start address + 0x40
  comm_write(page+y);
  comm_write(0x10 + (x & 0xF0>>4));           // column address upper 4 bits 0x10 + upper_x_offset
  comm_write(0x00 + (x & 0x0F   ));           // column address lower 4 bits 0x00 + lower_x_offset
}

void GLCD::DispPic(unsigned char *lcd_string)
{
  unsigned char page = 0xB0;
  comm_write(0x40);                         // Display start address + 0x40
  for(unsigned int i = 0; i < 4; i++){      // 32pixel display / 8 pixels per page = 4 pages
    comm_write(page);                       // send page address
    comm_write(0x10);                       // column address upper 4 bits + 0x10
    comm_write(0x00);                       // column address lower 4 bits + 0x00
    for(unsigned int j = 0; j < 128; j++){  // 128 columns wide
      data_write(*lcd_string);              // send picture data
      lcd_string++;
    }
    page++;                                 // after 128 columns, go to next page
  } 
}

// debug to view all characters
void GLCD::DispCharSet(){
  for(int k = 0; k < 4; k++){
      for(int j = 0; j < 4; j++){
        SetCursorPos(0, j);
        for(int i = 0; i < 18; i++){
          if((' ' + i + (18 * (j+k)) < 128))
            write_char(' ' + i + (18 * (j+k)));
          else
            write_char(' ');
        }
      }
      delay(1000);
    }
}

void GLCD::ClearLCD()
{
  for(unsigned int i = 0 ; i < 4; i++){     // 32pixel display / 8 pixels per page = 4 pages
    SetCursorPos(0,i);
    for(unsigned int j = 0 ; j < 128; j++){ // 128 columns wide
      data_write(0x00);                     // send picture data
    }
  }
}