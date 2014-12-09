#include "Use_Ability.h"
#include "Controller.h"

using namespace AISCRIPT;

Use_Ability nuke_location_impl(AISCRIPT::Enum::NUKE_LOCATION);
Use_Ability disruption_web_impl(AISCRIPT::Enum::DISRUPTION_WEB);
Use_Ability recall_location_impl(AISCRIPT::Enum::RECALL_LOCATION);

Unit GetSuitableCaster(int minEnergy, UnitType type)
{
  Unitset myUnits( Broodwar->self()->getUnits() );
  for ( auto u : myUnits )
  {
    if ( u->getType() == type && u->isIdle() && u->getEnergy() >= minEnergy )
      return u;
  }
  return nullptr;
}

bool Use_Ability::execute(aithread &thread) const
{
  // Get the appropriate tech and unit type for the opcode
  TechType tech;
  UnitType unit;
  switch ( this->getOpcode() )
  {
  case Enum::NUKE_LOCATION:
    tech = TechTypes::Nuclear_Strike;
    unit = UnitTypes::Terran_Ghost;
    break;
  case Enum::DISRUPTION_WEB:
    tech = TechTypes::Disruption_Web;
    unit = UnitTypes::Protoss_Corsair;
    break;
  case Enum::RECALL_LOCATION:
    tech = TechTypes::Recall;
    unit = UnitTypes::Protoss_Arbiter;
    break;
  }

  // Execute the ability after obtaining a unit
  Unit pCaster = GetSuitableCaster(tech.energyCost(), unit);
  if ( pCaster )
    pCaster->useTech(tech, thread.getLocation().center());

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
