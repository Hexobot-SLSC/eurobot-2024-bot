#include <Arduino.h>
#include <Servo.h>
#include <DataStructures.h>
#include <Actuators.h>
#include <ActuatorsConfig.h>
#include <Logging.h>
#include <Config.h>
 
Servo Actuators::pushersServos[2];
Servo Actuators::rodServo;
Servo Actuators::grabberHeightServo;
Servo Actuators::grabberOpeningServo;

Actuators::Actuators() {};

void Actuators::setup() {
    // pinMode(MAGNETS_PIN, OUTPUT);

    // grabberHeightServo.attach(GRABBER_HEIGHT_SERVO_PIN); // Pin
    // grabberHeightServo.write(0);

    grabberOpeningServo.attach(LEFT_GRABBER_OPEN_SERVO); // Pin
    grabberOpeningServo.write(0);

    pushersServos[0].attach(LEFT_PUSHER); // Pin
    this->setPusherL(RETRACTED);

    pushersServos[1].attach(RIGHT_PUSHER); // Pin
    this->setPusherR(RETRACTED);
}

void Actuators::reset() {
    // grabberHeightServo.write(0);
    // grabberOpeningServo.write(0);

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

  grabberOpeningServo.write(map(newData.grabberOpeningAngle, 0, 255, 0, 180));
}

void Actuators::setPusherL(PusherState state) {
  pushersServos[0].write(state == DEPLOYED ? LEFT_PUSHER_DEPLOYED : LEFT_PUSHER_RETRACTED);
}

void Actuators::setPusherR(PusherState state) {
  pushersServos[1].write(state == DEPLOYED ? RIGHT_PUSHER_DEPLOYED : RIGHT_PUSHER_RETRACTED);
}