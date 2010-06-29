#include "UseTechTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
UseTechTest::UseTechTest(BWAPI::TechType techType) : techType(techType),
                                                     running(false),
                                                     fail(false),
                                                     user(NULL),
                                                     startFrame(-1),
                                                     nextFrame(-1),
                                                     targetUnit(NULL),
                                                     targetPosition(Positions::None),
                                                     isInPosition(false),
                                                     usedTech(false),
                                                     testSucceeded(false)
{
  userType = *techType.whatUses().begin();
  BWAssertF(userType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(userType!=UnitTypes::Unknown,{fail=true;return;});
}
int maxTimer = 0;
void UseTechTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(userType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==userType)
      user = u;

  startPosition = user->getPosition();

  if (techType==TechTypes::Optical_Flare)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Firebat)
      {
        targetUnit = u;
      }
  }
  else if (techType==TechTypes::EMP_Shockwave)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Protoss_Arbiter)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Lockdown)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Protoss_Carrier)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Spider_Mines)
  {
    targetUnit = user;
    targetPosition = user->getPosition();
    isInPosition;
  }
  else if (techType==TechTypes::Scanner_Sweep)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Hatchery)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
  }
  else if (techType==TechTypes::Defensive_Matrix)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Marine)
      {
        targetUnit = u;
      }
  }
  else if (techType==TechTypes::Yamato_Gun)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Hatchery)
      {
        targetUnit = u;
      }
  }
  BWAssertF(user!=NULL,{fail=true;return;});
  if (targetUnit != NULL && user->getType().canMove())
    user->rightClick(targetUnit->getPosition());
  else
    isInPosition = true;
  nextFrame = Broodwar->getFrameCount();
}
void UseTechTest::checkPosition()
{
  if (targetUnit != NULL)
  {
    if (user->getDistance(targetUnit->getPosition())<32*5)
    {
      isInPosition = true;
    }
  }
  else
  {
    isInPosition = true;
  }
}
void UseTechTest::useTech()
{
  int energy = user->getEnergy();
  if (targetPosition != Positions::None)
    user->useTech(techType,targetPosition);
  else if (targetUnit != NULL)
    user->useTech(techType,targetUnit);
  else
    user->useTech(techType);
  BWAssert(user->getEnergy() == energy-techType.energyUsed());
  usedTech = true;
  startFrame = Broodwar->getFrameCount();
}
void UseTechTest::update()
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
  Broodwar->setScreenPosition(user->getPosition().x()-320,user->getPosition().y()-240);
  BWAssertF(user!=NULL,{fail=true;return;});
  if (!isInPosition)
    checkPosition();

  if (!isInPosition)
    return;

  if (!usedTech)
    useTech();

  if (techType==TechTypes::Stim_Packs)
  {
    if (user->isStimmed() && user->getStimTimer()>0 && user->getHitPoints()==user->getType().maxHitPoints()-10)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Optical_Flare)
  {
    if (targetUnit->isBlind())
      testSucceeded = true;

  }
  else if (techType==TechTypes::EMP_Shockwave)
  {
    if (targetUnit->getShields()==0)
      testSucceeded = true;

  }
  else if (techType==TechTypes::Lockdown)
  {
    if (targetUnit->isLockedDown() && targetUnit->getLockdownTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Spider_Mines)
  {
    bool spiderMineFound = false;
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Vulture_Spider_Mine)
        spiderMineFound = true;
    if (spiderMineFound && user->getSpiderMineCount() == 2)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Scanner_Sweep)
  {
    if (Broodwar->isVisible(targetPosition.x()/32,targetPosition.y()/32))
      testSucceeded = true;
  }
  else if (techType==TechTypes::Defensive_Matrix)
  {
    if (targetUnit->isDefenseMatrixed() && targetUnit->getDefenseMatrixTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Yamato_Gun)
  {
    if (targetUnit->getHitPoints()<targetUnit->getType().maxHitPoints())
      testSucceeded = true;
  }
  int lastFrame = startFrame+300;
  if (thisFrame>lastFrame) //terminate condition
  {
    running = false;
    return;
  }
}

void UseTechTest::stop()
{
  if (fail == true) return;
  BWAssert(testSucceeded == true);
  if (testSucceeded)
    Broodwar->printf("Used tech %s",techType.getName().c_str());
  else
    Broodwar->printf("Error: Unable to use tech %s",techType.getName().c_str());
}

bool UseTechTest::isRunning()
{
  return running;
}