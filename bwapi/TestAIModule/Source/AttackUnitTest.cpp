#include "AttackUnitTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
#define FAILTEST(C)\
{\
  if (!(C))\
  {\
  log("Assert failed @%s:%u %s[%s:%s]->%s[%s] (%s)",__FILE__,__LINE__, unit ? unit->getType().getName().c_str() : "NULL", unitType.getName().c_str(), unit ? unit->getOrder().getName().c_str() : "null", target ? target->getType().getName().c_str() : "NULL", targetType.getName().c_str(), Broodwar->getLastError().toString().c_str());\
    assert_fail_count++;\
    fail = true;\
    return;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}
AttackUnitTest::AttackUnitTest(UnitType unitType, UnitType targetType) : unitType(unitType),
                                                                         unit(NULL),
                                                                         startFrame(-1),
                                                                         nextFrame(-1),
                                                                         targetType(targetType),
                                                                         target(NULL),
                                                                         startingAttackFrame(-1)
{
  running = false;
  fail = false;
}
void AttackUnitTest::start()
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

  int targetCount = Broodwar->self()->completedUnitCount(targetType);
  FAILTEST(targetCount>=1);
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==targetType)
      target = u;
  FAILTEST(target!=NULL);
  FAILTEST(target->exists());
  startingHPS = target->getHitPoints() + target->getShields();
  FAILTEST(unit->isIdle()==true);
  FAILTEST(unit->attack(target));
  FAILTEST(unit->getOrder()==Orders::AttackUnit);
  FAILTEST(unit->getTarget()==target);
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
  {
    Broodwar->setScreenPosition(unit->getPosition().x()-320,unit->getPosition().y()-240);
  }
  else
  {
    Broodwar->setScreenPosition(target->getPosition().x()-320,target->getPosition().y()-240);
  }

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
    FAILTEST(unitType==UnitTypes::Zerg_Scourge || unitType==UnitTypes::Zerg_Infested_Terran);
    running = false;
    return;
  }
  if (reachedDamagePointFrame==-1)
  {
    FAILTEST(unit->getOrder()==Orders::AttackUnit);
    FAILTEST(unit->getTarget()==target || unit->getOrderTarget()==target);
    if ((unit->exists()==false || unit->isStartingAttack() || unit->getScarabCount()<initialScarabCount || (int)unit->getLoadedUnits().size()<unit->getInterceptorCount()) && startingAttackFrame==-1)
    {
      startingAttackFrame=thisFrame;
    }
    if (thisFrame>startFrame+2000)
    {
      BWAssertF(false,{log("Unit failed to attack target");fail=true;return;});
    }
  }
  else
  {
    if (unit->getType().groundWeapon()!=WeaponTypes::None ||
        unit->getType().airWeapon()!=WeaponTypes::None ||
        unit->getType()==UnitTypes::Protoss_Carrier ||
        unit->getType()==UnitTypes::Protoss_Reaver)
    {
      FAILTEST(target->getHitPoints()+target->getShields()<startingHPS);
    }

    if (stopped==false)
    {
      stopped=true;
      FAILTEST(unit->stop());
    }
    if (thisFrame==reachedDamagePointFrame+50)
    {
      if (unitType==UnitTypes::Protoss_Carrier)
      {
        FAILTEST(unit->stop());
      }
    }
    if (thisFrame>reachedDamagePointFrame+50)
    {
      FAILTEST(unit->isIdle()==true);
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
