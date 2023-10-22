#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Servo.h>

#define DEFAULT_SCORE 50

const uint64_t readingAddress = 2024134UL;
const uint64_t writingAddress = 314592654UL;

const unsigned char electroMagnetPins[3] = {
  6,
  7,
  8
};

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

  // Setup display

  const uint8_t middleBar[1] = {SEG_G};
  scoreDisplay.setBrightness(0x0f);
  scoreDisplay.setSegments(middleBar, 4, 0);

  Serial.println("Display setup completed");

  // Setup servos

  horizontalServo.attach(10); // Pin
  horizontalServo.write(0);

  leftGrabberServo.attach(11); // Pin
  leftGrabberServo.write(0);

  rightGrabberServo.attach(12); // Pin
  rightGrabberServo.write(0);

  // Setup radio

  radio.begin();

  radio.setChannel(99);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, readingAddress); // address
  radio.openWritingPipe(writingAddress); // address

  Serial.println("Radio setup completed");

  Serial.println("Initialization complete!");

  // Waiting for controller to connect

  Serial.println("Waiting for controller to connect...");

  radio.startListening();

  while (true) {
    delay(1000);
    if (radio.available()) {
      unsigned char data;

      radio.read(&data, sizeof(data));

      if (data == '!') {
        Serial.println("Controller connected!");
        scoreDisplay.showNumberDec(DEFAULT_SCORE, false, 4, 0);
        break;
      }
    }
  }

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

      joystickData.x = data[1];
      joystickData.y = data[2];
      joystickData.omniX = data[3];
      joystickData.omniY = data[4];

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

      unsigned char areMagnetEnabled;

      radio.read(&areMagnetEnabled, sizeof(areMagnetEnabled));

      for (unsigned char i = 0; i < 3; ++i) {
        digitalWrite(electroMagnetPins[i], areMagnetEnabled);
      }
    }
  }
}