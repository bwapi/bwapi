#include "RallyTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;

#define FAILTEST(C)\
{\
  if (!(C))\
  {\
  log("Assert failed @%s:%u %s[%s:%s] (%s)",__FILE__,__LINE__, unit ? unit->getType().getName().c_str() : "NULL", unitType.getName().c_str(), unit ? unit->getOrder().getName().c_str() : "null", Broodwar->getLastError().toString().c_str());\
    assert_fail_count++;\
    fail = true;\
    return;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}

RallyTest::RallyTest(UnitType unitType) : unitType(unitType),
                                          unit(NULL),
                                          rallyUnit(NULL),
                                          startFrame(-1),
                                          nextFrame(-1)
{
  fail = false;
  running = false;
}
void RallyTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(unitType);
  FAILTEST(userCount>=1);
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    if (u->getType()==unitType)
    {
      unit = u;
    }
    if (u->getType().isWorker())
    {
      rallyUnit = u;
    }
  }

  FAILTEST(rallyUnit!=NULL);
  rallyPosition=rallyUnit->getPosition();

  FAILTEST(unit!=NULL);
  FAILTEST(unit->exists());
  FAILTEST(unit->getType().isBuilding());
  unit->setRallyPoint(rallyUnit);
  FAILTEST(unit->getRallyUnit()==rallyUnit);
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
    FAILTEST(unit->getRallyUnit()==rallyUnit);
  }
  else if (thisFrame==startFrame+100)
  {
    FAILTEST(unit->getRallyUnit()==rallyUnit);
    unit->setRallyPoint(rallyPosition);
    FAILTEST(unit->getRallyPosition()==rallyPosition);
    FAILTEST(unit->getRallyUnit()==NULL);
  }
  else if (thisFrame<startFrame+200)
  {
    FAILTEST(unit->getRallyPosition()==rallyPosition);
    FAILTEST(unit->getRallyUnit()==NULL);
  }
  else
  {
    unit->setRallyPoint(unit);
    running=false;
  }
}

void RallyTest::stop()
{
}
