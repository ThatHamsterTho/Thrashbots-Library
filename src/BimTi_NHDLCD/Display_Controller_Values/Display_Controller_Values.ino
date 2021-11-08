#include <BimTi_Controller.h>
#include <BimTi_NHDLCD/NHDLCD.h>

CTRL Controller;

GLCD glcd;

/*****************************************************
*           Setup Function, to run once              *
*****************************************************/

void setup() {  
  Serial.begin(9600);
  while (!Serial) {}
  // some boards need to wait to ensure access to serial over USB

  InitControllerPins();

  glcd.begin(); 
}

/*****************************************************
*           Loop Function, to run repeatedly         *
*****************************************************/

void loop() {
	glcd.home();
    // write to screen
    glcd.clearLine(); 
    glcd.println(String(Controller.getPot(Left)) + " " + String(Controller.getPot(Right)));
    glcd.clearLine(); 
    glcd.println(String(Controller.getJoystick(Left,Hor))  + " " + String(Controller.getJoystick(Left,Ver)));
    glcd.clearLine(); 
    glcd.println(String(Controller.getJoystick(Right,Hor)) + " " + String(Controller.getJoystick(Right,Ver)));
    glcd.clearLine(); 
    glcd.println(String(Controller.getSwitch(Left)) + " " + String(Controller.getSwitch(Right)) + " | " 
               + String(Controller.getBumper(Left)) + " " + String(Controller.getBumper(Right)));
    // original was at 1000
    delay(10);
} // loop
