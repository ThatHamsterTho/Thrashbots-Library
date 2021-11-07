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

#define SERIAL_DEBUG
#define RF_HEAD_BYTE_SIZE		 4
#define RF_DATA_BYTE_SIZE		28

enum Side {
	Left = 0,
	Right = 1,
};

// you can create a custom RF_Package aslong as it is not bigger than 32 bytes.

union RF_PACKAGE {
	/**
	 * @brief Custom RF_PACKAGE Data.
	 * Used for any custom protocol made with the RF_PACKAGE union.
	 */
	struct {
		uint8_t RF_Header[RF_HEAD_BYTE_SIZE];	/*<< RF_Header bytes. */
		uint8_t RF_DATA[RF_DATA_BYTE_SIZE];	/*<< RF_DATA bytes. */
	};
	/**
	 * @brief Controller Data Struct.
	 * Contains all passed controller data.
	 */
	struct {
		uint32_t Package_Header;		/*<< Package header used to shift the other data further down. */
		uint16_t Pot[2];				/*<< Potentiometer values. */
		uint16_t Joystick[2][2];		/*<< Joystick values. */
		union {
			uint8_t toggles;	/*<< full byte containing the switch and bumper values. */
			struct {
				uint8_t switch_L : 1; /*<< switch_L boolean. */
				uint8_t switch_R : 1; /*<< switch_R boolean. */
				uint8_t bumper_L : 1; /*<< bumper_L boolean. */
				uint8_t bumper_R : 1; /*<< bumper_R boolean. */
			};
		};
	};

	/**
	 * @cond __JUNKDATA
	 * @brief Junk data.
	 * used in the union to make sure the union is 32 bytes long.
	 */
	struct {
		uint32_t __RF_HEADER;
		uint32_t __RF_DB0;
		uint32_t __RF_DB1;
		uint32_t __RF_DB2;
		uint32_t __RF_DB3;
		uint32_t __RF_DB4;
		uint32_t __RF_DB5;
		uint32_t __RF_DB6;
	};
	/* << @endcond */
};

class CTRL {
	RF24 radio;

	Joystick Joys[2];
	// It is very helpful to think of an address as a path instead of as
	// an identifying device destination
	const uint8_t *RF_Address;

	void GetStruct(RF_PACKAGE* buff);

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
	RF24* getRadio(){return &radio;}

	void beep(int delayms = 15);
};
