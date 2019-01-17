#include <BWAPI/Unit.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Game.h>
#include <BWAPI/Region.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Flag.h>

#include <limits>

using namespace BWAPI;

UnitData const &Unit::getUnitData() const
{
  return getGame().getUnitData(getID());
}

UnitData const &Unit::getInitialData() const
{
  return getGame().getInitialData(getID());
}
  
Player Unit::getPlayer() const
{
  return {getGame(), getUnitData().player};
}
//--------------------------------------------- ETC ----------------------------------
Unitset Unit::getLoadedUnits() const {
  Unitset units;
  for (auto &u : getUnitData().loadedUnits)
    units.emplace(getGame(), u.getID());
  return units;
}

int Unit::getSpaceRemaining() const {
  int space = getType().spaceProvided();

  // Decrease the space for each loaded unit
  for (auto &loadedUnit : getLoadedUnits())
    space -= Unit{ getGame(), loadedUnit }.getType().spaceRequired();

  return space;
}

//Unitset Unit::getInterceptors() const { return getUnitData().interceptors; }
Unitset Unit::getInterceptors() const { return Unitset::none; }
//Unitset Unit::getLarva() const { return getUnitData().larva; }
Unitset Unit::getLarva() const { return Unitset::none; }

UnitCommand Unit::getLastCommand() const { return getUnitData().lastCommand; }

//------------------------------------------------ GET UNITS IN RADIUS -------------------------------------
Unitset Unit::getUnitsInRadius(int radius, const UnitFilter &pred) const
{
// Return if this unit does not exist
if (!this->exists())
  return Unitset::none;

return getGame().getUnitsInRectangle(
    this->getLeft() - radius, this->getTop() - radius,
    this->getRight() + radius, this->getBottom() + radius, [&](Unit u) {
      return *this != u && getDistance(u) <= radius &&
              (!pred.isValid() || pred(u));
    });
}

