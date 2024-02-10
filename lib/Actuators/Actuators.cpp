#include <Arduino.h>
#include <Servo.h>
#include <DataStructures.h>
#include <Actuators.h>
#include <ActuatorsConfig.h>
#include <Logging.h>

Actuators::Actuators():
    rodServo(),
    grabberHeightServo(),
    grabberOpeningServo()
{
    pushersServos[0] = Servo();
    pushersServos[1] = Servo();
};

void Actuators::setup() {
    pinMode(MAGNETS_PIN, OUTPUT);

    grabberHeightServo.attach(GRABBER_HEIGHT_SERVO_PIN); // Pin
    grabberHeightServo.write(0);

    grabberOpeningServo.attach(GRABBER_OPENING_SERVO_PIN); // Pin
    grabberOpeningServo.write(0);

    pushersServos[0].attach(LEFT_PUSHER_SERVO_PIN); // Pin
    pushersServos[0].write(160);

    pushersServos[1].attach(RIGHT_PUSHER_SERVO_PIN); // Pin
    pushersServos[1].write(30);
}

void Actuators::reset() {
    grabberHeightServo.write(0);
    grabberOpeningServo.write(0);
    this->setPusherL(0);
    this->setPusherR(0);
    rodServo.write(0);
    digitalWrite(MAGNETS_PIN, LOW);
}

void Actuators::update(RadioData *newData, RadioData *currentData) {
// if (newData->grabberHeight != currentData->grabberHeight) {
//     grabberHeightServo.write(map(newData->grabberHeight, 0, 255, 0, 180));
//   }

//   if (newData->grabberOpeningAngle != currentData->grabberOpeningAngle) {
//     grabberOpeningServo.write(map(newData->grabberOpeningAngle, 0, 255, 0, 180));
//   }

//   if (newData->isRodDeployed != currentData->isRodDeployed) {
//     rodServo.write(newData->isRodDeployed ? 180 : 0);
//   }

  if (newData->isLeftPusherDeployed != currentData->isLeftPusherDeployed) {
    this->setPusherL(newData->isLeftPusherDeployed ? 180 : 0);
  }

  if (newData->isRightPusherDeployed != currentData->isRightPusherDeployed) {
    debug("Changing right pusher");
    this->setPusherR(newData->isRightPusherDeployed ? 180 : 0);
  }

//   if (newData->areMagnetsEnabled != currentData->areMagnetsEnabled) {
//     digitalWrite(magnetsPin, newData->areMagnetsEnabled ? HIGH : LOW);
//   }
}

void Actuators::setPusherL(byte angle) {
  pushersServos[0].write(map(angle, 0, 180, 0, 150));
}

void Actuators::setPusherR(byte angle) {
  pushersServos[1].write(map(180 - angle, 0, 180, 30, 180));
}