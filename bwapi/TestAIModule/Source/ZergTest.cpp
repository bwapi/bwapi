#include "ZergTest.h"
#include "MorphTest.h"
#include "BuildTest.h"
#include "TrainTest.h"
#include "ResearchTest.h"
#include "UpgradeTest.h"
#include "UseTechTest.h"
#include "CancelMorphTest.h"
#include "CancelResearchTest.h"
#include "CancelUpgradeTest.h"
#include "BurrowTest.h"
#include "RallyTest.h"
#include "MPHSTest.h"
#include "TransportTest.h"
#include "AttackMoveTest.h"
#include "AttackUnitTest.h"
#include "FollowTest.h"
using namespace std;
using namespace BWAPI;
void ZergTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  Broodwar->sendText("show me the money");
  Broodwar->sendText("show me the money");
  Broodwar->setLocalSpeed(0);
  this->addTestCase(new CancelMorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Drone,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Drone,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new RallyTest(UnitTypes::Zerg_Hatchery));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Hatchery));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Hatchery));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Creep_Colony));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Creep_Colony));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Creep_Colony));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Extractor));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Spawning_Pool));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Evolution_Chamber));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Hydralisk_Den));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Sunken_Colony));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Spore_Colony));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Lair));
  this->addTestCase(new RallyTest(UnitTypes::Zerg_Lair));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Lair));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Spire));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Spire));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Queens_Nest));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Hive));
  this->addTestCase(new RallyTest(UnitTypes::Zerg_Hive));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Greater_Spire));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Nydus_Canal));
  //add test case for Build Nydus Canal exit...
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Ultralisk_Cavern));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Defiler_Mound));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Overlord,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new CancelMorphTest(UnitTypes::Zerg_Zergling));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Zergling));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Zergling));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Zergling));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Zergling,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Zergling,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new CancelMorphTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Hydralisk,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Hydralisk,UnitTypes::Zerg_Hatchery));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Mutalisk,UnitTypes::Zerg_Lair));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Mutalisk,UnitTypes::Zerg_Lair));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Scourge));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Scourge));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Scourge,UnitTypes::Zerg_Overlord));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Scourge));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Scourge,UnitTypes::Zerg_Overlord));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Queen));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Queen));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Queen,UnitTypes::Zerg_Hive));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Ultralisk));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Ultralisk));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Ultralisk));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Ultralisk,UnitTypes::Zerg_Hive));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Ultralisk,UnitTypes::Zerg_Hive));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Defiler));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Defiler));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Defiler,UnitTypes::Zerg_Hive));
  this->addTestCase(new CancelMorphTest(UnitTypes::Zerg_Devourer));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Devourer));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Devourer));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Devourer));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Devourer,UnitTypes::Zerg_Overlord));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Devourer,UnitTypes::Zerg_Overlord));
  this->addTestCase(new CancelMorphTest(UnitTypes::Zerg_Guardian));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Guardian));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Guardian));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Guardian));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Guardian,UnitTypes::Zerg_Hive));
  this->addTestCase(new FollowTest(UnitTypes::Zerg_Guardian,UnitTypes::Zerg_Hive));

  this->addTestCase(new UseTechTest(TechTypes::Parasite));
  this->addTestCase(new CancelResearchTest(TechTypes::Lurker_Aspect));
  this->addTestCase(new ResearchTest(TechTypes::Lurker_Aspect));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Lurker));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Lurker));
  this->addTestCase(new ResearchTest(TechTypes::Consume));
  this->addTestCase(new UseTechTest(TechTypes::Consume));
  this->addTestCase(new ResearchTest(TechTypes::Spawn_Broodlings));
  this->addTestCase(new UseTechTest(TechTypes::Spawn_Broodlings));
  this->addTestCase(new UseTechTest(TechTypes::Dark_Swarm));
  this->addTestCase(new ResearchTest(TechTypes::Ensnare));
  this->addTestCase(new UseTechTest(TechTypes::Ensnare));
  this->addTestCase(new ResearchTest(TechTypes::Plague));
  this->addTestCase(new UseTechTest(TechTypes::Plague));
  this->addTestCase(new ResearchTest(TechTypes::Burrowing));
  this->addTestCase(new UseTechTest(TechTypes::Infestation));
  this->addTestCase(new TrainTest(UnitTypes::Zerg_Infested_Terran));
  this->addTestCase(new MPHSTest(UnitTypes::Zerg_Infested_Terran));
  this->addTestCase(new AttackMoveTest(UnitTypes::Zerg_Infested_Terran));
  this->addTestCase(new AttackUnitTest(UnitTypes::Zerg_Infested_Terran,UnitTypes::Zerg_Infested_Command_Center));

  
  this->addTestCase(new BurrowTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BurrowTest(UnitTypes::Zerg_Zergling));
  this->addTestCase(new BurrowTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new BurrowTest(UnitTypes::Zerg_Lurker));
  this->addTestCase(new BurrowTest(UnitTypes::Zerg_Defiler));

  this->addTestCase(new CancelUpgradeTest(UpgradeTypes::Ventral_Sacs));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Ventral_Sacs));
  this->addTestCase(new TransportTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Antennae));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Pneumatized_Carapace));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Metabolic_Boost));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Adrenal_Glands));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Muscular_Augments));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Grooved_Spines));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Gamete_Meiosis));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Anabolic_Synthesis));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Chitinous_Plating));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Metasynaptic_Node));
  for(int l=0;l<3;l++)
  {
    this->addTestCase(new UpgradeTest(UpgradeTypes::Zerg_Melee_Attacks));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Zerg_Missile_Attacks));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Zerg_Carapace));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Zerg_Flyer_Attacks));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Zerg_Flyer_Carapace));
  }

}