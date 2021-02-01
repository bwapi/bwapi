#include "CancelResearchTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
CancelResearchTest::CancelResearchTest(BWAPI::TechType techType) : techType(techType),
                                                                   researcherType(techType.whatResearches()),
                                                                   startFrame(-1),
                                                                   nextFrame(-1),
                                                                   researcher(NULL),
                                                                   correctMineralCount(0),
                                                                   correctGasCount(0),
                                                                   correctSupplyUsedCount(0)
{
  fail = false;
  running = false;
  BWAssertF(researcherType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(researcherType!=UnitTypes::Unknown,{fail=true;return;});
}
void CancelResearchTest::start()
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
  correctMineralCount = Broodwar->self()->minerals();
  correctGasCount = Broodwar->self()->gas();
  correctSupplyUsedCount = Broodwar->self()->supplyUsed();
  researcher->research(techType);

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;

}
void CancelResearchTest::update()
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
  Broodwar->setScreenPosition(researcher->getPosition() - Position(320,240));
  BWAssertF(researcher!=NULL,{fail=true;return;});
  if (thisFrame<startFrame+20)
  {
    BWAssertF(researcher->getBuildType()==UnitTypes::None,{fail=true;return;});
    BWAssertF(researcher->isMorphing()==false,{fail=true;return;});
    BWAssertF(researcher->isConstructing()==false,{fail=true;return;});
    BWAssertF(researcher->isTraining()==false,{fail=true;return;});
    BWAssertF(researcher->isResearching()==true,{fail=true;return;});
    BWAssertF(researcher->isUpgrading()==false,{fail=true;return;});
    BWAssertF(researcher->getTech()==techType,{fail=true;return;});
    BWAssertF(researcher->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
    BWAssertF(researcher->isIdle()==false,{fail=true;return;});
  }
  if (thisFrame==startFrame+20)
  {
    researcher->cancelResearch();
  }
  if (thisFrame>=startFrame+20)
  {
    BWAssertF(researcher->getBuildType()==UnitTypes::None,{fail=true;return;});
    BWAssertF(researcher->isMorphing()==false,{fail=true;return;});
    BWAssertF(researcher->isConstructing()==false,{fail=true;return;});
    BWAssertF(researcher->isTraining()==false,{fail=true;return;});
    BWAssertF(researcher->isResearching()==false,{fail=true;return;});
    BWAssertF(researcher->isUpgrading()==false,{fail=true;return;});
    BWAssertF(researcher->getTech()==TechTypes::None,{fail=true;return;});
    BWAssertF(researcher->getUpgrade()==UpgradeTypes::None,{fail=true;return;});
    BWAssertF(researcher->isIdle()==true,{fail=true;return;});
    BWAssertF(researcher->getRemainingBuildTime()==0,{fail=true;return;});
    BWAssertF(researcher->getRemainingTrainTime()==0,{fail=true;return;});
    BWAssertF(researcher->getRemainingResearchTime()==0,{fail=true;return;});
    BWAssertF(researcher->getRemainingUpgradeTime()==0,{fail=true;return;});
    BWAssertF(Broodwar->self()->minerals()==correctMineralCount,{fail=true;return;});
    BWAssertF(Broodwar->self()->gas()==correctGasCount,{fail=true;return;});
    BWAssertF(Broodwar->self()->supplyUsed()==correctSupplyUsedCount,{fail=true;return;});
  }
  if (thisFrame>=startFrame+40)
  {
    running = false;
  }
}

void CancelResearchTest::stop()
{
}
