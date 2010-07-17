#include "TransportTest.h"
#include "BWAssert.h"
using namespace std;
using namespace BWAPI;
TransportTest::TransportTest(BWAPI::UnitType unitType) : transportType(unitType),
                                                         transport(NULL),
                                                         currentUnit(NULL)
{
  fail = false;
  running = false;
}
void TransportTest::start()
{
  if (fail) return;
  running = true;

  int transportCount = Broodwar->self()->completedUnitCount(transportType);
  BWAssertF(transportCount>=1,{fail=true;return;});
  for each(Unit* u in Broodwar->self()->getUnits())
    if (u->getType()==transportType)
      transport = u;

  if (transportType.getRace()==Races::Terran)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Terran_Marine ||
          u->getType()==UnitTypes::Terran_Firebat ||
          u->getType()==UnitTypes::Terran_Ghost ||
          u->getType()==UnitTypes::Terran_Medic)
        unloadedUnits.insert(u);
  }
  else if (transportType.getRace()==Races::Protoss)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Protoss_Probe ||
          u->getType()==UnitTypes::Protoss_Zealot ||
          u->getType()==UnitTypes::Protoss_Dragoon)
        unloadedUnits.insert(u);
  }
  else if (transportType.getRace()==Races::Zerg)
  {
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Zerg_Drone ||
          u->getType()==UnitTypes::Zerg_Hydralisk ||
          u->getType()==UnitTypes::Zerg_Lurker)
        unloadedUnits.insert(u);
  }
  BWAssertF(transport!=NULL,{fail=true;return;});
  BWAssertF(transport->exists(),{fail=true;return;});
  BWAssertF(transport->getLoadedUnits().size()==0,{fail=true;return;});
  BWAssertF(unloadedUnits.size()==4,{fail=true;return;});
  for each(Unit* u in unloadedUnits)
  {
    BWAssertF(u!=NULL,{fail=true;return;});
    BWAssertF(u->exists()==true,{fail=true;return;});
    BWAssertF(u->isLoaded()==false,{fail=true;return;});
    BWAssertF(u->getTransport()==NULL,{fail=true;return;});
  }
  startFrame = Broodwar->getFrameCount();
  nextFrame = Broodwar->getFrameCount();
  state = Start;
}
bool TransportTest::verifyLoadedUnits()
{
  BWAssertF(transport!=NULL,{fail=true;return false;});
  std::set<Unit*> actualLoadedUnits = transport->getLoadedUnits();
  BWAssertF(actualLoadedUnits.size()==loadedUnits.size(),{fail=true;return false;});
  for each(Unit* u in loadedUnits)
  {
    BWAssertF(actualLoadedUnits.find(u)!=actualLoadedUnits.end(),{fail=true;return false;});
    BWAssertF(u->isLoaded()==true,{fail=true;return false;});
    BWAssertF(u->getTransport()==transport,{fail=true;return false;});
    BWAssertF(Broodwar->getAllUnits().find(u)!=Broodwar->getAllUnits().end(),{fail=true;return false;});
  }
  for each(Unit* u in unloadedUnits)
  {
    BWAssertF(u->isLoaded()==false,{fail=true;return false;});
    BWAssertF(u->getTransport()==NULL,{fail=true;return false;});
    BWAssertF(Broodwar->getAllUnits().find(u)!=Broodwar->getAllUnits().end(),{fail=true;return false;});
  }
  return true;
}

