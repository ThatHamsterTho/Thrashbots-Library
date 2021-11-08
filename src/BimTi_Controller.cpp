/* 
 	* BimTiController.cpp - Library for handling the RF24 
	* communication and reading the varios inputs on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#include "BimTi_Controller.h"
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

CTRL::CTRL(const char RF24_trans_Addr[32], bool bypass_name_conv, RInit_Flags* Errors) : RFHandler(RFCE, 0, RF24_trans_Addr, bypass_name_conv, Errors) {}
CTRL::~CTRL() {}

bool CTRL::begin(const char RF24_recv_Name[32], bool bypass_name_conv, RInit_Flags* Errors){
	bool var = RFHandler::begin(RF24_recv_Name, bypass_name_conv, Errors);
	toggleTransferMode(true); // set controller in Tx mode
	return var;
}


int CTRL::getSwitch(Side Id){ return digitalRead(SW_L + Id); }
int CTRL::getBumper(Side Id){ return digitalRead(BMPL + Id); }
int CTRL::getPot   (Side Id){ return analogRead (POTL - Id); }
int CTRL::getJoystick(Side Id, Axis A){return analogRead(Jpins[Id][A]);}

void CTRL::beep(int delayms){
	digitalWrite(BUZZ, HIGH);
  	delay(delayms);
  	digitalWrite(BUZZ, LOW);
}

bool CTRL::transmitCtrlData(){
	RF_PACKAGE payload;
	GetStruct(&payload);
	return transmitPacket(&payload);
}
