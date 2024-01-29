#include <TestSequence.h>
#include <Logging.h>
#include <Movers.h>

void run_test(Movers *movers) {
  debug("Going forward");

  movers->setFR(40);
  movers->setFL(40);
  movers->setBR(40);
  movers->setBL(40);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Going backwards");

  movers->setFR(-40);
  movers->setFL(-40);
  movers->setBR(-40);
  movers->setBL(-40);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Turn left");

  movers->setFR(-40);
  movers->setFL(-40);
  movers->setBR(40);
  movers->setBL(40);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Turn right");

  movers->setFR(40);
  movers->setFL(40);
  movers->setBR(-40);
  movers->setBL(-40);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(2000);

  debug("Pan right");

  movers->setFR(-90);
  movers->setFL(90);
  movers->setBR(-90);
  movers->setBL(90);

  delay(2500);
  
  debug("Pan left");

  movers->setFR(90);
  movers->setFL(-90);
  movers->setBR(90);
  movers->setBL(-90);

  delay(2500);

  debug("Stop");

  movers->stop();

  delay(8000);

  log("Test ended");
}

