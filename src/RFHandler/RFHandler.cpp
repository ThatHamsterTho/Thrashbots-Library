#include "RFHandler.h"


RFHandler::RFHandler(const uint8_t CE, const uint8_t CSN, const char RF24_trans_Addr[32], bool bypass_name_conv, RInit_Flags* Errors)
	: radio(RF24(CE, CSN)), RF_Address((uint8_t*)RF24_trans_Addr) {
	String RF24_trans_Addr_str = String(RF24_trans_Addr);
	if(RF24_trans_Addr_str == ""){
		Radio_Inittable.name_empty = true;
	}
	if(!bypass_name_conv){
		if(RF24_trans_Addr_str.substring(RF24_trans_Addr_str.length() - 4) != "__Tx" && 
		   RF24_trans_Addr_str.substring(RF24_trans_Addr_str.length() - 4) != "__Rx"){
			Radio_Inittable.w_name_tx = true;
		}
	}
	if(Errors != nullptr){
		Errors->flags = 0;
		Errors->name_empty = Radio_Inittable.name_empty;
		Errors->w_name_tx = Radio_Inittable.w_name_tx;
	}
}

RFHandler::~RFHandler() {}

bool RFHandler::begin(const char RF24_recv_addr[32], bool bypass_name_conv, RInit_Flags* Errors){
	if(!RF_initialized){
		if(Radio_Inittable.flags != 0x00){
			Serial.println("Radio not initializable:");
			if(Radio_Inittable.name_empty){
				Serial.println("Radio name is empty");
			}
			if(Radio_Inittable.w_name_tx){
				Serial.println("Name does not end in __Tx or __Rx");
			}
			if(Errors != nullptr){
				Errors->flags = 0;
				Errors->name_empty = Radio_Inittable.name_empty;
				Errors->w_name_tx = Radio_Inittable.w_name_tx;
			}
			return 0;
		}
		if(!bypass_name_conv){
			String RF24_recv_addr_str = String(RF24_recv_addr);
			if(RF24_recv_addr_str.substring(RF24_recv_addr_str.length() - 4) != "__Tx" && 
		   	   RF24_recv_addr_str.substring(RF24_recv_addr_str.length() - 4) != "__Rx"){
				Radio_Inittable.w_name_rx = true;
				Serial.println("name does not end in __Rx or __Tx");
				if(Errors != nullptr){
					Errors->flags = 0;
					Errors->name_empty = Radio_Inittable.name_empty;
					Errors->w_name_tx = Radio_Inittable.w_name_tx;
					Errors->w_name_rx = Radio_Inittable.w_name_rx;
				}
				return 0;
			}
		}
		if(!radio.begin()){
			Serial.println("radio hardware is not responding!");
			Radio_Inittable.hw_error = true;
			Errors->flags = 0;
			Errors->name_empty = Radio_Inittable.name_empty;
			Errors->w_name_tx  = Radio_Inittable.w_name_tx;
			Errors->w_name_rx  = Radio_Inittable.w_name_rx;
			Errors->hw_error   = Radio_Inittable.hw_error;
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
	 	RF_initialized = true;
	}

	// set the RX address of the TX node into a RX pipe
  	radio.openReadingPipe(1, (uint8_t*)RF24_recv_addr); // using pipe 1

	toggleReceiveMode(true);
	return 1;
}

void RFHandler::end(){
	radio.closeReadingPipe(1);
}

bool RFHandler::transmitPacket(RF_PACKAGE* payload){
	if(!ReceiveMode){
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
		Serial.print(" send " + String(sizeof(RF_PACKAGE) + " bytes."));
		
		} else {
		Serial.println("Transmission failed or timed out"); // payload was not delivered
		}
		#endif

		return report;
	}
	else{
		return 0;
	}
}

bool RFHandler::getPacket(RF_PACKAGE* buffer) {
	if(ReceiveMode){
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
	else{
		return 0;
	}
}

void RFHandler::toggleReceiveMode(bool ReceiveMode){
	if(!ReceiveMode){
		radio.stopListening();  // put radio in TX mode
	}
	else{
		radio.startListening(); // put radio in RX mode
	}
	this->ReceiveMode = ReceiveMode;
}

void RFHandler::toggleTransferMode(bool TransferMode){toggleReceiveMode(!TransferMode);}

