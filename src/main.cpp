#include <Arduino.h>
#include <printf.h>
#include <Config.h>
#include <Logging.h>
#include <Remote.h>
#include <Movers.h>
#include <ScoreDisplay.h>
#include <Actuators.h>
#include <TestSequence.h>

Remote remote;
Movers movers;
ScoreDisplay scoreDisplay;
Actuators actuators;

RadioData currentData = {
  .joystickData = {
    .x = 0,
    .holonomX = 0,
    .holonomY = 0
  },
  // .grabberHeight = 0,
  // .grabberOpeningAngle = 0,
  // .score = 0,
  // .areMagnetsEnabled = false,
  // .isRodDeployed = false,
  // .isRightPusherDeployed = false,
  // .isLeftPusherDeployed = false
};

unsigned long lastNoDataTimestamp = 0;

void applyDataChanges(RadioData newData);
bool areJoysticksDifferent(JoystickData firstJoystick, JoystickData secondJoystick);
void testLoop();

void setup() {
  // Setup Serial

  Serial.begin(9600);
  printf_begin();

  log("Setup in progress...");

  scoreDisplay.setup();
  debug("Display setup completed");

  movers.setup();
  debug("Movers setup completed");
  
  actuators.setup();
  debug("Actuators setup completed");

  remote.setup();
  debug("Remote setup completed");

  log("Robot ready!");

  #ifdef INFO_STATE
  remote.printDetails();
  #endif

  #ifdef TEST_MODE
  log("Running Test Mode");
  while(true) run_test(movers, actuators);
  #endif
}

void loop() {
  delay(HEARTBEAT_DELAY);

  //debug("Checking for data...")

  if (!(remote.hasData())) {
    if (lastNoDataTimestamp == 0) {
      lastNoDataTimestamp = millis();
    }

    if (millis() - lastNoDataTimestamp > RADIO_TIMEOUT) {
      debug("No data received for too long. Stopping robot.");
      movers.stop();
      actuators.reset();
      remote.restart();
      lastNoDataTimestamp = 0;
    }
    return;
  }

  lastNoDataTimestamp = 0;

  RadioData receivedData;

  remote.fetch(&receivedData);

  applyDataChanges(receivedData);

  currentData = receivedData;
}

void applyDataChanges(RadioData newData) {
  // if (newData->score != currentData.score) {
  //   scoreDisplay.update(newData->score);
  // }

  actuators.update(newData, currentData);

  if (!(areJoysticksDifferent(newData.joystickData, currentData.joystickData))) return;

  movers.drive(newData.joystickData);
}

bool areJoysticksDifferent(JoystickData firstJoystick, JoystickData secondJoystick) {
  return firstJoystick.x != secondJoystick.x || firstJoystick.holonomX != secondJoystick.holonomX || firstJoystick.holonomY != secondJoystick.holonomY;
}