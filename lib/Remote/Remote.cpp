#include <Arduino.h>
#include <Remote.h>
#include <DataStructures.h>

Remote::Remote(): radio(9, 53) {};

void Remote::setup() {
    const byte readingAddress[6] = "912CR";

    radio.begin();

    radio.setPALevel(RF24_PA_MIN);
    radio.openReadingPipe(0, readingAddress); // address

    radio.flush_rx();
}

void Remote::fetch(RadioData *dataBuffer) {
    radio.read(&dataBuffer, sizeof(&dataBuffer));
}

void Remote::print_details() {
    radio.printDetails();
}

bool Remote::hasData() {
    return radio.available();
}
