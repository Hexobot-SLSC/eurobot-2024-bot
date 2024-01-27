// DataStructures.h
#ifndef DataStructures_h
#define DataStructures_h

#include <Arduino.h>

struct JoystickData {
  // Classic joystick
  byte x;
  byte y;

  // Holonom joystick
  byte holonomX;
  byte holonomY;
};

struct RadioData {
  JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpeningAngle;

  byte score;

  bool areMagnetsEnabled;
  bool isRodDeployed;
  bool isRightPusherDeployed;
  bool isLeftPusherDeployed;
};

#endif