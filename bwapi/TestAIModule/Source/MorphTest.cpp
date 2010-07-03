#include "MorphTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
MorphTest::MorphTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                 running(false),
                                                 fail(false),
                                                 producer(NULL),
                                                 startTrainFrame(-1),
                                                 nextUpdateFrame(-1),
                                                 finishingMorph(false)
{
  producerType = unitType.whatBuilds().first;
  BWAssertF(producerType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(producerType!=UnitTypes::Unknown,{fail=true;return;});
}
void MorphTest::start()
{
  if (fail) return;
  running = true;

  int producerCount = Broodwar->self()->completedUnitCount(producerType);
  BWAssertF(producerCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    if (u->getType()==producerType)
    {
      producer = u;
      break;
    }
  }
  BWAssertF(producer!=NULL,{fail=true;return;});
  BWAssertF(producer->isBeingConstructed()==false,{fail=true;return;});
  BWAssertF(producer->isBurrowed()==false,{fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==true,{fail=true;return;});
  BWAssertF(producer->isMorphing()==false,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getBuildType()==UnitTypes::None,{fail=true;return;});
  BWAssertF(producer->getBuildUnit()==NULL,{fail=true;return;});
  BWAssertF(producer->getRemainingTrainTime()==0,{fail=true;return;});
  BWAssertF(producer->getRemainingBuildTime()==0,{fail=true;return;});
  correctMineralCount = Broodwar->self()->minerals() - unitType.mineralPrice();
  correctGasCount = Broodwar->self()->gas() - unitType.gasPrice();
  if (unitType.isTwoUnitsInOneEgg())
    correctSupplyUsedCount = Broodwar->self()->supplyUsed() + unitType.supplyRequired()*2 - producerType.supplyRequired();
  else
    correctSupplyUsedCount = Broodwar->self()->supplyUsed() + unitType.supplyRequired() - producerType.supplyRequired();

  producer->morph(unitType);

  BWAssertF(producer->isBeingConstructed()==true,{fail=true;return;});
  BWAssertF(producer->isBurrowed()==false,{fail=true;return;});
  BWAssertF(producer->isConstructing()==true,{fail=true;return;});
  BWAssertF(producer->isIdle()==false,{fail=true;return;});
  BWAssertF(producer->isMorphing()==true,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getBuildType()==unitType,{fail=true;return;});
  BWAssertF(producer->getBuildUnit()==NULL,{fail=true;return;});
  BWAssertF(producer->getRemainingTrainTime()==0,{fail=true;return;});
  BWAssertF(producer->getRemainingBuildTime()==unitType.buildTime(),{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->supplyUsed() == correctSupplyUsedCount,{fail=true;return;});

  startTrainFrame = Broodwar->getFrameCount();
  nextUpdateFrame = startTrainFrame;
  previousUnitCount = Broodwar->self()->completedUnitCount(unitType);

}
void MorphTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextUpdateFrame);
  BWAssertF(producer!=NULL,{fail=true;return;});
  nextUpdateFrame++;
  Broodwar->setScreenPosition(producer->getPosition().x()-320,producer->getPosition().y()-240);

  int correctRemainingTrainTime = startTrainFrame+Broodwar->getLatency()+unitType.buildTime()-thisFrame+1;
  if (correctRemainingTrainTime>unitType.buildTime())
    correctRemainingTrainTime=unitType.buildTime();
  if (correctRemainingTrainTime<0)
    correctRemainingTrainTime=0;

  if (finishingMorph)
  {
    BWAssertF(producer!=NULL,{fail=true;return;});
    BWAssertF(producer->getType()==unitType,{fail=true;return;});
    if (producerType.isBuilding())
    {
      BWAssertF(producer->isBeingConstructed()==false,{fail=true;return;});
      BWAssertF(producer->isConstructing()==false,{fail=true;return;});
      BWAssertF(producer->isIdle()==true,{fail=true;return;});
      BWAssertF(producer->isMorphing()==false,{fail=true;return;});
      BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
    }
    else if (unitType==UnitTypes::Zerg_Lurker || unitType==UnitTypes::Zerg_Devourer)
    {
      if (thisFrame<finishFrame+16)
      {
        BWAssertF(producer->isBeingConstructed()==true,{fail=true;return;});
        BWAssertF(producer->isConstructing()==true,{fail=true;return;});
        BWAssertF(producer->isIdle()==false,{fail=true;return;});
        BWAssertF(producer->isMorphing()==true,{fail=true;return;});
        BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount,{fail=true;return;});
      }
      else
      {
        BWAssertF(producer->isBeingConstructed()==false,{fail=true;return;});
        BWAssertF(producer->isConstructing()==false,{fail=true;return;});
        BWAssertF(producer->isIdle()==true,{fail=true;return;});
        BWAssertF(producer->isMorphing()==false,{fail=true;return;});
        BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
      }
    }
    else if (unitType==UnitTypes::Zerg_Guardian)
    {
      if (thisFrame<finishFrame+18)
      {
        BWAssertF(producer->isBeingConstructed()==true,{fail=true;return;});
        BWAssertF(producer->isConstructing()==true,{fail=true;return;});
        BWAssertF(producer->isIdle()==false,{fail=true;return;});
        BWAssertF(producer->isMorphing()==true,{fail=true;return;});
        BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount,{fail=true;return;});
      }
      else
      {
        BWAssertF(producer->isBeingConstructed()==false,{fail=true;return;});
        BWAssertF(producer->isConstructing()==false,{fail=true;return;});
        BWAssertF(producer->isIdle()==true,{fail=true;return;});
        BWAssertF(producer->isMorphing()==false,{fail=true;return;});
        BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
      }
    }
    else
    {
      BWAssertF(producer->isBeingConstructed()==true,{fail=true;return;});
      BWAssertF(producer->isConstructing()==true,{fail=true;return;});
      BWAssertF(producer->isIdle()==false,{fail=true;return;});
      BWAssertF(producer->isMorphing()==true,{fail=true;return;});
      BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount,{fail=true;return;});
    }
    BWAssertF(producer->isBurrowed()==false,{fail=true;return;});
    BWAssertF(producer->isTraining()==false,{fail=true;return;});
    BWAssertF(producer->getBuildType()==UnitTypes::None,{fail=true;return;});
    BWAssertF(producer->getBuildUnit()==NULL,{fail=true;return;});
    BWAssertF(producer->getRemainingTrainTime()==0,{fail=true;return;});
    BWAssertF(producer->getRemainingBuildTime()==0,{fail=true;return;});
    if (thisFrame > startTrainFrame+Broodwar->getLatency()+unitType.buildTime()+32)
      running = false;
    return;
  }

  finishFrame = startTrainFrame+Broodwar->getLatency()+unitType.buildTime()+18;
  if (producerType.isBuilding()) finishFrame-=10;
  if (producerType==UnitTypes::Zerg_Mutalisk || producerType==UnitTypes::Zerg_Hydralisk) finishFrame-=16;
  if (thisFrame>finishFrame) //terminate condition
  {
    finishingMorph = true;
    return;
  }
  if (unitType.isBuilding()==false)
  {
    if (unitType==UnitTypes::Zerg_Lurker)
    {
      BWAssertF(producer->getType()==UnitTypes::Zerg_Lurker_Egg,{fail=true;return;});
    }
    else if (unitType==UnitTypes::Zerg_Devourer || unitType==UnitTypes::Zerg_Guardian)
    {
      BWAssertF(producer->getType()==UnitTypes::Zerg_Cocoon,
      {
        Broodwar->printf("order: %s, type: %s != Zerg_Cocoon",producer->getOrder().getName().c_str(),producer->getType().getName().c_str());
        fail=true;
        return;
      });
    }
    else
    {
      BWAssertF(producer->getType()==UnitTypes::Zerg_Egg,{fail=true;return;});
    }
  }
  else
  {
    if (unitType.getRace()==Races::Zerg && producerType.isBuilding())
    {
      BWAssertF(producer->getType()==producerType,{fail=true;return;});
    }
    else
    {
      BWAssertF(producer->getType()==unitType,{fail=true;return;});
    }
  }
  BWAssertF(producer->isBeingConstructed()==true,{fail=true;return;});
  BWAssertF(producer->isBurrowed()==false,{fail=true;return;});
  BWAssertF(producer->isConstructing()==true,{fail=true;return;});
  BWAssertF(producer->isIdle()==false,{fail=true;return;});
  BWAssertF(producer->isMorphing()==true,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getBuildType()==unitType,
  {
    Broodwar->printf("order: %s, build type: %s, correct type: %s",producer->getOrder().getName().c_str(),producer->getBuildType().getName().c_str(),unitType.getName().c_str());
    fail=true;
    return;
  });
  BWAssertF(producer->getBuildUnit()==NULL,{fail=true;return;});
  BWAssertF(producer->getRemainingTrainTime()==0,{fail=true;return;});
  BWAssertF(abs(producer->getRemainingBuildTime() - correctRemainingTrainTime)<5,{Broodwar->printf("%d %d",producer->getRemainingBuildTime(), correctRemainingTrainTime);});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->supplyUsed() == correctSupplyUsedCount,{fail=true;return;});
  BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
}

void MorphTest::stop()
{
  if (fail == true) return;
  BWAssertF(producer!=NULL,{fail=true;return;});
  BWAssertF(producer->getType()==unitType,{fail=true;return;});
  BWAssertF(producer->isBeingConstructed()==false,{fail=true;return;});
  BWAssertF(producer->isBurrowed()==false,{fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==true,{fail=true;return;});
  BWAssertF(producer->isMorphing()==false,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getBuildType()==UnitTypes::None,{fail=true;return;});
  BWAssertF(producer->getBuildUnit()==NULL,{fail=true;return;});
  BWAssertF(producer->getRemainingTrainTime()==0,{fail=true;return;});
  BWAssertF(producer->getRemainingBuildTime()==0,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->supplyUsed() == correctSupplyUsedCount,{fail=true;return;});
  if (unitType.isTwoUnitsInOneEgg())
  {
    BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+2,{fail=true;return;});
  }
  else
  {
    BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
  }
}

bool MorphTest::isRunning()
{
  return running;
}