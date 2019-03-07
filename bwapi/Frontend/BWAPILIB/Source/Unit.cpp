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
  
Player Unit::getPlayer() const
{
  return *getGame().getPlayerData(getData().player);
}

Unit Unit::getBuildUnit() const
{
  return *getGame().getUnitData(getData().buildUnit);
}

Unit Unit::getTarget() const
{
  return *getGame().getUnitData(getData().target);
}

Unit Unit::getOrderTarget() const
{
  return *getGame().getUnitData(getData().orderTarget);
}

Unit Unit::getRallyUnit() const
{
  return *getGame().getUnitData(getData().rallyUnit);
}

Unit Unit::getAddon() const
{
  return *getGame().getUnitData(getData().addon);
}

Unit Unit::getNydusExit() const
{
  return *getGame().getUnitData(getData().nydusExit);
}

Unit Unit::getPowerUp() const
{
  return *getGame().getUnitData(getData().powerUp);
}

Unit Unit::getTransport() const
{
  return *getGame().getUnitData(getData().transport);
}

//--------------------------------------------- ETC ----------------------------------
BWAPI::Unitset Unit::getLoadedUnits() const {
  Unitset units;
  for (auto &id : getData().loadedUnits)
    units.emplace(*getGame().getUnitData(id));
  return units;
}

int Unit::getSpaceRemaining() const {
  int space = getType().spaceProvided();

  // Decrease the space for each loaded unit
  for (auto &loadedUnit : getLoadedUnits())
    space -= loadedUnit.getType().spaceRequired();

  return space;
}

Unit Unit::getCarrier() const
{
  return *getGame().getUnitData(getData().carrier);
}

BWAPI::Unitset Unit::getInterceptors() const {
  Unitset ret;
  for (auto &id : getData().interceptors) {
    ret.emplace(*getGame().getUnitData(id));
  }
  return ret;
}

Unit Unit::getHatchery() const
{
  return *getGame().getUnitData(getData().hatchery);
}

BWAPI::Unitset Unit::getLarva() const {
  Unitset ret;
  for (auto &id : getData().larva) {
    ret.emplace(*getGame().getUnitData(id));
  }
  return ret;
}

