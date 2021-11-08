/* 
 	* NHDLCD.h - Header for handeling the NHDLCD on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#pragma once
#include <inttypes.h>

/** @addtogroup GLCD_GROUP
 * @{
 */
/** Max characters over a row */
#define GLCD_CHAR_COLS 	  18 	
/** Amount of rows on the LCD */
#define GLCD_CHAR_ROWS     4 	

/** Width of the LCD in pixels */
#define GLCD_COLS		 128 	
/** Height of the LCD in rows */
#define GLCD_ROWS		   4 	

/** Designates if the Column value is in pixels */
#define PIXEL_COL		   0 	
/** Designates if the Column value is in characters */
#define CHAR_COL		   1 	
/** @} */

/**
 * @brief GLCD class
 * @details class used to interface with the NHDLCD
 */
class GLCD {
public:

	/**
	 * @brief Construct a new GLCD object
	 */
	GLCD();

	/**
	 * @brief Destroy the GLCD object
	 */
	~GLCD();
	
	/**
	 * @brief Initializes the NHDLCD for use
	 */
	void begin();

	// ====================== basic functions ====================== //

	/** 
	 * @brief clears the display.
	 * @details clears the display by setting all bytes to 0x00 in display ram.
	 * @remark note: if the clear function is called frequently the screen may start.
	 * to flikker, to prevent this use clearLine to clear the line you want to write to.
	 * You can also get the current cursor position through the getCursor function.
	 * If you want to clear the current line of the cursor use clearLine();
	 * @sa clearLine()
	 */
	void clear();

	/**
	 * @brief Clears the given line.
	 * @remark if the current line needs to be cleared don't pass anything.
	 * @param line the line to be cleared leave empty to clear the current line.
	 * @sa clear()
	 */
	void clearLine(uint8_t line = 0xFF);

	/**
	 * @brief Sets the cursor back to home position.
	 * @details set the cursor back to home position 0,0 or top-left of the screen.
	 * @sa setCursor()
	 */
	void home();
	
	/**
	 * @brief Set the Cursor position.
	 * @param x the X coordinate of the cursor.
	 * @param y the Y coordinate of the cursor.
	 * @param pixel_cor specifies if the X coordinate is in characters or pixels.
	 * @sa getCursor()
	 */
	void setCursor(uint8_t x, uint8_t y, bool pixel_cor = CHAR_COL); 
	
	/**
	 * @brief Get the Cursor Position.
	 * 
	 * @param C_X X position of the cursor.
	 * @param C_Y Y position of the cursor.
	 * @param pixel_cor specifies if the X coordinate is in characters or pixels.
	 * @sa setCursor()
	 */
	void getCursor(uint8_t& C_X, uint8_t& C_Y, bool pixel_cor = CHAR_COL);

	/**
	 * @brief prints the value to the LCD.
	 * @details prints the string to the LCD from the current cursor position.
	 * @param value the data to write to the screen.
	 * @return the amount of written characters.
	 * @sa println()
	 * @sa writeChar()
	 * @sa setCursor()
	 * @sa getCursor()
	 */
	template<typename T>
	int print(T value){
		return print(String(value));
	}

	/**
	 * @brief prints the string to the LCD.
	 * @details prints the string to the LCD from the current cursor position.
	 * @param s the string to write to the screen.
	 * @return the amount of written characters.
	 * @sa println()
	 * @sa writeChar()
	 * @sa setCursor()
	 * @sa getCursor()
	 */
	int print(String s);

	/**
	 * @brief prints the value to the LCD followed by a new line.
	 * @details prints the value to the LCD followed by a new line from the current cursor position.
	 * @param value the data to write to the screen.
	 * @return the amount of written characters.
	 * @sa print()
	 * @sa writeChar()
	 * @sa setCursor()
	 * @sa getCursor()
	 */
	template<typename T>
	int println(T value){
		int i = print(String(value)); setCursor(0, C_POS[1]+1);
		return i;
	}

