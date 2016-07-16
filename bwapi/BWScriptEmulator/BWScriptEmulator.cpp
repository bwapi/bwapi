#include "BWScriptEmulator.h"
#include <vector>

#include "ScriptThread.h"
#include "Controller.h"
#include "UnitInfo.h"

using namespace BWAPI;

int mapH, mapW;
Player self;

bool isResourceClaimed[256];

bool farcasting;

DWORD dwCount = 0;

void BWScriptEmulator::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  bw->enableFlag(Flag::CompleteMapInformation);
  farcasting = true;

  // Save some BW info locally for easier access
  self = bw->self();
  mapW = bw->mapWidth();
  mapH = bw->mapHeight();

  // Access full map just like the normal AI
  if ( !bw->isMultiplayer() )
    bw->sendText("black sheep wall");

  if ( !self )
    return;
  
  if ( !LoadAIBinary("bwapi-data\\AISCRIPT.BIN") )
    Broodwar << Text::Red << "Failed to load AISCRIPT binary!" << std::endl;

  Race      selfRace(self->getRace());
  Position  sLoc(self->getStartLocation());

  srand(GetTickCount());

  /*
  // Run default melee script
  if      ( selfRace == Races::Zerg )
    AICreateThread("ZMCx", sLoc );
  else if ( selfRace == Races::Protoss )
    AICreateThread("PMCx", sLoc );
  else // ( selfRace == Races::Terran )
    AICreateThread("TMCx", sLoc );
    */

  // Run map test script if it has a 4-letter title
  std::string title = Broodwar->mapName();
  if ( title.size() == 4 )
    AICreateThread(title.c_str(), sLoc);
}

void BWScriptEmulator::onEnd(bool isWinner)
{
}

void BWScriptEmulator::onFrame()
{
  if ( bw->isReplay() )
    return;

  UpdateScripts();

  Unitset myUnits( self->getUnits() );
  for ( auto u : myUnits)
  {
    if ( !u->exists() ) continue;

    UnitWrap uw = u;
    uw.EmulateOrder();

    Broodwar->drawTextMap(u->getPosition(), "    %s", Order(uw.GetUnitOrder()).c_str() );
  }
  
  Unitset allUnits( Broodwar->getAllUnits() );
  for ( auto u : allUnits)
    Broodwar->drawTextMap(u->getPosition(), "\n    %s", u->getOrder().c_str());
  
}

void BWScriptEmulator::onSendText(std::string text)
{
  std::stringstream ss(text);
  std::string cmd;
  ss >> cmd;

  if ( cmd == "/fc" || cmd == "/farcast" )
  {
    farcasting = !farcasting;
    Broodwar << "Farcasting " << (farcasting ? "ENABLED" : "DISABLED") << std::endl;
  }
  else if ( cmd == "/r" || cmd == "/run" || cmd == "/script" )
  {
    std::string script;
    ss >> script;

    AICreateThread(script.c_str(), Broodwar->getScreenPosition() + Broodwar->getMousePosition() );
  }
  else
  {
    Broodwar->sendText("%s", text.c_str());
  }
}

void BWScriptEmulator::onReceiveText(BWAPI::Player player, std::string text)
{}

void BWScriptEmulator::onPlayerLeft(BWAPI::Player player)
{}

void BWScriptEmulator::onNukeDetect(BWAPI::Position target)
{}

void BWScriptEmulator::onUnitDiscover(BWAPI::Unit unit)
{}

void BWScriptEmulator::onUnitEvade(BWAPI::Unit unit)
{}

void BWScriptEmulator::onUnitShow(BWAPI::Unit unit)
{}

void BWScriptEmulator::onUnitHide(BWAPI::Unit unit)
{}

void BWScriptEmulator::onUnitCreate(BWAPI::Unit unit)
{
  if ( unit->getPlayer() == self )  // If we own it
  {
    UnitWrap u(unit);

    // Run computer idle order
    u.AssignComputerIdleOrder();

    // Assign control types
    if ( u->getType().isWorker() )
      u.SetControlType(ControlTypes::Worker);
    else
    {
      u.SetControlType(ControlTypes::Guard);
      u.SetGuardReturnPosition(u->getPosition());
    }
  }
}

void BWScriptEmulator::onUnitDestroy(BWAPI::Unit unit)
{
  if ( unit->getPlayer() == self )  // If we own it
  {

  }
}

void BWScriptEmulator::onUnitMorph(BWAPI::Unit unit)
{}

void BWScriptEmulator::onUnitRenegade(BWAPI::Unit unit)
{}

void BWScriptEmulator::onSaveGame(std::string gameName)
{}
