#include "Guard_Resources.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Guard_Resources);

bool Guard_Resources::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wUnitType;
  thread.readTuple( std::tie(wUnitType) );

  // Execution
  // GuardResources(wType);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%s", AISCRIPT::getUnitName(wUnitType));
  return true;
}
