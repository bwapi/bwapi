#include "Allies_Watch.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Allies_Watch);

bool Allies_Watch::execute(aithread &thread) const
{
  // Params
  BYTE bExpansionId;
  WORD wBlock;
  thread.readTuple( std::tie(bExpansionId, wBlock) );
  
  // @TODO

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u %04X", bExpansionId, wBlock);
  return true;
}
