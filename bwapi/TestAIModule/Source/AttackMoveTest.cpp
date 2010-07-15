#include "AttackMoveTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
AttackMoveTest::AttackMoveTest(UnitType unitType) : unitType(unitType),
                                                    running(false),
                                                    fail(false),
                                                    unit(NULL),
                                                    startFrame(-1),
                                                    nextFrame(-1),
                                                    targetPosition(Positions::None)
{
}
void AttackMoveTest::start()
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
  BWAssertF(unit->isIdle()==true,{fail=true;return;});
  targetPosition = unit->getPosition();
  targetPosition.x()+=32*20;
  BWAssertF(unit->attackMove(targetPosition),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
  BWAssertF(unit->getOrder()==Orders::AttackMove,{fail=true;return;});
  BWAssertF(unit->getTargetPosition()==targetPosition,{fail=true;return;});
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
    BWAssertF(unit->getOrder()==Orders::AttackMove,{fail=true;return;});
    if (thisFrame>startFrame+60)
    {
      BWAssertF(unit->getTargetPosition().getDistance(targetPosition)<64,{Broodwar->printf("(%d,%d) != (%d,%d)",unit->getTargetPosition().x(),unit->getTargetPosition().y(),targetPosition.x(),targetPosition.y());fail=true;return;});
    }
  }
  else
  {
    BWAssertF(unit->isIdle() || unit->isBraking(),{fail=true;return;});
    BWAssertF(unit->getPosition().getDistance(targetPosition)<96,Broodwar->printf("(%d,%d) != (%d,%d)",unit->getPosition().x(),unit->getPosition().y(),targetPosition.x(),targetPosition.y());{fail=true;return;});
    if (doneFrame==-1)
      doneFrame=thisFrame;
    if (thisFrame>doneFrame+100)
    {
      running = false;
    }
  }
}

void AttackMoveTest::stop()
{
}

bool AttackMoveTest::isRunning()
{
  return running;
}