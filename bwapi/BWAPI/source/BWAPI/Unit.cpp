#include "Unit.h"

#include "../BWAPI/TargetType.h"
#include "../BWAPI/AbilityPrototype.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Player.h"
#include "../BWAPI/Globals.h"
#include "../BWAPI/CommandTrain.h"
#include "../BWAPI/CommandRightClick.h"

#include "../BW/Unit.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTypes.h"

#include "../../../Util/Logger.h"

#include <math.h>

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  Unit::Unit(BW::Unit* unitData, 
             BW::Unit* originalUnit,
             BW::Unit* unitDataLocal)
  :bwUnit(unitData)
  ,bwOriginalUnit(originalUnit)
  ,bwUnitLocal(unitDataLocal)
  {
  }
  //----------------------------- DESTRUCTOR -----------------------------------
  Unit::~Unit()
  {
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getHealthPoints() const
  {
    return bwUnit->healthPoints;
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u32 Unit::getShieldPoints() const
  {
    return bwUnit->shieldPoints;
  }
  //--------------------------  GET UNIT PROTOTYPE -----------------------------
  const UnitPrototype* Unit::getPrototype() const
  {
    if (this->getType() < 228)
      return BWAPI::Prototypes::unitIDToPrototypeTable[this->getType()];
    else 
      return NULL;
  }
  //--------------------------------- ORDER ------------------------------------
  void Unit::order(int commandCode, const BW::Position& target)
  {
  }
  //------------------------------- GET OWNER ----------------------------------
  Player* Unit::getOwner() const
  {
    if (this->bwUnit->playerID < 12)
      return Broodwar.players[this->bwUnit->playerID];
    else 
      return NULL;
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isValid() const
  {
    if (this->getOriginalRawData()->playerID > 11)
       return false;
    if (this->isMineral())
      return  !this->getOriginalRawData()->orderFlags.getBit(BW::OrderFlags::willWanderAgain);
    else         
      return (this->getHealthPoints() > 0)&&
             this->getPrototype() != NULL;
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isReady() const
  {
    return this->isValid() &&
           this->getRawData()->remainingBuildTime == 0;
  }
  //-------------------------------- GET POSITION ------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->bwUnit->position;
  }
  //-------------------------------- GET TATGET --------------------------------
  Unit* Unit::getTarget()
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //-------------------------------- GET TATGET --------------------------------
  const Unit* Unit::getTarget() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawData()->orderTargetUnit);
  }
  //----------------------------- GET TATGET LCCAL -----------------------------
  const Unit* Unit::getTargetLocal() const
  {
    return Unit::BWUnitToBWAPIUnit(this->getRawDataLocal()->orderTargetUnit);
  }
  //-------------------------------- GET RAW DATA ------------------------------
  BW::Unit *Unit::getRawData()
  {
    return this->bwUnit;
  }
  //-------------------------------- GET RAW DATA ------------------------------
  const BW::Unit *Unit::getRawData() const
  {
    return this->bwUnit;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  BW::Unit *Unit::getRawDataLocal()
  {
    return this->bwUnitLocal;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  const BW::Unit *Unit::getRawDataLocal() const
  {
    return this->bwUnitLocal;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  BW::Unit *Unit::getOriginalRawData()
  {
    return this->bwOriginalUnit;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  const BW::Unit *Unit::getOriginalRawData() const
  {
    return this->bwOriginalUnit;
  }
  //-------------------------------- GET ORDER ID ------------------------------
  BW::OrderID::Enum Unit::getOrderID() const
  {
    return this->getRawData()->orderID;
  }
  //-------------------------------- GET ORDER ID ------------------------------
  BW::OrderID::Enum Unit::getOrderIDLocal() const
  {
    return this->getRawDataLocal()->orderID;
  }
  //------------------------------- GET DISTANCE -------------------------------

  #pragma warning(push)
  #pragma warning(disable:4244)
  u16 Unit::getDistance(Unit *unit) const
  {
   u32 result;
   if (unit == this)
     return 0;
   if (this->getPosition().y - this->getPrototype()->dimensionUp() <= unit->getPosition().y + unit->getPrototype()->dimensionDown())
     if (this->getPosition().y + this->getPrototype()->dimensionDown() >= unit->getPosition().y - unit->getPrototype()->dimensionUp())
       if (this->getPosition().x > unit->getPosition().x)
         result = this->getPosition().x - this->getPrototype()->dimensionLeft() - unit->getPosition().x - unit->getPrototype()->dimensionRight();
       else
         result = unit->getPosition().x - unit->getPrototype()->dimensionRight() - this->getPosition().x - this->getPrototype()->dimensionLeft();

   if (this->getPosition().x - this->getPrototype()->dimensionLeft() <= unit->getPosition().x + unit->getPrototype()->dimensionRight())
     if (this->getPosition().x + this->getPrototype()->dimensionRight() >= unit->getPosition().x - unit->getPrototype()->dimensionLeft())
       if (this->getPosition().y > unit->getPosition().y)
         result = this->getPosition().y - this->getPrototype()->dimensionUp() - unit->getPosition().y - unit->getPrototype()->dimensionDown();
       else
         result = unit->getPosition().y - unit->getPrototype()->dimensionDown() - this->getPosition().y - this->getPrototype()->dimensionUp();

   if (this->getPosition().x > unit->getPosition().x)
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x - this->getPrototype()->dimensionLeft(),
                                  this->getPosition().y - this->getPrototype()->dimensionUp(),
                                  unit->getPosition().x + unit->getPrototype()->dimensionRight(),
                                  unit->getPosition().y + unit->getPrototype()->dimensionDown());
     else
       result = this->getDistance(this->getPosition().x - this->getPrototype()->dimensionLeft(),
                                 this->getPosition().y + this->getPrototype()->dimensionDown(),
                                 unit->getPosition().x + unit->getPrototype()->dimensionRight(),
                                 unit->getPosition().y - unit->getPrototype()->dimensionUp());
   else
     if (this->getPosition().y > unit->getPosition().y)
       result = this->getDistance(this->getPosition().x + this->getPrototype()->dimensionRight(),
                                this->getPosition().y - this->getPrototype()->dimensionUp(),
                                unit->getPosition().x - unit->getPrototype()->dimensionLeft(),
                                unit->getPosition().y + unit->getPrototype()->dimensionDown());
     else
       result = this->getDistance(this->getPosition().x + this->getPrototype()->dimensionRight(),
                                this->getPosition().y + this->getPrototype()->dimensionDown(),
                                unit->getPosition().x - unit->getPrototype()->dimensionLeft(),
                                unit->getPosition().y - unit->getPrototype()->dimensionUp());
   if (result > 0)
     return result;
   else
     return 0;
  }
  //-------------------------------- GET CENTER DISTANCE -----------------------
  u16 Unit::getCenterDistance(Unit *unit) const
  {
    return this->getDistance(this->getPosition().x, 
                             this->getPosition().y,
                             unit->getPosition().x, 
                             unit->getPosition().y);
  }
  //-------------------------------- GET DISTANCE ------------------------------
  u16 Unit::getDistance(int x1, int y1, int x2, int y2) const
  {
    return sqrt((long double)((long double)x1 - x2)*((long double)x1 - x2) +
                (long double)((long double)y1 - y2)*((long double)y1 - y2));
  }
  #pragma warning(pop)
  //------------------------------ HAS EMPTY QUEUE -----------------------------
  bool Unit::hasEmptyBuildQueue(void)
  {
     return this->getBuildQueue()[this->getBuildQueueSlot()] == 0xe4;
  }
  //------------------------------ HAS EMPTY QUEUE -----------------------------
  bool Unit::hasEmptyBuildQueueLocal(void)
  {
    if (this->getBuildQueueSlotLocal() < 5)
      return this->getBuildQueueLocal()[this->getBuildQueueSlotLocal()] == BW::UnitType::None;
    else
      return false;
  }
  //------------------------------- ORDER RIGHT CLICK -------------------------
  void Unit::orderRightClick(u16 x,u16 y)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(BW::Position(x, y)), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, BW::Position(x, y)));
  }
  //------------------------------- ORDER RIGHT CLICK -------------------------
  void Unit::orderRightClick(Unit *target)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::RightClick(target), sizeof(BW::Orders::RightClick)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, target));
  }
  //------------------------------------ BUILD --------------------------------
  void Unit::build(u16 tileX, u16 tileY, UnitPrototype *type)
  {
    this->orderSelect();
    Broodwar.IssueCommand((PBYTE)&BW::Orders::MakeBuilding(tileX, tileY, type->getUnitID()), sizeof(BW::Orders::MakeBuilding)); 
    Broodwar.addToCommandBuffer(new CommandRightClick(this, BW::Position(tileX*BW::TileSize, tileY*BW::TileSize)));
  }
  //------------------------------- ORDER SELECT ------------------------------
  void Unit::orderSelect()
  {
    if (Broodwar.quietSelect)
    {
      BW::Unit * * select = new BW::Unit * [1];
      select[0] = this->getOriginalRawData();
      void (_stdcall* selectUnits)(int, BW::Unit * *) = (void (_stdcall*) (int, BW::Unit * *)) 0x004C04E0;
	     selectUnits(1, select);
      delete [] select;
      //Broodwar.IssueCommand((PBYTE)&BW::Orders::SelectSingle(this),sizeof(BW::Orders::SelectSingle)); 
    }
    else
    {
      BW::Unit * * list = new BW::Unit * [2];
      list[0] = this->getOriginalRawData();
	     list[1] = NULL;
      int one = 1;
      void (_stdcall* selectUnitsHelperSTD)(int, BW::Unit * *, bool, bool) = (void (_stdcall*) (int, BW::Unit * *, bool, bool))0x0049AB90;
      selectUnitsHelperSTD(one, list, true, true);
    }
  }
  //---------------------------------- GET TYPE --------------------------------
  BW::UnitType::Enum Unit::getType() const
  {
    return this->getRawData()->unitID;
  }
  //---------------------------------- GET QUEUE -------------------------------
  BW::UnitType::Enum* Unit::getBuildQueue()
  {
    return this->getRawData()->buildQueue;
  }
  //-------------------------------- GET QUEUE LOCAL  --------------------------
  BW::UnitType::Enum* Unit::getBuildQueueLocal()
  {
    return this->getRawDataLocal()->buildQueue;
  }
  //----------------------------------- TRAIN UNIT -----------------------------
  void Unit::trainUnit(UnitPrototype *type)
  {
    this->orderSelect();
    Broodwar.addToCommandBuffer(new CommandTrain(this, type));
    Broodwar.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type->getUnitID()), 0x3);
  }
  //-------------------------------- GET QUEUE SLOT ----------------------------
  u8 Unit::getBuildQueueSlot() const
  {
    return this->getRawData()->buildQueueSlot;
  }
  //------------------------------- GET QUEUE SLOT LOCAL -----------------------
  u8 Unit::getBuildQueueSlotLocal() const
  {
    return this->getOriginalRawData()->buildQueueSlot;
  }
  //----------------------------------------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return Broodwar.getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
  }
  #pragma warning (pop)
  //----------------------------------------------------------------------------
  bool Unit::isMineral() const
  {
     return this->getType() == BW::UnitType::Resource_MineralPatch1 ||
            this->getType() == BW::UnitType::Resource_MineralPatch2 ||
            this->getType() == BW::UnitType::Resource_MineralPatch3;
  }
  char position[100];
  char index[50];
  char targetIndex[50];
  char owner[100];
  char unitName[100];
  char orderName[100];
  char message[400];
  //----------------------------------------------------------------------------
  std::string Unit::getName() const
  {
    
    sprintf(position, "Position = (%4u,%4u)", this->getPosition().x, 
                                             this->getPosition().y);
      
    #pragma warning(push)
    #pragma warning(disable:4311)
  
    sprintf(index, "[%4d]", ((int)this->getOriginalRawData() - (int)BW::BWXFN_UnitNodeTable)/336);
   
    if (this->getTargetLocal() == NULL)
      strcpy(targetIndex, "[NULL]");
    else
      sprintf(targetIndex, "[%4d]", ((int)this->getTargetLocal()->getOriginalRawData() - (int)BW::BWXFN_UnitNodeTable)/336);
  
    #pragma warning(pop)

    if (this->getOwner() != NULL)
      sprintf(owner,"Player = (%10s)",this->getOwner()->getName());
    else
      sprintf(owner,"error owner id = (%d)",this->getOriginalRawData()->playerID);
     
    if (this->getPrototype() != NULL)
      sprintf(unitName,"(%21s)", this->getPrototype()->getName().c_str());
    else
      sprintf(unitName,"(unitID = %12u)", this->getType());
  
    sprintf(orderName,"(%22s)", BW::OrderID::orderName(this->getOrderIDLocal()).c_str());
    sprintf(message,"%s %s %s %s ->%s Player = (%10s)", unitName,
                                                        orderName,
                                                        index,
                                                        position,
                                                        targetIndex,
                                                        owner);
  
    return std::string(message);
  }
  //--------------------------------- GET NEXT ---------------------------------
  BWAPI::Unit *Unit::getNext()
  {
    return this->next;
  }
  //-------------------------------- UPDATE NEXT -------------------------------
  void Unit::updateNext()
  {
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
    this->next = Unit::BWUnitToBWAPIUnit(this->getOriginalRawData()->nextUnit);
    if (this->next != NULL)
      this->next->updateNext();
  }
  //------------------------------- GET ORDER TIMER ----------------------------
  u8 Unit::getOrderTimer() const
  {
     return this->getOriginalRawData()->mainOrderTimer;
  }
  //----------------------------------------------------------------------------
};
