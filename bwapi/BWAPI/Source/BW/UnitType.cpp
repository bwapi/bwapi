#include "UnitType.h"

#include <math.h>
#include <Util/Logger.h>
#include <BWAPI/Unit.h>

#include "Unit.h"
#include "Offsets.h"
#include "Position.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitType::UnitType()
  :id(BW::UnitID::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitType::UnitType(const BW::UnitID::Enum& id)
  :id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator ==(const BW::UnitID::Enum& id)
  {
    return id == this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator !=(const BW::UnitID::Enum& id)
  {
    return id != this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool UnitType::operator ==(const UnitType& type)
  {
    return this->id == type.id;
  }  
  //------------------------------------------------ GET NAME ------------------------------------------------
  const char* UnitType::getName() const
  {
    if (this->getID() == BW::UnitID::None)
      return "None";
    else if (this->getID() == BW::UnitID::All)
      return "All";
    else if (this->getID() == BW::UnitID::Men)
      return "Men";
    else if (this->getID() == BW::UnitID::Buildings)
      return "Buildings";
    else if (this->getID() == BW::UnitID::Factories)
      return "Factories";
    else
      return (char*)(*((u16*)(*(u32*)0x6d1220 + this->getID()*2 + 2)) + *((u32*)0x6d1220));
  }
  //---------------------------------------------- GET UNIT ID -----------------------------------------------
  BW::UnitID::Enum UnitType::getID() const
  {
    return this->id;
  }
  //----------------------------------------- GET MAX HEALTH POINTS ------------------------------------------
  u16 UnitType::getMaxHealthPoints() const
  {
    return BW::BWXFN_MaxHealthPoints_NotAttackable_Repairable->raw[this->getID()].maxHealthPoints;
  }
  //----------------------------------------- GET MAX HEALTH POINTS ------------------------------------------
  u16 UnitType::getMaxShieldPoints() const
  {
    return BW::BWXFN_MaxShieldPoints->maxShieldPoints[this->getID()];
  }
  //------------------------------------------- GET MINERAL PRICE --------------------------------------------
  u16 UnitType::getMineralPrice() const
  {
    return BW::BWXFN_MineralPrices->mineralPrice[this->getID()];
  }
  //--------------------------------------------- GET GAS PRICE ----------------------------------------------
  u16 UnitType::getGasPrice() const
  {
    return BW::BWXFN_GasPrices->gasPrice[this->getID()];
  }
  //---------------------------------------------- GET SUPPLIES ----------------------------------------------
  s8 UnitType::getSupplies() const
  {
    return BW::BWXFN_SupplyDemands->supplyDemand[this->getID()];
  }
  //----------------------------------------------- GET ARMOR ------------------------------------------------
  u8 UnitType::getArmor() const
  {
    return BW::BWXFN_Armor->armor[this->getID()];
  }
  //--------------------------------------------- GET BUILD TIME ---------------------------------------------
  u16 UnitType::getBuildTime() const
  {
    return BW::BWXFN_BuildTime->buildTime[this->getID()];
  }
  //--------------------------------------------- DIMENSION LEFT ---------------------------------------------
  u16 UnitType::dimensionLeft() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getID()].left;
  }
  //---------------------------------------------- DIMENSION UP ----------------------------------------------
  u16 UnitType::dimensionUp() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getID()].up;
  }
  //-------------------------------------------- DIMENSION RIGHT ---------------------------------------------
  u16 UnitType::dimensionRight() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getID()].right;
  }
  //--------------------------------------------- DIMENSION DOWN ---------------------------------------------
  u16 UnitType::dimensionDown() const
  {
    return BW::BWXFN_UnitDimensions->units[this->getID()].down;
  }
  //------------------------------------------- GET DAMAGE FACTOR --------------------------------------------
  u8 UnitType::getDamageFactor() const
  {
    int weaponID = BW::BWXFN_UnitGroundWeapon->unit[this->getID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWXFN_WeaponDamageFactor->weapon[weaponID];
  }
  //------------------------------------------- GET GROUND DAMAGE --------------------------------------------
  u16 UnitType::getGroundDamage() const
  {
    int weaponID = BW::BWXFN_UnitGroundWeapon->unit[this->getID()];
    if (weaponID == BW::NoWeapon)
       return 0;
    else
       return BW::BWXFN_WeaponDamage->weapon[weaponID];
  }
  //----------------------------------------------- GET FLAGS ------------------------------------------------
  Util::BitMask<BW::UnitPrototypeFlags::Enum> UnitType::getFlags() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getID()];
  }
  //---------------------------------------------- CAN PRODUCE -----------------------------------------------
  bool UnitType::canProduce() const
  {
    return this->getGroupFlags().getBit(BW::GroupFlags::Factory);
  }
  //-------------------------------------------- GET GROUP FLAGS ---------------------------------------------
  Util::BitMask<BW::GroupFlags::Enum> UnitType::getGroupFlags() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getID()];
  }
  //--------------------------------------------- GET TILE WIDTH ---------------------------------------------
  u16 UnitType::getTileWidth() const
  {
    return (this->dimensionLeft() + this->dimensionRight() + BW::TILE_SIZE - 1)/BW::TILE_SIZE;
  }
  //-------------------------------------------- GET TILE HEIGHT ---------------------------------------------
  u16 UnitType::getTileHeight() const
  {
    return (this->dimensionUp() + this->dimensionDown() + BW::TILE_SIZE - 1)/BW::TILE_SIZE;
  }
  //------------------------------------------------ IS ZERG -------------------------------------------------
  bool UnitType::isZerg() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Zerg);
  }
  //----------------------------------------------- IS TERRAN ------------------------------------------------
  bool UnitType::isTerran() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Terran);
  }
  //----------------------------------------------- IS PROTOSS -----------------------------------------------
  bool UnitType::isProtoss() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Protoss);
  }
  //----------------------------------------------- IS WORKER ------------------------------------------------
  bool UnitType::isWorker() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getID()].getBit(BW::UnitPrototypeFlags::Worker);
  }
  //----------------------------------------------- CAN ATTACK -----------------------------------------------
  bool UnitType::canAttack() const
  {
    return BW::BWXFN_UnitPrototypeFlags->unit[this->getID()].getBit(BW::UnitPrototypeFlags::Attack);
  }
  //------------------------------------------------ CAN MOVE ------------------------------------------------
  bool UnitType::canMove() const
  {
    return BW::BWXFN_PrototypeGroupFlags->unit[this->getID()].getBit(BW::GroupFlags::Men);
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitType::isValid()
  {
    return this->id < 228;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BW::Race::Enum UnitType::getRace()
  {
    if (this->isZerg())
      return BW::Race::Zerg;
    else if (this->isProtoss())
      return BW::Race::Protoss;
    else if (this->isTerran())
      return BW::Race::Terran;
    else
      return BW::Race::Other;
  }
  //---------------------------------------------- IS BUILDING -----------------------------------------------
  bool UnitType::isBuilding() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::Building);
  }
  //------------------------------------------------ IS ADDON ------------------------------------------------
  bool UnitType::isAddon() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::Addon);
  }
  //----------------------------------------------- IS FLAYER ------------------------------------------------
  bool UnitType::isFlayer() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::Flyer);
  }  
  //----------------------------------------------- IS NEUTRAL -----------------------------------------------
  bool UnitType::isNeutral() const
  {
    return this->getGroupFlags().getBit(BW::GroupFlags::Neutral);
  }  
  //----------------------------------------- IS NEUTRAL ACCESORIES ------------------------------------------
  bool UnitType::isNeutralAccesories() const
  {
    return this->getFlags().getBit(BW::UnitPrototypeFlags::NeutralAccessories);
  }
  //--------------------------------------------- WHERE TO BUILD ---------------------------------------------
  BW::UnitType UnitType::whereToBuild()
  {
    switch (this->getID())
    {
      case BW::UnitID::Terran_ComsatStation :
      case BW::UnitID::Terran_NuclearSilo   : return BW::UnitID::Terran_CommandCenter;
      case BW::UnitID::Terran_MachineShop   : return BW::UnitID::Terran_Factory;
      case BW::UnitID::Terran_ControlTower  : return BW::UnitID::Terran_Starport;
      case BW::UnitID::Terran_CovertOps     :
      case BW::UnitID::Terran_PhysicsLab    : return BW::UnitID::Terran_ScienceFacility;
      default : return BW::UnitID::None;
    }
  }
  //------------------------------------------ GET SUPPLY PRODUCED -------------------------------------------
  u16 UnitType::getSupplyProduced()
  {
    return BWXFN_SupplyProduced->unitType[this->getID()];
  }
  //----------------------------------------------------------------------------------------------------------
}