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
  bw->enableFlag(Flag::CompleteMapInformation);

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
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if ( bw->isReplay() ) // ignore everything if in a replay
    return;

  for (auto &u : bw->getAllUnits())
  {
    if (!u->getType().isResourceContainer() && !u->getType().isWorker() && !u->getType().isCritter() && !u->getType().isResourceDepot()
      && u->getType() != UnitTypes::Zerg_Overlord && u->getType() != UnitTypes::Zerg_Larva && u->getType() != UnitTypes::Zerg_Egg)
    {
      Broodwar << u->getType() << std::endl;
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
}

void DevAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void DevAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void DevAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void DevAIModule::onUnitCreate(BWAPI::Unit unit)
{
}

void DevAIModule::onUnitDestroy(BWAPI::Unit unit)
{
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
