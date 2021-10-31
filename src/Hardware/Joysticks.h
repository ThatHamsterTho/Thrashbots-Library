/* 
 	* Joysticks.h - Header for handling the two joysticks on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/


#pragma once

enum Axis {
	Hor = 0,
	Ver = 1
};

class Joystick {
private:
	const unsigned char Pins[2];

public:
	
	Joystick(const unsigned char Hpin, const unsigned char Vpin) : Pins{Hpin, Vpin} {}

	int operator[](unsigned int index);

	int getHorizontal(void);
	int getVertical(void);
};
