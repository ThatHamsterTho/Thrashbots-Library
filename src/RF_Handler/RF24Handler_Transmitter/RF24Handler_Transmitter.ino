#include <BimTi_RFHandler.h>

// define a packet ID for the RF_PACKAGE
#define packet_ID 0x10000000

// ATMega2560 CE & CSN pins
#define CE    48
#define CSN   53
// Setup the RFHandler. The device name in this case is BattleBotTeam__Tx
RFHandler RH(CE, CSN, "BattleBotTeam__Tx");

void setup() {
	Serial.begin(115200);
	// initialize the nRF24l01 module and open a link to BattleBotTeam__Rx
	if(!RH.begin("BattleBotTeam__Rx")){
		while(1);
	}
	RH.toggleTransferMode(true);	// Set the nRF24l01 to transmit mode.

	Serial.println("starting listening");
}

void loop() {
	// create a RF_PACKAGE packet.
	RF_PACKAGE packet;
	// this sets the package_header to 0x10000028
	packet.Package_Header = packet_ID + RF_DATA_SIZE;
	for(int i = 0; i < RF_DATA_SIZE; i++){
		// this sets the data to the value of i.
		packet.RF_DATA[i] = i;
	}
	// This transmits the packet to the receiver
	RH.transmitPacket(&packet);
}