#pragma once

#include <string>
#include <map>

#include <Util/Bitmask.h>
#include <Util/Types.h>

#include "WeaponTargetFlags.h"
#include "WeaponID.h"
#include "Offsets.h"


namespace BW
{
  /** 
  * Definition of weapon prototype, it's possible types are listed in #BW#WeaponID.
  * The only data this class holds is the enumeration of type u16, so it can be used directly
  * in the mapping of #BW#Weapon
  * 
  */
  class WeaponType
  {
  public :
    WeaponType();
    WeaponType(const BW::WeaponID::Enum& id); 
    bool operator == (const BW::WeaponID::Enum& id) const;
    bool operator != (const BW::WeaponID::Enum& id) const;
    bool operator == (const WeaponType& type) const;
    bool operator  < (const WeaponType& type) const;
    const char*          getName() const;
    BW::WeaponID::Enum     getID() const;
    BW::WeaponID::Enum id;
  private :
  };
}