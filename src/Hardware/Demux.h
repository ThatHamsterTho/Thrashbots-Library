/* 
 	* Demux.h - Header for handling the CD4052BM multiplexer
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#pragma once
#include "pins.h"

typedef unsigned char byte;

enum CSN {
  NONE = 0,
  MPU = 1,
  RF  = 2,
  LCD = 3
};

/* Demux is master of the SPI CS line
 * Demux is implemented as a static class to make sure that data 
 * will only be transmitted when other data is finished transmitting
*/
class Demux {
private:
  typedef union {
    byte val;
    struct {
      byte A    : 1;
      byte B    : 1;
      byte    : 6;
    };
  } DemuxStruct;

  static DemuxStruct SelectedChip;

public:

  Demux() { Set(CSN::NONE); }

  static void Set(CSN chip);
  static void Clear(){Set(CSN::NONE);}
};
