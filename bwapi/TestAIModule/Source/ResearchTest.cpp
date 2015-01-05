#include "ResearchTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
ResearchTest::ResearchTest(BWAPI::TechType techType) : techType(techType),
                                                       researcherType(techType.whatResearches()),
                                                       startResearchFrame(-1),
                                                       nextUpdateFrame(-1),
                                                       researcher(NULL),
                                                       correctMineralCount(0),
                                                       correctGasCount(0)
{
  fail = false;
  running = false;
  BWAssertF(researcherType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(researcherType!=UnitTypes::Unknown,{fail=true;return;});
}
void ResearchTest::start()
{
  if (fail) return;
  running = true;

  int researcherCount = Broodwar->self()->completedUnitCount(researcherType);
  BWAssertF(researcherCount>=1,{fail=true;return;});
  for (Unit u : Broodwar->self()->getUnits())
  {
    if (u->getType()==researcherType)
    {
      researcher = u;
      break;
    }
  }
  BWAssertF(researcher!=NULL,{fail=true;return;});

  BWAssertF(researcher->getOrder()==Orders::Nothing,{fail=true;return;});
  BWAssertF(researcher->isIdle()==true,{fail=true;return;});
  BWAssertF(researcher->isLifted()==false,{fail=true;return;});
  BWAssertF(researcher->isResearching()==false,{fail=true;return;});
  BWAssertF(researcher->isUpgrading()==false,{fail=true;return;});
  BWAssertF(researcher->getTech()==TechTypes::None,{fail=true;return;});
  BWAssertF(researcher->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  BWAssertF(researcher->getRemainingResearchTime()==0,{fail=true;return;});
  BWAssertF(Broodwar->self()->hasResearched(techType)==false,{fail=true;return;});
  correctMineralCount = Broodwar->self()->minerals() - techType.mineralPrice();
  correctGasCount = Broodwar->self()->gas() - techType.gasPrice();

  researcher->research(techType);

  BWAssertF(researcher->getOrder()==Orders::ResearchTech,{fail=true;return;});
  BWAssertF(researcher->isIdle()==false,{fail=true;return;});
  BWAssertF(researcher->isLifted()==false,{fail=true;return;});
  BWAssertF(researcher->isResearching()==true,{fail=true;return;});
  BWAssertF(researcher->isUpgrading()==false,{fail=true;return;});
  BWAssertF(researcher->getTech()==techType,{fail=true;return;});
  BWAssertF(researcher->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  BWAssertF(researcher->getRemainingResearchTime()==techType.researchTime(),
  {
    Broodwar->printf("%d %d",researcher->getRemainingResearchTime(),techType.researchTime()/10);
    fail=true;
    return;
  });
  BWAssertF(Broodwar->self()->hasResearched(techType)==false,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});

  startResearchFrame = Broodwar->getFrameCount();
  nextUpdateFrame = startResearchFrame;

}
void ResearchTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextUpdateFrame);
  BWAssertF(researcher!=NULL,{fail=true;return;});
  nextUpdateFrame++;
  Broodwar->setScreenPosition(researcher->getPosition() - Position(320,240));
  int correctRemainingResearchTime = startResearchFrame + Broodwar->getLatency() + techType.researchTime()/10 - thisFrame;
  if (correctRemainingResearchTime > techType.researchTime()/10)
    correctRemainingResearchTime = techType.researchTime()/10;
  if (correctRemainingResearchTime < 0)
    correctRemainingResearchTime = 0;

  // @TODO: Workaround
  if ( thisFrame <= startResearchFrame + Broodwar->getLatency() )
  {
    BWAssertF(researcher->getRemainingResearchTime()/10 == correctRemainingResearchTime,{log("%d %d",researcher->getRemainingResearchTime()/10, correctRemainingResearchTime);});
  }
  else
  {
    BWAssertF(researcher->getRemainingResearchTime() == correctRemainingResearchTime,{log("%d %d",researcher->getRemainingResearchTime(), correctRemainingResearchTime);});
  }

  int lastFrame = startResearchFrame + Broodwar->getLatency() + techType.researchTime()/10;
  if (thisFrame > lastFrame) //terminate condition
  {
    running = false;
    return;
  }

  BWAssertF(researcher->getOrder()==Orders::ResearchTech,{fail=true;return;});
  BWAssertF(researcher->isIdle()==false,{fail=true;return;});
  BWAssertF(researcher->isLifted()==false,{fail=true;return;});
  BWAssertF(researcher->isResearching()==true,{fail=true;return;});
  BWAssertF(researcher->isUpgrading()==false,{fail=true;return;});
  BWAssertF(researcher->getTech()==techType,{fail=true;return;});
  BWAssertF(researcher->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  BWAssertF(Broodwar->self()->hasResearched(techType)==false,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
}

void ResearchTest::stop()
{
  if (fail == true) return;
  BWAssertF(researcher!=NULL,{fail=true;return;});
  BWAssertF(researcher->getOrder()==Orders::Nothing,{fail=true;return;});
  BWAssertF(researcher->isIdle()==true,{fail=true;return;});
  BWAssertF(researcher->isLifted()==false,{fail=true;return;});
  BWAssertF(researcher->isResearching()==false,{fail=true;return;});
  BWAssertF(researcher->isUpgrading()==false,{fail=true;return;});
  BWAssertF(researcher->getTech()==TechTypes::None,{fail=true;return;});
  BWAssertF(researcher->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
  BWAssertF(researcher->getRemainingResearchTime()==0,{fail=true;return;});
  BWAssertF(Broodwar->self()->hasResearched(techType)==true,{fail=true;return;});
  BWAssertF(Broodwar->self()->minerals() == correctMineralCount,{fail=true;return;});
  BWAssertF(Broodwar->self()->gas() == correctGasCount,{fail=true;return;});
}
