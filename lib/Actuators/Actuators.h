#ifndef Actuators_h
#define Actuators_h

#include <Arduino.h>
#include <Servo.h>
#include <DataStructures.h>

class Actuators {
    private:
        byte magnetsPin;
        Servo pushersServos[2];
        Servo rodServo;
        Servo grabberHeightServo;
        Servo grabberOpeningServo;
    public:
        Actuators();

        void setup();
        void update(RadioData *radioData, RadioData *currentData);
};

#endif