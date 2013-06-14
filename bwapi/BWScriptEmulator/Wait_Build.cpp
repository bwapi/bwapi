#include "Wait_Build.h"
#include "Controller.h"

using namespace AISCRIPT;

Wait_Build wait_build_impl(AISCRIPT::Enum::WAIT_BUILD);
Wait_Build wait_buildstart_impl(AISCRIPT::Enum::WAIT_BUILDSTART);

bool Wait_Build::execute(aithread &thread) const
{
  // Parameters
  BYTE bCount;
  WORD wType;
  thread.readTuple( std::tie(bCount, wType) );

  // debug
  thread.saveDebug(Text::Red, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wType) );

  // stuff
  if ( wType == Broodwar->self()->getRace().getWorker() )
  {
    // TOWN STUFF
  }

  //if ( // AI_BuildSomething >= bCount )
    //return thread.noretry();
  
  // Debug and return
  return thread.retry();
}
