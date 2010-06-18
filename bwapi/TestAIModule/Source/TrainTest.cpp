#include "TrainTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
TrainTest::TrainTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                             running(false),
                                                             fail(false),
                                                             producer(NULL),
                                                             startTrainFrame(-1),
                                                             nextUpdateFrame(-1)
{
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
  BWAssertF(producer->isIdle()==true,{fail=true;return;});
  BWAssertF(producer->isLifted()==false,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().empty()==true,{fail=true;return;});

  producer->train(unitType);
  BWAssertF(producer->isTraining()==true,{fail=true;return;});

  startTrainFrame = Broodwar->getFrameCount();
  nextUpdateFrame = startTrainFrame;
  previousUnitCount = Broodwar->self()->completedUnitCount(unitType);

  BWAssertF(producer->isTraining()==true,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().size()==1,{fail=true;return;});
  BWAssertF(*producer->getTrainingQueue().begin()==unitType,{fail=true;return;});
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
  BWAssert(thisFrame==nextUpdateFrame);
  BWAssertF(producer!=NULL,{fail=true;return;});
  nextUpdateFrame++;

  if (thisFrame>startTrainFrame+Broodwar->getLatency()+unitType.buildTime()) //terminate condition
  {
    running = false;
    return;
  }
  BWAssert(producer->isTraining()==true);
  BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
}

void TrainTest::stop()
{
  if (fail == true) return;
  BWAssertF(producer!=NULL,{fail=true;return;});
  BWAssertF(producer->isTraining()==false,{fail=true;return;});
  BWAssertF(producer->getTrainingQueue().empty()==true,{fail=true;return;});
  BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
}

bool TrainTest::isRunning()
{
  return running;
}