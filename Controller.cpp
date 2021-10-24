#include "Controller.h"
#include <Arduino.h>

Controller::Controller() : Joys{Joystick(J_HL, J_VL), Joystick(J_HR, J_VR)} {}
Controller::~Controller() {}

int Controller::GetSwitch(Identifier Id){ return digitalRead(SW_L + Id); }
int Controller::GetBumper(Identifier Id){ return digitalRead(BMPL + Id); }
int Controller::GetPot   (Identifier Id){ return analogRead (POTL - Id); }
