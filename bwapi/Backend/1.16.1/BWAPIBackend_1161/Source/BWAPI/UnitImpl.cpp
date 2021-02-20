#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"

#include <cmath>

#include <BWAPI/Order.h>
#include <BWAPI/WeaponType.h>

#include "Command.h"
#include "../DLLMain.h"
#include "../Detours.h"
#include "Templates.h"

#include "../BW/CUnit.h"
#include "../BW/COrder.h"
#include "../BW/Offsets.h"
#include "../BW/OrderTypes.h"

namespace BWAPI4
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  UnitImpl::UnitImpl(BW::CUnit* originalUnit, u16 index)
      : getOriginalRawData(originalUnit)
      , index(index)
  {
    clear();
  }
  //--------------------------------------------- SET ID -----------------------------------------------------
  void UnitImpl::setID(int newID)
  {
    id = newID;
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  void UnitImpl::setLastImmediateCommand(const UnitCommand &command)
  {
    // Set last immediate command and immediate command frame
    if ( command.type != UnitCommandTypes::Cloak && 
         command.type != UnitCommandTypes::Decloak && 
         command.type != UnitCommandTypes::Unload &&
         !command.isQueued() )
    {
      static_cast<UnitImpl*>(command.unit)->lastImmediateCommand = command;
      static_cast<UnitImpl*>(command.unit)->lastImmediateCommandFrame = BroodwarImpl.getFrameCount();
    }
  }
  void UnitImpl::prepareIssueCommand(UnitCommand &command)
  {
    command.unit = this;

    // If using train or morph on a hatchery, automatically switch selection to larva
    // (assuming canIssueCommand ensures that there is a larva)
    if ( (command.type == UnitCommandTypes::Train ||
          command.type == UnitCommandTypes::Morph) &&
         getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
    {
      Unitset larvae( this->getLarva() );
      for(Unit larva : larvae)
      {
        if ( !larva->isConstructing() && larva->isCompleted() && larva->canCommand() )
        {
          command.unit = larva;
          break;
        }
      }
      if ( command.unit == this )
        return;
    }

    // Set last command and command frames
    static_cast<UnitImpl*>(command.unit)->lastCommandFrame = BroodwarImpl.getFrameCount();
    static_cast<UnitImpl*>(command.unit)->lastCommand      = command;
    static_cast<UnitImpl*>(command.unit)->setLastImmediateCommand(command);
    if (command.type == UnitCommandTypes::Use_Tech_Unit && command.target && 
       (command.extra == TechTypes::Archon_Warp || command.extra == TechTypes::Dark_Archon_Meld))
    {
      static_cast<UnitImpl*>(command.target)->lastCommandFrame = BroodwarImpl.getFrameCount();
      static_cast<UnitImpl*>(command.target)->lastCommand      = command;
      static_cast<UnitImpl*>(command.target)->setLastImmediateCommand(command);
    }

    // Add to command optimizer if possible, as well as the latency compensation buffer
    BroodwarImpl.addToCommandBuffer(Command{ command });
  }
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    if ( !canIssueCommand(command) )
      return false;
    
    this->prepareIssueCommand(command);

    // Select High templar for morphing
    if (command.type == UnitCommandTypes::Use_Tech_Unit && command.target &&
       (command.extra == TechTypes::Archon_Warp || command.extra == TechTypes::Dark_Archon_Meld))
    {
      //select both units for archon warp or dark archon meld
      Unit sel2[2] = { command.unit, command.target };
      BW::Orders::Select sel(2, sel2);
      QueueGameCommand(&sel, sel.size());
      BroodwarImpl.apmCounter.addSelect();
    }
    else //if (command.type != UnitCommandTypes::Unload || BroodwarImpl.commandOptimizer.level < 2)
    {
      static_cast<UnitImpl*>(command.unit)->orderSelect();
    }

    // Immediately execute the command
    BroodwarImpl.executeCommand( command );
    return true;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    userSelected = selectedState;
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void UnitImpl::orderSelect()
  {
    Unit u = this;
    BW::Orders::Select sel = BW::Orders::Select(1, &u);
    QueueGameCommand(&sel, sel.size());
    BroodwarImpl.apmCounter.addSelect();
  }
  //----------------------------------------------------------------------------------------------------------
  UnitImpl* UnitImpl::BWUnitToBWAPIUnit(BW::CUnit* unit)
  {
    if ( !unit )
      return nullptr;

    int index = unit - BW::BWDATA::UnitNodeTable.data();
    if (index > BW::UNIT_ARRAY_MAX_LENGTH)
    {
      return nullptr;
    }
    return BroodwarImpl.getUnitFromIndex(index);
  }

  void UnitImpl::die()
  {
    index              = 0xFFFF;
    userSelected       = false;
    isAlive            = false;
    wasAlive           = false;
    wasCompleted       = false;
    wasAccessible      = false;
    wasVisible         = false;
    nukeDetected       = false;
    lastType           = UnitTypes::Unknown;
    lastPlayer         = nullptr;

    updateInternalData();

    //set pointers to null so we don't read information from unit table anymore
    getOriginalRawData = nullptr;

    updateData();
  }

  /* canAccess returns true if the AI module is allowed to access the unit.
    If the unit is visible, returns true.
    If the unit is does not exist, returns false.
    If the unit is not visible and exists, returns true only if CompleteMapInformation is enabled.
  */
  bool UnitImpl::canAccess() const
  {
    if (!this->isAlive)
      return false;
    if (this->isVisible())
      return true;

    //if we get here, the unit exists but is not visible
    if ( BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
      return true;

    // neutral units visible during AIModule::onStart
    if ( BroodwarImpl.getFrameCount() == 0 )
      if (this->_getType.isNeutral() || (this->_getPlayer && this->_getPlayer->isNeutral()) )
        return true;
    return false;
  }
  bool UnitImpl::canAccessDetected() const
  {
    if (!canAccess())
      return false;
    if (this->_getPlayer->getNationId() == BroodwarImpl.self()->getNationId())
      return true;
    if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return true;

    // neutral units visible during AIModule::onStart
    if (BroodwarImpl.getFrameCount() == 0)
      if (this->_getType.isNeutral())
        return true;

    return self->isDetected;
  }

  //returns true if canAccess() is true and the unit is owned by self (or complete map info is turned on)
  bool UnitImpl::canAccessInside() const
  {
    if (!canAccessDetected())
      return false;
    if (this->_getPlayer->getNationId() == BroodwarImpl.self()->getNationId())
      return true;

    //If we get here, canAccess()==true but unit is not owned by self.
    //Return value depends on if unit is a mineral field or vespene geyser and the state of complete map info flag
    return this->_getType.isMineralField() || this->_getType == BWAPI::UnitTypes::Resource_Vespene_Geyser || BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation);
  }

  //--------------------------------------------- GET NEXT ---------------------------------------------------
  UnitImpl* UnitImpl::getNext() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->next);
  }
  //--------------------------------------------- GET INDEX --------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
  //--------------------------------------------- CLEAR ------------------------------------------------------
  void UnitImpl::clear()
  {
    initialType = UnitTypes::None;
    initialResources = 0;
    initialHitPoints = 0;
    initialPosition = Positions::None;
    lastCommandFrame = 0;
    lastCommand = UnitCommand();

    connectedUnits.clear();
  }
  //------------------------------------- INITIAL INFORMATION FUNCTIONS --------------------------------------
  void UnitImpl::saveInitialState()
  {
    initialType = getType();
    initialPosition = getPosition();
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
  Player UnitImpl::getPlayer() const
  {
    return BroodwarImpl.getPlayer(self->player);
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
      return TilePositions::None;
    return TilePosition(initialPosition - Position(initialType.tileSize()) / 2);
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
    return self->interceptorCount;
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
  UnitType::list UnitImpl::getTrainingQueue() const
  {
    return UnitType::list(std::begin(self->trainingQueue), std::begin(self->trainingQueue) + self->trainingQueueCount);
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
  Unit UnitImpl::getBuildUnit() const
  {
    return BroodwarImpl.getUnit(self->buildUnit);
  }
  //--------------------------------------------- GET TARGET -------------------------------------------------
  Unit UnitImpl::getTarget() const
  {
    return BroodwarImpl.getUnit(self->target);
  }
  //--------------------------------------------- GET TARGET POSITION ----------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    return Position(self->targetPositionX, self->targetPositionY);
  }
  //--------------------------------------------- GET ORDER --------------------------------------------------
  Order UnitImpl::getOrder() const
  {
    return Order(self->order);
  }
  //--------------------------------------------- GET ORDER TARGET -------------------------------------------
  Unit UnitImpl::getOrderTarget() const
  {
    return BroodwarImpl.getUnit(self->orderTarget);
  }
  //--------------------------------------------- GET ORDER TARGET POSITION ----------------------------------
  Position UnitImpl::getOrderTargetPosition() const
  {
    return Position(self->orderTargetPositionX, self->orderTargetPositionY);
  }
  //--------------------------------------------- GET SECONDARY ORDER ID -------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return Order(self->secondaryOrder);
  }
  //--------------------------------------------- GET RALLY POSITION -----------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    return Position(self->rallyPositionX, self->rallyPositionY);
  }
  //--------------------------------------------- GET RALLY UNIT ---------------------------------------------
  Unit UnitImpl::getRallyUnit() const
  {
    return BroodwarImpl.getUnit(self->rallyUnit);
  }
  //--------------------------------------------- GET ADDON --------------------------------------------------
  Unit UnitImpl::getAddon() const
  {
    return BroodwarImpl.getUnit(self->addon);
  }
  //--------------------------------------------- GET NYDUS EXIT ---------------------------------------------
  Unit UnitImpl::getNydusExit() const
  {
    return BroodwarImpl.getUnit(self->nydusExit);
  }
  //--------------------------------------------- GET POWER UP -----------------------------------------------
  Unit UnitImpl::getPowerUp() const
  {
    return BroodwarImpl.getUnit(self->powerUp);
  }
  //--------------------------------------------- GET TRANSPORT ----------------------------------------------
  Unit UnitImpl::getTransport() const
  {
    return BroodwarImpl.getUnit(self->transport);
  }
  //--------------------------------------------- GET LOADED UNITS -------------------------------------------
  Unitset UnitImpl::getLoadedUnits() const
  {
    return loadedUnits;
  }
  //--------------------------------------------- GET CARRIER ------------------------------------------------
  Unit UnitImpl::getCarrier() const
  {
    return BroodwarImpl.getUnit(self->carrier);
  }
  //--------------------------------------------- GET INTERCEPTORS -------------------------------------------
  Unitset UnitImpl::getInterceptors() const
  {
    if (getType() != UnitTypes::Protoss_Carrier && getType() != UnitTypes::Hero_Gantrithor)
      return Unitset{};
    return connectedUnits;
  }
  //--------------------------------------------- GET HATCHERY -----------------------------------------------
  Unit UnitImpl::getHatchery() const
  {
    return BroodwarImpl.getUnit(self->hatchery);
  }
  //--------------------------------------------- GET LARVA --------------------------------------------------
  Unitset UnitImpl::getLarva() const
  {
    if (!getType().producesLarva())
      return Unitset{};
    return connectedUnits;
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
  //--------------------------------------------- IS BEING GATHERED ------------------------------------------
  bool UnitImpl::isBeingGathered() const
  {
    return self->isBeingGathered;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const
  {
    return self->isBeingHealed;
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
  //--------------------------------------------- IS DETECTED ------------------------------------------------
  bool UnitImpl::isDetected() const
  {
    return self->isDetected;
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
  //--------------------------------------------- IS LIFTED --------------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return self->isLifted;
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
  //--------------------------------------------- IS SELECTED ------------------------------------------------
  bool UnitImpl::isSelected() const
  {
    return self->isSelected;
  }
  //--------------------------------------------- IS STARTING ATTACK -----------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    return self->isStartingAttack;
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
  bool UnitImpl::isPowered() const
  {
    return self->isPowered;
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool UnitImpl::isVisible(Player player) const
  {
    if (player)
      return self->isVisible[player->getID()];

    if (BroodwarImpl.self())     // isVisible for current player
      return self->isVisible[BroodwarImpl.self()->getID()];

    for (int i = 0; i < 9; ++i)
    {
      if (self->isVisible[i])
        return true;
    }
    return false;
  }
  //--------------------------------------------- IS TARGETABLE ----------------------------------------------
  bool UnitImpl::isTargetable() const
  {
    return Templates::canTargetUnit(const_cast<UnitImpl*>(this));
  }
  //--------------------------------------------- CAN COMMAND ------------------------------------------------
  bool UnitImpl::canCommand() const
  {
    return Templates::canCommand(const_cast<UnitImpl*>(this));
  }
  bool UnitImpl::canCommandGrouped(bool checkCommandibility) const
  {
    return Templates::canCommandGrouped(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN TARGET UNIT --------------------------------------------
  bool UnitImpl::canTargetUnit(Unit targetUnit, bool checkCommandibility) const
  {
    return Templates::canTargetUnit(const_cast<UnitImpl*>(this), targetUnit, checkCommandibility);
  }
  //--------------------------------------------- CAN ATTACK -------------------------------------------------
  bool UnitImpl::canAttack(bool checkCommandibility) const
  {
    return Templates::canAttack(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canAttack(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canAttack(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canAttack(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canAttack(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canAttackGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canAttackGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  bool UnitImpl::canAttackGrouped(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canAttackGrouped(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  bool UnitImpl::canAttackGrouped(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canAttackGrouped(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN ATTACK MOVE --------------------------------------------
  bool UnitImpl::canAttackMove(bool checkCommandibility) const
  {
    return Templates::canAttackMove(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canAttackMoveGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canAttackMoveGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN ATTACK UNIT --------------------------------------------
  bool UnitImpl::canAttackUnit(bool checkCommandibility) const
  {
    return Templates::canAttackUnit(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canAttackUnit(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canAttackUnit(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canAttackUnitGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canAttackUnitGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  bool UnitImpl::canAttackUnitGrouped(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canAttackUnitGrouped(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN BUILD --------------------------------------------------
  bool UnitImpl::canBuild(bool checkCommandibility) const
  {
    return Templates::canBuild(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canBuild(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canBuild(const_cast<UnitImpl*>(this), uType, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canBuild(UnitType uType, BWAPI::TilePosition tilePos, bool checkTargetUnitType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canBuild(const_cast<UnitImpl*>(this), uType, tilePos, checkTargetUnitType, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN BUILD ADDON --------------------------------------------
  bool UnitImpl::canBuildAddon(bool checkCommandibility) const
  {
    return Templates::canBuildAddon(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canBuildAddon(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canBuildAddon(const_cast<UnitImpl*>(this), uType, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN TRAIN --------------------------------------------------
  bool UnitImpl::canTrain(bool checkCommandibility) const
  {
    return Templates::canTrain(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canTrain(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canTrain(const_cast<UnitImpl*>(this), uType, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN MORPH --------------------------------------------------
  bool UnitImpl::canMorph(bool checkCommandibility) const
  {
    return Templates::canMorph(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canMorph(UnitType uType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canMorph(const_cast<UnitImpl*>(this), uType, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool UnitImpl::canResearch(bool checkCommandibility) const
  {
    return Templates::canResearch(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canResearch(TechType type, bool checkCanIssueCommandType) const
  {
    return Templates::canResearch(const_cast<UnitImpl*>(this), type, checkCanIssueCommandType);
  }
  //--------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool UnitImpl::canUpgrade(bool checkCommandibility) const
  {
    return Templates::canUpgrade(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canUpgrade(UpgradeType type, bool checkCanIssueCommandType) const
  {
    return Templates::canUpgrade(const_cast<UnitImpl*>(this), type, checkCanIssueCommandType);
  }
  //--------------------------------------------- CAN SET RALLY POINT ----------------------------------------
  bool UnitImpl::canSetRallyPoint(bool checkCommandibility) const
  {
    return Templates::canSetRallyPoint(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canSetRallyPoint(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canSetRallyPoint(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canSetRallyPoint(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canSetRallyPoint(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN SET RALLY POSITION -------------------------------------
  bool UnitImpl::canSetRallyPosition(bool checkCommandibility) const
  {
    return Templates::canSetRallyPosition(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN SET RALLY UNIT -----------------------------------------
  bool UnitImpl::canSetRallyUnit(bool checkCommandibility) const
  {
    return Templates::canSetRallyUnit(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canSetRallyUnit(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canSetRallyUnit(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN MOVE ---------------------------------------------------
  bool UnitImpl::canMove(bool checkCommandibility) const
  {
    return Templates::canMove(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canMoveGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canMoveGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN PATROL -------------------------------------------------
  bool UnitImpl::canPatrol(bool checkCommandibility) const
  {
    return Templates::canPatrol(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canPatrolGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canPatrolGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN FOLLOW -------------------------------------------------
  bool UnitImpl::canFollow(bool checkCommandibility) const
  {
    return Templates::canFollow(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canFollow(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canFollow(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN GATHER -------------------------------------------------
  bool UnitImpl::canGather(bool checkCommandibility) const
  {
    return Templates::canGather(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canGather(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canGather(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN RETURN CARGO -------------------------------------------
  bool UnitImpl::canReturnCargo(bool checkCommandibility) const
  {
    return Templates::canReturnCargo(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN HOLD POSITION ------------------------------------------
  bool UnitImpl::canHoldPosition(bool checkCommandibility) const
  {
    return Templates::canHoldPosition(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN STOP ---------------------------------------------------
  bool UnitImpl::canStop(bool checkCommandibility) const
  {
    return Templates::canStop(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN REPAIR -------------------------------------------------
  bool UnitImpl::canRepair(bool checkCommandibility) const
  {
    return Templates::canRepair(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canRepair(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canRepair(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN BURROW -------------------------------------------------
  bool UnitImpl::canBurrow(bool checkCommandibility) const
  {
    return Templates::canBurrow(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN UNBURROW -----------------------------------------------
  bool UnitImpl::canUnburrow(bool checkCommandibility) const
  {
    return Templates::canUnburrow(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CLOAK --------------------------------------------------
  bool UnitImpl::canCloak(bool checkCommandibility) const
  {
    return Templates::canCloak(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN DECLOAK ------------------------------------------------
  bool UnitImpl::canDecloak(bool checkCommandibility) const
  {
    return Templates::canDecloak(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN SIEGE --------------------------------------------------
  bool UnitImpl::canSiege(bool checkCommandibility) const
  {
    return Templates::canSiege(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN UNSIEGE ------------------------------------------------
  bool UnitImpl::canUnsiege(bool checkCommandibility) const
  {
    return Templates::canUnsiege(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN LIFT ---------------------------------------------------
  bool UnitImpl::canLift(bool checkCommandibility) const
  {
    return Templates::canLift(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN LAND ---------------------------------------------------
  bool UnitImpl::canLand(bool checkCommandibility) const
  {
    return Templates::canLand(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canLand(TilePosition target, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canLand(const_cast<UnitImpl*>(this), target, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN LOAD ---------------------------------------------------
  bool UnitImpl::canLoad(bool checkCommandibility) const
  {
    return Templates::canLoad(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canLoad(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canLoad(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN UNLOAD -------------------------------------------------
  bool UnitImpl::canUnloadWithOrWithoutTarget(bool checkCommandibility) const
  {
    return Templates::canUnloadWithOrWithoutTarget(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canUnloadAtPosition(Position targDropPos, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUnloadAtPosition(const_cast<UnitImpl*>(this), targDropPos, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canUnload(bool checkCommandibility) const
  {
    return Templates::canUnload(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canUnload(Unit targetUnit, bool checkCanTargetUnit, bool checkPosition, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUnload(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkPosition, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN UNLOAD ALL ---------------------------------------------
  bool UnitImpl::canUnloadAll(bool checkCommandibility) const
  {
    return Templates::canUnloadAll(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN UNLOAD ALL POSITION ------------------------------------
  bool UnitImpl::canUnloadAllPosition(bool checkCommandibility) const
  {
    return Templates::canUnloadAllPosition(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canUnloadAllPosition(Position targDropPos, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUnloadAllPosition(const_cast<UnitImpl*>(this), targDropPos, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN RIGHT CLICK --------------------------------------------
  bool UnitImpl::canRightClick(bool checkCommandibility) const
  {
    return Templates::canRightClick(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canRightClick(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canRightClick(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canRightClick(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canRightClick(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canRightClickGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canRightClickGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  bool UnitImpl::canRightClickGrouped(Position target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canRightClickGrouped(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  bool UnitImpl::canRightClickGrouped(Unit target, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canRightClickGrouped(const_cast<UnitImpl*>(this), target, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN RIGHT CLICK POSITION -----------------------------------
  bool UnitImpl::canRightClickPosition(bool checkCommandibility) const
  {
    return Templates::canRightClickPosition(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canRightClickPositionGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canRightClickPositionGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN RIGHT CLICK UNIT ---------------------------------------
  bool UnitImpl::canRightClickUnit(bool checkCommandibility) const
  {
    return Templates::canRightClickUnit(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canRightClickUnit(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canRightClickUnit(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canRightClickUnitGrouped(bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canRightClickUnitGrouped(const_cast<UnitImpl*>(this), checkCommandibilityGrouped, checkCommandibility);
  }
  bool UnitImpl::canRightClickUnitGrouped(Unit targetUnit, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canRightClickUnitGrouped(const_cast<UnitImpl*>(this), targetUnit, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN HALT CONSTRUCTION --------------------------------------
  bool UnitImpl::canHaltConstruction(bool checkCommandibility) const
  {
    return Templates::canHaltConstruction(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL CONSTRUCTION ------------------------------------
  bool UnitImpl::canCancelConstruction(bool checkCommandibility) const
  {
    return Templates::canCancelConstruction(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL ADDON -------------------------------------------
  bool UnitImpl::canCancelAddon(bool checkCommandibility) const
  {
    return Templates::canCancelAddon(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL TRAIN -------------------------------------------
  bool UnitImpl::canCancelTrain(bool checkCommandibility) const
  {
    return Templates::canCancelTrain(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL TRAIN SLOT --------------------------------------
  bool UnitImpl::canCancelTrainSlot(bool checkCommandibility) const
  {
    return Templates::canCancelTrainSlot(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canCancelTrainSlot(int slot, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canCancelTrainSlot(const_cast<UnitImpl*>(this), slot, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL MORPH -------------------------------------------
  bool UnitImpl::canCancelMorph(bool checkCommandibility) const
  {
    return Templates::canCancelMorph(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL RESEARCH ----------------------------------------
  bool UnitImpl::canCancelResearch(bool checkCommandibility) const
  {
    return Templates::canCancelResearch(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN CANCEL UPGRADE -----------------------------------------
  bool UnitImpl::canCancelUpgrade(bool checkCommandibility) const
  {
    return Templates::canCancelUpgrade(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  //--------------------------------------------- CAN USE TECH -----------------------------------------------
  bool UnitImpl::canUseTechWithOrWithoutTarget(bool checkCommandibility) const
  {
    return Templates::canUseTechWithOrWithoutTarget(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canUseTechWithOrWithoutTarget(BWAPI4::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTechWithOrWithoutTarget(const_cast<UnitImpl*>(this), tech, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canUseTech(BWAPI4::TechType tech, Unit target, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTech(const_cast<UnitImpl*>(this), tech, target, checkCanTargetUnit, checkTargetsType, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canUseTech(BWAPI4::TechType tech, Position target, bool checkCanTargetUnit, bool checkTargetsType, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTech(const_cast<UnitImpl*>(this), tech, target, checkCanTargetUnit, checkTargetsType, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canUseTechWithoutTarget(BWAPI4::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTechWithoutTarget(const_cast<UnitImpl*>(this), tech, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN USE TECH UNIT ------------------------------------------
  bool UnitImpl::canUseTechUnit(BWAPI4::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTechUnit(const_cast<UnitImpl*>(this), tech, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canUseTechUnit(BWAPI4::TechType tech, Unit targetUnit, bool checkCanTargetUnit, bool checkTargetsUnits, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTechUnit(const_cast<UnitImpl*>(this), tech, targetUnit, checkCanTargetUnit, checkTargetsUnits, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN USE TECH POSITION --------------------------------------
  bool UnitImpl::canUseTechPosition(BWAPI4::TechType tech, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTechPosition(const_cast<UnitImpl*>(this), tech, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canUseTechPosition(BWAPI4::TechType tech, Position target, bool checkTargetsPositions, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canUseTechPosition(const_cast<UnitImpl*>(this), tech, target, checkTargetsPositions, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN PLACE COP ----------------------------------------------
  bool UnitImpl::canPlaceCOP(bool checkCommandibility) const
  {
    return Templates::canPlaceCOP(const_cast<UnitImpl*>(this), checkCommandibility);
  }
  bool UnitImpl::canPlaceCOP(TilePosition target, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canPlaceCOP(const_cast<UnitImpl*>(this), target, checkCanIssueCommandType, checkCommandibility);
  }
  //--------------------------------------------- CAN ISSUE COMMAND TYPE -------------------------------------
  bool UnitImpl::canIssueCommandType(BWAPI4::UnitCommandType ct, bool checkCommandibility) const
  {
    return Templates::canIssueCommandType(const_cast<UnitImpl*>(this), ct, checkCommandibility);
  }
  bool UnitImpl::canIssueCommandTypeGrouped(BWAPI4::UnitCommandType ct, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canIssueCommandTypeGrouped(const_cast<UnitImpl*>(this), ct, checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- CAN ISSUE COMMAND ------------------------------------------
  bool UnitImpl::canIssueCommand(UnitCommand command, bool checkCanUseTechPositionOnPositions, bool checkCanUseTechUnitOnUnits, bool checkCanBuildUnitType, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibility) const
  {
    return Templates::canIssueCommand(const_cast<UnitImpl*>(this), command, checkCanUseTechPositionOnPositions, checkCanUseTechUnitOnUnits, checkCanBuildUnitType, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibility);
  }
  bool UnitImpl::canIssueCommandGrouped(UnitCommand command, bool checkCanUseTechPositionOnPositions, bool checkCanUseTechUnitOnUnits, bool checkCanTargetUnit, bool checkCanIssueCommandType, bool checkCommandibilityGrouped, bool checkCommandibility) const
  {
    return Templates::canIssueCommandGrouped(const_cast<UnitImpl*>(this), command, checkCanUseTechPositionOnPositions, checkCanUseTechUnitOnUnits, checkCanTargetUnit, checkCanIssueCommandType, checkCommandibilityGrouped, checkCommandibility);
  }
  //--------------------------------------------- GET TILE POSITION ------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(abs(this->getPosition().x - this->getType().tileWidth() * 32 / 2),
      abs(this->getPosition().y - this->getType().tileHeight() * 32 / 2)));
  }
  //--------------------------------------------- GET DISTANCE -----------------------------------------------
  int UnitImpl::getDistance(Position target) const
  {
    // If this unit does not exist or target is invalid
    if (!exists() || !BroodwarImpl.isValid(target))
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
  int UnitImpl::getDistance(Unit target) const
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
  bool UnitImpl::hasPath(Position target) const
  {
    BroodwarImpl.setLastError();
    // Return error if the position is invalid
    if (!BroodwarImpl.isValid(target))
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
  bool UnitImpl::hasPath(Unit target) const
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
  int UnitImpl::getLeft() const
  {
    return this->getPosition().x - this->getType().dimensionLeft();
  }
  //--------------------------------------------- GET TOP ----------------------------------------------------
  int UnitImpl::getTop() const
  {
    return this->getPosition().y - this->getType().dimensionUp();
  }
  //--------------------------------------------- GET RIGHT --------------------------------------------------
  int UnitImpl::getRight() const
  {
    return this->getPosition().x + this->getType().dimensionRight();
  }
  //--------------------------------------------- GET BOTTOM -------------------------------------------------
  int UnitImpl::getBottom() const
  {
    return this->getPosition().y + this->getType().dimensionDown();
  }
  //--------------------------------------------- IS BEING CONSTRUCTED ---------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    if (this->isMorphing())
      return true;
    if (this->isCompleted())
      return false;
    if (this->getType().getRace() != Races::Terran)
      return true;
    return this->getBuildUnit() != nullptr;
  }
  // ------------------------------------------ STATUS ---------------------------------------------
  bool UnitImpl::isDefenseMatrixed() const
  {
    return this->getDefenseMatrixTimer() != 0;
  }

  bool UnitImpl::isEnsnared() const
  {
    return this->getEnsnareTimer() != 0;
  }

  bool UnitImpl::isFollowing() const
  {
    return this->getOrder() == Orders::Follow;
  }
  bool UnitImpl::isFlying() const
  {
    return this->getType().isFlyer() || this->isLifted();
  }

  bool UnitImpl::isHoldingPosition() const
  {
    return this->getOrder() == Orders::HoldPosition;
  }

  //--------------------------------------------- IS IN WEAPON RANGE -----------------------------------------
  bool UnitImpl::isInWeaponRange(Unit target) const
  {
    // Preliminary checks
    if (!this->exists() || !target || !target->exists() || this == target)
      return false;

    // Store the types as locals
    UnitType thisType = getType();
    UnitType targType = target->getType();

    // Obtain the weapon type
    WeaponType wpn = target->isFlying() ? thisType.airWeapon() : thisType.groundWeapon();

    // Return if there is no weapon type
    if (wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown)
      return false;

    // Retrieve the min and max weapon ranges
    int minRange = wpn.minRange();
    int maxRange = getPlayer()->weaponMaxRange(wpn);

    // Check if the distance to the unit is within the weapon range
    int distance = this->getDistance(target);
    return (minRange ? minRange < distance : true) && distance <= maxRange;
  }

  bool UnitImpl::isIrradiated() const
  {
    return this->getIrradiateTimer() != 0;
  }

  bool UnitImpl::isLoaded() const
  {
    return this->getTransport() != nullptr;
  }

  bool UnitImpl::isLockedDown() const
  {
    return this->getLockdownTimer() != 0;
  }

  bool UnitImpl::isMaelstrommed() const
  {
    return this->getMaelstromTimer() != 0;
  }

  bool UnitImpl::isPatrolling() const
  {
    return this->getOrder() == Orders::Patrol;
  }

  bool UnitImpl::isPlagued() const
  {
    return this->getPlagueTimer() != 0;
  }

  bool UnitImpl::isRepairing() const
  {
    return this->getOrder() == Orders::Repair;
  }

  bool UnitImpl::isResearching() const
  {
    return this->getOrder() == Orders::ResearchTech;
  }

  bool UnitImpl::isSieged() const
  {
    UnitType t(this->getType());
    return  t == UnitTypes::Terran_Siege_Tank_Siege_Mode ||
      t == UnitTypes::Hero_Edmund_Duke_Siege_Mode;
  }

  bool UnitImpl::isStasised() const
  {
    return this->getStasisTimer() != 0;
  }

  bool UnitImpl::isStimmed() const
  {
    return this->getStimTimer() != 0;
  }

  bool UnitImpl::isUpgrading() const
  {
    return this->getOrder() == Orders::Upgrade;
  }

  int UnitImpl::getSpaceRemaining() const
  {
    int space = this->getType().spaceProvided();

    // Decrease the space for each loaded unit
    for (auto &u : getLoadedUnits())
      space -= u->getType().spaceRequired();

    return std::max(space, 0);
  }

};
