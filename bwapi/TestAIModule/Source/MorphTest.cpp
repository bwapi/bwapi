#include "MorphTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
#define FAILTEST(C)\
{\
  if (!(C))\
  {\
    log("Assert failed @%s:%u %s[%s:%s] %s (%s)",__FILE__,__LINE__, producer ? producer->getType().c_str() : "NULL", unitType.c_str(), producer ? producer->getOrder().c_str() : "null", producerType.c_str(), Broodwar->getLastError().c_str());\
    assert_fail_count++;\
    fail = true;\
    return;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}
MorphTest::MorphTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                 producerType(unitType.whatBuilds().first),
                                                 previousUnitCount(0),
                                                 startFrame(-1),
                                                 nextFrame(-1),
                                                 producer(NULL),
                                                 finishingMorph(false),
                                                 finishFrame(-1),
                                                 correctMineralCount(0),
                                                 correctGasCount(0),
                                                 correctSupplyUsedCount(0)
{
  fail = false;
  running = false;
  FAILTEST(producerType!=UnitTypes::None);
  FAILTEST(producerType!=UnitTypes::Unknown);
}
void MorphTest::start()
{
  if (fail) return;
  running = true;

  int producerCount = Broodwar->self()->completedUnitCount(producerType);
  FAILTEST(producerCount>=1);
  for (Unit u : Broodwar->self()->getUnits())
  {
    if (u->getType()==producerType && u->isIdle()==true)
    {
      producer = u;
      break;
    }
  }
  FAILTEST(producer!=NULL);
  FAILTEST(producer->isBeingConstructed()==false);
  FAILTEST(producer->isBurrowed()==false);
  FAILTEST(producer->isConstructing()==false);
  FAILTEST(producer->isCompleted()==true);
  FAILTEST(producer->isIdle()==true);
  FAILTEST(producer->isMorphing()==false);
  FAILTEST(producer->isTraining()==false);
  FAILTEST(producer->getBuildType()==UnitTypes::None);
  FAILTEST(producer->getBuildUnit()==NULL);
  FAILTEST(producer->getRemainingTrainTime()==0);
  FAILTEST(producer->getRemainingBuildTime()==0);
  correctMineralCount = Broodwar->self()->minerals() - unitType.mineralPrice();
  correctGasCount = Broodwar->self()->gas() - unitType.gasPrice();
  if (unitType.isTwoUnitsInOneEgg())
    correctSupplyUsedCount = Broodwar->self()->supplyUsed() + unitType.supplyRequired()*2 - producerType.supplyRequired();
  else
    correctSupplyUsedCount = Broodwar->self()->supplyUsed() + unitType.supplyRequired() - producerType.supplyRequired();

  FAILTEST(producer->morph(unitType));

  FAILTEST(producer->isBeingConstructed()==true);
  FAILTEST(producer->isBurrowed()==false);
  FAILTEST(producer->isConstructing()==true);
  FAILTEST(producer->isCompleted()==false);
  FAILTEST(producer->isIdle()==false);
  FAILTEST(producer->isMorphing()==true);
  FAILTEST(producer->isTraining()==false);
  FAILTEST(producer->getBuildType()==unitType);
  FAILTEST(producer->getBuildUnit()==NULL);
  FAILTEST(producer->getRemainingTrainTime()==0);
  FAILTEST(producer->getRemainingBuildTime()==unitType.buildTime());
  FAILTEST(Broodwar->self()->minerals() == correctMineralCount);
  FAILTEST(Broodwar->self()->gas() == correctGasCount);
  FAILTEST(Broodwar->self()->supplyUsed() == correctSupplyUsedCount);

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;
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
  BWAssert(thisFrame == nextFrame);
  FAILTEST(producer != NULL);
  nextFrame++;
  Broodwar->setScreenPosition(producer->getPosition() - Position(320, 240));

  int correctRemainingTrainTime = startFrame + Broodwar->getLatency() + unitType.buildTime()/10 - thisFrame + 1;
  if (correctRemainingTrainTime > unitType.buildTime()/10)
    correctRemainingTrainTime = unitType.buildTime()/10;
  if (correctRemainingTrainTime < 0)
    correctRemainingTrainTime = 0;

  if (finishingMorph)
  {
    FAILTEST(producer != NULL);
    FAILTEST(producer->getType() == unitType);
    if (producerType.isBuilding())
    {
      FAILTEST(producer->isBeingConstructed()==false);
      FAILTEST(producer->isConstructing()==false);
      FAILTEST(producer->isCompleted()==true);
      FAILTEST(producer->isIdle()==true);
      FAILTEST(producer->isMorphing()==false);
      FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1);
    }
    else if (unitType == UnitTypes::Zerg_Lurker || unitType == UnitTypes::Zerg_Devourer)
    {
      if (thisFrame < finishFrame + 16)
      {
        FAILTEST(producer->isBeingConstructed()==true);
        FAILTEST(producer->isConstructing()==true);
        FAILTEST(producer->isCompleted()==false);
        FAILTEST(producer->isIdle()==false);
        FAILTEST(producer->isMorphing()==true);
        FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
      }
      else
      {
        FAILTEST(producer->isBeingConstructed()==false);
        FAILTEST(producer->isConstructing()==false);
        FAILTEST(producer->isCompleted()==true);
        FAILTEST(producer->isIdle()==true);
        FAILTEST(producer->isMorphing()==false);
        FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1);
      }
    }
    else if (unitType==UnitTypes::Zerg_Guardian)
    {
      if (thisFrame<finishFrame+18)
      {
        FAILTEST(producer->isBeingConstructed()==true);
        FAILTEST(producer->isConstructing()==true);
        FAILTEST(producer->isCompleted()==false);
        FAILTEST(producer->isIdle()==false);
        FAILTEST(producer->isMorphing()==true);
        FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
      }
      else
      {
        FAILTEST(producer->isBeingConstructed()==false);
        FAILTEST(producer->isConstructing()==false);
        FAILTEST(producer->isCompleted()==true);
        FAILTEST(producer->isIdle()==true);
        FAILTEST(producer->isMorphing()==false);
        FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1);
      }
    }
    else
    {
      FAILTEST(producer->isBeingConstructed()==true);
      FAILTEST(producer->isConstructing()==true);
      FAILTEST(producer->isCompleted()==false);
      FAILTEST(producer->isIdle()==false);
      FAILTEST(producer->isMorphing()==true);
      FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
    }
    FAILTEST(producer->isBurrowed()==false);
    FAILTEST(producer->isTraining()==false);
    FAILTEST(producer->getBuildType()==UnitTypes::None);
    FAILTEST(producer->getBuildUnit()==NULL);
    FAILTEST(producer->getRemainingTrainTime()==0);
    FAILTEST(producer->getRemainingBuildTime()==0);
    if (thisFrame > startFrame + Broodwar->getLatency() + unitType.buildTime()/10 + 32)
      running = false;
    return;
  }

  finishFrame = startFrame + Broodwar->getLatency() + unitType.buildTime()/10 + 18;
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
      FAILTEST(producer->getType()==UnitTypes::Zerg_Lurker_Egg);
    }
    else if (unitType==UnitTypes::Zerg_Devourer || unitType==UnitTypes::Zerg_Guardian)
    {
      FAILTEST(producer->getType()==UnitTypes::Zerg_Cocoon);
    }
    else
    {
      FAILTEST(producer->getType()==UnitTypes::Zerg_Egg);
    }
  }
  else
  {
    FAILTEST(producer->getType()==unitType);
  }
  FAILTEST(producer->isBeingConstructed()==true);
  FAILTEST(producer->isBurrowed()==false);
  FAILTEST(producer->isConstructing()==true);
  FAILTEST(producer->isCompleted()==false);
  FAILTEST(producer->isIdle()==false);
  BWAssertF(producer->isMorphing()==true,{log("%d",thisFrame-startFrame);fail=true;return;});
  FAILTEST(producer->isTraining()==false);
  BWAssertF(producer->getBuildType()==unitType,
  {
    log("order: %s, build type: %s, correct type: %s",producer->getOrder().c_str(),producer->getBuildType().c_str(),unitType.c_str());
    fail=true;
    return;
  });
  FAILTEST(producer->getBuildUnit()==NULL);
  FAILTEST(producer->getRemainingTrainTime()==0);

  // @TODO: Workaround
  if ( thisFrame >= startFrame + Broodwar->getLatency() + 5 )
  {
    BWAssertF(abs(producer->getRemainingBuildTime() - correctRemainingTrainTime) < 5,{log("%d %d %s",producer->getRemainingBuildTime(), correctRemainingTrainTime, producer->getType().c_str());});
  }
  FAILTEST(Broodwar->self()->minerals() == correctMineralCount);
  FAILTEST(Broodwar->self()->gas() == correctGasCount);
  FAILTEST(Broodwar->self()->supplyUsed() == correctSupplyUsedCount);
  BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
}

void MorphTest::stop()
{
  if (fail == true) return;
  FAILTEST(producer!=NULL);
  FAILTEST(producer->getType()==unitType);
  FAILTEST(producer->isBeingConstructed()==false);
  FAILTEST(producer->isBurrowed()==false);
  FAILTEST(producer->isConstructing()==false);
  FAILTEST(producer->isCompleted()==true);
  FAILTEST(producer->isIdle()==true);
  FAILTEST(producer->isMorphing()==false);
  FAILTEST(producer->isTraining()==false);
  FAILTEST(producer->getBuildType()==UnitTypes::None);
  FAILTEST(producer->getBuildUnit()==NULL);
  FAILTEST(producer->getRemainingTrainTime()==0);
  FAILTEST(producer->getRemainingBuildTime()==0);
  FAILTEST(Broodwar->self()->minerals() == correctMineralCount);
  FAILTEST(Broodwar->self()->gas() == correctGasCount);
  FAILTEST(Broodwar->self()->supplyUsed() == correctSupplyUsedCount);
  if (unitType.isTwoUnitsInOneEgg())
  {
    FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+2);
  }
  else
  {
    FAILTEST(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1);
  }
}
