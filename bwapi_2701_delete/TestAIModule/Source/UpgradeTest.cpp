#include "UpgradeTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
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
  BWAssertF(upgraderCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    if (u->getType()==upgraderType)
    {
      upgrader = u;
      break;
    }
  }
  BWAssertF(upgrader!=NULL,{fail=true;return;});

  BWAssertF(upgrader->getOrder()==Orders::Nothing,{fail=true;return;});
  BWAssertF(upgrader->isIdle()==true,{fail=true;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;return;});
  BWAssertF(upgrader->isUpgrading()==false,{fail=true;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;return;});
  BWAssertF(upgrader->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;return;});
  BWAssertF(upgrader->getRemainingUpgradeTime()==0,{fail=true;return;});
  previousUpgradeLevel = Broodwar->self()->getUpgradeLevel(upgradeType);
  correctMineralCount = Broodwar->self()->minerals() - (upgradeType.mineralPriceBase()+upgradeType.mineralPriceFactor()*previousUpgradeLevel);
  correctGasCount = Broodwar->self()->gas() - (upgradeType.gasPriceBase()+upgradeType.gasPriceFactor()*previousUpgradeLevel);

  upgrader->upgrade(upgradeType);

  BWAssertF(upgrader->getOrder()==Orders::Upgrade,{fail=true;return;});
  BWAssertF(upgrader->isIdle()==false,{fail=true;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;return;});
  BWAssertF(upgrader->isUpgrading()==true,{fail=true;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;return;});
  BWAssertF(upgrader->getUpgrade()==upgradeType,{fail=true;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;return;});
  BWAssertF(upgrader->getRemainingUpgradeTime()==upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*previousUpgradeLevel,{fail=true;return;});
  BWAssertF(Broodwar->self()->getUpgradeLevel(upgradeType)==previousUpgradeLevel,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});

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
  BWAssertF(upgrader!=NULL,{fail=true;return;});
  nextUpdateFrame++;
  Broodwar->setScreenPosition(upgrader->getPosition().x()-320,upgrader->getPosition().y()-240);
  int correctRemainingUpgradeTime = startUpgradeFrame+Broodwar->getLatency()+upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*previousUpgradeLevel-thisFrame;
  if (correctRemainingUpgradeTime>upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*previousUpgradeLevel)
    correctRemainingUpgradeTime=upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*previousUpgradeLevel;
  if (correctRemainingUpgradeTime<0)
    correctRemainingUpgradeTime=0;
  BWAssertF(upgrader->getRemainingUpgradeTime() == correctRemainingUpgradeTime,{Broodwar->printf("%d %d",upgrader->getRemainingUpgradeTime(), correctRemainingUpgradeTime);});
  int lastFrame = startUpgradeFrame+Broodwar->getLatency()+upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*previousUpgradeLevel;
  if (thisFrame>lastFrame) //terminate condition
  {
    running = false;
    return;
  }
  BWAssertF(upgrader->getOrder()==Orders::Upgrade,{fail=true;return;});
  BWAssertF(upgrader->isIdle()==false,{fail=true;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;return;});
  BWAssertF(upgrader->isUpgrading()==true,{fail=true;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;return;});
  BWAssertF(upgrader->getUpgrade()==upgradeType,{fail=true;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;return;});
  BWAssertF(Broodwar->self()->getUpgradeLevel(upgradeType)==previousUpgradeLevel,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
}

void UpgradeTest::stop()
{
  if (fail == true) return;
  BWAssertF(upgrader!=NULL,{fail=true;return;});
  BWAssertF(upgrader->getOrder()==Orders::Nothing,{fail=true;return;});
  BWAssertF(upgrader->isIdle()==true,{fail=true;return;});
  BWAssertF(upgrader->isLifted()==false,{fail=true;return;});
  BWAssertF(upgrader->isResearching()==false,{fail=true;return;});
  BWAssertF(upgrader->isUpgrading()==false,{fail=true;return;});
  BWAssertF(upgrader->getTech()==TechTypes::None,{fail=true;return;});
  BWAssertF(upgrader->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  BWAssertF(upgrader->getRemainingResearchTime()==0,{fail=true;return;});
  BWAssertF(upgrader->getRemainingUpgradeTime()==0,{fail=true;return;});
  BWAssertF(Broodwar->self()->getUpgradeLevel(upgradeType)==previousUpgradeLevel+1,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
}
