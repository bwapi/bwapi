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
  Broodwar->printf("Testing %s...",techType.getName().c_str());
  BWAssertF(user->getEnergy()>=techType.energyUsed(),{fail=true;return;});

  if (techType==TechTypes::Scanner_Sweep)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Hatchery)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Stim_Packs)
  {
    //no target unit/position
  }
  else if (techType==TechTypes::Optical_Flare)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Firebat)
      {
        targetUnit = u;
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Restoration)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->isBlind())
      {
        targetUnit = u;
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Spider_Mines)
  {
    targetUnit = user;
    targetPosition = user->getPosition();
    isInPosition = true;
  }
  else if (techType==TechTypes::Defensive_Matrix)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Marine)
      {
        targetUnit = u;
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::EMP_Shockwave)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Protoss_Arbiter)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Irradiate)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Mutalisk)
      {
        targetUnit = u;
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Lockdown)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Protoss_Carrier)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Yamato_Gun)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Hatchery)
      {
        targetUnit = u;
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Nuclear_Strike)
  {
    bool hasNuke = false;
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Terran_Nuclear_Silo && u->hasNuke())
        hasNuke = true;
    BWAssertF(hasNuke == true,{Broodwar->printf("Error: No nuke!");fail=true;return;});
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Protoss_Nexus)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  BWAssertF(user!=NULL,{fail=true;return;});
  if (!isInPosition)
  {
    if (targetUnit != NULL && user->getType().canMove())
      user->rightClick(targetUnit->getPosition());
    else
      isInPosition = true;
  }
  nextFrame = Broodwar->getFrameCount();
}
void UseTechTest::checkPosition()
{
  if (targetUnit != NULL)
  {
    if (user->getDistance(targetUnit->getPosition())<32*4)
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

  if (techType==TechTypes::Scanner_Sweep)
  {
    if (Broodwar->isVisible(targetPosition.x()/32,targetPosition.y()/32))
      testSucceeded = true;
  }
  else if (techType==TechTypes::Stim_Packs)
  {
    if (user->isStimmed() && user->getStimTimer()>0 && user->getHitPoints()==user->getType().maxHitPoints()-10)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Optical_Flare)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->isBlind())
      testSucceeded = true;

  }
  else if (techType==TechTypes::Restoration)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->isBlind()==false)
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
  else if (techType==TechTypes::Defensive_Matrix)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->isDefenseMatrixed() && targetUnit->getDefenseMatrixTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::EMP_Shockwave)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->getShields()==0)
      testSucceeded = true;

  }
  else if (techType==TechTypes::Irradiate)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->isIrradiated() && targetUnit->getIrradiateTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Lockdown)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->isLockedDown() && targetUnit->getLockdownTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Yamato_Gun)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    if (targetUnit->getHitPoints()<targetUnit->getType().maxHitPoints())
      testSucceeded = true;
  }
  else if (techType==TechTypes::Nuclear_Strike)
  {
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    bool hasNuke = false;
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->hasNuke())
        hasNuke = true;
    if (hasNuke==false)
    {
      if (targetUnit->getHitPoints()<targetUnit->getType().maxHitPoints()*0.7)
        testSucceeded = true;
    }
  }
  if (thisFrame == startFrame+800)
  {
    if (testSucceeded)
      Broodwar->printf("Used tech %s",techType.getName().c_str());
    else
      Broodwar->printf("Error: Unable to use tech %s",techType.getName().c_str());
  }
  int lastFrame = startFrame+1000;
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
}

bool UseTechTest::isRunning()
{
  return running;
}