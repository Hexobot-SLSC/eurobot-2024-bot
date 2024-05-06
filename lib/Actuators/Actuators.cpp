#include <Arduino.h>
#include <Servo.h>
#include <DataStructures.h>
#include <Actuators.h>
#include <ActuatorsConfig.h>
#include <Logging.h>
#include <Config.h>

#define LEFT 0
#define RIGHT 1
 
Servo Actuators::pushersServos[2];
Servo Actuators::grabberHeightServo[2];
Servo Actuators::grabberOpeningServo[2];

Actuators::Actuators() {};

void Actuators::setup() {
    // pinMode(MAGNETS_PIN, OUTPUT);

    grabberHeightServo[LEFT].attach(LEFT_GRABBER_HEIGHT_SERVO); // Pin
    grabberHeightServo[LEFT].write(0);
    
    grabberHeightServo[RIGHT].attach(RIGHT_GRABBER_HEIGHT_SERVO); // Pin
    grabberHeightServo[RIGHT].write(180);

    pushersServos[LEFT].attach(LEFT_PUSHER); // Pin
    this->setPusherL(RETRACTED);

    pushersServos[RIGHT].attach(RIGHT_PUSHER); // Pin
    this->setPusherR(RETRACTED);
}

void Actuators::reset() {
    grabberHeightServo[LEFT].write(0);
    grabberHeightServo[RIGHT].write(180);

    this->setPusherL(RETRACTED);
    this->setPusherR(RETRACTED);

    //digitalWrite(MAGNETS_PIN, LOW);
}

void Actuators::update(RadioData newData, RadioData currentData) {

  if (newData.isLeftPusherDeployed != currentData.isLeftPusherDeployed) {
    this->setPusherL(newData.isLeftPusherDeployed ? DEPLOYED : RETRACTED);
  }

  if (newData.isRightPusherDeployed != currentData.isRightPusherDeployed) {
    this->setPusherR(newData.isRightPusherDeployed ? DEPLOYED : RETRACTED);
  }

  byte height = map(newData.grabberOpeningAngle, 0, 255, 0, 180);
  grabberHeightServo[LEFT].write(height);
  grabberHeightServo[RIGHT].write(180 - height);
}

void Actuators::setPusherL(PusherState state) {
  pushersServos[LEFT].write(state == DEPLOYED ? LEFT_PUSHER_DEPLOYED : LEFT_PUSHER_RETRACTED);
}

void Actuators::setPusherR(PusherState state) {
  pushersServos[RIGHT].write(state == DEPLOYED ? RIGHT_PUSHER_DEPLOYED : RIGHT_PUSHER_RETRACTED);
}