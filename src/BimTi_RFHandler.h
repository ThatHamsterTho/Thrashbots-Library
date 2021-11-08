#pragma once

#include <arduino.h>
#include <SPI.h>
#include "BimTi_NRF24/BimTiController_RF24.h"



/** @defgroup RF_DEFS RFHandler Definitions
 * This module contains all the RFHandler definitions
 * @{
 */
/** If enabled prints debug info to the serial */
#define SERIAL_DEBUG
/** Size of the RF_PACKAGE Header */
#define RF_HEAD_SIZE		 4	
/** Size of the RF_PACKAGE Data section */
#define RF_DATA_SIZE		28	
/** @} */

/** @union RF_PACKAGE
 * @brief RF_PACKAGE union.
 * @details RF_PACKAGE Format for communication between the transmitter and receiver. \n
 * the Package_Header can be customized for different packages. \n
 * the default Package_Header value used for controller data is 0x0000001D. \n
 * A package_Header value consists of 4 bytes:
 * \li byte1: MessageType.
 * \li byte2: receiverID.
 * \li byte3: TransferID.
 * \li byte4: Amount of data transfered.
 * byte2 and byte3 are unused in the default header.
 * byte1 is 0x00 for the default header for speed.
 * The following 7 ints, or 24 bytes of data can be used freely.
 */
union RF_PACKAGE {
	/**
	 * @brief Custom RF_PACKAGE Data.
	 * Used for any custom protocol made with the RF_PACKAGE union.
	 */
	struct {
		/** RF_Header bytes. Size defined by RF_HEAD_SIZE @sa RF_DEFS */
		uint8_t RF_Header[RF_HEAD_SIZE];	
		/** RF_DATA bytes. Size defined by RF_DATA_SIZE @sa RF_DEFS */
		uint8_t RF_DATA[RF_DATA_SIZE];		
	};
	/**
	 * @brief Controller Data Struct.
	 * Contains all passed controller data.
	 */
	struct {
		/** Package header used to shift the other data further down. */
		uint32_t Package_Header;		
		/** Potentiometer values. */
		uint16_t Pot[2];				
		/** Joystick values. */
		uint16_t Joystick[2][2];		
		union {
			/** full byte containing the switch and bumper values. */
			uint8_t toggles;	
			struct {
				/** switch_L boolean. */
				uint8_t switch_L : 1; 
				/** switch_R boolean. */
				uint8_t switch_R : 1; 
				/** bumper_L boolean. */
				uint8_t bumper_L : 1; 
				/** bumper_R boolean. */
				uint8_t bumper_R : 1; 
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


/**
 * @brief RFHandler class
 * This class is used to easily control the RF24 module.
 * Can be used to implement a custom communication protocol that uses RF_PACKAGE objects
 */
class RFHandler {
public:
	/**
	 * @brief A union for radio initilization flags.
	 * Keeps track of Radio initalization erros.
	 */
	union RInit_Flags {
		uint8_t flags;				/**< if flags is 0x00 there were no errors. */
		struct {
		uint8_t name_empty 	: 1;	/**< if name_empty is true it means that no transmitter name was passed */
		uint8_t w_name_tx	: 1;	/**< if w_name_tx is true it means that the naming convention was not followed */
		uint8_t w_name_rx	: 1;	/**< if w_name_rx is true it means that the naming convention was not followed */
		uint8_t hw_error	: 1;	/**< if hw_error is true it means that a hardware error occured */
		};
	};
private:
	/**
	 * @brief The RF24 object. 
	 * This is used as a handle for all the RF24 library functions.
	 */
	RF24 radio;

	/**
	 * @brief RF transmitter address.
	 * @remark It is very helpful to think of an address as a path instead of as an identifying device destination
	 */
	const uint8_t *RF_Address;

	/**
	 * @brief Rinit_Flags instance to keep track of initilization flags.
	 */
	RInit_Flags Radio_Inittable = {Radio_Inittable.flags = 0x00};

	/**
	 * @brief Tracks if the RF24 object was initialized.
	 * This is used in the begin() function to prevent initializing the RF24 object twice.
	 * @sa begin()
	 */
	bool RF_initialized = false;

	/**
	 * @brief Keeps track in which Tx mode the controller is in.
	 * @sa toggleReceiveMode()
	 * @sa toggleTransferMode()
	 * @sa transmitCtrlData()
	 * @sa transmitRF_Package()
	 * @sa get_RF_Package()
	 */
	bool ReceiveMode = false;
public:
	/** 
	 * @brief Constructs a RFHandler object.
	 * @param CE The pin number on which the RF24 CE is connected.
	 * @param CSN the pin number on which the RF24 CSN is connected. If this library is used on the controller, pass 0.
	 * @param RF24_trans_Name Transmitter name should be the battlebot team name, or abbreviation ending with __Tx or __Rx Max transmitter name is 32 characters.
	 * @param bypass_name_conv Set to true if you want to use the controller for other use cases than the battle bot project.
	 * @param Errors A buffer to contain the produced errors from the constructor.
	 * @sa begin()
	 */
  	RFHandler(const uint8_t CE, const uint8_t CSN, const char RF24_trans_Name[32], bool bypass_name_conv = false, RInit_Flags* Errors = nullptr);
	