UnitCommand Unit::getLastCommand() const { return getData().lastCommand; }

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
    return nullptr;
    
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

  // compute x distance
  int xDist = this->getLeft() - target.x;
  if (xDist < 0)
  {
    xDist = target.x - (this->getRight() + 1);
    if (xDist < 0)
      xDist = 0;
  }

  // compute y distance
  int yDist = this->getTop() - target.y;
  if (yDist < 0)
  {
    yDist = target.y - (this->getBottom() + 1);
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
  return player && iplayer < 9 && getData().isVisible[iplayer];
}
bool Unit::isVisible() const {
  return isVisible(getGame().self());
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
  return issueCommand(UnitCommand::gather(getID(), target.getID(), shiftQueueCommand));
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
bool Unit::issueCommand(UnitCommand command) const
{
  // TODO: Fix up the commented out code regarding const dataptr and latcom.
  if (!canIssueCommand(command))
    return false;

  command.unit = this->getID();
  // If using train or morph on a hatchery, automatically switch selection to larva
  // (assuming canIssueCommand ensures that there is a larva)
  if ((command.type == UnitCommandTypes::Train ||
    command.type == UnitCommandTypes::Morph) &&
    getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva)
  {
    Unitset larvae(this->getLarva());
    for (Unit larva : larvae)
    {
      if (!larva->isConstructing() && larva->isCompleted() && larva->canCommand())
      {
        command.unit = larva.getID();
        break;
      }
    }
    if (command.unit == this->getID())
      return false;
  }
  //dataptr->lastCommand = command;
  //dataptr->lastCommandFrame = this->getGame().getFrameCount();

  return getGame().issueCommand(Unitset{ *this }, command);

  //Start of latCom work below:
  //Proceed if latcom is enabled or if command is not queued.
  /*if (getGame().isLatComEnabled() || !command.isQueued())
  {
    bool adjustUnit = true;
    if (getGame().getRemainingLatencyFrames() == 0)
    {
      switch (command.type)
      {
      case UnitCommandTypes::Morph:
      case UnitCommandTypes::Build_Addon:
      case UnitCommandTypes::Train:
        if (command.type != UnitCommandTypes::Halt_Construction)
          break;
        adjustUnit = false;
        break;
      default:
        adjustUnit = false;
      }
    }
    // Existence test
    if (!this->exists)
      adjustUnit = false;
    if (adjustUnit)
    {
      // Get the player (usually the unit's owner)
      auto player = this->getPlayer();


    }
  }*/
}

bool Unit::canIssueCommand(UnitCommand command, bool checkCanUseTechPositionOnPositions, bool checkCanUseTechUnitOnUnits, bool checkCanBuildUnitType, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  BWAPI::UnitCommandType ct = command.type;
  if (checkCanIssueCommandType && !canIssueCommandType(ct, false))
    return false;

  switch (ct)
  {
  case UnitCommandTypes::Enum::Attack_Move:
    return true;

  case UnitCommandTypes::Enum::Attack_Unit:
    return canAttackUnit(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Build:
    return canBuild(command.getUnitType(), BWAPI::TilePosition(command.x, command.y), checkCanBuildUnitType, false, false);

  case UnitCommandTypes::Enum::Build_Addon:
    return canBuildAddon(command.getUnitType(), false, false);

  case UnitCommandTypes::Enum::Train:
    return canTrain(command.getUnitType(), false, false);

  case UnitCommandTypes::Enum::Morph:
    return canMorph(command.getUnitType(), false, false);

  case UnitCommandTypes::Enum::Research:
    return getGame()->canResearch(command.getTechType(), *this, false);

  case UnitCommandTypes::Enum::Upgrade:
    return getGame()->canUpgrade(command.getUpgradeType(), *this, false);

  case UnitCommandTypes::Enum::Set_Rally_Position:
    return true;

  case UnitCommandTypes::Enum::Set_Rally_Unit:
    return canSetRallyUnit(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Move:
    return true;

  case UnitCommandTypes::Enum::Patrol:
    return true;

  case UnitCommandTypes::Enum::Hold_Position:
    return true;

  case UnitCommandTypes::Enum::Stop:
    return true;

  case UnitCommandTypes::Enum::Follow:
    return canFollow(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Gather:
    return canGather(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Return_Cargo:
    return true;

  case UnitCommandTypes::Enum::Repair:
    return canRepair(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Burrow:
    return true;

  case UnitCommandTypes::Enum::Unburrow:
    return true;

  case UnitCommandTypes::Enum::Cloak:
    return true;

  case UnitCommandTypes::Enum::Decloak:
    return true;

  case UnitCommandTypes::Enum::Siege:
    return true;

  case UnitCommandTypes::Enum::Unsiege:
    return true;

  case UnitCommandTypes::Enum::Lift:
    return true;

  case UnitCommandTypes::Enum::Land:
    return canLand(BWAPI::TilePosition(command.x, command.y), false, false);

  case UnitCommandTypes::Enum::Load:
    return canLoad(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Unload:
    return canUnload(getGame()->getUnit(command.target), checkCanTargetUnit, false, false, false);

  case UnitCommandTypes::Enum::Unload_All:
    return true;

  case UnitCommandTypes::Enum::Unload_All_Position:
    return canUnloadAllPosition(command.getTargetPosition(), false, false);

  case UnitCommandTypes::Enum::Right_Click_Position:
    return true;

  case UnitCommandTypes::Enum::Right_Click_Unit:
    return canRightClickUnit(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Halt_Construction:
    return true;

  case UnitCommandTypes::Enum::Cancel_Construction:
    return true;

  case UnitCommandTypes::Enum::Cancel_Addon:
    return true;

  case UnitCommandTypes::Enum::Cancel_Train:
    return true;

  case UnitCommandTypes::Enum::Cancel_Train_Slot:
    return canCancelTrainSlot(command.extra, false, false);

  case UnitCommandTypes::Enum::Cancel_Morph:
    return true;

  case UnitCommandTypes::Enum::Cancel_Research:
    return true;

  case UnitCommandTypes::Enum::Cancel_Upgrade:
    return true;

  case UnitCommandTypes::Enum::Use_Tech:
    return canUseTechWithoutTarget(command.getTechType(), false, false);

  case UnitCommandTypes::Enum::Use_Tech_Unit:
    return canUseTechUnit(command.getTechType(), getGame()->getUnit(command.target), checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

  case UnitCommandTypes::Enum::Use_Tech_Position:
    return canUseTechPosition(command.getTechType(), command.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

  case UnitCommandTypes::Enum::Place_COP:
    return canPlaceCOP(BWAPI::TilePosition(command.x, command.y), false, false);
  }

  return true;
}

bool Unit::canIssueCommandGrouped(UnitCommand command, bool checkCanUseTechPositionOnPositions, bool checkCanUseTechUnitOnUnits, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  BWAPI::UnitCommandType ct = command.type;
  if (checkCanIssueCommandType && !canIssueCommandTypeGrouped(ct, false, false))
    return false;

  switch (ct)
  {
  case UnitCommandTypes::Enum::Attack_Move:
    return true;

  case UnitCommandTypes::Enum::Attack_Unit:
    return canAttackUnitGrouped(getGame()->getUnit(command.target), checkCanTargetUnit, false, false, false);

  case UnitCommandTypes::Enum::Build:
    return false;

  case UnitCommandTypes::Enum::Build_Addon:
    return false;

  case UnitCommandTypes::Enum::Train:
    return canTrain(command.getUnitType(), false, false);

  case UnitCommandTypes::Enum::Morph:
    return canMorph(command.getUnitType(), false, false);

  case UnitCommandTypes::Enum::Research:
    return false;

  case UnitCommandTypes::Enum::Upgrade:
    return false;

  case UnitCommandTypes::Enum::Set_Rally_Position:
    return false;

  case UnitCommandTypes::Enum::Set_Rally_Unit:
    return false;

  case UnitCommandTypes::Enum::Move:
    return true;

  case UnitCommandTypes::Enum::Patrol:
    return true;

  case UnitCommandTypes::Enum::Hold_Position:
    return true;

  case UnitCommandTypes::Enum::Stop:
    return true;

  case UnitCommandTypes::Enum::Follow:
    return canFollow(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Gather:
    return canGather(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Return_Cargo:
    return true;

  case UnitCommandTypes::Enum::Repair:
    return canRepair(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Burrow:
    return true;

  case UnitCommandTypes::Enum::Unburrow:
    return true;

  case UnitCommandTypes::Enum::Cloak:
    return true;

  case UnitCommandTypes::Enum::Decloak:
    return true;

  case UnitCommandTypes::Enum::Siege:
    return true;

  case UnitCommandTypes::Enum::Unsiege:
    return true;

  case UnitCommandTypes::Enum::Lift:
    return false;

  case UnitCommandTypes::Enum::Land:
    return false;

  case UnitCommandTypes::Enum::Load:
    return canLoad(getGame()->getUnit(command.target), checkCanTargetUnit, false, false);

  case UnitCommandTypes::Enum::Unload:
    return false;

  case UnitCommandTypes::Enum::Unload_All:
    return false;

  case UnitCommandTypes::Enum::Unload_All_Position:
    return canUnloadAllPosition(command.getTargetPosition(), false, false);

  case UnitCommandTypes::Enum::Right_Click_Position:
    return true;

  case UnitCommandTypes::Enum::Right_Click_Unit:
    return canRightClickUnitGrouped(getGame()->getUnit(command.target), checkCanTargetUnit, false, false, false);

  case UnitCommandTypes::Enum::Halt_Construction:
    return true;

  case UnitCommandTypes::Enum::Cancel_Construction:
    return false;

  case UnitCommandTypes::Enum::Cancel_Addon:
    return false;

  case UnitCommandTypes::Enum::Cancel_Train:
    return false;

  case UnitCommandTypes::Enum::Cancel_Train_Slot:
    return false;

  case UnitCommandTypes::Enum::Cancel_Morph:
    return true;

  case UnitCommandTypes::Enum::Cancel_Research:
    return false;

  case UnitCommandTypes::Enum::Cancel_Upgrade:
    return false;

  case UnitCommandTypes::Enum::Use_Tech:
    return canUseTechWithoutTarget(command.getTechType(), false, false);

  case UnitCommandTypes::Enum::Use_Tech_Unit:
    return canUseTechUnit(command.getTechType(), getGame()->getUnit(command.target), checkCanTargetUnit, checkCanUseTechUnitOnUnits, false, false);

  case UnitCommandTypes::Enum::Use_Tech_Position:
    return canUseTechPosition(command.getTechType(), command.getTargetPosition(), checkCanUseTechPositionOnPositions, false, false);

  case UnitCommandTypes::Enum::Place_COP:
    return false;
  }

  return true;
}

bool Unit::canCommand() const
{
  // Basic header
  getGame().setLastError();
  if (this->getPlayer() != getGame().self())
    return getGame().setLastError(Errors::Unit_Not_Owned);

  if (!this->exists())
    return getGame().setLastError(Errors::Unit_Does_Not_Exist);

  // Global can be ordered check
  if (this->isLockedDown() ||
    this->isMaelstrommed() ||
    this->isStasised() ||
    !this->isPowered() ||
    this->getOrder() == Orders::ZergBirth ||
    this->isLoaded())
  {
    if (!this->getType().producesLarva())
    {
      return getGame().setLastError(Errors::Unit_Busy);
    }
    else
    {
      Unitset larvae(this->getLarva());
      for (Unit larva : larvae)
      {
        if (larva.canCommand())
          return getGame().setLastError();
      }
      return getGame().setLastError(Errors::Unit_Busy);
    }
  }

  UnitType uType = this->getType();
  if (uType == UnitTypes::Protoss_Interceptor ||
    uType == UnitTypes::Terran_Vulture_Spider_Mine ||
    uType == UnitTypes::Spell_Scanner_Sweep ||
    uType == UnitTypes::Special_Map_Revealer)
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->isCompleted() &&
    (uType == UnitTypes::Protoss_Pylon ||
      uType == UnitTypes::Terran_Supply_Depot ||
      uType.isResourceContainer() ||
      uType == UnitTypes::Protoss_Shield_Battery ||
      uType == UnitTypes::Terran_Nuclear_Missile ||
      uType.isPowerup() ||
      (uType.isSpecialBuilding() && !uType.isFlagBeacon())))
    return getGame().setLastError(Errors::Incompatible_State);
  if (!this->isCompleted() &&
    !uType.isBuilding() &&
    !this->isMorphing())
    return getGame().setLastError(Errors::Incompatible_State);

  return getGame().setLastError();
}

bool Unit::canCommandGrouped(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getType().isBuilding() || this->getType().isCritter())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canIssueCommandType(UnitCommandType ct, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  switch (ct)
  {
  case UnitCommandTypes::Enum::Attack_Move:
    return canAttackMove(false);

  case UnitCommandTypes::Enum::Attack_Unit:
    return canAttackUnit(false);

  case UnitCommandTypes::Enum::Build:
    return canBuild(false);

  case UnitCommandTypes::Enum::Build_Addon:
    return canBuildAddon(false);

  case UnitCommandTypes::Enum::Train:
    return canTrain(false);

  case UnitCommandTypes::Enum::Morph:
    return canMorph(false);

  case UnitCommandTypes::Enum::Research:
    return canResearch(false);

  case UnitCommandTypes::Enum::Upgrade:
    return canUpgrade(false);

  case UnitCommandTypes::Enum::Set_Rally_Position:
    return canSetRallyPosition(false);

  case UnitCommandTypes::Enum::Set_Rally_Unit:
    return canSetRallyUnit(false);

  case UnitCommandTypes::Enum::Move:
    return canMove(false);

  case UnitCommandTypes::Enum::Patrol:
    return canPatrol(false);

  case UnitCommandTypes::Enum::Hold_Position:
    return canHoldPosition(false);

  case UnitCommandTypes::Enum::Stop:
    return canStop(false);

  case UnitCommandTypes::Enum::Follow:
    return canFollow(false);

  case UnitCommandTypes::Enum::Gather:
    return canGather(false);

  case UnitCommandTypes::Enum::Return_Cargo:
    return canReturnCargo(false);

  case UnitCommandTypes::Enum::Repair:
    return canRepair(false);

  case UnitCommandTypes::Enum::Burrow:
    return canBurrow(false);

  case UnitCommandTypes::Enum::Unburrow:
    return canUnburrow(false);

  case UnitCommandTypes::Enum::Cloak:
    return canCloak(false);

  case UnitCommandTypes::Enum::Decloak:
    return canDecloak(false);

  case UnitCommandTypes::Enum::Siege:
    return canSiege(false);

  case UnitCommandTypes::Enum::Unsiege:
    return canUnsiege(false);

  case UnitCommandTypes::Enum::Lift:
    return canLift(false);

  case UnitCommandTypes::Enum::Land:
    return canLand(false);

  case UnitCommandTypes::Enum::Load:
    return canLoad(false);

  case UnitCommandTypes::Enum::Unload:
    return canUnload(false);

  case UnitCommandTypes::Enum::Unload_All:
    return canUnloadAll(false);

  case UnitCommandTypes::Enum::Unload_All_Position:
    return canUnloadAllPosition(false);

  case UnitCommandTypes::Enum::Right_Click_Position:
    return canRightClickPosition(false);

  case UnitCommandTypes::Enum::Right_Click_Unit:
    return canRightClickUnit(false);

  case UnitCommandTypes::Enum::Halt_Construction:
    return canHaltConstruction(false);

  case UnitCommandTypes::Enum::Cancel_Construction:
    return canCancelConstruction(false);

  case UnitCommandTypes::Enum::Cancel_Addon:
    return canCancelAddon(false);

  case UnitCommandTypes::Enum::Cancel_Train:
    return canCancelTrain(false);

  case UnitCommandTypes::Enum::Cancel_Train_Slot:
    return canCancelTrainSlot(false);

  case UnitCommandTypes::Enum::Cancel_Morph:
    return canCancelMorph(false);

  case UnitCommandTypes::Enum::Cancel_Research:
    return canCancelResearch(false);

  case UnitCommandTypes::Enum::Cancel_Upgrade:
    return canCancelUpgrade(false);

  case UnitCommandTypes::Enum::Use_Tech:
  case UnitCommandTypes::Enum::Use_Tech_Unit:
  case UnitCommandTypes::Enum::Use_Tech_Position:
    return canUseTechWithOrWithoutTarget(false);

  case UnitCommandTypes::Enum::Place_COP:
    return canPlaceCOP(false);
  }

  return true;
}

bool Unit::canIssueCommandTypeGrouped(UnitCommandType ct, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  switch (ct)
  {
  case UnitCommandTypes::Enum::Attack_Move:
    return canAttackMoveGrouped(false, false);

  case UnitCommandTypes::Enum::Attack_Unit:
    return canAttackUnitGrouped(false, false);

  case UnitCommandTypes::Enum::Build:
    return false;

  case UnitCommandTypes::Enum::Build_Addon:
    return false;

  case UnitCommandTypes::Enum::Train:
    return canTrain(false);

  case UnitCommandTypes::Enum::Morph:
    return canMorph(false);

  case UnitCommandTypes::Enum::Research:
    return false;

  case UnitCommandTypes::Enum::Upgrade:
    return false;

  case UnitCommandTypes::Enum::Set_Rally_Position:
    return false;

  case UnitCommandTypes::Enum::Set_Rally_Unit:
    return false;

  case UnitCommandTypes::Enum::Move:
    return canMoveGrouped(false, false);

  case UnitCommandTypes::Enum::Patrol:
    return canPatrolGrouped(false, false);

  case UnitCommandTypes::Enum::Hold_Position:
    return canHoldPosition(false);

  case UnitCommandTypes::Enum::Stop:
    return canStop(false);

  case UnitCommandTypes::Enum::Follow:
    return canFollow(false);

  case UnitCommandTypes::Enum::Gather:
    return canGather(false);

  case UnitCommandTypes::Enum::Return_Cargo:
    return canReturnCargo(false);

  case UnitCommandTypes::Enum::Repair:
    return canRepair(false);

  case UnitCommandTypes::Enum::Burrow:
    return canBurrow(false);

  case UnitCommandTypes::Enum::Unburrow:
    return canUnburrow(false);

  case UnitCommandTypes::Enum::Cloak:
    return canCloak(false);

  case UnitCommandTypes::Enum::Decloak:
    return canDecloak(false);

  case UnitCommandTypes::Enum::Siege:
    return canSiege(false);

  case UnitCommandTypes::Enum::Unsiege:
    return canUnsiege(false);

  case UnitCommandTypes::Enum::Lift:
    return false;

  case UnitCommandTypes::Enum::Land:
    return false;

  case UnitCommandTypes::Enum::Load:
    return canLoad(false);

  case UnitCommandTypes::Enum::Unload:
    return false;

  case UnitCommandTypes::Enum::Unload_All:
    return false;

  case UnitCommandTypes::Enum::Unload_All_Position:
    return canUnloadAllPosition(false);

  case UnitCommandTypes::Enum::Right_Click_Position:
    return canRightClickPositionGrouped(false, false);

  case UnitCommandTypes::Enum::Right_Click_Unit:
    return canRightClickUnitGrouped(false, false);

  case UnitCommandTypes::Enum::Halt_Construction:
    return canHaltConstruction(false);

  case UnitCommandTypes::Enum::Cancel_Construction:
    return false;

  case UnitCommandTypes::Enum::Cancel_Addon:
    return false;

  case UnitCommandTypes::Enum::Cancel_Train:
    return false;

  case UnitCommandTypes::Enum::Cancel_Train_Slot:
    return false;

  case UnitCommandTypes::Enum::Cancel_Morph:
    return canCancelMorph(false);

  case UnitCommandTypes::Enum::Cancel_Research:
    return false;

  case UnitCommandTypes::Enum::Cancel_Upgrade:
    return false;

  case UnitCommandTypes::Enum::Use_Tech:
  case UnitCommandTypes::Enum::Use_Tech_Unit:
  case UnitCommandTypes::Enum::Use_Tech_Position:
    return canUseTechWithOrWithoutTarget(false);

  case UnitCommandTypes::Enum::Place_COP:
    return false;
  }

  return true;
}

bool Unit::canTargetUnit(Unit targetUnit, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!targetUnit || !targetUnit->exists())
    return getGame().setLastError(Errors::Unit_Does_Not_Exist);

  if (!targetUnit->isVisible())
    return getGame().setLastError(Errors::Unit_Not_Visible);

  if (!targetUnit->isCompleted() &&
    !targetUnit->getType().isBuilding() &&
    !targetUnit->isMorphing() &&
    targetUnit->getType() != UnitTypes::Protoss_Archon &&
    targetUnit->getType() != UnitTypes::Protoss_Dark_Archon)
    return getGame().setLastError(Errors::Incompatible_State);

  if (targetUnit->getType() == UnitTypes::Spell_Scanner_Sweep ||
    targetUnit->getType() == UnitTypes::Spell_Dark_Swarm ||
    targetUnit->getType() == UnitTypes::Spell_Disruption_Web ||
    targetUnit->getType() == UnitTypes::Special_Map_Revealer)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canAttack(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canAttackMove(false) && !canAttackUnit(false))
    return false;

  return true;
}

bool Unit::canAttack(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canAttackMove(false))
    return false;

  return true;
}

bool Unit::canAttack(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (target == nullptr)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (!canAttackUnit(target, checkCanTargetUnit, checkCanIssueCommandType, false))
    return false;

  return true;
}

bool Unit::canAttackGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!canAttackMoveGrouped(false, false) && !canAttackUnitGrouped(false, false))
    return false;

  return true;
}

bool Unit::canAttackGrouped(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!canAttackMoveGrouped(false, false))
    return false;

  return true;
}

bool Unit::canAttackGrouped(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (target == nullptr)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (!canAttackUnitGrouped(target, checkCanTargetUnit, checkCommandibilityGrouped, false, false))
    return false;

  return true;
}

bool Unit::canAttackMove(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if ((this->getType() != UnitTypes::Terran_Medic && !canAttackUnit(false)) || !canMove(false))
    return false;

  return true;
}

bool Unit::canAttackMoveGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!this->getType().canMove() &&
    this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode &&
    this->getType() != UnitTypes::Zerg_Cocoon &&
    this->getType() != UnitTypes::Zerg_Lurker_Egg)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canAttackUnit(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->getType().groundWeapon() == WeaponTypes::None && this->getType().airWeapon() == WeaponTypes::None)
  {
    if (this->getType() == UnitTypes::Protoss_Carrier || this->getType() == UnitTypes::Hero_Gantrithor)
    {
      if (this->getInterceptorCount() <= 0)
        return getGame().setLastError(Errors::Unable_To_Hit);
    }
    else if (this->getType() == UnitTypes::Protoss_Reaver || this->getType() == UnitTypes::Hero_Warbringer)
    {
      if (this->getScarabCount() <= 0)
        return getGame().setLastError(Errors::Unable_To_Hit);
    }
    else
      return getGame().setLastError(Errors::Unable_To_Hit);
  }
  if (this->getType() == UnitTypes::Zerg_Lurker)
  {
    if (!this->isBurrowed())
      return getGame().setLastError(Errors::Unable_To_Hit);
  }
  else if (this->isBurrowed())
    return getGame().setLastError(Errors::Unable_To_Hit);
  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->getOrder() == Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canAttackUnit(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canAttackUnit(false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (targetUnit->isInvincible())
    return getGame().setLastError(Errors::Unable_To_Hit);

  UnitType type = this->getType();
  bool targetInAir = targetUnit->isFlying();
  WeaponType weapon = targetInAir ? type.airWeapon() : type.groundWeapon();

  if (weapon == WeaponTypes::None)
  {
    switch (type)
    {
    case UnitTypes::Enum::Protoss_Carrier:
    case UnitTypes::Enum::Hero_Gantrithor:
      break;
    case UnitTypes::Enum::Protoss_Reaver:
    case UnitTypes::Enum::Hero_Warbringer:
      if (targetInAir)
        getGame().setLastError(Errors::Unable_To_Hit);
      break;
    default:
      return getGame().setLastError(Errors::Unable_To_Hit);
    }
  }

  if (!type.canMove() && this->isInWeaponRange(targetUnit))
    return getGame().setLastError(Errors::Out_Of_Range);

  if (type == UnitTypes::Zerg_Lurker && !this->isInWeaponRange(targetUnit))
    return getGame().setLastError(Errors::Out_Of_Range);

  if (targetUnit == *this)
    return getGame().setLastError(Errors::Invalid_Parameter);

  return true;
}

bool Unit::canAttackUnitGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);

  if (!this->getType().canMove() &&
    this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);

  if (this->getType() == UnitTypes::Zerg_Lurker)
  {
    if (!this->isBurrowed())
      return getGame().setLastError(Errors::Unable_To_Hit);
  }
  else if (this->isBurrowed())
    return getGame().setLastError(Errors::Unable_To_Hit);

  if (this->getOrder() == Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canAttackUnitGrouped(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (checkCanIssueCommandType && !canAttackUnitGrouped(false, false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (targetUnit->isInvincible())
    return getGame().setLastError(Errors::Unable_To_Hit);

  if (this->getType() == UnitTypes::Zerg_Lurker && !this->isInWeaponRange(targetUnit))
    return getGame().setLastError(Errors::Out_Of_Range);

  if (this->getType() == UnitTypes::Zerg_Queen &&
    (targetUnit->getType() != UnitTypes::Terran_Command_Center ||
      targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0))
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (targetUnit == *this)
    return getGame().setLastError(Errors::Invalid_Parameter);

  return true;
}

bool Unit::canBuild(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->isConstructing() ||
    !this->isCompleted() ||
    (this->getType().isBuilding() && !this->isIdle()))
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canBuild(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canBuild(false))
    return false;

  if (!getGame().canMake(uType, *this))
    return false;

  if (!uType.isBuilding())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->getAddon())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canBuild(false))
    return false;

  if (checkTargetUnitType && !canBuild(uType, false, false))
    return false;
  
  if (getGame().isValid(tilePos) == false)
    return getGame().setLastError(Errors::Invalid_Tile_Position);

  if (!getGame().canBuildHere(tilePos, uType, *this, true))
    return false;

  return true;
}

bool Unit::canBuildAddon(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->isConstructing() ||
    !this->isCompleted() ||
    this->isLifted() ||
    (this->getType().isBuilding() && !this->isIdle()))
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->getAddon())
    return getGame().setLastError(Errors::Incompatible_State);
  if (!this->getType().canBuildAddon())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canBuildAddon(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canBuildAddon(false))
    return false;

  if (!getGame().canMake(uType, *this))
    return false;

  if (!uType.isAddon())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (!getGame().canBuildHere(this->getTilePosition(), uType, *this))
    return false;

  return true;
}

bool Unit::canTrain(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getType().producesLarva())
  {
    if (!this->isConstructing() && this->isCompleted())
      return getGame().setLastError();
    Unitset larvae(this->getLarva());
    for (Unit larva : larvae)
    {
      if (!larva->isConstructing() && larva->isCompleted() && larva.canCommand())
        return getGame().setLastError();
    }
    return getGame().setLastError(Errors::Unit_Busy);
  }

  if (this->isConstructing() ||
    !this->isCompleted() ||
    this->isLifted())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!this->getType().canProduce() &&
    this->getType() != UnitTypes::Enum::Terran_Nuclear_Silo &&
    this->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
    this->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
    this->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
    this->getType() != UnitTypes::Enum::Zerg_Spire &&
    this->getType() != UnitTypes::Enum::Zerg_Larva)
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canTrain(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  auto thisUnit = *this;
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canTrain(false))
    return false;

  if (this->getType().producesLarva())
  {
    if (uType.whatBuilds().first == UnitTypes::Zerg_Larva)
    {
      bool foundCommandableLarva = false;
      Unitset larvae(this->getLarva());
      for (Unit larva : larvae)
      {
        if (larva.canTrain(true))
        {
          foundCommandableLarva = true;
          thisUnit = larva;
          break;
        }
      }
      if (!foundCommandableLarva)
        return getGame().setLastError(Errors::Unit_Does_Not_Exist);
    }
    else if (this->isConstructing() ||
      !this->isCompleted())
      return getGame().setLastError(Errors::Unit_Busy);
  }

  if (!getGame().canMake(uType, thisUnit))
    return false;

  if (uType.isAddon() || (uType.isBuilding() && !thisUnit->getType().isBuilding()))
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canMorph(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getType().producesLarva())
  {
    if (!this->isConstructing() && this->isCompleted() && (!this->getType().isBuilding() || this->isIdle()))
      return getGame().setLastError();
    Unitset larvae(this->getLarva());
    for (Unit larva : larvae)
    {
      if (!larva->isConstructing() && larva->isCompleted() && larva->canCommand())
        return getGame().setLastError();
    }
    return getGame().setLastError(Errors::Unit_Busy);
  }

  if (this->isConstructing() ||
    !this->isCompleted() ||
    (this->getType().isBuilding() && !this->isIdle()))
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->getType() != UnitTypes::Enum::Zerg_Hydralisk &&
    this->getType() != UnitTypes::Enum::Zerg_Mutalisk &&
    this->getType() != UnitTypes::Enum::Zerg_Creep_Colony &&
    this->getType() != UnitTypes::Enum::Zerg_Spire &&
    this->getType() != UnitTypes::Enum::Zerg_Hatchery &&
    this->getType() != UnitTypes::Enum::Zerg_Lair &&
    this->getType() != UnitTypes::Enum::Zerg_Hive &&
    this->getType() != UnitTypes::Enum::Zerg_Larva)
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canMorph(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  auto thisUnit = *this;
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canMorph(false))
    return false;

  if (thisUnit->getType().producesLarva())
  {
    if (uType.whatBuilds().first == UnitTypes::Zerg_Larva)
    {
      bool foundCommandableLarva = false;
      Unitset larvae(thisUnit->getLarva());
      for (Unit larva : larvae)
      {
        if (larva->canMorph(true))
        {
          foundCommandableLarva = true;
          thisUnit = larva;
          break;
        }
      }
      if (!foundCommandableLarva)
        return getGame().setLastError(Errors::Unit_Does_Not_Exist);
    }
    else if (thisUnit->isConstructing() ||
      !thisUnit->isCompleted() ||
      (thisUnit->getType().isBuilding() && !thisUnit->isIdle()))
      return getGame().setLastError(Errors::Unit_Busy);
  }

  if (!getGame().canMake(uType, thisUnit))
    return false;
  if (uType == UnitTypes::Enum::Zerg_Larva || uType == UnitTypes::Enum::Zerg_Egg || uType == UnitTypes::Enum::Zerg_Cocoon)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canResearch(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canResearch(TechType type, bool checkCanIssueCommandType) const
{
  // Error checking
  if (!getGame().self())
    return getGame().setLastError(Errors::Unit_Not_Owned);

  if (this->getPlayer() != getGame().self())
    return getGame().setLastError(Errors::Unit_Not_Owned);

  if (!this->getType().isSuccessorOf(type.whatResearches()))
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (checkCanIssueCommandType && (this->isLifted() || !this->isIdle() || !this->isCompleted()))
    return getGame().setLastError(Errors::Unit_Busy);

  if (getGame().self()->isResearching(type))
    return getGame().setLastError(Errors::Currently_Researching);

  if (getGame().self()->hasResearched(type))
    return getGame().setLastError(Errors::Already_Researched);

  if (!getGame().self()->isResearchAvailable(type))
    return getGame().setLastError(Errors::Access_Denied);

  if (getGame().self()->minerals() < type.mineralPrice())
    return getGame().setLastError(Errors::Insufficient_Minerals);

  if (getGame().self()->gas() < type.gasPrice())
    return getGame().setLastError(Errors::Insufficient_Gas);

  if (!getGame().self()->hasUnitTypeRequirement(type.requiredUnit()))
    return getGame().setLastError(Errors::Insufficient_Tech);

  return getGame().setLastError();
}

bool Unit::canUpgrade(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canUpgrade(UpgradeType type, bool checkCanIssueCommandType) const
{
  Player self = getGame().self();
  if (!self)
    return getGame().setLastError(Errors::Unit_Not_Owned);

  if (this->getPlayer() != self)
    return getGame().setLastError(Errors::Unit_Not_Owned);

  if (!this->getType().isSuccessorOf(type.whatUpgrades()))
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (checkCanIssueCommandType && (this->isLifted() || !this->isIdle() || !this->isCompleted()))
    return getGame().setLastError(Errors::Unit_Busy);

  int nextLvl = self->getUpgradeLevel(type) + 1;

  if (!self->hasUnitTypeRequirement(type.whatUpgrades()))
    return getGame().setLastError(Errors::Unit_Does_Not_Exist);

  if (!self->hasUnitTypeRequirement(type.whatsRequired(nextLvl)))
    return getGame().setLastError(Errors::Insufficient_Tech);

  if (self->isUpgrading(type))
    return getGame().setLastError(Errors::Currently_Upgrading);

  if (self->getUpgradeLevel(type) >= self->getMaxUpgradeLevel(type))
    return getGame().setLastError(Errors::Fully_Upgraded);

  if (self->minerals() < type.mineralPrice(nextLvl))
    return getGame().setLastError(Errors::Insufficient_Minerals);

  if (self->gas() < type.gasPrice(nextLvl))
    return getGame().setLastError(Errors::Insufficient_Gas);

  return getGame().setLastError();
}

bool Unit::canSetRallyPoint(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canSetRallyPosition(false) && !canSetRallyUnit(false))
    return false;

  return true;
}

bool Unit::canSetRallyPoint(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
   getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canSetRallyPosition(false))
    return false;

  return true;
}

bool Unit::canSetRallyPoint(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (target == nullptr)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (!canSetRallyUnit(target, checkCanTargetUnit, checkCanIssueCommandType, false))
    return false;

  return true;
}

bool Unit::canSetRallyPosition(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().canProduce() || !this->getType().isBuilding())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (this->isLifted())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canSetRallyUnit(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().canProduce() || !this->getType().isBuilding())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (this->isLifted())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canSetRallyUnit(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canSetRallyUnit(false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  return true;
}

bool Unit::canMove(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding())
  {
    if (!this->isInterruptible())
      return getGame().setLastError(Errors::Unit_Busy);
    if (!this->getType().canMove())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (this->isBurrowed())
      return getGame().setLastError(Errors::Incompatible_State);
    if (this->getOrder() == Orders::ConstructingBuilding)
      return getGame().setLastError(Errors::Unit_Busy);
    if (this->getType() == UnitTypes::Zerg_Larva)
      return getGame().setLastError(Errors::Incompatible_UnitType);
  }
  else
  {
    if (!this->getType().isFlyingBuilding())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (!this->isLifted())
      return getGame().setLastError(Errors::Incompatible_State);
  }

  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canMoveGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!this->getType().canMove())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (!this->isCompleted() && !this->isMorphing())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canPatrol(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canMove(false))
    return false;

  return true;
}

bool Unit::canPatrolGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!canMoveGrouped(false, false))
    return false;

  return true;
}

