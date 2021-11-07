#define FAILURE_HANDLING
#define SERIAL_DEBUG

#include "src/BimTiController.h"
#include "src/NHDLCD/BimTi_NHDLCD.h"


//#define RECEIVER
#define TRANSMITTER

// RF24 Transmitter Code
#ifdef TRANSMITTER
  CTRL Controller("TransmitterName", true);
#else
  CTRL Controller("ReceiverName");
#endif

GLCD glcd;
RF24* radio;

/*****************************************************
*           Setup Function, to run once              *
*****************************************************/

void setup() {  
  Serial.begin(9600);
  while (!Serial) {}
  // some boards need to wait to ensure access to serial over USB

  InitControllerPins();

  glcd.begin(); 

  radio = Controller.getRadio();

  #ifdef TRANSMITTER
    bool exitcode = Controller.begin("ReceiverName", true);
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
RF_PACKAGE payload;

void loop() {
  #ifdef RECEIVER
    payload.toggles = 0;
    payload.Joystick[0][0] = 0; payload.Joystick[0][1] = 0;
    payload.Joystick[1][0] = 0; payload.Joystick[1][1] = 0;
    payload.Pot[0] = 0;         payload.Pot[1] = 0;
    if (Controller.get_RF_Package(&payload)) {   // is there a payload? get the pipe number that recieved it
      Serial.print("Header: "); Serial.println(payload.Package_Header, HEX);
      for(int i = 0; i < 28; i ++){
        Serial.print("pckt "+ String(i) + ": "); Serial.println(payload.RF_DATA[i], HEX);
      }
      /*
      Serial.println(payload.Package_Header, HEX);
      Serial.println(String(payload.Pot[Left]) + " " + String(payload.Pot[Right]));
      Serial.println(String(payload.Joystick[Left][Hor]) + " " + String(payload.Joystick[Left][Ver]));
      Serial.println(String(payload.Joystick[Right][Hor]) + " " + String(payload.Joystick[Right][Ver]));
      Serial.println(String(payload.switch_L) + " " + String(payload.switch_R) + " | " 
                + String(payload.bumper_L) + " " + String(payload.bumper_R));
      */
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
  
  
  payload.Package_Header = 0xA1A1A1A1;
  for(int i = 0; i < RF_DATA_BYTE_SIZE; i++){
    payload.RF_DATA[i] = i;
  }
  Serial.print("Header: "); Serial.println(payload.Package_Header, HEX);
  for(int i = 0; i < RF_DATA_BYTE_SIZE; i ++){
    Serial.print("pckt "+ String(i) + ": "); Serial.println(payload.RF_DATA[i], HEX);
  }
  //Controller.TransmitRF_Package(&payload);
  
  
  unsigned long start_timer = micros();                    // start the timer
  bool report = radio->write(&payload, sizeof(RF_PACKAGE));
  unsigned long end_timer = micros();                      // end the timer

  if (report) {
    Serial.print("Transmission successful! ");             // payload was delivered
    Serial.print("Time to transmit = ");
    Serial.print(end_timer - start_timer);                 // print the timer result
    Serial.println(" send " + String((uint32_t)sizeof(RF_PACKAGE)) + " bytes.");
  } else {
    Serial.println("Transmission failed or timed out"); // payload was not delivered
  }

  
  //  if(Controller.transmitCtrlData()){}
    delay(500);
    /*
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
    */
  #else

  #endif
} // loop
