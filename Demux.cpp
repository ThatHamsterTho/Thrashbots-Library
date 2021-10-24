#include "Demux.h"
#include <Arduino.h>

// assign memory to the SC member
Demux::DemuxStruct Demux::SelectedChip;

void Demux::Set(CSN chip){
	// unless multi-threading is used or two consecutive Set calss are made
	// this situation should never be reached.
	while(SelectedChip.val != CSN::NONE && chip != CSN::NONE){
		// stay in while loop until Selected chip is none
		Serial.println((String)("Trying to select chip: " + (String)chip + \
		" while chip: " + (String)(SelectedChip.val) + " was selected.")); 
		delay(10);
	}
	SelectedChip.val = (int)chip;
	digitalWrite(CS_A, SelectedChip.A);
	digitalWrite(CS_B, SelectedChip.B);
}