bool Unit::canFollow(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canMove(false))
    return false;

  return true;
}

bool Unit::canFollow(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canFollow(false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit || targetUnit == *this)
    return getGame().setLastError(Errors::Invalid_Parameter);

  return true;
}

bool Unit::canGather(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!this->getType().isWorker())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->getPowerUp())
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->isBurrowed())
    return getGame().setLastError(Errors::Incompatible_State);
  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->getOrder() == Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canGather(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canGather(false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  UnitType uType = targetUnit->getType();
  if (!uType.isResourceContainer() || uType == UnitTypes::Resource_Vespene_Geyser)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (!targetUnit->isCompleted())
    return getGame().setLastError(Errors::Unit_Busy);

  if (!this->hasPath(targetUnit->getPosition()))
    return getGame().setLastError(Errors::Unreachable_Location);

  if (uType.isRefinery() && targetUnit->getPlayer() != getGame().self())
    return getGame().setLastError(Errors::Unit_Not_Owned);

  return true;
}

bool Unit::canReturnCargo(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!this->getType().isWorker())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (!this->isCarryingGas() && !this->isCarryingMinerals())
    return getGame().setLastError(Errors::Insufficient_Ammo);
  if (this->isBurrowed())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->getOrder() == Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canHoldPosition(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding())
  {
    if (!this->getType().canMove())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (this->isBurrowed() && this->getType() != UnitTypes::Zerg_Lurker)
      return getGame().setLastError(Errors::Incompatible_State);
    if (this->getOrder() == Orders::ConstructingBuilding)
      return getGame().setLastError(Errors::Unit_Busy);
    if (this->getType() == UnitTypes::Zerg_Larva)
      return getGame().setLastError(Errors::Incompatible_UnitType);
  }
  else
  {
    if (!this->getType().isFlyingBuilding())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (!this->isLifted())
      return getGame().setLastError(Errors::Incompatible_State);
  }

  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canStop(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->isBurrowed() && this->getType() != UnitTypes::Zerg_Lurker)
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->getType().isBuilding() && !this->isLifted() &&
    this->getType() != UnitTypes::Protoss_Photon_Cannon &&
    this->getType() != UnitTypes::Zerg_Sunken_Colony &&
    this->getType() != UnitTypes::Zerg_Spore_Colony &&
    this->getType() != UnitTypes::Terran_Missile_Turret)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canRepair(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->getType() != BWAPI::UnitTypes::Terran_SCV)
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->getOrder() == Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canRepair(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canRepair(false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  UnitType targType = targetUnit->getType();
  if (targType.getRace() != BWAPI::Races::Terran || !targType.isMechanical())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (targetUnit->getHitPoints() == targType.maxHitPoints())
    return getGame().setLastError(Errors::Incompatible_State);
  if (!targetUnit->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (targetUnit == *this)
    return getGame().setLastError(Errors::Invalid_Parameter);

  return true;
}

bool Unit::canBurrow(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canUseTechWithoutTarget(BWAPI::TechTypes::Burrowing, true, false))
    return false;

  return true;
}

bool Unit::canUnburrow(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBurrowable())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (!this->isBurrowed() || this->getOrder() == Orders::Unburrowing)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canCloak(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canUseTechWithoutTarget(this->getType().cloakingTech(), true, false))
    return false;

  return true;
}

