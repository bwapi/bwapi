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
  for (Unit u : Broodwar->getAllUnits())
  {
    Broodwar->drawTextMap(u->getPosition(),"%x",u->getID());
  }
}
void ExistenceTest::onUnitCreate(Unit unit)
{
  Broodwar << "Unit " << unit->getID() << " [" << unit->getType() << "] has been created" << std::endl;
}
void ExistenceTest::onUnitDestroy(Unit unit)
{
  Broodwar << "Unit " << unit->getID() << " [" << unit->getType() << "] has been destroyed" << std::endl;
}