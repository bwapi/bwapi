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
  bw->setCommandOptimizationLevel(0);
}

void DevAIModule::onEnd(bool isWinner)
{
}

char const *bools(bool v)
{
  return v ? "true" : "false";
}

void DevAIModule::onFrame()
{
  if ( bw->isReplay() ) // ignore everything if in a replay
    return;

  bw->drawTextScreen(Positions::Origin, "APM Selects: %d\nAPM No Selects: %d", bw->getAPM(true), bw->getAPM(false));

  if ( bw->self() )
  {
    for (auto u : bw->getAllUnits())
    {
      bw->drawTextMap(u->getPosition(), "%c%d vs %d", Text::Teal, u->getID(), u->getReplayID());
    }
    //bw->self()->getUnits().morph(UnitTypes::Zerg_Overlord);
    for (auto u : bw->self()->getUnits())
    {
      if (u->getType() == UnitTypes::Zerg_Overlord && u->isIdle()) u->move(Positions::Origin);
      else if (u->getType() == UnitTypes::Zerg_Larva && self->completedUnitCount(UnitTypes::Zerg_Larva) > 2) u->morph(UnitTypes::Zerg_Overlord);
      else if (u->getType() == UnitTypes::Zerg_Larva && self->allUnitCount(UnitTypes::Zerg_Hatchery) < 8) u->morph(UnitTypes::Zerg_Drone);
    }
  }
}

void DevAIModule::onSendText(std::string text)
{
  if (text == "/addon")
  {
    Broodwar << (bw->getSelectedUnits().buildAddon(UnitTypes::Terran_Comsat_Station) ? "success" : "fail") << std::endl;
  }
  else if (text == "/test")
  {
    Broodwar->sendText("show me the money");
    Broodwar->sendText("show me the money");
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

void newOnUnitComplete(BWAPI::Unit unit);

void DevAIModule::onUnitCreate(BWAPI::Unit unit)
{
  //unit->registerEvent(newOnUnitComplete, IsCompleted, 1);
  //Broodwar << __FUNCTION__ " -- " << unit->getPlayer()->getName() << ": " << unit->getType() << std::endl;
}

void DevAIModule::onUnitDestroy(BWAPI::Unit unit)
{
  Broodwar << unit->getType() << " died; exists = " << std::boolalpha << unit->exists() << std::endl;
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
