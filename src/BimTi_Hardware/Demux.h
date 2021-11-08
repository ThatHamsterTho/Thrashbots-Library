/* 
 	* Demux.h - Header for handling the CD4052BM multiplexer
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#pragma once

typedef unsigned char byte;

/**
 * @brief Enum containing the different bit values of the SPI modules.
 */
enum CSN {
  NONE = 0, /* << Select no  SPI chip. */
  MPU = 1,  /* << Select MPU SPI chip. */
  RF  = 2,  /* << Select RF  SPI chip. */
  LCD = 3   /* << Select LCD SPI chip. */
};

/**
 * @brief 
 * Demux is implemented as a static class to make sure that data.
 * will only select other chip if none is selected.
 */
class Demux {
private:
  /**
  * @brief union to set CSN value to A and B bit.
  */
  union DemuxStruct{
    byte val;         /* << value to be set to the demux pins. */
    struct {
      byte A    : 1;  /* << A bit of the demux. */
      byte B    : 1;  /* << B bit of the demux. */
      byte    : 6;
    };
  };

  static DemuxStruct SelectedChip; /* << DemuxStruct to keep track of selected SPI module. */

public:

  /**
   * @brief Construct a new Demux object.
   * 
   */
  Demux() { Set(CSN::NONE); }

  /**
   * @brief selects the SPI module.
   * 
   * @param chip the SPI module.
   */
  static void Set(CSN chip);
  /**
   * @brief clears the select SPI module.
   * @details sets the demux to NONE.
   */
  static void Clear(){Set(CSN::NONE);}
};
