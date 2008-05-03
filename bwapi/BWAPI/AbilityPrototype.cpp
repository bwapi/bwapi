#include "AbilityPrototype.h"
//----------------------------- CONSTRUCTOR -----------------------------------
AbilityPrototype::AbilityPrototype(const u8           abilityIndex,
                                   const std::string& name,
                                   const u16          manaCost,
                                   const u16          targetFlags,
                                   const u16          mineralCost,
                                   const u16          gasCost)
:name(name)
,manaCost(manaCost)
,targetFlags(targetFlags)
,mineralCost(mineralCost)
,gasCost(gasCost)
,flagValue(1<<abilityIndex)
{
}
//----------------------------- DESTRUCTOR ------------------------------------
AbilityPrototype::~AbilityPrototype()
{
}
//------------------------------- GET NAME ------------------------------------
const std::string& AbilityPrototype::getName() const
{
  return this->name;
}
//----------------------------- GET MANA COST ---------------------------------
u16 AbilityPrototype::getManaCost() const
{
  return this->manaCost;
}
//----------------------------- GET TARGET FLAGS -----------------------------
u16 AbilityPrototype::getTargetFlags() const
{
  return this->targetFlags;
}
//----------------------------------------------------------------------------
s32 AbilityPrototype::getAbilityFlag() const
{
  return this->flagValue;
}
//----------------------------------------------------------------------------