bool Unit::canDecloak(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getType().cloakingTech() == TechTypes::None)
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->getSecondaryOrder() != Orders::Cloak)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canSiege(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canUseTechWithoutTarget(BWAPI::TechTypes::Tank_Siege_Mode, true, false))
    return false;

  return true;
}

bool Unit::canUnsiege(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->isSieged())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->getOrder() == Orders::Sieging || this->getOrder() == Orders::Unsieging)
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canLift(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isFlyingBuilding())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (this->isLifted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (!this->isIdle())
    return getGame().setLastError(Errors::Unit_Busy);

  return true;
}

bool Unit::canLand(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isFlyingBuilding())
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (!this->isLifted())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canLand(TilePosition target, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canLand(checkCommandibility))
    return false;

  if (!getGame().canBuildHere(target, this->getType(), nullptr, true))
    return false;

  return true;
}

bool Unit::canLoad(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!this->isCompleted())
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->getType() == UnitTypes::Zerg_Overlord && getGame().self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
    return getGame().setLastError(Errors::Insufficient_Tech);
  if (this->isBurrowed())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->getOrder() == Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Unit_Busy);
  if (this->getType() == UnitTypes::Zerg_Larva)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canLoad(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canLoad(false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  //target must also be owned by self
  if (targetUnit->getPlayer() != getGame().self())
    return getGame().setLastError(Errors::Unit_Not_Owned);

  if (targetUnit->isLoaded() || !targetUnit->isCompleted())
    return getGame().setLastError(Errors::Unit_Busy);

  // verify upgrade for Zerg Overlord
  if (targetUnit->getType() == UnitTypes::Zerg_Overlord && getGame().self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
    return getGame().setLastError(Errors::Insufficient_Tech);

  int thisSpaceProvided = this->getType().spaceProvided();
  int targetSpaceProvided = targetUnit->getType().spaceProvided();
  if (thisSpaceProvided <= 0 && targetSpaceProvided <= 0)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  const BWAPI::Unit unitToBeLoaded = (thisSpaceProvided > 0 ? targetUnit : *this);
  if (unitToBeLoaded->getType().canMove() == false || unitToBeLoaded->getType().isFlyer() || unitToBeLoaded->getType().spaceRequired() > 8)
    return getGame().setLastError(Errors::Incompatible_UnitType);
  if (!unitToBeLoaded->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (unitToBeLoaded->isBurrowed())
    return getGame().setLastError(Errors::Incompatible_State);

  const BWAPI::Unit unitThatLoads = (thisSpaceProvided > 0 ? *this : targetUnit);
  if (unitThatLoads->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (unitThatLoads->getType() == UnitTypes::Terran_Bunker)
  {
    if (!unitToBeLoaded->getType().isOrganic() || unitToBeLoaded->getType().getRace() != Races::Terran)
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (!unitToBeLoaded->hasPath(unitThatLoads->getPosition()))
      return getGame().setLastError(Errors::Unreachable_Location);
  }

  int freeSpace = (thisSpaceProvided > 0 ? thisSpaceProvided : targetSpaceProvided);
  Unitset loadedUnits = unitThatLoads->getLoadedUnits();
  for (Unit u : loadedUnits)
  {
    const int requiredSpace = u->getType().spaceRequired();
    if (requiredSpace > 0 && requiredSpace < 8)
      freeSpace -= requiredSpace;
  }
  if (unitToBeLoaded->getType().spaceRequired() > freeSpace)
    return getGame().setLastError(Errors::Insufficient_Space);

  return true;
}

bool Unit::canUnloadWithOrWithoutTarget(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);

  if (this->getLoadedUnits().size() == 0)
    return getGame().setLastError(Errors::Unit_Does_Not_Exist);

  // Check overlord tech
  if (this->getType() == UnitTypes::Zerg_Overlord && getGame().self()->getUpgradeLevel(UpgradeTypes::Ventral_Sacs) == 0)
    return getGame().setLastError(Errors::Insufficient_Tech);

  if (this->getType().spaceProvided() <= 0)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(false))
    return false;

  if (this->getType() != UnitTypes::Terran_Bunker)
  {
    if (getGame().isValid(WalkPosition(targDropPos.x / 8, targDropPos.y / 8)) == false)
      return getGame().setLastError(Errors::Invalid_Tile_Position);
    else if (!getGame().isWalkable(targDropPos.x / 8, targDropPos.y / 8))
      return getGame().setLastError(Errors::Unreachable_Location);
  }

  return true;
}

bool Unit::canUnload(bool checkCommandibility) const
{
  return canUnloadAtPosition(this->getPosition(), true, checkCommandibility);
}

bool Unit::canUnload(Unit targetUnit, bool checkCanTargetUnit, bool checkPosition, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUnloadWithOrWithoutTarget(false))
    return false;

  if (checkPosition && !canUnloadAtPosition(this->getPosition(), false, false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit->isLoaded())
    return getGame().setLastError(Errors::Incompatible_State);

  if (targetUnit->getTransport() != *this)
    return getGame().setLastError(Errors::Invalid_Parameter);

  return true;
}

bool Unit::canUnloadAll(bool checkCommandibility) const
{
  return canUnloadAtPosition(this->getPosition(), true, checkCommandibility);
}

bool Unit::canUnloadAllPosition(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canUnloadWithOrWithoutTarget(false))
    return false;

  if (this->getType() == UnitTypes::Terran_Bunker)
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUnloadAllPosition(false))
    return false;

  if (!canUnloadAtPosition(targDropPos, false, false))
    return false;

  return true;
}

