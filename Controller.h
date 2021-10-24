#pragma once
#include "pins.h"
#include "Joysticks.h"
#include "Demux.h"

class Controller {
	Joystick Joys[2];

public:
	enum Identifier {
		Left = 0,
		Right = 1,
	};

  Controller();
  ~Controller();

	int GetSwitch(Identifier Id);
	int GetBumper(Identifier Id);
	int GetPot   (Identifier Id);
	int GetJoystick(Identifier Id, Joystick::Axis A){ return Joys[Id][A]; }
};
