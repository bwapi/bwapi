#include "CancelConstructionTest.h"
#include "BWAssert.h"
#include "BuildingPlacer.h"
using namespace std;
using namespace BWAPI;
extern BuildingPlacer* placer;
CancelConstructionTest::CancelConstructionTest(BWAPI::UnitType unitType, bool terranHalt) : unitType(unitType),
                                               builder(NULL),
                                               building(NULL),
                                               startFrame(-1),
                                               nextFrame(-1),
                                               finishFrame(-1),
                                               terranHalt(terranHalt)
{
  fail = false;
  running = false;
  builderType = unitType.whatBuilds().first;
  BWAssertF(builderType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(builderType!=UnitTypes::Unknown,{fail=true;return;});
  if (placer==NULL) placer = new BuildingPlacer();
}
void CancelConstructionTest::start()
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

  correctMineralCount = Broodwar->self()->minerals()-unitType.mineralPrice();
  correctGasCount = Broodwar->self()->gas()-unitType.gasPrice();

  correctMineralCount += (int)(unitType.mineralPrice()*0.75);
  correctGasCount += (int)(unitType.gasPrice()*0.75);

  correctSupplyUsedCount = Broodwar->self()->supplyUsed();

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
  if (building!=NULL)
  {
    if (builderType.getRace()==Races::Terran && terranHalt)
    {
      if (thisFrame==startFrame+Broodwar->getLatency()+200)
      {
        builder->haltConstruction();
        BWAssertF(builder->getBuildType()==UnitTypes::None,{fail=true;return;});
        BWAssertF(builder->isConstructing()==false,{fail=true;return;});
        BWAssertF(building->isBeingConstructed()==false,{fail=true;return;});
      }
      if (thisFrame>=startFrame+Broodwar->getLatency()+200 && thisFrame<startFrame+Broodwar->getLatency()+300)
      {
        BWAssertF(builder->getBuildType()==UnitTypes::None,{fail=true;return;});
        BWAssertF(builder->isConstructing()==false,{fail=true;return;});
        BWAssertF(building->isBeingConstructed()==false,{fail=true;return;});
      }
    }
    if (thisFrame==startFrame+Broodwar->getLatency()+500)
    {
      building->cancelConstruction();
    }
    if (thisFrame>=startFrame+Broodwar->getLatency()+500)
    {
      BWAssertF(builder->getBuildType()==UnitTypes::None,{fail=true;return;});
      BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{Broodwar->printf("%d != %d",Broodwar->self()->minerals(),correctMineralCount);fail=true;return;});
      BWAssertF(Broodwar->self()->gas()==correctGasCount,{fail=true;return;});
      BWAssertF(Broodwar->self()->supplyUsed()==correctSupplyUsedCount,{Broodwar->printf("%d != %d",Broodwar->self()->supplyUsed(),correctSupplyUsedCount);fail=true;return;});
    }
    if (thisFrame>startFrame+Broodwar->getLatency()+550)
    {
      if (unitType.getRace()!=Races::Zerg)
      {
        BWAssertF(building->exists()==false,{fail=true;return;});
      }
      else
      {
        BWAssertF(building->getType()==UnitTypes::Zerg_Drone,{fail=true;return;});
      }
    } 
    if (thisFrame>startFrame+Broodwar->getLatency()+700)
    {
      running = false;
    }
  }
}

void CancelConstructionTest::stop()
{
}
