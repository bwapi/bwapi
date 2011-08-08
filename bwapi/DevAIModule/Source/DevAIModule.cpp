#include "DevAIModule.h"
#include <set>
#include <math.h>

using namespace BWAPI;

int mapH, mapW;

DWORD dwCount = 0;

void DevAIModule::onStart()
{
  // enable stuff
  bw->enableFlag(Flag::UserInput);

  // save player info
  self = bw->self();

  // save map info
  mapH = bw->mapHeight();
  mapW = bw->mapWidth();

  bw->setCommandOptimizationLevel(4);
}

void DevAIModule::onEnd(bool isWinner)
{
}

DWORD dwLastTickCount;
void DevAIModule::onFrame()
{
  if ( bw->isReplay() )
    return;

  if ( bw->getFrameCount() % 4 != 0 )
    return;
  for each ( Unit *u in self->getUnits() )
  {
    if ( u->getType() != UnitTypes::Hero_Tom_Kazansky || !u->exists() )
      continue;

    DWORD dwValue = 0;
    for each ( Unit *e in u->getUnitsInRadius(160) )
    {
      if ( !self->isEnemy(e->getPlayer()) || !e->exists() || !e->isVisible() )
        continue;
      int x = e->getPosition().x() - u->getPosition().x();
      int y = e->getPosition().y() - u->getPosition().y();
      if ( x < -UnitTypes::Terran_Wraith.dimensionLeft() )
        dwValue |= 1;
      if ( x > +UnitTypes::Terran_Wraith.dimensionRight() )
        dwValue |= 2;
      if ( y < -UnitTypes::Terran_Wraith.dimensionUp() )
        dwValue |= 4;
      if ( y > +UnitTypes::Terran_Wraith.dimensionDown() )
        dwValue |= 8;
    }
    int nx = u->getPosition().x();
    int ny = u->getPosition().y();
    if ( nx < 200 )
      dwValue |= 1;
    if ( nx > 96*32 - 200 )
      dwValue |= 2;
    if ( ny < 250 )
      dwValue |= 4;
    if ( ny > 96*32 - 200 )
      dwValue |= 8;

    bool bottom = ny > 64*32;
    bool right  = nx > 64*32;
    switch ( dwValue )
    {
    case 1: // from left
      u->rightClick(Position(nx + 200, ny));
      break;
    case 2: // right
      u->rightClick(Position(nx - 200, ny));
      break;
    case 3: // left, right
      u->rightClick(Position(nx, ny + (bottom ? -200 : 200)));
      break;
    case 4: // top
      u->rightClick(Position(nx, ny + 200));
      break;
    case 5: // top left
      u->rightClick(Position(nx + 140, ny + 140));
      break;
    case 6: // top right
      u->rightClick(Position(nx - 140, ny + 140));
      break;
    case 7: // top left right
      u->rightClick(Position(nx, ny + 200));
      break;
    case 8: // bottom
      u->rightClick(Position(nx, ny - 200));
      break;
    case 9: // bottom left
      u->rightClick(Position(nx + 140, ny - 140));
      break;
    case 10: // bottom right
      u->rightClick(Position(nx - 140, ny - 140));
      break;
    case 11: // bottom left right
      u->rightClick(Position(nx, ny - 200));
      break;
    case 12: // bottom top
      u->rightClick(Position(nx + (right ? -200 : 200), ny));
      break;
    case 13: // bottom top left
      u->rightClick(Position(nx + 200, ny));
      break;
    case 14: // bottom top right
      u->rightClick(Position(nx - 200, ny));
      break;
    }

    bool enemynear = false;
    for each ( Unit *e in u->getUnitsInRadius(160) )
    {
      if ( !self->isEnemy(e->getPlayer()) || !e->exists() || !e->isVisible() )
        continue;
      enemynear = true;
      break;
    }
    if ( !enemynear )
    {
      int shortest = 999999;
      Unit *closest = NULL;
      for each ( Unit *e in bw->getAllUnits() )
      {
        if ( !e->exists() || !self->isEnemy(e->getPlayer()) || e->isInvincible() || !e->isVisible() )
          continue;
        int dist = u->getDistance(e);
        if ( dist < shortest )
        {
          shortest = dist;
          closest  = e;
        }
      }
      if ( closest && u->getAirWeaponCooldown() == 0 )
        u->rightClick(closest);
    }
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

void DevAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
  if ( text == "?trigger" )
    Broodwar->sendText("The bot's current trigger is-... Hey wait a minute!");
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
  if ( unit->getType() == UnitTypes::Hero_Tom_Kazansky && 
       unit->getPlayer() && 
       self->isAlly(unit->getPlayer()) &&
       bw->elapsedTime() > 10 )
    bw->sendText("Welcome back %s!", unit->getPlayer()->getName().c_str() );
}

void DevAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  if ( unit->getType() != UnitTypes::Hero_Tom_Kazansky || !unit->getPlayer() || !self->isAlly(unit->getPlayer()) )
    return;

  bw->sendText("Oh Shi-! We just lost %s!", unit->getPlayer()->getName().c_str());
}

void DevAIModule::onUnitMorph(BWAPI::Unit* unit)
{
}

void DevAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
}

void DevAIModule::onSaveGame(std::string gameName)
{
  bw->sendText("Thanks for saving the game as \"%s\"!", gameName.c_str());
}
