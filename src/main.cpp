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

void applyDataChanges(RadioData *newData);
bool areJoysticksDifferent(JoystickData *first, JoystickData *second);
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

  #ifdef DEBUG_STATE
  remote.print_details();
  #endif

  #ifdef TEST_MODE
  log("Running Test Mode");
  while(true) run_test(&movers);
  #endif
}

void loop() {
  delay(HEARTBEAT_DELAY);

  if (!(remote.hasData())) return;

  RadioData receivedData;

  remote.fetch(&receivedData);

  applyDataChanges(&receivedData);

  currentData = receivedData;
}

void applyDataChanges(RadioData *newData) {
  if (newData->score != currentData.score) {
    scoreDisplay.update(newData->score);
  }

  if (!(areJoysticksDifferent(&newData->joystickData, &currentData.joystickData))) return;

  movers.drive(&newData->joystickData);
}

bool areJoysticksDifferent(JoystickData *first, JoystickData *second) {
  return first->x != second->x || first->y != second->y || first->holonomX != second->holonomX || first->holonomY != second->holonomY;
}