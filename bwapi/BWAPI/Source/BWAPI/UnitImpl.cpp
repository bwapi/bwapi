#include "UnitImpl.h"

#include <math.h>

#include <Util/Logger.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include "Globals.h"
#include "CommandTrain.h"
#include "CommandBuild.h"
#include "CommandAttackMove.h"
#include "CommandAttackUnit.h"
#include "CommandPatrol.h"
#include "CommandHoldPosition.h"
#include "CommandStop.h"
#include "CommandRightClick.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"
#include "CommandRepair.h"
#include "CommandMorphUnit.h"
#include "CommandMorphBuilding.h"
#include "CommandBurrow.h"
#include "CommandUnburrow.h"
#include "CommandCloak.h"
#include "CommandDecloak.h"
#include "CommandSiege.h"
#include "CommandUnsiege.h"
#include "CommandLift.h"
#include "CommandLand.h"
#include "CommandLoad.h"
#include "CommandUnload.h"
#include "CommandUnloadAll.h"
#include "CommandCancelResearch.h"
#include "CommandCancelUpgrade.h"
#include "CommandCancelConstruction.h"
#include "CommandCancelTrain.h"
#include "CommandCancelMorph.h"
#include "CommandCancelAddon.h"
#include "CommandFollow.h"
#include "CommandSetRally.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitType.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitImpl::UnitImpl(BW::Unit* unitData,
                     BW::Unit* originalUnit,
                     BW::Unit* unitDataLocal,
                     u16 index)
      : bwUnit(unitData)
      , bwOriginalUnit(originalUnit)
      , bwUnitLocal(unitDataLocal)
      , index(index)
      , userSelected(false)
      , buildUnit(NULL)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int UnitImpl::getHitPoints() const
  {
    return this->getRawDataLocal()->healthPoints;
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int UnitImpl::getShields() const
  {
    if (this->getBWType().maxShields()>0)
    {
      return this->getRawDataLocal()->shieldPoints;
    }
    return 0;
  }
  //------------------------------------------- GET ENERGY POINTS --------------------------------------------
  int UnitImpl::getEnergy() const
  {
    if (this->getBWType().isSpellcaster())
    {
      return this->getRawDataLocal()->energy;
    }
    return 0;
  }
  //----------------------------------------------- RESOURCES ------------------------------------------------
  int UnitImpl::getResources() const
  {
    if (this->getBWType() != BW::UnitID::Resource_MineralPatch1 &&
        this->getBWType() != BW::UnitID::Resource_MineralPatch2 &&
        this->getBWType() != BW::UnitID::Resource_MineralPatch3 &&
        this->getBWType() != BW::UnitID::Resource_VespeneGeyser &&
        this->getBWType() != BW::UnitID::Terran_Refinery &&
        this->getBWType() != BW::UnitID::Protoss_Assimilator &&
        this->getBWType() != BW::UnitID::Zerg_Extractor)
    {
      return 0;
    }
    return this->getRawDataLocal()->unitUnion1.unitUnion1Sub.resourceUnitUnionSub.resourceContained;
  }
  //-------------------------------------------- GET KILL COUNT ----------------------------------------------
  int UnitImpl::getKillCount() const
  {
    return this->getRawDataLocal()->killCount;
  }
  //----------------------------------------- GROUND WEAPON COOLDOWN -----------------------------------------
  int UnitImpl::getGroundWeaponCooldown() const
  {
    return this->getRawDataLocal()->groundWeaponCooldown;
  }
  //------------------------------------------ AIR WEAPON COOLDOWN -------------------------------------------
  int UnitImpl::getAirWeaponCooldown() const
  {
    return this->getRawDataLocal()->airWeaponCooldown;
  }
  //--------------------------------------------- SPELL COOLDOWN ---------------------------------------------
  int UnitImpl::getSpellCooldown() const
  {
    return this->getRawDataLocal()->spellCooldown;
  }
  //------------------------------------------ DEFENSE MATRIX POINTS -----------------------------------------
  int UnitImpl::getDefenseMatrixPoints() const
  {
    return this->getRawDataLocal()->defenseMatrixDamage;
  }
  //------------------------------------------ DEFENSE MATRIX TIMER ------------------------------------------
  int UnitImpl::getDefenseMatrixTimer() const
  {
    return this->getRawDataLocal()->defenseMatrixTimer;
  }
  //----------------------------------------------- STIM TIMER -----------------------------------------------
  int UnitImpl::getStimTimer() const
  {
    return this->getRawDataLocal()->stimTimer;
  }
  //---------------------------------------------- ENSNARE TIMER ---------------------------------------------
  int UnitImpl::getEnsnareTimer() const
  {
    return this->getRawDataLocal()->ensareTimer;
  }
  //--------------------------------------------- LOCKDOWN TIMER ---------------------------------------------
  int UnitImpl::getLockdownTimer() const
  {
    return this->getRawDataLocal()->lockdownTimer;
  }
  //--------------------------------------------- IRRADIATE TIMER --------------------------------------------
  int UnitImpl::getIrradiateTimer() const
  {
    return this->getRawDataLocal()->irradiateTimer;
  }
  //---------------------------------------------- STASIS TIMER ----------------------------------------------
  int UnitImpl::getStasisTimer() const
  {
    return this->getRawDataLocal()->stasisTimer;
  }
  //---------------------------------------------- PLAGUE TIMER ----------------------------------------------
  int UnitImpl::getPlagueTimer() const
  {
    return this->getRawDataLocal()->plagueTimer;
  }
  //--------------------------------------------- MAELSTROM TIMER --------------------------------------------
  int UnitImpl::getMaelstromTimer() const
  {
    return this->getRawDataLocal()->maelstromTimer;
  }
  //---------------------------------------------- REMOVE TIMER ----------------------------------------------
  int UnitImpl::getRemoveTimer() const
  {
    return this->getRawDataLocal()->removeTimer;
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* UnitImpl::getPlayer() const
  {
    if (this->getRawDataLocal()->playerID < 12)
      return (Player*)BroodwarImpl.players[this->bwUnit->playerID];
    else
      return NULL;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitImpl::isValid() const
  {
    if (this->getRawDataLocal()->playerID > 11)
      return false;
    if (this->isMineral())
      return  !this->getRawDataLocal()->orderFlags.getBit(BW::OrderFlags::willWanderAgain);
    else
      return this->getHitPoints() > 0 && this->getType() != BWAPI::UnitTypes::None && this->getType() != BWAPI::UnitTypes::Unknown;
  }
  //------------------------------------------------ IS READY ------------------------------------------------
  bool UnitImpl::isReady() const
  {
    return this->isValid() &&
           this->isCompleted();
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Accelerating);
  }
  //-------------------------------------------- IS BEING CONSTRUCTED ----------------------------------------
  bool UnitImpl::isBeingConstructed() const
  {
    if (!this->isCompleted() && this->getType().isBuilding())
      return this->buildUnit != NULL || this->getType().getRace() != Races::Terran;
    return false;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const // Not working right now
  {
    return this->getRawDataLocal()->isBeingHealed != 0;
  }
  //------------------------------------------------ IS BLIND ------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    return this->getRawDataLocal()->isBlind != 0;
  }
  //----------------------------------------------- IS BRAKING -----------------------------------------------
  bool UnitImpl::isBraking() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Braking);
  }
  //----------------------------------------------- IS BURROWED ----------------------------------------------
  bool UnitImpl::isBurrowed() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Burrowed);
  }
  //------------------------------------------------ IS CLOAKED ----------------------------------------------
  bool UnitImpl::isCloaked() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Cloaked);
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Completed);
  }
  //--------------------------------------------- IS CONSTRUCTING --------------------------------------------
  bool UnitImpl::isConstructing() const
  {
    return this->getBWOrder() == BW::OrderID::ConstructingBuilding;
  }
  //---------------------------------------------- IS FOLLOWING ----------------------------------------------
  bool UnitImpl::isFollowing() const
  {
    return this->getBWOrder() == BW::OrderID::Follow;
  }
  //-------------------------------------------- IS HALLUCINATION --------------------------------------------
  bool UnitImpl::isHallucination() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::IsHallucination);
  }
  //---------------------------------------------- IS IDLE ---------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return (this->getBWOrder() == BW::OrderID::PlayerGuard ||
            this->getBWOrder() == BW::OrderID::Guard ||
            this->getBWOrder() == BW::OrderID::Stop ||
            this->getBWOrder() == BW::OrderID::Pickup1 ||
            this->getBWOrder() == BW::OrderID::Nothing2 ||
            this->getBWOrder() == BW::OrderID::Medic ||
            this->getBWOrder() == BW::OrderID::Carrier ||
            this->getBWOrder() == BW::OrderID::Critter ||
            this->getBWOrder() == BW::OrderID::NukeTrain);
  }
  //------------------------------------------------ IS LIFTED -----------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InAir) &&
           this->getRawDataLocal()->unitID.isBuilding();
  }
  //------------------------------------------------ IS LOADED -----------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InTransport)
           || this->getRawDataLocal()->status.getBit(BW::StatusFlags::InBuilding);
  }
  //---------------------------------------------- IS LOCKED DOWN --------------------------------------------
  bool UnitImpl::isLockedDown() const
  {
    return this->getLockdownTimer() > 0;
  }
  //----------------------------------------------- IS MORPHING ----------------------------------------------
  bool UnitImpl::isMorphing() const
  {
    return this->getBWOrder() == BW::OrderID::Morph1 || this->getBWOrder() == BW::OrderID::Morph2 || this->getBWOrder() == BW::OrderID::ZergBuildSelf;
  }
  //------------------------------------------------ IS MOVING -----------------------------------------------
  bool UnitImpl::isMoving() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Moving);
  }
  //---------------------------------------------- IS PATROLLING ---------------------------------------------
  bool UnitImpl::isPatrolling() const
  {
    return this->getBWOrder() == BW::OrderID::Patrol;
  }
  //----------------------------------------------- IS REPAIRING ---------------------------------------------
  bool UnitImpl::isRepairing() const
  {
    return this->getBWOrder() == BW::OrderID::Repair1 || this->getBWOrder() == BW::OrderID::Repair2;
  }
  //---------------------------------------------- IS RESEARCHING --------------------------------------------
  bool UnitImpl::isResearching() const
  {
    return this->getBWOrder() == BW::OrderID::ResearchTech;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSelected() const
  {
    if (BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput) == false)
      return false;
    return this->userSelected;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSieged() const
  {
    return this->getBWType().getID() == BW::UnitID::Terran_SiegeTankSiegeMode;
  }
  //------------------------------------------- IS STARTING ATTACK -------------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::StartingAttack);
  }
  //----------------------------------------------- IS STASISED ----------------------------------------------
  bool UnitImpl::isStasised() const
  {
    return this->getStasisTimer() > 0;
  }
  //---------------------------------------------- IS TRAINING -----------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return !this->hasEmptyBuildQueue();
  }
  //---------------------------------------------- IS UNDER STORM --------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    return this->getRawDataLocal()->isUnderStorm != 0;
  }
  //------------------------------------------------ IS UNPOWERED --------------------------------------------
  bool UnitImpl::isUnpowered() const
  {
    if (this->getBWType().getRace() == BW::Race::Protoss && this->getBWType().isBuilding())
    {
      return this->getRawDataLocal()->status.getBit(BW::StatusFlags::DoodadStatesThing);
    }
    return false;
  }
  //----------------------------------------------- IS UPGRADING ---------------------------------------------
  bool UnitImpl::isUpgrading() const
  {
    return this->getBWOrder() == BW::OrderID::Upgrade;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool UnitImpl::isVisible() const
  {
    if (this->getPlayer() == BWAPI::BroodwarImpl.self())
    {
      return true;
    }
    if (!BWAPI::BroodwarImpl.visible(this->getTilePosition().x(), this->getTilePosition().y()))
    {
      return false;
    }
    /* TODO: Also need to check for detection (overlords, observers, Terran scans, etc) */
    if (this->isBurrowed() || this->isCloaked())
    {
      return false;
    }
    return true;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    this->userSelected = selectedState;
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  Position UnitImpl::getPosition() const
  {
    return BWAPI::Position(this->getRawDataLocal()->position.x, this->getRawDataLocal()->position.y);
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(this->getPosition().x() - this->getType().tileWidth() * BW::TILE_SIZE / 2,
                                 this->getPosition().y() - this->getType().tileHeight() * BW::TILE_SIZE / 2));
  }
  //----------------------------------------------- GET TARGET -----------------------------------------------
  Unit* UnitImpl::getTarget() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->targetUnit);
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  Unit* UnitImpl::getOrderTarget() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* UnitImpl::getBuildUnit() const
  {
    if (this->getRawDataLocal()->currentBuildUnit)
      return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
    return (Unit*)this->buildUnit;
  }
  //----------------------------------------------- GET CHILD ------------------------------------------------
  Unit* UnitImpl::getChild() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
  }
  //------------------------------------------ GET TARGET POSITION -------------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    return BWAPI::Position(this->getRawDataLocal()->moveToPos.x, this->getRawDataLocal()->moveToPos.y);
  }
  //-------------------------------------------- CURRENT DIRECTION -------------------------------------------
  int UnitImpl::getCurrentDirection() const
  {
    return this->getRawDataLocal()->currentDirection;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::Unit* UnitImpl::getRawData() const
  {
    return this->bwUnit;
  }
  //------------------------------------------- GET RAW DATA LOCAL -------------------------------------------
  BW::Unit* UnitImpl::getRawDataLocal() const
  {
    return this->bwUnitLocal;
  }
  //----------------------------------------- GET ORIGINAL RAW DATA ------------------------------------------
  BW::Unit* UnitImpl::getOriginalRawData() const
  {
    return this->bwOriginalUnit;
  }
  //---------------------------------------------- GET ORDER ID ----------------------------------------------
  Order UnitImpl::getOrder() const
  {
    return BWAPI::Order(this->getRawDataLocal()->orderID);
  }
  //---------------------------------------------- GET ORDER ID ----------------------------------------------
  BW::OrderID::Enum UnitImpl::getBWOrder() const
  {
    return this->getRawDataLocal()->orderID;
  }
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return BWAPI::Order(this->getRawDataLocal()->secondaryOrderID);
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------

#pragma warning(push)
#pragma warning(disable:4244)
  u16 UnitImpl::getDistance(Unit* unit) const
  {
    u32 result;
    if (unit == this)
      return 0;
    if (this->getPosition().y() - this->getType().dimensionUp() <= unit->getPosition().y() + unit->getType().dimensionDown())
      if (this->getPosition().y() + this->getType().dimensionDown() >= unit->getPosition().y() - unit->getType().dimensionUp())
        if (this->getPosition().x() > unit->getPosition().x())
          result = this->getPosition().x() - this->getType().dimensionLeft() - unit->getPosition().x() - unit->getType().dimensionRight();
        else
          result = unit->getPosition().x() - unit->getType().dimensionRight() - this->getPosition().x() - this->getType().dimensionLeft();

    if (this->getPosition().x() - this->getType().dimensionLeft() <= unit->getPosition().x() + unit->getType().dimensionRight())
      if (this->getPosition().x() + this->getType().dimensionRight() >= unit->getPosition().x() - unit->getType().dimensionLeft())
        if (this->getPosition().y() > unit->getPosition().y())
          result = this->getPosition().y() - this->getType().dimensionUp() - unit->getPosition().y() - unit->getType().dimensionDown();
        else
          result = unit->getPosition().y() - unit->getType().dimensionDown() - this->getPosition().y() - this->getType().dimensionUp();

    if (this->getPosition().x() > unit->getPosition().x())
      if (this->getPosition().y() > unit->getPosition().y())
        result = this->getDistance(this->getPosition().x() - this->getType().dimensionLeft(),
                                   this->getPosition().y() - this->getType().dimensionUp(),
                                   unit->getPosition().x() + unit->getType().dimensionRight(),
                                   unit->getPosition().y() + unit->getType().dimensionDown());
      else
        result = this->getDistance(this->getPosition().x() - this->getType().dimensionLeft(),
                                   this->getPosition().y() + this->getType().dimensionDown(),
                                   unit->getPosition().x() + unit->getType().dimensionRight(),
                                   unit->getPosition().y() - unit->getType().dimensionUp());
    else if (this->getPosition().y() > unit->getPosition().y())
      result = this->getDistance(this->getPosition().x() + this->getType().dimensionRight(),
                                 this->getPosition().y() - this->getType().dimensionUp(),
                                 unit->getPosition().x() - unit->getType().dimensionLeft(),
                                 unit->getPosition().y() + unit->getType().dimensionDown());
    else
      result = this->getDistance(this->getPosition().x() + this->getType().dimensionRight(),
                                 this->getPosition().y() + this->getType().dimensionDown(),
                                 unit->getPosition().x() - unit->getType().dimensionLeft(),
                                 unit->getPosition().y() - unit->getType().dimensionUp());
    if (result > 0)
      return result;
    else
      return 0;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 UnitImpl::getDistance(BW::Position position) const
  {
    return this->getDistance(this->getPosition().x(),
                             this->getPosition().y(),
                             position.x,
                             position.y);
  }
  //------------------------------------------ GET CENTER DISTANCE -------------------------------------------
  u16 UnitImpl::getCenterDistance(Unit* unit) const
  {
    return this->getDistance(this->getPosition().x(),
                             this->getPosition().y(),
                             unit->getPosition().x(),
                             unit->getPosition().y());
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 UnitImpl::getDistance(int x1, int y1, int x2, int y2) const
  {
    return sqrt((long double)((long double)x1 - x2) * ((long double)x1 - x2) +
                (long double)((long double)y1 - y2) * ((long double)y1 - y2));
  }
#pragma warning(pop)
  //------------------------------------------ GET TRAINING QUEUE --------------------------------------------
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainList;
    if (this->hasEmptyBuildQueue()) return trainList;
    int i = this->getBuildQueueSlot() % 5;
    int starti = i;
    trainList.push_front(BWAPI::UnitType(this->getBuildQueue()[i].id));
    i = (i + 1) % 5;
    while(this->getBuildQueue()[i] != BW::UnitID::None && i != starti)
    {
      trainList.push_back(BWAPI::UnitType(this->getBuildQueue()[i].id));
      i = (i + 1) % 5;
    }
    return trainList;
  }
  //-------------------------------------------- GET TRANSPORT -----------------------------------------------
  Unit* UnitImpl::getTransport() const
  {
    if (!this->isLoaded()) return NULL;
    return (Unit*)(UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->connectedUnit));
  }
  //------------------------------------------- GET LOADED UNITS ---------------------------------------------
  std::list<Unit*> UnitImpl::getLoadedUnits() const
  {
    std::list<Unit*> unitList;
    for(int i = 0; i < 8; i++)
    {
      if (this->getRawDataLocal()->loadedUnitIndex[i] != 0)
      {
        BW::Unit* bwunit = (BW::Unit*)(0x4F4B58 + this->getRawDataLocal()->loadedUnitIndex[i] * BW::UNIT_SIZE_IN_BYTES);
        UnitImpl* unit = BWUnitToBWAPIUnit(bwunit);
        if (unit!=NULL)
        {
          unitList.push_back((Unit*)unit);
        }
      }
    }
    return unitList;
  }
  //----------------------------------------------- GET TECH -------------------------------------------------
  TechType UnitImpl::getTech() const
  {
    int techID = this->getRawDataLocal()->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion1.techID;
    return TechType(techID);
  }
  //---------------------------------------------- GET UPGRADE -----------------------------------------------
  UpgradeType UnitImpl::getUpgrade() const
  {
    int upgradeID = this->getRawDataLocal()->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion2.upgradeID;
    return UpgradeType(upgradeID);
  }
  //-------------------------------------- GET REMAINING RESEARCH TIME ---------------------------------------
  int UnitImpl::getRemainingResearchTime() const
  {
    if (this->isResearching())
      return this->getRawDataLocal()->childUnitUnion1.unitIsBuilding.upgradeResearchTime;
    return 0;
  }
  //-------------------------------------- GET REMAINING UPGRADE TIME ----------------------------------------
  int UnitImpl::getRemainingUpgradeTime() const
  {
    if (this->isUpgrading())
      return this->getRawDataLocal()->childUnitUnion1.unitIsBuilding.upgradeResearchTime;
    return 0;
  }
  //------------------------------------------ GET RALLY POSITION --------------------------------------------
  Position UnitImpl::getRallyPosition() const
  {
    if (this->getBWType().canProduce())
      return Position(this->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyX,
                      this->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyY);
    return Positions::None;
  }
  //-------------------------------------------- GET RALLY UNIT ----------------------------------------------
  Unit* UnitImpl::getRallyUnit() const
  {
    if (this->getBWType().canProduce())
      return (Unit*)UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyUnit);
    return NULL;
  }
  //----------------------------------------------- GET ADDON ------------------------------------------------
  Unit* UnitImpl::getAddon() const
  {
    if (this->getType().isBuilding())
    {
      if (this->getRawDataLocal()->currentBuildUnit != NULL)
      {
        if (UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit)->getBWType().isAddon())
        {
          return  (Unit*)UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
        }
      }
      if (this->getRawDataLocal()->childInfoUnion.childUnit1 != NULL)
      {
        return (Unit*)UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
      }
    }
    return NULL;
  }
  //-------------------------------------------- HAS EMPTY QUEUE ---------------------------------------------
  bool UnitImpl::hasEmptyBuildQueueSync() const
  {
    return this->getBuildQueueSync()[this->getBuildQueueSlotSync()] == BW::UnitID::None;
  }
  //----------------------------------------- HAS EMPTY QUEUE LOCAL ------------------------------------------
  bool UnitImpl::hasEmptyBuildQueue() const
  {
    if (this->getBuildQueueSlot() < 5)
      return this->getBuildQueue()[this->getBuildQueueSlot()] == BW::UnitID::None;
    else
      return false;
  }
  //----------------------------------------- HAS FULL QUEUE LOCAL -------------------------------------------
  bool UnitImpl::hasFullBuildQueue() const
  {
    return this->getBuildQueue()[(this->getBuildQueueSlot() + 1) % 5] != BW::UnitID::None;
  }
  //------------------------------------------- ORDER Attack Location ----------------------------------------
  bool UnitImpl::attackMove(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandAttackMove(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //--------------------------------------------- ORDER Attack Unit ------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::AttackUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandAttackUnit(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position(position.x(), position.y())), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, (UnitImpl*)target));
    return true;
  }
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    BW::UnitType type((BW::UnitID::Enum)type1.getID());
    if (this->getType() == BWAPI::UnitTypes::Zerg_Larva ||
        this->getType() == BWAPI::UnitTypes::Zerg_Mutalisk ||
        this->getType() == BWAPI::UnitTypes::Zerg_Hydralisk)
    {
      this->orderSelect();
      BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnitMorph(type), 0x3);
    }
    else if (this->getType() == BWAPI::UnitTypes::Zerg_Hatchery ||
             this->getType() == BWAPI::UnitTypes::Zerg_Lair ||
             this->getType() == BWAPI::UnitTypes::Zerg_Spire ||
             this->getType() == BWAPI::UnitTypes::Zerg_Creep_Colony)
    {
      this->orderSelect();
      BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(type), 0x3);
    }
    else
    {
      this->orderSelect();
      BroodwarImpl.addToCommandBuffer(new CommandTrain(this, type));
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type), 0x3);
    }
    return true;
  }
  //------------------------------------------------- BUILD --------------------------------------------------
  bool UnitImpl::build(TilePosition position, UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!type1.isBuilding() || *type1.whatBuilds().first != this->getType())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    BW::UnitType type(BW::UnitID::Enum(type1.getID()));
    this->orderSelect();
    if (!type.isAddon())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition(position.x(), position.y()), type), sizeof(BW::Orders::MakeBuilding));
    else
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition(position.x(), position.y()), type), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new CommandBuild(this, type, BW::TilePosition(position.x(), position.y())));
    return true;
  }
  //------------------------------------------------ RESEARCH ------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (*tech.whatResearches() != this->getType())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_TechType);
      return false;
    }
    this->orderSelect();
    BW::TechID::Enum techenum = static_cast<BW::TechID::Enum>(tech.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new CommandInvent(this, BW::TechType(techenum)));
    return true;
  }
  //------------------------------------------------- UPGRADE ------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (*upgrade.whatUpgrades() != this->getType())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UpgradeType);
      return false;
    }
    this->orderSelect();
    BW::UpgradeID::Enum upgradeenum = static_cast<BW::UpgradeID::Enum>(upgrade.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade));
    BroodwarImpl.addToCommandBuffer(new CommandUpgrade(this, BW::UpgradeType(upgradeenum)));
    return true;
  }
  //-------------------------------------------------- STOP --------------------------------------------------
  bool UnitImpl::stop()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    this->getRawDataLocal()->orderID = BW::OrderID::Stop;
    BroodwarImpl.addToCommandBuffer(new CommandStop(this));
    return true;
  }
  //---------------------------------------------- HOLD POSITION ---------------------------------------------
  bool UnitImpl::holdPosition()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    switch (this->getBWType().getID())
    {
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition1;
        break;
      case BW::UnitID::Zerg_Queen:
      case BW::UnitID::Zerg_Hero_Matriarch:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition3;
        break;
      case BW::UnitID::Zerg_InfestedTerran:
      case BW::UnitID::Zerg_Scourge:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition4;
        break;
      case BW::UnitID::Terran_Medic:
        this->getRawDataLocal()->orderID = BW::OrderID::MedicHoldPosition;
        break;
      case BW::UnitID::Protoss_Reaver:
        this->getRawDataLocal()->orderID = BW::OrderID::ReaverHold;
        break;
      default:
        this->getRawDataLocal()->orderID = BW::OrderID::HoldPosition2;
    }
    BroodwarImpl.addToCommandBuffer(new CommandHoldPosition(this));
    return true;
  }
  //-------------------------------------------------- PATROL ------------------------------------------------
  bool UnitImpl::patrol(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandPatrol(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //-------------------------------------------------- FOLLOW ------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandFollow(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- SET RALLY ----------------------------------------------
  bool UnitImpl::setRallyPosition(Position target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(target.x(), target.y()), BW::OrderID::RallyPoint2), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandSetRally(this, BW::Position(target.x(), target.y())));
    return true;
  }
  //------------------------------------------------- SET RALLY ----------------------------------------------
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPoint1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandSetRally(this, (UnitImpl*)target));
    return true;
  }
  //-------------------------------------------------- REPAIR ------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_SCV || !target->getType().isMechanical())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandRepair(this, (UnitImpl*)target));
    return true;
  }
  //-------------------------------------------------- MORPH -------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().getRace() != Races::Zerg || *type.whatBuilds().first != this->getType())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BW::UnitType rawtype(((BW::UnitID::Enum)type.getID()));
    if(type.isBuilding())
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(rawtype), sizeof(BW::Orders::BuildingMorph));
      BroodwarImpl.addToCommandBuffer(new CommandMorphBuilding(this, rawtype));
    }
    else
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnitMorph(rawtype), sizeof(BW::Orders::UnitMorph));
      BroodwarImpl.addToCommandBuffer(new CommandMorphUnit(this, rawtype));
    }
    return true;
  }
  //-------------------------------------------------- BURROW ------------------------------------------------
  bool UnitImpl::burrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if(!this->isBurrowed())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
      BroodwarImpl.addToCommandBuffer(new CommandBurrow(this));
    }
    return true;
  }
  //------------------------------------------------- UNBURROW -----------------------------------------------
  bool UnitImpl::unburrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if(this->isBurrowed())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
      BroodwarImpl.addToCommandBuffer(new CommandUnburrow(this));
    }
    return true;
  }
  //-------------------------------------------------- SIEGE -------------------------------------------------
  bool UnitImpl::siege()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_Siege_Tank_Tank_Mode && this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }

    if (!this->isSieged())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
      BroodwarImpl.addToCommandBuffer(new CommandSiege(this));
    }
    return true;
  }
  //------------------------------------------------- UNSIEGE ------------------------------------------------
  bool UnitImpl::unsiege()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() != UnitTypes::Terran_Siege_Tank_Tank_Mode && this->getType() != UnitTypes::Terran_Siege_Tank_Siege_Mode)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isSieged())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
      BroodwarImpl.addToCommandBuffer(new CommandUnsiege(this));
    }
    return true;
  }
  //-------------------------------------------------- CLOAK -------------------------------------------------
  bool UnitImpl::cloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if(!this->isCloaked())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
      BroodwarImpl.addToCommandBuffer(new CommandCloak(this));
    }
    return true;
  }
  //------------------------------------------------- DECLOAK ------------------------------------------------
  bool UnitImpl::decloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if(this->isCloaked())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
      BroodwarImpl.addToCommandBuffer(new CommandDecloak(this));
    }
    return true;
  }
  //--------------------------------------------------- LIFT -------------------------------------------------
  bool UnitImpl::lift()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(!this->isLifted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
      BroodwarImpl.addToCommandBuffer(new CommandLift(this));
    }
    return true;
  }
  //--------------------------------------------------- LAND -------------------------------------------------
  bool UnitImpl::land(TilePosition position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(this->isLifted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Land(BW::TilePosition(position.x(), position.y()), this->getBWType()), sizeof(BW::Orders::Land));
      BroodwarImpl.addToCommandBuffer(new CommandLand(this, BW::TilePosition(position.x(), position.y())));
    }
    return true;
  }
  //--------------------------------------------------- LOAD -------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    bool loaded = false;
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Pickup3), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Pickup2), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (target->getType() == UnitTypes::Terran_Bunker || target->getType() == UnitTypes::Terran_Dropship || target->getType() == UnitTypes::Protoss_Shuttle || target->getType() == UnitTypes::Zerg_Overlord)
    {
      this->rightClick(target);
      loaded = true;
    }
    if (loaded)
    {
      BroodwarImpl.addToCommandBuffer(new CommandLoad(this, (UnitImpl*)target));
      return true;
    }
    //if neither this unit nor the target unit is a bunker, dropship, shuttle, or overlord, return false.
    BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    return false;
  }
  //-------------------------------------------------- UNLOAD ------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)target), sizeof(BW::Orders::UnloadUnit));
    BroodwarImpl.addToCommandBuffer(new CommandUnload(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      return this->unloadAll(this->getPosition());
    }
    if (this->getType() != UnitTypes::Terran_Bunker)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
    BroodwarImpl.addToCommandBuffer(new CommandUnloadAll(this));
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll(Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      this->unloadAll();
    }
    if (this->getType() != UnitTypes::Terran_Dropship && this->getType() != UnitTypes::Protoss_Shuttle && this->getType() != UnitTypes::Zerg_Overlord)
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandUnloadAll(this, BW::Position(position.x(), position.y())));
    return true;
  }
  //-------------------------------------------- CANCEL CONSTRUCTION -----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (!this->getType().isBuilding()) return false;
    if (!this->isCompleted())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
      BroodwarImpl.addToCommandBuffer(new CommandCancelConstruction(this));
    }
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getOrder() != Orders::ConstructingBuilding) return false;
    return this->stop();
  }
  //----------------------------------------------- CANCEL MORPH ---------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->getType().isBuilding())
    {
      return this->cancelConstruction();
    }
    if (this->isMorphing())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
      BroodwarImpl.addToCommandBuffer(new CommandCancelMorph(this));
    }
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isTraining())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelTrainLast(), sizeof(BW::Orders::CancelTrainLast));
      BroodwarImpl.addToCommandBuffer(new CommandCancelTrain(this));
    }
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isTraining() && (int)(this->getTrainingQueue().size()) > slot)
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelTrain(slot), sizeof(BW::Orders::CancelTrain));
      BroodwarImpl.addToCommandBuffer(new CommandCancelTrain(this, slot));
    }
    return true;
  }
  //----------------------------------------------- CANCEL ADDON ---------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    BroodwarImpl.addToCommandBuffer(new CommandCancelAddon(this));
    return true;
  }
  //---------------------------------------------- CANCEL RESEARCH -------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isResearching())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
      BroodwarImpl.addToCommandBuffer(new CommandCancelResearch(this));
    }
    return true;
  }
  //---------------------------------------------- CANCEL UPGRADE --------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    if (this->isUpgrading())
    {
      this->orderSelect();
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
      BroodwarImpl.addToCommandBuffer(new CommandCancelUpgrade(this));
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Stimpacks:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
        break;
      case BW::TechID::TankSiegeMode:
      {
        if (this->isSieged())
          this->unsiege();
        else
          this->siege();
      } break;
      case BW::TechID::PersonnelCloaking:
      case BW::TechID::CloakingField:
      {
        if(this->isCloaked())
          this->decloak();
        else
          this->cloak();
      } break;
      case BW::TechID::Burrowing:
      {
        if(this->isBurrowed())
          this->unburrow();
        else
          this->burrow();
      } break;
      case BW::TechID::ArchonWarp:
      {
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
      } break;
      case BW::TechID::DarkArchonMeld:
      {
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
      } break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::DarkSwarm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::DarkSwarm), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::DisruptionWeb:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::CastDisruptionWeb), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::EMPShockwave:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::EmpShockwave), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Ensnare:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Ensnare), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Maelstorm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::CastMaelstrom), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::NuclearStrike:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::NukePaint), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Plague:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Plague), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::PsionicStorm:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::PsiStorm), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Recall:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::Teleport), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::ScannerSweep:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::PlaceScanner), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::SpiderMines:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::PlaceMine), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::StasisField:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(), position.y()), BW::OrderID::StasisField), sizeof(BW::Orders::Attack));
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getPlayer() != Broodwar->self())
    {
      BroodwarImpl.setLastError(Errors::Unit_Not_Owned);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Consume:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Consume), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::DefensiveMatrix:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::DefensiveMatrix), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Feedback:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastFeedback), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Hallucination:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Hallucination1), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Healing:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::MedicHeal1), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Infestation:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::InfestMine2), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Irradiate:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Irradiate), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Lockdown:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::MagnaPulse), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::MindControl:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastMindControl), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::OpticalFlare:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastOpticalFlare), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Parasite:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CastParasite), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::Restoration:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Restoration), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::SpawnBroodlings:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::SummonBroodlings), sizeof(BW::Orders::Attack));
        break;
      case BW::TechID::YamatoGun:
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::FireYamatoGun1), sizeof(BW::Orders::Attack));
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    return true;
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void UnitImpl::orderSelect()
  {
    if (BroodwarImpl.quietSelect)
    {
      BW::Unit * * select = new BW::Unit * [1];
      select[0] = this->getOriginalRawData();
      BW::selectUnits(1, select);
      delete [] select;
      //Broodwar.IssueCommand((PBYTE)&BW::Orders::SelectSingle(this),sizeof(BW::Orders::SelectSingle));
    }
    else
    {
      BW::Unit * * list = new BW::Unit * [2];
      list[0] = this->getOriginalRawData();
      list[1] = NULL;
      int one = 1;
      BW::selectUnitsHelperSTD(one, list, true, true);
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::UnitType UnitImpl::getType() const
  {
    if ( this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch1
         || this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch2
         || this->getRawDataLocal()->unitID.id == BW::UnitID::Resource_MineralPatch3)
    {
      return BWAPI::UnitTypes::Resource_Mineral_Field;
    }
    else
    {
      return BWAPI::UnitType(this->getRawDataLocal()->unitID.id);
    }
  }
  //------------------------------------------------ GET BW TYPE ---------------------------------------------
  BW::UnitType UnitImpl::getBWType() const
  {
    return this->getRawDataLocal()->unitID;
  }
  //----------------------------------------------- GET QUEUE ------------------------------------------------
  BW::UnitType* UnitImpl::getBuildQueueSync() const
  {
    return this->getRawData()->buildQueue;
  }
  //-------------------------------------------- GET QUEUE LOCAL  --------------------------------------------
  BW::UnitType* UnitImpl::getBuildQueue() const
  {
    return this->getRawDataLocal()->buildQueue;
  }
  //--------------------------------------------- GET QUEUE SLOT ---------------------------------------------
  u8 UnitImpl::getBuildQueueSlotSync() const
  {
    return this->getRawData()->buildQueueSlot;
  }
  //------------------------------------------ GET QUEUE SLOT LOCAL ------------------------------------------
  u8 UnitImpl::getBuildQueueSlot() const
  {
    return this->getOriginalRawData()->buildQueueSlot;
  }
  //----------------------------------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable:4311)
  UnitImpl* UnitImpl::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    int index=((int)unit - (int)BW::BWDATA_UnitNodeTable) / 336;
    if (index<0 || index>=BW::UNIT_ARRAY_MAX_LENGTH)
    {
      if (BroodwarImpl.invalidIndices.find(index)==BroodwarImpl.invalidIndices.end())
      {
        BroodwarImpl.newUnitLog->log("Error: Found new invalid unit index: %d, broodwar address: 0x%x",index,unit);
        BroodwarImpl.invalidIndices.insert(index);
      }
      return NULL;
    }
    return BroodwarImpl.getUnit(((int)unit - (int)BW::BWDATA_UnitNodeTable) / 336);
  }
#pragma warning (pop)
  //----------------------------------------------------------------------------------------------------------
  bool UnitImpl::isMineral() const
  {
    return this->getType() == BW::UnitID::Resource_MineralPatch1 ||
           this->getType() == BW::UnitID::Resource_MineralPatch2 ||
           this->getType() == BW::UnitID::Resource_MineralPatch3;
  }
  char position[100];
  char indexName[50];
  char targetIndex[50];
  char orderTargetIndex[50];
  char owner[100];
  char unitName[100];
  char orderName[100];
  char connectedUnit[100];
  char message[400];
  //----------------------------------------------------------------------------------------------------------
  std::string UnitImpl::getName() const
  {
    sprintf_s(position, 100, "Position = (%d,%d)", this->getPosition().x(),
              this->getPosition().y());

    sprintf_s(indexName, 50, "[%d]", this->getIndex());

    if (this->getTarget() == NULL)
      strcpy_s(targetIndex, 50, "Target:[NULL]");
    else
      sprintf_s(targetIndex, 50, "Target:[%d](%s)", (int)this->getTarget(), this->getTarget()->getType().getName().c_str());

    if (this->getOrderTarget() == NULL)
      strcpy_s(orderTargetIndex, 50, "OrderTarget:[NULL]");
    else
      sprintf_s(orderTargetIndex, 50, "OrderTarget:[%d](%s)", (int)(this->getOrderTarget()), this->getOrderTarget()->getType().getName().c_str());

    if (this->getPlayer() != NULL)
      sprintf_s(owner, 100, "Player = (%s)", this->getPlayer()->getName().c_str());
    else
      sprintf_s(owner, 100, "error owner id = (%d)", this->getOriginalRawData()->playerID);

    sprintf_s(unitName, 100, "(%s)", this->getType().getName().c_str());

    if (this->getChild() == NULL)
      sprintf_s(connectedUnit, 100, "(childUnit1 = NULL)");
    else
      sprintf_s(connectedUnit, 100, "(childUnit1 = %s)", this->getChild()->getType().getName().c_str());

    sprintf_s(orderName, 100, "(%s)", this->getOrder().getName().c_str());
    sprintf_s(message, 400, "%s %s %s %s %s %s %s %s", unitName,
              orderName,
              indexName,
              position,
              targetIndex,
              orderTargetIndex,
              owner,
              connectedUnit);

    return std::string(message);
  }
  //---------------------------------------------- UPDATE NEXT -----------------------------------------------
  UnitImpl* UnitImpl::getNext() const
  {
#ifdef PARANOID_DEBUG
#pragma warning(push)
#pragma warning(disable:4311)
    if (this->getOriginalRawData()->nextUnit != NULL)
    {
      if (((int)this->getOriginalRawData()->nextUnit - (int)BW::BWXFN_UnitNodeTable) / BW::UNIT_SIZE_IN_BYTES >= BW::UNIT_ARRAY_MAX_LENGTH)
        BWAPI::Broodwar.fatalError->log("Unit array too small, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
      if ((int)this->getOriginalRawData()->nextUnit < (int)BW::BWXFN_UnitNodeTable)
        BWAPI::Broodwar.fatalError->log("Unit array begins at bad location, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
    }
#pragma warning(pop)
#endif PARANOID_DEBUG
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->nextUnit);
  }
  //-------------------------------------------- GET ORDER TIMER ---------------------------------------------
  int UnitImpl::getOrderTimer() const
  {
    return this->getRawDataLocal()->mainOrderTimer;
  }
  //---------------------------------------- GET REMAINING BUILD TIME ----------------------------------------
  int UnitImpl::getRemainingBuildTime() const
  {
    return this->getRawDataLocal()->remainingBuildTime;
  }
  int UnitImpl::getRemainingTrainTime() const
  {
    if (this->getRawDataLocal()->currentBuildUnit)
      return this->getRawDataLocal()->currentBuildUnit->remainingBuildTime;
    return 0;
  }
  //----------------------------------------------------------------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
  //----------------------------------------------------------------------------------------------------------
};
