#include "RightClickTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
RightClickTest::RightClickTest(UnitType unitType, UnitType enemyType, UnitType friendlyType) : unitType(unitType),
                                            enemyType(enemyType),
                                            friendlyType(friendlyType),
                                            unit(NULL),
                                            enemyUnit(NULL),
                                            friendUnit(NULL),
                                            startFrame(-1),
                                            nextFrame(-1)
{
  fail = false;
  running = false;
}
void RightClickTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(unitType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==unitType)
      unit = u;

  int enemyCount = Broodwar->self()->completedUnitCount(enemyType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->getAllUnits())
    if (u->getType()==enemyType && u->getPlayer()->isEnemy(Broodwar->self()))
      enemyUnit = u;

  int friendCount = Broodwar->self()->completedUnitCount(friendlyType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->getAllUnits())
    if (u->getType()==friendlyType && u->getPlayer()->isAlly(Broodwar->self()))
      friendUnit = u;
  BWAssertF(unit!=NULL,{fail=true;return;});
  BWAssertF(unit->exists(),{fail=true;return;});
  BWAssertF(enemyUnit!=NULL,{fail=true;return;});
  BWAssertF(enemyUnit->exists(),{fail=true;return;});
  BWAssertF(friendUnit!=NULL,{fail=true;return;});
  BWAssertF(friendUnit->exists(),{fail=true;return;});
  targetPosition = unit->getPosition();
  targetPosition.y()-=30*32;
  targetPosition.makeValid();
  unit->rightClick(targetPosition);
  BWAssertF(unit->getOrder()==Orders::Move,{fail=true;return;});
  BWAssertF(unit->getTargetPosition()==targetPosition,{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();
}
void RightClickTest::update()
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

  if (thisFrame < startFrame + 200)
  {
    BWAssertF(unit->isMoving(),{fail=true;return;});
    BWAssertF(unit->getTargetPosition()==targetPosition,{fail=true;return;});
  }
  else if (thisFrame == startFrame + 200)
  {
    unit->rightClick(enemyUnit);
    BWAssertF(unit->getOrder()==Orders::AttackUnit,{fail=true;return;});
    BWAssertF(unit->getTarget() == enemyUnit || unit->getOrderTarget() == enemyUnit,{fail=true;return;});
  }
  else if (thisFrame < startFrame + 400)
  {
    BWAssertF(unit->getOrder()==Orders::AttackUnit,{fail=true;return;});
    BWAssertF(unit->getTarget() == enemyUnit || unit->getOrderTarget() == enemyUnit,{fail=true;return;});
  }
  else if (thisFrame == startFrame + 400)
  {
    unit->rightClick(friendUnit);
    BWAssertF(unit->getOrder()==Orders::Follow,{fail=true;return;});
    BWAssertF(unit->getTarget() == friendUnit || unit->getOrderTarget() == friendUnit,{fail=true;return;});
  }
  else if (thisFrame < startFrame + 600)
  {
    BWAssertF(unit->getOrder()==Orders::Follow,{fail=true;return;});
    BWAssertF(unit->getTarget() == friendUnit || unit->getOrderTarget() == friendUnit,{fail=true;return;});
  }
  else if (thisFrame == startFrame + 600)
  {
    unit->stop();
  }
  else if (thisFrame < startFrame + 650)
  {
  }
  else
  {
    running = false;
  }
}

void RightClickTest::stop()
{
}
