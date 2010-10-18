#include "DevAIModule.h"

using namespace BWAPI;

void DevAIModule::onStart()
{
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->printf("%s", Broodwar->mapHash().c_str());
  scout = NULL;
  Broodwar->setLatCom(false);
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if (Broodwar->isReplay())
    return;

  int thisOrderFrame = Broodwar->getFrameCount();

  for each ( Unit *u in Broodwar->self()->getUnits() )
  {
    if ( u == scout )
      continue;

    if ( u->getLastOrderFrame() + 10 >= thisOrderFrame )
      continue;

    // make workers
    BWAPI::UnitType uType = u->getType();
    if ( uType.isResourceDepot() && u->isIdle() )
    {
      BWAPI::UnitType workerType = uType.getRace().getWorker();
      if ( Broodwar->canMake(NULL, workerType) )
      {
        u->train(workerType);
        continue;
      }
    }

    // make supplies
    if ( Broodwar->self()->supplyTotal(uType.getRace()) <= Broodwar->self()->supplyUsed(uType.getRace()) )
    {
      UnitType supplyType = uType.getRace().getSupplyProvider();
      if ( Broodwar->canMake(NULL, supplyType) && uType == supplyType.whatBuilds().first )
      {
        if ( uType == UnitTypes::Zerg_Larva )
        {
          u->morph(supplyType);
          continue;
        }
        else
        {
          //u->build(NULL, supplyType);
        }
      }
    }

    // harvest resources
    if ( uType.isWorker() )
    {
      if ( (!scout || !scout->exists()) && !u->isCarryingGas() && !u->isCarryingMinerals() )
      {
        scout = u;
        continue;
      }
      else if ( u->isIdle() )
      {
        Unit *best = NULL;
        for each ( Unit *r in Broodwar->getNeutralUnits() )
        {
          if ( r->getType() == UnitTypes::Resource_Mineral_Field &&
               u->getDistance(r) < u->getDistance(best) &&
               !r->isBeingGathered() )
              best = r;
        }
        if ( best )
        {
          u->gather(best);
          continue;
        }
      }
    }

  } // for each


  // scout
  if ( scout && scout->exists() && thisOrderFrame > scout->getLastOrderFrame() + 80 )
  {
    bool done = false;
    for ( int y = 0; y < Broodwar->mapWidth() && !done; ++y )
      for ( int x = 0; x < Broodwar->mapHeight() && !done; ++x )
        if ( !Broodwar->isExplored(x, y) && scout->hasPath(BWAPI::TilePosition(x,y)) )
        {
          scout->move(BWAPI::TilePosition(x,y));
          done = true;
        }
  }
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/ver" )
  {
    Broodwar->printf("Heinermann DevTest");
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
