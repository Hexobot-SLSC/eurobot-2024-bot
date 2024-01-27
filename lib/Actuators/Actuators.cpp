#include <Arduino.h>
#include <Servo.h>
#include <DataStructures.h>
#include <Actuators.h>

Actuators::Actuators():
    rodServo(),
    grabberHeightServo(),
    grabberOpeningServo()
{
    magnetsPin = 23;
    pushersServos[0] = Servo();
    pushersServos[1] = Servo();
};

void Actuators::setup() {
    pinMode(magnetsPin, OUTPUT);

    grabberHeightServo.attach(10); // Pin
    grabberHeightServo.write(0);

    grabberOpeningServo.attach(11); // Pin
    grabberOpeningServo.write(0);

    pushersServos[0].attach(6); // Pin
    pushersServos[0].write(0);

    pushersServos[1].attach(7); // Pin
    pushersServos[1].write(0);
}

void Actuators::update(RadioData *newData, RadioData *currentData) {
if (newData->grabberHeight != currentData->grabberHeight) {
    grabberHeightServo.write(map(newData->grabberHeight, 0, 255, 0, 180));
  }

  if (newData->grabberOpeningAngle != currentData->grabberOpeningAngle) {
    grabberOpeningServo.write(map(newData->grabberOpeningAngle, 0, 255, 0, 180));
  }

  if (newData->isRodDeployed != currentData->isRodDeployed) {
    rodServo.write(newData->isRodDeployed ? 180 : 0);
  }

  if (newData->isLeftPusherDeployed != currentData->isLeftPusherDeployed) {
    pushersServos[0].write(newData->isLeftPusherDeployed ? 180 : 0);
  }

  if (newData->isRightPusherDeployed != currentData->isRightPusherDeployed) {
    pushersServos[1].write(newData->isRightPusherDeployed ? 180 : 0);
  }

  if (newData->areMagnetsEnabled != currentData->areMagnetsEnabled) {
    digitalWrite(magnetsPin, newData->areMagnetsEnabled ? HIGH : LOW);
  }
}