bool Unit::canRightClick(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canRightClickPosition(false) && !canRightClickUnit(false))
    return false;

  return true;
}

bool Unit::canRightClick(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canRightClickPosition(false))
    return false;

  return true;
}

bool Unit::canRightClick(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (target == nullptr)
    return getGame().setLastError(Errors::Invalid_Parameter);
  if (!canRightClickUnit(target, checkCanTargetUnit, checkCanIssueCommandType, false))
    return false;

  return true;
}

bool Unit::canRightClickGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!canRightClickPositionGrouped(false, false) && !canRightClickUnitGrouped(false, false))
    return false;

  return true;
}

bool Unit::canRightClickGrouped(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!canRightClickPositionGrouped(false, false))
    return false;

  return true;
}

bool Unit::canRightClickGrouped(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (target == nullptr)
    return getGame().setLastError(Errors::Invalid_Parameter);
  if (!canRightClickUnitGrouped(target, checkCanTargetUnit, checkCanIssueCommandType, false, false))
    return false;

  return true;
}

bool Unit::canRightClickPosition(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!canMove(false) && !canSetRallyPosition(false))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canRightClickPositionGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!canMoveGrouped(false, false))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canRightClickUnit(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!canFollow(false) &&
    !canAttackUnit(false) &&
    !canLoad(false) &&
    !canSetRallyUnit(false))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canRightClickUnit(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canRightClickUnit(false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  if (!targetUnit->getPlayer()->isNeutral() && this->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
    !canAttackUnit(targetUnit, false, true, false))
    return false;

  if (!canFollow(targetUnit, false, true, false) &&
    !canLoad(targetUnit, false, true, false) &&
    !canSetRallyUnit(targetUnit, false, true, false))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canRightClickUnitGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (!this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!canFollow(false) &&
    !canAttackUnitGrouped(false, false) &&
    !canLoad(false))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canRightClickUnitGrouped(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCommandibilityGrouped && !canCommandGrouped(false))
    return false;

  if (checkCanIssueCommandType && !canRightClickUnitGrouped(false, false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit)
    getGame().setLastError(Errors::Invalid_Parameter);

  if (!targetUnit->getPlayer()->isNeutral() && this->getPlayer()->isEnemy(targetUnit->getPlayer()) &&
    !canAttackUnitGrouped(targetUnit, false, true, false, false))
    return false;

  if (!canFollow(targetUnit, false, true, false) &&
    !canLoad(targetUnit, false, true, false))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canHaltConstruction(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getOrder() != Orders::ConstructingBuilding)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canCancelConstruction(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (this->isCompleted() || (this->getType() == UnitTypes::Zerg_Nydus_Canal && this->getNydusExit()))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canCancelAddon(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getAddon() || this->getAddon()->isCompleted())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canCancelTrain(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->isTraining())
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canCancelTrainSlot(bool checkCommandibility) const
{
  return canCancelTrain(checkCommandibility);
}

bool Unit::canCancelTrainSlot(int slot, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canCancelTrainSlot(false))
    return false;

  if (!this->isTraining() || (this->getTrainingQueue().size() <= (unsigned int)slot && slot >= 0))
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canCancelMorph(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->isMorphing() || (!this->isCompleted() && this->getType() == UnitTypes::Zerg_Nydus_Canal && this->getNydusExit()))
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canCancelResearch(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getOrder() != Orders::ResearchTech)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canCancelUpgrade(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (this->getOrder() != Orders::Upgrade)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canUseTechWithOrWithoutTarget(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isBuilding() && !this->isInterruptible())
    return getGame().setLastError(Errors::Unit_Busy);
  if (!this->isCompleted())
    return getGame().setLastError(Errors::Incompatible_State);
  if (this->isHallucination())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  return true;
}

bool Unit::canUseTechWithOrWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(false))
    return false;

  // researched check
  if (!this->getType().isHero() && !getGame().self()->hasResearched(tech) && this->getType() != UnitTypes::Zerg_Lurker)
    return getGame().setLastError(Errors::Insufficient_Tech);

  // energy check
  if (this->getEnergy() < tech.energyCost())
    return getGame().setLastError(Errors::Insufficient_Energy);

  // unit check
  if (tech != TechTypes::Burrowing && !tech.whatUses().contains(this->getType()))
    return getGame().setLastError(Errors::Incompatible_UnitType);

  switch (tech)
  {
  case TechTypes::Enum::Spider_Mines:
    if (this->getSpiderMineCount() <= 0)
      return getGame().setLastError(Errors::Insufficient_Ammo);
    return true;

  case TechTypes::Enum::Tank_Siege_Mode:
    if (this->getOrder() == Orders::Sieging || this->getOrder() == Orders::Unsieging)
      return getGame().setLastError(Errors::Unit_Busy);
    return true;

  case TechTypes::Enum::Cloaking_Field:
  case TechTypes::Enum::Personnel_Cloaking:
    if (this->getSecondaryOrder() == Orders::Cloak)
      return getGame().setLastError(Errors::Incompatible_State);
    return true;

  case TechTypes::Enum::Burrowing:
    if (!this->getType().isBurrowable())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (this->isBurrowed() || this->getOrder() == Orders::Burrowing || this->getOrder() == Orders::Unburrowing)
      return getGame().setLastError(Errors::Incompatible_State);
    return true;

  case TechTypes::Enum::None:
    return getGame().setLastError(Errors::Incompatible_TechType);

  case TechTypes::Enum::Nuclear_Strike:
    if (this->getPlayer()->completedUnitCount(UnitTypes::Terran_Nuclear_Missile) == 0)
      return getGame().setLastError(Errors::Insufficient_Ammo);
    return true;

  case TechTypes::Enum::Unknown:
    return getGame().setLastError(Errors::Incompatible_TechType);
  }

  return true;
}

bool Unit::canUseTech(BWAPI::TechType tech, Position target, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!canUseTechPosition(tech, target, checkTargetsType, checkCanIssueCommandType, false))
    return false;

  return true;
}

bool Unit::canUseTech(BWAPI::TechType tech, Unit target, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (target == nullptr)
  {
    if (!canUseTechWithoutTarget(tech, checkCanIssueCommandType, false))
      return false;
  }
  else
  {
    if (!canUseTechUnit(tech, target, checkCanTargetUnit, checkTargetsType, checkCanIssueCommandType, false))
      return false;
  }

  return true;
}

/*bool Unit::canUseTech(BWAPI::TechType tech, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  // TODO: No 4.X equivalent found.
  return true;
}*/

bool Unit::canUseTechWithoutTarget(BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(false))
    return false;

  if (!canUseTechWithOrWithoutTarget(tech, false, false))
    return false;
  if (tech.targetsUnit() || tech.targetsPosition() || tech == TechTypes::None || tech == TechTypes::Unknown || tech == TechTypes::Lurker_Aspect)
    return getGame().setLastError(Errors::Incompatible_TechType);

  return true;
}

