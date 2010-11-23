#include "TrainTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
TrainTest::TrainTest(BWAPI::UnitType unitType) : unitType(unitType),
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
void TrainTest::start()
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

  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==true,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().empty()==true,{fail=true;return;});
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
  BWAssertF(producer->getRemainingTrainTime() == unitType.buildTime(),{fail=true;return;});
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
  Broodwar->setScreenPosition(producer->getPosition().x()-320,producer->getPosition().y()-240);
  int correctRemainingTrainTime = startFrame+Broodwar->getLatency()+unitType.buildTime()-thisFrame+1;
  if (Broodwar->getLatency()==5)
    correctRemainingTrainTime -= 2;
  if (correctRemainingTrainTime>unitType.buildTime())
    correctRemainingTrainTime=unitType.buildTime();
  if (correctRemainingTrainTime<0)
    correctRemainingTrainTime=0;
  BWAssertF(producer->getRemainingTrainTime() == correctRemainingTrainTime,{Broodwar->printf("%d %d",producer->getRemainingTrainTime(), correctRemainingTrainTime);});
  int lastFrame = startFrame+Broodwar->getLatency()+unitType.buildTime();
  if (Broodwar->getLatency()==5)
    lastFrame-=2;
  if (unitType==UnitTypes::Terran_Nuclear_Missile)
    lastFrame++;
  if (thisFrame>lastFrame) //terminate condition
  {
    running = false;
    return;
  }
  BWAssertF(producer->isTraining()==true,{Broodwar->printf("%d",thisFrame-startFrame);fail=true;return;});
  BWAssertF(producer->isConstructing()==false,{fail=true;return;});
  BWAssertF(producer->isIdle()==false,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().size()==1,{Broodwar->printf("%d tq size = %d",thisFrame-startFrame,producer->getTrainingQueue().size());fail=true;return;});
  BWAssertF(*producer->getTrainingQueue().begin()==unitType,{fail=true;return;});
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
