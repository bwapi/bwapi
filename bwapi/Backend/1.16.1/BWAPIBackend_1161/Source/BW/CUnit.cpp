#include "../Types.h"
#include <BWAPI/UnitType.h>
#include "CImage.h"
#include "CSprite.h"

#include "CUnit.h"

namespace BW
{
  BWAPI::UnitType CUnit::type() const
  {
    return BWAPI::UnitType(this->unitType);
  }
  bool CUnit::movementFlag(u8 flags) const
  {
    return (this->movementFlags & flags) != 0;
  }
  bool CUnit::statusFlag(u32 flags) const
  {
    return (this->statusFlags & flags) != 0;
  }
  const CUnit* CUnit::getDamageDealer() const
  {
    if (this->subUnit != nullptr) return this->subUnit;
    return this;
  }
  bool CUnit::isAttacking() const
  {
    BW::Anims::Enum animState = BW::Anims::Init;
    const BW::CUnit* damageDealer = this->getDamageDealer();
    if (damageDealer->sprite && damageDealer->sprite->pImagePrimary)
    {
      animState = damageDealer->sprite->pImagePrimary->anim;
    }
    return (animState == BW::Anims::GndAttkRpt ||  //isAttacking
            animState == BW::Anims::AirAttkRpt ||
            animState == BW::Anims::GndAttkInit ||
            animState == BW::Anims::AirAttkInit) && this->orderTarget.pUnit != nullptr;
  }
  int CUnit::getGroundWeaponCooldown() const
  {
    const BWAPI::UnitType type = this->type();
    if (type == BWAPI::UnitTypes::Protoss_Reaver || type == BWAPI::UnitTypes::Hero_Warbringer)
    {
      return this->mainOrderTimer;
    }
    return getDamageDealer()->groundWeaponCooldown;
  }
  int CUnit::getAirWeaponCooldown() const
  {
    return getDamageDealer()->airWeaponCooldown;
  }
}
