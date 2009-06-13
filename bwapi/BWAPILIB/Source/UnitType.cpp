#include <string>
#include <map>
#include <set>
#include <BWAPI/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/Race.h>


namespace BWAPI
{
  bool initializingUnitType=true;
  class UnitTypeInternal
  {
  public:
    UnitTypeInternal() {valid=false;}
    void set(const char* name, Race race, const UnitType* whatBuilds, int whatBuildsAmt, const UnitType* requiredUnit1, int requiredUnit1Amt, const UnitType* requiredUnit2, int requiredUnit2Amt, const UnitType* requiredUnit3, int requiredUnit3Amt, const TechType* requiredTech, int suppliesConsumed, int suppliesProduced, int maxHealth, int maxShield, int maxEnergy, int mineralPrice, int gasPrice, int armor, int buildTime, int dimensionLeft, int dimensionUp, int dimensionRight, int dimensionDown, int tileWidth, int tileHeight, int damageFactor, int groundDamage, bool canProduce, bool canAttack, bool canMove, bool isBuilding, bool isAddon, bool isFlyer, bool isNeutral, bool isOrganic, bool isMechanical)
    {
      if (initializingUnitType)
      {
        this->name=name;
        this->race=race;
        this->whatBuilds=std::make_pair(whatBuilds,whatBuildsAmt);
        if (requiredUnit1!=NULL)
        {
          this->requiredUnits.insert(std::make_pair(requiredUnit1,requiredUnit1Amt));
        }
        if (requiredUnit2!=NULL)
        {
          this->requiredUnits.insert(std::make_pair(requiredUnit2,requiredUnit2Amt));
        }
        if (requiredUnit3!=NULL)
        {
          this->requiredUnits.insert(std::make_pair(requiredUnit3,requiredUnit3Amt));
        }
        if (requiredTech==NULL)
        {
          this->requiredTech=&(TechTypes::None);
        }
        else
        {
          this->requiredTech=requiredTech;
        }
        this->suppliesConsumed=suppliesConsumed;
        this->suppliesProduced=suppliesProduced;
        this->maxHealth=maxHealth;
        this->maxShield=maxShield;
        this->maxEnergy=maxEnergy;
        this->mineralPrice=mineralPrice;
        this->gasPrice=gasPrice;
        this->armor=armor;
        this->buildTime=buildTime;
        this->dimensionLeft=dimensionLeft;
        this->dimensionUp=dimensionUp;
        this->dimensionRight=dimensionRight;
        this->dimensionDown=dimensionDown;
        this->tileWidth=tileWidth;
        this->tileHeight=tileHeight;
        this->damageFactor=damageFactor;
        this->groundDamage=groundDamage;
        this->canProduce=canProduce;
        this->canAttack=canAttack;
        this->canMove=canMove;
        this->isBuilding=isBuilding;
        this->isAddon=isAddon;
        this->isFlyer=isFlyer;
        this->isNeutral=isNeutral;
        this->isOrganic=isOrganic;
        this->isMechanical=isMechanical;
        this->valid=true;
      }
    }
    std::string name;
    Race race;
    std::pair< const UnitType*, int > whatBuilds;
    std::map< const UnitType*, int > requiredUnits;
    const TechType* requiredTech;
    int suppliesConsumed;
    int suppliesProduced;
    int maxHealth;
    int maxShield;
    int maxEnergy;
    int mineralPrice;
    int gasPrice;
    int armor;
    int buildTime;
    int dimensionLeft;
    int dimensionUp;
    int dimensionRight;
    int dimensionDown;
    int tileWidth;
    int tileHeight;
    int damageFactor;
    int groundDamage;
    bool canProduce;
    bool canAttack;
    bool canMove;
    bool isBuilding;
    bool isAddon;
    bool isFlyer;
    bool isNeutral;
    bool isOrganic;
    bool isMechanical;
    bool valid;
  };
  UnitTypeInternal unitTypeData[230];
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
    const UnitType Zerg_Extractor(149);
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
    const UnitType Protoss_Fleet_Beacon(169);
    const UnitType Protoss_Arbiter_Tribunal(170);
    const UnitType Protoss_Robotics_Support_Bay(171);
    const UnitType Protoss_Shield_Battery(172);
    const UnitType Resource_Mineral_Field(176);
    const UnitType Resource_Vespene_Geyser(188);
    const UnitType Spell_Dark_Swarm(202);
    const UnitType None(228);
    const UnitType Unknown(229);
    void init()
    {
      unitTypeData[Terran_Marine.getID()].set("Terran Marine",Races::Terran,&(Terran_Barracks),1,&(Terran_Barracks),1,NULL,0,NULL,0,NULL,2,0,10240,100,0,50,0,0,360,8,9,8,10,1,1,1,6,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Terran_Ghost.getID()].set("Terran Ghost",Races::Terran,&(Terran_Barracks),1,&(Terran_Barracks),1,&(Terran_Academy),1,&(Terran_Covert_Ops),1,NULL,2,0,11520,100,0,25,75,0,750,7,10,7,11,1,1,1,10,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Terran_Vulture.getID()].set("Terran Vulture",Races::Terran,&(Terran_Factory),1,&(Terran_Factory),1,NULL,0,NULL,0,NULL,4,0,20480,100,0,75,0,0,450,16,16,15,15,1,1,1,20,false,true,true,false,false,false,false,false,true); 
      unitTypeData[Terran_Goliath.getID()].set("Terran Goliath",Races::Terran,&(Terran_Factory),1,&(Terran_Factory),1,&(Terran_Armory),1,NULL,0,NULL,4,0,32000,100,0,100,50,1,600,16,16,15,15,1,1,0,0,false,true,true,false,false,false,false,false,true); 
      unitTypeData[Terran_Siege_Tank_Tank_Mode.getID()].set("Terran Siege Tank",Races::Terran,&(Terran_Factory),1,&(Terran_Factory),1,&(Terran_Machine_Shop),1,NULL,0,NULL,4,0,38400,100,0,150,100,1,750,16,16,15,15,1,1,0,0,false,true,true,false,false,false,false,false,true); 
      unitTypeData[Terran_SCV.getID()].set("Terran SCV",Races::Terran,&(Terran_Command_Center),1,&(Terran_Command_Center),1,NULL,0,NULL,0,NULL,2,0,15360,100,0,50,0,0,300,11,11,11,11,1,1,1,5,false,true,true,false,false,false,false,true,true); 
      unitTypeData[Terran_Wraith.getID()].set("Terran Wraith",Races::Terran,&(Terran_Starport),1,&(Terran_Starport),1,NULL,0,NULL,0,NULL,4,0,30720,100,0,150,100,0,900,19,15,18,14,2,1,1,8,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Terran_Science_Vessel.getID()].set("Terran Science Vessel",Races::Terran,&(Terran_Starport),1,&(Terran_Starport),1,&(Terran_Control_Tower),1,&(Terran_Science_Facility),1,NULL,4,0,51200,100,0,100,225,1,1200,32,33,32,16,2,2,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Terran_Dropship.getID()].set("Terran Dropship",Races::Terran,&(Terran_Starport),1,&(Terran_Starport),1,&(Terran_Control_Tower),1,NULL,0,NULL,4,0,38400,100,0,100,100,1,750,24,16,24,20,2,2,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Terran_Battlecruiser.getID()].set("Terran Battlecruiser",Races::Terran,&(Terran_Starport),1,&(Terran_Starport),1,&(Terran_Control_Tower),1,&(Terran_Physics_Lab),1,NULL,12,0,62464,100,0,400,300,3,2000,37,29,37,29,3,2,1,25,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Terran_Vulture_Spider_Mine.getID()].set("Vulture Spider Mine",Races::Terran,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,5120,100,0,1,0,0,1,7,7,7,7,1,1,1,125,false,true,true,false,false,false,false,false,false); 
      unitTypeData[Terran_Nuclear_Missile.getID()].set("Nuclear Missile",Races::Terran,&(None),0,NULL,0,NULL,0,NULL,0,NULL,16,0,25600,100,0,200,200,0,1500,7,14,7,14,1,1,0,0,false,true,true,false,false,true,false,false,false); 
      unitTypeData[Terran_Siege_Tank_Siege_Mode.getID()].set("Terran Siege Tank",Races::Terran,&(Terran_Factory),1,&(Terran_Factory),1,&(Terran_Machine_Shop),1,NULL,0,NULL,4,0,38400,100,0,150,100,1,750,16,16,15,15,1,1,0,0,false,false,true,false,false,false,false,false,true); 
      unitTypeData[Terran_Firebat.getID()].set("Terran Firebat",Races::Terran,&(Terran_Barracks),1,&(Terran_Barracks),1,&(Terran_Academy),1,NULL,0,NULL,2,0,12800,100,0,50,25,1,360,11,7,11,14,1,1,1,8,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Spell_Scanner_Sweep.getID()].set("Scanner Sweep",Races::Terran,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,1,0,0,0,0,0,1,13,13,13,17,1,1,0,0,false,true,false,false,false,true,false,false,false); 
      unitTypeData[Terran_Medic.getID()].set("Terran Medic",Races::Terran,&(Terran_Barracks),1,&(Terran_Barracks),1,&(Terran_Academy),1,NULL,0,NULL,2,0,15360,100,0,50,25,1,450,8,9,8,10,1,1,0,0,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Larva.getID()].set("Zerg Larva",Races::Zerg,&(Zerg_Hatchery),1,&(Zerg_Hatchery),1,NULL,0,NULL,0,NULL,0,0,6400,0,0,1,1,10,1,8,8,7,7,1,1,0,0,false,true,false,false,false,false,false,true,false); 
      unitTypeData[Zerg_Egg.getID()].set("Zerg Egg",Races::Zerg,&(Zerg_Larva),1,&(Zerg_Larva),1,NULL,0,NULL,0,NULL,0,0,51200,0,0,1,1,10,1,16,16,15,15,1,1,0,0,false,false,false,false,false,false,false,true,false); 
      unitTypeData[Zerg_Zergling.getID()].set("Zerg Zergling",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Spawning_Pool),1,NULL,0,NULL,1,0,8960,100,0,50,0,0,420,8,4,7,11,1,1,1,5,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Hydralisk.getID()].set("Zerg Hydralisk",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Hydralisk_Den),1,NULL,0,NULL,2,0,20480,100,0,75,25,0,420,10,10,10,12,1,1,1,10,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Ultralisk.getID()].set("Zerg Ultralisk",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Ultralisk_Cavern),1,NULL,0,NULL,8,0,36864,100,0,200,200,1,900,19,16,18,15,2,1,1,20,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Broodling.getID()].set("Zerg Broodling",Races::Zerg,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,7680,100,0,1,1,0,1,9,9,9,9,1,1,1,4,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Drone.getID()].set("Zerg Drone",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,NULL,0,NULL,0,NULL,2,0,10240,100,0,50,0,0,300,11,11,11,11,1,1,1,5,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Overlord.getID()].set("Zerg Overlord",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,NULL,0,NULL,0,NULL,0,16,51200,100,0,100,0,0,600,25,25,24,24,2,2,0,0,false,true,true,false,false,true,false,true,false); 
      unitTypeData[Zerg_Mutalisk.getID()].set("Zerg Mutalisk",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Spire),1,NULL,0,NULL,4,0,30720,100,0,100,100,0,600,22,22,21,21,2,2,1,9,false,true,true,false,false,true,false,true,false); 
      unitTypeData[Zerg_Guardian.getID()].set("Zerg Guardian",Races::Zerg,&(Zerg_Cocoon),1,&(Zerg_Cocoon),1,&(Zerg_Greater_Spire),1,NULL,0,NULL,4,0,38400,100,0,50,100,2,600,22,22,21,21,2,2,1,20,false,true,true,false,false,true,false,true,false); 
      unitTypeData[Zerg_Queen.getID()].set("Zerg Queen",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Queens_Nest),1,NULL,0,NULL,4,0,30720,100,0,100,100,0,750,24,24,23,23,2,2,0,0,false,true,true,false,false,true,false,true,false); 
      unitTypeData[Zerg_Defiler.getID()].set("Zerg Defiler",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Defiler_Mound),1,NULL,0,NULL,4,0,20480,100,0,50,150,1,750,13,12,13,12,1,1,0,0,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Scourge.getID()].set("Zerg Scourge",Races::Zerg,&(Zerg_Egg),1,&(Zerg_Egg),1,&(Zerg_Spire),1,NULL,0,NULL,1,0,6400,100,0,25,75,0,450,12,12,11,11,1,1,0,0,false,true,true,false,false,true,false,true,false); 
      unitTypeData[Zerg_Infested_Terran.getID()].set("Infested Terran",Races::Zerg,&(Zerg_Infested_Command_Center),1,&(Zerg_Infested_Command_Center),1,NULL,0,NULL,0,NULL,2,0,15360,100,0,100,50,0,600,8,9,8,10,1,1,1,500,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Terran_Valkyrie.getID()].set("Terran Valkyrie",Races::Terran,&(Terran_Starport),1,&(Terran_Starport),1,&(Terran_Control_Tower),1,&(Terran_Armory),1,NULL,6,0,51200,100,0,250,125,2,750,24,16,24,20,2,2,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Zerg_Cocoon.getID()].set("Cocoon",Races::Zerg,&(Zerg_Mutalisk),1,&(Zerg_Mutalisk),1,&(Zerg_Greater_Spire),1,NULL,0,NULL,0,0,51200,0,0,1,1,0,1,16,16,15,15,1,1,0,0,false,false,false,false,false,true,false,true,false); 
      unitTypeData[Protoss_Corsair.getID()].set("Protoss Corsair",Races::Protoss,&(Protoss_Stargate),1,&(Protoss_Stargate),1,NULL,0,NULL,0,NULL,4,0,25600,80,0,150,100,1,600,18,16,17,15,2,1,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Dark_Templar.getID()].set("Protoss Dark Templar",Races::Protoss,&(Protoss_Gateway),1,&(Protoss_Gateway),1,&(Protoss_Templar_Archives),1,NULL,0,NULL,4,0,20480,40,0,125,100,1,750,12,6,11,19,1,1,1,40,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Zerg_Devourer.getID()].set("Zerg Devourer",Races::Zerg,&(Zerg_Cocoon),1,&(Zerg_Cocoon),1,&(Zerg_Greater_Spire),1,NULL,0,NULL,4,0,64000,100,0,150,50,2,600,22,22,21,21,2,2,0,0,false,true,true,false,false,true,false,true,false); 
      unitTypeData[Protoss_Dark_Archon.getID()].set("Protoss Dark Archon",Races::Protoss,&(Protoss_Dark_Templar),2,&(Protoss_Dark_Templar),2,NULL,0,NULL,0,NULL,8,0,6400,200,0,0,0,1,300,16,16,15,15,1,1,0,0,false,true,true,false,false,false,false,false,false); 
      unitTypeData[Protoss_Probe.getID()].set("Protoss Probe",Races::Protoss,&(Protoss_Nexus),1,&(Protoss_Nexus),1,NULL,0,NULL,0,NULL,2,0,5120,20,0,50,0,0,300,11,11,11,11,1,1,1,5,false,true,true,false,false,false,false,false,true); 
      unitTypeData[Protoss_Zealot.getID()].set("Protoss Zealot",Races::Protoss,&(Protoss_Gateway),1,&(Protoss_Gateway),1,NULL,0,NULL,0,NULL,4,0,25600,60,0,100,0,1,600,11,5,11,13,1,1,1,8,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Protoss_Dragoon.getID()].set("Protoss Dragoon",Races::Protoss,&(Protoss_Gateway),1,&(Protoss_Gateway),1,&(Protoss_Cybernetics_Core),1,NULL,0,NULL,4,0,25600,80,0,125,50,1,750,15,15,16,16,1,1,1,20,false,true,true,false,false,false,false,false,true); 
      unitTypeData[Protoss_High_Templar.getID()].set("Protoss High Templar",Races::Protoss,&(Protoss_Gateway),1,&(Protoss_Gateway),1,&(Protoss_Templar_Archives),1,NULL,0,NULL,4,0,10240,40,0,50,150,0,750,12,10,11,13,1,1,0,0,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Protoss_Archon.getID()].set("Protoss Archon",Races::Protoss,&(Protoss_High_Templar),2,&(Protoss_High_Templar),2,NULL,0,NULL,0,NULL,8,0,2560,350,0,0,0,0,300,16,16,15,15,1,1,1,30,false,true,true,false,false,false,false,false,false); 
      unitTypeData[Protoss_Shuttle.getID()].set("Protoss Shuttle",Races::Protoss,&(Protoss_Robotics_Facility),1,&(Protoss_Robotics_Facility),1,NULL,0,NULL,0,NULL,4,0,20480,60,0,200,0,1,900,20,16,19,15,2,1,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Scout.getID()].set("Protoss Scout",Races::Protoss,&(Protoss_Stargate),1,&(Protoss_Stargate),1,NULL,0,NULL,0,NULL,6,0,38400,100,0,275,125,0,1200,18,16,17,15,2,1,1,8,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Arbiter.getID()].set("Protoss Arbiter",Races::Protoss,&(Protoss_Stargate),1,&(Protoss_Stargate),1,&(Protoss_Arbiter_Tribunal),1,NULL,0,NULL,8,0,51200,150,0,100,350,1,2400,22,22,21,21,2,2,1,10,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Carrier.getID()].set("Protoss Carrier",Races::Protoss,&(Protoss_Stargate),1,&(Protoss_Stargate),1,&(Protoss_Fleet_Beacon),1,NULL,0,NULL,12,0,11264,150,0,350,250,4,2100,32,32,31,31,2,2,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Interceptor.getID()].set("Protoss Interceptor",Races::Protoss,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,10240,40,0,25,0,0,300,8,8,7,7,1,1,1,6,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Reaver.getID()].set("Protoss Reaver",Races::Protoss,&(Protoss_Robotics_Facility),1,&(Protoss_Robotics_Facility),1,&(Protoss_Robotics_Support_Bay),1,NULL,0,NULL,8,0,25600,80,0,200,100,0,1050,16,16,15,15,1,1,0,0,false,true,true,false,false,false,false,false,true); 
      unitTypeData[Protoss_Observer.getID()].set("Protoss Observer",Races::Protoss,&(Protoss_Robotics_Facility),1,&(Protoss_Robotics_Facility),1,&(Protoss_Observatory),1,NULL,0,NULL,2,0,10240,20,0,25,75,0,600,16,16,15,15,1,1,0,0,false,true,true,false,false,true,false,false,true); 
      unitTypeData[Protoss_Scarab.getID()].set("Protoss Scarab",Races::Protoss,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,5120,10,0,15,0,0,105,2,2,2,2,1,1,1,100,false,true,false,false,false,false,false,false,true); 
      unitTypeData[Critter_Rhynadon.getID()].set("Rhynadon",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,15360,100,0,1,1,0,1,16,16,15,15,1,1,0,0,false,true,true,false,false,false,true,true,false); 
      unitTypeData[Critter_Bengalaas.getID()].set("Bengalaas",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,15360,100,0,1,1,0,1,16,16,15,15,1,1,0,0,false,true,true,false,false,false,true,true,false); 
      unitTypeData[Critter_Scantid.getID()].set("Scantid",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,15360,100,0,1,1,0,1,16,16,15,15,1,1,0,0,false,true,true,false,false,false,true,true,false); 
      unitTypeData[Critter_Kakaru.getID()].set("Kakaru",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,15360,100,0,1,1,0,1,16,16,15,15,1,1,0,0,false,true,true,false,false,true,true,true,false); 
      unitTypeData[Critter_Ragnasaur.getID()].set("Ragnasaur",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,15360,100,0,1,1,0,1,16,16,15,15,1,1,0,0,false,true,true,false,false,false,true,true,false); 
      unitTypeData[Critter_Ursadon.getID()].set("Ursadon",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,15360,100,0,1,1,0,1,16,16,15,15,1,1,0,0,false,true,true,false,false,false,true,true,false); 
      unitTypeData[Zerg_Lurker_Egg.getID()].set("Zerg Lurker Egg",Races::Zerg,&(Zerg_Lurker_Egg),1,&(Zerg_Hydralisk),1,NULL,0,NULL,0,&(TechTypes::Lurker_Aspect),0,0,51200,0,0,1,1,10,1,16,16,15,15,1,1,0,0,false,false,false,false,false,false,false,true,false); 
      unitTypeData[Zerg_Lurker.getID()].set("Zerg Lurker",Races::Zerg,&(Zerg_Lurker_Egg),1,&(Zerg_Lurker_Egg),1,NULL,0,NULL,0,&(TechTypes::Lurker_Aspect),4,0,32000,100,0,50,100,1,600,15,15,16,16,1,1,1,20,false,true,true,false,false,false,false,true,false); 
      unitTypeData[Spell_Disruption_Web.getID()].set("Disruption Field",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,8192,100,0,250,250,0,2400,60,40,59,39,4,3,0,0,false,false,false,false,false,false,true,false,false); 
      unitTypeData[Terran_Command_Center.getID()].set("Terran Command Center",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,NULL,0,NULL,0,NULL,0,20,56320,100,0,400,0,1,1800,58,41,58,41,4,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Comsat_Station.getID()].set("Terran Comsat Station",Races::Terran,&(Terran_Command_Center),1,&(Terran_Command_Center),1,&(Terran_Academy),1,NULL,0,NULL,0,0,62464,100,0,50,50,1,600,37,16,31,25,3,2,0,0,false,false,false,true,true,false,false,false,true); 
      unitTypeData[Terran_Nuclear_Silo.getID()].set("Terran Nuclear Silo",Races::Terran,&(Terran_Command_Center),1,&(Terran_Command_Center),1,&(Terran_Science_Facility),1,&(Terran_Covert_Ops),1,NULL,0,0,22528,100,0,100,100,1,1200,37,16,31,25,3,2,0,0,false,false,false,true,true,false,false,false,true); 
      unitTypeData[Terran_Supply_Depot.getID()].set("Terran Supply Depot",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,NULL,0,NULL,0,NULL,0,16,62464,100,0,100,0,1,600,38,22,38,26,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Refinery.getID()].set("Terran Refinery",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,NULL,0,NULL,0,NULL,0,0,60928,100,0,100,0,1,600,56,32,56,31,4,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Barracks.getID()].set("Terran Barracks",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Command_Center),1,NULL,0,NULL,0,0,59392,100,0,150,0,1,1200,48,40,56,32,4,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Academy.getID()].set("Terran Academy",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Barracks),1,NULL,0,NULL,0,0,22528,100,0,150,0,1,1200,40,32,44,24,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Factory.getID()].set("Terran Factory",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Barracks),1,NULL,0,NULL,0,0,57856,100,0,200,100,1,1200,56,40,56,40,4,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Starport.getID()].set("Terran Starport",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Factory),1,NULL,0,NULL,0,0,5120,100,0,150,100,1,1050,48,40,48,38,3,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Control_Tower.getID()].set("Terran Control Tower",Races::Terran,&(Terran_Starport),1,&(Terran_Starport),1,NULL,0,NULL,0,NULL,0,0,62464,100,0,50,50,1,600,47,24,28,22,3,2,0,0,false,false,false,true,true,false,false,false,true); 
      unitTypeData[Terran_Science_Facility.getID()].set("Terran Science Facility",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Starport),1,NULL,0,NULL,0,0,20992,100,0,100,150,1,900,48,38,48,38,3,3,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Covert_Ops.getID()].set("Terran Covert Ops",Races::Terran,&(Terran_Science_Facility),1,&(Terran_Science_Facility),1,NULL,0,NULL,0,NULL,0,0,60928,100,0,50,50,1,600,47,24,28,22,3,2,0,0,false,false,false,true,true,false,false,false,true); 
      unitTypeData[Terran_Physics_Lab.getID()].set("Terran Physics Lab",Races::Terran,&(Terran_Science_Facility),1,&(Terran_Science_Facility),1,NULL,0,NULL,0,NULL,0,0,22528,100,0,50,50,1,600,47,24,28,22,3,2,0,0,false,false,false,true,true,false,false,false,true); 
      unitTypeData[Terran_Machine_Shop.getID()].set("Terran Machine Shop",Races::Terran,&(Terran_Factory),1,&(Terran_Factory),1,NULL,0,NULL,0,NULL,0,0,60928,100,0,50,50,1,600,39,24,31,24,3,2,0,0,false,false,false,true,true,false,false,false,true); 
      unitTypeData[Terran_Engineering_Bay.getID()].set("Terran Engineering Bay",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Command_Center),1,NULL,0,NULL,0,0,20992,100,0,125,0,1,900,48,32,48,28,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Armory.getID()].set("Terran Armory",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Factory),1,NULL,0,NULL,0,0,60928,100,0,100,50,1,1200,48,32,47,22,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Missile_Turret.getID()].set("Terran Missile Turret",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Engineering_Bay),1,NULL,0,NULL,0,0,51200,100,0,75,0,0,450,16,32,16,16,1,2,0,0,false,true,false,true,false,false,false,false,true); 
      unitTypeData[Terran_Bunker.getID()].set("Terran Bunker",Races::Terran,&(Terran_SCV),1,&(Terran_SCV),1,&(Terran_Barracks),1,NULL,0,NULL,0,0,24064,100,0,100,0,1,450,32,24,32,16,2,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Zerg_Infested_Command_Center.getID()].set("Infested Command Center",Races::Zerg,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,56320,100,0,1,1,1,1800,58,41,58,41,4,3,0,0,true,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Hatchery.getID()].set("Zerg Hatchery",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,NULL,0,NULL,0,NULL,0,2,57856,100,0,300,0,1,1800,49,32,49,32,4,2,0,0,true,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Lair.getID()].set("Zerg Lair",Races::Zerg,&(Zerg_Hatchery),1,&(Zerg_Hatchery),1,&(Zerg_Spawning_Pool),1,NULL,0,NULL,0,2,2048,100,0,150,100,1,1500,49,32,49,32,4,2,0,0,true,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Hive.getID()].set("Zerg Hive",Races::Zerg,&(Zerg_Lair),1,&(Zerg_Lair),1,&(Zerg_Queens_Nest),1,NULL,0,NULL,0,2,50176,100,0,200,150,1,1800,49,32,49,32,4,2,0,0,true,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Nydus_Canal.getID()].set("Zerg Nydus Canal",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Hive),1,NULL,0,NULL,0,0,64000,100,0,150,0,1,600,32,32,31,31,2,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Hydralisk_Den.getID()].set("Zerg Hydralisk Den",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Spawning_Pool),1,NULL,0,NULL,0,0,20992,100,0,100,50,1,600,40,32,40,24,3,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Defiler_Mound.getID()].set("Zerg Defiler Mound",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Hive),1,NULL,0,NULL,0,0,20992,100,0,100,100,1,900,48,32,48,4,3,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Greater_Spire.getID()].set("Zerg Greater Spire",Races::Zerg,&(Zerg_Spire),1,&(Zerg_Hive),1,&(Zerg_Spire),1,NULL,0,NULL,0,0,59392,100,0,100,150,1,1800,28,32,28,24,2,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Queens_Nest.getID()].set("Zerg Queen's Nest",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Lair),1,NULL,0,NULL,0,0,20992,100,0,150,100,1,900,38,28,32,28,3,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Evolution_Chamber.getID()].set("Zerg Evolution Chamber",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Hatchery),1,NULL,0,NULL,0,0,60928,100,0,75,0,1,600,44,32,32,20,3,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Ultralisk_Cavern.getID()].set("Zerg Ultralisk Cavern",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Hive),1,NULL,0,NULL,0,0,22528,100,0,150,200,1,1200,40,32,32,31,3,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Spire.getID()].set("Zerg Spire",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Lair),1,NULL,0,NULL,0,0,22528,100,0,200,150,1,1800,28,32,28,24,2,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Spawning_Pool.getID()].set("Zerg Spawning Pool",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,&(Zerg_Hatchery),1,NULL,0,NULL,0,0,60928,100,0,200,0,1,1200,36,28,40,18,3,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Creep_Colony.getID()].set("Zerg Creep Colony",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,NULL,0,NULL,0,NULL,0,0,36864,100,0,75,0,0,300,24,24,23,23,2,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Spore_Colony.getID()].set("Zerg Spore Colony",Races::Zerg,&(Zerg_Creep_Colony),1,&(Zerg_Evolution_Chamber),1,&(Zerg_Creep_Colony),1,NULL,0,NULL,0,0,36864,100,0,50,0,0,300,24,24,23,23,2,2,0,0,false,true,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Sunken_Colony.getID()].set("Zerg Sunken Colony",Races::Zerg,&(Zerg_Creep_Colony),1,&(Zerg_Spawning_Pool),1,&(Zerg_Creep_Colony),1,NULL,0,NULL,0,0,11264,100,0,50,0,2,300,24,24,23,23,2,2,1,40,false,true,false,true,false,false,false,true,false); 
      unitTypeData[Zerg_Extractor.getID()].set("Zerg Extractor",Races::Zerg,&(Zerg_Drone),1,&(Zerg_Drone),1,NULL,0,NULL,0,NULL,0,0,60928,100,0,50,0,1,600,64,32,63,31,4,2,0,0,false,false,false,true,false,false,false,true,false); 
      unitTypeData[Protoss_Nexus.getID()].set("Protoss Nexus",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,NULL,0,NULL,0,NULL,0,18,60928,750,0,400,0,1,1800,56,39,56,39,4,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Robotics_Facility.getID()].set("Protoss Robotics Facility",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Cybernetics_Core),1,NULL,0,NULL,0,0,62464,500,0,200,200,1,1200,36,16,40,20,3,2,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Pylon.getID()].set("Protoss Pylon",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,NULL,0,NULL,0,NULL,0,16,11264,300,0,100,0,0,450,16,12,16,20,1,1,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Assimilator.getID()].set("Protoss Assimilator",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,NULL,0,NULL,0,NULL,0,0,49664,450,0,100,0,1,600,48,32,48,24,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Observatory.getID()].set("Protoss Observatory",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Robotics_Facility),1,NULL,0,NULL,0,0,64000,250,0,50,100,1,450,44,16,44,28,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Gateway.getID()].set("Protoss Gateway",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Nexus),1,NULL,0,NULL,0,0,62464,500,0,150,0,1,900,48,32,48,40,3,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Photon_Cannon.getID()].set("Protoss Photon Cannon",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Forge),1,NULL,0,NULL,0,0,25600,100,0,150,0,0,750,20,16,20,16,2,1,1,20,false,true,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Citadel_of_Adun.getID()].set("Protoss Citadel of Adun",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Cybernetics_Core),1,NULL,0,NULL,0,0,49664,450,0,150,100,1,900,24,24,40,24,2,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Cybernetics_Core.getID()].set("Protoss Cybernetics Core",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Gateway),1,NULL,0,NULL,0,0,62464,500,0,200,0,1,900,40,24,40,24,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Templar_Archives.getID()].set("Protoss Templar Archives",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Citadel_of_Adun),1,NULL,0,NULL,0,0,62464,500,0,150,200,1,900,32,24,32,24,2,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Forge.getID()].set("Protoss Forge",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Nexus),1,NULL,0,NULL,0,0,9728,550,0,150,0,1,600,36,24,36,20,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Stargate.getID()].set("Protoss Stargate",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Cybernetics_Core),1,NULL,0,NULL,0,0,22528,600,0,150,150,1,1050,48,40,48,32,3,3,0,0,true,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Fleet_Beacon.getID()].set("Protoss Fleet Beacon",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Stargate),1,NULL,0,NULL,0,0,62464,500,0,300,200,1,900,40,32,47,24,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Arbiter_Tribunal.getID()].set("Protoss Arbiter Tribunal",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Templar_Archives),1,NULL,0,NULL,0,0,62464,500,0,200,150,1,900,44,28,44,28,3,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Robotics_Support_Bay.getID()].set("Protoss Robotics Support Bay",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Robotics_Facility),1,NULL,0,NULL,0,0,49664,450,0,150,100,1,450,32,32,32,20,2,2,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Protoss_Shield_Battery.getID()].set("Protoss Shield Battery",Races::Protoss,&(Protoss_Probe),1,&(Protoss_Probe),1,&(Protoss_Gateway),1,NULL,0,NULL,0,0,51200,200,0,100,0,1,450,32,16,32,16,2,1,0,0,false,false,false,true,false,false,false,false,true); 
      unitTypeData[Resource_Mineral_Field.getID()].set("Mineral Field",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,40960,100,0,1,1,0,1,32,16,31,15,2,1,0,0,false,false,false,true,false,false,true,false,false); 
      unitTypeData[Resource_Vespene_Geyser.getID()].set("Vespene Geyser",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,40960,100,0,1,1,0,1,64,32,63,31,4,2,0,0,false,false,false,true,false,false,true,false,false); 
      unitTypeData[Spell_Dark_Swarm.getID()].set("Dark Swarm",Races::Other,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,8192,100,0,250,200,0,2400,80,80,79,79,5,5,0,0,false,false,false,false,false,false,true,false,false); 
      unitTypeData[None.getID()].set("None",Races::None,&(None),0,NULL,0,NULL,0,NULL,0,NULL,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,false,false,false,false,false,false,false,false,false); 
      unitTypeData[Unknown.getID()].set("Unknown",Races::Unknown,&(Unknown),0,NULL,0,NULL,0,NULL,0,NULL,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,false,false,false,false,false,false,false,false,false); 
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
      unitTypeSet.insert(Zerg_Extractor);
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
      unitTypeSet.insert(Protoss_Fleet_Beacon);
      unitTypeSet.insert(Protoss_Arbiter_Tribunal);
      unitTypeSet.insert(Protoss_Robotics_Support_Bay);
      unitTypeSet.insert(Protoss_Shield_Battery);
      unitTypeSet.insert(Resource_Mineral_Field);
      unitTypeSet.insert(Resource_Vespene_Geyser);
      unitTypeSet.insert(Spell_Dark_Swarm);
      unitTypeSet.insert(None);
      unitTypeSet.insert(Unknown);

      for(std::set<UnitType>::iterator i=unitTypeSet.begin();i!=unitTypeSet.end();i++)
      {
        unitTypeMap.insert(std::make_pair((*i).getName(),*i));
      }
      initializingUnitType=false;
    }
  }
  UnitType::UnitType()
  {
    this->id=UnitTypes::None.id;
  }
  UnitType::UnitType(int id)
  {
    this->id=id;
    if (!initializingUnitType)
    {
      if (id<0 || id>=230 || !unitTypeData[id].valid)
      {
        this->id=UnitTypes::Unknown.id;
      }
    }
  }
  UnitType::UnitType(const UnitType &other)
  {
    this->id=other.id;
  }
  UnitType& UnitType::operator=(const UnitType &other)
  {
    this->id=other.id;
    return *this;
  }
  bool UnitType::operator==(const UnitType &other) const
  {
    return this->id==other.id;
  }
  bool UnitType::operator!=(const UnitType &other) const
  {
    return this->id!=other.id;
  }
  bool UnitType::operator<(const UnitType &other) const
  {
    return this->id<other.id;
  }
  int UnitType::getID() const
  {
    return this->id;
  }
  std::string UnitType::getName() const
  {
    return unitTypeData[this->id].name;
  }
  Race UnitType::getRace() const
  {
    return unitTypeData[this->id].race;
  }
  const std::pair< const UnitType*,int> UnitType::whatBuilds() const
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
  int UnitType::suppliesConsumed() const
  {
    return unitTypeData[this->id].suppliesConsumed;
  }
  int UnitType::suppliesProduced() const
  {
    return unitTypeData[this->id].suppliesProduced;
  }
  int UnitType::maxHealth() const
  {
    return unitTypeData[this->id].maxHealth;
  }
  int UnitType::maxShield() const
  {
    return unitTypeData[this->id].maxShield;
  }
  int UnitType::maxEnergy() const
  {
    return unitTypeData[this->id].maxEnergy;
  }
  int UnitType::mineralPrice() const
  {
    return unitTypeData[this->id].mineralPrice;
  }
  int UnitType::gasPrice() const
  {
    return unitTypeData[this->id].gasPrice;
  }
  int UnitType::armor() const
  {
    return unitTypeData[this->id].armor;
  }
  int UnitType::buildTime() const
  {
    return unitTypeData[this->id].buildTime;
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
  int UnitType::tileWidth() const
  {
    return unitTypeData[this->id].tileWidth;
  }
  int UnitType::tileHeight() const
  {
    return unitTypeData[this->id].tileHeight;
  }
  int UnitType::damageFactor() const
  {
    return unitTypeData[this->id].damageFactor;
  }
  int UnitType::groundDamage() const
  {
    return unitTypeData[this->id].groundDamage;
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
  bool UnitType::isWorker() const
  {
    return (this->id==UnitTypes::Terran_SCV.id || this->id==UnitTypes::Zerg_Drone.id || this->id==UnitTypes::Protoss_Probe.id);
  }
  bool UnitType::isBuilding() const
  {
    return unitTypeData[this->id].isBuilding;
  }
  bool UnitType::isAddon() const
  {
    return unitTypeData[this->id].isAddon;
  }
  bool UnitType::isFlyer() const
  {
    return unitTypeData[this->id].isFlyer;
  }
  bool UnitType::isNeutral() const
  {
    return unitTypeData[this->id].isNeutral;
  }
  bool UnitType::isOrganic() const
  {
    return unitTypeData[this->id].isOrganic;
  }
  bool UnitType::isMechanical() const
  {
    return unitTypeData[this->id].isMechanical;
  }
  UnitType UnitTypes::getUnitType(std::string &name)
  {
    std::map<std::string, UnitType>::iterator i=unitTypeMap.find(name);
  //  if (i==unitTypeMap.end()) return UnitTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitType>& UnitTypes::allUnitTypes()
  {
    return unitTypeSet;
  }
}