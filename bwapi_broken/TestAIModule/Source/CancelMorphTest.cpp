#include "CancelMorphTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
CancelMorphTest::CancelMorphTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                             producer(NULL),
                                                             startFrame(-1),
                                                             nextFrame(-1)
{
  fail = false;
  running = false;
  producerType = unitType.whatBuilds().first;
  BWAssertF(producerType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(producerType!=UnitTypes::Unknown,{fail=true;return;});
}
void CancelMorphTest::start()
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
  correctMineralCount = Broodwar->self()->minerals();
  correctGasCount = Broodwar->self()->gas();
  correctSupplyUsedCount = Broodwar->self()->supplyUsed();
  correctCompletedUnitCount = Broodwar->self()->completedUnitCount(unitType);
  correctIncompleteUnitCount = Broodwar->self()->incompleteUnitCount(unitType);
  correctAllUnitCount = Broodwar->self()->allUnitCount(unitType);
  producer->morph(unitType);
  BWAssertF(producer->getBuildType()==unitType,
  {
    Broodwar->printf("%s != %s",producer->getBuildType().getName().c_str(),unitType.getName().c_str());
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
  BWAssertF(thisFrame==nextFrame,{fail=true;return;});
  nextFrame++;
  BWAssertF(producer!=NULL,{fail=true;return;});
  if (producer->exists())
  {
    Broodwar->setScreenPosition(producer->getPosition().x()-320,producer->getPosition().y()-240);
  }
  if (thisFrame<=startFrame+200)
  {
    BWAssertF(producer->getBuildType()==unitType,
    {
      Broodwar->printf("%s != %s, this frame = %d, start frame = %d",producer->getBuildType().getName().c_str(),unitType.getName().c_str(),thisFrame,startFrame);
      fail=true;
      return;
    });
    BWAssertF(producer->isMorphing()==true,{fail=true;return;});
    BWAssertF(producer->isConstructing()==true,{fail=true;return;});
    BWAssertF(producer->isTraining()==false,{fail=true;return;});
    BWAssertF(producer->isIdle()==false,{fail=true;return;});
    BWAssertF(producer->isResearching()==false,{fail=true;return;});
    BWAssertF(producer->isUpgrading()==false,{fail=true;return;});
    BWAssertF(producer->getTech()==TechTypes::None,{Broodwar->printf("%s",producer->getTech().getName().c_str());fail=true;return;});
    BWAssertF(producer->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  }
  if (thisFrame==startFrame+200)
  {
    BWAssertF(producer->cancelMorph(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;});
  }
  if (thisFrame>=startFrame+200)
  {
    if (producer->exists() || producerType!=UnitTypes::Zerg_Larva)
    {
      BWAssertF(producer->getBuildType()==UnitTypes::None,{Broodwar->printf("%d: %s",thisFrame-startFrame,producer->getBuildType().getName().c_str());fail=true;return;});
      BWAssertF(producer->isMorphing()==false,{Broodwar->printf("%d",thisFrame-startFrame);});
      BWAssertF(producer->isConstructing()==false,{Broodwar->printf("%d",thisFrame-startFrame);});
      BWAssertF(producer->isTraining()==false,{fail=true;return;});
      BWAssertF(producer->isIdle()==true,{fail=true;return;});
      BWAssertF(producer->isResearching()==false,{fail=true;return;});
      BWAssertF(producer->isUpgrading()==false,{fail=true;return;});
      BWAssertF(producer->getTech()==TechTypes::None,{fail=true;return;});
      BWAssertF(producer->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
      BWAssertF(producer->getRemainingBuildTime()==0,{fail=true;return;});
      BWAssertF(producer->getRemainingTrainTime()==0,{fail=true;return;});
      BWAssertF(producer->getRemainingResearchTime()==0,{fail=true;return;});
      BWAssertF(producer->getRemainingUpgradeTime()==0,{fail=true;return;});
      BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{Broodwar->printf("%d: %d!=%d",thisFrame-startFrame,Broodwar->self()->minerals(),correctMineralCount);});
      BWAssertF(Broodwar->self()->gas()==correctGasCount,{fail=true;return;});
      BWAssertF(Broodwar->self()->supplyUsed()==correctSupplyUsedCount,{Broodwar->printf("%d!=%d",Broodwar->self()->supplyUsed(),correctSupplyUsedCount);fail=true;return;});
      BWAssertF(Broodwar->self()->completedUnitCount(unitType)==correctCompletedUnitCount,{fail=true;return;});
      BWAssertF(Broodwar->self()->incompleteUnitCount(unitType)==correctIncompleteUnitCount,{fail=true;return;});
      BWAssertF(Broodwar->self()->allUnitCount(unitType)==correctAllUnitCount,{fail=true;return;});
    }
  }
  if (thisFrame>=startFrame+400)
  {
    running = false;
  }
}

void CancelMorphTest::stop()
{
}
