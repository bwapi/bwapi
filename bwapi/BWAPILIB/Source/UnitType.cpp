#include <string>
#include <map>
#include <set>
#include <BWAPI/UnitType.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/UnitSizeType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Race.h>


namespace BWAPI
{
  bool initializingUnitType = true;
  class UnitTypeInternal
  {
    public:
      UnitTypeInternal() {valid = false;}
      void set(const char* name, const char* subLabel, Race race, const UnitType* whatBuilds, int whatBuildsAmt, const UnitType* requiredUnit1, int requiredUnit1Amt, const UnitType* requiredUnit2, int requiredUnit2Amt, const UnitType* requiredUnit3, int requiredUnit3Amt, const TechType* requiredTech, const TechType* ability1, const TechType* ability2, const TechType* ability3, const TechType* ability4, const UpgradeType* armorUpgrade, int maxHitPoints, int maxShields, int maxEnergy, int armor,  int mineralPrice, int gasPrice, int buildTime, int supplyRequired, int supplyProvided, int spaceRequired, int spaceProvided, int buildScore, int destroyScore, const UnitSizeType* unitSizeType, int tileWidth, int tileHeight, int dimensionLeft, int dimensionUp, int dimensionRight, int dimensionDown,  int seekRange, int sightRange, const WeaponType* groundWeapon, int maxGroundHits, const WeaponType* airWeapon, int maxAirHits, int topSpeed, int acceleration, int haltDistance, int turnRadius, bool canProduce, bool canAttack, bool canMove, bool isFlyer, bool regeneratesHP, bool isSpellcaster,  bool hasPermanentCloak, bool isInvincible, bool isOrganic, bool isMechanical, bool isRobotic, bool isDetector, bool isResourceContainer, bool isResourceDepot, bool isWorker, bool requiresPsi, bool requiresCreep, bool isTwoUnitsInOneEgg, bool isBurrowable, bool isCloakable, bool isBuilding, bool isAddon, bool isFlyingBuilding, bool isNeutral, bool isRefinery)
      {
        if (initializingUnitType)
        {
          this->name = name;
          this->subLabel = subLabel;
          this->race = race;

          this->whatBuilds = std::make_pair(whatBuilds, whatBuildsAmt);
          if (requiredUnit1 != NULL && requiredUnit1 != &(UnitTypes::None))
          {
            this->requiredUnits.insert(std::make_pair(requiredUnit1, requiredUnit1Amt));
          }
          if (requiredUnit2 != NULL && requiredUnit2 != &(UnitTypes::None))
          {
            this->requiredUnits.insert(std::make_pair(requiredUnit2, requiredUnit2Amt));
          }
          if (requiredUnit3 != NULL && requiredUnit3 != &(UnitTypes::None))
          {
            this->requiredUnits.insert(std::make_pair(requiredUnit3, requiredUnit3Amt));
          }
          this->requiredTech = requiredTech;
          if (ability1 != NULL && ability1 != &(TechTypes::None))
          {
            this->abilities.insert(ability1);
          }
          if (ability2 != NULL && ability2 != &(TechTypes::None))
          {
            this->abilities.insert(ability2);
          }
          if (ability3 != NULL && ability3 != &(TechTypes::None))
          {
            this->abilities.insert(ability3);
          }
          if (ability4 != NULL && ability4 != &(TechTypes::None))
          {
            this->abilities.insert(ability4);
          }
          this->armorUpgrade = armorUpgrade;

          this->maxHitPoints = maxHitPoints;
          this->maxShields = maxShields;
          this->maxEnergy = maxEnergy;
          this->armor = armor;

          this->mineralPrice = mineralPrice;
          this->gasPrice = gasPrice;
          this->buildTime = buildTime;

          this->supplyRequired = supplyRequired;
          this->supplyProvided = supplyProvided;
          this->spaceRequired = spaceRequired;
          this->spaceProvided = spaceProvided;
          this->buildScore = buildScore;
          this->destroyScore = destroyScore;

          this->unitSizeType = unitSizeType;
          this->tileWidth = tileWidth;
          this->tileHeight = tileHeight;
          this->dimensionLeft = dimensionLeft;
          this->dimensionUp = dimensionUp;
          this->dimensionRight = dimensionRight;
          this->dimensionDown = dimensionDown;

          this->seekRange = seekRange;
          this->sightRange = sightRange;
          this->groundWeapon = groundWeapon;
          this->maxGroundHits = maxGroundHits;
          this->airWeapon = airWeapon;
          this->maxAirHits = maxAirHits;

          this->topSpeed = topSpeed;
          this->acceleration = acceleration;
          this->haltDistance = haltDistance;
          this->turnRadius = turnRadius;

          this->canProduce = canProduce;
          this->canAttack = canAttack;
          this->canMove = canMove;

          this->isFlyer = isFlyer;
          this->regeneratesHP = regeneratesHP;
          this->isSpellcaster = isSpellcaster;
          this->hasPermanentCloak = hasPermanentCloak;
          this->isInvincible = isInvincible;
          this->isOrganic = isOrganic;
          this->isMechanical = isMechanical;
          this->isRobotic = isRobotic;
          this->isDetector = isDetector;
          this->isResourceContainer = isResourceContainer;
          this->isResourceDepot = isResourceDepot;
          this->isWorker = isWorker;
          this->requiresPsi = requiresPsi;
          this->requiresCreep = requiresCreep;
          this->isTwoUnitsInOneEgg = isTwoUnitsInOneEgg;
          this->isBurrowable = isBurrowable;
          this->isCloakable = isCloakable;
          this->isBuilding = isBuilding;
          this->isAddon = isAddon;
          this->isFlyingBuilding = isFlyingBuilding;
          this->isNeutral = isNeutral;
          this->isRefinery = isRefinery;

          this->valid = true;
        }
      }
      std::string name;
      std::string subLabel;
      Race race;

      std::pair< const UnitType*, int > whatBuilds;
      std::map< const UnitType*, int > requiredUnits;
      const TechType* requiredTech;
      std::set< const TechType* > abilities;
      std::set< const UpgradeType* > upgrades;
      const UpgradeType* armorUpgrade;

      int maxHitPoints;
      int maxShields;
      int maxEnergy;
      int armor;

      int mineralPrice;
      int gasPrice;
      int buildTime;

      int supplyRequired;
      int supplyProvided;
      int spaceRequired;
      int spaceProvided;
      int buildScore;
      int destroyScore;

      const UnitSizeType* unitSizeType;
      int tileWidth;
      int tileHeight;
      int dimensionLeft;
      int dimensionUp;
      int dimensionRight;
      int dimensionDown;

      int seekRange;
      int sightRange;
      const WeaponType* groundWeapon;
      int maxGroundHits;
      const WeaponType* airWeapon;
      int maxAirHits;

      int topSpeed;
      int acceleration;
      int haltDistance;
      int turnRadius;

      bool canProduce;
      bool canAttack;
      bool canMove;
      bool isFlyer;
      bool regeneratesHP;
      bool isSpellcaster;
      bool hasPermanentCloak;
      bool isInvincible;
      bool isOrganic;
      bool isMechanical;
      bool isRobotic;
      bool isDetector;
      bool isResourceContainer;
      bool isResourceDepot;
      bool isWorker;
      bool requiresPsi;
      bool requiresCreep;
      bool isTwoUnitsInOneEgg;
      bool isBurrowable;
      bool isCloakable;
      bool isBuilding;
      bool isAddon;
      bool isFlyingBuilding;
      bool isNeutral;
      bool isRefinery;

