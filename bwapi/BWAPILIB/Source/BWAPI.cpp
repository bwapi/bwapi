#include <BWAPI.h>
void BWAPI::BWAPI_init()
{
  BWAPI::Races::init();
  BWAPI::DamageTypes::init();
  BWAPI::Orders::init();
  BWAPI::TechTypes::init();
  BWAPI::PlayerTypes::init();
  BWAPI::UpgradeTypes::init();
  BWAPI::UnitTypes::init();
  BWAPI::Errors::init();
  BWAPI::Colors::init();
}
