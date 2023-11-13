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

typedef struct RadioData {
  struct JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpeningAngle;

  byte score;

  bool areMagnetsEnabled;
  bool isRodDeployed;
  bool isRightPusherDeployed;
  bool isLeftPusherDeployed;
}RadioData;

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

void getData(struct RadioData *dataBuffer);
void applyDataUpdates(struct RadioData *newData);

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

void getData(struct RadioData *dataBuffer) {
  radio.read(&dataBuffer, sizeof(&dataBuffer));
}

void applyDataUpdates(struct RadioData *newData) {
  if (newData->score != lastReceivedData.score) {
    scoreDisplay.showNumberDec(newData->score);
  }

  if (newData->grabberHeight != lastReceivedData.grabberHeight) {
    grabberHeightServo.write(newData->grabberHeight);
  }

  if (newData->grabberOpeningAngle != lastReceivedData.grabberOpeningAngle) {
    grabberOpeningServo.write(newData->grabberOpeningAngle);
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
}