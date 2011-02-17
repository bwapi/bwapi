#include "GatherTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
GatherTest::GatherTest() : worker(NULL),
                           mineral(NULL),
                           refinery(NULL),
                           startFrame(-1),
                           nextFrame(-1)
{
  fail = false;
  running = false;
}
void GatherTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(Broodwar->self()->getRace().getWorker());
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType().isWorker())
      worker = u;
  for each(Unit* u in Broodwar->getMinerals())
    if ( u->getType().isMineralField() )
      mineral = u;
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType().isRefinery())
      refinery = u;
  BWAssertF(worker!=NULL,{fail=true;return;});
  BWAssertF(worker->exists(),{fail=true;return;});
  BWAssertF(mineral!=NULL,{fail=true;return;});
  BWAssertF(mineral->exists(),{fail=true;return;});
  BWAssertF(refinery!=NULL,{fail=true;return;});
  BWAssertF(refinery->exists(),{fail=true;return;});
  worker->gather(mineral);
  BWAssertF(worker->isGatheringMinerals(),{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();
  initialMinerals = Broodwar->self()->minerals();
  initialGas = Broodwar->self()->gas();
  state=1;
}
void GatherTest::update()
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
  Broodwar->setScreenPosition(worker->getPosition().x()-320,worker->getPosition().y()-240);
  if (state==1 && Broodwar->self()->minerals()>initialMinerals+50 && worker->isGatheringMinerals())
  {
    BWAssertF(worker->gather(refinery),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;})
    state = 2;
  }
  if (state==2 && Broodwar->self()->gas()>initialGas+50 && worker->isGatheringGas() && worker->isCarryingGas())
  {
    BWAssertF(worker->stop(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;})
    startFrame = thisFrame;
    state = 3;
  }
  if (state==3 && thisFrame>startFrame+100)
  {
    state = 4;
    BWAssertF(worker->returnCargo(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;})
    startFrame = thisFrame;
  }
  if (state==4 && !worker->isCarryingGas())
  {
    BWAssertF(worker->stop(),{Broodwar->printf("%s",Broodwar->getLastError().toString().c_str());fail=true;return;})
    startFrame = thisFrame;
    state = 5;
  }
  if (state==5 && thisFrame>startFrame+100)
  {
    running=false;
    return;
  }

  if (state==1)
  {
    BWAssertF(worker->isGatheringMinerals(),{log("%d",thisFrame-startFrame);fail=true;return;});
    BWAssertF(worker->isGatheringGas()==false,{fail=true;return;});
  }
  else if (state==2)
  {
    BWAssertF(worker->isGatheringGas(),{fail=true;return;});
    BWAssertF(worker->isGatheringMinerals()==false,{fail=true;return;});
  }
  else if (state==3)
  {
    BWAssertF(worker->isGatheringGas()==false,{log("%d",thisFrame-startFrame);fail=true;return;});
    BWAssertF(worker->isGatheringMinerals()==false,{fail=true;return;});
    BWAssertF(worker->isCarryingGas()==true,{fail=true;return;});
  }
  else if (state==4)
  {
    BWAssertF(worker->isGatheringGas()==true,{log("%d",thisFrame-startFrame);fail=true;return;});
    BWAssertF(worker->isGatheringMinerals()==false,{fail=true;return;});
    BWAssertF(worker->isCarryingGas()==true,{fail=true;return;});
  }
  else if (state==5)
  {
    BWAssertF(worker->isIdle()==true,{fail=true;return;});
    BWAssertF(worker->isGatheringGas()==false,{fail=true;return;});
    BWAssertF(worker->isGatheringMinerals()==false,{fail=true;return;});
    BWAssertF(worker->isCarryingGas()==false,{fail=true;return;});
    BWAssertF(worker->isCarryingGas()==false,{fail=true;return;});
  }
}

void GatherTest::stop()
{
}
