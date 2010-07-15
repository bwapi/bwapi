#include "AttackUnitTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
AttackUnitTest::AttackUnitTest(UnitType unitType, UnitType targetType) : unitType(unitType),
                                                                         running(false),
                                                                         fail(false),
                                                                         unit(NULL),
                                                                         startFrame(-1),
                                                                         nextFrame(-1),
                                                                         targetType(targetType),
                                                                         target(NULL),
                                                                         startingAttackFrame(-1)
{
}
void AttackUnitTest::start()
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
  startingHPS = target->getHitPoints() + target->getShields();
  BWAssertF(unit->isIdle()==true,{fail=true;return;});
  BWAssertF(unit->attackUnit(target),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
  BWAssertF(unit->getOrder()==Orders::AttackUnit,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
  BWAssertF(unit->getTarget()==target,{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();
  startingAttackFrame = -1;
  reachedDamagePointFrame = -1;
  stopped = false;
  initialScarabCount = unit->getScarabCount();
}
void AttackUnitTest::update()
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
  if (unit->exists())
    Broodwar->setScreenPosition(unit->getPosition().x()-320,unit->getPosition().y()-240);
  else
    Broodwar->setScreenPosition(target->getPosition().x()-320,target->getPosition().y()-240);

  if (reachedDamagePointFrame==-1 &&
      thisFrame>startFrame+100 &&
      startingAttackFrame!=-1 &&
    (target->getHitPoints()+target->getShields()<startingHPS-50 || (unit->getType().groundWeapon()==WeaponTypes::None &&
                                                                    unit->getType().airWeapon()==WeaponTypes::None &&
                                                                    unit->getType()!=UnitTypes::Protoss_Carrier &&
                                                                    unit->getType()!=UnitTypes::Protoss_Reaver)))
    reachedDamagePointFrame = thisFrame;
  if (reachedDamagePointFrame==-1 && unit->exists()==false)
  {
    BWAssertF(unitType==UnitTypes::Zerg_Scourge || unitType==UnitTypes::Zerg_Infested_Terran,{fail=true;return;});
    running = false;
    return;
  }
  if (reachedDamagePointFrame==-1)
  {
    BWAssertF(unit->getOrder()==Orders::AttackUnit,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
    BWAssertF(unit->getTarget()==target || unit->getOrderTarget()==target,{fail=true;return;});
    if ((unit->exists()==false || unit->isStartingAttack() || unit->getScarabCount()<initialScarabCount || (int)unit->getLoadedUnits().size()<unit->getInterceptorCount()) && startingAttackFrame==-1)
    {
      startingAttackFrame=thisFrame;
    }
    if (thisFrame>startFrame+2000)
    {
      BWAssertF(false,{Broodwar->printf("Unit failed to attack target");fail=true;return;});
    }
  }
  else
  {
    if (unit->getType().groundWeapon()!=WeaponTypes::None ||
        unit->getType().airWeapon()!=WeaponTypes::None ||
        unit->getType()==UnitTypes::Protoss_Carrier ||
        unit->getType()==UnitTypes::Protoss_Reaver)
    {
      BWAssertF(target->getHitPoints()+target->getShields()<startingHPS,{fail=true;return;});
    }
    if (stopped==false)
    {
      stopped=true;
      BWAssertF(unit->stop(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
    }
    if (thisFrame==reachedDamagePointFrame+50)
    {
      if (unitType==UnitTypes::Protoss_Carrier)
      {
        BWAssertF(unit->stop(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
      }
    }
    if (thisFrame>reachedDamagePointFrame+50)
    {
      BWAssertF(unit->isIdle()==true,{Broodwar->printf("%s",unit->getOrder().getName().c_str());fail=true;return;});
    }
    if (thisFrame>reachedDamagePointFrame+100)
    {
      running = false;
    }
  }
}

void AttackUnitTest::stop()
{
}

bool AttackUnitTest::isRunning()
{
  return running;
}