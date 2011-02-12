#include "DevAIModule.h"
#include <set>

using namespace BWAPI;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  enabled = true;

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();
}

void DevAIModule::onEnd(bool isWinner)
{

}

DWORD dwLastTickCount;
void DevAIModule::onFrame()
{
  if ( bw->isReplay() )
    return;

  if ( !enabled )
    return;

  if ( bw->getFrameCount() % 2 )
    return;

  std::set<Unit*> alreadyCast;
  for each( Unit *u in self->getUnits() )
  {
    if ( !u->exists() || u->getSpellCooldown() || u->getOrderTarget() || !u->isCompleted() || bw->getFrameCount() - u->getLastCommandFrame() <= bw->getLatencyFrames() )
      continue;

    UnitType type = u->getType();
    if ( type == UnitTypes::Protoss_Dark_Archon )
    {
      if ( !self->hasResearched(TechTypes::Mind_Control) )
        continue;

      Position pos = u->getPosition();
      //std::set<Unit*> targs = bw->getUnitsInRectangle(pos.x()-256, pos.y()-256, pos.x()+256, pos.y()+256);
      for each (Unit *t in bw->getAllUnits())
      {
        if ( !t || !t->exists() || (self->isAlly(t->getPlayer()) && !t->getPlayer()->isNeutral()) || !t->isDetected() || t->isInvincible() )
          continue;

        UnitType targType = t->getType();
        if ( targType.isBuilding() ||
             targType == UnitTypes::Terran_Vulture_Spider_Mine ||
             targType == UnitTypes::Protoss_Interceptor ||
             targType == UnitTypes::Zerg_Larva ||
             targType == UnitTypes::Zerg_Cocoon ||
             targType == UnitTypes::Zerg_Egg ||
             targType == UnitTypes::Zerg_Lurker_Egg)
          continue;

        if ( u->getDistance(t) > 256 )
          continue;

        if ( alreadyCast.find(t) == alreadyCast.end() )
        {
          if ( u->useTech(TechTypes::Mind_Control, t) )
          {
            alreadyCast.insert(t);
            break;
          }
        }
      }// for each target
    }// is dark archon
  }// for each self units
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/t" )
  {
    enabled = !enabled;
    Broodwar->printf("DevAITest %s", enabled ? "ENABLED" : "DISABLED");
  }
  else if ( text == "/wiki" )
  {
    writeUnitWiki();
    writeWeaponWiki();
    Broodwar->printf("Generated wiki pages!");
  }
  else
  {
    Broodwar->sendText("%s", text.c_str());
  }
}

void DevAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
}

void DevAIModule::onPlayerLeft(BWAPI::Player* player)
{
}

void DevAIModule::onNukeDetect(BWAPI::Position target)
{
}

void DevAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitEvade(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitShow(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitHide(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitCreate(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitMorph(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
}

void DevAIModule::onSaveGame(std::string gameName)
{
}
