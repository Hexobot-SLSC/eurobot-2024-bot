#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>

#define DEFAULT_SCORE 50
#define ELECTRO_MAGNETS_COUNT 3

const byte electroMagnetPins[ELECTRO_MAGNETS_COUNT] = {2, 3, 4};

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

SoftwareSerial sabertoothRightSerial(NOT_A_PIN, 18); // RX, TX
SabertoothSimplified sabertoothRight(sabertoothRightSerial);
SabertoothSimplified sabertoothLeft;

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
} JoystickData;

typedef struct RadioData {
  struct JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpeningAngle;

  byte score;

  bool areMagnetsEnabled;
  bool isRodDeployed;
  bool isRightPusherDeployed;
  bool isLeftPusherDeployed;
} RadioData;

RadioData lastReceivedData = {
  .joystickData = {
    .x = 0,
    .y = 0,
    .holonomX = 0,
    .holonomY = 0
  },
  .grabberHeight = 0,
  .grabberOpeningAngle = 0,
  .score = 0,
  .areMagnetsEnabled = false,
  .isRodDeployed = false,
  .isRightPusherDeployed = false,
  .isLeftPusherDeployed = false
};

void getData(RadioData *dataBuffer);
void applyDataUpdates(RadioData *newData);
void updateMotors(RadioData *newData);
bool areJoysticksDifferent(JoystickData *first, JoystickData *second);

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

    pushersServos[0].attach(12); // Pin
    pushersServos[0].write(0);

    pushersServos[1].attach(13); // Pin
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

  radio.flush_rx();

  Serial.println("Robot ready!");
}

void loop() {
  delay(20);

  if (!(radio.available())) return;

  RadioData receivedData;

  getData(&receivedData);
  applyDataUpdates(&receivedData);

  lastReceivedData = receivedData;
}

void getData(RadioData *dataBuffer) {
  radio.read(&dataBuffer, sizeof(&dataBuffer));
}

void applyDataUpdates(RadioData *newData) {
  if (newData->score != lastReceivedData.score) {
    scoreDisplay.showNumberDec(newData->score);
  }

  if (newData->grabberHeight != lastReceivedData.grabberHeight) {
    grabberHeightServo.write(map(newData->grabberHeight, 0, 255, 0, 180));
  }

  if (newData->grabberOpeningAngle != lastReceivedData.grabberOpeningAngle) {
    grabberOpeningServo.write(map(newData->grabberOpeningAngle, 0, 255, 0, 180));
  }

  if (newData->isRodDeployed != lastReceivedData.isRodDeployed) {
    rodServo.write(newData->isRodDeployed ? 180 : 0);
  }

  if (newData->isLeftPusherDeployed != lastReceivedData.isLeftPusherDeployed) {
    pushersServos[0].write(newData->isLeftPusherDeployed ? 180 : 0);
  }

  if (newData->isRightPusherDeployed != lastReceivedData.isRightPusherDeployed) {
    pushersServos[1].write(newData->isRightPusherDeployed ? 180 : 0);
  }

  if (newData->areMagnetsEnabled != lastReceivedData.areMagnetsEnabled) {
    for (byte i = 0; i < ELECTRO_MAGNETS_COUNT; ++i) {
      digitalWrite(electroMagnetPins[i], newData->areMagnetsEnabled ? HIGH : LOW);
    }
  }

  if (!(areJoysticksDifferent(&newData->joystickData, &lastReceivedData.joystickData))) return;

  updateMotors(newData);
}

bool areJoysticksDifferent(JoystickData *first, JoystickData *second) {
  return first->x != second->x || first->y != second->y || first->holonomX != second->holonomX || first->holonomY != second->holonomY;
}

void updateMotors(RadioData *newData) {
  short theta = degrees(atan2(newData->joystickData.holonomY, newData->joystickData.holonomX));
  short thetaCos = cos(theta - PI / 4);
  short thetaSin = sin(theta - PI / 4);
  short maxValue = max(abs(thetaCos), abs(thetaSin));

  short turnValue = map(newData->joystickData.x, 0, 255, -25, 25);

  sabertoothLeft.motor(0, thetaCos / maxValue * 100 + turnValue);
  sabertoothLeft.motor(1, thetaSin / maxValue * 100 - turnValue); 
  sabertoothRight.motor(0, thetaCos / maxValue * 100 + turnValue);
  sabertoothRight.motor(1, thetaSin / maxValue * 100 - turnValue);
}