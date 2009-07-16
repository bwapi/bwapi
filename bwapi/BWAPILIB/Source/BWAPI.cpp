#include <BWAPI.h>
void BWAPI::BWAPI_init()
{
  BWAPI::Races::init();
  BWAPI::DamageTypes::init();
  BWAPI::ExplosionTypes::init();
  BWAPI::Orders::init();
  BWAPI::TechTypes::init();
  BWAPI::PlayerTypes::init();
  BWAPI::UpgradeTypes::init();
  BWAPI::WeaponTypes::init();
  BWAPI::UnitTypes::init();
  BWAPI::Errors::init();
  BWAPI::Colors::init();
}
