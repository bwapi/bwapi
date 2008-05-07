#include "UnitPrototype.h"

#include "../BWAPI/Unit.h"
#include "../BWAPI/AbilityPrototype.h"

#include "../BW/TargetType.h"
#include "../BW/UnitData.h"
#include "../BW/Position.h"

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  UnitPrototype::UnitPrototype( const std::string&       name, 
                                const BW::UnitType::Enum unitID,
                                const u16                maxHealthPoints,
                                const u16                maxShieldPoints,
                                const u16                maxMana,
                                const s32                mineralPrice,
                                const s32                gasPrice,
                                const u32                abilityFlags)
    :name(name)
    ,unitID(unitID)
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
  //------------------------------ GET UNIT ID ----------------------------------
  BW::UnitType::Enum UnitPrototype::getUnitID() const
  {
    return this->unitID;
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
  //------------------------------ GET ABILITY FLAGS ---------------------------
  s32 UnitPrototype::getAbilityFlags() const
  {
    return this->abilityFlags;
  }
  //------------------------------ GET MINERAL PRICE ---------------------------
  s32 UnitPrototype::getMineralPrice() const
  {
    return this->mineralPrice;
  }
  //------------------------------ GET GAS PRICE -------------------------------
  s32 UnitPrototype::getGasPrice() const
  {
    return this->gasPrice;
  }
  //----------------------------------------------------------------------------
};
