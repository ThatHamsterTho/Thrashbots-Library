/* 
 	* BimTiController.h - Header for handling the RF24 
	* communication and reading the varios inputs on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/


#pragma once

#include "BimTi_Hardware/pins.h"
#include "BimTi_RFHandler.h"


/** @defgroup designators designators
 * This module contains all the designators used to indicate which side or which axis should be read
 * @sa CTRL::getSwitch()
 * @sa CTRL::getBumper()
 * @sa CTRL::getPot()
 * @sa CTRL::getJoystick()
 * @{
 */

/**
 * @brief Side enum
 * Used for the input side designation
 */
enum Side {
	/** (0) represents left side of controller */
	Left = 0,  
	/** (1) represents right side of controller */
	Right = 1, 
};

/**
 * @brief Axis enum
 * Used for the joystick axis designation
 */
enum Axis {
	/** (0) represents horizontal axis of joystick */
	Hor = 0, 
	/** (1) represents vertical axis of joystick */
	Ver = 1  
};
/** @} */


/**
 * @brief CTRL class
 * @details is used a general and easy way to access the different components on the Controller
 * For control over the LCD use the GLCD class.
 * @sa GLCD
 * @sa RFHandler
 */
class CTRL : public RFHandler {
private:
	/**
	 * @brief The RF24 object. 
	 * This is used as a handle for all the RF24 library functions.
	 */
	RF24 radio;

	/**
	 * @brief A matrix for the joystick pins.
	 */
	const uint8_t Jpins[2][2] = {
		{J_HL, J_VL},
		{J_HR, J_VR}
	};

	/**
	 * @brief Transfer all controller inputs to a struct.
	 * 
	 * @return RF_PACKAGE containing controller inputs at time of read.
	 */
	void GetStruct(RF_PACKAGE* buff);

public:
	
	/** 
	 * @brief Constructs a CTRL object.
	 * @param RF24_trans_Name Transmitter name should be the battlebot team name, or abbreviation ending with __Tx Max transmitter name is 32 characters.
	 * @param bypass_name_conv Set to true if you want to use the controller for other use cases than the battle bot project.
	 * @param Errors A buffer to contain the produced errors from the constructor.
	 * @sa begin()
	 */
  	CTRL(const char RF24_trans_Name[32] = "", bool bypass_name_conv = false, RInit_Flags* Errors = nullptr);

	/**
	 * @brief Destroy the CTRL object.
	 */
  	~CTRL();

	/**
	 * @brief Initializes the radio object.
	 * @param RF24_recv_Name The receiver name should be the battlebot team name, or abbreviation ending with __Rx Max Receiver name is 32 characters.
	 * @param bypass_name_conv Set to true if you want to use the controller for other use cases than the battle bot project.
	 * @param Errors A buffer to contain the produced errors from the constructor.
	 * @remark if begin is called after an end it will open a reading pipe to the new address.
	 * @return 1 on successfull exit. 0 on error exit. Check the serial monitor if any errors occur.
	 */
	bool begin(const char RF24_recv_Name[32] = "", bool bypass_name_conv = false, RInit_Flags* Errors = nullptr);

	/** 
	 * @brief Gets the given rocker switches' state.
	 * @param Id Right or Left Bumper.
	 * @return state of either switch (0 or 1).
	 * @sa Side
	 * @sa getBumper()
	 * @sa getPot()
	 * @sa getJoystick()
	 */
	int getSwitch(Side Id);

	/**
	 * @brief Gets the given Bumper's state.
	 * @param Id Right or Left Bumper.
	 * @return state of either bumper (0 or 1).
	 * @sa Side
	 * @sa getSwitch()
	 * @sa getPot()
	 * @sa getJoystick()
	 */
	int getBumper(Side Id);

	/** 
	 * @brief Gets the given pot's value.
	 * @param Id Right or Left Potentiometer.
	 * @return state of the two pot meters (0-1052).
	 * @sa Side
	 * @sa getSwitch()
	 * @sa getBumper()
	 * @sa getJoystick()
	 */
	int getPot   (Side Id);

	/**
	 * @brief Gets the value of the given joystick's Axis.
	 * @param Id Right or Left Joystick.
	 * @param A Horizontal or Vertical Axis.
	 * @return returns the position of either joystick Axis (0-1052).
	 * @remark middle is ~512. Axis is either Hor or Ver.
	 * @sa Side
	 * @sa Axis
	 * @sa getSwitch()
	 * @sa getBumper()
	 * @sa getPot()
	 */
	int getJoystick(Side Id, Axis A);

	/** 
	 * @brief Turns on the buzzer for a delay.
	 * @param delayms The time the buzzer should be on default is 15ms for a short beep.
	 * @warning The buzzer is very load.
	 */
	void beep(int delayms = 15);

	/** 
	 * @brief transmits the current controller positions to the receiver.
	 * @remark use SERIAL_DEBUG to get debug information on this function.
	 * @return 0 on transmission failed. 1 on transmission success.
	 * 
	 * @warning if the controller is in receive mode this will return 0.
	 * @sa SERIAL_DEBUG
	 */
	bool transmitCtrlData(void);	
};
