#include "MicroTest.h"
using namespace std;
using namespace BWAPI;
bool lastIsAttackFrame;
void MicroTest::onStart()
{
  Broodwar->enableFlag(Flag::UserInput);
}
void MicroTest::onFrame()
{
  Position goal=Broodwar->getMousePosition()+Broodwar->getScreenPosition();
  std::map<Unit*, int> targetAdjustedHP;
  std::map<Unit*, std::set<Unit*> > targetsInRange;
  std::map<Unit*, std::set<Unit*> > targetGetAttackers;
  for each(Unit* e in Broodwar->enemy()->getUnits())
  {
    targetAdjustedHP[e]=e->getHitPoints();
    for each(Unit* s in Broodwar->self()->getUnits())
    {
      if (s->isInWeaponRange(e))
      {
        targetAdjustedHP[e]-=5;
        targetsInRange[s].insert(e);
      }
    }
  }
  for each(Unit* s in Broodwar->self()->getUnits())
  {
    int minAdjHP=100000;
    Unit* localTarget = NULL;
    for each(Unit* e in targetsInRange[s])
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
  for each(Unit* s in Broodwar->self()->getUnits())
  {
    isAttackFrame = isAttackFrame || s->isAttackFrame();
    if (s->getGroundWeaponCooldown()>maxCoolDown)
      maxCoolDown = s->getGroundWeaponCooldown();
  }
  if (lastIsAttackFrame && !isAttackFrame)
  {
    Broodwar->issueCommand(Broodwar->self()->getUnits(),UnitCommand::rightClick(NULL,goal));
    for each(Unit* s in Broodwar->self()->getUnits())
    {
      Broodwar->drawLineMap(s->getPosition().x(),s->getPosition().y(),goal.x(),goal.y(),Colors::Green);
    }
  }
  else
  {
    if (maxCoolDown == 0)
    {
      for each(std::pair<Unit*,std::set<Unit*> > p in targetGetAttackers)
      {
        Unit* e = p.first;
        for each(Unit* s in p.second)
        {
          if (Broodwar->getFrameCount()-s->getLastCommandFrame()>4)
          {
            if (e)
            {
              s->attack(e);
              Broodwar->drawLineMap(s->getPosition().x(),s->getPosition().y(),e->getPosition().x(),e->getPosition().y(),Colors::Red);
            }
            else
            {
              s->rightClick(goal);
              Broodwar->drawLineMap(s->getPosition().x(),s->getPosition().y(),goal.x(),goal.y(),Colors::Green);
            }
          }
        }
      }
    }
    for each(Unit* s in Broodwar->self()->getUnits())
    {
      Broodwar->drawTextMap(s->getPosition().x(),s->getPosition().y(),"CD: %d",s->getGroundWeaponCooldown());
    }

  }
  lastIsAttackFrame=isAttackFrame;
}