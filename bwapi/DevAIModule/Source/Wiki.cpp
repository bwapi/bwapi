#include "Wiki.h"

using namespace BWAPI;

std::string wiki(std::string name, bool makeTitle = false)
{
  char temp[64];
  strcpy(temp, name.c_str());

  bool inword      = false;
  bool inAcronym   = false;
  bool testAcronym = true;
  size_t tlen = strlen(temp);
  for ( unsigned int i = 0; i < tlen; ++i )
  {
    if ( !inword )
    {
      int i2 = 0;
      while ( isalpha(temp[i+i2]) )
      {
        if ( islower(temp[i+i2]) )
        {
          inAcronym = false;
          break;
        }
        inAcronym = true;
        ++i2;
      }
    }
    
    if ( temp[i] == ' ' )
    {
      inword = false;
      if ( !makeTitle )
        temp[i] = '_';
    }
    else if ( !isalpha(temp[i]) )
    {
      inword = false;
    }
    else
    {
      if ( inword && !inAcronym )
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

bool unitNameSort(const UnitType &u1, const UnitType &u2)
{
  return strcmpi(maketitle(u1), maketitle(u2)) < 0;
}

void writeUnitWiki()
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

  std::sort(unitTypes.begin(), unitTypes.end(), unitNameSort);

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
    if ( u.getRace() == Races::Other && 
         !u.isBuilding() && 
         !u.isPowerup() && 
         !u.isResourceContainer() &&
         u != UnitTypes::Special_Cargo_Ship &&
         u != UnitTypes::Special_Mercenary_Gunship)
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

  fprintf(outWiki, "\n== Special ==\n");
  printUnitData(outWiki, UnitTypes::Special_Cargo_Ship);
  printUnitData(outWiki, UnitTypes::Special_Mercenary_Gunship);
  printUnitData(outWiki, UnitTypes::Special_Independant_Starport);

  fprintf(outWiki, "\n== Misc ==\n");
  fprintf(outWiki, "=== None ===\n");
  fprintf(outWiki, "A type used to specify that no unit type exists.\n\n");
  fprintf(outWiki, "=== All Units ===\n");
  fprintf(outWiki, "A macro type used to specify that we want to obtain data for all the units and buildings.\n\n");
  fprintf(outWiki, "=== Men ===\n");
  fprintf(outWiki, "A macro type used to specify that we want to obtain data for all the units that aren't buildings.\n\n");
  fprintf(outWiki, "=== Buildings ===\n");
  fprintf(outWiki, "A macro type used to specify that we want to obtain data for all the buildings.\n\n");
  fprintf(outWiki, "=== Factories ===\n");
  fprintf(outWiki, "A macro type used to specify that we want to obtain data for only the buildings that can produce units.\n\n");
  fprintf(outWiki, "=== Unknown ===\n");
  fprintf(outWiki, "A type used to specify that we shouldn't know what the unit is, and that it has been made unknown to us. It is also used for unit types that have not been implemented.\n\n");
  fclose(outWiki);
}

void printWeaponData(FILE *outWiki, WeaponType w)
{
  fprintf(outWiki, "== %s ==\n", maketitle(w));
  
  fprintf(outWiki, "  * *Explosion Type*: %s\n", w.explosionType().getName().c_str());

  if ( w.explosionType() == ExplosionTypes::Air_Splash      ||
       w.explosionType() == ExplosionTypes::Corrosive_Acid  ||
       w.explosionType() == ExplosionTypes::Enemy_Splash    ||
       w.explosionType() == ExplosionTypes::Normal          ||
       w.explosionType() == ExplosionTypes::Nuclear_Missile ||
       w.explosionType() == ExplosionTypes::Radial_Splash   ||
       w.explosionType() == ExplosionTypes::Yamato_Gun)
  {
    if ( w.damageAmount() || w.damageBonus() )
    {
      fprintf(outWiki, "  * *Damage Type*: %s\n", w.damageType().getName().c_str());
      fprintf(outWiki, "  * *Damage*: %u\n", w.damageAmount());
      if ( w.damageBonus() )
        fprintf(outWiki, "  * *Bonus*: %u\n", w.damageBonus());
      if ( w.explosionType() != ExplosionTypes::Yamato_Gun )
        fprintf(outWiki, "  * *Factor*: %u\n", w.damageFactor());
    }
    fprintf(outWiki, "  * *Cooldown*: %u\n", w.damageCooldown());
  }
  if ( w.explosionType() == ExplosionTypes::Enemy_Splash ||
       w.explosionType() == ExplosionTypes::Air_Splash ||
       w.explosionType() == ExplosionTypes::Radial_Splash )
    fprintf(outWiki, "  * *Splash Radius*: %u - %u - %u\n", w.innerSplashRadius(), w.medianSplashRadius(), w.outerSplashRadius());
  fprintf(outWiki, "  * *Range*: %u - %u\n", w.minRange(), w.maxRange());

  if ( w.getTech() != TechTypes::None )
    fprintf(outWiki, "  * *Technology*: [TechTypes#%s %s]\n", makelink(w.getTech()));
  if ( w.upgradeType() != UpgradeTypes::None )
    fprintf(outWiki, "  * *Upgrade*: [UpgradeTypes#%s %s]\n", makelink(w.upgradeType()));

  if ( w.targetsAir()         ||
       w.targetsGround()      ||
       w.targetsMechanical()  ||
       w.targetsNonBuilding() ||
       w.targetsNonRobotic()  ||
       w.targetsOrganic()     ||
       w.targetsOrgOrMech()   ||
       w.targetsOwn()         ||
       w.targetsTerrain() )
  {
    fprintf(outWiki, "\n  === Targets ===\n");
    if ( w.targetsAir() )
      fprintf(outWiki, "    * Air\n");
    if ( w.targetsGround() )
      fprintf(outWiki, "    * Ground\n");
    if ( w.targetsMechanical() )
      fprintf(outWiki, "    * Mechanical\n");
    if ( w.targetsNonBuilding() )
      fprintf(outWiki, "    * Non-Buildings\n");
    if ( w.targetsNonRobotic() )
      fprintf(outWiki, "    * Non-Robotic\n");
    if ( w.targetsOrganic() )
      fprintf(outWiki, "    * Organic\n");
    if ( w.targetsOrgOrMech() )
      fprintf(outWiki, "    * Organic or Mechanical\n");
    if ( w.targetsOwn() )
      fprintf(outWiki, "    * Own\n");
    if ( w.targetsTerrain() )
      fprintf(outWiki, "    * Terrain\n");
  }
  if ( w.whatUses() != UnitTypes::None )
  {
    fprintf(outWiki, "\n  === What Uses ===\n");
    for each ( UnitType u in UnitTypes::allUnitTypes() )
    {
      if ( u.groundWeapon() == w || u.airWeapon() == w )
        fprintf(outWiki, "    * [UnitTypes#%s %s]\n", makelink(u) );
      for each ( TechType t in u.abilities() )
        if ( t.getWeapon() == w )
          fprintf(outWiki, "    * [UnitTypes#%s %s]\n", makelink(u) );
    }
  }
  fprintf(outWiki, "\n\n");
}

bool weaponNameSort(const WeaponType &u1, const WeaponType &u2)
{
  return strcmpi(maketitle(u1), maketitle(u2)) < 0;
}

void writeWeaponWiki()
{
  FILE *outWiki = fopen("WeaponTypes.wiki", "w");
  if ( !outWiki )
    return;

  fprintf(outWiki, "#summary Weapon Types in BWAPI\n");
  fprintf(outWiki, "#sidebar TableOfContents\n\n");

  fprintf(outWiki, "This page contains a list of WeaponTypes provided by BWAPI.\n\n");

  fprintf(outWiki, "<wiki:toc max_depth=\"2\" />\n\n");

  std::vector<WeaponType> normWeaponTypes;
  std::vector<WeaponType> spellWeaponTypes;
  for each ( WeaponType u in WeaponTypes::normalWeaponTypes() )
    normWeaponTypes.push_back(u);

  for each ( WeaponType u in WeaponTypes::specialWeaponTypes() )
    spellWeaponTypes.push_back(u);

  std::sort(normWeaponTypes.begin(), normWeaponTypes.end(), weaponNameSort);
  std::sort(spellWeaponTypes.begin(), spellWeaponTypes.end(), weaponNameSort);

  fprintf(outWiki, "\n= Normal Weapons =\n");
  for each ( WeaponType w in normWeaponTypes )
    printWeaponData(outWiki, w);
  fprintf(outWiki, "\n= Special Weapons =\n");
  for each ( WeaponType w in spellWeaponTypes )
    printWeaponData(outWiki, w);

  fprintf(outWiki, "= None =\n");
  fprintf(outWiki, "A type used to specify that no weapon type exists.\n\n");
  fprintf(outWiki, "= Unknown =\n");
  fprintf(outWiki, "A type used to specify that we shouldn't know what the weapon type is, and that it has been made unknown to us. It is also used for weapon types that have not been implemented.\n\n");
  fclose(outWiki);
}
