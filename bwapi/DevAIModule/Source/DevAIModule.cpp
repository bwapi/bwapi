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

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  // make things go fast
  //bw->setLocalSpeed(0);
  //bw->setFrameSkip(16);

  // set command optimization
  bw->setCommandOptimizationLevel(3);
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if ( bw->isReplay() ) // ignore everything if in a replay
    return;

  if ( bw->self() )
  {
    Unitset myUnits = bw->getSelectedUnits();
    for ( auto u : myUnits )
    {
      bool result = bw->canBuildHere(u->getTilePosition(), UnitTypes::Terran_Machine_Shop, u);
      bw->drawTextMap(u->getPosition(), "%s %s", result ? "true" : "false", bw->getLastError().c_str());
    } // for
  }
}

void DevAIModule::onSendText(std::string text)
{
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

void newOnUnitComplete(BWAPI::Unit unit);

void DevAIModule::onUnitCreate(BWAPI::Unit unit)
{
  //unit->registerEvent(newOnUnitComplete, IsCompleted, 1);
  //Broodwar << __FUNCTION__ " -- " << unit->getPlayer()->getName() << ": " << unit->getType() << std::endl;
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
  //Broodwar << __FUNCTION__ << " -- " << unit->getType() << std::endl;
}

void newOnUnitComplete(BWAPI::Unit unit)
{
  //Broodwar << __FUNCTION__ " -- " << unit->getType() << std::endl;
}