bool Unit::canUseTechUnit(BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(false))
    return false;

  if (!canUseTechWithOrWithoutTarget(tech, false, false))
    return false;
  if (!tech.targetsUnit())
    return getGame().setLastError(Errors::Incompatible_TechType);

  return true;
}

bool Unit::canUseTechUnit(BWAPI::TechType tech, Unit targetUnit, bool checkCanTargetUnit, bool checkTargetsUnits, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(false))
    return false;

  if (checkTargetsUnits && !canUseTechUnit(tech, false, false))
    return false;

  if (checkCanTargetUnit && !canTargetUnit(targetUnit, false))
    return false;

  if (!targetUnit)
    return getGame().setLastError(Errors::Invalid_Parameter);

  UnitType targetType = targetUnit->getType();

  switch (tech)
  {
  case TechTypes::Enum::Archon_Warp:
    if (targetType != UnitTypes::Protoss_High_Templar)
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (targetUnit->getPlayer() != this->getPlayer())
      return getGame().setLastError(Errors::Unit_Not_Owned);
    break;

  case TechTypes::Enum::Dark_Archon_Meld:
    if (targetType != UnitTypes::Protoss_Dark_Templar)
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (targetUnit->getPlayer() != this->getPlayer())
      return getGame().setLastError(Errors::Unit_Not_Owned);
    break;

  case TechTypes::Enum::Consume:
    if (targetUnit->getPlayer() != this->getPlayer())
      return getGame().setLastError(Errors::Unit_Not_Owned);
    if (targetType.getRace() != Races::Zerg || targetType == UnitTypes::Zerg_Larva)
      return getGame().setLastError(Errors::Incompatible_UnitType);
    break;

  case TechTypes::Enum::Spawn_Broodlings:
    if ((!targetType.isOrganic() && !targetType.isMechanical()) ||
      targetType.isRobotic() ||
      targetType.isFlyer())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    break;

  case TechTypes::Enum::Lockdown:
    if (!targetType.isMechanical())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    break;

  case TechTypes::Enum::Healing:
    if (targetUnit->getHitPoints() == targetType.maxHitPoints())
      return getGame().setLastError(Errors::Incompatible_State);
    if (!targetType.isOrganic() ||
      targetType.isFlyer())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    if (!targetUnit->getPlayer()->isNeutral() && this->getPlayer()->isEnemy(targetUnit->getPlayer()))
      return getGame().setLastError(Errors::Invalid_Parameter);
    break;

  case TechTypes::Enum::Mind_Control:
    if (targetUnit->getPlayer() == this->getPlayer())
      return getGame().setLastError(Errors::Invalid_Parameter);
    if (targetType == UnitTypes::Protoss_Interceptor ||
      targetType == UnitTypes::Terran_Vulture_Spider_Mine ||
      targetType == UnitTypes::Zerg_Lurker_Egg ||
      targetType == UnitTypes::Zerg_Cocoon ||
      targetType == UnitTypes::Zerg_Larva ||
      targetType == UnitTypes::Zerg_Egg)
      return getGame().setLastError(Errors::Incompatible_UnitType);
    break;

  case TechTypes::Enum::Feedback:
    if (!targetType.isSpellcaster())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    break;

  case TechTypes::Enum::Infestation:
    if (targetType != UnitTypes::Terran_Command_Center ||
      targetUnit->getHitPoints() >= 750 || targetUnit->getHitPoints() <= 0)
      return getGame().setLastError(Errors::Invalid_Parameter);
    break;
  }

  switch (tech)
  {
  case TechTypes::Enum::Archon_Warp:
  case TechTypes::Enum::Dark_Archon_Meld:
    if (!this->hasPath(targetUnit->getPosition()))
      return getGame().setLastError(Errors::Unreachable_Location);
    if (targetUnit->isHallucination())
      return getGame().setLastError(Errors::Invalid_Parameter);
    if (targetUnit->isMaelstrommed())
      return getGame().setLastError(Errors::Incompatible_State);
    // Fall through (don't break).
  case TechTypes::Enum::Parasite:
  case TechTypes::Enum::Irradiate:
  case TechTypes::Enum::Optical_Flare:
  case TechTypes::Enum::Spawn_Broodlings:
  case TechTypes::Enum::Lockdown:
  case TechTypes::Enum::Defensive_Matrix:
  case TechTypes::Enum::Hallucination:
  case TechTypes::Enum::Healing:
  case TechTypes::Enum::Restoration:
  case TechTypes::Enum::Mind_Control:
  case TechTypes::Enum::Consume:
  case TechTypes::Enum::Feedback:
  case TechTypes::Enum::Yamato_Gun:
    if (targetUnit->isStasised())
      return getGame().setLastError(Errors::Incompatible_State);
    break;
  }

  switch (tech)
  {
  case TechTypes::Enum::Yamato_Gun:
    if (targetUnit->isInvincible())
      return getGame().setLastError(Errors::Invalid_Parameter);
    break;

  case TechTypes::Enum::Parasite:
  case TechTypes::Enum::Irradiate:
  case TechTypes::Enum::Optical_Flare:
  case TechTypes::Enum::Spawn_Broodlings:
  case TechTypes::Enum::Lockdown:
  case TechTypes::Enum::Defensive_Matrix:
  case TechTypes::Enum::Hallucination:
  case TechTypes::Enum::Healing:
  case TechTypes::Enum::Restoration:
  case TechTypes::Enum::Mind_Control:
    if (targetUnit->isInvincible())
      return getGame().setLastError(Errors::Invalid_Parameter);
    // Fall through (don't break).
  case TechTypes::Enum::Consume:
  case TechTypes::Enum::Feedback:
    if (targetType.isBuilding())
      return getGame().setLastError(Errors::Incompatible_UnitType);
    break;
  }

  if (targetUnit == *this)
    return getGame().setLastError(Errors::Invalid_Parameter);

  return true;
}

