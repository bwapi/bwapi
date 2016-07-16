#include "Try_Townpoint.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Try_Townpoint);

bool Try_Townpoint::execute(aithread &thread) const
{
  // Params
  BYTE bExpansionCount;
  WORD wBlock;
  thread.readTuple( std::tie(bExpansionCount, wBlock) );
  
  // @TODO
  // if ( GetExpansionCount() == bExpansions )
  //   thread.setScriptOffset(wJump);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u %04X", bExpansionCount, wBlock);
  return true;
}
