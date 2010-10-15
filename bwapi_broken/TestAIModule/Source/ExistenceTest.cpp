#include "ExistenceTest.h"
using namespace std;
using namespace BWAPI;
void ExistenceTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->sendText("show me the money");
}
void ExistenceTest::onFrame()
{
  for each(Unit* u in Broodwar->getAllUnits())
  {
    Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y(),"%x",u->getID());
  }
}
void ExistenceTest::onUnitCreate(Unit* unit)
{
  Broodwar->printf("Unit %d [%s] has been created",unit->getID(), unit->getType().getName().c_str());
}
void ExistenceTest::onUnitDestroy(Unit* unit)
{
  Broodwar->printf("Unit %d [%s] has been destroyed",unit->getID(), unit->getType().getName().c_str());
}