void TransportTest::update()
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
  Broodwar->setScreenPosition(transport->getPosition().x()-320,transport->getPosition().y()-240);
  if (state == Start)
  {
    state = WaitForFirstUnit;
    currentUnit = *unloadedUnits.begin();
    transport->load(currentUnit);
    BWAssertF(unloadedUnits.size()==4,{fail=true;return;});
    BWAssertF(loadedUnits.size()==0,{fail=true;return;});
    state=WaitForFirstUnit;
  }
  else if (state == WaitForFirstUnit)
  {
    if (currentUnit->isLoaded())
    {
      loadedUnits.insert(currentUnit);
      unloadedUnits.erase(currentUnit);
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==3,{fail=true;return;});
      BWAssertF(loadedUnits.size()==1,{fail=true;return;});
      currentUnit = *unloadedUnits.begin();
      transport->load(currentUnit);
      state=WaitForSecondUnit;
    }
  }
  else if (state == WaitForSecondUnit)
  {
    if (currentUnit->isLoaded())
    {
      loadedUnits.insert(currentUnit);
      unloadedUnits.erase(currentUnit);
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==2,{fail=true;return;});
      BWAssertF(loadedUnits.size()==2,{fail=true;return;});
      currentUnit = *unloadedUnits.begin();
      transport->load(currentUnit);
      state=WaitForThirdUnit;
    }
  }
  else if (state == WaitForThirdUnit)
  {
    if (currentUnit->isLoaded())
    {
      loadedUnits.insert(currentUnit);
      unloadedUnits.erase(currentUnit);
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==1,{fail=true;return;});
      BWAssertF(loadedUnits.size()==3,{fail=true;return;});
      currentUnit = *unloadedUnits.begin();
      transport->load(currentUnit);
      state=WaitForFourthUnit;
    }
  }
  else if (state == WaitForFourthUnit)
  {
    if (currentUnit->isLoaded())
    {
      loadedUnits.insert(currentUnit);
      unloadedUnits.erase(currentUnit);
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==0,{fail=true;return;});
      BWAssertF(loadedUnits.size()==4,{fail=true;return;});
      state=WaitDelay1;
      waitFrameStart=thisFrame;
      
    }
  }
  else if (state == WaitDelay1)
  {
    if (thisFrame>waitFrameStart+50)
    {
      currentUnit = *loadedUnits.begin();
      transport->unload(currentUnit);
      state=WaitUnloadUnit;
    }
  }
  else if (state == WaitUnloadUnit)
  {
    if (currentUnit->isLoaded()==false)
    {
      unloadedUnits.insert(currentUnit);
      loadedUnits.erase(currentUnit);
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==1,{fail=true;return;});
      BWAssertF(loadedUnits.size()==3,{fail=true;return;});
      transport->load(currentUnit);
      state=WaitReloadUnit;
    }
  }
  else if (state == WaitReloadUnit)
  {
    if (currentUnit->isLoaded())
    {
      loadedUnits.insert(currentUnit);
      unloadedUnits.erase(currentUnit);
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==0,{fail=true;return;});
      BWAssertF(loadedUnits.size()==4,{fail=true;return;});
      transport->unloadAll();
      state=WaitUnloadAll;
    }
  }
  else if (state == WaitUnloadAll)
  {
    if (transport->getLoadedUnits().size()==0)
    {
      unloadedUnits=loadedUnits;
      loadedUnits.clear();
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==4,{fail=true;return;});
      BWAssertF(loadedUnits.size()==0,{fail=true;return;});
      for each(Unit* u in unloadedUnits)
      {
        u->load(transport);
      }
      state=WaitReloadAll;
    }
  }
  else if (state == WaitReloadAll)
  {
    if (transport->getLoadedUnits().size()==4)
    {
      loadedUnits=unloadedUnits;
      unloadedUnits.clear();
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==0,{fail=true;return;});
      BWAssertF(loadedUnits.size()==4,{fail=true;return;});
      transport->unloadAll(Position(transport->getPosition().x()-32*10,transport->getPosition().y()));
      state=WaitUnloadAllPosition;
    }
  }
  else if (state == WaitUnloadAllPosition)
  {
    if (transport->getLoadedUnits().size()==0)
    {
      unloadedUnits=loadedUnits;
      loadedUnits.clear();
      if (verifyLoadedUnits()==false) return;
      BWAssertF(unloadedUnits.size()==4,{fail=true;return;});
      BWAssertF(loadedUnits.size()==0,{fail=true;return;});
      state=End;
    }
  }
  else if (state == End)
  {
    running = false;
  }
}
void TransportTest::stop()
{
}
