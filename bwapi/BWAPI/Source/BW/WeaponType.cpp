#include <math.h>

#include "WeaponType.h"
#include "Offsets.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  WeaponType::WeaponType()
      : id(BW::WeaponID::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  WeaponType::WeaponType(const u8& id)
      : id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WeaponType::operator ==(const u8& id) const
  {
    return id == this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WeaponType::operator !=(const u8& id) const
  {
    return id != this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WeaponType::operator ==(const WeaponType& type) const
  {
    return this->id == type.id;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool WeaponType::operator <(const WeaponType& type) const
  {
    return this->id < type.id;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  const char* WeaponType::getName() const
  {
    if (this->getID() == BW::WeaponID::None)
      return "None";
    else if (this->getID() < BW::WEAPON_TYPE_COUNT)
    {
      u16 stringNum = BW::BWDATA_WeaponLabel->weaponType[this->getID()];
      return (char*)(*((u16*)(*(u32*)BW::BWDATA_StringTableOff + stringNum * 2)) + *((u32*)BW::BWDATA_StringTableOff));
    }
    else
      return "Invalid";
  }
  //----------------------------------------------- GET ID ---------------------------------------------------
  u8 WeaponType::getID() const
  {
    return this->id;
  }
  //--------------------------------------------- DAMAGE AMOUNT ----------------------------------------------
  u16 WeaponType::damageAmount() const
  {
    return BW::BWDATA_WeaponDamageAmount->weaponType[this->getID()];
  }
  //--------------------------------------------- DAMAGE BONUS -----------------------------------------------
  u16 WeaponType::damageBonus() const
  {
    return BW::BWDATA_WeaponDamageBonus->weaponType[this->getID()];
  }
  //-------------------------------------------- DAMAGE COOLDOWN ---------------------------------------------
  u8 WeaponType::damageCooldown() const
  {
    return BW::BWDATA_WeaponDamageCooldown->weaponType[this->getID()];
  }
  //--------------------------------------------- DAMAGE FACTOR ----------------------------------------------
  u8 WeaponType::damageFactor() const
  {
    return BW::BWDATA_WeaponDamageFactor->weaponType[this->getID()];
  }
  //---------------------------------------------- UPGRADE TYPE ----------------------------------------------
  BW::UpgradeType WeaponType::upgradeType() const
  {
    return BW::UpgradeType(BW::BWDATA_WeaponUpgrade->weaponType[this->getID()]);
  }
  //----------------------------------------------- DAMAGE TYPE ----------------------------------------------
  u8 WeaponType::damageType() const
  {
    return BW::BWDATA_WeaponDamageType->weaponType[this->getID()];
  }
  //---------------------------------------------- EXPLOSION TYPE --------------------------------------------
  u8 WeaponType::explosionType() const
  {
    return BW::BWDATA_WeaponExplosionType->weaponType[this->getID()];
  }
  //------------------------------------------------ MIN RANGE -----------------------------------------------
  u32 WeaponType::minRange() const
  {
    return BW::BWDATA_WeaponMinRange->weaponType[this->getID()];
  }
  //------------------------------------------------ MAX RANGE -----------------------------------------------
  u32 WeaponType::maxRange() const
  {
    return BW::BWDATA_WeaponMaxRange->weaponType[this->getID()];
  }
  //------------------------------------------- INNER SPLASH RADIUS ------------------------------------------
  u16 WeaponType::innerSplashRadius() const
  {
    return BW::BWDATA_WeaponInnerSplashRadius->weaponType[this->getID()];
  }
  //------------------------------------------- MEDIAN SPLASH RADIUS -----------------------------------------
  u16 WeaponType::medianSplashRadius() const
  {
    return BW::BWDATA_WeaponMedianSplashRadius->weaponType[this->getID()];
  }
  //------------------------------------------- OUTER SPLASH RADIUS ------------------------------------------
  u16 WeaponType::outerSplashRadius() const
  {
    return BW::BWDATA_WeaponOuterSplashRadius->weaponType[this->getID()];
  }
  //----------------------------------------------- TARGETS AIR ----------------------------------------------
  bool WeaponType::targetsAir() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::Air);
  }
  //---------------------------------------------- TARGETS GROUND --------------------------------------------
  bool WeaponType::targetsGround() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::Ground);
  }
  //------------------------------------------- TARGETS MECHANICAL -------------------------------------------
  bool WeaponType::targetsMechanical() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::Mechanical);
  }
  //--------------------------------------------- TARGETS ORGANIC --------------------------------------------
  bool WeaponType::targetsOrganic() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::Organic);
  }
  //------------------------------------------- TARGETS NONBUILDING ------------------------------------------
  bool WeaponType::targetsNonBuilding() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::NonBuilding);
  }
  //------------------------------------------- TARGETS NONROBOTIC -------------------------------------------
  bool WeaponType::targetsNonRobotic() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::NonRobotic);
  }
  //-------------------------------------------- TARGETS TERRAIN ---------------------------------------------
  bool WeaponType::targetsTerrain() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::Terrain);
  }
  //------------------------------------------- TARGETS ORGORMECH --------------------------------------------
  bool WeaponType::targetsOrgOrMech() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::OrgOrMech);
  }
  //---------------------------------------------- TARGETS OWN -----------------------------------------------
  bool WeaponType::targetsOwn() const
  {
    return BW::BWDATA_WeaponTargetFlags->weaponType[this->getID()].getBit(BW::WeaponTargetFlags::Own);
  }
}