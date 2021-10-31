#define FAILURE_HANDLING
//#define SERIAL_DEBUG

#include "src/Controller.h"
#include "src/NHDLCD/NHDLCD.h"


//#define RECEIVER
#define TRANSMITTER

// RF24 Transmitter Code
#ifdef TRANSMITTER
  CTRL Controller("TransmitterName");
#else
  CTRL Controller("ReceiverName");
#endif

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

  #ifdef TRANSMITTER
    bool exitcode = Controller.begin("ReceiverName");
  #elif defined(RECEIVER)
    bool exitcode = Controller.begin("TransmitterName", true);
  #endif

  #if defined(TRANSMITTER) || defined(RECEIVER)
  if(!exitcode){
    while(1);
  }
  #endif
}

/*****************************************************
*           Loop Function, to run repeatedly         *
*****************************************************/

void loop() {
  #ifdef RECEIVER
    payloadDT payload;
    if (Controller.get_RF_Package(&payload)) {   // is there a payload? get the pipe number that recieved it
      //Serial.println("Received: ");
      //Serial.println(String(payload.Pot[Left]) + " " + String(payload.Pot[Right])); // print the payload's value
      //Serial.println(String(payload.Joystick[Left][Hor]) + " " + String(payload.Joystick[Left][Ver])); 
      //                "   " + String(payload.Joystick[Right][Hor]) + " " + String(payload.Joystick[Right][Ver]))); 
      //Serial.println(String(payload.switch_L) + " " + String(payload.switch_R)); 
      //Serial.println(String(payload.bumper_L) + " " + String(payload.bumper_R)); 
    }
    // write to screen
    glcd.home();
    uint8_t cursor[2];
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(payload.Pot[Left]) + " " + String(payload.Pot[Right]));
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(payload.Joystick[Left][Hor]) + " " + String(payload.Joystick[Left][Ver]));
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(payload.Joystick[Right][Hor]) + " " + String(payload.Joystick[Right][Ver]));
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(payload.switch_L) + " " + String(payload.switch_R) + " | " 
               + String(payload.bumper_L) + " " + String(payload.bumper_R));
    delay(20);
  #elif (defined (TRANSMITTER))
    Controller.TransmitCtrlData();

    glcd.home();
    // write to screen
    uint8_t cursor[2];
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(Controller.getPot(Left)) + " " + String(Controller.getPot(Right)));
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(Controller.getJoystick(Left,Hor))  + " " + String(Controller.getJoystick(Left,Ver)));
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(Controller.getJoystick(Right,Hor)) + " " + String(Controller.getJoystick(Right,Ver)));
    glcd.clearLine((glcd.getCursorPos(&cursor[0], &cursor[1]), cursor[1])); 
    glcd.println(String(Controller.getSwitch(Left)) + " " + String(Controller.getSwitch(Right)) + " | " 
               + String(Controller.getBumper(Left)) + " " + String(Controller.getBumper(Right)));
    // original was at 1000
    delay(10);
  #else

  #endif
} // loop
