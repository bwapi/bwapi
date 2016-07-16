#include "Random_Jump.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Random_Jump);

bool Random_Jump::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bChance;
  WORD wOffset;
  thread.readTuple( std::tie(bChance, wOffset) );

  /// Set the new script offset
  if ( (BYTE)rand() <= bChance )
    thread.setScriptOffset( wOffset );

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u p_%04X", bChance, wOffset);
  return true;
}
