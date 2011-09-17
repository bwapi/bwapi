#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "TemplatesImpl.h"
#include "Command.h"
#include <Util\Foreach.h>

#include <limits>
#include <string>
namespace BWAPI
{
  UnitImpl::UnitImpl(int id)
    : clientInfo(NULL)
  {
    this->self = &(BWAPI::BWAPIClient.data->units[id]);
    this->id   = id;
    clear();
  }
  void UnitImpl::clear()
  {
    initialType      = UnitTypes::None;
    initialResources = 0;
    initialHitPoints = 0;
    initialPosition  = Positions::None;
    lastCommandFrame = 0;
    lastCommand      = UnitCommand();
    clientInfo       = NULL;
    connectedUnits.clear();
  }
  void UnitImpl::saveInitialState()
  {
    this->initialType      = getType();
    this->initialResources = getResources();
    this->initialHitPoints = getHitPoints();
    this->initialPosition  = getPosition();
  }
  /* UnitShared.cpp */
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Unit* target) const
  {
    if ( !target )
      return Broodwar->setLastError(Errors::Invalid_Parameter);

    Broodwar->setLastError(Errors::None);
    if ( !exists() ) return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    return Broodwar->hasPath(this->getPosition(), target->getPosition());
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Position target) const
  {
    Broodwar->setLastError(Errors::None);
    if ( !exists() ) return Broodwar->setLastError(Errors::Unit_Does_Not_Exist);

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    return Broodwar->hasPath(this->getPosition(), target);
  }
  //--------------------------------------------- GET INITIAL TYPE -------------------------------------------
  UnitType UnitImpl::getInitialType() const
  {
    return this->initialType;
  }
  //--------------------------------------------- GET INITIAL POSITION ---------------------------------------
  Position UnitImpl::getInitialPosition() const
  {
    return this->initialPosition;
  }
  //--------------------------------------------- GET INITIAL TILE POSITION ----------------------------------
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    return TilePosition(Position(this->initialPosition.x() - this->initialType.tileWidth() * TILE_SIZE / 2,
                                 this->initialPosition.y() - this->initialType.tileHeight() * TILE_SIZE / 2));
  }
  //--------------------------------------------- GET INITIAL HIT POINTS -------------------------------------
  int UnitImpl::getInitialHitPoints() const
  {
    return this->initialHitPoints;
  }
  //--------------------------------------------- GET INITIAL RESOURCES --------------------------------------
  int UnitImpl::getInitialResources() const
  {
    return this->initialResources;
  }
  //------------------------------------------------ GET UNITS IN RADIUS -------------------------------------
  const Unit *unitsInRadius_Unit;
  int unitsInRadius_Radius;
  bool __fastcall Client_unitRadiusIterator_callback(Unit *uIterator)
  {
    return unitsInRadius_Unit != uIterator && unitsInRadius_Unit->getDistance(uIterator) <= unitsInRadius_Radius;
  }
  std::set<Unit*>& UnitImpl::getUnitsInRadius(int radius) const
  {
    // Initialize static variables
    static std::set<Unit*> unitFinderResults;
    static DWORD g_dwFinderFlags[1701] = { 0 };
    unitFinderResults.clear();

    // Return if this unit does not exist
    if ( !exists() )
      return unitFinderResults;

    // Declare some variables
    int left    = this->getLeft()    - radius;
    int top     = this->getTop()     - radius;
    int right   = this->getRight()   + radius;
    int bottom  = this->getBottom()  + radius;

    // Store the data we are comparing found units to
    unitsInRadius_Unit = this;
    unitsInRadius_Radius = radius;

    // Have the unit finder do its stuff
    Templates::manageUnitFinder<const unitFinder>(((GameImpl*)Broodwar)->getGameData()->xUnitSearch, 
                                                  ((GameImpl*)Broodwar)->getGameData()->yUnitSearch, 
                                                  g_dwFinderFlags, 
                                                  left, 
                                                  top, 
                                                  right, 
                                                  bottom,
                                                  &Client_unitRadiusIterator_callback,
                                                  unitFinderResults);
    // Return results
    return unitFinderResults;
  }
  //--------------------------------------------- GET UNITS IN WEAPON RANGE ----------------------------------
  const Unit *unitsInWpnRange_Unit;
  WeaponType unitsInWpnRange_Wpn;
  int unitsInWpnRange_Max;
  bool __fastcall Client_unitInWpnRange_callback(Unit *uIterator)
  {
    // Unit check and unit status
    if ( uIterator == unitsInWpnRange_Unit || uIterator->isInvincible() )
      return false;

    // Weapon distance check
    int dist = unitsInWpnRange_Unit->getDistance(uIterator);
    if ( (unitsInWpnRange_Wpn.minRange() && dist < unitsInWpnRange_Wpn.minRange()) || dist > unitsInWpnRange_Max )
      return false;

    // Weapon behavioural checks
    UnitType ut = uIterator->getType();
    if ( (( unitsInWpnRange_Wpn.targetsOwn()          && uIterator->getPlayer() != unitsInWpnRange_Unit->getPlayer() ) ||
          ( !unitsInWpnRange_Wpn.targetsAir()         && (!uIterator->isLifted() && !ut.isFlyer()) ) ||
          ( !unitsInWpnRange_Wpn.targetsGround()      && (uIterator->isLifted() || ut.isFlyer())   ) ||
          ( unitsInWpnRange_Wpn.targetsMechanical()   && ut.isMechanical()                 ) ||
          ( unitsInWpnRange_Wpn.targetsOrganic()      && ut.isOrganic()                    ) ||
          ( unitsInWpnRange_Wpn.targetsNonBuilding()  && !ut.isBuilding()                  ) ||
          ( unitsInWpnRange_Wpn.targetsNonRobotic()   && !ut.isRobotic()                   ) ||
          ( unitsInWpnRange_Wpn.targetsOrgOrMech()    && (ut.isOrganic() || ut.isMechanical()) ))  )
      return false;

    return true;
  }
  std::set<Unit*>& UnitImpl::getUnitsInWeaponRange(WeaponType weapon) const
  {
    // Initialize static variables
    static std::set<Unit*> unitFinderResults;
    static DWORD g_dwFinderFlags[1701] = { 0 };
    unitFinderResults.clear();

    // Return if this unit does not exist
    if ( !exists() )
      return unitFinderResults;

    int max = getPlayer()->weaponMaxRange(weapon);

    // Declare some variables
    int left    = this->getLeft()    - max;
    int top     = this->getTop()     - max;
    int right   = this->getRight()   + max;
    int bottom  = this->getBottom()  + max;


    // Store the data we are comparing found units to
    unitsInWpnRange_Unit = this;
    unitsInWpnRange_Max  = max;
    unitsInWpnRange_Wpn  = weapon;

    // Have the unit finder do its stuff
    Templates::manageUnitFinder<const unitFinder>(((GameImpl*)Broodwar)->getGameData()->xUnitSearch, 
                                                  ((GameImpl*)Broodwar)->getGameData()->yUnitSearch, 
                                                  g_dwFinderFlags, 
                                                  left, 
                                                  top, 
                                                  right, 
                                                  bottom,
                                                  &Client_unitInWpnRange_callback,
                                                  unitFinderResults);
    // Return results
    return unitFinderResults;
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    if ( !canIssueCommand(command) )
      return false;

    command.unit = this;

    if (command.type == UnitCommandTypes::Train ||
        command.type == UnitCommandTypes::Morph)
      if (getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
        command.unit = *getLarva().begin();

    BWAPIC::UnitCommand c;
    c.type      = command.type;
    c.unitIndex = command.unit->getID();
    if ( command.target )
      c.targetIndex = command.target->getID();
    else
      c.targetIndex = -1;
    c.x     = command.x;
    c.y     = command.y;
    c.extra = command.extra;
    Command(command).execute(0);
    ((GameImpl*)Broodwar)->addUnitCommand(c);
    lastCommandFrame = Broodwar->getFrameCount();
    lastCommand      = command;
    return true;
  }

}