	/**
	 * @brief Destroy the RFHandler object
	 * 
	 */
	~RFHandler();
	
	/**
	 * @brief Initializes the radio object.
	 * @param RF24_recv_addr The receiver name should be the battlebot team name, or abbreviation ending with __Rx or __Tx Max Receiver name is 32 characters.
	 * @param bypass_name_conv Set to true if you want to use the controller for other use cases than the battle bot project.
	 * @param Errors A buffer to contain the produced errors from the constructor.
	 * @remark if begin is called after an end it will open a reading pipe to the new address.
	 * @return 1 on successfull exit. 0 on error exit. Check the serial monitor if any errors occur.
	 * @sa end()
	 */
	bool begin(const char RF24_recv_addr[32], bool bypass_name_conv = false, RInit_Flags* Errors = nullptr);

	/**
	 * @brief Closes connection with RF24 module passed at the begin command.
	 * @remark if called without a begin command this will not do anything.
	 * @warning if called with a hardware failure this function will malfunction.
	 * @sa begin()
	 */
	void end();

	/**
	 * @brief Switches between Receiving on the RF24 module or transmitting.
	 * @param ReceiveMode Sets the controller as receiver ( true ) or transmitter ( false ).
	 * @sa toggleTransferMode()
	 */
	void toggleReceiveMode(bool ReceiveMode = true);

	/** 
	 * @brief switches between transmitting on the RF24 module or receiving.
	 * @param TransferMode sets the controller as transmitter ( true ) or receiver ( false ).
	 * @sa toggleReceiveMode()
	 */
	void toggleTransferMode(bool TransferMode = true);

	/** 
	 * @brief transmits a RF_PACKAGE payload to the receiver.
	 * 
	 * @remark use SERIAL_DEBUG to get debug information on this function.
	 * @param payload an RF_PACKAGE pointer to the data being send.
	 * @return 0 on transmission failed. 1 on transmission success.
	 * 
	 * @warning if the controller is in receive mode this will return 0.
	 * @sa transmitCtrlData()
	 * @sa getPacket()
	 * @sa SERIAL_DEBUG
	 */
	bool transmitPacket(RF_PACKAGE* payload);

	/** 
	 * @brief if in receiving mode returns the received RF_PACKAGE.
	 * 
	 * @remark use SERIAL_DEBUG to get debug information on this function.
	 * @param buffer an RF_PACKAGE pointer.
	 * @return 0 if no package is received. 1 if a package is received.
	 * 
	 * @warning is the controller is in transmit mode this will return 0.
	 * @sa transmitCtrlData()
	 * @sa transmitPacket()
	 * @sa SERIAL_DEBUG
	 */
	bool getPacket(RF_PACKAGE* buffer);

	/**
	 * @brief Getter for the RF24 object inside the controller.
	 * @return pointer to the RF24 object of the controller.
	 * 
	 * @warning if radio was not properly initialized this object could malfunction.
	 * @sa begin()
	 * @sa radio
	 */
	RF24* getRadio(void){return &radio;}
};

/** @example RF_Handler/Controller_Transmitter/Controller_Transmitter.ino
 * This is an example of how to use the CTRL class to transmit data.
 * The sketch sets the controller name to "BattleBotTeam__Tx". This is the name of the transmitter
 * Then in the setup it calls the CTRL::begin() is called and the name of the receiver is passed to open a link to the receiver.
 * It checks if any errors occured, which will be printed to the serial and continues to the loop.
 * In the loop it uses the CTRL::transmitCtrlData() method to send the current position of all the hardware switches, bumpers, pots and joysticks.
 */

/** @example RF_Handler/RF24Handler_Receiver/RF24Handler_Receiver.ino
 * This is an example of how to use the RFHandler class as a receiver.
 * it defines the pins the nRF24l01 is connected to, in this case an Arduino 2560 is used
 * Then it passes these values and the name of the device to the RFHandler constructor.
 * In the setup the RFHandler::begin() is called and the name of the transmitter is passed to open a link to the transmitter.
 * In the loop it continually checks if a packet was received, and if that is the case it prints the value.
 * This example is meant to be used inconjunction with the example of @link RF_Handler/Controller_Transmitter/Controller_Transmitter.ino @endlink
 */

/** @example RF_Handler/RF24Handler_Transmitter/RF24Handler_Transmitter.ino
 * This is an example of how to use the RFHanlder class as a transmitter
 * it defines the pins the nRF24l01 is connected to, in this case an Arduino 2560 is used
 * Then it passes these values and the name of the device to the RFHandler constructor.
 * Then in the setup it calls the CTRL::begin() is called and the name of the receiver is passed to open a link to the receiver.
 * After the nRF24l01 module is initialized the RFHandler::toggleTransferMode() is called to indicate that the module is to be used as a transmitter.
 * In the loop it creates a RF_PACKAGE packet and sets the Package_Header, this is done by adding the size of the packet to the packet_ID
 * Afterwards the data is filled and transmitted.
 */