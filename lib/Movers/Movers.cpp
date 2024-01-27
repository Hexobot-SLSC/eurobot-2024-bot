#include <Movers.h>
#include <Arduino.h>
#include <SabertoothSimplified.h>
#include <DataStructures.h>
#include <SoftwareSerial.h>

Movers::Movers():
    sabertoothRightSerial(NOT_A_PIN, 16),
    sabertoothLeftSerial(NOT_A_PIN, 18),
    sabertoothRight(sabertoothRightSerial),
    sabertoothLeft(sabertoothLeftSerial) {}

void Movers::setup() {
    sabertoothLeftSerial.begin(9600);
    sabertoothRightSerial.begin(9600);
}

void Movers::drive(JoystickData *joystickData) {
    short theta = degrees(atan2(joystickData->holonomY, joystickData->holonomX));
    short thetaCos = cos(theta - PI / 4);
    short thetaSin = sin(theta - PI / 4);
    short maxValue = max(abs(thetaCos), abs(thetaSin));

    short turnValue = map(joystickData->x, 0, 255, -40, 40);

    sabertoothLeft.motor(1, thetaCos / maxValue * 100 + turnValue);
    sabertoothLeft.motor(2, thetaSin / maxValue * 100 - turnValue); 
    sabertoothRight.motor(1, thetaCos / maxValue * 100 + turnValue);
    sabertoothRight.motor(2, thetaSin / maxValue * 100 - turnValue);
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