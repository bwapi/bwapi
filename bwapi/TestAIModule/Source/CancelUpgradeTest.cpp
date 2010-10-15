#include "CancelUpgradeTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
CancelUpgradeTest::CancelUpgradeTest(BWAPI::UpgradeType upgradeType) : upgradeType(upgradeType),
                                                                       upgrader(NULL),
                                                                       startFrame(-1),
                                                                       nextFrame(-1)
{
  fail = false;
  running = false;
  upgraderType = upgradeType.whatUpgrades();
  BWAssertF(upgraderType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(upgraderType!=UnitTypes::Unknown,{fail=true;return;});
}
void CancelUpgradeTest::start()
{
  if (fail) return;
  running = true;

  int upgraderCount = Broodwar->self()->completedUnitCount(upgraderType);
  BWAssertF(upgraderCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    if (u->getType()==upgraderType)
    {
      upgrader = u;
      break;
    }
  }
  correctMineralCount = Broodwar->self()->minerals();
  correctGasCount = Broodwar->self()->gas();
  correctSupplyUsedCount = Broodwar->self()->supplyUsed();
  upgrader->upgrade(upgradeType);

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;

}
void CancelUpgradeTest::update()
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
  Broodwar->setScreenPosition(upgrader->getPosition().x()-320,upgrader->getPosition().y()-240);
  BWAssertF(upgrader!=NULL,{fail=true;return;});
  if (thisFrame<startFrame+200)
  {
    BWAssertF(upgrader->getBuildType()==UnitTypes::None,{fail=true;return;});
    BWAssertF(upgrader->isMorphing()==false,{fail=true;return;});
    BWAssertF(upgrader->isConstructing()==false,{fail=true;return;});
    BWAssertF(upgrader->isTraining()==false,{fail=true;return;});
    BWAssertF(upgrader->isResearching()==false,{fail=true;return;});
    BWAssertF(upgrader->isUpgrading()==true,{fail=true;return;});
    BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;return;});
    BWAssertF(upgrader->getUpgrade()==upgradeType,{fail=true;return;});
    BWAssertF(upgrader->isIdle()==false,{fail=true;return;});
  }
  if (thisFrame==startFrame+200)
  {
    upgrader->cancelUpgrade();
  }
  if (thisFrame>=startFrame+200)
  {
    BWAssertF(upgrader->getBuildType()==UnitTypes::None,{fail=true;return;});
    BWAssertF(upgrader->isMorphing()==false,{fail=true;return;});
    BWAssertF(upgrader->isConstructing()==false,{fail=true;return;});
    BWAssertF(upgrader->isTraining()==false,{fail=true;return;});
    BWAssertF(upgrader->isResearching()==false,{fail=true;return;});
    BWAssertF(upgrader->isUpgrading()==false,{fail=true;return;});
    BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;return;});
    BWAssertF(upgrader->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
    BWAssertF(upgrader->isIdle()==true,{fail=true;return;});
    BWAssertF(upgrader->getRemainingBuildTime()==0,{fail=true;return;});
    BWAssertF(upgrader->getRemainingTrainTime()==0,{fail=true;return;});
    BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;return;});
    BWAssertF(upgrader->getRemainingUpgradeTime()==0,{fail=true;return;});
    BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{fail=true;return;});
    BWAssertF(Broodwar->self()->gas()==correctGasCount,{fail=true;return;});
    BWAssertF(Broodwar->self()->supplyUsed()==correctSupplyUsedCount,{fail=true;return;});
  }
  if (thisFrame>=startFrame+400)
  {
    running = false;
  }
}

void CancelUpgradeTest::stop()
{
}
