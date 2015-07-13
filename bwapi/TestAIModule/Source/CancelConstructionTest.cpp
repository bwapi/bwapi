#include "CancelConstructionTest.h"
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
CancelConstructionTest::CancelConstructionTest(BWAPI::UnitType unitType, bool terranHalt) : unitType(unitType),
                                               builderType(unitType.whatBuilds().first),
                                               previousUnitCount(0),
                                               startFrame(-1),
                                               nextFrame(-1),
                                               finishFrame(-1),
                                               builder(NULL),
                                               building(NULL),
                                               correctMineralCount(0),
                                               correctGasCount(0),
                                               correctSupplyUsedCount(0),
                                               terranHalt(terranHalt)
{
  fail = false;
  running = false;
  FAILTEST(builderType!=UnitTypes::None);
  FAILTEST(builderType!=UnitTypes::Unknown);
}
void CancelConstructionTest::start()
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

  correctMineralCount = Broodwar->self()->minerals()-unitType.mineralPrice();
  correctGasCount = Broodwar->self()->gas()-unitType.gasPrice();

  correctMineralCount += (int)(unitType.mineralPrice()*0.75);
  correctGasCount += (int)(unitType.gasPrice()*0.75);

  correctSupplyUsedCount = Broodwar->self()->supplyUsed();

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
    else if (unitType.getRace()==Races::Zerg && unitType.isResourceDepot())
    {
      buildLocation = BuildingPlacer::getBuildLocationNear(builder->getTilePosition(), unitType, 2);
    }
    else
    {
      buildLocation = BuildingPlacer::getBuildLocationNear(builder->getTilePosition(), unitType, 1);
    }
    BWAssertF(builder->build(unitType,buildLocation),{Broodwar->printf("%s",Broodwar->getLastError().c_str());fail=true;return;});
  }
  FAILTEST(builder->isIdle()==false);
  FAILTEST(builder->isConstructing()==true);
  BWAssertF(builder->getBuildType()==unitType,
  {
    log("Error: %s != %s",builder->getBuildType().c_str(),unitType.c_str());
    fail=true;
    return;
  });

  nextFrame = Broodwar->getFrameCount();
  previousUnitCount = Broodwar->self()->completedUnitCount(unitType);

}
void CancelConstructionTest::update()
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
    Unitset buildingsOnTile;
    Unitset unitsOnTile = Broodwar->getUnitsOnTile(buildLocation.x,buildLocation.y);
    for (Unit u : unitsOnTile)
    {
      if (u->getType()==unitType)
      {
        buildingsOnTile.insert(u);
      }
    }
    if (buildingsOnTile.empty()==false)
    {
      if (unitType==UnitTypes::Zerg_Extractor)
      {
        builder=*buildingsOnTile.begin();
      }
    }
    FAILTEST(builder!=NULL);
  }
  FAILTEST(builder->exists());
  Broodwar->setScreenPosition(builder->getPosition() - Position(320,240));

  if (building==NULL && builder->getBuildUnit()!=NULL)
  {
    building = builder->getBuildUnit();
  }
  if (building==NULL && builder->getAddon()!=NULL)
  {
    building = builder->getAddon();
  }

  Unitset buildingsOnTile;
  Unitset unitsOnTile = Broodwar->getUnitsOnTile(buildLocation.x,buildLocation.y);
  for (Unit u : unitsOnTile)
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
    {
      builder=building;
    }
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
  if (building!=NULL)
  {
    if (builderType.getRace()==Races::Terran && terranHalt)
    {
      if (thisFrame==startFrame+Broodwar->getLatency()+20)
      {
        builder->haltConstruction();
        FAILTEST(builder->getBuildType()==UnitTypes::None);
        FAILTEST(builder->isConstructing()==false);
        FAILTEST(building->isBeingConstructed()==false);
      }
      if (thisFrame>=startFrame+Broodwar->getLatency()+20 && thisFrame<startFrame+Broodwar->getLatency()+30)
      {
        FAILTEST(builder->getBuildType()==UnitTypes::None);
        FAILTEST(builder->isConstructing()==false);
        FAILTEST(building->isBeingConstructed()==false);
      }
    }
    if (thisFrame==startFrame+Broodwar->getLatency()+50)
    {
      if (unitType.isAddon())
        builder->cancelAddon();
      else
        building->cancelConstruction();
    }
    if (thisFrame>=startFrame+Broodwar->getLatency()+50)
    {
      FAILTEST(builder->getBuildType()==UnitTypes::None);
      BWAssertF(builder->isConstructing()==false,{log("%d, %s: %s,%s",thisFrame-(startFrame+Broodwar->getLatency()+50), builder->getType().c_str(), builder->getOrder().c_str(), builder->getSecondaryOrder().c_str());});
      BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{log("%d: %d != %d",thisFrame-(startFrame+Broodwar->getLatency()+50),Broodwar->self()->minerals(),correctMineralCount);fail=true;return;});
      BWAssertF(Broodwar->self()->gas()==correctGasCount,{log("%d: %d != %d, diff = %d",thisFrame-(startFrame+Broodwar->getLatency()+50), Broodwar->self()->gas(),correctGasCount,Broodwar->self()->gas()-correctGasCount);fail=true;return;});
      BWAssertF(Broodwar->self()->supplyUsed()==correctSupplyUsedCount,{log("%d: %d != %d",thisFrame-(startFrame+Broodwar->getLatency()+50),Broodwar->self()->supplyUsed(),correctSupplyUsedCount);fail=true;return;});
    }
    if (thisFrame>startFrame+Broodwar->getLatency()+55)
    {
      if (unitType.getRace()!=Races::Zerg)
      {
        FAILTEST(building->exists()==false);
      }
      else
      {
        FAILTEST(building->getType()==UnitTypes::Zerg_Drone);
      }
    } 
    if (thisFrame>startFrame+Broodwar->getLatency()+70)
    {
      running = false;
    }
  }
}

void CancelConstructionTest::stop()
{
}
