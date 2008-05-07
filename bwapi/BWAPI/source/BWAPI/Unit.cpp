#include "Unit.h"

#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Player.h"
#include "../BWAPI/Globals.h"
#include "../BWAPI/CommandTrain.h"
//#include "../BWAPI/Game.h"

#include "../BW/UnitData.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTypes.h"

#include <math.h>

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  Unit::Unit(BW::UnitData* unitData, 
             BW::UnitData* originalUnitData,
             BW::UnitData* unitDataLocal)
    :bwUnitData(unitData)
    ,bwOriginalUnitData(originalUnitData)
    ,bwUnitDataLocal(unitDataLocal)
  {
  }
  //----------------------------- DESTRUCTOR -----------------------------------
  Unit::~Unit()
  {
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getHealthPoints() const
  {
    return bwUnitData->healthPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getHealthPointsFraction() const
  {
    return bwUnitData->healthPointsFraction;
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getShieldPoints() const
  {
    return bwUnitData->shieldPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getShieldPointsFraction() const
  {
    return bwUnitData->shieldPointsFraction;
  }
  //--------------------------  GET UNIT PROTOTYPE -----------------------------
  const UnitPrototype* const Unit::getPrototype() const
  {
     switch (this->bwUnitData->unitID)
     {
       case BW::UnitType::Terran_SCV             : return BWAPI::Prototypes::SCV;
       case BW::UnitType::Terran_CommandCenter   : return BWAPI::Prototypes::CommandCenter;
       case BW::UnitType::Terran_SupplyDepot     : return BWAPI::Prototypes::SupplyDepot;
       case BW::UnitType::Terran_Barracks        : return BWAPI::Prototypes::Barracks;
       case BW::UnitType::Terran_Marine          : return BWAPI::Prototypes::Marine;
       /** Protoss*/
       case BW::UnitType::Protoss_Probe          : return BWAPI::Prototypes::Probe;
       case BW::UnitType::Protoss_Zealot         : return BWAPI::Prototypes::Zealot;
       case BW::UnitType::Protoss_Nexus          : return BWAPI::Prototypes::Nexus;
       case BW::UnitType::Protoss_Pylon          : return BWAPI::Prototypes::Pylon;
       case BW::UnitType::Protoss_Gateway        : return BWAPI::Prototypes::Gateway;
       /** Zerg */
       case BW::UnitType::Zerg_Queen             : return BWAPI::Prototypes::Queen;
       case BW::UnitType::Zerg_Larva             : return BWAPI::Prototypes::Larva;
       case BW::UnitType::Zerg_Drone             : return BWAPI::Prototypes::Drone;
       case BW::UnitType::Zerg_Zergling          : return BWAPI::Prototypes::Zergling;
       case BW::UnitType::Zerg_Overlord          : return BWAPI::Prototypes::Overlord;
       case BW::UnitType::Zerg_Hatchery          : return BWAPI::Prototypes::Hatchery;
       case BW::UnitType::Zerg_SpawningPool      : return BWAPI::Prototypes::SpawningPool;
       /** Neutral */
       case BW::UnitType::Resource_MineralPatch1 :
       case BW::UnitType::Resource_MineralPatch2 :
       case BW::UnitType::Resource_MineralPatch3 : return BWAPI::Prototypes::Minerals;
       default : return NULL;
     }
  }
  //------------------------------ CAN ORDER -----------------------------------
  bool Unit::canOrder(const AbilityPrototype* const ability, Unit* target) const
  {
    if (!this->getPrototype()->canOrder(ability, target))
      return false;

    if (this == target &&
      !(ability->getTargetFlags() & TargetType::TARGET_SELF))
      return false;

    if (this->getOwner() == target->getOwner() &&
      !(ability->getTargetFlags() & TargetType::TARGET_MY_UNITS))
      return false;

    if (this->getOwner() == target->getOwner() &&
      !(ability->getTargetFlags() & TargetType::TARGET_OPPONENTS_UNITS))
      return false;

    return true;
  }
  //------------------------------ CAN ORDER -----------------------------------
  bool Unit::canOrder(const AbilityPrototype* const ability, const BW::Position& target) const
  {
    if (!this->getPrototype()->canOrder(ability, target))
      return false;
    return true;
  }
  //-------------------------------- ORDER -------------------------------------
  void Unit::order(const AbilityPrototype* const ability, Unit* target)
  {
  }
 //--------------------------------- ORDER ------------------------------------
  void Unit::order(const AbilityPrototype* const ability, const BW::Position& target)
  {
  }
  //--------------------------------- ORDER ------------------------------------
  void Unit::order(int commandCode, const BW::Position& target)
  {
  }
  //------------------------------- GET OWNER ----------------------------------
  Player* Unit::getOwner() const
  {
    return Broodwar.players[this->bwUnitData->playerID];
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isValid() const
  {
    return this->getHealthPoints() > 0 || 
           this->getHealthPointsFraction() > 0;
  }
  //-------------------------------- GET POSITION ------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->bwUnitData->currentPos;
  }
  //-------------------------------- GET RAW DATA ------------------------------
  BW::UnitData *Unit::getRawData()
  {
    return this->bwUnitData;
  }
  //------------------------------ GET RAW DATA LOCAL --------------------------
  BW::UnitData *Unit::getRawDataLocal()
  {
    return this->bwUnitDataLocal;
  }
  //------------------------------ GET ORIGINAL RAW DATA -----------------------
  BW::UnitData *Unit::getOriginalRawData()
  {
    return this->bwOriginalUnitData;
  }
  //-------------------------------- GET ORDER ID ------------------------------
  BW::OrderID::Enum Unit::getOrderID() const
  {
    return this->bwUnitData->orderID;
  }
  //------------------------------- GET DISTANCE -------------------------------

  #pragma warning(push)
  #pragma warning(disable:4244)
  u16 Unit::getDistance(Unit *unit) const
  {
   
    return sqrt((long double)((s32)this->getPosition().x - unit->getPosition().x)*((s32)this->getPosition().x - unit->getPosition().x) +
                             ((s32)this->getPosition().y - unit->getPosition().y)*((s32)this->getPosition().y - unit->getPosition().y));
  }
  #pragma warning(pop)
  //------------------------------ HAS EMPTY QUEUE -----------------------------
  bool Unit::hasEmptyQueue(void)
  {
     return this->getQueue()[this->getQueueSlot()] == 0xe4;
  }
  //------------------------------ HAS EMPTY QUEUE -----------------------------
  bool Unit::hasEmptyQueueLocal(void)
  {
     return this->getQueueLocal()[this->getQueueSlotLocal()] == 0xe4;
  }
  //-------------------------------- ORDER MOVE --------------------------------
  void Unit::orderMove(u16 x,u16 y, Unit *target)
  {
    this->orderSelect();
    if (target == NULL)
      Broodwar.IssueCommand((PBYTE)&BW::Orders::Move(x, y), sizeof(BW::Orders::Move)); 
    else
      Broodwar.IssueCommand((PBYTE)&BW::Orders::MoveTarget(target), sizeof(BW::Orders::MoveTarget)); 
  }
  //-------------------------------- ORDER SELECT --------------------------------
  void Unit::orderSelect()
  {
    BW::UnitData * * list = new BW::UnitData * [2];
    list[0] = this->getOriginalRawData();
	   list[1] = NULL;
    int one = 1;
    void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
    selectUnitsHelperSTD(one, list, true, true);
    //Broodwar.IssueCommand((PBYTE)&BW::Orders::SelectSingle(this),sizeof(BW::Orders::SelectSingle)); 
  }
  //---------------------------------- GET TYPE --------------------------------
  BW::UnitType::Enum Unit::getType()
  {
    return this->getRawData()->unitID;
  }
  //---------------------------------- GET QUEUE -------------------------------
  BW::UnitType::Enum* Unit::getQueue()
  {
    return this->getRawData()->queue;
  }
  //-------------------------------- GET QUEUE LOCAL  --------------------------
  BW::UnitType::Enum* Unit::getQueueLocal()
  {
    return this->getRawDataLocal()->queue;
  }
  //----------------------------------- TRAIN UNIT -----------------------------
  void Unit::trainUnit(UnitPrototype *type)
  {
    Broodwar.IssueCommand((PBYTE)&BW::Orders::TrainUnit(type->getUnitID()), 0x3);
    Broodwar.addToCommandBuffer(new CommandTrain(this, type));
  }
  //-------------------------------- GET QUEUE SLOT ----------------------------
  u8 Unit::getQueueSlot()
  {
    return this->getRawData()->queueSlot;
  }
  //------------------------------- GET QUEUE SLOT LOCAL -----------------------
  u8 Unit::getQueueSlotLocal()
  {
    return this->getOriginalRawData()->queueSlot;
  }
  //----------------------------------------------------------------------------
};
