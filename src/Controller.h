/* 
 	* Controller.cpp - Header for handling the RF24 
	* communication and reading the varios inputs on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/


#pragma once
#include "Hardware/pins.h"
#include "Hardware/Joysticks.h"
#include "NRF24/RF24.h"

enum Side {
	Left = 0,
	Right = 1,
};

// you can create a custom RF_Package aslong as it is not bigger than 32 bytes.
union RF_PACKAGE {
	int RF_DATA[8];
	struct {
		int Pot[2];
		int Joystick[2][2];
		union {
			uint8_t toggles;
			struct {
				uint8_t switch_L : 1;
				uint8_t switch_R : 1;
				uint8_t bumper_L : 1;
				uint8_t bumper_R : 1;
			};
		};
	};
};

class CTRL {
	RF24 radio;

	Joystick Joys[2];
	// It is very helpful to think of an address as a path instead of as
	// an identifying device destination
	const uint8_t *RF_Address;

	RF_PACKAGE GetStruct();

public:
	

  	CTRL(const char RF24_trans_Name[32]);
  	~CTRL();

	bool begin(const char RF24_recv_Name[32], bool Receiver = false);
	void end();

	int getSwitch(Side Id);
	int getBumper(Side Id);
	int getPot   (Side Id);
	int getJoystick(Side Id, Axis A){ return Joys[Id][A]; }

	void ToggleReceiveMode(bool ReceiveMode = true);
	void ToggleTransferMode(bool TransferMode = true);
	bool TransmitRF_Package(RF_PACKAGE* payload);
	bool TransmitCtrlData();
	bool get_RF_Package(RF_PACKAGE* buffer);

	void beep(int delayms = 15);
};
