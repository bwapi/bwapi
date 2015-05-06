#include <Util/Types.h>
#include <BWAPI/UnitType.h>

#include "Unit.h"

#include "../../Debug.h"

namespace BW
{
  BWAPI::UnitType Unit::type() const
  {
    return BWAPI::UnitType(this->unitType);
  }
  bool Unit::movementFlag(u8 flags) const
  {
    return (this->movementFlags & flags) != 0;
  }
  bool Unit::statusFlag(u32 flags) const
  {
    return (this->statusFlags & flags) != 0;
  }
  const Unit* Unit::getDamageDealer() const
  {
    if (this->subUnit != NULL) return this->subUnit;
    return this;
  }
  bool Unit::isAttacking() const
  {
    int animState = BW::Anims::Init;
    const BW::Unit* damageDealer = this->getDamageDealer();
    if (damageDealer->sprite && damageDealer->sprite->mainGraphic)
    {
      animState = damageDealer->sprite->mainGraphic->anim;
    }
    return (animState == BW::Anims::GndAttkRpt ||  //isAttacking
            animState == BW::Anims::AirAttkRpt ||
            animState == BW::Anims::GndAttkInit ||
            animState == BW::Anims::AirAttkInit) && this->orderTargetUnit != NULL;
  }
  int Unit::getGroundWeaponCooldown() const
  {
    const BWAPI::UnitType type = this->type();
    if (type == BWAPI::UnitTypes::Protoss_Reaver || type == BWAPI::UnitTypes::Hero_Warbringer)
    {
      return this->mainOrderTimer;
    }
    return getDamageDealer()->groundWeaponCooldown;
  }
  int Unit::getAirWeaponCooldown() const
  {
    return getDamageDealer()->airWeaponCooldown;
  }
}
