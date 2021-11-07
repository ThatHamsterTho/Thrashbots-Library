/* 
 	* Demux.cpp - Library for handling the CD4052BM multiplexer
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/
#include "BimTiDemux.h"
#include <Arduino.h>

// assign memory to the SC member
Demux::DemuxStruct Demux::SelectedChip;

void Demux::Set(CSN chip){
	if(SelectedChip.val == chip){ // chip is already set
		#ifdef SERIAL_DEBUG
		if(chip != CSN::NONE)
			Serial.println((String)("Chip already selected: " + (String)chip));
		#endif
		return;
	}
	// unless multi-threading is used or two consecutive Set calss are made
	// this situation should never be reached.
	while(SelectedChip.val != CSN::NONE && chip != CSN::NONE){
		// stay in while loop until Selected chip is none
		#ifdef SERIAL_DEBUG
		Serial.println((String)("Trying to select chip: " + (String)chip + \
		" while chip: " + (String)(SelectedChip.val) + " was selected.")); 
		delay(100);
		#endif
	}
	SelectedChip.val = (int)chip;
	digitalWrite(CS_A, SelectedChip.A);
	digitalWrite(CS_B, SelectedChip.B);
}
