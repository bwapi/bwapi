#include "AttackMoveTest.h"
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
AttackMoveTest::AttackMoveTest(UnitType unitType) : unitType(unitType),
                                                    unit(NULL),
                                                    startFrame(-1),
                                                    nextFrame(-1),
                                                    targetPosition(Positions::None)
{
  fail = false;
  running = false;
}
void AttackMoveTest::start()
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
  FAILTEST(unit->isIdle()==true);
  targetPosition = unit->getPosition();
  targetPosition.x()+=32*30;
  targetPosition.makeValid();
  FAILTEST(unit->attackMove(targetPosition));
  FAILTEST(unit->getOrder()==Orders::AttackMove);
  FAILTEST(unit->getTargetPosition()==targetPosition);
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();
  doneFrame = -1;
}
void AttackMoveTest::update()
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

  if (thisFrame<startFrame+100 || unit->isIdle()==false)
  {
    FAILTEST(unit->getOrder()==Orders::AttackMove);
    if (thisFrame>startFrame+60)
    {
      BWAssertF(unit->getTargetPosition().getDistance(targetPosition)<128,{Broodwar->printf("(%d,%d) != (%d,%d)",unit->getTargetPosition().x(),unit->getTargetPosition().y(),targetPosition.x(),targetPosition.y());fail=true;return;});
    }
  }
  else
  {
    FAILTEST(unit->isIdle() || unit->isBraking());
    BWAssertF(unit->getPosition().getDistance(targetPosition)<128,Broodwar->printf("(%d,%d) != (%d,%d)",unit->getPosition().x(),unit->getPosition().y(),targetPosition.x(),targetPosition.y());{fail=true;return;});
    if (doneFrame==-1)
    {
      doneFrame=thisFrame;
    }
    if (thisFrame>doneFrame+100)
    {
      running = false;
    }
  }
}

void AttackMoveTest::stop()
{
}
