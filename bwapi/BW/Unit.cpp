#include "Unit.h"
#include "BW_Unit.h"
#include "UnitPrototypeDefinitions.h"
#include "Player.h"
//----------------------------- CONSTRUCTOR -----------------------------------
Unit::Unit(BW_Unit *rawData)
{
  this->rawData = rawData;
  switch (this->rawData->unitID)
  {
    case BW_UnitType::Protoss_Probe  : this->prototype = Prototypes::Probe; break;
    case BW_UnitType::Protoss_Zealot : this->prototype = Prototypes::Zealot; break;
    case BW_UnitType::Terran_SCV     : this->prototype = Prototypes::SCV; break;
    case BW_UnitType::Zerg_Queen     : this->prototype = Prototypes::Queen; break;
  }
}
//----------------------------- DESTRUCTOR -----------------------------------
Unit::~Unit()
{
}
//-------------------------- GET HEALTH POINTS --------------------------------
u16 Unit::getHealthPoints() const
{
  return rawData->healthPoints;
}
//----------------------- GET HEALT POINTS FACTION ----------------------------
u8 Unit::getHealthPointsFraction() const
{
  return rawData->healthPointsFraction;
}
//--------------------------  GET UNIT PROTOTYPE -----------------------------
const UnitPrototype* const Unit::getPrototype() const
{
  return this->prototype;
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
bool Unit::canOrder(const AbilityPrototype* const ability, BW_Position target) const
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
void Unit::order(const AbilityPrototype* const ability, BW_Position target)
{
}
//----------------------------------------------------------------------------
Player* Unit::getOwner() const
{
  return this->owner;
}
//----------------------------------------------------------------------------
