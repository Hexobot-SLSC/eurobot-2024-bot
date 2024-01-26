#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include <printf.h>

// USER DEFINED //

/* Define wether to enable info logs or not. Add // before #define to disable. */
#define INFO_STATE

/* Define wether to enable debug logs or not. Add // before #define to disable. */
#define DEBUG_STATE

/* Enable or disable test sequence. /!\ MUST BE DISABLED BEFORE NORMAL USAGE /!\ */
#define TEST_MODE

// END USER DEFINED //

#ifdef INFO_STATE
#define log(msg) \
Serial.print("[INFO] : "); \
Serial.println(msg);
#else
#define log(msg)
#endif

#ifdef DEBUG_STATE
#define debug(msg) \
Serial.print("[DEBUG] : "); \
Serial.println(msg);
#else
#define debug(msg)
#endif

#define DEFAULT_SCORE 40
#define ELECTRO_MAGNETS_COUNT 3

const byte electroMagnetPins[ELECTRO_MAGNETS_COUNT] = {3, 4, 5};

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

SoftwareSerial sabertoothRightSerial(NOT_A_PIN, 16); // RX, TX
SoftwareSerial sabertoothLeftSerial(NOT_A_PIN, 18); // RX, TX
SabertoothSimplified sabertoothRight(sabertoothRightSerial);
SabertoothSimplified sabertoothLeft(sabertoothLeftSerial);

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
void testLoop();

void setup() {
  // Setup Serial

  Serial.begin(9600);
  printf_begin();
  sabertoothLeftSerial.begin(9600);
  sabertoothRightSerial.begin(9600);

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

    pushersServos[0].attach(6); // Pin
    pushersServos[0].write(0);

    pushersServos[1].attach(7); // Pin
    pushersServos[1].write(0);

    for (byte i = 0; i < ELECTRO_MAGNETS_COUNT; ++i) digitalWrite(electroMagnetPins[i], LOW);
  };

  Serial.begin(9600);
  printf_begin();

  log("Setup in progress...");

  setupScoreDisplay();
  debug("Display setup completed");
  
  setupComponents();
  debug("Components setup completed");

  setupRadio();
  debug("Radio setup completed");

  radio.flush_rx();

  log("Robot ready!");

  #ifdef DEBUG_STATE
  radio.printDetails();
  #endif

  #ifdef TEST_MODE
  while(true) testLoop();
  #endif
}

void testLoop() {
  log("Test started!");

  // for (char i = -127; i < 127; ++i) {
  //   sabertoothLeft.motor(1, i);
  //   delay(10);
  // }
  // for (char i = -127; i < 127; ++i) {
  //   sabertoothLeft.motor(2, i);
  //   delay(40);
  // }
  // for (char i = -127; i < 127; ++i) {
  //   sabertoothRight.motor(1, i);
  //   delay(40);
  // }
  // for (char i = -127; i < 127; ++i) {
  //   sabertoothRight.motor(2, i);
  //   delay(40);
  // }

  debug("Going forward");

  sabertoothLeft.motor(1, 40);
  sabertoothLeft.motor(2, 40);
  sabertoothRight.motor(1, 40);
  sabertoothRight.motor(2, 40);

  delay(2500);

  debug("Stop");

  sabertoothLeft.stop();
  sabertoothRight.stop();

  delay(2000);

  debug("Going backwards");

  sabertoothLeft.motor(1, -40);
  sabertoothLeft.motor(2, -40);
  sabertoothRight.motor(1, -40);
  sabertoothRight.motor(2, -40);

  delay(2500);

  debug("Stop");

  sabertoothLeft.stop();
  sabertoothRight.stop();

  delay(2000);

  debug("Turn left");

  sabertoothLeft.motor(1, -40);
  sabertoothLeft.motor(2, -40);
  sabertoothRight.motor(1, 40);
  sabertoothRight.motor(2, 40);

  delay(2500);

  debug("Stop");

  sabertoothLeft.stop();
  sabertoothRight.stop();

  delay(2000);

  debug("Turn right");

  sabertoothLeft.motor(1, 40);
  sabertoothLeft.motor(2, 40);
  sabertoothRight.motor(1, -40);
  sabertoothRight.motor(2, -40);

  delay(2500);

  debug("Stop")

  sabertoothLeft.stop();
  sabertoothRight.stop();

  delay(2000);

  debug("Pan right");

  sabertoothLeft.motor(1, -90);
  sabertoothLeft.motor(2, 90);
  sabertoothRight.motor(1, -90);
  sabertoothRight.motor(2, 90);

  delay(2500);
  
  debug("Pan left");

  sabertoothLeft.motor(1, 40);
  sabertoothLeft.motor(2, -40);
  sabertoothRight.motor(1, 40);
  sabertoothRight.motor(2, -40);

  delay(2500);

  debug("Stop");

  debug("Stop");

  delay(8000);

  log("Test ended");

  // sabertoothLeft.stop();
  // sabertoothRight.stop();

  // pushersServos[0].write(0);
  // pushersServos[1].write(180);

  // delay(1000);

  // pushersServos[0].write(180);
  // pushersServos[1].write(0);

  // // for (int i = 100; i > 0; --i) {
  // //   sabertoothLeft.motor(0, i);
  // //   sabertoothLeft.motor(1, i);
  // //   sabertoothRight.motor(0, i);
  // //   sabertoothRight.motor(1, i);
  // //   delay(40);
  // // }

  // sabertoothLeft.stop();
  // sabertoothRight.stop();

  // Serial.println("Test completed!");
  // delay(5000);
}

void loop() {
  delay(20);

  if (!(radio.available())) return;

  RadioData receivedData;

  getData(&receivedData);
  applyDataUpdates(&receivedData);

  Serial.println(receivedData.areMagnetsEnabled);

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

  short turnValue = map(newData->joystickData.x, 0, 255, -40, 40);

  sabertoothLeft.motor(0, thetaCos / maxValue * 100 + turnValue);
  sabertoothLeft.motor(1, thetaSin / maxValue * 100 - turnValue); 
  sabertoothRight.motor(0, thetaCos / maxValue * 100 + turnValue);
  sabertoothRight.motor(1, thetaSin / maxValue * 100 - turnValue);
}