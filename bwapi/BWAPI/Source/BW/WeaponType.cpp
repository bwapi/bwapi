#include <math.h>

#include "WeaponType.h"
#include "Offsets.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  WeaponType::WeaponType()
  :id(BW::WeaponID::None)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  WeaponType::WeaponType(const BW::WeaponID::Enum& id)
  :id(id)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WeaponType::operator ==(const BW::WeaponID::Enum& id) const
  {
    return id == this->id;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WeaponType::operator !=(const BW::WeaponID::Enum& id) const
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
      int stringNum=BW::BWDATA_WeaponLabel->weaponType[this->id];
      //doesn't work yet
      return (char*)(*((u16*)(*(u32*)BW::BWDATA_StringTableOff + stringNum*2 + 2)) + *((u32*)BW::BWDATA_StringTableOff));
    }
    else
      return "Invalid";
  }
  //----------------------------------------------- GET ID ---------------------------------------------------
  BW::WeaponID::Enum WeaponType::getID() const
  {
    return this->id;
  }
}