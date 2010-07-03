#include "CancelMorphTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
CancelMorphTest::CancelMorphTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                             running(false),
                                                             fail(false),
                                                             producer(NULL),
                                                             startFrame(-1),
                                                             nextFrame(-1)
{
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

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;

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
  BWAssert(thisFrame==nextFrame);
  nextFrame++;
  BWAssertF(producer!=NULL,{fail=true;return;});
  if (thisFrame<startFrame+200)
  {
    BWAssertF(producer->getBuildType()==unitType,
    {
      Broodwar->printf("%s != %s",producer->getBuildType().getName().c_str(),unitType.getName().c_str());
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
    producer->cancelMorph();
  }
  if (thisFrame>=startFrame+240)
  {
    if (producer->exists() || producerType!=UnitTypes::Zerg_Larva)
    {
      BWAssertF(producer->getBuildType()==UnitTypes::None,{fail=true;return;});
//      BWAssertF(producer->isMorphing()==false,{fail=true;return;});
//      BWAssertF(producer->isConstructing()==false,{fail=true;return;});
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
      BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{fail=true;return;});
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

bool CancelMorphTest::isRunning()
{
  return running;
}