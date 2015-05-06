#include "DevAIModule.h"
#include <set>

using namespace BWAPI;

int mapH, mapW;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();
}

void DevAIModule::onEnd(bool isWinner)
{

}

void DevAIModule::onFrame()
{
	if ( bw->isReplay() )
		return;

  std::set<Unit*> units = bw->getAllUnits();
  for each (Unit * u in units)
  {
    bw->drawTextMap(u->getPosition().x(), u->getPosition().y(), "%d", u->isAttacking());
  }
}

void DevAIModule::onSendText(std::string text)
{
  Broodwar->sendText("%s", text.c_str());
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
