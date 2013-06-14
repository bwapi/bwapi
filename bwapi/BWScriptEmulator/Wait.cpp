#include "Wait.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Wait);

bool Wait::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wFrames;
  thread.readTuple( std::tie(wFrames) );

  thread.saveDebug(Text::Green, this->getOpcode(), "%u", wFrames);
  thread.noretry();
  return false;
}
