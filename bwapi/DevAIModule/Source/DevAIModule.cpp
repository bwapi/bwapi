#include "DevAIModule.h"

using namespace BWAPI;

void DevAIModule::onStart()
{
  bw->enableFlag(Flag::UserInput);
  scout = NULL;
  bw->setLatCom(false);
Broodwar->setLocalSpeed(0);
  self = bw->self();
}

void DevAIModule::onEnd(bool isWinner)
{
}

void DevAIModule::onFrame()
{
  if ( bw->isReplay() )
    return;

  for each ( Unit *u in Broodwar->getAllUnits() )
  {
    if ( u->isSelected() )
    {
      TilePosition tp = u->getTilePosition();
      Position p = (Position)tp;
      Broodwar->drawTextMap(p.x(), p.y(), "(%u,%u)", tp.x(), tp.y());
    }
  }

  int thisOrderFrame = bw->getFrameCount();

  for each ( Unit *u in self->getUnits() )
  {
    if ( u == scout )
      continue;

    if ( u->getLastOrderFrame() + 20 >= thisOrderFrame )
      continue;

    BWAPI::UnitType uType = u->getType();
    BWAPI::Race     uRace = uType.getRace();

    // make workers
    if ( uType.isResourceDepot() && u->isIdle() )
    {
      BWAPI::UnitType workerType = uRace.getWorker();
      if ( bw->canMake(u, workerType) )
      {
        u->train(workerType);
        continue;
      }
    }

    // make supplies
    if ( self->supplyTotal(uRace) + self->incompleteUnitCount(uRace.getSupplyProvider()) <= self->supplyUsed(uRace) )
    {
      if ( bw->canMake(NULL, uRace.getSupplyProvider()) )
      {
        if ( uType.isResourceDepot() && uRace == Races::Zerg )
        {
          u->morph(uRace.getSupplyProvider());
          continue;
        }
        else if ( uType == uRace.getSupplyProvider().whatBuilds().first && !u->isCarryingGas() && !u->isCarryingMinerals() )
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
      }
      else if ( u->isIdle() )
      {
        Unit *best = NULL;
        for each ( Unit *r in Broodwar->getNeutralUnits() )
        {
          if ( r->getType() == UnitTypes::Resource_Mineral_Field &&
               u->getDistance(r) < u->getDistance(best) &&
               u->hasPath(r->getPosition()) &&
               !r->isBeingGathered() )
              best = r;
        }

        if ( best )
          u->gather(best);
      }

      continue;
    }

    if ( uType == BWAPI::UnitTypes::Zerg_Overlord && thisOrderFrame > u->getLastOrderFrame() + 120 )
    {
      BWAPI::TilePosition targ = spiralSearch(SEARCH_NOTVISIBLE, (TilePosition)u->getPosition(), 255);
      if ( targ != BWAPI::TilePositions::None )
        u->move((Position)targ);

      continue;
    }

  } // for each

  // scout
  if ( scout && scout->exists() && thisOrderFrame > scout->getLastOrderFrame() + 40 )
  {
    BWAPI::TilePosition targ = spiralSearch(SEARCH_UNEXPLORED, (TilePosition)scout->getPosition(), 255, scout);
    if ( targ != BWAPI::TilePositions::None )
      scout->move((Position)targ);
  }

}

bool DevAIModule::pointSearch(int dwType, BWAPI::TilePosition pt, BWAPI::Unit *unit, int width, int height)
{
  switch ( dwType )
  {
  case SEARCH_UNEXPLORED:
    if ( unit )
      return !Broodwar->isExplored(pt) && unit->hasPath((Position)pt);
    else
      return !Broodwar->isExplored(pt);
  case SEARCH_EXPLORED:
    if ( unit )
      return Broodwar->isExplored(pt) && !Broodwar->isVisible(pt) && unit->hasPath((Position)pt);
    else
      return Broodwar->isExplored(pt) && !Broodwar->isVisible(pt);
  case SEARCH_NOTVISIBLE:
    if ( unit )
      return (!Broodwar->isExplored(pt) || !Broodwar->isVisible(pt)) && unit->hasPath((Position)pt);
    else
      return !Broodwar->isExplored(pt) || !Broodwar->isVisible(pt);
  }
  return false;
}

BWAPI::TilePosition DevAIModule::spiralSearch(int dwType, BWAPI::TilePosition start, int radius, BWAPI::Unit *unit, int width, int height)
{
  int x = 0, y = 0, iter = 1, d = 1;
  int mapH = bw->mapHeight();
  int mapW = bw->mapWidth();

  for ( int i = 0; iter <= radius*2; ++i )
  {
    for ( int iy = 0; iy < iter; ++iy )
    {
      if ( pointSearch(dwType, BWAPI::TilePosition(start.x()+x, start.y()+y), unit, width, height) )
        return BWAPI::TilePosition(start.x()+x, start.y()+y);
      if ( y + d + start.y() < 0 || y + d + start.y() >= mapH )
        break;
      y += d;
    }

    for ( int ix = 0; ix < iter; ++ix )
    {
      if ( pointSearch(dwType, BWAPI::TilePosition(start.x()+x, start.y()+y), unit, width, height) )
        return BWAPI::TilePosition(start.x()+x, start.y()+y);
      if ( x + d + start.x() < 0 || x + d + start.x() >= mapW )
        break;
      x += d;
    }
    d = -d;
    ++iter;
  }
  return BWAPI::TilePositions::None;
}

void DevAIModule::onSendText(std::string text)
{
  if (text=="tpos")
  {
    for each ( Unit *u in Broodwar->getAllUnits() )
    {
      if ( u->isSelected() )
      {
        Broodwar->printf("Tile position is %d/%d!", u->getTilePosition().x(), u->getTilePosition().y());
      }
    }
  }

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
