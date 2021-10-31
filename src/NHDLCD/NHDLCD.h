/* 
 	* NHDLCD.h - Header for handeling the NHDLCD on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#pragma once
#include <inttypes.h>

#define GLCD_CHAR_COLS 	  18
#define GLCD_CHAR_ROWS     4

// display has 128x32 pixels. each column contains 1 byte (8bits)
#define GLCD_COLS		 128
#define GLCD_ROWS		   4

#define PIXEL_COL		   0
#define CHAR_COL		   1

class GLCD {
public:

	GLCD();
	~GLCD();
	
	void begin();

	void clear();
	void clearLine(uint8_t);
	void home();

	void noDisplay();
	void display();

	void setCursor(uint8_t, uint8_t, bool = CHAR_COL); 
	
	// T should be a numerical value or string or a character
	template<typename T>
	int print(T value){
		return print(String(value));
	}

	int print(String s);

	template<typename T>
	int println(T value){
		int i = print(String(value)); setCursor(0, C_POS[1]+1);
		return i;
	}

	void writeChar(char c);

	// graphic functions
	void writeCustomChar(uint8_t[5]);
	void writeCustomBlock(uint8_t[7]);
	void DispPic(unsigned char *lcd_string);
	void DispCharSet();

	void command(uint8_t);
	void write(uint8_t);
	
	void getCursorPos(uint8_t* C_X, uint8_t* C_Y, bool = CHAR_COL);
private:
	void send(uint8_t, uint8_t);

	void HandleSpecialChar(char);

	enum {
		C_POS_X,
		C_POS_Y
	};
	const uint8_t _numlines = 4;
	uint8_t C_POS[2] = {0, 0};

	
};
