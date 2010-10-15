#include "CloakTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
CloakTest::CloakTest(UnitType unitType) : unitType(unitType),
                                          unit(NULL),
                                          startFrame(-1),
                                          nextFrame(-1),
                                          completedCloak(false)
{
  fail = false;
  running = false;
}
void CloakTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(unitType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==unitType)
      unit = u;

  BWAssertF(unit!=NULL,{fail=true;return;});
  BWAssertF(unit->exists(),{fail=true;return;});
  BWAssertF(unit->isCloaked()==false,{fail=true;return;});
  BWAssertF(unit->getEnergy()>=50,{fail=true;return;});
  currentEnergy = unit->getEnergy();
  unit->cloak();
  if (unitType==UnitTypes::Terran_Ghost)
    currentEnergy-=TechTypes::Personnel_Cloaking.energyUsed();
  else
    currentEnergy-=TechTypes::Cloaking_Field.energyUsed();
  BWAssertF(unit->getEnergy()==currentEnergy,{fail=true;return;});
  BWAssertF(unit->getOrder()==Orders::Cloak,{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();

}
void CloakTest::update()
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
  Broodwar->setScreenPosition(unit->getPosition().x()-320,unit->getPosition().y()-240);
  if (unit->getEnergy()!=currentEnergy)
  {
    currentEnergy++;
    BWAssertF(abs(unit->getEnergy()-currentEnergy)<5,{Broodwar->printf("%d != %d",unit->getEnergy(),currentEnergy);fail=true;return;});
    currentEnergy=unit->getEnergy();
  }

  if (completedCloak==false)
  {
    if (unit->getOrder()!=Orders::Cloak && thisFrame>startFrame+60)
    {
      BWAssertF(unit->isCloaked()==true,{fail=true;return;});
      completedCloak=true;
      unit->decloak();
      BWAssertF(unit->getOrder()==Orders::Decloak,{fail=true;return;});
      startFrame=thisFrame;
    }
    else
    {
      if (thisFrame>startFrame+300)
        fail = true;
    }
  }
  else
  {
    if (unit->getOrder()!=Orders::Decloak && thisFrame>startFrame+60)
    {
      BWAssertF(unit->isCloaked()==false,{fail=true;return;});
      running = false;
    }
    else
    {
      if (thisFrame>startFrame+300)
        fail = true;
    }
  }
}

void CloakTest::stop()
{
}
