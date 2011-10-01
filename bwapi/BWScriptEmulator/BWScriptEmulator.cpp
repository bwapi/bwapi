#include "BWScriptEmulator.h"
#include <vector>
#include <set>

#include "ScriptThread.h"
#include "Controller.h"
#include "Starcraft.h"
#include "UnitProc.h"

using namespace BWAPI;

bool enabled;
int mapH, mapW;
Player *self;

bool isResourceClaimed[256];

bool farcasting;

DWORD dwCount = 0;

void BWScriptEmulator::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  bw->enableFlag(Flag::CompleteMapInformation);
  enabled    = true;
  farcasting = true;

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  if ( !bw->isMultiplayer() )
    bw->sendText("black sheep wall");

  dwRandomSeed = GetTickCount();
  if ( !self )
    return;

  if ( !LoadAIBinary("bwapi-data\\AISCRIPT.BIN") )
    Broodwar->printf("%cFailed to load AISCRIPT binary!", 6);

  Race      selfRace(self->getRace());
  Position  sLoc(self->getStartLocation());

  MainController = new AIController();

  if      ( selfRace == Races::Zerg )
    AICreateThread("ZMCx", sLoc );
  else if ( selfRace == Races::Protoss )
    AICreateThread("PMCx", sLoc );
  else // ( selfRace == Races::Terran )
    AICreateThread("TMCx", sLoc );

  srand(dwRandomSeed);
  dwLastRandom = dwRandomSeed;
}

void BWScriptEmulator::onEnd(bool isWinner)
{
  if ( MainController )
    delete MainController;
}

void BWScriptEmulator::onFrame()
{
  if ( bw->isReplay() )
    return;

  if ( !enabled )
    return;

  UpdateScripts();
  for each ( Unit *u in self->getUnits() )
  {
    if ( !u || !u->exists() )
      continue;
    UnitProc *proc = (UnitProc*)u->getClientInfo();
    proc->execute();
  }
/*
  for ( std::vector<spell>::iterator i = spellsCast.begin(); i != spellsCast.end(); ++i )
  {
    if (  !i->pUnit || !i->pUnit->exists() || i->pUnit->isInvincible() ||
          ( i->tech == TechTypes::Defensive_Matrix  && i->pUnit->getDefenseMatrixPoints() > 20 )       ||
          ( i->tech == TechTypes::EMP_Shockwave     && i->pUnit->getShields() + i->pUnit->getEnergy() < 40 )  ||
          ( i->tech == TechTypes::Ensnare           && i->pUnit->getEnsnareTimer() > 5 )               ||
          ( i->tech == TechTypes::Feedback          && i->pUnit->getEnergy() < 30 )                    ||
          ( i->tech == TechTypes::Irradiate         && i->pUnit->getIrradiateTimer() > 5 )             ||
          ( i->tech == TechTypes::Lockdown          && i->pUnit->getLockdownTimer() > 5 )              ||
          ( i->tech == TechTypes::Maelstrom         && i->pUnit->getMaelstromTimer() > 5 )             ||
          ( i->tech == TechTypes::Mind_Control      && i->pUnit->getPlayer() == self )                 ||
          ( i->tech == TechTypes::Optical_Flare     && i->pUnit->isBlind() )                           ||
          ( i->tech == TechTypes::Parasite          && i->pUnit->isParasited() )                       ||
          ( i->tech == TechTypes::Plague            && i->pUnit->getPlagueTimer() > 5 )                ||
          ( i->tech == TechTypes::Psionic_Storm     && i->pUnit->isUnderStorm() )                      ||
          ( i->tech == TechTypes::Restoration       && !i->pUnit->isParasited()
                                                    && !i->pUnit->isBlind()
                                                    && !i->pUnit->isEnsnared()
                                                    && !i->pUnit->isIrradiated()
                                                    && !i->pUnit->isMaelstrommed()
                                                    && !i->pUnit->isPlagued()
                                                    && !i->pUnit->getAcidSporeCount() )         ||
          ( i->tech == TechTypes::Stasis_Field      && !i->pUnit->isStasised() )                ||
          ( i->tech == TechTypes::Disruption_Web    && i->pUnit->isUnderDisruptionWeb() )       ||
          ( i->tech == TechTypes::Dark_Swarm        && i->pUnit->isUnderDarkSwarm() )   )
      i = spellsCast.erase(i);
    if ( i == spellsCast.end() )
      break;
  }
  */
}

void BWScriptEmulator::onSendText(std::string text)
{
  if ( text == "/t" || text == "/toggle" )
  {
    enabled = !enabled;
    Broodwar->printf("AI %s", enabled ? "ENABLED" : "DISABLED");
  }
  if ( text == "/fc" || text == "/farcast" )
  {
    farcasting = !farcasting;
    Broodwar->printf("Farcasting %s", farcasting ? "ENABLED" : "DISABLED");
  }
  else
  {
    Broodwar->sendText("%s", text.c_str());
  }
}

void BWScriptEmulator::onReceiveText(BWAPI::Player* player, std::string text)
{
}

void BWScriptEmulator::onPlayerLeft(BWAPI::Player* player)
{
}

void BWScriptEmulator::onNukeDetect(BWAPI::Position target)
{
}

void BWScriptEmulator::onUnitDiscover(BWAPI::Unit* unit)
{
}

void BWScriptEmulator::onUnitEvade(BWAPI::Unit* unit)
{
}

void BWScriptEmulator::onUnitShow(BWAPI::Unit* unit)
{
}

void BWScriptEmulator::onUnitHide(BWAPI::Unit* unit)
{
}

void BWScriptEmulator::onUnitCreate(BWAPI::Unit* unit)
{
  if ( unit && unit->exists() && unit->getPlayer() == self )
    unit->setClientInfo(getUnitProc(unit));
}

void BWScriptEmulator::onUnitDestroy(BWAPI::Unit* unit)
{
  if ( unit->getPlayer() == self )
  {
    UnitProc *p = (UnitProc*)unit->getClientInfo();
    if ( p )
      delete p;
  }
}

void BWScriptEmulator::onUnitMorph(BWAPI::Unit* unit)
{
}

void BWScriptEmulator::onUnitRenegade(BWAPI::Unit* unit)
{
}

void BWScriptEmulator::onSaveGame(std::string gameName)
{
}
