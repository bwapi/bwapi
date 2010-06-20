#include "ProtossTest.h"
#include "TrainTest.h"
#include "BuildTest.h"
using namespace std;
using namespace BWAPI;
void ProtossTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->sendText("show me the money");
  Broodwar->setLocalSpeed(0);
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Probe));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Nexus));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Pylon));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Assimilator));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Gateway));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Forge));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Photon_Cannon));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Cybernetics_Core));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Shield_Battery));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Pylon));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Robotics_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Stargate));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Citadel_of_Adun));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Robotics_Support_Bay));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Pylon));

  this->addTestCase(new BuildTest(UnitTypes::Protoss_Fleet_Beacon));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Templar_Archives));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Observatory));
  this->addTestCase(new BuildTest(UnitTypes::Protoss_Arbiter_Tribunal));

  this->addTestCase(new TrainTest(UnitTypes::Protoss_Zealot));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dragoon));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_High_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dark_Templar));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Dark_Templar));

  this->addTestCase(new TrainTest(UnitTypes::Protoss_Shuttle));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Reaver));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Observer));

  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scout));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Carrier));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Arbiter));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Corsair));
  
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Scarab));
  this->addTestCase(new TrainTest(UnitTypes::Protoss_Interceptor));
}