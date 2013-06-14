#include "Call.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Call);

bool Call::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wOffset;
  thread.readTuple( std::tie(wOffset) );

  // Call the offset
  thread.call( wOffset );

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "p_%04X", wOffset);
  return true;
}
