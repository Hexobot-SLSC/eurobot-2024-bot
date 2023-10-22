#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

const uint64_t readingAddress = 314592654UL;
const uint64_t writingAddress = 314592654UL;

RF24 radio(9, 53); // CE, CSN

void setup() {
  // Setup Serial

  Serial.begin(9600);

  Serial.println("Setup in progress...");

  // Setup radio

  radio.begin();

  radio.setChannel(99);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, readingAddress); // address
  radio.openWritingPipe(writingAddress); // address

  Serial.println("Radio started");

  Serial.println("Setup complete!");

  // Waiting for controller to connect

  Serial.println("Waiting for controller to connect...");

  radio.startListening();

  while (true) {
    delay(1000);
    if (radio.available()) {
      Serial.println("Controller connected!");
      break;
    }
  }

  Serial.println("Bot ready!");
}

void loop() {
  
}