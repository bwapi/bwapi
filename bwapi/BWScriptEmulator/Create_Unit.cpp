#include "Create_Unit.h"
#include "Controller.h"

using namespace AISCRIPT;

Create_Unit create_nuke_impl(AISCRIPT::Enum::CREATE_NUKE);
Create_Unit create_unit_impl(AISCRIPT::Enum::CREATE_UNIT);

bool Create_Unit::execute(aithread &thread) const
{
  if ( this->getOpcode() == Enum::CREATE_NUKE )
  {
    // Do nothing/Debug (Can't be emulated)
    thread.saveDebug(Text::Green, this->getOpcode());
  }
  else if ( this->getOpcode() == Enum::CREATE_UNIT )
  {
    // Params
    WORD wUnitType, x, y;
    thread.readTuple( std::tie(wUnitType, x, y) );

    // Do nothing/Debug (Can't be emulated)
    thread.saveDebug(Text::Green, this->getOpcode(), "%s %u %u", AISCRIPT::getUnitName(wUnitType), x, y);
  }
  
  return true;
}
