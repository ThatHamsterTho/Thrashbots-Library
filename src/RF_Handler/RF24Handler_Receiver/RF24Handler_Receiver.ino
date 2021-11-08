#include <BimTi_RFHandler.h>

// ATMega2560 CE & CSN pins
#define CE    48
#define CSN   53
// Setup the RFHandler. The device name in this case is BattleBotTeam__Tx
RFHandler RH(CE, CSN, "BattleBotTeam__Rx");

void setup() {
	Serial.begin(115200);
	// initialize the nRF24l01 module and open a link to BattleBotTeam__Tx
	if(!RH.begin("BattleBotTeam__Tx")){
		while(1);
	}

	Serial.println("starting listening");
}

void loop() {
	RF_PACKAGE buff;
	// check if a packet was received
	if(RH.getPacket(&buff)){
		// read and display packet in serial monitor
		Serial.println(buff.Package_Header, HEX);
		Serial.println(String(buff.Pot[0]) + " | " + String(buff.Pot[1]));
		Serial.println(String(buff.Joystick[0][0]) + " " + String(buff.Joystick[0][1]) + " | " + String(buff.Joystick[1][0]) + " " + String(buff.Joystick[1][1]));
		Serial.println(buff.toggles, BIN);
	}
}