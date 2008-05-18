#include "UnitPrototype.h"

#include "../BWAPI/Unit.h"
#include "../BWAPI/AbilityPrototype.h"
#include "../BWAPI/TargetType.h"

#include "../BW/Unit.h"
#include "../BW/Position.h"
#include "../BW/Offsets.h"
#include <math.h>

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
  //------------------------------ DIMENSION LEFT ------------------------------
  u16 UnitPrototype::dimensionLeft() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].left;
  }
  //------------------------------- DIMENSION UP -------------------------------
  u16 UnitPrototype::dimensionUp() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].up;
  }
  //----------------------------- DIMENSION RIGHT ------------------------------
  u16 UnitPrototype::dimensionRight() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].right;
  }
  //------------------------------ DIMENSION DOWN ------------------------------
  u16 UnitPrototype::dimensionDown() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getUnitID()].down;
  }
  //----------------------------- GET DAMAGE FACTOR ----------------------------
  u8 UnitPrototype::getDamageFactor() const
  {
    int weaponID = BW::BWXFN_UnitGroundWeapon->unit[this->getUnitID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWXFN_WeaponDamageFactor->weapon[weaponID];
  }
  //------------------------------ GET GROUND DAMAGE --------------------------
  u16 UnitPrototype::getGroundDamage() const
  {
    int weaponID = BW::BWXFN_UnitGroundWeapon->unit[this->getUnitID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWXFN_WeaponDamage->weapon[weaponID];
  }
  //--------------------------------- GET FLAGS --------------------------------
  BW::BitMask<BW::UnitPrototypeFlags::Enum> UnitPrototype::getFlags() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getUnitID()];
  }
  //-------------------------------- CAN PRODUCE -------------------------------
  bool UnitPrototype::canProduce() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::ProducesUnits);
  }
  //------------------------------- GET TILE WIDTH -----------------------------
  u16 UnitPrototype::getTileWidth() const
  {
    return (u16) ceil(((float)this->dimensionLeft() + this->dimensionRight() )/(float)BW::TileSize);
  }
  //------------------------------ GET TILE HEIGHT -----------------------------
  u16 UnitPrototype::getTileHeight() const
  {
    return (u16) ceil(((float)this->dimensionUp() + this->dimensionDown() )/(float)BW::TileSize);
  }
  //----------------------------------------------------------------------------
};
