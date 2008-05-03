#include "UnitPrototype.h"

#include "../BWAPI/Unit.h"
#include "../BWAPI/AbilityPrototype.h"

#include "../BW/TargetType.h"
#include "../BW/UnitData.h"
#include "../BW/Position.h"

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  UnitPrototype::UnitPrototype( const std::string& name, 
                                const u16          maxHealthPoints,
                                const u16          maxShieldPoints,
                                const u16          maxMana,
                                const u32          abilityFlags)
    :name(name)
    ,maxHealthPoints(maxHealthPoints)
    ,maxShieldPoints(maxShieldPoints)
    ,maxMana(maxMana)
    ,abilityFlags(abilityFlags)
  {
  }
  //----------------------------- DESTRUCTOR ------------------------------------
  UnitPrototype::~UnitPrototype()
  {
  }
  //------------------------------- GET NAME ------------------------------------
  const std::string& UnitPrototype::getName() const
  {
    return this->name;
  }
  //------------------------- GET MAX HEALTH POINTS -----------------------------
  u16 UnitPrototype::getMaxHealthPoints() const
  {
    return this->maxHealthPoints;
  }
  //------------------------- GET MAX HEALTH POINTS -----------------------------
  u16 UnitPrototype::getMaxShieldPoints() const
  {
    return this->maxShieldPoints;
  }
  //----------------------------- GET MAX MANA ----------------------------------
  u16 UnitPrototype::getMaxMana() const
  {
    return this->maxMana;
  }
  //------------------------------- CAN ORDER -----------------------------------
  bool UnitPrototype::canOrder(const AbilityPrototype* const ability, Unit* target) const
  {
    // Firstly, we check if the unit even has the ability
    if (!(this->getAbilityFlags() & ability->getAbilityFlag()))
      return false;
    return true;
  }
  //-------------------------------- CAN ORDER ---------------------------------
  bool UnitPrototype::canOrder(const AbilityPrototype* const ability, const BW::Position& target) const
  {
    if (!(ability->getTargetFlags() & TargetType::TARGET_LOCATION))
      return false;
    return true;
  }
  //-------------------------------- GET ABILITY FLAGS -------------------------
  s32 UnitPrototype::getAbilityFlags() const
  {
    return this->abilityFlags;
  }
  //----------------------------------------------------------------------------
};
