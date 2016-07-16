#include "CancelMorphTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
#define FAILTEST(C)\
{\
  if (!(C))\
  {\
    log("Assert failed @%s:%u %s[%s:%s] (%s)",__FILE__,__LINE__, producer ? producer->getType().c_str() : "NULL", unitType.c_str(), producer ? producer->getOrder().c_str() : "null", Broodwar->getLastError().c_str());\
    assert_fail_count++;\
    fail = true;\
    return;\
  }\
  else\
  {\
    assert_success_count++;\
  }\
}
CancelMorphTest::CancelMorphTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                             producerType(unitType.whatBuilds().first),
                                                             startFrame(-1),
                                                             nextFrame(-1),
                                                             producer(NULL),
                                                             correctMineralCount(0),
                                                             correctGasCount(0),
                                                             correctSupplyUsedCount(0),
                                                             correctCompletedUnitCount(0),
                                                             correctIncompleteUnitCount(0),
                                                             correctAllUnitCount(0)
{
  fail = false;
  running = false;
  FAILTEST(producerType!=UnitTypes::None);
  FAILTEST(producerType!=UnitTypes::Unknown);
}
void CancelMorphTest::start()
{
  if (fail) return;
  running = true;

  int producerCount = Broodwar->self()->completedUnitCount(producerType);
  FAILTEST(producerCount>=1);
  for (Unit u : Broodwar->self()->getUnits())
  {
    if (u->getType()==producerType)
    {
      producer = u;
      break;
    }
  }
  correctMineralCount = Broodwar->self()->minerals();
  correctGasCount = Broodwar->self()->gas();
  correctSupplyUsedCount = Broodwar->self()->supplyUsed();
  correctCompletedUnitCount = Broodwar->self()->completedUnitCount(unitType);
  correctIncompleteUnitCount = Broodwar->self()->incompleteUnitCount(unitType);
  correctAllUnitCount = Broodwar->self()->allUnitCount(unitType);
  producer->morph(unitType);
  BWAssertF(producer->getBuildType()==unitType,
  {
    log("%s != %s",producer->getBuildType().c_str(),unitType.c_str());
    fail=true;
    return;
  });

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;
  if (producerType.isBuilding())
  {
    correctMineralCount -= unitType.mineralPrice();
    correctGasCount -= unitType.gasPrice();
    correctMineralCount += (int)(unitType.mineralPrice()*0.75);
    correctGasCount += (int)(unitType.gasPrice()*0.75);
  }

}
void CancelMorphTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  FAILTEST(thisFrame==nextFrame);
  nextFrame++;
  FAILTEST(producer!=NULL);
  if (producer->exists())
    Broodwar->setScreenPosition(producer->getPosition() - Position(320,240));

  if (thisFrame<=startFrame+20)
  {
    BWAssertF(producer->getBuildType()==unitType,
    {
      log("%s != %s, this frame = %d, start frame = %d",producer->getBuildType().c_str(),unitType.c_str(),thisFrame,startFrame);
      fail=true;
      return;
    });
    FAILTEST(producer->isMorphing()==true);
    FAILTEST(producer->isConstructing()==true);
    FAILTEST(producer->isTraining()==false);
    FAILTEST(producer->isIdle()==false);
    FAILTEST(producer->isResearching()==false);
    FAILTEST(producer->isUpgrading()==false);
    BWAssertF(producer->getTech()==TechTypes::None,{log("%s",producer->getTech().c_str());fail=true;return;});
    FAILTEST(producer->getUpgrade()==UpgradeTypes::None);
  }
  if (thisFrame==startFrame+20)
  {
    FAILTEST(producer->cancelMorph());
  }

  if (thisFrame>=startFrame+20)
  {
    if (producer->exists() || producerType!=UnitTypes::Zerg_Larva)
    {
      BWAssertF(producer->getBuildType()==UnitTypes::None,{log("%d: %s",thisFrame-startFrame,producer->getBuildType().c_str());fail=true;return;});
      BWAssertF(producer->isMorphing()==false,{log("%d",thisFrame-startFrame);});
      BWAssertF(producer->isConstructing()==false,{log("%d",thisFrame-startFrame);});
      FAILTEST(producer->isTraining()==false);
      FAILTEST(producer->isIdle()==true);
      FAILTEST(producer->isResearching()==false);
      FAILTEST(producer->isUpgrading()==false);
      FAILTEST(producer->getTech()==TechTypes::None);
      FAILTEST(producer->getUpgrade()==UpgradeTypes::None);
      FAILTEST(producer->getRemainingBuildTime()==0);
      FAILTEST(producer->getRemainingTrainTime()==0);
      FAILTEST(producer->getRemainingResearchTime()==0);
      FAILTEST(producer->getRemainingUpgradeTime()==0);
      BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{log("%d: %d!=%d",thisFrame-startFrame,Broodwar->self()->minerals(),correctMineralCount);});
      FAILTEST(Broodwar->self()->gas()==correctGasCount);
      BWAssertF(Broodwar->self()->supplyUsed()==correctSupplyUsedCount,{log("%d!=%d",Broodwar->self()->supplyUsed(),correctSupplyUsedCount);fail=true;return;});
      if (thisFrame>=startFrame+25)
      {
        FAILTEST(Broodwar->self()->completedUnitCount(unitType)==correctCompletedUnitCount);
        FAILTEST(Broodwar->self()->incompleteUnitCount(unitType)==correctIncompleteUnitCount);
        FAILTEST(Broodwar->self()->allUnitCount(unitType)==correctAllUnitCount);
      }
    }
  }
  if (thisFrame>=startFrame+40)
  {
    running = false;
  }
}

void CancelMorphTest::stop()
{
}
