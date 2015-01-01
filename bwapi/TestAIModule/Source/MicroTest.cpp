#include "MicroTest.h"
using namespace std;
using namespace BWAPI;
bool lastIsAttackFrame;
void MicroTest::onStart()
{
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->setCommandOptimizationLevel(4);
}
void MicroTest::onFrame()
{
  Position goal=Broodwar->getMousePosition()+Broodwar->getScreenPosition();
  std::map<Unit , int> targetAdjustedHP;
  std::map<Unit , Unitset > targetsInRange;
  std::map<Unit , Unitset > targetGetAttackers;
  for (Unit e : Broodwar->enemy()->getUnits())
  {
    targetAdjustedHP[e]=e->getHitPoints();
    for (Unit s : Broodwar->self()->getUnits())
    {
      if (s->isInWeaponRange(e))
      {
        targetAdjustedHP[e]-=5;
        targetsInRange[s].insert(e);
      }
    }
  }
  for (Unit s : Broodwar->self()->getUnits())
  {
    int minAdjHP=100000;
    Unit localTarget = NULL;
    for (Unit e : targetsInRange[s])
    {
      if (localTarget == NULL || targetAdjustedHP[e]<minAdjHP)
      {
        localTarget = e;
        minAdjHP = targetAdjustedHP[e];
      }
    }
    targetGetAttackers[localTarget].insert(s);
  }
  bool isAttackFrame = false;
  int maxCoolDown = 0;
  for (Unit s : Broodwar->self()->getUnits())
  {
    isAttackFrame = isAttackFrame || s->isAttackFrame();
    if (s->getGroundWeaponCooldown() > maxCoolDown)
      maxCoolDown = s->getGroundWeaponCooldown();
  }
  if (lastIsAttackFrame && !isAttackFrame)
  {
    Broodwar->issueCommand(Broodwar->self()->getUnits(),UnitCommand::rightClick(NULL,goal));
    for (Unit s : Broodwar->self()->getUnits())
    {
      Broodwar->drawLineMap(s->getPosition(), goal, Colors::Green);
    }
  }
  else
  {
    if (maxCoolDown == 0)
    {
      for (std::pair<Unit ,Unitset > p : targetGetAttackers)
      {
        Unit e = p.first;
        for (Unit s : p.second)
        {
          if (Broodwar->getFrameCount() - s->getLastCommandFrame() > 4)
          {
            if (e)
            {
              s->attack(e);
              Broodwar->drawLineMap(s->getPosition(), e->getPosition(), Colors::Red);
            }
            else
            {
              s->rightClick(goal);
              Broodwar->drawLineMap(s->getPosition(), goal, Colors::Green);
            }
          }
        }
      }
    }
    for (Unit s : Broodwar->self()->getUnits())
    {
      Broodwar->drawTextMap(s->getPosition(),"CD: %d",s->getGroundWeaponCooldown());
    }

  }
  lastIsAttackFrame=isAttackFrame;
}