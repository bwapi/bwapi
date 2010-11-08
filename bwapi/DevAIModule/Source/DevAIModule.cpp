#include <windows.h>
#include "DevAIModule.h"

using namespace BWAPI;

bool enabled;

void DevAIModule::onStart()
{
  bw->enableFlag(Flag::UserInput);
  scout = NULL;
  bw->setLatCom(false);

  enabled = true;
  //Broodwar->setLocalSpeed(0);
  //Broodwar->sendText("modify the phase variance");
  self = bw->self();
}

void DevAIModule::onEnd(bool isWinner)
{
}

DWORD dwLastTickCount;
void DevAIModule::onFrame()
{
  bw->drawTextScreen(20, 20, "%.2f | %u\n%u / %u", Broodwar->getAverageFPS(), Broodwar->getFPS(), Broodwar->getFrameCount(), Broodwar->getReplayFrameCount());

  if ( bw->isReplay() )
    return;

  if ( enabled )
  {
    int thisOrderFrame = bw->getFrameCount();
    for each ( Unit *u in self->getUnits() )
    {
      if ( u->cancelTrain() )
        bw->printf("OK");
    }
  }
    /*
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
*/
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
  if ( text == "/t" )
  {
    enabled = !enabled;
    Broodwar->printf("DevAITest %s", enabled ? "ENABLED" : "DISABLED");
  }
  else if ( text == "/wikiTypes" )
  {
    FILE *outWiki = fopen("wiki.txt", "w");
    if ( !outWiki )
      return;

    for each ( UnitType u in UnitTypes::allUnitTypes() )
    {
      fprintf(outWiki, "=== %s ===\n", u.getName().c_str());
      if ( !u.isInvincible() )
      {
        fprintf(outWiki, "  * *Max HP*: %d\n", u.maxHitPoints());
        if ( u.maxShields() )
          fprintf(outWiki, "  * *Max Shields*: %d\n", u.maxShields());
        if ( u.maxEnergy() )
          fprintf(outWiki, "  * *Max Energy*: %u\n", u.maxEnergy());
        fprintf(outWiki, "  * *Armor*: %u\n", u.armor());
        fprintf(outWiki, "  * *Unit Size*: %s\n", u.size().getName().c_str());
      }
      if ( u.whatBuilds().second > 0 )
      {
        if ( u.whatBuilds().second == 1 )
        {
          fprintf(outWiki, "  * *Mineral Cost*: %u\n", u.mineralPrice());
          fprintf(outWiki, "  * *Gas Cost*: %u\n", u.gasPrice());
        }
        fprintf(outWiki, "  * *Build Time*: %u\n", u.buildTime());
      }
      if ( u.supplyProvided() )
        fprintf(outWiki, "  * *Supply Provided*: %u\n", u.supplyProvided());
      if ( u.supplyRequired() )
        fprintf(outWiki, "  * *Supply Required*: %u\n", u.supplyRequired());
      if ( u.spaceProvided() )
        fprintf(outWiki, "  * *Space Provided*: %u\n", u.spaceProvided());
      if ( u.spaceRequired() != 255 )
        fprintf(outWiki, "  * *Space Required*: %u\n", u.spaceRequired());
      if ( u.isBuilding() )
        fprintf(outWiki, "  * *Tile Size*: %u x %u\n", u.tileWidth(), u.tileHeight());
      fprintf(outWiki, "  * *Unit Dimensions*: %u x %u\n", u.dimensionLeft() + u.dimensionRight() + 1, u.dimensionUp() + u.dimensionDown() + 1);
      fprintf(outWiki, "  * *Sight Range*: %u (%u)\n", u.sightRange(), u.sightRange()/32);
      if ( u.buildScore() && u.whatBuilds().second > 0 )
        fprintf(outWiki, "  * *Build Score*: %u\n", u.buildScore());
      if ( u.destroyScore() && !u.isInvincible() )
        fprintf(outWiki, "  * *Destroy Score*: %u\n", u.destroyScore());
      if ( u.topSpeed() > 0 )
        fprintf(outWiki, "  * *Top Speed*: %Lf\n", u.topSpeed());
      if ( u.groundWeapon() == u.airWeapon() )
      {
        if ( u.groundWeapon() != WeaponTypes::None )
          fprintf(outWiki, "  * *Weapon*: %s\n", u.groundWeapon().getName().c_str());
      }
      else
      {
        if ( u.groundWeapon() != WeaponTypes::None )
          fprintf(outWiki, "  * *Ground Weapon*: %s\n", u.groundWeapon().getName().c_str());
        if ( u.airWeapon() != WeaponTypes::None )
          fprintf(outWiki, "  * *Air Weapon*: %s\n", u.airWeapon().getName().c_str());
      }
      if ( u.whatBuilds().first != UnitTypes::None )
      {
        if ( u.whatBuilds().second == 1 )
          fprintf(outWiki, "  * *Comes from*: %s\n", u.whatBuilds().first.getName().c_str());
        else if ( u.whatBuilds().second > 1 )
          fprintf(outWiki, "  * *Comes from*: %u %s\n", u.whatBuilds().second, u.whatBuilds().first.getName().c_str());
      }
      if ( !u.abilities().empty() ||
            u.hasPermanentCloak() ||
            u.isDetector()        ||
            u.isFlyingBuilding()  ||
            u.isInvincible()      ||
            u.regeneratesHP())
      {
        fprintf(outWiki, "\n  ==== Abilities ====\n");
        for each ( TechType t in u.abilities() )
          fprintf(outWiki, "    * %s\n", t.getName().c_str());
        if ( u.hasPermanentCloak() )
          fprintf(outWiki, "    * Permanently Cloaked\n");
        if ( u.isDetector() )
          fprintf(outWiki, "    * Detector\n");
        if ( u.isFlyingBuilding() )
          fprintf(outWiki, "    * Liftoff\n");
        if ( u.isInvincible() )
          fprintf(outWiki, "    * Invincible\n");
        if ( u.regeneratesHP() )
          fprintf(outWiki, "    * Regenerates Hit Points\n");
      }
      if ( (!u.requiredUnits().empty()  ||
            u.requiresCreep()           ||
            u.requiresPsi()             ||
            u.requiredTech() != TechTypes::None) &&
           !u.isSpecialBuilding() )
      {
        fprintf(outWiki, "\n  ==== Requirements ====\n");
        for each ( std::pair<UnitType,int> p in u.requiredUnits() )
        {
          if ( p.second == 1 )
            fprintf(outWiki, "    * %s\n", p.first.getName().c_str());
          else if ( p.second > 1 )
            fprintf(outWiki, "    * %u %s\n", p.second, p.first.getName().c_str());
        }
        if ( u.requiresCreep() )
          fprintf(outWiki, "    * Requires Creep\n");
        if ( u.requiresPsi() )
          fprintf(outWiki, "    * Requires Psi Field\n");
        if ( u.requiredTech() != TechTypes::None )
          fprintf(outWiki, "    * %s\n", u.requiredTech().getName().c_str());
      }
      if ( !u.upgrades().empty() )
      {
        fprintf(outWiki, "\n  ==== Upgrades ====\n");
        for each ( UpgradeType upg in u.upgrades() )
          fprintf(outWiki, "    * %s\n", upg.getName().c_str() );
      }

      fprintf(outWiki, "\n\n");
    }
    fclose(outWiki);
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
