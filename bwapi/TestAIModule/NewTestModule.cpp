#include "NewTestModule.h"
#include "NewTestModuleFilters.h"
#include "Assertions.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

Player self;

void DebugUnit(Unit pUnit, const char * const fxn)
{
  Broodwar << fxn << ": " << pUnit->getType() << " owned by " << (pUnit->getPlayer() ? pUnit->getPlayer()->getName() : "unknown") << endl;
}

struct BuildAction
{
  UnitType type;
  BuildAction(UnitType t) : type(t) {}
  void operator()(Unit u)
  {
    TilePosition targPos = Broodwar->getBuildLocation(type, TilePosition(u->getPosition()) );
    if ( Broodwar->getLastError() != Errors::None )
      Broodwar << Broodwar->getLastError() << endl;
    
    u->build(type, targPos );
    if ( Broodwar->getLastError() != Errors::None )
      Broodwar << Broodwar->getLastError() << endl;
    
    Position topLeft(targPos), botRight(targPos + type.tileSize());
    Broodwar->registerEvent( [=](Game*){ Broodwar->drawBoxMap(topLeft,botRight, Colors::Green);}, nullptr, type.buildTime()/10 + 100 );
  }
};

struct CompleteAction
{
  UnitFilter completionCmp;
  CompleteAction(const UnitFilter &c) : completionCmp(c) {}
  void operator()(Unit u)
  {
    Broodwar << ((!completionCmp.isValid() || completionCmp(u)) ? "PASS" : "FAIL") << endl;
  }
};

void addTestCase(Unit pUnit, const std::function<void(Unit )> &action, const UnitFilter &completion, int timeout)
{
  if ( !pUnit )
    return;
  
  pUnit->registerEvent(action, IsCompleted && IsIdle, 1, 6);
  pUnit->registerEvent(CompleteAction(completion), Timeout(timeout), 1, 6);
}

void NewTestModule::onStart()
{
  Broodwar->enableFlag(Flag::UserInput);
  self = Broodwar->self();

  Assert.start();

  // Issue 489
  Broodwar->canBuildHere(TilePosition(Broodwar->mapWidth()-4,10), UnitTypes::Terran_Barracks) ? Assert.Pass() : Assert.Fail(__FILE__, __LINE__);
  !Broodwar->canBuildHere(TilePosition(Broodwar->mapWidth()-3,10), UnitTypes::Terran_Barracks) ? Assert.Pass() : Assert.Fail(__FILE__, __LINE__);
  Broodwar->canBuildHere(TilePosition(Broodwar->mapWidth()-4,Broodwar->mapHeight()-5), UnitTypes::Terran_Barracks) ? Assert.Pass() : Assert.Fail(__FILE__, __LINE__);
  !Broodwar->canBuildHere(TilePosition(Broodwar->mapWidth()-3,Broodwar->mapHeight()-5), UnitTypes::Terran_Barracks) ? Assert.Pass() : Assert.Fail(__FILE__, __LINE__);
}
void NewTestModule::onEnd(bool isWinner)
{
}
void NewTestModule::onFrame()
{
}
void NewTestModule::onSendText(std::string text)
{
}
void NewTestModule::onReceiveText(BWAPI::Player player, std::string text)
{
}
void NewTestModule::onPlayerLeft(BWAPI::Player player)
{
}
void NewTestModule::onNukeDetect(BWAPI::Position target)
{
}
void NewTestModule::onUnitDiscover(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onUnitEvade(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onUnitShow(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onUnitHide(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onUnitCreate(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
  if ( unit->getPlayer() == self )
  {
    if ( unit->getType().isWorker() )
    {
      UnitType toBuild = unit->getType().getRace().getCenter();
      addTestCase(unit, BuildAction(toBuild), Owns(1, toBuild), toBuild.buildTime()/10 + 100);
    }
    else if ( unit->isSieged() )
    {
      auto myUnits = unit->getUnitsInWeaponRange(WeaponTypes::Arclite_Shock_Cannon);
      TEST(Assert, std::find_if(myUnits.begin(), myUnits.end(), Filter::GetType != UnitTypes::Terran_Civilian) != myUnits.end() || myUnits.size() != 3 );
    }
  }
}
void NewTestModule::onUnitDestroy(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onUnitMorph(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onUnitRenegade(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}
void NewTestModule::onSaveGame(std::string gameName)
{
}
void NewTestModule::onUnitComplete(BWAPI::Unit unit)
{
  //DebugUnit(unit, __FUNCTION__);
}

