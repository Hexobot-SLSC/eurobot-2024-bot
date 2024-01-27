#ifndef Remote_h
#define Remote_h

#include <Arduino.h>
#include <RF24.h>

class Remote {
    public:
        Remote();

        void setup();
        bool hasData();
        void fetch(RadioData *dataBuffer);
        void print_details();
    
    private:
        RF24 radio;
};

#endif