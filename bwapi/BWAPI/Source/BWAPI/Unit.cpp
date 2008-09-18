#include "Unit.h"

#include <math.h>

#include <Util/Logger.h>

#include "Player.h"
#include "Globals.h"
#include "CommandTrain.h"
#include "CommandBuild.h"
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
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Unit::~Unit()
  {
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  u16 Unit::getHealthPoints() const
  {
    return bwUnit->healthPoints;
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  u32 Unit::getShieldPoints() const
  {
    return bwUnit->shieldPoints;
  }
  //------------------------------------------------- ORDER --------------------------------------------------
  void Unit::order(int commandCode, const BW::Position& target)
  {
  }
  //----------------------------------------------- GET OWNER ------------------------------------------------
  Player* Unit::getOwner() const
  {
    if (this->bwUnit->playerID < 12)
      return Broodwar.players[this->bwUnit->playerID];
    else 
      return NULL;
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool Unit::isValid() const
  {
    if (this->getOriginalRawData()->playerID > 11)
       return false;
    if (this->isMineral())
      return  !this->getOriginalRawData()->orderFlags.getBit(BW::OrderFlags::willWanderAgain);
    else         
      return this->getHealthPoints() > 0 &&
             this->getType().isValid();
  }
  //------------------------------------------------ IS VALID ------------------------------------------------
  bool Unit::isReady() const
  {
    return this->isValid() &&
           this->isCompleted();
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool Unit::isCompleted() const
  {
    return this->getRawData()->status.getBit(BW::StatusFlags::Completed);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->getRawData()->position;
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  BW::TilePosition Unit::getTilePosition() const
  {
    return BW::Position(this->getPosition().x - this->getType().getTileWidth()*BW::TILE_SIZE/2,
                        this->getPosition().y - this->getType().getTileHeight()*BW::TILE_SIZE/2);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  Unit* Unit::getTarget()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->targetUnit);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  const Unit* Unit::getTarget() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->targetUnit);
  }
  //-------------------------------------------- GET TATGET LOCAL --------------------------------------------
  Unit* Unit::getTargetLocal()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->targetUnit);
  }
  //-------------------------------------------- GET TATGET LOCAL --------------------------------------------
  const Unit* Unit::getTargetLocal() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->targetUnit);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  Unit* Unit::getOrderTarget()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //----------------------------------------------- GET TATGET -----------------------------------------------
  const Unit* Unit::getOrderTarget() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //-------------------------------------------- GET TATGET LCCAL --------------------------------------------
  const Unit* Unit::getOrderTargetLocal() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->orderTargetUnit);
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* Unit::getBuildUnit()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->currentBuildUnit);
  }
  //------------------------------------------ GET TATGET POSITION -------------------------------------------
  BW::Position Unit::getTargetPosition() const
  {
   return this->getRawData()->moveToPos;
  }
  //--------------------------------------- GET TARGET POSITION LOCAL ----------------------------------------
  BW::Position Unit::getTargetPositionLocal() const
  {
    return this->getRawDataLocal()->moveToPos;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::Unit* Unit::getRawData()
  {
    return this->bwUnit;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  const BW::Unit* Unit::getRawData() const
  {
    return this->bwUnit;
  }
  //------------------------------------------- GET RAW DATA LOCAL -------------------------------------------
  BW::Unit* Unit::getRawDataLocal()
  {
    return this->bwUnitLocal;
  }
  //------------------------------------------- GET RAW DATA LOCAL -------------------------------------------
  const BW::Unit* Unit::getRawDataLocal() const
  {
    return this->bwUnitLocal;
  }
  //----------------------------------------- GET ORIGINAL RAW DATA ------------------------------------------
  BW::Unit* Unit::getOriginalRawData()
  {
    return this->bwOriginalUnit;
  }
  //----------------------------------------- GET ORIGINAL RAW DATA ------------------------------------------
  const BW::Unit* Unit::getOriginalRawData() const
  {
    return this->bwOriginalUnit;
  }
  //---------------------------------------------- GET ORDER ID ----------------------------------------------
  BW::OrderID::Enum Unit::getOrderID() const
  {
    return this->getRawData()->orderID;
  }
  //----------------------------------------- GET SECONDARY ORDER ID -----------------------------------------
  BW::OrderID::Enum Unit::getSecondaryOrderID() const
  {
    return this->getRawData()->secondaryOrderID;
  }
  //-------------------------------------- GET SECONDARY ORDER ID LOCAL --------------------------------------
  BW::OrderID::Enum Unit::getSecondaryOrderIDLocal() const
  {
    return this->getRawDataLocal()->secondaryOrderID;
  }
  //---------------------------------------------- GET ORDER ID ----------------------------------------------
  BW::OrderID::Enum Unit::getOrderIDLocal() const
  {
    return this->getRawDataLocal()->orderID;
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
  //-------------------------------------------- HAS EMPTY QUEUE ---------------------------------------------
  bool Unit::hasEmptyBuildQueue(void)
  {
    return this->getBuildQueue()[this->getBuildQueueSlot()] == BW::UnitID::None;
  }
  //-------------------------------------------- HAS EMPTY QUEUE ---------------------------------------------
  bool Unit::hasEmptyBuildQueueLocal(void)
  {
    if (this->getBuildQueueSlotLocal() < 5)
      return this->getBuildQueueLocal()[this->getBuildQueueSlotLocal()] == BW::UnitID::None;
    else
      return false;
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  void Unit::orderRightClick(BW::Position position)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(position), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, position));
  }
  //------------------------------------------- ORDER RIGHT CLICK --------------------------------------------
  void Unit::orderRightClick(Unit *target)
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
   return this->getRawData()->unitID;
  }
  //----------------------------------------------- GET QUEUE ------------------------------------------------
  BW::UnitType* Unit::getBuildQueue()
  {
    return this->getRawData()->buildQueue;
  }
  //-------------------------------------------- GET QUEUE LOCAL  --------------------------------------------
  BW::UnitType* Unit::getBuildQueueLocal()
  {
    return this->getRawDataLocal()->buildQueue;
  }
  //----------------------------------------------- TRAIN UNIT -----------------------------------------------
  void Unit::trainUnit(BW::UnitType type)
  {
    this->orderSelect();
    Broodwar.addToCommandBuffer(new CommandTrain(this, type));
    Broodwar.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type), 0x3);
  }
  //--------------------------------------------- GET QUEUE SLOT ---------------------------------------------
  u8 Unit::getBuildQueueSlot() const
  {
    return this->getRawData()->buildQueueSlot;
  }
  //------------------------------------------ GET QUEUE SLOT LOCAL ------------------------------------------
  u8 Unit::getBuildQueueSlotLocal() const
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
    return Broodwar.getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
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
    
    sprintf(position, "Position = (%u,%u)", this->getPosition().x, 
                                              this->getPosition().y);
      
  
    sprintf(indexName, "[%d]", this->getIndex());
   
    if (this->getTarget() == NULL)
      strcpy(targetIndex, "Target:[NULL]");
    else
      sprintf(targetIndex, "Target:[%d](%s)", this->getTarget()->getIndex(), this->getTarget()->getType().getName());

    if (this->getOrderTarget() == NULL)
      strcpy(orderTargetIndex, "OrderTarget:[NULL]");
    else
      sprintf(orderTargetIndex, "OrderTarget:[%d](%s)", this->getOrderTarget()->getIndex(), this->getOrderTarget()->getType().getName());
  

    if (this->getOwner() != NULL)
      sprintf(owner,"Player = (%s)",this->getOwner()->getName());
    else
      sprintf(owner,"error owner id = (%d)",this->getOriginalRawData()->playerID);
     
    if (this->getType().isValid())
      sprintf(unitName, "(%s)", this->getType().getName());
    else
      sprintf(unitName, "(unitID = %u)", this->getType().getID());
      
    if (BWAPI::Unit::BWUnitToBWAPIUnit(this->getRawData()->childInfoUnion.childUnit1) == NULL)
      sprintf(connectedUnit, "(childUnit1 = NULL)");
    else
      sprintf(connectedUnit, "(childUnit1 = %s)", BWAPI::Unit::BWUnitToBWAPIUnit(this->getRawData()->childInfoUnion.childUnit1)->getType().getName());
  
    sprintf(orderName,"(%s)", BW::OrderID::orderName(this->getOrderID()).c_str());
    sprintf(message,"%s %s %s %s %s %s %s %s", unitName,
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
  Unit* Unit::getNext()
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
    return Unit::BWUnitToBWAPIUnit(this->getOriginalRawData()->nextUnit);
  }
  //-------------------------------------------- GET ORDER TIMER ---------------------------------------------
  u8 Unit::getOrderTimer() const
  {
     return this->getOriginalRawData()->mainOrderTimer;
  }
  //----------------------------------------------------------------------------------------------------------
  u16 Unit::getIndex() const
  {
    return this->index;
  }
  //----------------------------------------------------------------------------------------------------------
};
