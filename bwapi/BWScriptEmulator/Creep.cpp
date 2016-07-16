#include "Creep.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Creep);

bool Creep::execute(aithread &thread) const
{
  BYTE bCreepFlag;
  thread.readTuple( std::tie(bCreepFlag) );

  // Set/clear the creep flag
  if ( bCreepFlag == 4 )  // far
    MainController.setFlags(CONTROLLER_SPREAD_CREEP);
  else                    // near
    MainController.clearFlags(CONTROLLER_SPREAD_CREEP);

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u", bCreepFlag);
  return true;
}
