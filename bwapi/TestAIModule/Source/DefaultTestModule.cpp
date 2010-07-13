#include "DefaultTestModule.h"
using namespace std;
using namespace BWAPI;
void DefaultTestModule::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->sendText("show me the money");
  Broodwar->sendText("operation cwal");
}