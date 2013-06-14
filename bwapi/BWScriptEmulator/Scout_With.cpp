#include "Scout_With.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Scout_With);

bool Scout_With::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wScoutType;
  thread.readTuple( std::tie(wScoutType) );

  // Do nothing (correct behaviour)

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%s", AISCRIPT::getUnitName(wScoutType) );
  return true;
}
