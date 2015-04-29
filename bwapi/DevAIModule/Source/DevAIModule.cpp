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

  for (Unit res : Broodwar->getStaticMinerals())
  {
    if (Broodwar->isVisible(res->getInitialTilePosition()) && !res->isVisible())
    {
      // mined out or error
      //Broodwar->setLocalSpeed(1000000);
      Broodwar->drawBoxMap(res->getInitialPosition() - Position(8, 8), res->getInitialPosition() + Position(8, 8), Colors::Red);
    }
  }

  if ( bw->self() )
  {
    for (auto u : bw->getSelectedUnits())
    {
      if (bw->getKeyState(K_T))
        u->cancelTrain(-3);
      else if (bw->getKeyState(K_Y))
        u->cancelTrain(-4);
    }
    for (auto u : bw->getAllUnits())
    {
      bw->drawTextMap(u->getPosition(), "%s %d", u->getOrder().c_str(), u->isHoldingPosition());
      /*
      if (u->getType() != UnitTypes::Terran_Goliath) continue;
      Unit targ = u->getOrderTarget();
      if (!targ) continue;

      if (targ->getID() == -1)
      {
        Broodwar << "Whoops! " << targ->getType() << " - " << targ->getPlayer()->getName() << std::endl;
      }
      */
    }
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
