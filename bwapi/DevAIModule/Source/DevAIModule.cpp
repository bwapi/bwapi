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

  for each ( Unit *u in self->getUnits() )
  {
    if ( !u->isIdle() || bw->getFrameCount() <= u->getLastCommandFrame() + bw->getLatencyFrames() )
      continue;
    if ( u->getType() == UnitTypes::Protoss_Reaver || u->getType() == UnitTypes::Zerg_Sunken_Colony || u->getType() == UnitTypes::Protoss_Carrier )
    {
      for each ( Unit *atk in u->getUnitsInRadius(200) )
      {
        if ( !atk->isInvincible() && u->attack(atk) )
        {
          bw->printf("Attemted attack of %s: %s", atk->getType().getName().c_str(), bw->getLastError().toString().c_str() );
          break;
        }
        else
          bw->printf("Failed attack of %s: %s", atk->getType().getName().c_str(), bw->getLastError().toString().c_str() );
      }
    }
  }
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
