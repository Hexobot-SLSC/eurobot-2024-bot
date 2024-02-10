#include <TestSequence.h>
#include <Logging.h>
#include <Movers.h>
#include <Actuators.h>

void run_test(Movers *movers, Actuators *actuators) {
  debug("Going forward");

  movers->setFR(50);
  movers->setFL(50);
  movers->setBR(50);
  movers->setBL(50);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Going backwards");

  movers->setFR(-50);
  movers->setFL(-50);
  movers->setBR(-50);
  movers->setBL(-50);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Turn left");

  movers->setFR(-50);
  movers->setBR(-50);
  movers->setFL(50);
  movers->setBL(50);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Turn right");

  movers->setFR(50);
  movers->setBR(50);
  movers->setFL(-50);
  movers->setBL(-50);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Pan right");

  movers->setFR(-90);
  movers->setFL(90);
  movers->setBR(90);
  movers->setBL(-90);

  delay(2500);
  
  debug("Pan left");

  movers->setFR(90);
  movers->setFL(-90);
  movers->setBR(-90);
  movers->setBL(90);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2500);

  debug("Servos")
  
  actuators->setPusherL(30);
  delay(1000);
  actuators->setPusherL(160);
  delay(1000);

  actuators->setPusherR(30);
  delay(1000);
  actuators->setPusherR(160);
  delay(1000);
  
  delay(4000);

  log("Test ended");
}

