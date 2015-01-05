#include "UpgradeTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
#define LOGTYPE log("UpgradeType: %s", upgradeType.c_str())
UpgradeTest::UpgradeTest(BWAPI::UpgradeType upgradeType) : upgradeType(upgradeType),
                                                           upgraderType(upgradeType.whatUpgrades()),
                                                           previousUpgradeLevel(0),
                                                           startUpgradeFrame(-1),
                                                           nextUpdateFrame(-1),
                                                           upgrader(NULL),
                                                           correctMineralCount(0),
                                                           correctGasCount(0)
{
  fail = false;
  running = false;
  BWAssertF(upgraderType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(upgraderType!=UnitTypes::Unknown,{fail=true;return;});
}
void UpgradeTest::start()
{
  if (fail) return;
  running = true;

  int upgraderCount = Broodwar->self()->completedUnitCount(upgraderType);
  BWAssertF(upgraderCount>=1,{fail=true;LOGTYPE;return;});
  for (Unit u : Broodwar->self()->getUnits())
  {
    if (u->getType()==upgraderType)
    {
      upgrader = u;
      break;
    }
  }
  BWAssertF(upgrader!=NULL,{fail=true;LOGTYPE;return;});

  BWAssertF(upgrader->getOrder()==Orders::Nothing,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isIdle()==true,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isUpgrading()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getUpgrade()==UpgradeTypes::None,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingUpgradeTime()==0,{fail=true;LOGTYPE;return;});
  previousUpgradeLevel = Broodwar->self()->getUpgradeLevel(upgradeType);
  correctMineralCount  = Broodwar->self()->minerals() - upgradeType.mineralPrice(previousUpgradeLevel+1);
  correctGasCount      = Broodwar->self()->gas() - upgradeType.gasPrice(previousUpgradeLevel+1);

  upgrader->upgrade(upgradeType);

  BWAssertF(upgrader->getOrder()==Orders::Upgrade,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isIdle()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isUpgrading()==true,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getUpgrade()==upgradeType,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingUpgradeTime()==upgradeType.upgradeTime(previousUpgradeLevel+1),{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->getUpgradeLevel(upgradeType)==previousUpgradeLevel,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;LOGTYPE;return;});

  startUpgradeFrame = Broodwar->getFrameCount();
  nextUpdateFrame = startUpgradeFrame;

}
void UpgradeTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextUpdateFrame);
  BWAssertF(upgrader!=NULL,{fail=true;LOGTYPE;return;});
  nextUpdateFrame++;
  Broodwar->setScreenPosition(upgrader->getPosition() - Position(320,240));
  
  // NOTE: /15*15 is to account for the editor not displaying the real times
  int expectedUpgradeTime = (upgradeType.upgradeTime()/15/10*15) + (upgradeType.upgradeTimeFactor()/15/10*15) * (std::max(0,previousUpgradeLevel+1-1));

  int correctRemainingUpgradeTime = startUpgradeFrame + Broodwar->getLatency() + expectedUpgradeTime - thisFrame;
  if (correctRemainingUpgradeTime > expectedUpgradeTime)
    correctRemainingUpgradeTime = expectedUpgradeTime;
  if (correctRemainingUpgradeTime < 0)
    correctRemainingUpgradeTime = 0;

  // @TODO: Workaround
  if ( thisFrame <= startUpgradeFrame + Broodwar->getLatency() )
  {
    BWAssertF(upgrader->getRemainingUpgradeTime() == upgradeType.upgradeTime(previousUpgradeLevel+1),{log("%d %d",upgrader->getRemainingUpgradeTime(), upgradeType.upgradeTime(previousUpgradeLevel+1));});
  }
  else
  {
    BWAssertF(upgrader->getRemainingUpgradeTime() == correctRemainingUpgradeTime,{log("%d %d",upgrader->getRemainingUpgradeTime(), correctRemainingUpgradeTime);});
  }

  int lastFrame = startUpgradeFrame+Broodwar->getLatency() + expectedUpgradeTime;
  if (thisFrame>lastFrame) //terminate condition
  {
    running = false;
    return;
  }
  BWAssertF(upgrader->getOrder()==Orders::Upgrade,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isIdle()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isUpgrading()==true,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getUpgrade()==upgradeType,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->getUpgradeLevel(upgradeType)==previousUpgradeLevel,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;LOGTYPE;return;});
}

void UpgradeTest::stop()
{
  if (fail == true) return;
  BWAssertF(upgrader!=NULL,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getOrder()==Orders::Nothing,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isIdle()==true,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->isUpgrading()==false,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getUpgrade()==UpgradeTypes::None,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;LOGTYPE;return;});
  BWAssertF(upgrader->getRemainingUpgradeTime()==0,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->getUpgradeLevel(upgradeType)==previousUpgradeLevel+1,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;LOGTYPE;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;LOGTYPE;return;});
}
