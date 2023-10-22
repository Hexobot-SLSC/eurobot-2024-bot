#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>

#define DEFAULT_SCORE 50

const uint64_t readingAddress = 2024134UL;
const uint64_t writingAddress = 314592654UL;

TM1637Display display(12, 13); // CLK, DIO
RF24 radio(9, 53); // CE, CSN

struct JOYSTICK_DATA {
  // First joystick
  unsigned short x;
  unsigned short y;

  // Second joystick
  unsigned short omniX;
};

void setup() {
  // Setup Serial

  Serial.begin(9600);

  Serial.println("Setup in progress...");

  const uint8_t middleBar[1] = {SEG_G};
  display.setSegments(middleBar, 4, 0);

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
      unsigned char data[2];

      radio.read(&data, sizeof(data));

      if (data[1] == 255 && data[2] == 255) {
        Serial.println("Controller connected!");
        display.showNumberDec(DEFAULT_SCORE, false, 4, 0);
        break;
      }
    }
  }

  Serial.println("Bot ready!");
}

void loop() {
  if (radio.available()) {
    unsigned char data[4];

    radio.read(&data, sizeof(data));

    if (data[1] == 'J') {
      JOYSTICK_DATA joystickData;

      joystickData.x = short(data[2]);
      joystickData.y = short(data[3]);
      joystickData.omniX = short(data[4]);

    } else if (data[1] == 'S') {
      short score = short(data[2]);
    }
  }
}