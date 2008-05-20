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
                                const u16                   maxMana)
    :name(name)
    ,unitID(unitID)
    ,maxMana(maxMana)
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
  //---------------------------------- IS ZERG ---------------------------------
  bool UnitPrototype::isZerg() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getUnitID()].getBit(BW::GroupFlags::Zerg);
  }
  //--------------------------------- IS TERRAN --------------------------------
  bool UnitPrototype::isTerran() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getUnitID()].getBit(BW::GroupFlags::Terran);
  }
  //-------------------------------- IS PROTOSS --------------------------------
  bool UnitPrototype::isProtoss() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getUnitID()].getBit(BW::GroupFlags::Protoss);
  }
  //--------------------------------- IS WORKER --------------------------------
  bool UnitPrototype::isWorker() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getUnitID()].getBit(BW::UnitPrototypeFlags::Worker);
  }
  //--------------------------------- CAN ATTACK -------------------------------
  bool UnitPrototype::canAttack() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getUnitID()].getBit(BW::UnitPrototypeFlags::Attack);
  }
  //--------------------------------- CAN MOVE ---------------------------------
  bool UnitPrototype::canMove() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getUnitID()].getBit(BW::GroupFlags::Men);
  }
  //----------------------------------------------------------------------------
};
