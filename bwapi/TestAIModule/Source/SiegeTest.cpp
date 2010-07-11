#include "SiegeTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
SiegeTest::SiegeTest() : running(false),
                         fail(false),
                         unit(NULL),
                         startFrame(-1),
                         nextFrame(-1),
                         completedSiege(false)
{

}
void SiegeTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==UnitTypes::Terran_Siege_Tank_Tank_Mode)
      unit = u;

  BWAssertF(unit!=NULL,{fail=true;return;});
  BWAssertF(unit->exists(),{fail=true;return;});
  BWAssertF(unit->isSieged()==false,{fail=true;return;});
  unit->siege();
  BWAssertF(unit->getOrder()==Orders::SiegeMode,{fail=true;return;});
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();

}
void SiegeTest::update()
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
  if (completedSiege==false)
  {
    if (unit->getOrder()!=Orders::SiegeMode)
    {
      BWAssertF(unit->isSieged()==true,{fail=true;return;});
      BWAssertF(unit->getType()==UnitTypes::Terran_Siege_Tank_Siege_Mode,{fail=true;return;});
      completedSiege=true;
      unit->unsiege();
      BWAssertF(unit->getOrder()==Orders::TankMode,{fail=true;return;});
    }
    else
    {
      if (thisFrame>startFrame+300)
        fail = true;
    }
  }
  else
  {
    if (unit->getOrder()!=Orders::TankMode)
    {
      BWAssertF(unit->isSieged()==false,{fail=true;return;});
      BWAssertF(unit->getType()==UnitTypes::Terran_Siege_Tank_Tank_Mode,{fail=true;return;});
      running = false;
    }
    else
    {
      if (thisFrame>startFrame+300)
        fail = true;
    }
  }
}

void SiegeTest::stop()
{
}

bool SiegeTest::isRunning()
{
  return running;
}