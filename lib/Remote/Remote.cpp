#include <Arduino.h>
#include <Remote.h>
#include <DataStructures.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Logging.h>
#include <Config.h>

Remote::Remote(): radio(RADIO_CE, RADIO_CSN) {};

void Remote::setup() {
    const byte readingAddress[6] = "912CR";
    
    radio.begin();
    radio.setChannel(110);
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_2MBPS);
    radio.openReadingPipe(0, readingAddress); // address

    radio.startListening();
}

void Remote::fetch(RadioData *dataBuffer) {
    radio.read(dataBuffer, sizeof(*dataBuffer));
}

void Remote::printDetails() {
    radio.printDetails();
}

void Remote::restart() {
    radio.stopListening();
    radio.startListening();
}

bool Remote::hasData() {
    return radio.available();
}
