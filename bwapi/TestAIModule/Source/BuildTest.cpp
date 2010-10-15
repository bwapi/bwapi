#include "BuildTest.h"
#include "BWAssert.h"
#include "BuildingPlacer.h"
using namespace std;
using namespace BWAPI;
BuildingPlacer* placer=NULL;
BuildTest::BuildTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                 builder(NULL),
                                                 building(NULL),
                                                 startFrame(-1),
                                                 nextFrame(-1),
                                                 finishFrame(-1),
                                                 finishingBuilding(false)
{
  fail = false;
  running = false;
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
    if (unitType.getRace()==Races::Zerg && unitType.isResourceDepot())
      buildLocation = placer->getBuildLocationNear(builder->getTilePosition(),unitType,2);
    builder->build(buildLocation,unitType);
  }
  BWAssertF(builder->isIdle()==false,{fail=true;return;});
  BWAssertF(builder->isConstructing()==true,{fail=true;return;});
  BWAssertF(builder->getBuildType()==unitType,
  {
    Broodwar->printf("Error: %s != %s",builder->getBuildType().getName().c_str(),unitType.getName().c_str());
    fail=true;
    return;
  });

  nextFrame = Broodwar->getFrameCount();
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
  BWAssert(thisFrame==nextFrame);
  BWAssertF(builder!=NULL,{fail=true;return;});
  nextFrame++;
  if (unitType==UnitTypes::Zerg_Extractor && builder->exists()==false)
  {
    std::set<Unit*> buildingsOnTile;
    std::set<Unit*> unitsOnTile = Broodwar->unitsOnTile(buildLocation.x(),buildLocation.y());
    for each(Unit* u in unitsOnTile)
    {
      if (u->getType()==unitType)
      {
        buildingsOnTile.insert(u);
      }
    }
    if (buildingsOnTile.empty()==false)
    {
      if (unitType==UnitTypes::Zerg_Extractor)
        builder=*buildingsOnTile.begin();
    }
    BWAssertF(builder!=NULL,{fail=true;return;});
  }
  BWAssertF(builder->exists(),{fail=true;return;});
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
      if (thisFrame==finishFrame+1 && unitType.isAddon()==false && builder->getType().getRace()!=Races::Protoss)
      {
        BWAssertF(builder->isIdle()==false,{fail=true;return;});
      }
      else
      {
        if (thisFrame<finishFrame+8 && builder->getType().getRace()==Races::Zerg)
        {
          if (unitType == UnitTypes::Zerg_Extractor)
          {
            BWAssertF(builder->isIdle()==true,{fail=true;return;});
          }
          else
          {
            BWAssertF(builder->isIdle()==false,{fail=true;return;});
          }
        }
        else
        {
          BWAssertF(builder->isIdle()==true,{fail=true;return;});
        }
      }
    }
    if (thisFrame<finishFrame+8 && builder->getType().getRace()==Races::Zerg)
    {
      BWAssertF(builder->isConstructing()==true,{fail=true;return;});
      BWAssertF(builder->getBuildType()==unitType,{fail=true;return;});
      BWAssertF(building->isCompleted()==false,{fail=true;return;});
      BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount,{fail=true;return;});
    }
    else
    {
      BWAssertF(builder->isConstructing()==false,{fail=true;return;});
      BWAssertF(builder->getBuildType()==UnitTypes::None,{fail=true;return;});
      BWAssertF(building->isCompleted()==true,{fail=true;return;});
      BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
    }
    BWAssertF(building!=NULL,{fail=true;return;});
    if (thisFrame>finishFrame+200)
    {
      running = false;
    }
    return;
  }
  if (building==NULL && builder->getBuildUnit()!=NULL)
    building = builder->getBuildUnit();
  if (building==NULL && builder->getAddon()!=NULL)
    building = builder->getAddon();

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
  {
    building = *buildingsOnTile.begin();
    if (unitType==UnitTypes::Zerg_Extractor)
      builder=building;
  }

  if (startFrame == -1 && building!=NULL)
  {
    startFrame = Broodwar->getFrameCount();
    if (unitType.isAddon())
      startFrame-=2;
    if (unitType==UnitTypes::Protoss_Assimilator)
      startFrame--;
  }
  BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
  int correctRemainingBuildTime = -1;
  if (building!=NULL)
  {
    correctRemainingBuildTime = startFrame+unitType.buildTime()-thisFrame+1;
    if (builder->getType().getRace()==Races::Protoss)
      correctRemainingBuildTime--;
    if (builder->getType().getRace()==Races::Zerg)
      correctRemainingBuildTime--;
    if (builder->getType()==UnitTypes::Zerg_Extractor)
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
      correctIsConstructing = (building==NULL) || thisFrame<startFrame+2;
    else
      correctIsConstructing = (building==NULL) || thisFrame<startFrame+1;
  }
  if (builder->getType().getRace()==Races::Zerg)
  {
    correctIsConstructing = true;
  }
  if (correctRemainingBuildTime>0 || building==NULL)
  {
    BWAssertF(builder->isConstructing() == correctIsConstructing,{Broodwar->printf("%d %d, %d",builder->isConstructing() , correctIsConstructing, correctRemainingBuildTime);});
    BWAssertF(builder->isIdle() != builder->isConstructing(),{Broodwar->printf("%d %d, %d",builder->isIdle() , !builder->isConstructing(), correctRemainingBuildTime);});
  }
  if (building!=NULL)
  {  
    if (builder->getType().getRace()==Races::Terran)
    {
      BWAssert(builder->getBuildUnit()==building);
      BWAssert(building->getBuildUnit()==builder);
      BWAssert(builder->getBuildType()==building->getType());
    }
    BWAssert(building->getBuildType()==building->getType());
    BWAssertF(building->getRemainingBuildTime()==correctRemainingBuildTime,{Broodwar->printf("%d %d",building->getRemainingBuildTime(),correctRemainingBuildTime);});
    BWAssert(building->isCompleted()==false);
    BWAssert(building->isConstructing()==true);
    BWAssert(building->isIdle()==false);
    BWAssert(building->isBeingConstructed()==true);
    if (building->getType().getRace()==Races::Protoss)
    {
      if (thisFrame>startFrame+Broodwar->getLatency()+unitType.buildTime() + 67)
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
