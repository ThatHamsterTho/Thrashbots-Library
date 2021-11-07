/* 
 	* Controller.cpp - Library for handling the RF24 
	* communication and reading the varios inputs on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#include "Controller.h"
#include <Arduino.h>

void CTRL::GetStruct(RF_PACKAGE* buff){
	buff->Package_Header		= 0x0000001D;
	buff->Pot[Left]  			= (uint16_t)getPot(Left);
	buff->Pot[Right] 			= (uint16_t)getPot(Right);
	buff->Joystick[Left][Hor]   = (uint16_t)getJoystick(Left,  Hor);
	buff->Joystick[Left][Ver]   = (uint16_t)getJoystick(Left,  Ver);
	buff->Joystick[Right][Hor]  = (uint16_t)getJoystick(Right, Hor);
	buff->Joystick[Right][Ver]  = (uint16_t)getJoystick(Right, Ver);
	buff->switch_L 				= (bool)getSwitch(Left);
	buff->switch_R 				= (bool)getSwitch(Right);
	buff->bumper_L 				= (bool)getBumper(Left);
	buff->bumper_R 				= (bool)getBumper(Right);
}

CTRL::CTRL(const char RF24_trans_Addr[32]) 
	: Joys{Joystick(J_HL, J_VL), Joystick(J_HR, J_VR)}, RF_Address((uint8_t*)RF24_trans_Addr) {}
CTRL::~CTRL() {}

int CTRL::getSwitch(Side Id){ return digitalRead(SW_L + Id); }
int CTRL::getBumper(Side Id){ return digitalRead(BMPL + Id); }
int CTRL::getPot   (Side Id){ return analogRead (POTL - Id); }

bool CTRL::begin(const char RF24_recv_addr[32], bool Receiver){
	if(!radio.begin()){
		Serial.println("radio hardware is not responding!");
		return 0;
	}

	// Set the PA Level low to try preventing power supply related problems
	// because these examples are likely run with nodes in close proximity to
	// each other.
	radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

	// save on transmission time by setting the radio to only transmit the
	// number of bytes we need to transmit a float
	radio.setPayloadSize(sizeof(RF_PACKAGE)); // 32 bytes

	// set the TX address of the RX node into the TX pipe
  	radio.openWritingPipe(RF_Address);     // always uses pipe 0

	// set the RX address of the TX node into a RX pipe
  	radio.openReadingPipe(1, (uint8_t*)RF24_recv_addr); // using pipe 1

	ToggleReceiveMode(Receiver);
	return 1;
}

bool CTRL::TransmitCtrlData(){
	RF_PACKAGE payload;
	GetStruct(&payload);
	return TransmitRF_Package(&payload);
}

bool CTRL::TransmitRF_Package(RF_PACKAGE* payload){
	// transmitter code
	#ifdef SERIAL_DEBUG
    unsigned long start_timer = micros();                    // start the timer
	#endif
    bool report = radio.write(payload, sizeof(RF_PACKAGE));  // transmit & save the report
	#ifdef SERIAL_DEBUG
    unsigned long end_timer = micros();                      // end the timer
	#endif

	#ifdef SERIAL_DEBUG
	if (report) {
      Serial.print("Transmission successful! ");             // payload was delivered
      Serial.print("Time to transmit = ");
      Serial.print(end_timer - start_timer);                 // print the timer result
	  Serial.println(" send " + String((uint32_t)sizeof(RF_PACKAGE)) + " bytes.");
      
    } else {
      Serial.println("Transmission failed or timed out"); // payload was not delivered
    }
	#endif

	return report;
}
bool CTRL::get_RF_Package(RF_PACKAGE* buffer) {
	uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
		uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
		radio.read(buffer, bytes);             // fetch payload from FIFO
		#ifdef SERIAL_DEBUG
			Serial.println("Received " + String(bytes) + " bytes on pipe " + String(pipe));
		#endif
		return true;
    }
	return false;
}

void CTRL::ToggleReceiveMode(bool ReceiveMode){
	if(!ReceiveMode){
		radio.stopListening();  // put radio in TX mode
	}
	else{
		radio.startListening(); // put radio in RX mode
	}
}
void CTRL::ToggleTransferMode(bool TransferMode){ToggleReceiveMode(!TransferMode);}

void CTRL::beep(int delayms){
	digitalWrite(BUZZ, HIGH);
  	delay(delayms);
  	digitalWrite(BUZZ, LOW);
}