#include "BuildTest.h"
#include "BWAssert.h"
#include "BuildingPlacer.h"
using namespace std;
using namespace BWAPI;
BuildingPlacer* placer=NULL;
BuildTest::BuildTest(BWAPI::UnitType unitType) : unitType(unitType),
                                                             running(false),
                                                             fail(false),
                                                             builder(NULL),
                                                             startTrainFrame(-1),
                                                             nextUpdateFrame(-1),
                                                             finishFrame(-1)
{
  builderType = unitType.whatBuilds().first;
  BWAssertF(builderType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(builderType!=UnitTypes::Unknown,{fail=true;return;});
  if (placer==NULL) placer = new BuildingPlacer();
}
void BuildTest::start()
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


  if (unitType.isAddon())
  {
    builder->buildAddon(unitType);
  }
  else
  {
    TilePosition loc = placer->getBuildLocationNear(builder->getTilePosition(),unitType,1);
    builder->build(loc,unitType);
  }
  BWAssertF(builder->isIdle()==false,{fail=true;return;});
  BWAssertF(builder->isConstructing()==true,{fail=true;return;});
  BWAssertF(builder->getBuildType()==unitType,{fail=true;return;});

  nextUpdateFrame = Broodwar->getFrameCount();
  previousUnitCount = Broodwar->self()->completedUnitCount(unitType);

}
void BuildTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextUpdateFrame);
  BWAssertF(builder!=NULL,{fail=true;return;});
  nextUpdateFrame++;
  if (startTrainFrame == -1 && builder->getBuildUnit()!=NULL)
  {
    startTrainFrame = Broodwar->getFrameCount()-1;
    if (unitType.isAddon())
      startTrainFrame-=Broodwar->getLatency();
  }

  if (builder->getBuildUnit()!=NULL)
    building = builder->getBuildUnit();

  if (building!=NULL && thisFrame>startTrainFrame+Broodwar->getLatency()+unitType.buildTime()) //terminate condition
  {
    if (builder->isIdle()==false && unitType.isRefinery())
    {
      builder->stop();
    }
    else
    {
      if (finishFrame == -1)
        finishFrame = Broodwar->getFrameCount();
      else
      {
        if (Broodwar->getFrameCount()>finishFrame + 10)
        {
          running = false;
          return;
        }
      }
    }
  }
  else
  {
    BWAssert(builder->isConstructing()==true);
    BWAssert(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount);
  }
}

void BuildTest::stop()
{
  if (fail == true) return;
  BWAssertF(builder!=NULL,{fail=true;return;});
  BWAssertF(builder->isIdle()==true,{fail=true;return;});
  BWAssertF(builder->isConstructing()==false,{fail=true;return;});
  BWAssertF(builder->getBuildType()==UnitTypes::None,{fail=true;return;});
  BWAssertF(building!=NULL,{fail=true;return;});
  BWAssertF(building->isCompleted()==true,{fail=true;return;});
  BWAssertF(Broodwar->self()->completedUnitCount(unitType) == previousUnitCount+1,{fail=true;return;});
}

bool BuildTest::isRunning()
{
  return running;
}