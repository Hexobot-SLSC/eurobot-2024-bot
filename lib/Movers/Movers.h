#ifndef __MOVERS_H__
#define __MOVERS_H__

#include <Arduino.h>
#include <Remote.h>
#include <DataStructures.h>
#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>

class Movers {
    private:
        SoftwareSerial sabertoothRightSerial;
        SoftwareSerial sabertoothLeftSerial;
        SabertoothSimplified sabertoothRight;
        SabertoothSimplified sabertoothLeft;

    public:
        Movers();

        void setup();

        void drive(JoystickData *joystickData);
        void stop();

        void setFR(int value);
        void setFL(int value);
        void setBR(int value);
        void setBL(int value);
};

#endif