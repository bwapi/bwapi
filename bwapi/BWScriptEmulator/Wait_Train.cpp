#include "Wait_Train.h"
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Wait_Train);

bool Wait_Train::execute(aithread &thread) const
{
  // Parameters
  BYTE bCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bCount, wUnitType) );

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wUnitType) );

  // Perform actions
  if ( GetStandardUnitCount(wUnitType) < bCount )
    return thread.retry();
  return thread.noretry();
}
