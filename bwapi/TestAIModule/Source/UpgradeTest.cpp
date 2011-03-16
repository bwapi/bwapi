#include "UpgradeTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
#define LOGTYPE log("UpgradeType: %s", upgradeType.getName().c_str())
UpgradeTest::UpgradeTest(BWAPI::UpgradeType upgradeType) : upgradeType(upgradeType),
                                                           upgrader(NULL),
                                                           startUpgradeFrame(-1),
                                                           nextUpdateFrame(-1)
{
  fail = false;
  running = false;
  upgraderType = upgradeType.whatUpgrades();
  BWAssertF(upgraderType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(upgraderType!=UnitTypes::Unknown,{fail=true;return;});
}
void UpgradeTest::start()
{
  if (fail) return;
  running = true;

  int upgraderCount = Broodwar->self()->completedUnitCount(upgraderType);
  BWAssertF(upgraderCount>=1,{fail=true;LOGTYPE;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
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
  Broodwar->setScreenPosition(upgrader->getPosition().x()-320,upgrader->getPosition().y()-240);
  int correctRemainingUpgradeTime = startUpgradeFrame+Broodwar->getLatency()+upgradeType.upgradeTime(previousUpgradeLevel + 1) - thisFrame;
  if (correctRemainingUpgradeTime>upgradeType.upgradeTime(previousUpgradeLevel + 1))
    correctRemainingUpgradeTime=upgradeType.upgradeTime(previousUpgradeLevel + 1);
  if (correctRemainingUpgradeTime<0)
    correctRemainingUpgradeTime=0;
  BWAssertF(upgrader->getRemainingUpgradeTime() == correctRemainingUpgradeTime,{Broodwar->printf("%d %d",upgrader->getRemainingUpgradeTime(), correctRemainingUpgradeTime);});
  int lastFrame = startUpgradeFrame+Broodwar->getLatency() + upgradeType.upgradeTime(previousUpgradeLevel + 1);
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
