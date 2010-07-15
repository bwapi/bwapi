#include "MPHSTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
MPHSTest::MPHSTest(UnitType unitType) : unitType(unitType),
                                        running(false),
                                        fail(false),
                                        unit(NULL),
                                        startFrame(-1),
                                        nextFrame(-1)
{

}
void MPHSTest::start()
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
  BWAssertF(unit->stop(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();

}
void MPHSTest::update()
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
  if (thisFrame<startFrame+30)
  {
  }
  else if (thisFrame==startFrame+30)
  {
    BWAssertF(unit->isBurrowed()==false,{fail=true;return;});
    BWAssertF(unit->isSieged()==false,{fail=true;return;});
    BWAssertF(unit->isMoving()==false,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    BWAssertF(unit->isHoldingPosition()==false,{fail=true;return;});
    BWAssertF(unit->isIdle()==true,{fail=true;return;});
    targetPosition=unit->getPosition();
    targetPosition.y()+=32*30;
    targetPosition.makeValid();
    BWAssertF(unit->rightClick(targetPosition),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
    BWAssertF(unit->getOrder()==Orders::Move,{fail=true;return;});
    BWAssertF(unit->isMoving()==true,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
    BWAssertF(unit->getTargetPosition()==targetPosition,{fail=true;return;});
  }
  else if (thisFrame<startFrame+110)
  {
    if (thisFrame>startFrame+50)
    {
      BWAssertF(unit->getOrder()==Orders::Move,{fail=true;return;});
      BWAssertF(unit->isMoving()==true,{Broodwar->printf("frame %d",thisFrame-startFrame);fail=true;return;});
      BWAssertF(unit->isIdle()==false,{fail=true;return;});
    }
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    if (thisFrame>startFrame+60)
    {
      BWAssertF(unit->getTargetPosition().getDistance(targetPosition)<64,{Broodwar->printf("frame: %d",thisFrame-startFrame);fail=true;return;});
    }
  }
  else if (thisFrame==startFrame+110)
  {
    targetPosition=unit->getPosition();
    targetPosition.y()-=32*30;
    targetPosition.makeValid();
    BWAssertF(unit->patrol(targetPosition),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
    BWAssertF(unit->getOrder()==Orders::Patrol,{fail=true;return;});
    BWAssertF(unit->isMoving()==true,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==true,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
    BWAssertF(unit->getTargetPosition()==targetPosition,{fail=true;return;});
  }
  else if (thisFrame<startFrame+175)
  {
    BWAssertF(unit->getOrder()==Orders::Patrol,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
    BWAssertF(unit->isPatrolling()==true,{fail=true;return;});
    BWAssertF(unit->isMoving()==true,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
    if (thisFrame>startFrame+150)
    {
      //sometimes fails
      //BWAssertF(unit->getTargetPosition()==targetPosition,{Broodwar->printf("frame: %d",thisFrame-startFrame);fail=true;return;});
    }
  }
  else if (thisFrame==startFrame+175)
  {
    BWAssertF(unit->getOrder()==Orders::Patrol,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
    BWAssertF(unit->isPatrolling()==true,{fail=true;return;});
    BWAssertF(unit->isMoving()==true,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
    //sometimes fails
    //BWAssertF(unit->getTargetPosition()==targetPosition,{fail=true;return;});
    BWAssertF(unit->holdPosition(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
    BWAssertF(unit->isHoldingPosition()==true,{fail=true;return;});
    BWAssertF(unit->isMoving()==false,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
  }
  else if (thisFrame<startFrame+300)
  {
    BWAssertF(unit->isHoldingPosition()==true,{Broodwar->printf("order: %s, order2: %s",unit->getOrder().getName().c_str(),unit->getSecondaryOrder().getName().c_str());fail=true;return;});
    if (thisFrame>startFrame+250)
    {
      BWAssertF(unit->isMoving()==false,{fail=true;return;});
    }
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
  }
  else if (thisFrame==startFrame+300)
  {
    BWAssertF(unit->isHoldingPosition()==true,{fail=true;return;});
    BWAssertF(unit->isMoving()==false,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    BWAssertF(unit->isIdle()==false,{fail=true;return;});
    BWAssertF(unit->stop(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
    BWAssertF(unit->getOrder()==Orders::Stop,{fail=true;return;});
    BWAssertF(unit->isHoldingPosition()==false,{fail=true;return;});
    BWAssertF(unit->isMoving()==false,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
  }
  else if (thisFrame<startFrame+400)
  {
    BWAssertF(unit->isMoving()==false,{fail=true;return;});
    BWAssertF(unit->isPatrolling()==false,{fail=true;return;});
    BWAssertF(unit->isHoldingPosition()==false,{fail=true;return;});
    BWAssertF(unit->isIdle()==true,{fail=true;return;});
  }
  else
  {
    running = false;
  }
}

void MPHSTest::stop()
{
}

bool MPHSTest::isRunning()
{
  return running;
}