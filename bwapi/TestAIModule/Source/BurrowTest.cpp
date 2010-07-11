#include "BurrowTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
BurrowTest::BurrowTest(UnitType unitType) : unitType(unitType),
                                           running(false),
                                           fail(false),
                                           unit(NULL),
                                           startFrame(-1),
                                           nextFrame(-1),
                                           completedBurrow(false)
{

}
void BurrowTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(unitType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==unitType)
      unit = u;

  BWAssertF(unit!=NULL,{fail=true;return;});
  BWAssertF(unit->exists(),{fail=true;return;});
  BWAssertF(unit->isBurrowed()==false,{fail=true;return;});
  unit->burrow();
  BWAssertF(unit->getOrder()==Orders::Burrow,{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();

}
void BurrowTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextFrame);
  nextFrame++;
  Broodwar->setScreenPosition(unit->getPosition().x()-320,unit->getPosition().y()-240);

  if (completedBurrow==false)
  {
    if (unit->getOrder()!=Orders::Burrow && thisFrame>startFrame+60)
    {
      BWAssertF(unit->isBurrowed()==true,{fail=true;return;});
      completedBurrow=true;
      unit->unburrow();
      BWAssertF(unit->getOrder()==Orders::Unburrow,{fail=true;return;});
      startFrame=thisFrame;
    }
    else
    {
      if (thisFrame>startFrame+300)
        fail = true;
    }
  }
  else
  {
    if (unit->getOrder()!=Orders::Unburrow && thisFrame>startFrame+60)
    {
      BWAssertF(unit->isBurrowed()==false,{fail=true;return;});
      running = false;
    }
    else
    {
      if (thisFrame>startFrame+300)
        fail = true;
    }
  }
}

void BurrowTest::stop()
{
}

bool BurrowTest::isRunning()
{
  return running;
}