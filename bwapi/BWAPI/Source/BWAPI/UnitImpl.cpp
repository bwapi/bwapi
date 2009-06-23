#include "UnitImpl.h"

#include <math.h>

#include <Util/Logger.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include "Globals.h"
#include "CommandTrain.h"
#include "CommandBuild.h"
#include "CommandAttackLocation.h"
#include "CommandAttackUnit.h"
#include "CommandPatrol.h"
#include "CommandHoldPosition.h"
#include "CommandStop.h"
#include "CommandRightClick.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"
#include "CommandRepair.h"

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
  :bwUnit(unitData)
  ,bwOriginalUnit(originalUnit)
  ,bwUnitLocal(unitDataLocal)
  ,index(index)
  ,userSelected(false)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int UnitImpl::health() const
  {
    return this->getRawDataLocal()->healthPoints;
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int UnitImpl::shield() const
  {
    return this->getRawDataLocal()->shieldPoints;
  }
  //------------------------------------------- GET ENERGY POINTS --------------------------------------------
  int UnitImpl::energy() const
  {
    return this->getRawDataLocal()->energy;
  }
  //-------------------------------------------- GET KILL COUNT ----------------------------------------------
  int UnitImpl::getKillCount() const
  {
    return this->getRawDataLocal()->killCount;
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* UnitImpl::getOwner() const
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
      return this->health() > 0 && this->getType()!=BWAPI::UnitTypes::None && this->getType()!=BWAPI::UnitTypes::Unknown;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool UnitImpl::isReady() const
  {
    return this->isValid() &&
           this->isCompleted();
  }
  //------------------------------------------------ IS LOADED -----------------------------------------------
  bool UnitImpl::isLoaded() const
  {
    return this->loaded;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool UnitImpl::isVisible() const
  {
    if (this->getOwner()==BWAPI::BroodwarImpl.self())
    {
      return true;
    }
    if (!BWAPI::BroodwarImpl.visible(this->getTilePosition().x(),this->getTilePosition().y()))
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
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool UnitImpl::isCompleted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Completed);
  }
  //------------------------------------------------ IS LIFTED -----------------------------------------------
  bool UnitImpl::isLifted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InAir) &&
           this->getRawDataLocal()->unitID.isBuilding();
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
  //------------------------------------------------ IS DISABLED ---------------------------------------------
  bool UnitImpl::isDisabled() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Disabled);
  }
  //------------------------------------------------ IS BLIND ------------------------------------------------
  bool UnitImpl::isBlind() const
  {
    return this->getRawDataLocal()->isBlind != 0;
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool UnitImpl::isBeingHealed() const // Not working right now
  {
    return this->getRawDataLocal()->isBeingHealed != 0;
  }
  //---------------------------------------------- IS UNDER STORM --------------------------------------------
  bool UnitImpl::isUnderStorm() const
  {
    return this->getRawDataLocal()->isUnderStorm != 0;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSelected() const
  {
    if (BWAPI::BroodwarImpl.isFlagEnabled(BWAPI::Flag::UserInput)==false)
      return false;
    return this->userSelected;
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool UnitImpl::isSieged() const
  {
    return this->getBWType().getID()==BW::UnitID::Terran_SiegeTankSiegeMode;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    this->userSelected=selectedState;
  }
  //---------------------------------------------- SET LOADED ------------------------------------------------
  void UnitImpl::setLoaded(bool loadedState)
  {
    this->loaded=loadedState;
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  Position UnitImpl::getPosition() const
  {
    return BWAPI::Position(this->getRawDataLocal()->position.x,this->getRawDataLocal()->position.y);
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  TilePosition UnitImpl::getTilePosition() const
  {
    return TilePosition(Position(this->getPosition().x() - this->getType().tileWidth()*BW::TILE_SIZE/2,
                        this->getPosition().y() - this->getType().tileHeight()*BW::TILE_SIZE/2));
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
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
  }
  //----------------------------------------------- GET CHILD ------------------------------------------------
  Unit* UnitImpl::getChild() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
  }
  //------------------------------------------ GET TARGET POSITION -------------------------------------------
  Position UnitImpl::getTargetPosition() const
  {
    return BWAPI::Position(this->getRawDataLocal()->moveToPos.x,this->getRawDataLocal()->moveToPos.y);
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
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  Order UnitImpl::getSecondaryOrder() const
  {
    return BWAPI::Order(this->getRawDataLocal()->secondaryOrderID);
  }
  //---------------------------------------------- IS IDLE ---------------------------------------------------
  bool UnitImpl::isIdle() const
  {
    return (this->getOrder() == BWAPI::Orders::Guard ||
            this->getOrder() == BWAPI::Orders::Stop ||
            this->getOrder() == BWAPI::Orders::Pickup1 ||
            this->getOrder() == BWAPI::Orders::Nothing2 ||
            this->getOrder() == BWAPI::Orders::Medic ||
            this->getOrder() == BWAPI::Orders::Carrier ||
            this->getOrder() == BWAPI::Orders::Critter ||
            this->getOrder() == BWAPI::Orders::NukeTrain);
  }
  //------------------------------------------------ IS MOVING -----------------------------------------------
  bool UnitImpl::isMoving() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Moving);
  }
  //--------------------------------------------- IS ACCELERATING --------------------------------------------
  bool UnitImpl::isAccelerating() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Accelerating);
  }
  //----------------------------------------------- IS BRAKING -----------------------------------------------
  bool UnitImpl::isBraking() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::Braking);
  }
  //------------------------------------------- IS STARTING ATTACK -------------------------------------------
  bool UnitImpl::isStartingAttack() const
  {
    return this->getRawDataLocal()->movementFlags.getBit(BW::MovementFlags::StartingAttack);
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------

  #pragma warning(push)
  #pragma warning(disable:4244)
  u16 UnitImpl::getDistance(Unit *unit) const
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
   else
     if (this->getPosition().y() > unit->getPosition().y())
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
  u16 UnitImpl::getCenterDistance(Unit *unit) const
  {
    return this->getDistance(this->getPosition().x(), 
                             this->getPosition().y(),
                             unit->getPosition().x(), 
                             unit->getPosition().y());
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 UnitImpl::getDistance(int x1, int y1, int x2, int y2) const
  {
    return sqrt((long double)((long double)x1 - x2)*((long double)x1 - x2) +
                (long double)((long double)y1 - y2)*((long double)y1 - y2));
  }
  #pragma warning(pop)
  //---------------------------------------------- IS TRAINING -----------------------------------------------
  bool UnitImpl::isTraining() const
  {
    return !this->hasEmptyBuildQueue();
  }
  //------------------------------------------ GET TRAINING QUEUE --------------------------------------------
  std::list<UnitType > UnitImpl::getTrainingQueue() const
  {
    std::list<UnitType > trainList;
    if (this->hasEmptyBuildQueue()) return trainList;
    int i = this->getBuildQueueSlot()%5;
    int starti=i;
    trainList.push_front(BWAPI::UnitType(this->getBuildQueue()[i].id));
    i=(i + 1)%5;
    while(this->getBuildQueue()[i] != BW::UnitID::None && i!=starti)
    {
      trainList.push_back(BWAPI::UnitType(this->getBuildQueue()[i].id));
      i=(i + 1)%5;
    }
    return trainList;
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
    if (this->getOwner()!=Broodwar->self()) return false;
    Order order=Orders::AttackMove;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(),position.y()), order.getID()), sizeof(BW::Orders::Attack)); 
    BroodwarImpl.addToCommandBuffer(new CommandAttackLocation(this, BW::Position(position.x(),position.y())));
    return true;
  }
  //--------------------------------------------- ORDER Attack Unit ------------------------------------------
  bool UnitImpl::attackUnit(Unit *target)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    Order order=Orders::AttackUnit;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, order.getID()), sizeof(BW::Orders::Attack)); 
    BroodwarImpl.addToCommandBuffer(new CommandAttackUnit(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Position position)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position(position.x(),position.y())), sizeof(BW::Orders::RightClick)); 
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, BW::Position(position.x(),position.y())));
    return true;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  bool UnitImpl::rightClick(Unit *target)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick)); 
    BroodwarImpl.addToCommandBuffer(new CommandRightClick(this, (UnitImpl*)target));
    return true;
  }
  //------------------------------------------------- BUILD --------------------------------------------------
  bool UnitImpl::build(TilePosition position, UnitType type1)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    BW::UnitType type(BW::UnitID::Enum(type1.getID()));
    this->orderSelect();
    if (!type.isAddon())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition(position.x(),position.y()), type), sizeof(BW::Orders::MakeBuilding)); 
    else
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition(position.x(),position.y()), type), sizeof(BW::Orders::MakeAddon)); 
    BroodwarImpl.addToCommandBuffer(new CommandBuild(this, type, BW::TilePosition(position.x(),position.y())));
    return true;
  }
  //------------------------------------------------ RESEARCH ------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BW::TechID::Enum techenum=static_cast<BW::TechID::Enum>(tech.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new CommandInvent(this, BW::TechType(techenum)));
    return true;
  }
  //------------------------------------------------- UPGRADE ------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BW::UpgradeID::Enum upgradeenum=static_cast<BW::UpgradeID::Enum>(upgrade.getID());
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade)); 
    BroodwarImpl.addToCommandBuffer(new CommandUpgrade(this, BW::UpgradeType(upgradeenum)));
    return true;
  }
  //-------------------------------------------------- STOP --------------------------------------------------
  bool UnitImpl::stop()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    this->getRawDataLocal()->orderID = BW::OrderID::Stop;
    BroodwarImpl.addToCommandBuffer(new CommandStop(this));
    return true;
  }
  //---------------------------------------------- HOLD POSITION ---------------------------------------------
  bool UnitImpl::holdPosition()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
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
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack(BW::Position(position.x(),position.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack)); 
    BroodwarImpl.addToCommandBuffer(new CommandPatrol(this, BW::Position(position.x(),position.y())));
    return true;
  }
  //-------------------------------------------------- REPAIR ------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new CommandRepair(this, (UnitImpl*)target));
    return true;
  }
  //-------------------------------------------------- MORPH -------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle morph (Zerg)
    return true;
  }
  //-------------------------------------------------- BURROW ------------------------------------------------
  bool UnitImpl::burrow()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if(!this->isBurrowed())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
    return true;
  }
  //------------------------------------------------- UNBURROW -----------------------------------------------
  bool UnitImpl::unburrow()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if(this->isBurrowed())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
    return true;
  }
  //-------------------------------------------------- SIEGE -------------------------------------------------
  bool UnitImpl::siege()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if (!this->isSieged())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
    return true;
  }
  //------------------------------------------------- UNSIEGE ------------------------------------------------
  bool UnitImpl::unsiege()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if (this->isSieged())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
    return true;
  }
  //-------------------------------------------------- CLOAK -------------------------------------------------
  bool UnitImpl::cloak()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if(!this->isCloaked())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
    return true;
  }
  //------------------------------------------------- DECLOAK ------------------------------------------------
  bool UnitImpl::decloak()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if(this->isCloaked())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
    return true;
  }
  //--------------------------------------------------- LIFT -------------------------------------------------
  bool UnitImpl::lift()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if(!this->isLifted())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
    return true;
  }
  //--------------------------------------------------- LAND -------------------------------------------------
  bool UnitImpl::land()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    if(this->isLifted())
      BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));//Doesn't work. Need to find Land order.
    return true;
  }
  //--------------------------------------------------- LOAD -------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle loadUnit (Bunker/Dropship/Shuttle/Overload)
    //Should handle both cases:
    //1) this is a transport unit and target is a loadable ground unit
    //2) this is a loadable ground unit and target is a tranport unit
    return true;
  }
  //-------------------------------------------------- UNLOAD ------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle unloadUnit (Bunker/Dropship/Shuttle/Overload)
    //Should handle both cases:
    //1) this is a transport unit and target is a loadable ground unit
    //2) this is a loadable ground unit and target is a tranport unit
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle loadAll (Bunker)
    return true;
  }
  //------------------------------------------------- UNLOADALL ----------------------------------------------
  bool UnitImpl::unloadAll(Position position)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle loadAll (Dropship/Shuttle/Overload)
    return true;
  }
  //-------------------------------------------- CANCEL CONSTRUCTION -----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle cancelConstruction
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle haltConstruction (Terran)
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    //TODO: Handle cancelTrain
    return true;
  }
  //----------------------------------------------- CANCEL TRAIN ---------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle cancelTrain for any slot
    return true;
  }
  //----------------------------------------------- CANCEL ADDON ---------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle cancelAddon
    return true;
  }
  //---------------------------------------------- CANCEL UPGRADE --------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle cancelUpgrade
    return true;
  }
  //---------------------------------------------- CANCEL RESEARCH -------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle cancelResearch
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
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
    }
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle use tech order
    return true;
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
    this->orderSelect();
    //TODO: Handle use tech order
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
    if ( this->getRawDataLocal()->unitID.id==BW::UnitID::Resource_MineralPatch1
      || this->getRawDataLocal()->unitID.id==BW::UnitID::Resource_MineralPatch2
      || this->getRawDataLocal()->unitID.id==BW::UnitID::Resource_MineralPatch3)
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
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    if (this->getOwner()!=Broodwar->self()) return false;
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
    return BroodwarImpl.getUnit(((int)unit - (int)BW::BWDATA_UnitNodeTable)/336);
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
  
    if (this->getOwner() != NULL)
      sprintf_s(owner, 100, "Player = (%s)", this->getOwner()->getName().c_str());
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
      if (((int)this->getOriginalRawData()->nextUnit - (int)BW::BWXFN_UnitNodeTable)/BW::UNIT_SIZE_IN_BYTES >= BW::UNIT_ARRAY_MAX_LENGTH)
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
  //----------------------------------------------------------------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
  //----------------------------------------------------------------------------------------------------------
};