	/**
	 * @brief writes the given character to display.
	 * @details writes the given character to display from current cursor position.
	 * @remark you can add special character support (ascii 0 to 31) in the library some characters are already supported!
	 * @sa print()
	 * @sa println()
	 * @sa setCursor()
	 * @sa getCursor()
	 * @sa handleSpecialChar()
	 */
	void writeChar(char c);

	// ====================== graphic functions ====================== //

	/**
	 * @brief writes a custom character to the display.
	 * @details writes the given 5 bytes to the screen with a blank column on the left and right side of the character.
	 * @param character a 5 byte array which indicates the pixels to be on(1) or off(0).
	 * @sa writeChar()
	 */
	void writeCustomChar(uint8_t character[5]);
	
	/**
	 * @brief writes a custom block to the display
	 * @details writes the given 7 bytes to the screen.
	 * @param character a 5 byte array which indicates the pixels to be on(1) or off(0).
	 * @sa writeChar()
	 */
	void writeCustomBlock(uint8_t character[7]);
	
	// displays a picture. 
	// lcdstring should be 512 bytes. (4 rows of 8 bits, 128 columns);
	/**
	 * @brief Writes a picture to the screen
	 * @details Writes an array of bytes to the screen. array should occupy 512 bytes.
	 * There are 4 rows of 128 bytes. Each byte has 8 pixels.
	 * @param lcd_string the byte array that contains the picture.
	 */
	void DispPic(unsigned char *lcd_string);

	/**
	 * @brief displays the character set in flash memory. (debug function)
	 */
	void DispCharSet();

	/**
	 * @brief disables writing to the display
	 * @sa display()
	 */
	void noDisplay();

	/**
	 * @brief enables writing to the display
	 * @sa noDisplay()
	 */
	void display();

	/**
	 * @brief sends a byte as a command to the LCD
	 * @param comm the byte of data. 
	 * @sa write()
	 */
	void command(uint8_t comm);

	/**
	 * @brief sends a byte as data to the LCD
	 * @param byte the byte of data. 
	 * @sa command()
	 */
	void write(uint8_t byte);

private:
	/**
	 * @brief sends a byte to the screen.
	 * @param byte the byte to be send to the LCD
	 * @param command boolean value LOW is a command, HIGH is data.
	 * @sa command()
	 * @sa write()
	 */
	void send(uint8_t byte, uint8_t command);

	// handles special ascii characters ( 0 to 31 )
	/**
	 * @brief Handles the special ascii characters
	 * @details Handles the special (0-31) ascii characters
	 * @param c the special character to be handled
	 * @remark for more functionality you can add your own code to handle specific characters.
	 * @sa writeChar()
	 */
	void handleSpecialChar(char c);

	/**
	 * @brief enum for tracked cursor identifier C_POS
	 * @sa C_POS
	 */
	enum {
		XP,
		YP
	};
	

	uint8_t C_POS[2] = {0, 0}; /* << keeps track of the cursor position. X is between 0 and 127 */

	
};

/**
 * @example BimTi_NHDLCD/NHDLCD_display/NHDLCD_display.ino
 * this is an example that shows how to display text onto the NHDLCD display.
 * It starts in the setup by calling the GLCD::begin() method. This initializes the NHDLCD screen.
 * In the loop it uses the GLCD::home() method to reset the cursor position to the top left.
 * Afterwards it clears the screen by using the GLCD::clear() method.
 * Then it prints the given texts to the NHDLCD by using the GLCD::println() and GLCD::print() methods.
 * These methods can accept a wide variety of types, as long as they can be made into a string.
 */

/**
 * @example BimTi_NHDLCD/Display_Controller_Values/Display_Controller_Values.ino
 * This is an example which uses the GLCD and CTRL classes. It reads the controller input and then displays it onto the NHDLCD display
 * In the setup the GLCD::begin() method is called to initialize the NHDLCD display.
 * In the loop the cursor is set back to top left by calling GLCD::home().
 * Afterwards the line that will be written to is cleared and written to.
 * The values are gathered by calling CTRL::getPot(), CTRL::getJoystick(), CTRL::getSwitch() and CTRL::getBumper().
 * GLCD::clearLine() is used instead of GLCD::clear() to counteract flickering.
 */