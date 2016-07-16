#include "TrainTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
TrainTest::TrainTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                 producerType(unitType.whatBuilds().first),
                                                 previousUnitCount(0),
                                                 startFrame(-1),
                                                 nextFrame(-1),
                                                 producer(NULL),
                                                 correctMineralCount(0),
                                                 correctGasCount(0),
                                                 correctSupplyUsedCount(0)
{
  fail = false;
  running = false;
  BWAssertF(producerType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(producerType!=UnitTypes::Unknown,{fail=true;return;});
}
void TrainTest::start()
{
  if (fail) return;
  running = true;

  int producerCount = Broodwar->self()->completedUnitCount(producerType);
  BWAssertF(producerCount>=1,{fail=true;return;});
  for (Unit u : Broodwar->self()->getUnits())
  {
    if (u->getType()==producerType)
    {
      producer = u;
      break;
    }
  }
  BWAssertF(producer!=NULL,{fail=true;return;});

  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==true,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().empty()==true,{log("%s", producer->getTrainingQueue().front().c_str());fail=true;return;});
  BWAssertF(producer->getRemainingTrainTime() == 0,{fail=true;return;});
  correctMineralCount = Broodwar->self()->minerals() - unitType.mineralPrice();
  correctGasCount = Broodwar->self()->gas() - unitType.gasPrice();
  correctSupplyUsedCount = Broodwar->self()->supplyUsed() + unitType.supplyRequired();

  producer->train(unitType);

  BWAssertF(producer->isTraining()==true,{fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==false,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().size()==1,{fail=true;return;});
  BWAssertF(*producer->getTrainingQueue().begin()==unitType,{fail=true;return;});
  BWAssertF(producer->getRemainingTrainTime() == unitType.buildTime(),{fail=true; log("%d == %d (%s)", producer->getRemainingTrainTime(), unitType.buildTime()/10, unitType.c_str()); return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->supplyUsed() == correctSupplyUsedCount,{fail=true;return;});

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;
  previousUnitCount = Broodwar->self()->completedUnitCount(unitType);

}
void TrainTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextFrame);
  BWAssertF(producer!=NULL,{fail=true;return;});
  nextFrame++;
  Broodwar->setScreenPosition(producer->getPosition() - Position(320,240));
  int correctRemainingTrainTime = startFrame + Broodwar->getLatency() + unitType.buildTime()/10 - thisFrame + 1;
  if (Broodwar->getLatency()==5)
    correctRemainingTrainTime += 1;
  if (correctRemainingTrainTime > unitType.buildTime()/10)
    correctRemainingTrainTime = unitType.buildTime()/10;
  if (correctRemainingTrainTime < 0)
    correctRemainingTrainTime = 0;

  // @TODO: Workaround
  if ( thisFrame <= startFrame + Broodwar->getLatency() )
  {
    BWAssertF(producer->getRemainingTrainTime()/10 == correctRemainingTrainTime,{log("%d Error %d != %d",thisFrame-startFrame,producer->getRemainingTrainTime()/10, correctRemainingTrainTime);});
  }
  else
  {
    BWAssertF(producer->getRemainingTrainTime() == correctRemainingTrainTime,{log("%d Error %d != %d %s",thisFrame-startFrame,producer->getRemainingTrainTime(), correctRemainingTrainTime, producer->getType().c_str());});
  }

  int lastFrame = startFrame + Broodwar->getLatency() + unitType.buildTime()/10;
  if (Broodwar->getLatency()==5)
    lastFrame++;
  if (unitType==UnitTypes::Terran_Nuclear_Missile)
    lastFrame++;
  if (thisFrame>lastFrame) //terminate condition
  {
    running = false;
    return;
  }
  BWAssertF(producer->isTraining()==true,{log("%s: %s, %s", producer->getType().c_str(), producer->getOrder().c_str(), producer->getSecondaryOrder().c_str());Broodwar->printf("%d",thisFrame-startFrame);fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==false,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().size()==1,{log("%d tq size = %d",thisFrame-startFrame,producer->getTrainingQueue().size());fail=true;return;});
  BWAssertF(producer->getTrainingQueue().front()==unitType,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->supplyUsed() == correctSupplyUsedCount,{Broodwar->printf("%d: %d!=%d",thisFrame-startFrame,Broodwar->self()->supplyUsed(),correctSupplyUsedCount);fail=true;return;});
  BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount,{fail=true;return;});
}

void TrainTest::stop()
{
  if (fail == true) return;
  BWAssertF(producer!=NULL,{fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==true,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().empty()==true,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->supplyUsed() == correctSupplyUsedCount,{fail=true;return;});
  if (unitType != UnitTypes::Protoss_Scarab)
    BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
}
