#include "Time_Jump.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Time_Jump);

bool Time_Jump::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bTime;
  WORD wOffset;
  thread.readTuple( std::tie(bTime, wOffset) );

  /// Set the new script offset
  if ( Broodwar->elapsedTime() >= (bTime * 60) )
    thread.setScriptOffset( wOffset );

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u p_%04X", bTime, wOffset);
  return true;
}
