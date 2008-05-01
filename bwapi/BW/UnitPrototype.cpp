#include "UnitPrototype.h"
#include "Unit.h"
#include "BW_Unit.h"
#include "AbilityPrototype.h"
#include "TargetType.h"
//----------------------------- CONSTRUCTOR -----------------------------------
UnitPrototype::UnitPrototype(const std::string& name, 
                             const u16          maxHealthPoints,
                             const u16          maxMana,
                             const u32          abilityFlags)
:name(name)
,maxHealthPoints(maxHealthPoints)
,maxMana(maxMana)
,abilityFlags(abilityFlags)
{
}
//----------------------------- DESTRUCTOR ------------------------------------
UnitPrototype::~UnitPrototype()
{
}
//------------------------------- GET NAME ------------------------------------
const std::string& UnitPrototype::getName() const
{
  return this->name;
}
//--------------------------- GET HEALT POINTS --------------------------------
u16 UnitPrototype::getMaxHealthPoints() const
{
  return this->maxHealthPoints;
}
//----------------------- GET HEALT POINTS FACTION ----------------------------
u16 UnitPrototype::getMaxMana() const
{
  return this->maxMana;
}
//------------------------------- CAN ORDER ----------------------------------
bool UnitPrototype::canOrder(const AbilityPrototype* const ability, Unit* target) const
{
  // Firstly, we check if the unit even has the ability
  if (!(this->getAbilityFlags() & ability->getAbilityFlag()))
    return false;
 return true;
}
//-------------------------------- CAN ORDER ---------------------------------
bool UnitPrototype::canOrder(const AbilityPrototype* const ability, BW_Position target) const
{
  if (!(ability->getTargetFlags() & TargetType::TARGET_LOCATION))
    return false;
  return true;
}
//-------------------------------- GET ABILITY FLAGS -------------------------
s32 UnitPrototype::getAbilityFlags() const
{
  return this->abilityFlags;
}
//----------------------------------------------------------------------------