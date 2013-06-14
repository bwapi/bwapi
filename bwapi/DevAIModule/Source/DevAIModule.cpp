#include "DevAIModule.h"

#include <BWAPI/Filters.h>
#include <algorithm>
#include <iostream>

using namespace BWAPI;
using namespace Filter;
using namespace std;

bool enabled;
int mapH, mapW;

DWORD dwCount = 0;

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

  Broodwar << "Minerals: " << Broodwar->getMinerals().size() << endl;
  bw << "Static minerals: " << bw->getStaticMinerals().size() << endl;
  Unitset mins = bw->getStaticMinerals();
  for( auto it = mins.begin(); it != mins.end(); ++it )
  {
    Unit u = *it;
    bw->registerEvent([u](Game*){ bw->drawTextMap(u->getInitialPosition(), "%d", u->getResources()); });
  }
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if ( bw->isReplay() ) // ignore everything if in a replay
    return;

  if ( bw->getFrameCount() < 3 )
    Broodwar << Broodwar->getMinerals().size() << endl;

  // Log and display the best logical FPS seen in the game
  static int bestFPS = 0;
  bestFPS = std::max(bestFPS, Broodwar->getFPS());
  Broodwar->drawTextScreen(Positions::Origin, "%cBest: %d FPS\nCurrent: %d FPS", Text::White, bestFPS, Broodwar->getFPS() );
  
  // Limit logical frames processed to prevent stacking commands
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;

  if ( BWAPI::Broodwar->self() )
  {
    BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    {
      if ( u->getType().isRefinery() )
      {
        int nWorkersAssigned = u->getClientInfo<int>('work');
        if ( nWorkersAssigned < 3 )
        {
          Unit pClosestIdleWorker = u->getClosestUnit(BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle);
          if ( pClosestIdleWorker )
          {
            // gather from the refinery (and check if it was successful)
            if ( pClosestIdleWorker->gather(*u) )
            {
              // set a back reference for when the unit is killed or re-assigned (code not provided)
              pClosestIdleWorker->setClientInfo(*u, 'ref');
  
              // Increment the number of workers assigned and associate it with the refinery
              ++nWorkersAssigned;
              u->setClientInfo(nWorkersAssigned, 'work');
            }
          }
        }
      }
    } // for
  }
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/wiki" )
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
