#include <BWAPI.h>
#include "../../svnrev.h"

#include "../../Debug.h"

const char *__revisionInfo = "XBWAPIXREVISIONXSTAMPX" SVN_REV_STR;
const char *__buildInfo = "XBWAPIXBUILDXSTAMPX" BUILD_STR;

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
  BWAPI::UnitSizeTypes::init();
  BWAPI::UnitCommandTypes::init();
  BWAPI::UnitTypes::init();
  BWAPI::BulletTypes::init();
  BWAPI::Errors::init();
  BWAPI::Colors::init();
  BWAPI::GameTypes::init();
}
int BWAPI::BWAPI_getRevision()
{
  return SVN_REV;
}
bool BWAPI::BWAPI_isDebug()
{
  return BUILD_DEBUG == 1;
}