#include "UnitPrototype.h"

#include "../BWAPI/Unit.h"
#include "../BWAPI/AbilityPrototype.h"
#include "../BWAPI/TargetType.h"

#include "../BW/UnitData.h"
#include "../BW/Position.h"
#include "../BW/Offsets.h"

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  UnitPrototype::UnitPrototype( const std::string&          name, 
                                const BW::UnitType::Enum    unitID,
                                const BWAPI::RaceType::Enum race,
                                const u16                   maxMana,
                                const u32                   abilityFlags)
    :name(name)
    ,unitID(unitID)
    ,race(race)
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
    return BW::BWXFN_MaxHealthPoints_NotAttackable_Repairable->raw[this->getUnitID()].maxHealthPoints;
  }
  //------------------------- GET MAX HEALTH POINTS -----------------------------
  u16 UnitPrototype::getMaxShieldPoints() const
  {
    return BW::BWXFN_MaxShieldPoints->maxShieldPoints[this->getUnitID()];
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
  u16 UnitPrototype::getMineralPrice() const
  {
    return BW::BWXFN_MineralPrices->mineralPrice[this->getUnitID()];
  }
  //------------------------------ GET GAS PRICE -------------------------------
  u16 UnitPrototype::getGasPrice() const
  {
    return BW::BWXFN_GasPrices->gasPrice[this->getUnitID()];
  }
  //-------------------------------- GET RACE ----------------------------------
  BWAPI::RaceType::Enum UnitPrototype::getRace() const
  {
    return this->race;
  }
  //------------------------------- GET SUPPLIES -------------------------------
  s8 UnitPrototype::getSupplies() const
  {
    return BW::BWXFN_SupplyDemands->supplyDemand[this->getUnitID()];
  }
  //------------------------------- GET ARMOR ----------------------------------
  u8 UnitPrototype::getArmor() const
  {
    return BW::BWXFN_Armor->armor[this->getUnitID()];
  }
  //------------------------------ GET BUILD TIME ------------------------------
  u16 UnitPrototype::getBuildTime() const
  {
    return BW::BWXFN_BuildTime->buildTime[this->getUnitID()];
  }
  //----------------------------------------------------------------------------
  UNKNOWN_TYPE UnitPrototype::getUnknown() const
  {
    return BW::BWXFN_Unknown->unknown[this->getUnitID()];
  }
  //----------------------------------------------------------------------------
  u16 UnitPrototype::dimensionLeft() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].left;
  }
  //----------------------------------------------------------------------------
  u16 UnitPrototype::dimensionUp() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].up;
  }
  //----------------------------------------------------------------------------
  u16 UnitPrototype::dimensionRight() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].right;
  }
  //----------------------------------------------------------------------------
  u16 UnitPrototype::dimensionDown() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].down;
  }
  //----------------------------------------------------------------------------
  u8 UnitPrototype::getDamageFactor() const
  {
    int weaponID = BW::BWXFN_UnitGroundWeapon->unit[this->getUnitID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWXFN_WeaponDamageFactor->weapon[weaponID];
  }
  //----------------------------------------------------------------------------
  u16 UnitPrototype::getGroundDamage() const
  {
    int weaponID = BW::BWXFN_UnitGroundWeapon->unit[this->getUnitID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWXFN_WeaponDamage->weapon[weaponID];
  }
  //----------------------------------------------------------------------------
  BW::BitMask<BW::UnitPrototypeFlags::Enum> UnitPrototype::getFlags() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getUnitID()];
  }
  //----------------------------------------------------------------------------
  bool UnitPrototype::canProduce() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::ProducesUnits);
  }
  //----------------------------------------------------------------------------
};
