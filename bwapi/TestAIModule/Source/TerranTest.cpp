#include "TerranTest.h"
#include "TrainTest.h"
#include "BuildTest.h"
using namespace std;
using namespace BWAPI;
void TerranTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->sendText("show me the money");
  Broodwar->setLocalSpeed(0);
  this->addTestCase(new TrainTest(UnitTypes::Terran_SCV));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Command_Center));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Supply_Depot));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Supply_Depot));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Refinery));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Barracks));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Engineering_Bay));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Missile_Turret));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Academy));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Bunker));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Factory));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Starport));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Science_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Science_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Armory));

  this->addTestCase(new BuildTest(UnitTypes::Terran_Comsat_Station));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Machine_Shop));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Control_Tower));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Physics_Lab));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Covert_Ops));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Nuclear_Silo));

  this->addTestCase(new TrainTest(UnitTypes::Terran_Marine));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Firebat));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Ghost));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Medic));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Vulture));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Siege_Tank_Tank_Mode));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Goliath));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Wraith));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Dropship));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Valkyrie));

  this->addTestCase(new TrainTest(UnitTypes::Terran_Nuclear_Missile));
}