bool Unit::canUseTechPosition(BWAPI::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(false))
    return false;

  if (!canUseTechWithOrWithoutTarget(tech, false, false))
    return false;
  if (!tech.targetsPosition())
    return getGame().setLastError(Errors::Incompatible_TechType);

  return true;
}

bool Unit::canUseTechPosition(BWAPI::TechType tech, Position target, bool checkTargetsPositions, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canUseTechWithOrWithoutTarget(false))
    return false;

  if (checkTargetsPositions && !canUseTechPosition(tech, false, false))
    return false;

  if (tech == TechTypes::Enum::Spider_Mines && !this->hasPath(target))
    return getGame().setLastError(Errors::Unreachable_Location);

  return true;
}

bool Unit::canPlaceCOP(bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (!this->getType().isFlagBeacon())
    return getGame().setLastError(Errors::Incompatible_UnitType);

  if (this->getData().buttonset == 228 || this->getOrder() != BWAPI::Orders::CTFCOPInit)
    return getGame().setLastError(Errors::Incompatible_State);

  return true;
}

bool Unit::canPlaceCOP(TilePosition target, bool checkCanIssueCommandType, bool checkCommandibility) const
{
  if (!checkCommandibility)
    getGame().setLastError();
  else if (!canCommand())
    return false;

  if (checkCanIssueCommandType && !canPlaceCOP(checkCommandibility))
    return false;
  
  if (!getGame().canBuildHere(target, this->getType(), *this, true))
    return false;

  return true;
}

