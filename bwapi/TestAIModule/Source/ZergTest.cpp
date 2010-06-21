#include "ZergTest.h"
#include "MorphTest.h"
#include "BuildTest.h"
using namespace std;
using namespace BWAPI;
void ZergTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->sendText("show me the money");
  Broodwar->setLocalSpeed(0);
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Hatchery));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Hatchery));
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
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Lair));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Spire));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Spire));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Queens_Nest));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Hive));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Greater_Spire));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Nydus_Canal));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Ultralisk_Cavern));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new BuildTest(UnitTypes::Zerg_Defiler_Mound));

  this->addTestCase(new MorphTest(UnitTypes::Zerg_Drone));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Zergling));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Overlord));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Hydralisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Mutalisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Scourge));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Queen));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Ultralisk));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Defiler));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Devourer));
  this->addTestCase(new MorphTest(UnitTypes::Zerg_Guardian));
}