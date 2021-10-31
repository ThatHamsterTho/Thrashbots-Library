/* 
 	* Joysticks.cpp - Library for handling the two joysticks on the PCB
	* Created by Imre Korf, 31 Oktober 2021
	* Released into the public domain
*/

#include "Joysticks.h"
#include <Arduino.h>

int Joystick::operator[](unsigned int index){
  if(index < 2)   return analogRead(Pins[index]);
  else      return 0.0f;
}

int Joystick::getHorizontal(void){return analogRead(Pins[Axis::Hor]);}
int Joystick::getVertical(void){return analogRead(Pins[Axis::Ver]);}