bool Unit::isGatheringGas() const
{
  if (!this->getData().isGathering)
    return false;
  
  if (this->getData().order != Orders::Harvest1   &&
    this->getData().order != Orders::Harvest2   &&
    this->getData().order != Orders::MoveToGas  &&
    this->getData().order != Orders::WaitForGas &&
    this->getData().order != Orders::HarvestGas &&
    this->getData().order != Orders::ReturnGas  &&
    this->getData().order != Orders::ResetCollision)
    return false;

  if (this->getData().order == Orders::ResetCollision)
    return this->getData().carryResourceType == 1;

  //return true if BWOrder is WaitForGas, HarvestGas, or ReturnGas
  if (this->getData().order == Orders::WaitForGas ||
    this->getData().order == Orders::HarvestGas ||
    this->getData().order == Orders::ReturnGas)
    return true;

  //if BWOrder is MoveToGas, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
  Unit targ = getTarget();
  if (targ &&
    targ->exists() &&
    targ->isCompleted() &&
    targ->getPlayer() == getPlayer() &&
    targ->getType() != UnitTypes::Resource_Vespene_Geyser &&
    (targ->getType().isRefinery() || targ->getType().isResourceDepot()))
    return true;
  targ = getOrderTarget();
  if (targ &&
    targ->exists() &&
    targ->isCompleted() &&
    targ->getPlayer() == getPlayer() &&
    targ->getType() != UnitTypes::Resource_Vespene_Geyser &&
    (targ->getType().isRefinery() || targ->getType().isResourceDepot()))
    return true;
  return false;
}

bool Unit::isGatheringMinerals() const
{
  if (!this->getData().isGathering)
    return false;

  if (this->getData().order != Orders::Harvest1        &&
    this->getData().order != Orders::Harvest2        &&
    this->getData().order != Orders::MoveToMinerals  &&
    this->getData().order != Orders::WaitForMinerals &&
    this->getData().order != Orders::MiningMinerals  &&
    this->getData().order != Orders::ReturnMinerals  &&
    this->getData().order != Orders::ResetCollision)
    return false;

  if (this->getData().order == Orders::ResetCollision)
    return this->getData().carryResourceType == 2;

  //return true if BWOrder is WaitForMinerals, MiningMinerals, or ReturnMinerals
  if (this->getData().order == Orders::WaitForMinerals ||
    this->getData().order == Orders::MiningMinerals ||
    this->getData().order == Orders::ReturnMinerals)
    return true;

  //if BWOrder is MoveToMinerals, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
  if (getTarget() &&
    getTarget()->exists() &&
    (getTarget()->getType().isMineralField() ||
    (getTarget()->isCompleted() &&
      getTarget()->getPlayer() == getPlayer() &&
      getTarget()->getType().isResourceDepot())))
    return true;
  if (getOrderTarget() &&
    getOrderTarget()->exists() &&
    (getOrderTarget()->getType().isMineralField() ||
    (getOrderTarget()->isCompleted() &&
      getOrderTarget()->getPlayer() == getPlayer() &&
      getOrderTarget()->getType().isResourceDepot())))
    return true;
  return false;
}