#include "Joysticks.h"
#include <Arduino.h>

int Joystick::operator[](unsigned int index){
  if(index < 2)   return analogRead(Pins[index]);
  else      return 0.0f;
}

int Joystick::getHorizontal(void){return analogRead(Pins[Axis::H]);}
int Joystick::getVertical(void){return analogRead(Pins[Axis::V]);}
