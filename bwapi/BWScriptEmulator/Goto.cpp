#include "Goto.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Goto);

bool Goto::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wOffset;
  thread.readTuple( std::tie(wOffset) );

  /// Set the new script offset
  thread.setScriptOffset( wOffset );

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "p_%04X", wOffset);
  return true;
}
