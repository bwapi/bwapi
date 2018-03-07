#include "DevAIModule.h"

#include <BWAPI/Filters.h>
#include <algorithm>
#include <iostream>

using namespace BWAPI;
using namespace Filter;
using namespace std;

bool enabled;
int mapH, mapW;

unsigned dwCount = 0;

Player self;

////////////////////////////////
void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  //bw->enableFlag(Flag::CompleteMapInformation);

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  // make things go fast
  //bw->setLocalSpeed(0);
  //bw->setFrameSkip(16);

  // set command optimization
  bw->setCommandOptimizationLevel(0);
  //147...610
  bw << "Random Seed: " << bw->getRandomSeed() << std::endl;
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if ( bw->isReplay() ) // ignore everything if in a replay
    return;

  for (Unit u : bw->getAllUnits())
  {
    if (u->getType() == UnitTypes::Spell_Scanner_Sweep) {
      Broodwar << "SCANNER @ " << u->getPosition() << std::endl;
    }
  }
}

void DevAIModule::onSendText(std::string text)
{
  if (text == "/morph")
  {
    BWAPI::Unitset larvae = self->getUnits();
    larvae.erase_if(Filter::GetType != UnitTypes::Zerg_Larva);
    if (!larvae.empty())
    {
      if (!(*larvae.begin())->morph(UnitTypes::Zerg_Mutalisk))
      {
        Broodwar << bw->getLastError() << ":" << self->incompleteUnitCount(UnitTypes::Zerg_Greater_Spire) << ":" << self->incompleteUnitCount(UnitTypes::Zerg_Spire) << std::endl;
      }
    }
  }
  Broodwar->sendText("%s", text.c_str());
}

void DevAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
}

void DevAIModule::onPlayerLeft(BWAPI::Player player)
{
}

void DevAIModule::onNukeDetect(BWAPI::Position target)
{
}

void DevAIModule::onUnitDiscover(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Spell_Scanner_Sweep) {
    Broodwar << "discover @ " << unit->getPosition() << std::endl;
  }
}

void DevAIModule::onUnitEvade(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Spell_Scanner_Sweep) {
    Broodwar << "evade @ " << unit->getPosition() << std::endl;
  }
}

void DevAIModule::onUnitShow(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Spell_Scanner_Sweep) {
    Broodwar << "show @ " << unit->getPosition() << std::endl;
  }
}

void DevAIModule::onUnitHide(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Spell_Scanner_Sweep) {
    Broodwar << "hide @ " << unit->getPosition() << std::endl;
  }
}

void DevAIModule::onUnitCreate(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Spell_Scanner_Sweep) {
    Broodwar << "create @ " << unit->getPosition() << std::endl;
  }
}

void DevAIModule::onUnitDestroy(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Spell_Scanner_Sweep) {
    Broodwar << "destroy @ " << unit->getPosition() << std::endl;
  }
}

void DevAIModule::onUnitMorph(BWAPI::Unit unit)
{
}

void DevAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void DevAIModule::onSaveGame(std::string gameName)
{
}

void DevAIModule::onUnitComplete(BWAPI::Unit unit)
{
}
