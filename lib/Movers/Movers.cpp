#include <Movers.h>
#include <Arduino.h>
#include <SabertoothSimplified.h>
#include <DataStructures.h>
#include <SoftwareSerial.h>
#include <Logging.h>
#include <Config.h>

const char MOTORS_POWER (MOVERS_POWER_PRCT / 100.0 * 127);

Movers::Movers():
    sabertoothRightSerial(NOT_A_PIN, 18),
    sabertoothLeftSerial(NOT_A_PIN, 16),
    sabertoothRight(sabertoothRightSerial),
    sabertoothLeft(sabertoothLeftSerial) {}

void Movers::setup() {
    sabertoothLeftSerial.begin(9600);
    sabertoothRightSerial.begin(9600);
}

void Movers::drive(JoystickData *joystickData) {
    char speed = map(joystickData->holonomY, 0, 255, -MOTORS_POWER, MOTORS_POWER);
    char turn = -map(joystickData->x, 0, 255, -MOTORS_POWER, MOTORS_POWER);
    char strafe = map(joystickData->holonomX, 0, 255, -MOTORS_POWER, MOTORS_POWER);

    this->setFR((speed - turn * 0.8 - strafe * 1.45) / 2);
    this->setFL((speed + turn * 0.8 + strafe * 1.45) / 2);
    this->setBR((speed - turn * 0.8 + strafe * 1.45) / 2);
    this->setBL((speed + turn * 0.8 - strafe * 1.45) / 2);
}


void Movers::stop() {
    sabertoothLeft.stop();
    sabertoothRight.stop();
}

void Movers::setFR(int value) {
    sabertoothRight.motor(2, value);
}

void Movers::setFL(int value) {
    sabertoothLeft.motor(1, value);
}

void Movers::setBR(int value) {
    sabertoothRight.motor(1, value);
}

void Movers::setBL(int value) {
    sabertoothLeft.motor(2, value);
}