#include "ProtossTest.h"
#include "TrainTest.h"
#include "BuildTest.h"
#include "ResearchTest.h"
#include "UpgradeTest.h"
#include "UseTechTest.h"
#include "CancelResearchTest.h"
#include "CancelUpgradeTest.h"
#include "RallyTest.h"
#include "MPHSTest.h"
#include "TransportTest.h"
#include "AttackMoveTest.h"
#include "AttackUnitTest.h"
using namespace std;
using namespace BWAPI;
void ProtossTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  Broodwar->sendText("show me the money");
  Broodwar->sendText("show me the money");
  Broodwar->setLocalSpeed(0);
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Probe));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Probe));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Probe));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Probe,UnitTypes::Protoss_Nexus));
  this->addTestCase(new RallyTest(UnitTypes::Protoss_Nexus));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Nexus));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Pylon));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Assimilator));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Gateway));
  this->addTestCase(new RallyTest(UnitTypes::Protoss_Gateway));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Forge));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Photon_Cannon));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Cybernetics_Core));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Shield_Battery));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Pylon));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Robotics_Facility));
  this->addTestCase(new RallyTest(UnitTypes::Protoss_Robotics_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Stargate));
  this->addTestCase(new RallyTest(UnitTypes::Protoss_Stargate));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Citadel_of_Adun));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Robotics_Support_Bay));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Pylon));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Fleet_Beacon));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Templar_Archives));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Observatory));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Arbiter_Tribunal));

  this->addTestCase(new TrainTest(UnitTypes::Protoss_Zealot));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Zealot));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Zealot));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Zealot,UnitTypes::Protoss_Gateway));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dragoon));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Dragoon));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Dragoon));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Dragoon,UnitTypes::Protoss_Gateway));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Dark_Templar,UnitTypes::Protoss_Gateway));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new UseTechTest(TechTypes::Archon_Warp));
  this->addTestCase(new UseTechTest(TechTypes::Dark_Archon_Meld));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Archon));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Archon));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Archon,UnitTypes::Protoss_Gateway));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Dark_Archon));

  this->addTestCase(new TrainTest(UnitTypes::Protoss_Shuttle));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Shuttle));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Probe));
  this->addTestCase(new TransportTest(UnitTypes::Protoss_Shuttle));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Reaver));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scarab));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scarab));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scarab));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scarab));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Reaver));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Reaver));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Reaver,UnitTypes::Protoss_Robotics_Facility));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Observer));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Observer));

  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scout));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Scout));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Scout));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Scout,UnitTypes::Protoss_Stargate));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Carrier));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Interceptor));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Interceptor));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Interceptor));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Interceptor));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Carrier));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Carrier));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Carrier,UnitTypes::Protoss_Stargate));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Arbiter));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Arbiter));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Arbiter));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Arbiter,UnitTypes::Protoss_Stargate));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Corsair));
  this->addTestCase(new MPHSTest(UnitTypes::Protoss_Corsair));
  this->addTestCase(new AttackMoveTest(UnitTypes::Protoss_Corsair));
  this->addTestCase(new AttackUnitTest(UnitTypes::Protoss_Corsair,UnitTypes::Protoss_Carrier));
  

  this->addTestCase(new CancelResearchTest(TechTypes::Disruption_Web));
  this->addTestCase(new ResearchTest(TechTypes::Disruption_Web));
  this->addTestCase(new ResearchTest(TechTypes::Psionic_Storm));
  this->addTestCase(new ResearchTest(TechTypes::Hallucination));
  this->addTestCase(new ResearchTest(TechTypes::Mind_Control));
  this->addTestCase(new ResearchTest(TechTypes::Maelstrom));
  this->addTestCase(new ResearchTest(TechTypes::Recall));
  this->addTestCase(new ResearchTest(TechTypes::Stasis_Field));

  this->addTestCase(new UseTechTest(TechTypes::Psionic_Storm));
  this->addTestCase(new UseTechTest(TechTypes::Mind_Control));
  this->addTestCase(new UseTechTest(TechTypes::Stasis_Field));
  this->addTestCase(new UseTechTest(TechTypes::Disruption_Web));
  this->addTestCase(new UseTechTest(TechTypes::Hallucination));
  this->addTestCase(new UseTechTest(TechTypes::Maelstrom));
  this->addTestCase(new UseTechTest(TechTypes::Recall));
  this->addTestCase(new UseTechTest(TechTypes::Feedback));

  this->addTestCase(new CancelUpgradeTest(UpgradeTypes::Singularity_Charge));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Singularity_Charge));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Leg_Enhancements));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Scarab_Damage));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Reaver_Capacity));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Gravitic_Drive));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Apial_Sensors));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Gravitic_Thrusters));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Carrier_Capacity));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Argus_Jewel));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Khaydarin_Amulet));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Argus_Talisman));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Gravitic_Boosters));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Sensor_Array));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Khaydarin_Core));
  for(int l=0;l<3;l++)
  {
    this->addTestCase(new UpgradeTest(UpgradeTypes::Protoss_Ground_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Protoss_Ground_Armor));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Protoss_Plasma_Shields));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Protoss_Air_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Protoss_Air_Armor));
  }

}