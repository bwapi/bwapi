#include "FollowTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
FollowTest::FollowTest(UnitType unitType, UnitType targetType) : unitType(unitType),
                                                                 unit(NULL),
                                                                 startFrame(-1),
                                                                 nextFrame(-1),
                                                                 targetType(targetType),
                                                                 target(NULL)
{
  fail = false;
  running = false;
}
void FollowTest::start()
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

  int targetCount = Broodwar->self()->completedUnitCount(targetType);
  BWAssertF(targetCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==targetType)
      target = u;
  BWAssertF(target!=NULL,{fail=true;return;});
  BWAssertF(target->exists(),{fail=true;return;});
  BWAssertF(unit->isIdle()==true,{fail=true;return;});
  BWAssertF(unit->rightClick(Position(target->getPosition().x()-32*20,target->getPosition().y())),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();
  started=false;
}
void FollowTest::update()
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

  if (unit->getDistance(target)>32*15 && started==false)
  {
    BWAssertF(unit->follow(target),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
    BWAssertF(unit->getOrder()==Orders::Follow,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
    BWAssertF(unit->getTarget()==target,{fail=true;return;});
    started=true;
    startFrame=Broodwar->getFrameCount();
  }

  if (started)
  {
    if (thisFrame<startFrame+50)
    {
      BWAssertF(unit->getOrder()==Orders::Follow,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
      BWAssertF(unit->getTarget()==target || unit->getOrderTarget()==target,{fail=true;return;});
    }
    else if (thisFrame==startFrame+50)
    {
      BWAssertF(unit->stop(),{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
      BWAssertF(unit->isIdle()==true,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
    }
    else if (thisFrame<startFrame+100)
    {
    }
    else if (thisFrame<startFrame+200)
    {
      BWAssertF(unit->isIdle()==true,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
      BWAssertF(unit->isMoving()==false || unit->isBraking(),{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
      running = false;
    }
    else if (thisFrame==startFrame+200)
    {
      running = false;
    }
  }
}

void FollowTest::stop()
{
}
