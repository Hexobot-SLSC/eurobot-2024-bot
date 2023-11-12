#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Servo.h>

#define DEFAULT_SCORE 50
#define ELECTRO_MAGNETS_COUNT 3

const byte electroMagnetPins[ELECTRO_MAGNETS_COUNT] = {2, 3, 4};

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

Servo grabberHeightServo;
Servo grabberOpeningServo;
Servo rodServo;
Servo pushersServos[2];

typedef struct JoystickData {
  // Classic joystick
  byte x;
  byte y;

  // Holonom joystick
  byte holonomX;
  byte holonomY;
}JoystickData;

void setup() {
  // Setup Serial

  Serial.begin(9600);

  Serial.println("Setup in progress...");

 auto setupScoreDisplay = []() {
    const uint8_t middleBar[1] = {SEG_G};
    scoreDisplay.setBrightness(0x0f);
    scoreDisplay.setSegments(middleBar, 4, 0);
  };

  auto setupRadio = []() {
    const byte readingAddress[6] = "912CR";

    radio.begin();

    radio.setPALevel(RF24_PA_MIN);
    radio.openReadingPipe(0, readingAddress); // address
  };

  auto setupComponents = []() {
    for (unsigned char i = 0; i < 3; ++i) {
      pinMode(electroMagnetPins[i], OUTPUT);
    }

    grabberHeightServo.attach(10); // Pin
    grabberHeightServo.write(0);

    grabberOpeningServo.attach(11); // Pin
    grabberOpeningServo.write(0);

    pushersServos[0].attach(12);
    pushersServos[0].write(0);

    pushersServos[1].attach(13);
    pushersServos[1].write(0);

    for (byte i = 0; i < ELECTRO_MAGNETS_COUNT; ++i) digitalWrite(electroMagnetPins[i], LOW);
  };

  Serial.begin(9600);

  Serial.println("Setup in progress...");

  setupScoreDisplay();
  Serial.println("Display setup completed");
  
  setupComponents();
  Serial.println("Components setup completed");

  setupRadio();
  Serial.println("Radio setup completed");

  Serial.println("Robot ready!");
}

void loop() {
  if (radio.available()) {
    byte dataType;

    radio.read(&dataType, sizeof(dataType));

    switch (dataType) {
      case 'J': {
        // Joystick data

        JoystickData joystickData;

        while (!(radio.available())) delay(0.05);

        radio.read(&joystickData, sizeof(joystickData));

        // TODO: Implement joystick data
        break;
      }
      case 'S': {
        // Score data
        
        byte score;

        while (!(radio.available())) delay(0.05);

        radio.read(&score, sizeof(score));

        scoreDisplay.showNumberDec(score);
        break;
      }
      case 'H': {
        // Grabber height servo data

        byte servoAngle;

        while (!(radio.available())) delay(0.05);

        radio.read(&servoAngle, sizeof(servoAngle));

        grabberHeightServo.write(servoAngle);
        break;
      }
      case 'O': {
        // Grabber opening servo data

        byte servoAngle;

        while (!(radio.available())) delay(0.05);

        radio.read(&servoAngle, sizeof(servoAngle));

        grabberOpeningServo.write(servoAngle);
        break;
      }
      case 'R': {
        // Rod servo data

        bool isRodDeployed;

        while (!(radio.available())) delay(0.05);

        radio.read(&isRodDeployed, sizeof(isRodDeployed));

        rodServo.write((isRodDeployed && 180) || 0);
        break;
      }
      case 'M': {
      // Electro magnet data

        bool areMagnetEnabled;

        while (!(radio.available())) delay(0.05);

        radio.read(&areMagnetEnabled, sizeof(areMagnetEnabled));

        for (unsigned char i = 0; i < 3; ++i) digitalWrite(electroMagnetPins[i], areMagnetEnabled);
        break;
      }
      case 'P': {
        // Pushers servo data

        bool isEnabled;

        while (!(radio.available())) delay(0.05);

        radio.read(&isEnabled, sizeof(isEnabled));

        pushersServos[0].write((isEnabled && 180) || 0);
        pushersServos[1].write((isEnabled && 0) || 180);
        break;
      }
      default: {
        Serial.println("Unknown data type");
        Serial.print(dataType);
        break;
      }
    }
  }
}