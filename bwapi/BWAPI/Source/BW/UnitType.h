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
#include "Offsets.h"


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
    UnitType(const BW::UnitID::Enum& id); /**< Conversion constructor */
    bool operator == (const BW::UnitID::Enum& id) const;
    bool operator != (const BW::UnitID::Enum& id) const;
    bool operator == (const UnitType& type) const;
    bool operator  < (const UnitType& type) const;
    const char*          getName() const;
    BW::UnitID::Enum     getID() const;
    u16                  getMaxHealthPoints() const;
    u16                  getMaxShieldPoints() const;
    u16                  getMaxEnergyPoints() const;
    u16                  getMineralPrice() const;
    u16                  getGasPrice() const;
    u8                   getArmor() const;
    u16                  getBuildTime() const;
    s8                   supplyRequired() const;
    s8                   supplyProvided() const;
    u16                  dimensionLeft() const;
    u16                  dimensionUp() const;
    u16                  dimensionRight() const;
    u16                  dimensionDown() const;
    u16                  getTileWidth() const;
    u16                  getTileHeight() const;
    /**
     * Gets Damage factor of the ground weapon of the unit @todo find out what
     * does it mean.
     */
    u8                   getDamageFactor() const;
    /** Gets base Damage of the ground weapon of the unit. */
    u16                  getGroundDamage() const;
    bool                 canProduce() const;
    bool                 isZerg() const;
    bool                 isTerran() const;
    bool                 isProtoss() const;
    bool                 canAttack() const;
    bool                 canMove() const; /**< I use some internal heuristic that every unit from [men]  group
                                            * can move, @todo verify (or load some value specially for that) 
                                            * Note that there will have to be special canMove function for
                                            * BWAPI::Unit that will take unit instance specific things
                                            * like Not borrowed, not statised, not mealstormed, not under construction.
                                            */
    bool                 isFlyer() const;
    bool                 regeneratesHP() const;
    bool                 isSpellcaster() const;
    bool                 hasPermanentCloak() const;
    bool                 isInvincible() const;
    bool                 isOrganic() const;
    bool                 isMechanical() const;
    bool                 isRobotic() const;
    bool                 isDetector() const;
    bool                 isResourceContainer() const;
    bool                 isResourceDepot() const;
    bool                 isWorker() const;
    bool                 requiresPsi() const;
    bool                 requiresCreep() const;
    bool                 isTwoUnitsInOneEgg() const;
    bool                 isBurrowable() const;
    bool                 isCloakable() const;
    bool                 isBuilding() const;
    bool                 isAddon() const;
    bool                 isFlyingBuilding() const;

    bool                 isNeutral() const;
    bool                 isNeutralAccesories() const;
    BW::Race::Enum       getRace();                                            
    bool                 isValid();
    /** Used to determine what building builds this addon. Can be extended later if needed.*/
    std::pair<BW::UnitType,int> UnitType::whatBuilds() const;
    const std::map< BW::UnitType, int >& getRequiredUnits() const;
    BW::TechID::Enum        getRequiredTech() const;
    BW::UnitID::Enum id;
    static void initialize();
  private :
    Util::BitMask<BW::UnitPrototypeFlags::Enum> getFlags() const;
    Util::BitMask<BW::GroupFlags::Enum> getGroupFlags() const;
  };
};
