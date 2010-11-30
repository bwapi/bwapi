#include "DevAIModule.h"

using namespace BWAPI;

bool enabled;
int mapH, mapW;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);
  enabled = true;

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  //bw->setLocalSpeed(0);
}

void DevAIModule::onEnd(bool isWinner)
{
  //bw->restartGame();
}

DWORD dwLastTickCount;
void DevAIModule::onFrame()
{
  bw->drawTextScreen(20, 20, "%.2f | %u\n%u / %u", Broodwar->getAverageFPS(), Broodwar->getFPS(), Broodwar->getFrameCount(), Broodwar->getReplayFrameCount());

  if ( bw->isReplay() )
    return;

  if ( !enabled )
    return;

  for each (Unit *u in bw->getSelectedUnits())
  {
    if ( u->siege() )
    {
      bw->printf("SIEGE!");
    }
    bw->drawTextMap(u->getPosition().x(), u->getPosition().y(), "%s", bw->getLastError().toString().c_str());
      /*
    for each (Unit *t in self->getUnits())
    {
      if ( u->repair(t) )
      {
        bw->printf("Repairing! %s -> %s", u->getType().getName().c_str(), t->getType().getName().c_str());
      }
    }*/

  }
}

bool pointSearch(int dwType, BWAPI::TilePosition pt, BWAPI::Unit *unit, BWAPI::UnitType type, int width, int height)
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
  case SEARCH_CONSTRUCT:
    if ( type != BWAPI::UnitTypes::None && type != BWAPI::UnitTypes::Unknown )
      return Broodwar->canBuildHere(unit, pt, type) && unit ? unit->hasPath(Position(pt)) : true;
    break;
  }
  return false;
}

BWAPI::TilePosition spiralSearch(int dwType, BWAPI::TilePosition start, int radius, BWAPI::Unit *unit, BWAPI::UnitType type, int width, int height)
{
  int x = 0, y = 0, d = 1;
  for ( int i = 1; i <= radius*2; ++i, d = -d )
  {
    for ( int iy = 0; iy < i; ++iy )
    {
      if ( pointSearch(dwType, BWAPI::TilePosition(start.x()+x, start.y()+y), unit, type, width, height) )
        return BWAPI::TilePosition(start.x()+x, start.y()+y);
      if ( y + d + start.y() < 0 || y + d + start.y() >= mapH )
        break;
      y += d;
    }

    for ( int ix = 0; ix < i; ++ix )
    {
      if ( pointSearch(dwType, BWAPI::TilePosition(start.x()+x, start.y()+y), unit, type, width, height) )
        return BWAPI::TilePosition(start.x()+x, start.y()+y);
      if ( x + d + start.x() < 0 || x + d + start.x() >= mapW )
        break;
      x += d;
    }
  }
  return BWAPI::TilePositions::None;
}

void DevAIModule::onSendText(std::string text)
{
  if ( text == "/t" )
  {
    enabled = !enabled;
    Broodwar->printf("DevAITest %s", enabled ? "ENABLED" : "DISABLED");
  }
  else if ( text == "/wikiTypes" )
  {
    writeUnitWiki();
    Broodwar->printf("Printed wiki unit type information");
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
