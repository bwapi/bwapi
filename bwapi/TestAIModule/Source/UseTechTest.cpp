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
  BWAssertF(user->getEnergy()>=techType.energyUsed(),{Broodwar->printf("Error: Not enough energy!");fail=true;return;});

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
  else if (techType==TechTypes::Archon_Warp)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Protoss_High_Templar && u!=user)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Dark_Archon_Meld)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Protoss_Dark_Templar && u!=user)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Disruption_Web)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Hydralisk)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Psionic_Storm)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Ultralisk)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Hallucination)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Protoss_Zealot)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Mind_Control)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Marine)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Maelstrom)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Mutalisk)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Recall)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Protoss_Dragoon)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
    isInPosition = true;
  }
  else if (techType==TechTypes::Stasis_Field)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Siege_Tank_Tank_Mode)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Feedback)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Ghost)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Consume)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Zerg_Zergling)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Dark_Swarm)
  {
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
  else if (techType==TechTypes::Ensnare)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Ghost)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Infestation)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Command_Center)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Parasite)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_SCV)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  else if (techType==TechTypes::Plague)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Terran_Marine)
      {
        targetUnit = u;
        targetPosition = u->getPosition();
      }
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::None,{fail=true;return;});
    BWAssertF(targetPosition!=Positions::Unknown,{fail=true;return;});
  }
  else if (techType==TechTypes::Spawn_Broodlings)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Protoss_Dragoon)
        targetUnit = u;
    BWAssertF(targetUnit!=NULL,{fail=true;return;});
  }
  if (targetUnit!=NULL)
  {
    targetType=targetUnit->getType();
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
  bool used=false;
  if (targetPosition != Positions::None)
    used=user->useTech(techType,targetPosition);
  else if (targetUnit != NULL)
    used=user->useTech(techType,targetUnit);
  else
    used=user->useTech(techType);
  if (used==false)
    Broodwar->printf("Error: %s",Broodwar->getLastError().toString().c_str());

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
  if (user->exists()==false)
    user=targetUnit;
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
  else if (techType==TechTypes::Archon_Warp)
  {
    if ((user->exists()       && user->getType()      ==UnitTypes::Protoss_Archon) ||
        (targetUnit->exists() && targetUnit->getType()==UnitTypes::Protoss_Archon))
      testSucceeded = true;
  }
  else if (techType==TechTypes::Dark_Archon_Meld)
  {
    if ((user->exists()       && user->getType()      ==UnitTypes::Protoss_Dark_Archon) ||
        (targetUnit->exists() && targetUnit->getType()==UnitTypes::Protoss_Dark_Archon))
      testSucceeded = true;
  }
  else if (techType==TechTypes::Disruption_Web)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Spell_Disruption_Web)
        testSucceeded = true;
  }
  else if (techType==TechTypes::Psionic_Storm)
  {
    if (targetUnit->isUnderStorm())
      testSucceeded = true;
  }
  else if (techType==TechTypes::Hallucination)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Protoss_Zealot && u->isHallucination())
        testSucceeded = true;
    
  }
  else if (techType==TechTypes::Mind_Control)
  {
    if (targetUnit->getPlayer()==Broodwar->self())
      testSucceeded = true;
  }
  else if (techType==TechTypes::Maelstrom)
  {
    if (targetUnit->isMaelstrommed() && targetUnit->getMaelstromTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Recall)
  {
    if (targetUnit->getDistance(user)<200)
      testSucceeded = true;      
  }
  else if (techType==TechTypes::Stasis_Field)
  {
    if (targetUnit->isStasised() && targetUnit->getStasisTimer()>0)
      testSucceeded = true;  
  }
  else if (techType==TechTypes::Feedback)
  {
    if (targetUnit->exists()==false)
      testSucceeded = true;

  }
  else if (techType==TechTypes::Consume)
  {
    if (targetUnit->exists()==false)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Dark_Swarm)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Spell_Dark_Swarm)
        testSucceeded = true;
  }
  else if (techType==TechTypes::Ensnare)
  {
    if (targetUnit->isEnsnared() && targetUnit->getEnsnareTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Infestation)
  {
    if (targetUnit->getPlayer()==Broodwar->self())
      testSucceeded = true;
  }
  else if (techType==TechTypes::Parasite)
  {
    if (targetUnit->isParasited())
      testSucceeded = true;
  }
  else if (techType==TechTypes::Plague)
  {
    if (targetUnit->isPlagued() && targetUnit->getPlagueTimer()>0)
      testSucceeded = true;
  }
  else if (techType==TechTypes::Spawn_Broodlings)
  {
    for each(Unit* u in Broodwar->getAllUnits())
      if (u->getType()==UnitTypes::Zerg_Broodling)
        testSucceeded = true;
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