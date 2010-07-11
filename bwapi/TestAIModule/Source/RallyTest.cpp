#include "RallyTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
RallyTest::RallyTest(UnitType unitType) : unitType(unitType),
                                          running(false),
                                          fail(false),
                                          unit(NULL),
                                          startFrame(-1),
                                          nextFrame(-1)
{

}
void RallyTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(unitType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==unitType)
      unit = u;
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType().isWorker())
      rallyUnit = u;

  rallyPosition=rallyUnit->getPosition();

  BWAssertF(unit!=NULL,{fail=true;return;});
  BWAssertF(unit->exists(),{fail=true;return;});
  BWAssertF(unit->getType().isBuilding(),{fail=true;return;});
  unit->setRallyUnit(rallyUnit);
  BWAssertF(unit->getRallyUnit()==rallyUnit,{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();

}
void RallyTest::update()
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

  if (thisFrame<startFrame+100)
  {
    BWAssertF(unit->getRallyUnit()==rallyUnit,{fail=true;return;});
  }
  else if (thisFrame==startFrame+100)
  {
    BWAssertF(unit->getRallyUnit()==rallyUnit,{fail=true;return;});
    unit->setRallyPosition(rallyPosition);
    BWAssertF(unit->getRallyPosition()==rallyPosition,{fail=true;return;});
    BWAssertF(unit->getRallyUnit()==NULL,{fail=true;return;});
  }
  else if (thisFrame<startFrame+200)
  {
    BWAssertF(unit->getRallyPosition()==rallyPosition,{fail=true;return;});
    BWAssertF(unit->getRallyUnit()==NULL,{fail=true;return;});
  }
  else
  {
    unit->setRallyUnit(unit);
    running=false;
  }
}

void RallyTest::stop()
{
}

bool RallyTest::isRunning()
{
  return running;
}