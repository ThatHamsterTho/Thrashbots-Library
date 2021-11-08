#include <BimTi_Controller.h>

// RF24 Transmitter Code
// Create a CTRL Object and give it the device name BattleBotTeam__Tx
CTRL Controller("BattleBotTeam__Tx");

/*****************************************************
*           Setup Function, to run once              *
*****************************************************/

void setup() {  
	Serial.begin(9600);
	while (!Serial) {}
	// some boards need to wait to ensure access to serial over USB

	// Initialize the controller pins to input and output
	InitControllerPins();

	// initialize the nRF24l01 module and open a link to BattleBotTeam__Rx
	bool exitcode = Controller.begin("BattleBotTeam__Rx");

	// if the controller.begin function returned an error catch it here.
	if(!exitcode){
		while(1);
	}
}

/*****************************************************
*           Loop Function, to run repeatedly         *
*****************************************************/

void loop() {
	// send all the current input positions to the receiver
    Controller.transmitCtrlData();
} // loop
