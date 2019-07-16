#include <BWAPI/Unit.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Unitset.h>
#include "../BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Flag.h>

#include <limits>

#include "GameImpl.h"

namespace BWAPI
{
  //--------------------------------------------- GET TILE POSITION ------------------------------------------
  TilePosition UnitInterface::getTilePosition() const
  {
    return TilePosition( Position(abs(this->getPosition().x - this->getType().tileWidth()  * 32 / 2),
                                  abs(this->getPosition().y - this->getType().tileHeight() * 32 / 2)) );
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  int UnitInterface::getDistance(Position target) const
  {
    // If this unit does not exist or target is invalid
    if (!exists() || !target)
      return std::numeric_limits<int>::max();

    /////// Compute distance

    // retrieve left/top/right/bottom values for calculations
    int left = target.x - 1;
    int top = target.y - 1;
    int right = target.x + 1;
    int bottom = target.y + 1;

    // compute x distance
    int xDist = this->getLeft() - right;
    if (xDist < 0)
    {
      xDist = left - this->getRight();
      if (xDist < 0)
        xDist = 0;
    }

    // compute y distance
    int yDist = this->getTop() - bottom;
    if (yDist < 0)
    {
      yDist = top - this->getBottom();
      if (yDist < 0)
        yDist = 0;
    }

    // compute actual distance
    return Positions::Origin.getApproxDistance(Position(xDist, yDist));
  }
  int UnitInterface::getDistance(Unit target) const
  {
    // If this unit does not exist or target is invalid
    if (!exists() || !target || !target->exists())
      return std::numeric_limits<int>::max();

    // If target is the same as the source
    if (this == target)
      return 0;

    /////// Compute distance

    // retrieve left/top/right/bottom values for calculations
    int left = target->getLeft() - 1;
    int top = target->getTop() - 1;
    int right = target->getRight() + 1;
    int bottom = target->getBottom() + 1;

    // compute x distance
    int xDist = this->getLeft() - right;
    if (xDist < 0)
    {
      xDist = left - this->getRight();
      if (xDist < 0)
        xDist = 0;
    }

    // compute y distance
    int yDist = this->getTop() - bottom;
    if (yDist < 0)
    {
      yDist = top - this->getBottom();
      if (yDist < 0)
        yDist = 0;
    }

    // compute actual distance
    return Positions::Origin.getApproxDistance(Position(xDist, yDist));
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitInterface::hasPath(Position target) const
  {
    BroodwarImpl.setLastError();
    // Return error if the position is invalid
    if (!target)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    // Return true if this unit is an air unit
    if (this->isFlying())
      return true;

    // Return error if this does not exist
    if (!exists())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Visible);

    // Check the center of the unit (most common behaviour)
    if (BroodwarImpl.hasPath(this->getPosition(), target))
      return true;

    // Otherwise check all four corners of the unit, in case it accidentally fell out of its region
    if (BroodwarImpl.hasPath(Position(getLeft(), getTop()), target))
      return true;

    if (BroodwarImpl.hasPath(Position(getRight(), getTop()), target))
      return true;

    if (BroodwarImpl.hasPath(Position(getLeft(), getBottom()), target))
      return true;

    return BroodwarImpl.hasPath(Position(getRight(), getBottom()), target);
  }
  bool UnitInterface::hasPath(Unit target) const
  {
    BroodwarImpl.setLastError();
    // Return error if the target is invalid
    if (!target)
      return BroodwarImpl.setLastError(Errors::Invalid_Parameter);

    // Return error if target does not exist
    if (!target->exists())
      return BroodwarImpl.setLastError(Errors::Unit_Not_Visible);

    return hasPath(target->getPosition());
  }
  //--------------------------------------------- GET LEFT ---------------------------------------------------
  int UnitInterface::getLeft() const
  {
    return this->getPosition().x - this->getType().dimensionLeft();
  }
  //--------------------------------------------- GET TOP ----------------------------------------------------
  int UnitInterface::getTop() const
  {
    return this->getPosition().y - this->getType().dimensionUp();
  }
  //--------------------------------------------- GET RIGHT --------------------------------------------------
  int UnitInterface::getRight() const
  {
    return this->getPosition().x + this->getType().dimensionRight();
  }
  //--------------------------------------------- GET BOTTOM -------------------------------------------------
  int UnitInterface::getBottom() const
  {
    return this->getPosition().y + this->getType().dimensionDown();
  }
  //--------------------------------------------- IS BEING CONSTRUCTED ---------------------------------------
  bool UnitInterface::isBeingConstructed() const
  {
    if ( this->isMorphing() )
      return true;
    if ( this->isCompleted() )
      return false;
    if ( this->getType().getRace() != Races::Terran )
      return true;
    return this->getBuildUnit() != nullptr;
  }
  // ------------------------------------------ STATUS ---------------------------------------------
  bool UnitInterface::isDefenseMatrixed() const
  {
    return this->getDefenseMatrixTimer() != 0;
  }

  bool UnitInterface::isEnsnared() const
  {
    return this->getEnsnareTimer() != 0;
  }

  bool UnitInterface::isFollowing() const
  {
    return this->getOrder() == Orders::Follow;
  }
  bool UnitInterface::isFlying() const
  {
    return this->getType().isFlyer() || this->isLifted();
  }

  bool UnitInterface::isHoldingPosition() const
  {
    return this->getOrder() == Orders::HoldPosition;
  }

  //--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
  bool UnitInterface::isInWeaponRange(Unit target) const
  {
    // Preliminary checks
    if ( !this->exists() || !target || !target->exists() || this == target )
      return false;

    // Store the types as locals
    UnitType thisType = getType();
    UnitType targType = target->getType();

    // Obtain the weapon type
    WeaponType wpn = target->isFlying() ? thisType.airWeapon() : thisType.groundWeapon();

    // Return if there is no weapon type
    if ( wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown )
      return false;

    // Retrieve the min and max weapon ranges
    int minRange = wpn.minRange();
    int maxRange = getPlayer()->weaponMaxRange(wpn);

    // Check if the distance to the unit is within the weapon range
    int distance = this->getDistance(target);
    return (minRange ? minRange < distance : true) && distance <= maxRange;
  }

  bool UnitInterface::isIrradiated() const
  {
    return this->getIrradiateTimer() != 0;
  }

  bool UnitInterface::isLoaded() const
  {
    return this->getTransport() != nullptr;
  }

  bool UnitInterface::isLockedDown() const
  {
    return this->getLockdownTimer() != 0;
  }

  bool UnitInterface::isMaelstrommed() const
  {
    return this->getMaelstromTimer() != 0;
  }

  bool UnitInterface::isPatrolling() const
  {
    return this->getOrder() == Orders::Patrol;
  }

  bool UnitInterface::isPlagued() const
  {
    return this->getPlagueTimer() != 0;
  }

  bool UnitInterface::isRepairing() const
  {
    return this->getOrder() == Orders::Repair;
  }

  bool UnitInterface::isResearching() const
  {
    return this->getOrder() == Orders::ResearchTech;
  }

  bool UnitInterface::isSieged() const
  {
    UnitType t(this->getType());
    return  t == UnitTypes::Terran_Siege_Tank_Siege_Mode || 
            t == UnitTypes::Hero_Edmund_Duke_Siege_Mode;
  }

  bool UnitInterface::isStasised() const
  {
    return this->getStasisTimer() != 0;
  }

  bool UnitInterface::isStimmed() const
  {
    return this->getStimTimer() != 0;
  }

  bool UnitInterface::isUpgrading() const
  {
    return this->getOrder() == Orders::Upgrade;
  }

  int UnitInterface::getSpaceRemaining() const
  {
    int space = this->getType().spaceProvided();

    // Decrease the space for each loaded unit
    for (auto &u : getLoadedUnits())
      space -= u->getType().spaceRequired();

    return std::max(space, 0);
  }
};
