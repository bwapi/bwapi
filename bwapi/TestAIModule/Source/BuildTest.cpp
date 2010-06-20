#include "BuildTest.h"
#include "BWAssert.h"
#include "BuildingPlacer.h"
using namespace std;
using namespace BWAPI;
BuildingPlacer* placer=NULL;
BuildTest::BuildTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                             running(false),
                                                             fail(false),
                                                             builder(NULL),
                                                             startTrainFrame(-1),
                                                             nextUpdateFrame(-1),
                                                             finishFrame(-1),
                                                             finishingBuilding(false)
{
  builderType = unitType.whatBuilds().first;
  BWAssertF(builderType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(builderType!=UnitTypes::Unknown,{fail=true;return;});
  if (placer==NULL) placer = new BuildingPlacer();
}
void BuildTest::start()
{
  if (fail) return;
  running = true;

  int builderCount = Broodwar->self()->completedUnitCount(builderType);
  BWAssertF(builderCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    if (u->getType()==builderType && u->getAddon()==NULL)
    {
      builder = u;
      break;
    }
  }
  BWAssertF(builder!=NULL,{fail=true;return;});
  BWAssertF(builder->isIdle()==true,{fail=true;return;});


  if (unitType.isAddon())
  {
    builder->buildAddon(unitType);
  }
  else
  {
    buildLocation = placer->getBuildLocationNear(builder->getTilePosition(),unitType,1);
    if (unitType==UnitTypes::Protoss_Pylon)
      buildLocation = placer->getBuildLocationNear(builder->getTilePosition(),unitType,4);
    builder->build(buildLocation,unitType);
  }
  BWAssertF(builder->isIdle()==false,{fail=true;return;});
  BWAssertF(builder->isConstructing()==true,{fail=true;return;});
  BWAssertF(builder->getBuildType()==unitType,{fail=true;return;});

  nextUpdateFrame = Broodwar->getFrameCount();
  previousUnitCount = Broodwar->self()->completedUnitCount(unitType);

}
void BuildTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextUpdateFrame);
  BWAssertF(builder!=NULL,{fail=true;return;});
  nextUpdateFrame++;
  Broodwar->setScreenPosition(builder->getPosition().x()-320,builder->getPosition().y()-240);

  if (finishingBuilding==true)
  {
    BWAssertF(builder!=NULL,{fail=true;return;});
    if (builder->isIdle()==false && unitType.isRefinery())
    {
      BWAssertF(builder->isIdle()==false,{fail=true;return;});
      builder->stop();
      BWAssertF(builder->isIdle()==true,{fail=true;return;});
    }
    else
    {
      if (thisFrame==finishFrame+1 && unitType.isAddon()==false && builder->getType().getRace()==Races::Terran)
      {
        BWAssertF(builder->isIdle()==false,{fail=true;return;});
      }
      else
      {
        BWAssertF(builder->isIdle()==true,{fail=true;return;});
      }
    }
    BWAssertF(builder->isConstructing()==false,{fail=true;return;});
    BWAssertF(builder->getBuildType()==UnitTypes::None,{fail=true;return;});
    BWAssertF(building!=NULL,{fail=true;return;});
    BWAssertF(building->isCompleted()==true,{fail=true;return;});
    BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
    if (thisFrame>finishFrame+30)
    {
      running = false;
    }
    return;
  }
  if (building==NULL && builder->getBuildUnit()!=NULL)
    building = builder->getBuildUnit();

  std::set<Unit*> buildingsOnTile;
  std::set<Unit*> unitsOnTile = Broodwar->unitsOnTile(buildLocation.x(),buildLocation.y());
  for each(Unit* u in unitsOnTile)
  {
    if (u->getType()==unitType)
    {
      buildingsOnTile.insert(u);
    }
  }
  if (building==NULL && buildingsOnTile.empty()==false)
    building = *buildingsOnTile.begin();

  if (startTrainFrame == -1 && building!=NULL)
  {
    startTrainFrame = Broodwar->getFrameCount();
    if (unitType.isAddon())
      startTrainFrame-=2;
    if (unitType==UnitTypes::Protoss_Assimilator)
      startTrainFrame--;
  }
  BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
  int correctRemainingBuildTime = -1;
  if (building!=NULL)
  {
    correctRemainingBuildTime = startTrainFrame+unitType.buildTime()-thisFrame+1;
    if (builder->getType().getRace()==Races::Protoss)
      correctRemainingBuildTime--;
    if (correctRemainingBuildTime<0) correctRemainingBuildTime=0;
    if (correctRemainingBuildTime>unitType.buildTime()) correctRemainingBuildTime=unitType.buildTime();
  }
  bool correctIsConstructing = false;
      
  if (builder->getType().getRace()==Races::Terran)
  {
    correctIsConstructing = true;
  }
  if (builder->getType().getRace()==Races::Protoss)
  {
    if (unitType==UnitTypes::Protoss_Assimilator)
      correctIsConstructing = (building==NULL) || thisFrame<startTrainFrame+2;
    else
      correctIsConstructing = (building==NULL) || thisFrame<startTrainFrame+1;
  }
  BWAssertF(builder->isConstructing() == correctIsConstructing,{Broodwar->printf("%d %d",builder->isConstructing() , correctIsConstructing);});
  BWAssertF(builder->isIdle() != builder->isConstructing(),{Broodwar->printf("%d %d",builder->isIdle() , !builder->isConstructing());});
  if (building!=NULL)
  {
    BWAssertF(building->getRemainingBuildTime()==correctRemainingBuildTime,{Broodwar->printf("%d %d",building->getRemainingBuildTime(),correctRemainingBuildTime);});
    BWAssert(building->isCompleted()==false);
    BWAssert(building->isConstructing()==true);
    BWAssert(building->isIdle()==false);
    BWAssert(building->isBeingConstructed()==true);
    if (building->getType().getRace()==Races::Protoss)
    {
      if (thisFrame>startTrainFrame+Broodwar->getLatency()+unitType.buildTime() + 67)
      {
        finishingBuilding = true;
        finishFrame = thisFrame;
      }
    }
    else
    {
      if (correctRemainingBuildTime==0)
      {
        finishingBuilding = true;
        finishFrame = thisFrame;
      }
    }
  }
}

void BuildTest::stop()
{
}

bool BuildTest::isRunning()
{
  return running;
}