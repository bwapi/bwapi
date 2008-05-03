#include "Unit.h"

#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Player.h"
#include "../BWAPI/Globals.h"

#include "../BW/UnitData.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTypes.h"

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  Unit::Unit(BW::UnitData* unitData)
    :bwUnitData(unitData)
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
       case BW::UnitType::Protoss_Probe  : return BWAPI::Prototypes::Probe;
       case BW::UnitType::Protoss_Zealot : return BWAPI::Prototypes::Zealot;
       case BW::UnitType::Terran_SCV     : return BWAPI::Prototypes::SCV;
       case BW::UnitType::Zerg_Queen     : return BWAPI::Prototypes::Queen;
       default : return NULL;
     }

    return NULL;
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

#pragma warning(push)
#pragma warning(disable:4312)
  void (_stdcall*sendCommand)(u16, u16, u16, u16) = (void(_stdcall*)(u16, u16, u16, u16))BW::BWFXN_CommandUnit;
#pragma warning(pop)

  //--------------------------------- ORDER ------------------------------------
  void Unit::order(const AbilityPrototype* const ability, const BW::Position& target)
  {
    sendCommand(6, 0xe4, target.y, 0);
    //sendcommand(command, 0xe4, y, x);
  }
  //--------------------------------- ORDER ------------------------------------
  void Unit::order(int commandCode, const BW::Position& target)
  {
    sendCommand(commandCode, 0xe4, target.x, target.y);
    //sendcommand(command, 0xe4, y, x);
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
  //----------------------------------------------------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return bwUnitData->currentPos;
  }
  //----------------------------------------------------------------------------
};
