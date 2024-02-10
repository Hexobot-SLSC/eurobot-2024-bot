#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__

#include <Arduino.h>
#include <Servo.h>
#include <DataStructures.h>

class Actuators {
    private:
        Servo pushersServos[2];
        Servo rodServo;
        Servo grabberHeightServo;
        Servo grabberOpeningServo;
    public:
        Actuators();

        void setup();
        void setPusherL(byte angle);
        void setPusherR(byte angle);
        void reset();
        void update(RadioData *radioData, RadioData *currentData);
};

#endif