Unit Unit::getClosestUnit(const UnitFilter &pred, int radius) const
{
  // Return if this unit does not exist
  if ( !exists() )
    return {getGame(), UnitID{-1}};
    
  return getGame().getClosestUnitInRectangle(this->getPosition(),
                                              [&](Unit u){ return *this != u && getDistance(u) <= radius && (!pred.isValid() || pred(u)); }, 
                                              this->getLeft()   - radius,
                                              this->getTop()    - radius,
                                              this->getRight()  + radius,
                                              this->getBottom() + radius);
}
//--------------------------------------------- GET UNITS IN WEAPON RANGE ----------------------------------
Unitset Unit::getUnitsInWeaponRange(WeaponType weapon, const UnitFilter &pred) const
{
  // Return if this unit does not exist
  if ( !this->exists() )
    return Unitset::none;

  int max = getPlayer().weaponMaxRange(weapon);

  return getGame().getUnitsInRectangle(this->getLeft()    - max,
                                        this->getTop()     - max,
                                        this->getRight()   + max,
                                        this->getBottom()  + max,
                                        [&](Unit u)->bool
                                        {
                                        // Unit check and unit status
                                        if ( u == *this || u->isInvincible() )
                                          return false;

                                        // Weapon distance check
                                        int dist = this->getDistance(u);
                                        if ( (weapon.minRange() && dist < weapon.minRange()) || dist > max )
                                          return false;

                                        // Weapon behavioural checks
                                        UnitType ut = u->getType();
                                        if ( (( weapon.targetsOwn()          && u->getPlayer() != this->getPlayer() ) ||
                                              ( !weapon.targetsAir()         && !u->isFlying() ) ||
                                              ( !weapon.targetsGround()      && u->isFlying()  ) ||
                                              ( weapon.targetsMechanical()   && ut.isMechanical()                 ) ||
                                              ( weapon.targetsOrganic()      && ut.isOrganic()                    ) ||
                                              ( weapon.targetsNonBuilding()  && !ut.isBuilding()                  ) ||
                                              ( weapon.targetsNonRobotic()   && !ut.isRobotic()                   ) ||
                                              ( weapon.targetsOrgOrMech()    && (ut.isOrganic() || ut.isMechanical()) ))  )
                                          return false;

                                        return pred(u);
                                      });
}
//--------------------------------------------- GET DISTANCE -----------------------------------------------
int Unit::getDistance(Position target) const
{
  // If this unit does not exist or target is invalid
  if (!exists() || !getGame().isValid(target))
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
int Unit::getDistance(Unit target) const
{
  // If this unit does not exist or target is invalid
  if (!exists() || !target || !target->exists())
    return std::numeric_limits<int>::max();

  // If target is the same as the source
  if (*this == target)
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
bool Unit::hasPath(Position target) const
{
  getGame().setLastError();
  // Return error if the position is invalid
  if (!getGame().isValid(target))
    return getGame().setLastError(Errors::Invalid_Parameter);

  // Return true if this unit is an air unit
  if (this->isFlying())
    return true;

  // Return error if this does not exist
  if (!exists())
    return getGame().setLastError(Errors::Unit_Not_Visible);

  // Check the center of the unit (most common behaviour)
  if (getGame().hasPath(this->getPosition(), target))
    return true;

  // Otherwise check all four corners of the unit, in case it accidentally fell out of its region
  if (getGame().hasPath(Position(getLeft(), getTop()), target))
    return true;

  if (getGame().hasPath(Position(getRight(), getTop()), target))
    return true;

  if (getGame().hasPath(Position(getLeft(), getBottom()), target))
    return true;

  return getGame().hasPath(Position(getRight(), getBottom()), target);
}
bool Unit::hasPath(Unit target) const
{
  getGame().setLastError();
  // Return error if the target is invalid
  if (!target)
    return getGame().setLastError(Errors::Invalid_Parameter);

  // Return error if target does not exist
  if (!target->exists())
    return getGame().setLastError(Errors::Unit_Not_Visible);

  return hasPath(target->getPosition());
}
Player Unit::getLastAttackingPlayer() const
{
  return { getGame(), getUnitData().lastAttackerPlayer };
}
//--------------------------------------------- GET REGION -------------------------------------------------
Region Unit::getRegion() const
{
  return getGame().getRegionAt(this->getPosition());
}

//--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
bool Unit::isInWeaponRange(Unit target) const
{
  // Preliminary checks
  if ( !this->exists() || !target || !target->exists() || *this == target )
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

bool Unit::isLockedDown() const
{
  return this->getLockdownTimer() != 0;
}

bool Unit::isMaelstrommed() const
{
  return this->getMaelstromTimer() != 0;
}
bool Unit::isVisible(Player player) const {
  auto const iplayer = static_cast<int>(player.getID());
  return player && iplayer < 9 && getUnitData().isVisible[iplayer];
}
bool Unit::isVisible() const {
  return isVisible(getPlayer());
}
//--------------------------------------------- ATTACK MOVE ------------------------------------------------
bool Unit::attack(Position target, bool shiftQueueCommand) const
{
  return this->issueCommand(UnitCommand::attack(getID(), target, shiftQueueCommand));
}
bool Unit::attack(Unit target, bool shiftQueueCommand) const
{
  return this->issueCommand(UnitCommand::attack(getID(), target.getID(), shiftQueueCommand));
}
//--------------------------------------------- BUILD ------------------------------------------------------
bool Unit::build(UnitType type, TilePosition target) const
{
  if ( target == TilePositions::None )
    return this->train(type);
  return this->issueCommand(UnitCommand::build(getID(), target, type));
}
//--------------------------------------------- BUILD ADDON ------------------------------------------------
bool Unit::buildAddon(UnitType type) const
{
  return issueCommand(UnitCommand::buildAddon(getID(),type));
}
//--------------------------------------------- TRAIN ------------------------------------------------------
bool Unit::train(UnitType type) const
{
  // Carrier/Reaver specialization
  if ( type == UnitTypes::None )
  {
    switch (getType())
    {
    case UnitTypes::Protoss_Carrier:
    case UnitTypes::Hero_Gantrithor:
      type = UnitTypes::Protoss_Interceptor;
      break;
    case UnitTypes::Protoss_Reaver:
    case UnitTypes::Hero_Warbringer:
      type = UnitTypes::Protoss_Scarab;
      break;
    }
  }
  return issueCommand(UnitCommand::train(getID(),type));
}
//--------------------------------------------- MORPH ------------------------------------------------------
bool Unit::morph(UnitType type) const
{
  return issueCommand(UnitCommand::morph(getID(),type));
}
//--------------------------------------------- RESEARCH ---------------------------------------------------
bool Unit::research(TechType tech) const
{
  return issueCommand(UnitCommand::research(getID(),tech));
}
//--------------------------------------------- UPGRADE ----------------------------------------------------
bool Unit::upgrade(UpgradeType upgrade) const
{
  return issueCommand(UnitCommand::upgrade(getID(),upgrade));
}
//--------------------------------------------- SET RALLY POSITION -----------------------------------------
bool Unit::setRallyPoint(Unit target) const
{
  return issueCommand(UnitCommand::setRallyPoint(getID(), target.getID()));
}
bool Unit::setRallyPoint(Position target) const
{
  return issueCommand(UnitCommand::setRallyPoint(getID(), target));
}
//--------------------------------------------- MOVE -------------------------------------------------------
bool Unit::move(Position target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::move(getID(),target, shiftQueueCommand));
}
//--------------------------------------------- PATROL -----------------------------------------------------
bool Unit::patrol(Position target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::patrol(getID(),target, shiftQueueCommand));
}
//--------------------------------------------- HOLD POSITION ----------------------------------------------
bool Unit::holdPosition(bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::holdPosition(getID(), shiftQueueCommand));
}
//--------------------------------------------- STOP -------------------------------------------------------
bool Unit::stop(bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::stop(getID(), shiftQueueCommand));
}
//--------------------------------------------- FOLLOW -----------------------------------------------------
bool Unit::follow(Unit target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::follow(getID(),target.getID(), shiftQueueCommand));
}
//--------------------------------------------- GATHER -----------------------------------------------------
bool Unit::gather(Unit target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::gather(getID(),target.getID(), shiftQueueCommand));
}
//--------------------------------------------- RETURN CARGO -----------------------------------------------
bool Unit::returnCargo(bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::returnCargo(getID(), shiftQueueCommand));
}
//--------------------------------------------- REPAIR -----------------------------------------------------
bool Unit::repair(Unit target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::repair(getID(),target.getID(), shiftQueueCommand));
}
//--------------------------------------------- BURROW -----------------------------------------------------
bool Unit::burrow() const
{
  return issueCommand(UnitCommand::burrow(getID()));
}
//--------------------------------------------- UNBURROW ---------------------------------------------------
bool Unit::unburrow() const
{
  return issueCommand(UnitCommand::unburrow(getID()));
}
//--------------------------------------------- CLOAK ------------------------------------------------------
bool Unit::cloak() const
{
  return issueCommand(UnitCommand::cloak(getID()));
}
//--------------------------------------------- DECLOAK ----------------------------------------------------
bool Unit::decloak() const
{
  return issueCommand(UnitCommand::decloak(getID()));
}
//--------------------------------------------- SIEGE ------------------------------------------------------
bool Unit::siege() const
{
  return issueCommand(UnitCommand::siege(getID()));
}
//--------------------------------------------- UNSIEGE ----------------------------------------------------
bool Unit::unsiege() const
{
  return issueCommand(UnitCommand::unsiege(getID()));
}
//--------------------------------------------- LIFT -------------------------------------------------------
bool Unit::lift() const
{
  return issueCommand(UnitCommand::lift(getID()));
}
//--------------------------------------------- LAND -------------------------------------------------------
bool Unit::land(TilePosition target) const
{
  return issueCommand(UnitCommand::land(getID(),target));
}
//--------------------------------------------- LOAD -------------------------------------------------------
bool Unit::load(Unit target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::load(getID(),target.getID(), shiftQueueCommand));
}
//--------------------------------------------- UNLOAD -----------------------------------------------------
bool Unit::unload(Unit target) const
{
  return issueCommand(UnitCommand::unload(getID(),target.getID()));
}
//--------------------------------------------- UNLOAD ALL -------------------------------------------------
bool Unit::unloadAll(bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::unloadAll(getID(), shiftQueueCommand));
}
//--------------------------------------------- UNLOAD ALL -------------------------------------------------
bool Unit::unloadAll(Position target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::unloadAll(getID(),target, shiftQueueCommand));
}
//--------------------------------------------- RIGHT CLICK ------------------------------------------------
bool Unit::rightClick(Position target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::rightClick(getID(),target, shiftQueueCommand));
}
bool Unit::rightClick(Unit target, bool shiftQueueCommand) const
{
  return issueCommand(UnitCommand::rightClick(getID(), target.getID(), shiftQueueCommand));
}
//--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
bool Unit::haltConstruction() const
{
  return issueCommand(UnitCommand::haltConstruction(getID()));
}
//--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
bool Unit::cancelConstruction() const
{
  return issueCommand(UnitCommand::cancelConstruction(getID()));
}
//--------------------------------------------- CANCEL ADDON -----------------------------------------------
bool Unit::cancelAddon() const
{
  return issueCommand(UnitCommand::cancelAddon(getID()));
}
//--------------------------------------------- CANCEL TRAIN -----------------------------------------------
bool Unit::cancelTrain(int slot) const
{
  return issueCommand(UnitCommand::cancelTrain(getID(), slot));
}
//--------------------------------------------- CANCEL MORPH -----------------------------------------------
bool Unit::cancelMorph() const
{
  return issueCommand(UnitCommand::cancelMorph(getID()));
}
//--------------------------------------------- CANCEL RESEARCH --------------------------------------------
bool Unit::cancelResearch() const
{
  return issueCommand(UnitCommand::cancelResearch(getID()));
}
//--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
bool Unit::cancelUpgrade() const
{
  return issueCommand(UnitCommand::cancelUpgrade(getID()));
}
//--------------------------------------------- USE TECH ---------------------------------------------------
bool Unit::useTech(TechType tech, Position target) const
{
  return issueCommand(UnitCommand::useTech(getID(), tech, target));
}
bool Unit::useTech(TechType tech, Unit target) const
{
  return issueCommand(UnitCommand::useTech(getID(), tech, target.getID()));
}
bool Unit::useTech(TechType tech) const
{
  return issueCommand(UnitCommand::useTech(getID(), tech));
}
//--------------------------------------------- PLACE COP --------------------------------------------------
bool Unit::placeCOP(TilePosition target) const
{
  return issueCommand(UnitCommand::placeCOP(getID(), target));
}
