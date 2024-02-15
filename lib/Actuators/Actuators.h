#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__

#include <Servo.h>
#include <DataStructures.h>

enum PusherState {
  RETRACTED,
  DEPLOYED
};

class Actuators {
    private:
        static Servo pushersServos[2];
        static Servo rodServo;
        static Servo grabberHeightServo;
        static Servo grabberOpeningServo;
    public:
        Actuators();

        void setup();
        void setPusherL(PusherState state);
        void setPusherR(PusherState state);
        void reset();
        void update(RadioData radioData, RadioData currentData);
};

#endif