      bool valid;
  };
  UnitTypeInternal unitTypeData[245];
  std::map<std::string, UnitType> unitTypeMap;
  std::set< UnitType > unitTypeSet;
  namespace UnitTypes
  {
    const UnitType Terran_Marine(0);
    const UnitType Terran_Ghost(1);
    const UnitType Terran_Vulture(2);
    const UnitType Terran_Goliath(3);
    const UnitType Terran_Siege_Tank_Tank_Mode(5);
    const UnitType Terran_SCV(7);
    const UnitType Terran_Wraith(8);
    const UnitType Terran_Science_Vessel(9);
    const UnitType Terran_Dropship(11);
    const UnitType Terran_Battlecruiser(12);
    const UnitType Terran_Vulture_Spider_Mine(13);
    const UnitType Terran_Nuclear_Missile(14);
    const UnitType Terran_Siege_Tank_Siege_Mode(30);
    const UnitType Terran_Firebat(32);
    const UnitType Spell_Scanner_Sweep(33);
    const UnitType Terran_Medic(34);
    const UnitType Zerg_Larva(35);
    const UnitType Zerg_Egg(36);
    const UnitType Zerg_Zergling(37);
    const UnitType Zerg_Hydralisk(38);
    const UnitType Zerg_Ultralisk(39);
    const UnitType Zerg_Broodling(40);
    const UnitType Zerg_Drone(41);
    const UnitType Zerg_Overlord(42);
    const UnitType Zerg_Mutalisk(43);
    const UnitType Zerg_Guardian(44);
    const UnitType Zerg_Queen(45);
    const UnitType Zerg_Defiler(46);
    const UnitType Zerg_Scourge(47);
    const UnitType Zerg_Infested_Terran(50);
    const UnitType Terran_Valkyrie(58);
    const UnitType Zerg_Cocoon(59);
    const UnitType Protoss_Corsair(60);
    const UnitType Protoss_Dark_Templar(61);
    const UnitType Zerg_Devourer(62);
    const UnitType Protoss_Dark_Archon(63);
    const UnitType Protoss_Probe(64);
    const UnitType Protoss_Zealot(65);
    const UnitType Protoss_Dragoon(66);
    const UnitType Protoss_High_Templar(67);
    const UnitType Protoss_Archon(68);
    const UnitType Protoss_Shuttle(69);
    const UnitType Protoss_Scout(70);
    const UnitType Protoss_Arbiter(71);
    const UnitType Protoss_Carrier(72);
    const UnitType Protoss_Interceptor(73);
    const UnitType Protoss_Reaver(83);
    const UnitType Protoss_Observer(84);
    const UnitType Protoss_Scarab(85);
    const UnitType Critter_Rhynadon(89);
    const UnitType Critter_Bengalaas(90);
    const UnitType Critter_Scantid(93);
    const UnitType Critter_Kakaru(94);
    const UnitType Critter_Ragnasaur(95);
    const UnitType Critter_Ursadon(96);
    const UnitType Zerg_Lurker_Egg(97);
    const UnitType Zerg_Lurker(103);
    const UnitType Spell_Disruption_Web(105);
    const UnitType Terran_Command_Center(106);
    const UnitType Terran_Comsat_Station(107);
    const UnitType Terran_Nuclear_Silo(108);
    const UnitType Terran_Supply_Depot(109);
    const UnitType Terran_Refinery(110);
    const UnitType Terran_Barracks(111);
    const UnitType Terran_Academy(112);
    const UnitType Terran_Factory(113);
    const UnitType Terran_Starport(114);
    const UnitType Terran_Control_Tower(115);
    const UnitType Terran_Science_Facility(116);
    const UnitType Terran_Covert_Ops(117);
    const UnitType Terran_Physics_Lab(118);
    const UnitType Terran_Machine_Shop(120);
    const UnitType Terran_Engineering_Bay(122);
    const UnitType Terran_Armory(123);
    const UnitType Terran_Missile_Turret(124);
    const UnitType Terran_Bunker(125);
    const UnitType Special_Crashed_Norad_II(126);
    const UnitType Special_Ion_Cannon(127);
    const UnitType Zerg_Infested_Command_Center(130);
    const UnitType Zerg_Hatchery(131);
    const UnitType Zerg_Lair(132);
    const UnitType Zerg_Hive(133);
    const UnitType Zerg_Nydus_Canal(134);
    const UnitType Zerg_Hydralisk_Den(135);
    const UnitType Zerg_Defiler_Mound(136);
    const UnitType Zerg_Greater_Spire(137);
    const UnitType Zerg_Queens_Nest(138);
    const UnitType Zerg_Evolution_Chamber(139);
    const UnitType Zerg_Ultralisk_Cavern(140);
    const UnitType Zerg_Spire(141);
    const UnitType Zerg_Spawning_Pool(142);
    const UnitType Zerg_Creep_Colony(143);
    const UnitType Zerg_Spore_Colony(144);
    const UnitType Zerg_Sunken_Colony(146);
    const UnitType Special_Overmind_With_Shell(147);
    const UnitType Special_Overmind(148);
    const UnitType Zerg_Extractor(149);
    const UnitType Special_Mature_Chrysalis(150);
    const UnitType Special_Cerebrate(151);
    const UnitType Special_Cerebrate_Daggoth(152);
    const UnitType Protoss_Nexus(154);
    const UnitType Protoss_Robotics_Facility(155);
    const UnitType Protoss_Pylon(156);
    const UnitType Protoss_Assimilator(157);
    const UnitType Protoss_Observatory(159);
    const UnitType Protoss_Gateway(160);
    const UnitType Protoss_Photon_Cannon(162);
    const UnitType Protoss_Citadel_of_Adun(163);
    const UnitType Protoss_Cybernetics_Core(164);
    const UnitType Protoss_Templar_Archives(165);
    const UnitType Protoss_Forge(166);
    const UnitType Protoss_Stargate(167);
    const UnitType Special_Stasis_Cell_Prison(168);
    const UnitType Protoss_Fleet_Beacon(169);
    const UnitType Protoss_Arbiter_Tribunal(170);
    const UnitType Protoss_Robotics_Support_Bay(171);
    const UnitType Protoss_Shield_Battery(172);
    const UnitType Special_Khaydarin_Crystal_Form(173);
    const UnitType Special_Protoss_Temple(174);
    const UnitType Special_XelNaga_Temple(175);
    const UnitType Resource_Mineral_Field(176);
    const UnitType Resource_Vespene_Geyser(188);
    const UnitType Special_Warp_Gate(189);
    const UnitType Special_Psi_Disrupter(190);
    const UnitType Special_Power_Generator(200);
    const UnitType Special_Overmind_Cocoon(201);
    const UnitType Spell_Dark_Swarm(202);
    const UnitType None(228);
    const UnitType Unknown(229);
    void init()
    {
      unitTypeData[Terran_Marine.getID()].set("Terran Marine", "Private", Races::Terran, &(Terran_Barracks), 1, &(Terran_Barracks), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::Stim_Packs), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Infantry_Armor), 10240, 0, 0, 0, 50, 0, 360, 2, 0, 1, 0, 50, 100, &(UnitSizeTypes::Small), 1, 1, 8, 9, 8, 10, 4, 7, &(WeaponTypes::Gauss_Rifle), 1, &(WeaponTypes::Gauss_Rifle), 1, 1, 1, 1, 40, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Ghost.getID()].set("Terran Ghost", "Specialist", Races::Terran, &(Terran_Barracks), 1, &(Terran_Barracks), 1, &(Terran_Academy), 1, &(Terran_Covert_Ops), 1, &(TechTypes::None), &(TechTypes::Lockdown), &(TechTypes::Personnel_Cloaking), &(TechTypes::Nuclear_Strike), &(TechTypes::None), &(UpgradeTypes::Terran_Infantry_Armor), 11520, 0, 200, 0, 25, 75, 750, 2, 0, 1, 0, 175, 350, &(UnitSizeTypes::Small), 1, 1, 7, 10, 7, 11, 7, 9, &(WeaponTypes::C_10_Canister_Rifle), 1, &(WeaponTypes::C_10_Canister_Rifle), 1, 1, 1, 1, 40, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Vulture.getID()].set("Terran Vulture", "Sergeant", Races::Terran, &(Terran_Factory), 1, &(Terran_Factory), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::Spider_Mines), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Vehicle_Plating), 20480, 0, 0, 0, 75, 0, 450, 4, 0, 2, 0, 75, 150, &(UnitSizeTypes::Medium), 1, 1, 16, 16, 15, 15, 5, 8, &(WeaponTypes::Fragmentation_Grenade), 1, &(WeaponTypes::None), 0, 1707, 100, 14569, 40, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Goliath.getID()].set("Terran Goliath", "First Sergeant", Races::Terran, &(Terran_Factory), 1, &(Terran_Factory), 1, &(Terran_Armory), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Vehicle_Plating), 32000, 0, 0, 1, 100, 50, 600, 4, 0, 2, 0, 200, 400, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 6, 8, &(WeaponTypes::Twin_Autocannons), 0, &(WeaponTypes::Hellfire_Missile_Pack), 0, 1, 1, 1, 17, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Siege_Tank_Tank_Mode.getID()].set("Terran Siege Tank Tank Mode", "Master Sergeant", Races::Terran, &(Terran_Factory), 1, &(Terran_Factory), 1, &(Terran_Machine_Shop), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Tank_Siege_Mode), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Vehicle_Plating), 38400, 0, 0, 1, 150, 100, 750, 4, 0, 4, 0, 350, 700, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 8, 10, &(WeaponTypes::Arclite_Cannon), 0, &(WeaponTypes::None), 0, 1, 1, 1, 13, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_SCV.getID()].set("Terran SCV", "Private", Races::Terran, &(Terran_Command_Center), 1, &(Terran_Command_Center), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Infantry_Armor), 15360, 0, 0, 0, 50, 0, 300, 2, 0, 1, 0, 50, 100, &(UnitSizeTypes::Small), 1, 1, 11, 11, 11, 11, 1, 7, &(WeaponTypes::Fusion_Cutter), 1, &(WeaponTypes::None), 0, 1280, 67, 12227, 40, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Wraith.getID()].set("Terran Wraith", "Captain", Races::Terran, &(Terran_Starport), 1, &(Terran_Starport), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::Cloaking_Field), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Ship_Plating), 30720, 0, 200, 0, 150, 100, 900, 4, 0, 255, 0, 400, 800, &(UnitSizeTypes::Large), 1, 1, 19, 15, 18, 14, 5, 7, &(WeaponTypes::Burst_Lasers), 1, &(WeaponTypes::Gemini_Missiles), 1, 1707, 67, 21745, 40, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Science_Vessel.getID()].set("Terran Science Vessel", "Major", Races::Terran, &(Terran_Starport), 1, &(Terran_Starport), 1, &(Terran_Control_Tower), 1, &(Terran_Science_Facility), 1, &(TechTypes::None), &(TechTypes::EMP_Shockwave), &(TechTypes::Defensive_Matrix), &(TechTypes::Irradiate), &(TechTypes::None), &(UpgradeTypes::Terran_Ship_Plating), 51200, 0, 200, 1, 100, 225, 1200, 4, 0, 255, 0, 625, 1250, &(UnitSizeTypes::Large), 2, 2, 32, 33, 32, 16, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1280, 50, 5120, 40, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Dropship.getID()].set("Terran Dropship", "Warrant Officer", Races::Terran, &(Terran_Starport), 1, &(Terran_Starport), 1, &(Terran_Control_Tower), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Ship_Plating), 38400, 0, 0, 1, 100, 100, 750, 4, 0, 255, 8, 300, 600, &(UnitSizeTypes::Large), 2, 2, 24, 16, 24, 20, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1400, 17, 37756, 20, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Battlecruiser.getID()].set("Terran Battlecruiser", "Commodore", Races::Terran, &(Terran_Starport), 1, &(Terran_Starport), 1, &(Terran_Control_Tower), 1, &(Terran_Physics_Lab), 1, &(TechTypes::None), &(TechTypes::Yamato_Gun), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Ship_Plating), 62464, 0, 200, 3, 400, 300, 2000, 12, 0, 255, 0, 1200, 2400, &(UnitSizeTypes::Large), 2, 2, 37, 29, 37, 29, 6, 11, &(WeaponTypes::ATS_Laser_Battery), 1, &(WeaponTypes::ATA_Laser_Battery), 1, 640, 27, 7585, 20, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Vulture_Spider_Mine.getID()].set("Terran Vulture Spider Mine", "", Races::Terran, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 5120, 0, 0, 0, 1, 0, 1, 0, 0, 255, 0, 0, 25, &(UnitSizeTypes::Small), 1, 1, 7, 7, 7, 7, 3, 3, &(WeaponTypes::Spider_Mines), 1, &(WeaponTypes::None), 0, 1, 1, 1, 127, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Nuclear_Missile.getID()].set("Terran Nuclear Missile", "", Races::Terran, &(Terran_Nuclear_Silo), 1, &(Terran_Nuclear_Silo), 1, &(Terran_Command_Center), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 25600, 0, 0, 0, 200, 200, 1500, 16, 0, 255, 0, 800, 0, &(UnitSizeTypes::Independent), 1, 1, 7, 14, 7, 14, 0, 3, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 8533, 33, 1103213, 127, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Siege_Tank_Siege_Mode.getID()].set("Terran Siege Tank Siege Mode", "Master Sergeant", Races::Terran, &(Terran_Factory), 1, &(Terran_Factory), 1, &(Terran_Machine_Shop), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Tank_Siege_Mode), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Vehicle_Plating), 38400, 0, 0, 1, 150, 100, 750, 4, 0, 255, 0, 0, 700, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 12, 10, &(WeaponTypes::Arclite_Shock_Cannon), 0, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Firebat.getID()].set("Terran Firebat", "Corporal", Races::Terran, &(Terran_Barracks), 1, &(Terran_Barracks), 1, &(Terran_Academy), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Stim_Packs), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Infantry_Armor), 12800, 0, 0, 1, 50, 25, 360, 2, 0, 1, 0, 100, 200, &(UnitSizeTypes::Small), 1, 1, 11, 7, 11, 14, 3, 7, &(WeaponTypes::Flame_Thrower), 3, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Spell_Scanner_Sweep.getID()].set("Spell Scanner Sweep", "", Races::Terran, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 1, 0, 0, 0, 0, 0, 1, 0, 0, 255, 0, 0, 0, &(UnitSizeTypes::Independent), 1, 1, 13, 13, 13, 17, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Medic.getID()].set("Terran Medic", "1st Lieutenant", Races::Terran, &(Terran_Barracks), 1, &(Terran_Barracks), 1, &(Terran_Academy), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Restoration), &(TechTypes::Optical_Flare), &(TechTypes::Healing), &(TechTypes::None), &(UpgradeTypes::Terran_Infantry_Armor), 15360, 0, 200, 1, 50, 25, 450, 2, 0, 1, 0, 125, 250, &(UnitSizeTypes::Small), 1, 1, 8, 9, 8, 10, 9, 9, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Larva.getID()].set("Zerg Larva", "", Races::Zerg, &(Zerg_Hatchery), 1, &(Zerg_Hatchery), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 6400, 0, 0, 10, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 8, 8, 7, 7, 0, 4, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 20, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Egg.getID()].set("Zerg Egg", "Private", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 51200, 0, 0, 10, 1, 1, 1, 0, 0, 255, 0, 0, 25, &(UnitSizeTypes::Medium), 1, 1, 16, 16, 15, 15, 0, 4, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Zergling.getID()].set("Zerg Zergling", "Specialist", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Spawning_Pool), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Burrowing), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 8960, 0, 0, 0, 50, 0, 420, 1, 0, 1, 0, 25, 50, &(UnitSizeTypes::Small), 1, 1, 8, 4, 7, 11, 3, 5, &(WeaponTypes::Claws), 1, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Hydralisk.getID()].set("Zerg Hydralisk", "Warrant Officer", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Hydralisk_Den), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Burrowing), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 20480, 0, 0, 0, 75, 25, 420, 2, 0, 2, 0, 125, 350, &(UnitSizeTypes::Medium), 1, 1, 10, 10, 10, 12, 4, 6, &(WeaponTypes::Needle_Spines), 1, &(WeaponTypes::Needle_Spines), 1, 1, 1, 1, 27, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Ultralisk.getID()].set("Zerg Ultralisk", "Colonel", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Ultralisk_Cavern), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 36864, 0, 0, 1, 200, 200, 900, 8, 0, 4, 0, 650, 1300, &(UnitSizeTypes::Large), 2, 2, 19, 16, 18, 15, 3, 7, &(WeaponTypes::Kaiser_Blades), 1, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Broodling.getID()].set("Zerg Broodling", "1st Lieutenant", Races::Zerg, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 7680, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 25, &(UnitSizeTypes::Small), 1, 1, 9, 9, 9, 9, 3, 5, &(WeaponTypes::Toxic_Spores), 1, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Drone.getID()].set("Zerg Drone", "Corporal", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::Burrowing), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 10240, 0, 0, 0, 50, 0, 300, 2, 0, 1, 0, 50, 100, &(UnitSizeTypes::Small), 1, 1, 11, 11, 11, 11, 1, 7, &(WeaponTypes::Spines), 1, &(WeaponTypes::None), 0, 1280, 67, 12227, 40, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Overlord.getID()].set("Zerg Overlord", "Master Sergeant", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Flyer_Carapace), 51200, 0, 0, 0, 100, 0, 600, 0, 16, 255, 8, 100, 200, &(UnitSizeTypes::Large), 2, 2, 25, 25, 24, 24, 0, 9, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 213, 27, 840, 20, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Mutalisk.getID()].set("Zerg Mutalisk", "Captain", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Spire), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Flyer_Carapace), 30720, 0, 0, 0, 100, 100, 600, 4, 0, 255, 0, 300, 600, &(UnitSizeTypes::Small), 2, 2, 22, 22, 21, 21, 3, 7, &(WeaponTypes::Glave_Wurm), 1, &(WeaponTypes::Glave_Wurm), 1, 1707, 67, 21745, 40, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Guardian.getID()].set("Zerg Guardian", "Major", Races::Zerg, &(Zerg_Mutalisk), 1, &(Zerg_Mutalisk), 1, &(Zerg_Greater_Spire), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Flyer_Carapace), 38400, 0, 0, 2, 50, 100, 600, 4, 0, 255, 0, 550, 1100, &(UnitSizeTypes::Large), 2, 2, 22, 22, 21, 21, 8, 11, &(WeaponTypes::Acid_Spore), 1, &(WeaponTypes::None), 0, 640, 27, 7585, 20, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Queen.getID()].set("Zerg Queen", "Sergeant Major", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Queens_Nest), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Infestation), &(TechTypes::Spawn_Broodlings), &(TechTypes::Ensnare), &(TechTypes::Parasite), &(UpgradeTypes::Zerg_Flyer_Carapace), 30720, 0, 200, 0, 100, 100, 750, 4, 0, 255, 0, 400, 800, &(UnitSizeTypes::Medium), 2, 2, 24, 24, 23, 23, 8, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1707, 67, 21745, 40, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Defiler.getID()].set("Zerg Defiler", "First Sergeant", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Defiler_Mound), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Burrowing), &(TechTypes::Dark_Swarm), &(TechTypes::Plague), &(TechTypes::Consume), &(UpgradeTypes::Zerg_Carapace), 20480, 0, 200, 1, 50, 150, 750, 4, 0, 2, 0, 225, 450, &(UnitSizeTypes::Medium), 1, 1, 13, 12, 13, 12, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Scourge.getID()].set("Zerg Scourge", "Sergeant", Races::Zerg, &(Zerg_Larva), 1, &(Zerg_Larva), 1, &(Zerg_Spire), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Flyer_Carapace), 6400, 0, 0, 0, 25, 75, 450, 1, 0, 255, 0, 100, 200, &(UnitSizeTypes::Small), 1, 1, 12, 12, 11, 11, 3, 5, &(WeaponTypes::None), 0, &(WeaponTypes::Suicide_Scourge), 1, 1707, 107, 13616, 40, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Infested_Terran.getID()].set("Zerg Infested Terran", "First Sergeant", Races::Zerg, &(Zerg_Infested_Command_Center), 1, &(Zerg_Infested_Command_Center), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 15360, 0, 0, 0, 100, 50, 600, 2, 0, 1, 0, 200, 400, &(UnitSizeTypes::Small), 1, 1, 8, 9, 8, 10, 3, 5, &(WeaponTypes::Suicide_Infested_Terran), 1, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
      unitTypeData[Terran_Valkyrie.getID()].set("Terran Valkyrie", "Lt Commander", Races::Terran, &(Terran_Starport), 1, &(Terran_Starport), 1, &(Terran_Control_Tower), 1, &(Terran_Armory), 1, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Terran_Ship_Plating), 51200, 0, 0, 2, 250, 125, 750, 6, 0, 255, 0, 400, 800, &(UnitSizeTypes::Large), 2, 2, 24, 16, 24, 20, 6, 8, &(WeaponTypes::None), 0, &(WeaponTypes::Halo_Rockets), 4, 1690, 65, 21901, 30, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Cocoon.getID()].set("Zerg Cocoon", "Private", Races::Zerg, &(Zerg_Mutalisk), 1, &(Zerg_Mutalisk), 1, &(Zerg_Greater_Spire), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 51200, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 1100, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 0, 4, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Corsair.getID()].set("Protoss Corsair", "Master Sergeant", Races::Protoss, &(Protoss_Stargate), 1, &(Protoss_Stargate), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::Disruption_Web), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 25600, 80, 200, 1, 150, 100, 600, 4, 0, 255, 0, 350, 700, &(UnitSizeTypes::Medium), 1, 1, 18, 16, 17, 15, 9, 9, &(WeaponTypes::None), 0, &(WeaponTypes::Neutron_Flare), 1, 1707, 67, 17067, 30, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Dark_Templar.getID()].set("Protoss Dark Templar", "First Sergeant", Races::Protoss, &(Protoss_Gateway), 1, &(Protoss_Gateway), 1, &(Protoss_Templar_Archives), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Dark_Archon_Meld), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 20480, 40, 0, 1, 125, 100, 750, 4, 0, 2, 0, 325, 650, &(UnitSizeTypes::Small), 1, 1, 12, 6, 11, 19, 3, 7, &(WeaponTypes::Warp_Blades), 1, &(WeaponTypes::None), 0, 750, 27, 13474, 40, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Devourer.getID()].set("Zerg Devourer", "First Sergeant", Races::Zerg, &(Zerg_Mutalisk), 1, &(Zerg_Mutalisk), 1, &(Zerg_Greater_Spire), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Flyer_Carapace), 64000, 0, 0, 2, 150, 50, 600, 4, 0, 255, 0, 550, 1100, &(UnitSizeTypes::Large), 2, 2, 22, 22, 21, 21, 7, 10, &(WeaponTypes::None), 0, &(WeaponTypes::Corrosive_Acid), 1, 1280, 48, 17067, 30, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Dark_Archon.getID()].set("Protoss Dark Archon", "Warrant Officer", Races::Protoss, &(Protoss_Dark_Templar), 2, &(Protoss_Dark_Templar), 2, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::Mind_Control), &(TechTypes::Feedback), &(TechTypes::Maelstrom), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 6400, 200, 200, 1, 0, 0, 300, 8, 0, 4, 0, 650, 1300, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 7, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1280, 160, 5120, 40, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Probe.getID()].set("Protoss Probe", "", Races::Protoss, &(Protoss_Nexus), 1, &(Protoss_Nexus), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 5120, 20, 0, 0, 50, 0, 300, 2, 0, 1, 0, 50, 100, &(UnitSizeTypes::Small), 1, 1, 11, 11, 11, 11, 1, 8, &(WeaponTypes::Particle_Beam), 1, &(WeaponTypes::None), 0, 1280, 67, 12227, 40, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Zealot.getID()].set("Protoss Zealot", "1st Lieutenant", Races::Protoss, &(Protoss_Gateway), 1, &(Protoss_Gateway), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 25600, 60, 0, 1, 100, 0, 600, 4, 0, 2, 0, 100, 200, &(UnitSizeTypes::Small), 1, 1, 11, 5, 11, 13, 3, 7, &(WeaponTypes::Psi_Blades), 2, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Dragoon.getID()].set("Protoss Dragoon", "Corporal", Races::Protoss, &(Protoss_Gateway), 1, &(Protoss_Gateway), 1, &(Protoss_Cybernetics_Core), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 25600, 80, 0, 1, 125, 50, 750, 4, 0, 4, 0, 250, 500, &(UnitSizeTypes::Large), 1, 1, 15, 15, 16, 16, 4, 8, &(WeaponTypes::Phase_Disruptor), 1, &(WeaponTypes::Phase_Disruptor), 1, 1, 1, 1, 40, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_High_Templar.getID()].set("Protoss High Templar", "Sergeant", Races::Protoss, &(Protoss_Gateway), 1, &(Protoss_Gateway), 1, &(Protoss_Templar_Archives), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Psionic_Storm), &(TechTypes::Hallucination), &(TechTypes::Archon_Warp), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 10240, 40, 200, 0, 50, 150, 750, 4, 0, 2, 0, 350, 700, &(UnitSizeTypes::Small), 1, 1, 12, 10, 11, 13, 3, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 853, 27, 13474, 40, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Archon.getID()].set("Protoss Archon", "Warrant Officer", Races::Protoss, &(Protoss_High_Templar), 2, &(Protoss_High_Templar), 2, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 2560, 350, 0, 0, 0, 0, 300, 8, 0, 4, 0, 700, 1400, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 3, 8, &(WeaponTypes::Psionic_Shockwave), 1, &(WeaponTypes::Psionic_Shockwave), 1, 1280, 160, 5120, 40, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Shuttle.getID()].set("Protoss Shuttle", "First Sergeant", Races::Protoss, &(Protoss_Robotics_Facility), 1, &(Protoss_Robotics_Facility), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 20480, 60, 0, 1, 200, 0, 900, 4, 0, 255, 8, 200, 400, &(UnitSizeTypes::Large), 2, 1, 20, 16, 19, 15, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1133, 17, 37756, 20, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Scout.getID()].set("Protoss Scout", "Master Sergeant", Races::Protoss, &(Protoss_Stargate), 1, &(Protoss_Stargate), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 38400, 100, 0, 0, 275, 125, 1200, 6, 0, 255, 0, 650, 1300, &(UnitSizeTypes::Large), 2, 1, 18, 16, 17, 15, 4, 8, &(WeaponTypes::Dual_Photon_Blasters), 1, &(WeaponTypes::Anti_matter_Missiles), 1, 1280, 48, 17067, 30, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Arbiter.getID()].set("Protoss Arbiter", "Captain", Races::Protoss, &(Protoss_Stargate), 1, &(Protoss_Stargate), 1, &(Protoss_Arbiter_Tribunal), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Recall), &(TechTypes::Stasis_Field), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 51200, 150, 200, 1, 100, 350, 2400, 8, 0, 255, 0, 1025, 2050, &(UnitSizeTypes::Large), 2, 2, 22, 22, 21, 21, 5, 9, &(WeaponTypes::Phase_Disruptor_Cannon), 1, &(WeaponTypes::Phase_Disruptor_Cannon), 1, 1280, 33, 24824, 40, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Carrier.getID()].set("Protoss Carrier", "Major", Races::Protoss, &(Protoss_Stargate), 1, &(Protoss_Stargate), 1, &(Protoss_Fleet_Beacon), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 11264, 150, 0, 4, 350, 250, 2100, 12, 0, 255, 0, 950, 1900, &(UnitSizeTypes::Large), 2, 2, 32, 32, 31, 31, 8, 11, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 853, 27, 13474, 20, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Interceptor.getID()].set("Protoss Interceptor", "Private", Races::Protoss, &(Protoss_Carrier), 1, &(Protoss_Carrier), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 10240, 40, 0, 0, 25, 0, 300, 0, 0, 255, 0, 30, 60, &(UnitSizeTypes::Small), 1, 1, 8, 8, 7, 7, 4, 6, &(WeaponTypes::Pulse_Cannon), 1, &(WeaponTypes::Pulse_Cannon), 1, 3413, 427, 13640, 40, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Reaver.getID()].set("Protoss Reaver", "Specialist", Races::Protoss, &(Protoss_Robotics_Facility), 1, &(Protoss_Robotics_Facility), 1, &(Protoss_Robotics_Support_Bay), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 25600, 80, 0, 0, 200, 100, 1050, 8, 0, 4, 0, 400, 800, &(UnitSizeTypes::Large), 1, 1, 16, 16, 15, 15, 8, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 20, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Observer.getID()].set("Protoss Observer", "Private", Races::Protoss, &(Protoss_Robotics_Facility), 1, &(Protoss_Robotics_Facility), 1, &(Protoss_Observatory), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Plating), 10240, 20, 0, 0, 25, 75, 600, 2, 0, 255, 0, 225, 450, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 9, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 853, 27, 13474, 20, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Protoss_Scarab.getID()].set("Protoss Scarab", "", Races::Protoss, &(Protoss_Reaver), 1, &(Protoss_Reaver), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Protoss_Armor), 5120, 10, 0, 0, 15, 0, 105, 0, 0, 255, 0, 0, 0, &(UnitSizeTypes::Small), 1, 1, 2, 2, 2, 2, 4, 5, &(WeaponTypes::Scarab), 1, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Critter_Rhynadon.getID()].set("Critter Rhynadon", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 15360, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Critter_Bengalaas.getID()].set("Critter Bengalaas", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 15360, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Critter_Scantid.getID()].set("Critter Scantid", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 15360, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Critter_Kakaru.getID()].set("Critter Kakaru", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 15360, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1280, 16, 51200, 14, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Critter_Ragnasaur.getID()].set("Critter Ragnasaur", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 15360, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Critter_Ursadon.getID()].set("Critter Ursadon", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 15360, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 0, 10, &(UnitSizeTypes::Small), 1, 1, 16, 16, 15, 15, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 1, 1, 1, 27, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Zerg_Lurker_Egg.getID()].set("Zerg Lurker Egg", "Private", Races::Zerg, &(Zerg_Hydralisk), 1, &(Zerg_Hydralisk), 1, NULL, 0, NULL, 0, &(TechTypes::Lurker_Aspect), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 51200, 0, 0, 10, 1, 1, 1, 0, 0, 255, 0, 0, 500, &(UnitSizeTypes::Medium), 1, 1, 16, 16, 15, 15, 0, 4, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Zerg_Lurker.getID()].set("Zerg Lurker", "Captain", Races::Zerg, &(Zerg_Hydralisk), 1, &(Zerg_Hydralisk), 1, NULL, 0, NULL, 0, &(TechTypes::Lurker_Aspect), &(TechTypes::Lurker_Aspect), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::Zerg_Carapace), 32000, 0, 0, 1, 50, 100, 600, 4, 0, 4, 0, 250, 500, &(UnitSizeTypes::Medium), 1, 1, 15, 15, 16, 16, 6, 8, &(WeaponTypes::Subterranean_Spines), 1, &(WeaponTypes::None), 0, 1, 1, 1, 40, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
      unitTypeData[Spell_Disruption_Web.getID()].set("Spell Disruption Web", "", Races::Protoss, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 8192, 0, 0, 0, 250, 250, 2400, 0, 0, 255, 0, 0, 0, &(UnitSizeTypes::Independent), 4, 3, 60, 40, 59, 39, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[Terran_Command_Center.getID()].set("Terran Command Center", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 56320, 0, 0, 1, 400, 0, 1800, 0, 20, 255, 0, 400, 1200, &(UnitSizeTypes::Large), 4, 3, 58, 41, 58, 41, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Terran_Comsat_Station.getID()].set("Terran Comsat Station", "", Races::Terran, &(Terran_Command_Center), 1, &(Terran_Command_Center), 1, &(Terran_Academy), 1, NULL, 0, &(TechTypes::None), &(TechTypes::Scanner_Sweep), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 0, 200, 1, 50, 50, 600, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 2, 2, 37, 16, 31, 25, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0);
      unitTypeData[Terran_Nuclear_Silo.getID()].set("Terran Nuclear Silo", "", Races::Terran, &(Terran_Command_Center), 1, &(Terran_Command_Center), 1, &(Terran_Science_Facility), 1, &(Terran_Covert_Ops), 1, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 22528, 0, 0, 1, 100, 100, 1200, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 2, 2, 37, 16, 31, 25, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0);
      unitTypeData[Terran_Supply_Depot.getID()].set("Terran Supply Depot", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 0, 0, 1, 100, 0, 600, 0, 16, 255, 0, 50, 150, &(UnitSizeTypes::Large), 3, 2, 38, 22, 38, 26, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Terran_Refinery.getID()].set("Terran Refinery", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 100, 0, 600, 0, 0, 255, 0, 50, 150, &(UnitSizeTypes::Large), 4, 2, 56, 32, 56, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1);
      unitTypeData[Terran_Barracks.getID()].set("Terran Barracks", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Command_Center), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 59392, 0, 0, 1, 150, 0, 1200, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 4, 3, 48, 40, 56, 32, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Terran_Academy.getID()].set("Terran Academy", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Barracks), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 22528, 0, 0, 1, 150, 0, 1200, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 3, 2, 40, 32, 44, 24, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Terran_Factory.getID()].set("Terran Factory", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Barracks), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 57856, 0, 0, 1, 200, 100, 1200, 0, 0, 255, 0, 200, 600, &(UnitSizeTypes::Large), 4, 3, 56, 40, 56, 40, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Terran_Starport.getID()].set("Terran Starport", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Factory), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 5120, 0, 0, 1, 150, 100, 1050, 0, 0, 255, 0, 200, 600, &(UnitSizeTypes::Large), 4, 3, 48, 40, 48, 38, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Terran_Control_Tower.getID()].set("Terran Control Tower", "", Races::Terran, &(Terran_Starport), 1, &(Terran_Starport), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 0, 0, 1, 50, 50, 600, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 2, 2, 47, 24, 28, 22, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0);
      unitTypeData[Terran_Science_Facility.getID()].set("Terran Science Facility", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Starport), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 20992, 0, 0, 1, 100, 150, 900, 0, 0, 255, 0, 275, 825, &(UnitSizeTypes::Large), 4, 3, 48, 38, 48, 38, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Terran_Covert_Ops.getID()].set("Terran Covert Ops", "", Races::Terran, &(Terran_Science_Facility), 1, &(Terran_Science_Facility), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 50, 50, 600, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 2, 2, 47, 24, 28, 22, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0);
      unitTypeData[Terran_Physics_Lab.getID()].set("Terran Physics Lab", "", Races::Terran, &(Terran_Science_Facility), 1, &(Terran_Science_Facility), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 22528, 0, 0, 1, 50, 50, 600, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 2, 2, 47, 24, 28, 22, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0);
      unitTypeData[Terran_Machine_Shop.getID()].set("Terran Machine Shop", "", Races::Terran, &(Terran_Factory), 1, &(Terran_Factory), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 50, 50, 600, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 2, 2, 39, 24, 31, 24, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0);
      unitTypeData[Terran_Engineering_Bay.getID()].set("Terran Engineering Bay", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Command_Center), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 20992, 0, 0, 1, 125, 0, 900, 0, 0, 255, 0, 65, 195, &(UnitSizeTypes::Large), 4, 3, 48, 32, 48, 28, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Terran_Armory.getID()].set("Terran Armory", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Factory), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 100, 50, 1200, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 3, 2, 48, 32, 47, 22, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Terran_Missile_Turret.getID()].set("Terran Missile Turret", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Engineering_Bay), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 51200, 0, 0, 0, 75, 0, 450, 0, 0, 255, 0, 50, 150, &(UnitSizeTypes::Large), 2, 2, 16, 32, 16, 16, 7, 11, &(WeaponTypes::None), 0, &(WeaponTypes::Longbolt_Missile), 1, 0, 0, 0, 40, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Terran_Bunker.getID()].set("Terran Bunker", "", Races::Terran, &(Terran_SCV), 1, &(Terran_SCV), 1, &(Terran_Barracks), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 24064, 0, 0, 1, 100, 0, 450, 0, 0, 255, 4, 50, 150, &(UnitSizeTypes::Large), 3, 2, 32, 24, 32, 16, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Crashed_Norad_II.getID()].set("Special Crashed Norad II", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 48128, 0, 0, 1, 800, 600, 4800, 0, 0, 255, 0, 0, 5000, &(UnitSizeTypes::Large), 3, 2, 48, 32, 47, 31, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Ion_Cannon.getID()].set("Special Ion Cannon", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 53248, 0, 0, 1, 200, 0, 900, 0, 0, 255, 0, 0, 5000, &(UnitSizeTypes::Large), 3, 2, 48, 32, 47, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Infested_Command_Center.getID()].set("Zerg Infested Command Center", "", Races::Zerg, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 56320, 0, 0, 1, 1, 1, 1800, 0, 0, 255, 0, 300, 900, &(UnitSizeTypes::Large), 4, 3, 58, 41, 58, 41, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 427, 33, 2763, 27, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0);
      unitTypeData[Zerg_Hatchery.getID()].set("Zerg Hatchery", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 57856, 0, 0, 1, 300, 0, 1800, 0, 2, 255, 0, 300, 900, &(UnitSizeTypes::Large), 4, 3, 49, 32, 49, 32, 0, 9, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Lair.getID()].set("Zerg Lair", "", Races::Zerg, &(Zerg_Hatchery), 1, &(Zerg_Hatchery), 1, &(Zerg_Spawning_Pool), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 2048, 0, 0, 1, 150, 100, 1500, 0, 2, 255, 0, 100, 1200, &(UnitSizeTypes::Large), 4, 3, 49, 32, 49, 32, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Hive.getID()].set("Zerg Hive", "", Races::Zerg, &(Zerg_Lair), 1, &(Zerg_Lair), 1, &(Zerg_Queens_Nest), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 50176, 0, 0, 1, 200, 150, 1800, 0, 2, 255, 0, 100, 1500, &(UnitSizeTypes::Large), 4, 3, 49, 32, 49, 32, 0, 11, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Nydus_Canal.getID()].set("Zerg Nydus Canal", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Hive), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 64000, 0, 0, 1, 150, 0, 600, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 2, 2, 32, 32, 31, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Hydralisk_Den.getID()].set("Zerg Hydralisk Den", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Spawning_Pool), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 20992, 0, 0, 1, 100, 50, 600, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 3, 2, 40, 32, 40, 24, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Defiler_Mound.getID()].set("Zerg Defiler Mound", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Hive), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 20992, 0, 0, 1, 100, 100, 900, 0, 0, 255, 0, 150, 450, &(UnitSizeTypes::Large), 4, 2, 48, 32, 48, 4, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Greater_Spire.getID()].set("Zerg Greater Spire", "", Races::Zerg, &(Zerg_Spire), 1, &(Zerg_Hive), 1, &(Zerg_Spire), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 59392, 0, 0, 1, 100, 150, 1800, 0, 0, 255, 0, 200, 1350, &(UnitSizeTypes::Large), 2, 2, 28, 32, 28, 24, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Queens_Nest.getID()].set("Zerg Queen's Nest", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Lair), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 20992, 0, 0, 1, 150, 100, 900, 0, 0, 255, 0, 175, 525, &(UnitSizeTypes::Large), 3, 2, 38, 28, 32, 28, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Evolution_Chamber.getID()].set("Zerg Evolution Chamber", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Hatchery), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 75, 0, 600, 0, 0, 255, 0, 40, 120, &(UnitSizeTypes::Large), 3, 2, 44, 32, 32, 20, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Ultralisk_Cavern.getID()].set("Zerg Ultralisk Cavern", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Hive), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 22528, 0, 0, 1, 150, 200, 1200, 0, 0, 255, 0, 275, 825, &(UnitSizeTypes::Large), 3, 2, 40, 32, 32, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Spire.getID()].set("Zerg Spire", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Lair), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 22528, 0, 0, 1, 200, 150, 1800, 0, 0, 255, 0, 250, 750, &(UnitSizeTypes::Large), 2, 2, 28, 32, 28, 24, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Spawning_Pool.getID()].set("Zerg Spawning Pool", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, &(Zerg_Hatchery), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 200, 0, 1200, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 3, 2, 36, 28, 40, 18, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Creep_Colony.getID()].set("Zerg Creep Colony", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 36864, 0, 0, 0, 75, 0, 300, 0, 0, 255, 0, 40, 120, &(UnitSizeTypes::Large), 2, 2, 24, 24, 23, 23, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Spore_Colony.getID()].set("Zerg Spore Colony", "", Races::Zerg, &(Zerg_Creep_Colony), 1, &(Zerg_Evolution_Chamber), 1, &(Zerg_Creep_Colony), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 36864, 0, 0, 0, 50, 0, 300, 0, 0, 255, 0, 25, 195, &(UnitSizeTypes::Large), 2, 2, 24, 24, 23, 23, 7, 10, &(WeaponTypes::None), 0, &(WeaponTypes::Seeker_Spores), 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Sunken_Colony.getID()].set("Zerg Sunken Colony", "", Races::Zerg, &(Zerg_Creep_Colony), 1, &(Zerg_Spawning_Pool), 1, &(Zerg_Creep_Colony), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 11264, 0, 0, 2, 50, 0, 300, 0, 0, 255, 0, 40, 240, &(UnitSizeTypes::Large), 2, 2, 24, 24, 23, 23, 7, 10, &(WeaponTypes::Subterranean_Tentacle), 1, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Overmind_With_Shell.getID()].set("Special Overmind With Shell", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 34816, 0, 0, 1, 1, 1, 1, 0, 0, 255, 0, 0, 10000, &(UnitSizeTypes::Large), 5, 3, 80, 32, 79, 40, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Overmind.getID()].set("Special Overmind", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 50176, 0, 0, 1, 1, 1, 1, 0, 0, 255, 0, 0, 10000, &(UnitSizeTypes::Large), 5, 3, 80, 32, 79, 40, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Zerg_Extractor.getID()].set("Zerg Extractor", "", Races::Zerg, &(Zerg_Drone), 1, &(Zerg_Drone), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 0, 0, 1, 50, 0, 600, 0, 0, 255, 0, 25, 75, &(UnitSizeTypes::Large), 4, 2, 64, 32, 63, 31, 0, 7, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1);
      unitTypeData[Special_Mature_Chrysalis.getID()].set("Special Mature Chrysalis", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 64000, 0, 0, 1, 0, 0, 0, 0, 0, 255, 0, 0, 5000, &(UnitSizeTypes::Large), 2, 2, 32, 32, 31, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Cerebrate.getID()].set("Special Cerebrate", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 56320, 0, 0, 1, 0, 0, 0, 0, 0, 255, 0, 0, 2500, &(UnitSizeTypes::Large), 3, 2, 40, 32, 32, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Cerebrate_Daggoth.getID()].set("Special Cerebrate Daggoth", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 56320, 0, 0, 1, 0, 0, 0, 0, 0, 255, 0, 0, 2500, &(UnitSizeTypes::Large), 3, 2, 40, 32, 32, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Nexus.getID()].set("Protoss Nexus", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 60928, 750, 0, 1, 400, 0, 1800, 0, 18, 255, 0, 400, 1200, &(UnitSizeTypes::Large), 4, 3, 56, 39, 56, 39, 0, 11, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Robotics_Facility.getID()].set("Protoss Robotics Facility", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Cybernetics_Core), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 500, 0, 1, 200, 200, 1200, 0, 0, 255, 0, 300, 900, &(UnitSizeTypes::Large), 3, 2, 36, 16, 40, 20, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Pylon.getID()].set("Protoss Pylon", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 11264, 300, 0, 0, 100, 0, 450, 0, 16, 255, 0, 50, 150, &(UnitSizeTypes::Large), 2, 2, 16, 12, 16, 20, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Assimilator.getID()].set("Protoss Assimilator", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 49664, 450, 0, 1, 100, 0, 600, 0, 0, 255, 0, 50, 150, &(UnitSizeTypes::Large), 4, 2, 48, 32, 48, 24, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1);
      unitTypeData[Protoss_Observatory.getID()].set("Protoss Observatory", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Robotics_Facility), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 64000, 250, 0, 1, 50, 100, 450, 0, 0, 255, 0, 175, 525, &(UnitSizeTypes::Large), 3, 2, 44, 16, 44, 28, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Gateway.getID()].set("Protoss Gateway", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Nexus), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 500, 0, 1, 150, 0, 900, 0, 0, 255, 0, 75, 225, &(UnitSizeTypes::Large), 4, 3, 48, 32, 48, 40, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Photon_Cannon.getID()].set("Protoss Photon Cannon", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Forge), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 25600, 100, 0, 0, 150, 0, 750, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 2, 2, 20, 16, 20, 16, 7, 11, &(WeaponTypes::STS_Photon_Cannon), 1, &(WeaponTypes::STA_Photon_Cannon), 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Citadel_of_Adun.getID()].set("Protoss Citadel of Adun", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Cybernetics_Core), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 49664, 450, 0, 1, 150, 100, 900, 0, 0, 255, 0, 200, 600, &(UnitSizeTypes::Large), 3, 2, 24, 24, 40, 24, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Cybernetics_Core.getID()].set("Protoss Cybernetics Core", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Gateway), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 500, 0, 1, 200, 0, 900, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 3, 2, 40, 24, 40, 24, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Templar_Archives.getID()].set("Protoss Templar Archives", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Citadel_of_Adun), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 500, 0, 1, 150, 200, 900, 0, 0, 255, 0, 250, 750, &(UnitSizeTypes::Large), 3, 2, 32, 24, 32, 24, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Forge.getID()].set("Protoss Forge", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Nexus), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 9728, 550, 0, 1, 150, 0, 600, 0, 0, 255, 0, 100, 300, &(UnitSizeTypes::Large), 3, 2, 36, 24, 36, 20, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Stargate.getID()].set("Protoss Stargate", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Cybernetics_Core), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 22528, 600, 0, 1, 150, 150, 1050, 0, 0, 255, 0, 300, 900, &(UnitSizeTypes::Large), 4, 3, 48, 40, 48, 32, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Stasis_Cell_Prison.getID()].set("Special Stasis Cell Prison", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 53248, 0, 0, 1, 150, 0, 1, 0, 0, 255, 0, 0, 5000, &(UnitSizeTypes::Large), 4, 3, 64, 48, 63, 47, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Fleet_Beacon.getID()].set("Protoss Fleet Beacon", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Stargate), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 500, 0, 1, 300, 200, 900, 0, 0, 255, 0, 350, 1050, &(UnitSizeTypes::Large), 3, 2, 40, 32, 47, 24, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Arbiter_Tribunal.getID()].set("Protoss Arbiter Tribunal", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Templar_Archives), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 62464, 500, 0, 1, 200, 150, 900, 0, 0, 255, 0, 450, 1350, &(UnitSizeTypes::Large), 3, 2, 44, 28, 44, 28, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Robotics_Support_Bay.getID()].set("Protoss Robotics Support Bay", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Robotics_Facility), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 49664, 450, 0, 1, 150, 100, 450, 0, 0, 255, 0, 125, 375, &(UnitSizeTypes::Large), 3, 2, 32, 32, 32, 20, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Protoss_Shield_Battery.getID()].set("Protoss Shield Battery", "", Races::Protoss, &(Protoss_Probe), 1, &(Protoss_Probe), 1, &(Protoss_Gateway), 1, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 51200, 200, 200, 1, 100, 0, 450, 0, 0, 255, 0, 50, 150, &(UnitSizeTypes::Large), 3, 2, 32, 16, 32, 16, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Khaydarin_Crystal_Form.getID()].set("Special Khaydarin Crystal Form", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 40960, 0, 0, 1, 250, 0, 1, 0, 0, 255, 0, 0, 2500, &(UnitSizeTypes::Large), 4, 3, 64, 48, 63, 47, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Protoss_Temple.getID()].set("Special Protoss Temple", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 56320, 0, 0, 1, 250, 0, 1, 0, 0, 255, 0, 0, 5000, &(UnitSizeTypes::Large), 7, 3, 112, 48, 111, 47, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_XelNaga_Temple.getID()].set("Special XelNaga Temple", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 34816, 0, 0, 1, 1500, 500, 4800, 0, 0, 255, 0, 0, 5000, &(UnitSizeTypes::Large), 5, 4, 80, 34, 79, 63, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Resource_Mineral_Field.getID()].set("Resource Mineral Field", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 40960, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 10, 10, &(UnitSizeTypes::Independent), 2, 1, 32, 16, 31, 15, 0, 9, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0);
      unitTypeData[Resource_Vespene_Geyser.getID()].set("Resource Vespene Geyser", "", Races::Other, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 40960, 0, 0, 0, 1, 1, 1, 0, 0, 255, 0, 10, 10, &(UnitSizeTypes::Independent), 4, 2, 64, 32, 63, 31, 0, 9, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0);
      unitTypeData[Special_Warp_Gate.getID()].set("Special Warp Gate", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 48128, 0, 0, 1, 600, 200, 2400, 0, 0, 255, 0, 0, 2000, &(UnitSizeTypes::Large), 3, 2, 48, 32, 47, 31, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Psi_Disrupter.getID()].set("Special Psi Disrupter", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 53248, 0, 0, 1, 1000, 400, 4800, 0, 0, 255, 0, 0, 3600, &(UnitSizeTypes::Large), 5, 3, 80, 38, 69, 47, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Power_Generator.getID()].set("Special Power Generator", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 8192, 0, 0, 1, 200, 50, 2400, 0, 0, 255, 0, 0, 600, &(UnitSizeTypes::Large), 4, 3, 56, 28, 63, 43, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Special_Overmind_Cocoon.getID()].set("Special Overmind Cocoon", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 50176, 0, 0, 1, 1000, 500, 2400, 0, 0, 255, 0, 0, 4000, &(UnitSizeTypes::Large), 3, 2, 48, 32, 47, 31, 0, 10, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
      unitTypeData[Spell_Dark_Swarm.getID()].set("Spell Dark Swarm", "", Races::Zerg, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 8192, 0, 0, 0, 250, 200, 2400, 0, 0, 255, 0, 0, 0, &(UnitSizeTypes::Independent), 5, 5, 80, 80, 79, 79, 0, 8, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
      unitTypeData[None.getID()].set("None", "", Races::None, &(None), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, &(UnitSizeTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      unitTypeData[Unknown.getID()].set("Unknown", "", Races::Unknown, &(Unknown), 0, NULL, 0, NULL, 0, NULL, 0, &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(TechTypes::None), &(UpgradeTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, &(UnitSizeTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, &(WeaponTypes::None), 0, &(WeaponTypes::None), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

      for (std::set<UpgradeType>::iterator i=UpgradeTypes::allUpgradeTypes().begin();i!=UpgradeTypes::allUpgradeTypes().end();i++)
      {
        for each (const UnitType* ut in i->whatUses())
        {
          unitTypeData[ut->getID()].upgrades.insert(&(*i));
        }
      }
      unitTypeSet.insert(Terran_Marine);
      unitTypeSet.insert(Terran_Ghost);
      unitTypeSet.insert(Terran_Vulture);
      unitTypeSet.insert(Terran_Goliath);
      unitTypeSet.insert(Terran_Siege_Tank_Tank_Mode);
      unitTypeSet.insert(Terran_SCV);
      unitTypeSet.insert(Terran_Wraith);
      unitTypeSet.insert(Terran_Science_Vessel);
      unitTypeSet.insert(Terran_Dropship);
      unitTypeSet.insert(Terran_Battlecruiser);
      unitTypeSet.insert(Terran_Vulture_Spider_Mine);
      unitTypeSet.insert(Terran_Nuclear_Missile);
      unitTypeSet.insert(Terran_Siege_Tank_Siege_Mode);
      unitTypeSet.insert(Terran_Firebat);
      unitTypeSet.insert(Spell_Scanner_Sweep);
      unitTypeSet.insert(Terran_Medic);
      unitTypeSet.insert(Zerg_Larva);
      unitTypeSet.insert(Zerg_Egg);
      unitTypeSet.insert(Zerg_Zergling);
      unitTypeSet.insert(Zerg_Hydralisk);
      unitTypeSet.insert(Zerg_Ultralisk);
      unitTypeSet.insert(Zerg_Broodling);
      unitTypeSet.insert(Zerg_Drone);
      unitTypeSet.insert(Zerg_Overlord);
      unitTypeSet.insert(Zerg_Mutalisk);
      unitTypeSet.insert(Zerg_Guardian);
      unitTypeSet.insert(Zerg_Queen);
      unitTypeSet.insert(Zerg_Defiler);
      unitTypeSet.insert(Zerg_Scourge);
      unitTypeSet.insert(Zerg_Infested_Terran);
      unitTypeSet.insert(Terran_Valkyrie);
      unitTypeSet.insert(Zerg_Cocoon);
      unitTypeSet.insert(Protoss_Corsair);
      unitTypeSet.insert(Protoss_Dark_Templar);
      unitTypeSet.insert(Zerg_Devourer);
      unitTypeSet.insert(Protoss_Dark_Archon);
      unitTypeSet.insert(Protoss_Probe);
      unitTypeSet.insert(Protoss_Zealot);
      unitTypeSet.insert(Protoss_Dragoon);
      unitTypeSet.insert(Protoss_High_Templar);
      unitTypeSet.insert(Protoss_Archon);
      unitTypeSet.insert(Protoss_Shuttle);
      unitTypeSet.insert(Protoss_Scout);
      unitTypeSet.insert(Protoss_Arbiter);
      unitTypeSet.insert(Protoss_Carrier);
      unitTypeSet.insert(Protoss_Interceptor);
      unitTypeSet.insert(Protoss_Reaver);
      unitTypeSet.insert(Protoss_Observer);
      unitTypeSet.insert(Protoss_Scarab);
      unitTypeSet.insert(Critter_Rhynadon);
      unitTypeSet.insert(Critter_Bengalaas);
      unitTypeSet.insert(Critter_Scantid);
      unitTypeSet.insert(Critter_Kakaru);
      unitTypeSet.insert(Critter_Ragnasaur);
      unitTypeSet.insert(Critter_Ursadon);
      unitTypeSet.insert(Zerg_Lurker_Egg);
      unitTypeSet.insert(Zerg_Lurker);
      unitTypeSet.insert(Spell_Disruption_Web);
      unitTypeSet.insert(Terran_Command_Center);
      unitTypeSet.insert(Terran_Comsat_Station);
      unitTypeSet.insert(Terran_Nuclear_Silo);
      unitTypeSet.insert(Terran_Supply_Depot);
      unitTypeSet.insert(Terran_Refinery);
      unitTypeSet.insert(Terran_Barracks);
      unitTypeSet.insert(Terran_Academy);
      unitTypeSet.insert(Terran_Factory);
      unitTypeSet.insert(Terran_Starport);
      unitTypeSet.insert(Terran_Control_Tower);
      unitTypeSet.insert(Terran_Science_Facility);
      unitTypeSet.insert(Terran_Covert_Ops);
      unitTypeSet.insert(Terran_Physics_Lab);
      unitTypeSet.insert(Terran_Machine_Shop);
      unitTypeSet.insert(Terran_Engineering_Bay);
      unitTypeSet.insert(Terran_Armory);
      unitTypeSet.insert(Terran_Missile_Turret);
      unitTypeSet.insert(Terran_Bunker);
      unitTypeSet.insert(Special_Crashed_Norad_II);
      unitTypeSet.insert(Special_Ion_Cannon);
      unitTypeSet.insert(Zerg_Infested_Command_Center);
      unitTypeSet.insert(Zerg_Hatchery);
      unitTypeSet.insert(Zerg_Lair);
      unitTypeSet.insert(Zerg_Hive);
      unitTypeSet.insert(Zerg_Nydus_Canal);
      unitTypeSet.insert(Zerg_Hydralisk_Den);
      unitTypeSet.insert(Zerg_Defiler_Mound);
      unitTypeSet.insert(Zerg_Greater_Spire);
      unitTypeSet.insert(Zerg_Queens_Nest);
      unitTypeSet.insert(Zerg_Evolution_Chamber);
      unitTypeSet.insert(Zerg_Ultralisk_Cavern);
      unitTypeSet.insert(Zerg_Spire);
      unitTypeSet.insert(Zerg_Spawning_Pool);
      unitTypeSet.insert(Zerg_Creep_Colony);
      unitTypeSet.insert(Zerg_Spore_Colony);
      unitTypeSet.insert(Zerg_Sunken_Colony);
      unitTypeSet.insert(Special_Overmind_With_Shell);
      unitTypeSet.insert(Special_Overmind);
      unitTypeSet.insert(Zerg_Extractor);
      unitTypeSet.insert(Special_Mature_Chrysalis);
      unitTypeSet.insert(Special_Cerebrate);
      unitTypeSet.insert(Special_Cerebrate_Daggoth);
      unitTypeSet.insert(Protoss_Nexus);
      unitTypeSet.insert(Protoss_Robotics_Facility);
      unitTypeSet.insert(Protoss_Pylon);
      unitTypeSet.insert(Protoss_Assimilator);
      unitTypeSet.insert(Protoss_Observatory);
      unitTypeSet.insert(Protoss_Gateway);
      unitTypeSet.insert(Protoss_Photon_Cannon);
      unitTypeSet.insert(Protoss_Citadel_of_Adun);
      unitTypeSet.insert(Protoss_Cybernetics_Core);
      unitTypeSet.insert(Protoss_Templar_Archives);
      unitTypeSet.insert(Protoss_Forge);
      unitTypeSet.insert(Protoss_Stargate);
      unitTypeSet.insert(Special_Stasis_Cell_Prison);
      unitTypeSet.insert(Protoss_Fleet_Beacon);
      unitTypeSet.insert(Protoss_Arbiter_Tribunal);
      unitTypeSet.insert(Protoss_Robotics_Support_Bay);
      unitTypeSet.insert(Protoss_Shield_Battery);
      unitTypeSet.insert(Special_Khaydarin_Crystal_Form);
      unitTypeSet.insert(Special_Protoss_Temple);
      unitTypeSet.insert(Special_XelNaga_Temple);
      unitTypeSet.insert(Resource_Mineral_Field);
      unitTypeSet.insert(Resource_Vespene_Geyser);
      unitTypeSet.insert(Special_Warp_Gate);
      unitTypeSet.insert(Special_Psi_Disrupter);
      unitTypeSet.insert(Special_Power_Generator);
      unitTypeSet.insert(Special_Overmind_Cocoon);
      unitTypeSet.insert(Spell_Dark_Swarm);
      unitTypeSet.insert(None);
      unitTypeSet.insert(Unknown);

      for(std::set<UnitType>::iterator i = unitTypeSet.begin(); i != unitTypeSet.end(); i++)
      {
        unitTypeMap.insert(std::make_pair((*i).getName(), *i));
      }
      initializingUnitType = false;
    }
  }
  UnitType::UnitType()
  {
    this->id = UnitTypes::None.id;
  }
  UnitType::UnitType(int id)
  {
    this->id = id;
    if (!initializingUnitType)
    {
      if (id < 0 || id >= 230 || !unitTypeData[id].valid)
      {
        this->id = UnitTypes::Unknown.id;
      }
    }
  }
  UnitType::UnitType(const UnitType& other)
  {
    this->id = other.id;
  }
  UnitType& UnitType::operator=(const UnitType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool UnitType::operator==(const UnitType& other) const
  {
    return this->id == other.id;
  }
  bool UnitType::operator!=(const UnitType& other) const
  {
    return this->id != other.id;
  }
  bool UnitType::operator<(const UnitType& other) const
  {
    return this->id < other.id;
  }
  int UnitType::getID() const
  {
    return this->id;
  }
  std::string UnitType::getName() const
  {
    return unitTypeData[this->id].name;
  }
  std::string UnitType::getSubLabel() const
  {
    return unitTypeData[this->id].subLabel;
  }
  Race UnitType::getRace() const
  {
    return unitTypeData[this->id].race;
  }

  const std::pair< const UnitType*, int> UnitType::whatBuilds() const
  {
    return unitTypeData[this->id].whatBuilds;
  }
  const std::map< const UnitType*, int >& UnitType::requiredUnits() const
  {
    return unitTypeData[this->id].requiredUnits;
  }
  const TechType* UnitType::requiredTech() const
  {
    return unitTypeData[this->id].requiredTech;
  }
  const std::set<const TechType* >& UnitType::abilities() const
  {
    return unitTypeData[this->id].abilities;
  }
  const std::set<const UpgradeType* >& UnitType::upgrades() const
  {
    return unitTypeData[this->id].upgrades;
  }
  const UpgradeType* UnitType::armorUpgrade() const
  {
    return unitTypeData[this->id].armorUpgrade;
  }

  int UnitType::maxHitPoints() const
  {
    return unitTypeData[this->id].maxHitPoints;
  }
  int UnitType::maxShields() const
  {
    return unitTypeData[this->id].maxShields;
  }
  int UnitType::maxEnergy() const
  {
    return unitTypeData[this->id].maxEnergy;
  }
  int UnitType::armor() const
  {
    return unitTypeData[this->id].armor;
  }

  int UnitType::mineralPrice() const
  {
    return unitTypeData[this->id].mineralPrice;
  }
  int UnitType::gasPrice() const
  {
    return unitTypeData[this->id].gasPrice;
  }
  int UnitType::buildTime() const
  {
    return unitTypeData[this->id].buildTime;
  }

  int UnitType::supplyRequired() const
  {
    return unitTypeData[this->id].supplyRequired;
  }
  int UnitType::supplyProvided() const
  {
    return unitTypeData[this->id].supplyProvided;
  }
  int UnitType::spaceRequired() const
  {
    return unitTypeData[this->id].spaceRequired;
  }
  int UnitType::spaceProvided() const
  {
    return unitTypeData[this->id].spaceProvided;
  }
  int UnitType::buildScore() const
  {
    return unitTypeData[this->id].buildScore;
  }
  int UnitType::destroyScore() const
  {
    return unitTypeData[this->id].destroyScore;
  }

  UnitSizeType UnitType::size() const
  {
    return *unitTypeData[this->id].unitSizeType;
  }
  int UnitType::tileWidth() const
  {
    return unitTypeData[this->id].tileWidth;
  }
  int UnitType::tileHeight() const
  {
    return unitTypeData[this->id].tileHeight;
  }
  int UnitType::dimensionLeft() const
  {
    return unitTypeData[this->id].dimensionLeft;
  }
  int UnitType::dimensionUp() const
  {
    return unitTypeData[this->id].dimensionUp;
  }
  int UnitType::dimensionRight() const
  {
    return unitTypeData[this->id].dimensionRight;
  }
  int UnitType::dimensionDown() const
  {
    return unitTypeData[this->id].dimensionDown;
  }

  int UnitType::seekRange() const
  {
    return unitTypeData[this->id].seekRange;
  }
  int UnitType::sightRange() const
  {
    return unitTypeData[this->id].sightRange;
  }
  const WeaponType* UnitType::groundWeapon() const
  {
    return unitTypeData[this->id].groundWeapon;
  }
  int UnitType::maxGroundHits() const
  {
    return unitTypeData[this->id].maxGroundHits;
  }
  const WeaponType* UnitType::airWeapon() const
  {
    return unitTypeData[this->id].airWeapon;
  }
  int UnitType::maxAirHits() const
  {
    return unitTypeData[this->id].maxAirHits;
  }

  int UnitType::topSpeed() const
  {
    return unitTypeData[this->id].topSpeed;
  }
  int UnitType::acceleration() const
  {
    return unitTypeData[this->id].acceleration;
  }
  int UnitType::haltDistance() const
  {
    return unitTypeData[this->id].haltDistance;
  }
  int UnitType::turnRadius() const
  {
    return unitTypeData[this->id].turnRadius;
  }

  bool UnitType::canProduce() const
  {
    return unitTypeData[this->id].canProduce;
  }
  bool UnitType::canAttack() const
  {
    return unitTypeData[this->id].canAttack;
  }
  bool UnitType::canMove() const
  {
    return unitTypeData[this->id].canMove;
  }
  bool UnitType::isFlyer() const
  {
    return unitTypeData[this->id].isFlyer;
  }
  bool UnitType::regeneratesHP() const
  {
    return unitTypeData[this->id].regeneratesHP;
  }
  bool UnitType::isSpellcaster() const
  {
    return unitTypeData[this->id].isSpellcaster;
  }
  bool UnitType::hasPermanentCloak() const
  {
    return unitTypeData[this->id].hasPermanentCloak;
  }
  bool UnitType::isInvincible() const
  {
    return unitTypeData[this->id].isInvincible;
  }
  bool UnitType::isOrganic() const
  {
    return unitTypeData[this->id].isOrganic;
  }
  bool UnitType::isMechanical() const
  {
    return unitTypeData[this->id].isMechanical;
  }
  bool UnitType::isRobotic() const
  {
    return unitTypeData[this->id].isRobotic;
  }
  bool UnitType::isDetector() const
  {
    return unitTypeData[this->id].isDetector;
  }
  bool UnitType::isResourceContainer() const
  {
    return unitTypeData[this->id].isResourceContainer;
  }
  bool UnitType::isResourceDepot() const
  {
    return unitTypeData[this->id].isResourceDepot;
  }
  bool UnitType::isRefinery() const
  {
    return unitTypeData[this->id].isRefinery;
  }
  bool UnitType::isWorker() const
  {
    return unitTypeData[this->id].isWorker;
  }
  bool UnitType::requiresPsi() const
  {
    return unitTypeData[this->id].requiresPsi;
  }
  bool UnitType::requiresCreep() const
  {
    return unitTypeData[this->id].requiresCreep;
  }
  bool UnitType::isTwoUnitsInOneEgg() const
  {
    return unitTypeData[this->id].isTwoUnitsInOneEgg;
  }
  bool UnitType::isBurrowable() const
  {
    return unitTypeData[this->id].isBurrowable;
  }
  bool UnitType::isCloakable() const
  {
    return unitTypeData[this->id].isCloakable;
  }
  bool UnitType::isBuilding() const
  {
    return unitTypeData[this->id].isBuilding;
  }
  bool UnitType::isAddon() const
  {
    return unitTypeData[this->id].isAddon;
  }
  bool UnitType::isFlyingBuilding() const
  {
    return unitTypeData[this->id].isFlyingBuilding;
  }
  bool UnitType::isNeutral() const
  {
    return unitTypeData[this->id].isNeutral;
  }

  UnitType UnitTypes::getUnitType(std::string& name)
  {
    std::map<std::string, UnitType>::iterator i = unitTypeMap.find(name);
    if (i == unitTypeMap.end()) return UnitTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitType>& UnitTypes::allUnitTypes()
  {
    return unitTypeSet;
  }
}
