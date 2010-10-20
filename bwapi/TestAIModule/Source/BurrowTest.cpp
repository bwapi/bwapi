#include "BurrowTest.h"
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
BurrowTest::BurrowTest(UnitType unitType) : unitType(unitType),
                                            unit(NULL),
                                            startFrame(-1),
                                            nextFrame(-1),
                                            completedBurrow(false)
{
  fail = false;
  running = false;
}
void BurrowTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(unitType);
  FAILTEST(userCount>=1);
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==unitType)
      unit = u;

  FAILTEST(unit!=NULL);
  FAILTEST(unit->exists());
  FAILTEST(unit->isBurrowed()==false);
  unit->burrow();
  FAILTEST(unit->getOrder()==Orders::Burrowing);
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
    if (unit->getOrder()!=Orders::Burrowing && thisFrame>startFrame+60)
    {
      FAILTEST(unit->isBurrowed()==true);
      completedBurrow=true;
      unit->unburrow();
      FAILTEST(unit->getOrder()==Orders::Unburrowing);
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
    if (unit->getOrder()!=Orders::Unburrowing && thisFrame>startFrame+60)
    {
      FAILTEST(unit->isBurrowed()==false);
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
