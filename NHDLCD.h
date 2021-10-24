#pragma once
#include "Demux.h"

class GLCD {
private:	
	void write(unsigned char d);

public:

	GLCD();
	~GLCD();
	
	void init();

	void data_write(unsigned char b);
	void comm_write(unsigned char b);

	void DispPic(unsigned char *lcd_string);
	void DispCharSet();
	void ClearLCD();

	void write_char(char c);
	void SetCursorPos(unsigned char x, unsigned char y);
};
