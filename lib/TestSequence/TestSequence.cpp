#include <TestSequence.h>
#include <Logging.h>
#include <Movers.h>
#include <Actuators.h>

void run_test(Movers movers, Actuators actuators) {
  debug("Going forward");

  movers.setFR(127);
  movers.setFL(127);
  movers.setBR(127);
  movers.setBL(127);

  delay(2500);

  debug("Stop");

  movers.stop();

  delay(2000);

  debug("Going backwards");

  movers.setFR(-127);
  movers.setFL(-127);
  movers.setBR(-127);
  movers.setBL(-127);

  delay(2500);

  debug("Stop");

  movers.stop();

  delay(2000);

  debug("Turn left");

  movers.setFR(-127);
  movers.setBR(-127);
  movers.setFL(127);
  movers.setBL(127);

  delay(2500);

  debug("Stop");

  movers.stop();

  delay(2000);

  debug("Turn right");

  movers.setFR(127);
  movers.setBR(127);
  movers.setFL(-127);
  movers.setBL(-127);

  delay(2500);

  debug("Stop");

  movers.stop();

  delay(2000);

  debug("Pan right");

  movers.setFR(-127);
  movers.setFL(127);
  movers.setBR(127);
  movers.setBL(-127);

  delay(2500);
  
  debug("Pan left");

  movers.setFR(127);
  movers.setFL(-127);
  movers.setBR(-127);
  movers.setBL(127);

  delay(2500);

  debug("Stop");

  movers.stop();

  delay(2500);

  debug("Servos")
  
  actuators.setPusherL(RETRACTED);
  delay(1000);
  actuators.setPusherL(RETRACTED);
  delay(1000);

  actuators.setPusherR(DEPLOYED);
  delay(1000);
  actuators.setPusherR(DEPLOYED);
  delay(1000);
  
  delay(4000);

  log("Test ended");
}

