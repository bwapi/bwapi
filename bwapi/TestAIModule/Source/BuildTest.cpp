#include "BuildTest.h"
#include "BWAssert.h"
#include "BuildingPlacer.h"
using namespace std;
using namespace BWAPI;

#define FAILTEST(C)\
{\
  if (!(C))\
  {\
    log("Assert failed @%s:%u %s[%s:%s]->%s[%s] (%s)",__FILE__,__LINE__, builder ? builder->getType().c_str() : "NULL", unitType.c_str(), builder ? builder->getOrder().c_str() : "null", building ? building->getType().c_str() : "NULL", building ? building->getOrder().c_str() : "null", Broodwar->getLastError().c_str());\
    assert_fail_count++;\
    fail = true;\
    return;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}

BuildTest::BuildTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                 builderType(unitType.whatBuilds().first),
                                                 previousUnitCount(0),
                                                 startFrame(-1),
                                                 nextFrame(-1),
                                                 finishFrame(-1),
                                                 builder(NULL),
                                                 building(NULL),
                                                 finishingBuilding(false)
{
  fail = false;
  running = false;
  FAILTEST(builderType!=UnitTypes::None);
  FAILTEST(builderType!=UnitTypes::Unknown);
}
void BuildTest::start()
{
  if (fail) return;
  running = true;

  int builderCount = Broodwar->self()->completedUnitCount(builderType);
  FAILTEST(builderCount>=1);
  for (Unit u : Broodwar->self()->getUnits())
  {
    if (u->getType()==builderType && u->getAddon()==NULL)
    {
      builder = u;
      break;
    }
  }
  FAILTEST(builder!=NULL);
  FAILTEST(builder->isIdle()==true);


  if (unitType.isAddon())
  {
    FAILTEST(builder->buildAddon(unitType));
  }
  else
  {
    if (unitType==UnitTypes::Protoss_Pylon)
    {
      buildLocation = BuildingPlacer::getBuildLocationNear(builder->getTilePosition(), unitType, 4);
    }
    else if ((unitType.getRace()==Races::Zerg && unitType.isResourceDepot()) || unitType==UnitTypes::Protoss_Gateway)
    {
      buildLocation = BuildingPlacer::getBuildLocationNear(builder->getTilePosition(), unitType, 2);
    }
    {
      buildLocation = BuildingPlacer::getBuildLocationNear(builder->getTilePosition(), unitType, 1);
    }
    FAILTEST(builder->build(unitType, buildLocation));
  }
  FAILTEST( !builder->isIdle() );
  FAILTEST( builder->isConstructing() );
  BWAssertF(builder->getBuildType()==unitType,
  {
    log("Error: %s != %s",builder->getBuildType().c_str(),unitType.c_str());
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
  FAILTEST(builder!=NULL);
  nextFrame++;
  if (unitType==UnitTypes::Zerg_Extractor && builder->exists()==false)
  {
    Unitset buildingsOnTile( Broodwar->getUnitsOnTile(buildLocation.x,buildLocation.y, BWAPI::Filter::GetType == unitType ) );
    if ( !buildingsOnTile.empty() )
    {
      if (unitType==UnitTypes::Zerg_Extractor)
      {
        builder = *buildingsOnTile.begin();
      }
    }
    FAILTEST(builder!=NULL);
  }
  FAILTEST(builder->exists());
  Broodwar->setScreenPosition(builder->getPosition() - Position(320,240));

  if (finishingBuilding==true)
  {
    FAILTEST(builder!=NULL);
    if (builder->isIdle()==false && unitType.isRefinery() && builder->getType().getRace()!=Races::Zerg)
    {
      FAILTEST(builder->isIdle()==false);
      builder->stop();
      FAILTEST(builder->isIdle()==true);
    }
    else
    {
      if (thisFrame==finishFrame+1 && unitType.isAddon()==false && builder->getType().getRace()!=Races::Protoss)
      {
        FAILTEST(builder->isIdle()==false);
      }
      else
      {
        if (thisFrame<finishFrame+8 && builder->getType().getRace()==Races::Zerg)
        {
          FAILTEST(builder->isIdle()==false);
        }
        else
        {
          FAILTEST(builder->isIdle()==true);
        }
      }
    }
    if (thisFrame<finishFrame+8 && builder->getType().getRace()==Races::Zerg)
    {
      FAILTEST(builder->isConstructing()==true);
      FAILTEST(builder->getBuildType()==unitType);
      FAILTEST(building->isCompleted()==false);
      FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
    }
    else
    {
      FAILTEST(builder->isConstructing()==false);
      FAILTEST(builder->getBuildType()==UnitTypes::None);
      FAILTEST(building->isCompleted()==true);
      FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1);
    }
    FAILTEST(building!=NULL);
    if (thisFrame>finishFrame+200)
    {
      running = false;
    }
    return;
  }
  if (building==NULL && builder->getBuildUnit()!=NULL)
  {
    building = builder->getBuildUnit();
  }
  if (building==NULL && builder->getAddon()!=NULL)
  {
    building = builder->getAddon();
  }

  Unitset buildingsOnTile( Broodwar->getUnitsOnTile(buildLocation.x,buildLocation.y, BWAPI::Filter::GetType == unitType) );
  if (building==NULL && !buildingsOnTile.empty() )
  {
    building = *buildingsOnTile.begin();
    if (unitType==UnitTypes::Zerg_Extractor)
    {
      builder=building;
    }
  }

  if (startFrame == -1 && building!=NULL)
  {
    startFrame = Broodwar->getFrameCount();
    if (unitType.isAddon())
    {
      startFrame-=2;
    }
    if (unitType==UnitTypes::Protoss_Assimilator)
    {
      startFrame--;
    }
  }
  BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
  int correctRemainingBuildTime = 0;
  if (building!=NULL)
  {
    correctRemainingBuildTime = startFrame + unitType.buildTime()/10 - thisFrame + 1;
    if (builder->getType().getRace() == Races::Protoss)
      correctRemainingBuildTime--;
    if (builder->getType().getRace() == Races::Zerg)
      correctRemainingBuildTime--;
    if (builder->getType() == UnitTypes::Zerg_Extractor)
      correctRemainingBuildTime--;
    if (correctRemainingBuildTime < 0) correctRemainingBuildTime = 0;
    if (correctRemainingBuildTime > unitType.buildTime()/10) correctRemainingBuildTime = unitType.buildTime()/10;
  }
  bool correctIsConstructing = false;
      
  if (builder->getType().getRace() == Races::Terran)
    correctIsConstructing = true;

  if (builder->getType().getRace() == Races::Protoss)
  {
    if (unitType == UnitTypes::Protoss_Assimilator)
      correctIsConstructing = !building || thisFrame < startFrame+2;
    else
      correctIsConstructing = !building || thisFrame < startFrame+1;
  }
  if (builder->getType().getRace() == Races::Zerg)
    correctIsConstructing = true;

  if ( correctRemainingBuildTime > 0 || !building )
  {
    BWAssertF(builder->isConstructing() == correctIsConstructing,{log("%d %d, %d", builder->isConstructing(), correctIsConstructing, correctRemainingBuildTime);});
    BWAssertF(builder->isIdle() != builder->isConstructing(),{log("%d %d, %d", builder->isIdle(), !builder->isConstructing(), correctRemainingBuildTime);});
  }
  if ( building )
  {  
    if ( builder->getType().getRace() == Races::Terran )
    {
      FAILTEST(builder->getBuildUnit() == building);
      FAILTEST(building->getBuildUnit() == builder);
      FAILTEST(builder->getBuildType() == building->getType());
    }
    FAILTEST(building->getBuildType() == building->getType());
    BWAssertF(building->getRemainingBuildTime() == correctRemainingBuildTime,{log("%d %d",building->getRemainingBuildTime(),correctRemainingBuildTime);});
    FAILTEST(building->isCompleted() == false);
    FAILTEST(building->isConstructing() == true);
    FAILTEST(building->isIdle() == false);
    FAILTEST(building->isBeingConstructed() == true);
    if (building->getType().getRace() == Races::Protoss)
    {
      if (thisFrame>startFrame + Broodwar->getLatency() + unitType.buildTime()/10 + 67)
      {
        finishingBuilding = true;
        finishFrame = thisFrame;
      }
    }
    else
    {
      if (correctRemainingBuildTime == 0)
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
