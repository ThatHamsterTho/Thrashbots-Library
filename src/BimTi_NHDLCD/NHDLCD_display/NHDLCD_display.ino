#include <BimTi_Controller.h>
#include <BimTi_NHDLCD/NHDLCD.h>

GLCD glcd;

/*****************************************************
*           Setup Function, to run once              *
*****************************************************/

void setup() {  
  	Serial.begin(9600);
	while (!Serial) {}
	// some boards need to wait to ensure access to serial over USB

	InitControllerPins();

	// Initialize the display
	glcd.begin();
}

/*****************************************************
*           Loop Function, to run repeatedly         *
*****************************************************/

void loop() {
	glcd.home(); // set the cursor back to (0,0) or top left.

	glcd.clear(); // clear the screen.
	
	/* prints:
		Hello world!
		The value of X is: 25!

	*/

	glcd.println("Hello world!");
	glcd.println("The value of X is: ");
	int x = 25;
	glcd.print(x);
	glcd.println("!");

	// delay to reduce screen flikkering.
	delay(500);
} // loop
