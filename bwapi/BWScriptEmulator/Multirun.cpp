#include "Multirun.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Multirun);

bool Multirun::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wOffset;
  thread.readTuple( std::tie(wOffset) );

  // Create new script thread
  AICreateThread(wOffset, thread.getLocation().center(), thread.town());

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "p_%04X", wOffset);
  return true;
}
