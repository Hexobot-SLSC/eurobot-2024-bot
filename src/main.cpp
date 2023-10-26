#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Servo.h>

#define DEFAULT_SCORE 50

const byte readingAddress[6] = "912CR";
const byte transimittingAddress[6] = "912RC";

const unsigned char electroMagnetPins[3] = {2, 3, 4};

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

Servo horizontalServo;
Servo leftGrabberServo;
Servo rightGrabberServo;

struct JOYSTICK_DATA {
  // First joystick
  char x;
  char y;

  // Second joystick
  char omniX;
  char omniY;
};

void setup() {
  // Setup Serial

  Serial.begin(9600);

  Serial.println("Setup in progress...");

 auto setupDisplay = []() {
    const uint8_t middleBar[1] = {SEG_G};
    scoreDisplay.setBrightness(0x0f);
    scoreDisplay.setSegments(middleBar, 4, 0);
  };

  auto setupRadio = []() {
    radio.begin();

    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(transimittingAddress); // address
    radio.openReadingPipe(0, readingAddress); // address
  };

  auto connectToControllerAsync = []() {
    const char connectionMessage = '!';

    while (true) {
      radio.stopListening();

      radio.write(&connectionMessage, sizeof(connectionMessage));

      radio.startListening();

      delay(1000);

      while (radio.available()) {
        char data;

        radio.read(&data, sizeof(data));
        
        if (data != '!') continue;

        return;
      };
    }
  };

  auto setupComponents = []() {
    for (unsigned char i = 0; i < 3; ++i) {
      pinMode(electroMagnetPins[i], OUTPUT);
    }

    horizontalServo.attach(10); // Pin
    horizontalServo.write(0);

    leftGrabberServo.attach(11); // Pin
    leftGrabberServo.write(0);

    rightGrabberServo.attach(12); // Pin
    rightGrabberServo.write(0);
  };

  Serial.begin(9600);

  Serial.println("Setup in progress...");

  setupDisplay();
  Serial.println("Display setup completed");

  setupRadio();
  Serial.println("Radio setup completed");

  setupComponents();
  Serial.println("Components setup completed");

  Serial.println("Initialization completed!");

  Serial.println("Attempting to connect to controller...");
  connectToControllerAsync();
  Serial.println("Controller connected!");

  scoreDisplay.showNumberDec(DEFAULT_SCORE, false, 4, 0);

  Serial.println("Robot ready!");
}

void loop() {
  if (radio.available()) {
    char dataType;

    radio.read(&dataType, sizeof(dataType));

    if (dataType == 'J') {
      // Joystick data

      short data[4];
      JOYSTICK_DATA joystickData;

      radio.read(&data, sizeof(data));

      joystickData.x = data[0];
      joystickData.y = data[1];
      joystickData.omniX = data[2];
      joystickData.omniY = data[3];

      // TODO: Implement joystick data
    } else if (dataType == 'S') {
      // Score data
      
      char score;

      radio.read(&score, sizeof(score));

      scoreDisplay.showNumberDec(score);
    } else if (dataType == 'H') {
      // Horizontal servo data

      unsigned char servoAngle;

      radio.read(&servoAngle, sizeof(servoAngle));
    } else if (dataType == 'L') {
      // Left grabber servo data

      unsigned char servoAngle;

      radio.read(&servoAngle, sizeof(servoAngle));

      leftGrabberServo.write(servoAngle);
    } else if (dataType == 'R') {
      // Right grabber servo data

      unsigned char servoAngle;

      radio.read(&servoAngle, sizeof(servoAngle));

      rightGrabberServo.write(servoAngle);
    } else if (dataType == 'E') {
      // Electro magnet data

      bool areMagnetEnabled;

      radio.read(&areMagnetEnabled, sizeof(areMagnetEnabled));

      for (unsigned char i = 0; i < 3; ++i) {
        digitalWrite(electroMagnetPins[i], areMagnetEnabled);
      }
    }
  }
}