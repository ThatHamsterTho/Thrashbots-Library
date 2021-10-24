#pragma once

class Joystick {
private:
	const unsigned char Pins[2];

public:
	enum Axis {
		H = 0,
		V = 1
	};

	Joystick(const unsigned char Hpin, const unsigned char Vpin) : Pins{Hpin, Vpin} {}

	int operator[](unsigned int index);

	int getHorizontal(void);
	int getVertical(void);
};
