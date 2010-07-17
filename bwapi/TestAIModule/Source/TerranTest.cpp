#include "TerranTest.h"
#include "TrainTest.h"
#include "BuildTest.h"
#include "ResearchTest.h"
#include "UpgradeTest.h"
#include "UseTechTest.h"
#include "CancelResearchTest.h"
#include "CancelUpgradeTest.h"
#include "SiegeTest.h"
#include "CloakTest.h"
#include "LiftTest.h"
#include "RallyTest.h"
#include "MPHSTest.h"
#include "TransportTest.h"
#include "AttackMoveTest.h"
#include "AttackUnitTest.h"
#include "RepairTest.h"
#include "FollowTest.h"
#include "CancelTrainTest.h"
#include "GatherTest.h"
using namespace std;
using namespace BWAPI;
void TerranTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  Broodwar->sendText("show me the money");
  Broodwar->sendText("show me the money");

  Broodwar->setLocalSpeed(0);
  this->addTestCase(new LiftTest(UnitTypes::Terran_Command_Center));
  this->addTestCase(new TrainTest(UnitTypes::Terran_SCV));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_SCV));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_SCV));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Command_Center));
  this->addTestCase(new RepairTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Command_Center));
  this->addTestCase(new FollowTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Command_Center));
  this->addTestCase(new RallyTest(UnitTypes::Terran_Command_Center));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Command_Center));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Supply_Depot));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Supply_Depot));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Refinery));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Barracks));
  this->addTestCase(new GatherTest());
  this->addTestCase(new RallyTest(UnitTypes::Terran_Barracks));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Engineering_Bay));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Missile_Turret));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Academy));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Marine,UnitTypes::Terran_Medic,UnitTypes::Terran_Firebat));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Bunker));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Factory));
  this->addTestCase(new RallyTest(UnitTypes::Terran_Factory));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Machine_Shop));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Starport));
  this->addTestCase(new RallyTest(UnitTypes::Terran_Starport));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Science_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Science_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Armory));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Vulture,UnitTypes::Terran_Siege_Tank_Tank_Mode,UnitTypes::Terran_Goliath));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Siege_Tank_Tank_Mode,UnitTypes::Terran_Goliath,UnitTypes::Terran_Vulture));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Goliath,UnitTypes::Terran_Vulture,UnitTypes::Terran_Siege_Tank_Tank_Mode));

  this->addTestCase(new BuildTest(UnitTypes::Terran_Comsat_Station));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Control_Tower));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Physics_Lab));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Covert_Ops));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Nuclear_Silo));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Firebat,UnitTypes::Terran_Marine,UnitTypes::Terran_Ghost));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Wraith,UnitTypes::Terran_Dropship,UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Battlecruiser,UnitTypes::Terran_Valkyrie,UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Battlecruiser,UnitTypes::Terran_Valkyrie,UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Valkyrie,UnitTypes::Terran_Science_Vessel,UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new CancelTrainTest(UnitTypes::Terran_Science_Vessel,UnitTypes::Terran_Battlecruiser,UnitTypes::Terran_Valkyrie));

  this->addTestCase(new TrainTest(UnitTypes::Terran_Marine));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Marine));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Marine));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Marine,UnitTypes::Terran_Barracks));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Firebat));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Firebat));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Firebat));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Firebat,UnitTypes::Terran_Barracks));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Ghost));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Ghost));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Ghost));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Ghost,UnitTypes::Terran_Barracks));
  this->addTestCase(new RepairTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Barracks));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Medic));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Medic));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Vulture));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Vulture));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Vulture));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Vulture,UnitTypes::Terran_Factory));
  this->addTestCase(new FollowTest(UnitTypes::Terran_Marine,UnitTypes::Terran_Vulture));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Siege_Tank_Tank_Mode));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Siege_Tank_Tank_Mode));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Siege_Tank_Tank_Mode));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Siege_Tank_Tank_Mode,UnitTypes::Terran_Factory));
  this->addTestCase(new FollowTest(UnitTypes::Terran_Firebat,UnitTypes::Terran_Siege_Tank_Tank_Mode));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Goliath));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Goliath));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Goliath));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Goliath,UnitTypes::Terran_Factory));
  this->addTestCase(new FollowTest(UnitTypes::Terran_Ghost,UnitTypes::Terran_Goliath));
  this->addTestCase(new RepairTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Factory));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Wraith));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Wraith));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Wraith));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Wraith,UnitTypes::Terran_Starport));
  this->addTestCase(new FollowTest(UnitTypes::Terran_Vulture,UnitTypes::Terran_Wraith));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Dropship));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Dropship));
  this->addTestCase(new FollowTest(UnitTypes::Terran_Siege_Tank_Tank_Mode,UnitTypes::Terran_Dropship));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new FollowTest(UnitTypes::Terran_Goliath,UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Battlecruiser,UnitTypes::Terran_Starport));
  this->addTestCase(new RepairTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Starport));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Valkyrie));
  this->addTestCase(new MPHSTest(UnitTypes::Terran_Valkyrie));
  this->addTestCase(new AttackMoveTest(UnitTypes::Terran_Valkyrie));
  this->addTestCase(new AttackUnitTest(UnitTypes::Terran_Valkyrie,UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new RepairTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new FollowTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Marine));
  this->addTestCase(new FollowTest(UnitTypes::Terran_SCV,UnitTypes::Terran_Armory));

  this->addTestCase(new TrainTest(UnitTypes::Terran_Nuclear_Missile));

  this->addTestCase(new TransportTest(UnitTypes::Terran_Dropship));
  this->addTestCase(new TransportTest(UnitTypes::Terran_Bunker));
  this->addTestCase(new CancelResearchTest(TechTypes::Stim_Packs));
  this->addTestCase(new ResearchTest(TechTypes::Stim_Packs));
  this->addTestCase(new ResearchTest(TechTypes::Optical_Flare));
  this->addTestCase(new ResearchTest(TechTypes::Restoration));
  this->addTestCase(new ResearchTest(TechTypes::Spider_Mines));
  this->addTestCase(new ResearchTest(TechTypes::Tank_Siege_Mode));
  this->addTestCase(new ResearchTest(TechTypes::Cloaking_Field));
  this->addTestCase(new ResearchTest(TechTypes::EMP_Shockwave));
  this->addTestCase(new ResearchTest(TechTypes::Irradiate));
  this->addTestCase(new ResearchTest(TechTypes::Lockdown));
  this->addTestCase(new ResearchTest(TechTypes::Personnel_Cloaking));
  this->addTestCase(new ResearchTest(TechTypes::Yamato_Gun));
  this->addTestCase(new SiegeTest());
  this->addTestCase(new CloakTest(UnitTypes::Terran_Ghost));
  this->addTestCase(new CloakTest(UnitTypes::Terran_Wraith));

  this->addTestCase(new UseTechTest(TechTypes::Scanner_Sweep));
  this->addTestCase(new UseTechTest(TechTypes::Stim_Packs));
  this->addTestCase(new UseTechTest(TechTypes::Optical_Flare));
  this->addTestCase(new UseTechTest(TechTypes::Defensive_Matrix));
  this->addTestCase(new UseTechTest(TechTypes::Restoration));
  this->addTestCase(new UseTechTest(TechTypes::Spider_Mines));
  this->addTestCase(new UseTechTest(TechTypes::EMP_Shockwave));
  this->addTestCase(new UseTechTest(TechTypes::Lockdown));
  this->addTestCase(new UseTechTest(TechTypes::Yamato_Gun));
  this->addTestCase(new UseTechTest(TechTypes::Irradiate));
  this->addTestCase(new UseTechTest(TechTypes::Nuclear_Strike));

  this->addTestCase(new UpgradeTest(UpgradeTypes::U_238_Shells));
  this->addTestCase(new CancelUpgradeTest(UpgradeTypes::Caduceus_Reactor));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Caduceus_Reactor));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Ion_Thrusters));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Charon_Boosters));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Apollo_Reactor));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Titan_Reactor));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Ocular_Implants));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Moebius_Reactor));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Colossus_Reactor));
  for(int l=0;l<3;l++)
  {
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Infantry_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Infantry_Armor));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Vehicle_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Ship_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Vehicle_Plating));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Ship_Plating));
  }
}