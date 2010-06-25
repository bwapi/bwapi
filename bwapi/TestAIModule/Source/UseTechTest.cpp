#include "UseTechTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
UseTechTest::UseTechTest(BWAPI::TechType techType) : techType(techType),
                                                     running(false),
                                                     fail(false),
                                                     user(NULL),
                                                     startFrame(-1),
                                                     nextFrame(-1)
{
  userType = *techType.whatUses().begin();
  BWAssertF(userType!=UnitTypes::None,{fail=true;return;});
  BWAssertF(userType!=UnitTypes::Unknown,{fail=true;return;});
}
int maxTimer = 0;
void UseTechTest::start()
{
  if (fail) return;
  running = true;

  int userCount = Broodwar->self()->completedUnitCount(userType);
  BWAssertF(userCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    if (u->getType()==userType)
    {
      user = u;
      break;
    }
  }
  BWAssertF(user!=NULL,{fail=true;return;});
  int previousEnergy=user->getEnergy();
  int previousHitPoints=user->getHitPoints();
  BWAssertF(previousEnergy>=techType.energyUsed(),{fail=true;return;});
  if (techType==TechTypes::Stim_Packs)
  {
    BWAssertF(previousHitPoints>10,{fail=true;return;});
  }

  BWAssertF(user->isIdle()==true,{fail=true;return;});

  user->useTech(techType);

  if (techType==TechTypes::Stim_Packs)
  {
    BWAssertF(user->getHitPoints()==previousHitPoints-10,
    {
      Broodwar->printf("%d!=%d",user->getHitPoints(),previousHitPoints-10);
      fail=true;
      return;
    });
  }
  BWAssertF(user->getEnergy()==previousEnergy-techType.energyUsed(),{fail=true;return;});

  startFrame = Broodwar->getFrameCount();
  nextFrame = startFrame;
  maxTimer = 0;

}
void UseTechTest::update()
{
  if (running == false) return;
  if (fail)
  {
    running = false;
    return;
  }
  int thisFrame = Broodwar->getFrameCount();
  BWAssert(thisFrame==nextFrame);
  BWAssertF(user!=NULL,{fail=true;return;});
  nextFrame++;
  Broodwar->setScreenPosition(user->getPosition().x()-320,user->getPosition().y()-240);
  Broodwar->drawTextMap(user->getPosition().x(),user->getPosition().y()+20,"stim: %d",user->getStimTimer());
  if (user->getStimTimer()>maxTimer)
    maxTimer=user->getStimTimer();
  Broodwar->printf("%d",maxTimer);
  int lastFrame = startFrame+300;
  /*
  if (techType==TechTypes::Stim_Packs)
  {
    lastFrame = startFrame+38*8+Broodwar->getLatency();
    int correctStimTimer = (lastFrame-thisFrame)/8;
    if (correctStimTimer<0) correctStimTimer=0;
    if (correctStimTimer>38) correctStimTimer=38;
//    BWAssert(user->getStimTimer()==correctStimTimer);
  }
  */
  if (thisFrame>lastFrame) //terminate condition
  {
    running = false;
    return;
  }
}

void UseTechTest::stop()
{
  if (fail == true) return;
}

bool UseTechTest::isRunning()
{
  return running;
}