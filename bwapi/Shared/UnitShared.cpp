#include <BWAPI.h>
#include <BWAPI/Client/UnitData.h>
#include <TemplatesImpl.h>

namespace BWAPI
{
  //--------------------------------------------- CLEAR ------------------------------------------------------
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
  //------------------------------------- INITIAL INFORMATION FUNCTIONS --------------------------------------
  void UnitImpl::saveInitialState()
  {
    initialType      = getType();
    initialPosition  = getPosition();
    initialResources = getResources();
    initialHitPoints = getHitPoints();
  }
  //--------------------------------------------- GET ID -----------------------------------------------------
  int UnitImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET REPLAY ID ----------------------------------------------
  int UnitImpl::getReplayID() const
  {
    return self->replayID;
  }
  //--------------------------------------------- GET PLAYER -------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    return Broodwar->getPlayer(self->player);
  }
  //--------------------------------------------- GET TYPE ---------------------------------------------------
  UnitType UnitImpl::getType() const
  {
    return UnitType(self->type);
  }
  //--------------------------------------------- GET POSITION -----------------------------------------------
  Position UnitImpl::getPosition() const
  {
    return Position(self->positionX, self->positionY);
  }
  //--------------------------------------------- GET TILE POSITION ------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(abs(self->positionX - getType().tileWidth()  * TILE_SIZE / 2),
                                 abs(self->positionY - getType().tileHeight() * TILE_SIZE / 2)) );
  }
  //--------------------------------------------- GET ANGLE --------------------------------------------------
  double UnitImpl::getAngle() const
  {
    return self->angle;
  }
  //--------------------------------------------- GET VELOCITY X ---------------------------------------------
  double UnitImpl::getVelocityX() const
  {
    return self->velocityX;
  }
  //--------------------------------------------- GET VELOCITY Y ---------------------------------------------
  double UnitImpl::getVelocityY() const
  {
    return self->velocityY;
  }
  //--------------------------------------------- GET REGION -------------------------------------------------
  BWAPI::Region *UnitImpl::getRegion() const
  {
    return Broodwar->getRegionAt(getPosition());
  }
  //--------------------------------------------- GET LEFT ---------------------------------------------------
  int UnitImpl::getLeft() const
  {
    return self->positionX - UnitType(self->type).dimensionLeft();
  }
  //--------------------------------------------- GET TOP ----------------------------------------------------
  int UnitImpl::getTop() const
  {
    return self->positionY - UnitType(self->type).dimensionUp();
  }
  //--------------------------------------------- GET RIGHT --------------------------------------------------
  int UnitImpl::getRight() const
  {
    return self->positionX + UnitType(self->type).dimensionRight();
  }
  //--------------------------------------------- GET BOTTOM -------------------------------------------------
  int UnitImpl::getBottom() const
  {
    return self->positionY + UnitType(self->type).dimensionDown();
  }
  //--------------------------------------------- GET HIT POINTS ---------------------------------------------
  int UnitImpl::getHitPoints() const
  {
    return self->hitPoints;
  }
  //--------------------------------------------- GET SHIELDS ------------------------------------------------
  int UnitImpl::getShields() const
  {
    return self->shields;
  }
  //--------------------------------------------- GET ENERGY -------------------------------------------------
  int UnitImpl::getEnergy() const
  {
    return self->energy;
  }
  //--------------------------------------------- GET RESOURCES ----------------------------------------------
  int UnitImpl::getResources() const
  {
    return self->resources;
  }
  //--------------------------------------------- GET RESOURCE GROUP -----------------------------------------
  int UnitImpl::getResourceGroup() const
  {
    return self->resourceGroup;
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  int UnitImpl::getDistance(Unit* target) const
  {
    if ( !exists() || !target || !target->exists() )
      return MAXINT;

    if (this == target)
      return 0;
    
    return computeDistance<UnitImpl>(this,target);
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  int UnitImpl::getDistance(Position target) const
  {
    if (!exists())
      return MAXINT;
    return computeDistance<UnitImpl>(this,target);
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Unit* target) const
  {
    Broodwar->setLastError(Errors::None);
    if ( !target )
      return Broodwar->setLastError(Errors::Invalid_Parameter);
    if ( !target->exists() || !exists() )
      return Broodwar->setLastError(Errors::Unit_Not_Visible);

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    return Broodwar->hasPath(this->getPosition(), target->getPosition());
  }
  //--------------------------------------------- HAS PATH ---------------------------------------------------
  bool UnitImpl::hasPath(Position target) const
  {
    Broodwar->setLastError(Errors::None);
    if (!exists())
      return Broodwar->setLastError(Errors::Unit_Not_Visible);

    if ( this->getType().isFlyer() || this->isLifted() )
      return true;

    return Broodwar->hasPath(this->getPosition(), target);
  }
  //------------------------------------------------ GET UNITS IN RADIUS -------------------------------------
  const Unit *unitsInRadius_Unit;
  int unitsInRadius_Radius;
  bool __fastcall Shared_unitRadiusIterator_callback(Unit *uIterator)
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
    unitsInRadius_Unit    = this;
    unitsInRadius_Radius  = radius;

    // Have the unit finder do its stuff
    Templates::manageUnitFinder<const unitFinder>(((GameImpl*)Broodwar)->getGameData()->xUnitSearch, 
                                                  ((GameImpl*)Broodwar)->getGameData()->yUnitSearch, 
                                                  g_dwFinderFlags, 
                                                  left, 
                                                  top, 
                                                  right, 
                                                  bottom,
                                                  &Shared_unitRadiusIterator_callback,
                                                  unitFinderResults);
    // Return results
    return unitFinderResults;
  }
  //--------------------------------------------- GET UNITS IN WEAPON RANGE ----------------------------------
  const Unit *unitsInWpnRange_Unit;
  WeaponType unitsInWpnRange_Wpn;
  int unitsInWpnRange_Max;
  bool __fastcall Shared_unitInWpnRange_callback(Unit *uIterator)
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
                                                  &Shared_unitInWpnRange_callback,
                                                  unitFinderResults);
    // Return results
    return unitFinderResults;
  }
  //--------------------------------------------- GET LAST COMMAND FRAME -------------------------------------
  int UnitImpl::getLastCommandFrame() const
  {
    return lastCommandFrame;
  }
  //--------------------------------------------- GET LAST COMMAND -------------------------------------------
  UnitCommand UnitImpl::getLastCommand() const
  {
    return lastCommand;
  }
  //--------------------------------------------- GET LAST ATTACKING PLAYER ----------------------------------
  BWAPI::Player *UnitImpl::getLastAttackingPlayer() const
  {
    return Broodwar->getPlayer(self->lastAttackerPlayer);
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int UnitImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if ( !getPlayer() ||
          getPlayer()->getUpgradeLevel(upgrade) == 0 ||
          upgrade.whatUses().find(getType()) == upgrade.whatUses().end())
      return 0;
    return getPlayer()->getUpgradeLevel(upgrade);
  }
  //--------------------------------------------- GET INITIAL TYPE -------------------------------------------
  UnitType UnitImpl::getInitialType() const
  {
    return initialType;
  }
  //--------------------------------------------- GET INITIAL POSITION ---------------------------------------
  Position UnitImpl::getInitialPosition() const
  {
    return initialPosition;
  }
  //--------------------------------------------- GET INITIAL TILE POSITION ----------------------------------
  TilePosition UnitImpl::getInitialTilePosition() const
  {
    if (initialPosition == Positions::None)
    {
      return TilePositions::None;
    }
    return TilePosition(Position(initialPosition.x() - initialType.tileWidth() * TILE_SIZE / 2,
                                 initialPosition.y() - initialType.tileHeight() * TILE_SIZE / 2));
  }
  //--------------------------------------------- GET INITIAL HIT POINTS -------------------------------------
  int UnitImpl::getInitialHitPoints() const
  {
    return initialHitPoints;
  }
  //--------------------------------------------- GET INITIAL RESOURCES --------------------------------------
  int UnitImpl::getInitialResources() const
  {
    return initialResources;
  }
  //--------------------------------------------- GET KILL COUNT ---------------------------------------------
  int UnitImpl::getKillCount() const
  {
    return self->killCount;
  }
  //--------------------------------------------- GET ACID SPORE COUNT ---------------------------------------
  int UnitImpl::getAcidSporeCount() const
  {
    return self->acidSporeCount;
  }
  //--------------------------------------------- GET INTERCEPTOR COUNT --------------------------------------
  int UnitImpl::getInterceptorCount() const
  {
    if (getType() != UnitTypes::Protoss_Carrier && getType() != UnitTypes::Hero_Gantrithor)
      return 0;
    return connectedUnits.size();
  }
  //--------------------------------------------- GET SCARAB COUNT -------------------------------------------
  int UnitImpl::getScarabCount() const
  {
    return self->scarabCount;
  }
  //--------------------------------------------- GET SPIDER MINE COUNT --------------------------------------
  int UnitImpl::getSpiderMineCount() const
  {
    return self->spiderMineCount;
  }
  //--------------------------------------------- GET GROUND WEAPON COOLDOWN ---------------------------------
  int UnitImpl::getGroundWeaponCooldown() const
  {
    return self->groundWeaponCooldown;
  }
  //--------------------------------------------- GET AIR WEAPON COOLDOWN ------------------------------------
  int UnitImpl::getAirWeaponCooldown() const
  {
    return self->airWeaponCooldown;
  }
  //--------------------------------------------- GET SPELL COOLDOWN -----------------------------------------
  int UnitImpl::getSpellCooldown() const
  {
    return self->spellCooldown;
  }
  //--------------------------------------------- GET DEFENSE MATRIX POINTS ----------------------------------
  int UnitImpl::getDefenseMatrixPoints() const
  {
    return self->defenseMatrixPoints;
  }
  //--------------------------------------------- GET DEFENSE MATRIX TIMER -----------------------------------
  int UnitImpl::getDefenseMatrixTimer() const
  {
    return self->defenseMatrixTimer;
  }
  //--------------------------------------------- GET ENSNARE TIMER ------------------------------------------
  int UnitImpl::getEnsnareTimer() const
  {
    return self->ensnareTimer;
  }
  //--------------------------------------------- GET IRRADIATE TIMER ----------------------------------------
  int UnitImpl::getIrradiateTimer() const
  {
    return self->irradiateTimer;
  }
  //--------------------------------------------- GET LOCKDOWN TIMER -----------------------------------------
  int UnitImpl::getLockdownTimer() const
  {
    return self->lockdownTimer;
  }
  //--------------------------------------------- GET MAELSTROM TIMER ----------------------------------------
  int UnitImpl::getMaelstromTimer() const
  {
    return self->maelstromTimer;
  }
  //--------------------------------------------- GET ORDER TIMER --------------------------------------------
  int UnitImpl::getOrderTimer() const
  {
    return self->orderTimer;
  }
  //--------------------------------------------- GET PLAGUE TIMER -------------------------------------------
  int UnitImpl::getPlagueTimer() const
  {
    return self->plagueTimer;
  }
  //--------------------------------------------- GET REMOVE TIMER -------------------------------------------
  int UnitImpl::getRemoveTimer() const
  {
    return self->removeTimer;
  }
  //--------------------------------------------- GET STASIS TIMER -------------------------------------------
  int UnitImpl::getStasisTimer() const
  {
    return self->stasisTimer;
  }
  //--------------------------------------------- GET STIM TIMER ---------------------------------------------
  int UnitImpl::getStimTimer() const
  {
    return self->stimTimer;
  }
  //--------------------------------------------- GET BUILD TYPE ---------------------------------------------
  UnitType UnitImpl::getBuildType() const
  {
    return UnitType(self->buildType);
  }
  //--------------------------------------------- GET TRAINING QUEUE -----------------------------------------
  std::list<UnitType> UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType> trainingQueue;
    for (int i = 0; i < self->trainingQueueCount; ++i)
      trainingQueue.push_back(self->trainingQueue[i]);
    return trainingQueue;
  }
  //--------------------------------------------- GET TECH ---------------------------------------------------
  TechType UnitImpl::getTech() const
  {
    return TechType(self->tech);
  }
  //--------------------------------------------- GET UPGRADE ------------------------------------------------
  UpgradeType UnitImpl::getUpgrade() const
  {
    return UpgradeType(self->upgrade);
  }
  //--------------------------------------------- GET REMAINING BUILD TIME -----------------------------------
  int UnitImpl::getRemainingBuildTime() const
  {
    return self->remainingBuildTime;
  }
  //--------------------------------------------- GET REMAINING TRAIN TIME -----------------------------------
  int UnitImpl::getRemainingTrainTime() const
  {
    return self->remainingTrainTime;
  }
  //--------------------------------------------- GET REMAINING RESEARCH TIME --------------------------------
  int UnitImpl::getRemainingResearchTime() const
  {
    return self->remainingResearchTime;
  }
  //--------------------------------------------- GET REMAINING UPGRADE TIME ---------------------------------
  int UnitImpl::getRemainingUpgradeTime() const
  {
    return self->remainingUpgradeTime;
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    return Broodwar->getUnit(self->buildUnit);
  }
  //--------------------------------------------- GET TARGET -------------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    return Broodwar->getUnit(self->target);
  }
  //--------------------------------------------- GET TARGET POSITION ----------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    return Position(self->targetPositionX,self->targetPositionY);
  }
  //--------------------------------------------- GET ORDER --------------------------------------------------
  Order UnitImpl::getOrder() const
  {
    return Order(self->order);
  }
  //--------------------------------------------- GET ORDER TARGET -------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    return Broodwar->getUnit(self->orderTarget);
  }
  //--------------------------------------------- GET ORDER TARGET POSITION ----------------------------------
  Position UnitImpl::getOrderTargetPosition() const
  {
	  return Position(self->orderTargetPositionX,self->orderTargetPositionY);
  }
  //--------------------------------------------- GET SECONDARY ORDER ID -------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return Order(self->secondaryOrder);
  }
  //--------------------------------------------- GET RALLY POSITION -----------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    return Position(self->rallyPositionX,self->rallyPositionY);
  }
  //--------------------------------------------- GET RALLY UNIT ---------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    return Broodwar->getUnit(self->rallyUnit);
  }
  //--------------------------------------------- GET ADDON --------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    return Broodwar->getUnit(self->addon);
  }
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit* UnitImpl::getNydusExit() const
  {
    return Broodwar->getUnit(self->nydusExit);
  }
  //--------------------------------------------- GET POWER UP -----------------------------------------------
  Unit* UnitImpl::getPowerUp() const
  {
    return Broodwar->getUnit(self->powerUp);
  }
  //--------------------------------------------- GET TRANSPORT ----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    return Broodwar->getUnit(self->transport);
  }
  //--------------------------------------------- GET LOADED UNITS -------------------------------------------
  std::set<Unit*> UnitImpl::getLoadedUnits() const
  {
    return loadedUnits;
  }
  //--------------------------------------------- GET CARRIER ------------------------------------------------
  Unit* UnitImpl::getCarrier() const
  {
    return Broodwar->getUnit(self->carrier);
  }
  //--------------------------------------------- GET INTERCEPTORS -------------------------------------------
  std::set<Unit*> UnitImpl::getInterceptors() const
  {
    std::set<Unit*> nothing;
    if (getType() != UnitTypes::Protoss_Carrier && getType() != UnitTypes::Hero_Gantrithor)
      return nothing;
    return connectedUnits;
  }
  //--------------------------------------------- GET HATCHERY -----------------------------------------------
  Unit* UnitImpl::getHatchery() const
  {
    return Broodwar->getUnit(self->hatchery);
  }
  //--------------------------------------------- GET LARVA --------------------------------------------------
  std::set<Unit*> UnitImpl::getLarva() const
  {
    std::set<Unit*> nothing;
    if (!getType().producesLarva())
        return nothing;
    return connectedUnits;
  }
  //--------------------------------------------- GET CLIENT INFO --------------------------------------------
  void* UnitImpl::getClientInfo() const
  {
    return clientInfo;
  }
  //--------------------------------------------- SET CLIENT INFO --------------------------------------------
  void UnitImpl::setClientInfo(void* clientinfo)
  {
    clientInfo = clientinfo;
  }
  //--------------------------------------------- EXISTS -----------------------------------------------------
  bool UnitImpl::exists() const
  {
    return self->exists;
  }
  //--------------------------------------------- HAS NUKE ---------------------------------------------------
  bool UnitImpl::hasNuke() const
  {
    return self->hasNuke;
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    return self->isAccelerating;
  }
  //--------------------------------------------- IS ATTACKING -----------------------------------------------
  bool UnitImpl::isAttacking() const
  {
    return self->isAttacking;
  }
  //--------------------------------------------- IS ATTACK FRAME --------------------------------------------
  bool UnitImpl::isAttackFrame() const
  {
    return self->isAttackFrame;
  }
  //--------------------------------------------- IS BEING CONSTRUCTED ---------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    if (self->isMorphing)
      return true;
    if (self->isCompleted)
      return false;
    if (getType().getRace() != Races::Terran)
      return true;
    return self->buildUnit != -1;
  }
  //--------------------------------------------- IS BEING GATHERED ------------------------------------------
  bool UnitImpl::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const
  {
    return getType().getRace() == Races::Terran &&
           self->isCompleted &&
           self->hitPoints > self->lastHitPoints;
  }
  //--------------------------------------------- IS BLIND ---------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    return self->isBlind;
  }
  //--------------------------------------------- IS BRAKING -------------------------------------------------
  bool UnitImpl::isBraking() const
  {
    return self->isBraking;
  }
  //--------------------------------------------- IS BURROWED ------------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    return self->isBurrowed;
  }
  //--------------------------------------------- IS CARRYING GAS --------------------------------------------
  bool UnitImpl::isCarryingGas() const
  {
    return self->carryResourceType == 1;
  }
  //--------------------------------------------- IS CARRYING MINERALS ---------------------------------------
  bool UnitImpl::isCarryingMinerals() const
  {
    return self->carryResourceType == 2;
  }
  //--------------------------------------------- IS CLOAKED -------------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    return self->isCloaked;
  }
  //--------------------------------------------- IS COMPLETED -----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return self->isCompleted;
  }
  //--------------------------------------------- IS CONSTRUCTING --------------------------------------------
  bool UnitImpl::isConstructing() const
  {
    return self->isConstructing;
  }
  //--------------------------------------------- IS DEFENSE MATRIXED ----------------------------------------
  bool UnitImpl::isDefenseMatrixed() const
  {
    return self->defenseMatrixTimer > 0;
  }
  //--------------------------------------------- IS DETECTED ------------------------------------------------
  bool UnitImpl::isDetected() const
  {
    return self->isDetected;
  }
  //--------------------------------------------- IS ENSNARED ------------------------------------------------
  bool UnitImpl::isEnsnared() const
  {
    return self->ensnareTimer > 0;
  }
  //--------------------------------------------- IS FOLLOWING -----------------------------------------------
  bool UnitImpl::isFollowing() const
  {
    return self->order == Orders::Follow;
  }
  //--------------------------------------------- IS GATHERING GAS -------------------------------------------
  bool UnitImpl::isGatheringGas() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::Harvest1   &&
        self->order != Orders::Harvest2   &&
        self->order != Orders::MoveToGas  &&
        self->order != Orders::WaitForGas &&
        self->order != Orders::HarvestGas &&
        self->order != Orders::ReturnGas  &&
        self->order != Orders::ResetCollision)
      return false;

    if (self->order == Orders::ResetCollision)
      return self->carryResourceType == 1;

    //return true if BWOrder is WaitForGas, HarvestGas, or ReturnGas
    if (self->order == Orders::WaitForGas ||
        self->order == Orders::HarvestGas ||
        self->order == Orders::ReturnGas)
      return true;

    //if BWOrder is MoveToGas, Harvest1, or Harvest2 we need to do some additional checks to make sure the unit is really gathering
    if (getTarget() &&
        getTarget()->exists() && 
        getTarget()->isCompleted() &&
        getTarget()->getPlayer() == getPlayer() &&
        getTarget()->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (getTarget()->getType().isRefinery() || getTarget()->getType().isResourceDepot()))
      return true;
    if (getOrderTarget() &&
        getOrderTarget()->exists() && 
        getOrderTarget()->isCompleted() &&
        getOrderTarget()->getPlayer() == getPlayer() &&
        getOrderTarget()->getType() != UnitTypes::Resource_Vespene_Geyser &&
        (getOrderTarget()->getType().isRefinery() || getOrderTarget()->getType().isResourceDepot()))
      return true;
    return false;
  }
  //--------------------------------------------- IS GATHERING MINERALS --------------------------------------
  bool UnitImpl::isGatheringMinerals() const
  {
    if (!self->isGathering)
      return false;

    if (self->order != Orders::Harvest1        &&
        self->order != Orders::Harvest2        &&
        self->order != Orders::MoveToMinerals  &&
        self->order != Orders::WaitForMinerals &&
        self->order != Orders::MiningMinerals  &&
        self->order != Orders::ReturnMinerals  &&
        self->order != Orders::ResetCollision)
      return false;

    if (self->order == Orders::ResetCollision)
      return self->carryResourceType == 2;

    //return true if BWOrder is WaitForMinerals, MiningMinerals, or ReturnMinerals
    if (self->order == Orders::WaitForMinerals ||
        self->order == Orders::MiningMinerals ||
        self->order == Orders::ReturnMinerals)
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
  //--------------------------------------------- IS HALLUCINATION -------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    return self->isHallucination;
  }
  //--------------------------------------------- IS HOLDING POSITION ----------------------------------------
  bool UnitImpl::isHoldingPosition() const
  {
    return self->order == Orders::HoldPosition;
  }
  //--------------------------------------------- IS IDLE ----------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return self->isIdle;
  }
  //--------------------------------------------- IS INTERRUPTIBLE -------------------------------------------
  bool UnitImpl::isInterruptible() const
  {
    return self->isInterruptible;
  }
  //--------------------------------------------- IS INVINCIBLE ----------------------------------------------
  bool UnitImpl::isInvincible() const
  {
    return self->isInvincible;
  }
  //--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
  bool UnitImpl::isInWeaponRange(Unit *target) const
  {
    if ( !exists() || !target || !target->exists() || this == target )
      return false;

    UnitType thisType = getType();
    UnitType targType = target->getType();

    WeaponType wpn = thisType.groundWeapon();
    int minRange = wpn.minRange();
    int maxRange = getPlayer()->groundWeaponMaxRange(thisType);
    if ( targType.isFlyer() || target->isLifted() )
    {
      wpn = thisType.airWeapon();
      minRange = wpn.minRange();
      maxRange = getPlayer()->airWeaponMaxRange(thisType);
    }
    if ( wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown )
      return false;

    int distance = computeDistance<UnitImpl>(this,target);
    return (minRange ? minRange < distance : true) && maxRange >= distance;
  }
  //--------------------------------------------- IS IRRADIATED ----------------------------------------------
  bool UnitImpl::isIrradiated() const
  {
    return self->irradiateTimer > 0;
  }
  //--------------------------------------------- IS LIFTED --------------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return self->isLifted;
  }
  //--------------------------------------------- IS LOADED --------------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    return self->transport != -1;
  }
  //--------------------------------------------- IS LOCKED DOWN ---------------------------------------------
  bool UnitImpl::isLockedDown() const
  {
    return self->lockdownTimer > 0;
  }
  //--------------------------------------------- IS MAELSTROMMED --------------------------------------------
  bool UnitImpl::isMaelstrommed() const
  {
    return self->maelstromTimer > 0;
  }
  //--------------------------------------------- IS MORPHING ------------------------------------------------
  bool UnitImpl::isMorphing() const
  {
    return self->isMorphing;
  }
  //--------------------------------------------- IS MOVING --------------------------------------------------
  bool UnitImpl::isMoving() const
  {
    return self->isMoving;
  }
  //--------------------------------------------- IS PARASITED -----------------------------------------------
  bool UnitImpl::isParasited() const
  {
    return self->isParasited;
  }
  //--------------------------------------------- IS PATROLLING ----------------------------------------------
  bool UnitImpl::isPatrolling() const
  {
    return self->order == Orders::Patrol;
  }
  //--------------------------------------------- IS PLAGUED -------------------------------------------------
  bool UnitImpl::isPlagued() const
  {
    return self->plagueTimer > 0;
  }
  //--------------------------------------------- IS REPAIRING -----------------------------------------------
  bool UnitImpl::isRepairing() const
  {
    return self->order == Orders::Repair;
  }
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool UnitImpl::isResearching() const
  {
    return self->order == Orders::ResearchTech;
  }
  //--------------------------------------------- IS SELECTED ------------------------------------------------
  bool UnitImpl::isSelected() const
  {
    return self->isSelected;
  }
  //--------------------------------------------- IS SELECTED ------------------------------------------------
  bool UnitImpl::isSieged() const
  {
    return self->type == UnitTypes::Terran_Siege_Tank_Siege_Mode ||
           self->type == UnitTypes::Hero_Edmund_Duke_Siege_Mode;
  }
  //--------------------------------------------- IS STARTING ATTACK -----------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    return self->isStartingAttack;
  }
  //--------------------------------------------- IS STASISED ------------------------------------------------
  bool UnitImpl::isStasised() const
  {
    return self->stasisTimer > 0;
  }
  //--------------------------------------------- IS STIMMED -------------------------------------------------
  bool UnitImpl::isStimmed() const
  {
    return self->stimTimer > 0;
  }
  //--------------------------------------------- IS STUCK ---------------------------------------------------
  bool UnitImpl::isStuck() const
  {
    return self->isStuck;
  }
  //--------------------------------------------- IS TRAINING ------------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return self->isTraining;
  }
  //--------------------------------------------- IS UNDER ATTACK --------------------------------------------
  bool UnitImpl::isUnderAttack() const
  {
    return self->recentlyAttacked;
  }
  //--------------------------------------------- IS UNDER DARK SWARM ----------------------------------------
  bool UnitImpl::isUnderDarkSwarm() const
  {
    return self->isUnderDarkSwarm;
  }
  //--------------------------------------------- IS UNDER DISRUPTION WEB ------------------------------------
  bool UnitImpl::isUnderDisruptionWeb() const
  {
    return self->isUnderDWeb;
  }
  //--------------------------------------------- IS UNDER STORM ---------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    return self->isUnderStorm;
  }
  //--------------------------------------------- IS UNPOWERED -----------------------------------------------
  bool UnitImpl::isUnpowered() const
  {
    return self->isUnpowered;
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool UnitImpl::isUpgrading() const
  {
    return self->order == Orders::Upgrade;
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible() const
  {
    if (!Broodwar->self())
    {
      for(int i = 0; i < 9; ++i)
      {
        if (self->isVisible[i])
          return true;
      }
      return false;
    }
    return self->isVisible[Broodwar->self()->getID()];
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible(Player* player) const
  {
    if ( !player )
      return false;
    return self->isVisible[player->getID()];
  }
  //--------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
  bool UnitImpl::canIssueCommand(UnitCommand command) const
  {
    return Templates::canIssueCommand(this,command);
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool UnitImpl::attack(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::attack(this, target, shiftQueueCommand));
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attack(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::attack(this, target, shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type)
  {
    return issueCommand(UnitCommand::build(this, target, type));
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type)
  {
    return issueCommand(UnitCommand::buildAddon(this,type));
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type)
  {
    return issueCommand(UnitCommand::train(this,type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    return issueCommand(UnitCommand::morph(this,type));
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    return issueCommand(UnitCommand::research(this,tech));
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    return issueCommand(UnitCommand::upgrade(this,upgrade));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPoint(Position target)
  {
    return issueCommand(UnitCommand::setRallyPoint(this,target));
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyPoint(Unit* target)
  {
    return issueCommand(UnitCommand::setRallyPoint(this,target));
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::move(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::patrol(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::holdPosition(this, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::stop(this, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::follow(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::gather(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::returnCargo(this, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::repair(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    return issueCommand(UnitCommand::burrow(this));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    return issueCommand(UnitCommand::unburrow(this));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    return issueCommand(UnitCommand::cloak(this));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    return issueCommand(UnitCommand::decloak(this));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    return issueCommand(UnitCommand::siege(this));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    return issueCommand(UnitCommand::unsiege(this));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    return issueCommand(UnitCommand::lift(this));
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    return issueCommand(UnitCommand::land(this,target));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::load(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    return issueCommand(UnitCommand::unload(this,target));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::unloadAll(this, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::unloadAll(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::rightClick(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target, bool shiftQueueCommand)
  {
    return issueCommand(UnitCommand::rightClick(this,target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    return issueCommand(UnitCommand::haltConstruction(this));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    return issueCommand(UnitCommand::cancelConstruction(this));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    return issueCommand(UnitCommand::cancelAddon(this));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    return issueCommand(UnitCommand::cancelTrain(this, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    return issueCommand(UnitCommand::cancelMorph(this));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    return issueCommand(UnitCommand::cancelResearch(this));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    return issueCommand(UnitCommand::cancelUpgrade(this));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    return issueCommand(UnitCommand::useTech(this,tech));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position target)
  {
    return issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    return issueCommand(UnitCommand::useTech(this,tech,target));
  }
  //--------------------------------------------- PLACE COP --------------------------------------------------
  bool UnitImpl::placeCOP(TilePosition target)
  {
    return issueCommand(UnitCommand::placeCOP(this, target));
  }
}