#include "ZergTest.h"
#include "TrainTest.h"
#include "BuildTest.h"
using namespace std;
using namespace BWAPI;
void ZergTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->sendText("show me the money");
  Broodwar->setLocalSpeed(0);
}