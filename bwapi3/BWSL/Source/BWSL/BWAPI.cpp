#include "BWAPI.h"

#include "Race.h"
#include "DamageType.h"
#include "ExplosionType.h"
#include "Order.h"
#include "TechType.h"
#include "PlayerType.h"
#include "UpgradeType.h"
#include "WeaponType.h"
#include "UnitSizeType.h"
#include "UnitType.h"
#include "AttackType.h"
#include "Error.h"
#include "Color.h"

namespace BWSL
{
  void BWAPI_init()
  {
    Races::init();
    DamageTypes::init();
    ExplosionTypes::init();
    Orders::init();
    TechTypes::init();
    PlayerTypes::init();
    UpgradeTypes::init();
    WeaponTypes::init();
    UnitSizeTypes::init();
    UnitTypes::init();
    AttackTypes::init();
    Errors::init();
    Colors::init();
  }
}
