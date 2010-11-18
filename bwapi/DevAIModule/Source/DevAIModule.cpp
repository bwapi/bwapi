#include <windows.h>
#include <algorithm>
#include "DevAIModule.h"

#define makelink(x) wiki(x.getName()).c_str(), x.getName().c_str()
#define maketitle(x) wiki(x.getName(), true).c_str()
using namespace BWAPI;

bool enabled;

void DevAIModule::onStart()
{
  bw->enableFlag(Flag::UserInput);
  scout = NULL;

  enabled = true;
  //Broodwar->setLocalSpeed(0);
  //Broodwar->sendText("modify the phase variance");
  self = bw->self();
}

void DevAIModule::onEnd(bool isWinner)
{
  MessageBox(NULL, isWinner ? "I won" : "I lost", "!", MB_OK);

  bw->restartGame();
}

DWORD dwLastTickCount;
void DevAIModule::onFrame()
{
  bw->drawTextScreen(20, 20, "%.2f | %u\n%u / %u", Broodwar->getAverageFPS(), Broodwar->getFPS(), Broodwar->getFrameCount(), Broodwar->getReplayFrameCount());

  if ( bw->isReplay() )
    return;

  /*
  if ( enabled )
  {
    int thisOrderFrame = bw->getFrameCount();
    for each ( Unit *u in self->getUnits() )
    {
      if ( u->cancelTrain() )
        bw->printf("OK");
    }
  }
  
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

std::string wiki(std::string name, bool makeTitle = false)
{
  char temp[64];
  strcpy(temp, name.c_str());

  bool inword = false;
  size_t tlen = strlen(temp);
  for ( unsigned int i = 0; i < tlen; ++i )
  {
    if ( temp[i] == ' ' )
    {
      inword = false;
      if ( !makeTitle )
        temp[i] = '_';
    }
    else
    {
      if ( inword )
        temp[i] = tolower(temp[i]);
      inword = true;
    }

  }
  return std::string(temp);
}

void printUnitData(FILE *outWiki, UnitType u)
{
  fprintf(outWiki, "=== %s ===\n", maketitle(u));
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
    {
      fprintf(outWiki, "  * *Weapon*: [WeaponTypes#%s %s]\n", makelink(u.groundWeapon()) );
    }
  }
  else
  {
    if ( u.groundWeapon() != WeaponTypes::None )
      fprintf(outWiki, "  * *Ground Weapon*: [WeaponTypes#%s %s]\n", makelink(u.groundWeapon()) );
    if ( u.airWeapon() != WeaponTypes::None )
      fprintf(outWiki, "  * *Air Weapon*: [WeaponTypes#%s %s]\n", makelink(u.airWeapon()) );
  }
  if ( u.whatBuilds().first != UnitTypes::None )
  {
    if ( u.whatBuilds().second == 1 )
      fprintf(outWiki, "  * *Comes from*: [UnitTypes#%s %s]\n", makelink(u.whatBuilds().first) );
    else if ( u.whatBuilds().second > 1 )
      fprintf(outWiki, "  * *Comes from*: %u [UnitTypes#%s %s]\n", u.whatBuilds().second, makelink(u.whatBuilds().first) );
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
      fprintf(outWiki, "    * [TechTypes#%s %s]\n", makelink(t));
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
        fprintf(outWiki, "    * [UnitTypes#%s %s]\n", makelink(p.first) );
      else if ( p.second > 1 )
        fprintf(outWiki, "    * %u [UnitTypes#%s %s]\n", p.second, makelink(p.first) );
    }
    if ( u.requiresCreep() )
      fprintf(outWiki, "    * Requires Creep\n");
    if ( u.requiresPsi() )
      fprintf(outWiki, "    * Requires Psi Field\n");
    if ( u.requiredTech() != TechTypes::None )
      fprintf(outWiki, "    * [TechTypes#%s %s]\n", makelink(u.requiredTech()) );
  }
  if ( !u.upgrades().empty() )
  {
    fprintf(outWiki, "\n  ==== Upgrades ====\n");
    for each ( UpgradeType upg in u.upgrades() )
      fprintf(outWiki, "    * [UpgradeTypes#%s %s]\n", makelink(upg) );
  }
  fprintf(outWiki, "\n\n");
}

bool nameSort(const UnitType &u1, const UnitType &u2)
{
  return strcmp(maketitle(u1), maketitle(u2)) < 0;
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
    FILE *outWiki = fopen("UnitTypes.wiki", "w");
    if ( !outWiki )
      return;

    fprintf(outWiki, "#summary Unit Types in BWAPI\n");
    fprintf(outWiki, "#sidebar TableOfContents\n\n");

    fprintf(outWiki, "This page contains a list of UnitTypes provided by BWAPI.\n\n");

    fprintf(outWiki, "<wiki:toc max_depth=\"3\" />\n\n");

    fprintf(outWiki, "\n= Terran =\n");
    fprintf(outWiki, "\n== Terran Ground Units ==\n");

    std::vector<UnitType> unitTypes;
    for each ( UnitType u in UnitTypes::allUnitTypes() )
      unitTypes.push_back(u);

    std::sort(unitTypes.begin(), unitTypes.end(), nameSort);

    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Terran && !u.isHero() && !u.isBuilding() && !u.isSpell() && !u.isFlyer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Terran Air Units ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Terran && !u.isHero() && !u.isBuilding() && !u.isSpell() && u.isFlyer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Terran Heroes ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Terran && u.isHero() && !u.isBuilding() && !u.isSpell() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Terran Buildings ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Terran && u.isBuilding() && !u.isSpecialBuilding() && !u.isAddon() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Terran Addons ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Terran && u.isBuilding() && !u.isSpecialBuilding() && u.isAddon() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Terran Special Buildings ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Terran && u.isSpecialBuilding() && !u.isBeacon() && !u.isFlagBeacon() )
        printUnitData(outWiki, u);


    fprintf(outWiki, "\n= Protoss =\n");
    fprintf(outWiki, "\n== Protoss Ground Units ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Protoss && !u.isHero() && !u.isBuilding() && !u.isSpell() && !u.isFlyer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Protoss Air Units ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Protoss && !u.isHero() && !u.isBuilding() && !u.isSpell() && u.isFlyer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Protoss Heroes ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Protoss && u.isHero() && !u.isBuilding() && !u.isSpell() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Protoss Buildings ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Protoss && u.isBuilding() && !u.isSpecialBuilding() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Protoss Special Buildings ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Protoss && u.isSpecialBuilding() && !u.isBeacon() && !u.isFlagBeacon() )
        printUnitData(outWiki, u);


    fprintf(outWiki, "\n= Zerg =\n");
    fprintf(outWiki, "\n== Zerg Ground Units ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Zerg && !u.isHero() && !u.isBuilding() && !u.isSpell() && !u.isFlyer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Zerg Air Units ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Zerg && !u.isHero() && !u.isBuilding() && !u.isSpell() && u.isFlyer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Zerg Heroes ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Zerg && u.isHero() && !u.isBuilding() && !u.isSpell() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Zerg Buildings ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Zerg && u.isBuilding() && !u.isSpecialBuilding() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Zerg Special Buildings ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Zerg && u.isSpecialBuilding() && !u.isBeacon() && !u.isFlagBeacon() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n= Other =\n");
    fprintf(outWiki, "\n== Critters ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.getRace() == Races::Other && !u.isBuilding() && !u.isPowerup() && !u.isResourceContainer() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Resources ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.isResourceContainer() && !u.isRefinery() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Spells ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.isSpell() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Beacons ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.isBeacon() || u.isFlagBeacon() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Powerups ==\n");
    for each ( UnitType u in unitTypes )
      if ( u.isPowerup() )
        printUnitData(outWiki, u);

    fprintf(outWiki, "\n== Misc ==\n");
    printUnitData(outWiki, UnitTypes::None);
    printUnitData(outWiki, UnitTypes::Unknown);

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
