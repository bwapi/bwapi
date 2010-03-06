#pragma once

#include <string>
#include <map>

#include <Util/Bitmask.h>
#include <Util/Types.h>

#include "Race.h"
#include "UnitPrototypeFlags.h"
#include "GroupFlags.h"
#include "UnitID.h"
#include "TechID.h"
#include "UpgradeID.h"
#include "WeaponID.h"
#include "Offsets.h"
#include "UnitSizeType.h"


namespace BW
{
  /**
  * Definition of unit prototype, it's possible types are listed in #BW#UnitID.
  * The only data this class holds is the enumeration of type u16, so it can be used directly
  * in the mapping of #BW#Unit
  *
  * Use This class instance instead of the #BW#UnitID enumeration value as most as possible
  * alghough they are the same thing practicly, but the enumeration doesn't have all these
  * help functions, note that as there is == nad != operator and conversion constructor, so
  * the enumeration and this class can be compared/assigned freely.
  *
  */
  class UnitType
  {
    public :
      UnitType(); /**< Default constructor orderId will be BW::OrderID::None. */
      UnitType(int id); /**< Conversion constructor */
      UnitType(const UnitType& type);
      bool operator == (int id) const;
      bool operator != (int id) const;
      bool operator  < (int id) const;
      bool operator == (const UnitType& type) const;
      bool operator != (const UnitType& type) const;
      bool operator  < (const UnitType& type) const;

      int         getID() const;
      std::string getName() const;
      std::string getSubLabel() const;

      int maxHitPoints() const;
      int maxShields() const;
      int maxEnergy() const;
      int armor() const;

      int mineralPrice() const;
      int gasPrice() const;
      int buildTime() const;

      int supplyRequired() const;
      int supplyProvided() const;
      int spaceRequired() const;
      int spaceProvided() const;
      int buildScore() const;
      int destroyScore() const;

      int tileWidth() const;
      int tileHeight() const;
      int dimensionLeft() const;
      int dimensionUp() const;
      int dimensionRight() const;
      int dimensionDown() const;

      int seekRange() const;
      int sightRange() const;
      int maxGroundHits() const;
      int maxAirHits() const;
      
      int acceleration() const;
      int haltDistance() const;
      int turnRadius() const;

      bool canProduce() const;
      bool canAttack() const;
      bool canMove() const;

      bool isFlyer() const;
      bool regeneratesHP() const;
      bool isSpellcaster() const;
      bool hasPermanentCloak() const;
      bool isInvincible() const;
      bool isOrganic() const;
      bool isMechanical() const;
      bool isRobotic() const;
      bool isDetector() const;
      bool isResourceContainer() const;
      bool isResourceDepot() const;
      bool isRefinery() const;
      bool isWorker() const;
      bool requiresPsi() const;
      bool requiresCreep() const;
      bool isTwoUnitsInOneEgg() const;
      bool isBurrowable() const;
      bool isCloakable() const;
      bool isBuilding() const;
      bool isAddon() const;
      bool isFlyingBuilding() const;
      bool isNeutral() const;

      std::pair<BW::UnitType, int>         _whatBuilds() const;
      const std::map< BW::UnitType, int >& _requiredUnits() const;

      const char* _getName() const;
      const char* _getSubLabel() const;

      u8   _getRace() const;
      u8   _requiredTech() const;
      u8   _armorUpgrade() const;
      u8   _size() const;
      u8   _groundWeapon() const;
      u8   _airWeapon() const;
      u32  _topSpeed() const;
      u8   graphics() const;
      bool isNeutralAccesories() const;
      bool isZerg() const;
      bool isTerran() const;
      bool isProtoss() const;
      bool isValid() const;
      int  id;
      static void initialize();
    private :
      Util::BitMask<u32> getFlags() const;
      Util::BitMask<u8>  getGroupFlags() const;
  };
};
