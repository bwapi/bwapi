#include "Unit.h"

#include <math.h>

#include <Util/Logger.h>

#include "BWAPI/Player.h"
#include "Globals.h"
#include "CommandTrain.h"
#include "CommandBuild.h"
#include "CommandAttackLocation.h"
#include "CommandRightClick.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitType.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Unit::Unit(BW::Unit* unitData, 
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
  Unit::~Unit()
  {
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  u16 Unit::health() const
  {
    return this->getRawDataLocal()->healthPoints;
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  u32 Unit::shield() const
  {
    return this->getRawDataLocal()->shieldPoints;
  }
  //------------------------------------------- GET ENERGY POINTS --------------------------------------------
  u16 Unit::energy() const
  {
    return this->getRawDataLocal()->energy; /* Unverified */
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* Unit::getOwner() const
  {
    if (this->getRawDataLocal()->playerID < 12)
      return (Player*)Broodwar.players[this->bwUnit->playerID];
    else 
      return NULL;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool Unit::isValid() const
  {
    if (this->getRawDataLocal()->playerID > 11)
       return false;
    if (this->isMineral())
      return  !this->getRawDataLocal()->orderFlags.getBit(BW::OrderFlags::willWanderAgain);
    else         
      return this->health() > 0 &&
             this->getType().isValid();
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool Unit::isReady() const
  {
    return this->isValid() &&
           this->isCompleted();
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool Unit::isVisible() const
  {
    if (this->getOwner()==BWAPI::Broodwar.self())
    {
      return true;
    }
    if (!BWAPI::Broodwar.map.visible(this->getTilePosition().x,this->getTilePosition().y))
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
  bool Unit::isCompleted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Completed);
  }
  //------------------------------------------------ IS LIFTED -----------------------------------------------
  bool Unit::isLifted() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::InAir) &&
           this->getRawDataLocal()->unitID.isBuilding();
  }
  //----------------------------------------------- IS BURROWED ----------------------------------------------
  bool Unit::isBurrowed() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Burrowed);
  }
  //------------------------------------------------ IS CLOAKED ----------------------------------------------
  bool Unit::isCloaked() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Cloaked);
  }
  //------------------------------------------------ IS DISABLED ---------------------------------------------
  bool Unit::isDisabled() const
  {
    return this->getRawDataLocal()->status.getBit(BW::StatusFlags::Disabled);
  }
  //---------------------------------------------- IS SELECTED -----------------------------------------------
  bool Unit::isSelected() const
  {
    if (BWAPI::Broodwar.isFlagEnabled(BWAPI::Flag::UserInput)==false)
      return false;
    return this->userSelected;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void Unit::setSelected(bool selectedState)
  {
    this->userSelected=selectedState;
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->getRawDataLocal()->position;
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  BW::TilePosition Unit::getTilePosition() const
  {
    return BW::Position(this->getPosition().x - this->getType().getTileWidth()*BW::TILE_SIZE/2,
                        this->getPosition().y - this->getType().getTileHeight()*BW::TILE_SIZE/2);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  Unit* Unit::getTarget() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->targetUnit);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  Unit* Unit::getOrderTarget() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* Unit::getBuildUnit() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* Unit::getChild() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->childInfoUnion.childUnit1);
  }
  //------------------------------------------ GET TATGET POSITION -------------------------------------------
  BW::Position Unit::getTargetPosition() const
  {
   return this->getRawDataLocal()->moveToPos;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::Unit* Unit::getRawData() const
  {
    return this->bwUnit;
  }
  //------------------------------------------- GET RAW DATA LOCAL -------------------------------------------
  BW::Unit* Unit::getRawDataLocal() const
  {
    return this->bwUnitLocal;
  }
  //----------------------------------------- GET ORIGINAL RAW DATA ------------------------------------------
  BW::Unit* Unit::getOriginalRawData() const
  {
    return this->bwOriginalUnit;
  }
  //---------------------------------------------- GET ORDER ID ----------------------------------------------
  BW::OrderID::Enum Unit::getOrderID() const
  {
    return this->getRawDataLocal()->orderID;
  }
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  BW::OrderID::Enum Unit::getSecondaryOrderID() const
  {
    return this->getRawDataLocal()->secondaryOrderID;
  }
  //---------------------------------------------- IS IDLE ---------------------------------------------------
  bool Unit::isIdle() const
  {
    return (this->getOrderID() == BW::OrderID::Guard ||
            this->getOrderID() == BW::OrderID::Stop ||
            this->getOrderID() == BW::OrderID::Pickup1 ||
            this->getOrderID() == BW::OrderID::Nothing2 ||
            this->getOrderID() == BW::OrderID::Medic ||
            this->getOrderID() == BW::OrderID::Carrier ||
            this->getOrderID() == BW::OrderID::Critter ||
            this->getOrderID() == BW::OrderID::NukeTrain);
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------

  #pragma warning(push)
  #pragma warning(disable:4244)
  u16 Unit::getDistance(Unit *unit) const
  {
   u32 result;
   if (unit == this)
     return 0;
   if (this->getPosition().y - this->getType().dimensionUp() <= unit->getPosition().y + unit->getType().dimensionDown())
     if (this->getPosition().y + this->getType().dimensionDown() >= unit->getPosition().y - unit->getType().dimensionUp())
       if (this->getPosition().x > unit->getPosition().x)
         result = this->getPosition().x - this->getType().dimensionLeft() - unit->getPosition().x - unit->getType().dimensionRight();
       else
         result = unit->getPosition().x - unit->getType().dimensionRight() - this->getPosition().x - this->getType().dimensionLeft();

   if (this->getPosition().x - this->getType().dimensionLeft() <= unit->getPosition().x + unit->getType().dimensionRight())
     if (this->getPosition().x + this->getType().dimensionRight() >= unit->getPosition().x - unit->getType().dimensionLeft())
       if (this->getPosition().y > unit->getPosition().y)
         result = this->getPosition().y - this->getType().dimensionUp() - unit->getPosition().y - unit->getType().dimensionDown();
       else
         result = unit->getPosition().y - unit->getType().dimensionDown() - this->getPosition().y - this->getType().dimensionUp();

   if (this->getPosition().x > unit->getPosition().x)
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x - this->getType().dimensionLeft(),
                                  this->getPosition().y - this->getType().dimensionUp(),
                                  unit->getPosition().x + unit->getType().dimensionRight(),
                                  unit->getPosition().y + unit->getType().dimensionDown());
     else
       result = this->getDistance(this->getPosition().x - this->getType().dimensionLeft(),
                                 this->getPosition().y + this->getType().dimensionDown(),
                                 unit->getPosition().x + unit->getType().dimensionRight(),
                                 unit->getPosition().y - unit->getType().dimensionUp());
   else
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x + this->getType().dimensionRight(),
                                this->getPosition().y - this->getType().dimensionUp(),
                                unit->getPosition().x - unit->getType().dimensionLeft(),
                                unit->getPosition().y + unit->getType().dimensionDown());
     else
       result = this->getDistance(this->getPosition().x + this->getType().dimensionRight(),
                                this->getPosition().y + this->getType().dimensionDown(),
                                unit->getPosition().x - unit->getType().dimensionLeft(),
                                unit->getPosition().y - unit->getType().dimensionUp());
   if (result > 0)
     return result;
   else
     return 0;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 Unit::getDistance(BW::Position position) const
  {
    return this->getDistance(this->getPosition().x, 
                             this->getPosition().y,
                             position.x, 
                             position.y);
  }
  //------------------------------------------ GET CENTER DISTANCE -------------------------------------------
  u16 Unit::getCenterDistance(Unit *unit) const
  {
    return this->getDistance(this->getPosition().x, 
                             this->getPosition().y,
                             unit->getPosition().x, 
                             unit->getPosition().y);
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  u16 Unit::getDistance(int x1, int y1, int x2, int y2) const
  {
    return sqrt((long double)((long double)x1 - x2)*((long double)x1 - x2) +
                (long double)((long double)y1 - y2)*((long double)y1 - y2));
  }
  #pragma warning(pop)
  //---------------------------------------------- IS TRAINING -----------------------------------------------
  bool Unit::isTraining() const
  {
    return !this->hasEmptyBuildQueue();
  }
  //------------------------------------------ GET TRAINING QUEUE --------------------------------------------
  std::list<BW::UnitType > Unit::getTrainingQueue() const
  {
    std::list<BW::UnitType > trainList;
    if (this->hasEmptyBuildQueue()) return trainList;
    int i = this->getBuildQueueSlot();
    int starti=i;
    trainList.push_back(this->getBuildQueue()[i]);
    i=(i + 1)%5;
    while(this->getBuildQueue()[this->getBuildQueueSlot()] != BW::UnitID::None && i!=starti)
    {
      trainList.push_back(this->getBuildQueue()[i]);
      i=(i + 1)%5;
    }
    return trainList;
  }
  //-------------------------------------------- HAS EMPTY QUEUE ---------------------------------------------
  bool Unit::hasEmptyBuildQueueSync() const
  {
    return this->getBuildQueueSync()[this->getBuildQueueSlotSync()] == BW::UnitID::None;
  }
  //----------------------------------------- HAS EMPTY QUEUE LOCAL ------------------------------------------
  bool Unit::hasEmptyBuildQueue() const
  {
    if (this->getBuildQueueSlot() < 5)
      return this->getBuildQueue()[this->getBuildQueueSlot()] == BW::UnitID::None;
    else
      return false;
  }
  //----------------------------------------- HAS FULL QUEUE LOCAL -------------------------------------------
  bool Unit::hasFullBuildQueue() const
  {
    return this->getBuildQueue()[(this->getBuildQueueSlot() + 1) % 5] != BW::UnitID::None;
  }
  //------------------------------------------- ORDER Attack Location ----------------------------------------
  void Unit::attackLocation(BW::Position position, u8 orderID)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::Attack(position, orderID), sizeof(BW::Orders::Attack)); 
    Broodwar.addToCommandBuffer(new CommandAttackLocation(this, position));
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  void Unit::rightClick(BW::Position position)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(position), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, position));
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  void Unit::rightClick(Unit *target)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(target), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, target));
  }
  //------------------------------------------------- BUILD --------------------------------------------------
  void Unit::build(BW::TilePosition position, BW::UnitType type)
  {
    this->orderSelect();
    if (!type.isAddon())
      Broodwar.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(position, type), sizeof(BW::Orders::MakeBuilding)); 
    else
      Broodwar.IssueCommand((PBYTE)&BW::Orders::MakeAddon(position, type), sizeof(BW::Orders::MakeAddon)); 
    Broodwar.addToCommandBuffer(new CommandBuild(this, type, position));
  }
  //------------------------------------------------- INVENT -------------------------------------------------
  void Unit::invent(BW::TechType tech)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::Invent(tech), sizeof(BW::Orders::Invent)); 
    Broodwar.addToCommandBuffer(new CommandInvent(this, tech));
  }
  //------------------------------------------------- INVENT -------------------------------------------------
  void Unit::upgrade(BW::UpgradeType upgrade)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::Upgrade(upgrade), sizeof(BW::Orders::Upgrade)); 
    Broodwar.addToCommandBuffer(new CommandUpgrade(this, upgrade));
  }
  //-------------------------------------------------- STOP --------------------------------------------------
  void Unit::stop()
  {
    //TODO: Handle stop order
  }
  //---------------------------------------------- HOLD POSITION ---------------------------------------------
  void Unit::holdPosition()
  {
    //TODO: Handle hold position order
  }
  //-------------------------------------------------- PATROL ------------------------------------------------
  void Unit::patrol(BW::Position position)
  {
    //TODO: Handle patrol order
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  void Unit::useTech(BW::TechType tech)
  {
    //TODO: Handle use tech order
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  void Unit::useTech(BW::TechType tech, BW::Position position)
  {
    //TODO: Handle use tech order
  }
  //------------------------------------------------- USE TECH -----------------------------------------------
  void Unit::useTech(BW::TechType tech, Unit* target)
  {
    //TODO: Handle use tech order
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void Unit::orderSelect()
  {
    if (Broodwar.quietSelect)
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
  BW::UnitType Unit::getType() const
  {
   return this->getRawDataLocal()->unitID;
  }
  //----------------------------------------------- GET QUEUE ------------------------------------------------
  BW::UnitType* Unit::getBuildQueueSync() const
  {
    return this->getRawData()->buildQueue;
  }
  //-------------------------------------------- GET QUEUE LOCAL  --------------------------------------------
  BW::UnitType* Unit::getBuildQueue() const
  {
    return this->getRawDataLocal()->buildQueue;
  }
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  void Unit::train(BW::UnitType type)
  {
    if (this->getType() == BW::UnitID::Zerg_Larva ||
        this->getType() == BW::UnitID::Zerg_Mutalisk ||
        this->getType() == BW::UnitID::Zerg_Hydralisk)
    {
      this->orderSelect();
      Broodwar.addToCommandBuffer(new CommandTrain(this, type));
      Broodwar.IssueCommand((PBYTE)&BW::Orders::UnitMorph(type), 0x3);
    }
    else if (this->getType() == BW::UnitID::Zerg_Hatchery ||
             this->getType() == BW::UnitID::Zerg_Lair ||
             this->getType() == BW::UnitID::Zerg_Spire ||
             this->getType() == BW::UnitID::Zerg_CreepColony)
    {
      this->orderSelect();
      Broodwar.addToCommandBuffer(new CommandTrain(this, type));
      Broodwar.IssueCommand((PBYTE)&BW::Orders::BuildingMorph(type), 0x3);
    }
    else
    {
      this->orderSelect();
      Broodwar.addToCommandBuffer(new CommandTrain(this, type));
      Broodwar.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type), 0x3);
    }
  }
  //--------------------------------------------- GET QUEUE SLOT ---------------------------------------------
  u8 Unit::getBuildQueueSlotSync() const
  {
    return this->getRawData()->buildQueueSlot;
  }
  //------------------------------------------ GET QUEUE SLOT LOCAL ------------------------------------------
  u8 Unit::getBuildQueueSlot() const
  {
    return this->getOriginalRawData()->buildQueueSlot;
  }
  //----------------------------------------------------------------------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return Broodwar.getUnit(((int)unit - (int)BW::BWDATA_UnitNodeTable)/336);
  }
  #pragma warning (pop)
  //----------------------------------------------------------------------------------------------------------
  bool Unit::isMineral() const
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
  std::string Unit::getName() const
  {
    sprintf_s(position, 100, "Position = (%u,%u)", this->getPosition().x, 
                                              this->getPosition().y);

    sprintf_s(indexName, 50, "[%d]", this->getIndex());

    if (this->getTarget() == NULL)
      strcpy_s(targetIndex, 50, "Target:[NULL]");
    else
      sprintf_s(targetIndex, 50, "Target:[%d](%s)", this->getTarget()->getIndex(), this->getTarget()->getType().getName());

    if (this->getOrderTarget() == NULL)
      strcpy_s(orderTargetIndex, 50, "OrderTarget:[NULL]");
    else
      sprintf_s(orderTargetIndex, 50, "OrderTarget:[%d](%s)", this->getOrderTarget()->getIndex(), this->getOrderTarget()->getType().getName());
  
    if (this->getOwner() != NULL)
      sprintf_s(owner, 100, "Player = (%s)", this->getOwner()->getName().c_str());
    else
      sprintf_s(owner, 100, "error owner id = (%d)", this->getOriginalRawData()->playerID);

    if (this->getType().isValid())
      sprintf_s(unitName, 100, "(%s)", this->getType().getName());
    else
      sprintf_s(unitName, 100, "(unitID = %u)", this->getType().getID());

    if (this->getType() == BW::UnitID::Terran_Vulture ||
        BWAPI::Unit::BWUnitToBWAPIUnit(this->getRawData()->childInfoUnion.childUnit1) == NULL)
      sprintf_s(connectedUnit, 100, "(childUnit1 = NULL)");
    else
      sprintf_s(connectedUnit, 100, "(childUnit1 = %s)", this->getChild()->getType().getName());

    sprintf_s(orderName, 100, "(%s)", BW::OrderID::orderName(this->getOrderID()).c_str());
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
  Unit* Unit::getNext() const
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
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->nextUnit);
  }
  //-------------------------------------------- GET ORDER TIMER ---------------------------------------------
  u8 Unit::getOrderTimer() const
  {
     return this->getRawDataLocal()->mainOrderTimer;
  }
  //---------------------------------------- GET REMAINING BUILD TIME ----------------------------------------
  u16 Unit::getRemainingBuildTime() const
  {
     return this->getRawDataLocal()->remainingBuildTime;
  }
  //----------------------------------------------------------------------------------------------------------
  u16 Unit::getIndex() const
  {
    return this->index;
  }
  //----------------------------------------------------------------------------------------------------------
};
