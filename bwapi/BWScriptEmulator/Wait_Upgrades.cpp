#include "Wait_Upgrades.h"
#include <tuple>
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Wait_Upgrades);

bool Wait_Upgrades::execute(aithread &thread) const
{
  // Debug
  thread.saveDebug(Text::Green, this->getOpcode());

  // If upgrades are finished, then move on to next opcode
  if ( MainController.getFlags() & CONTROLLER_UPGRADES_FINISHED )
    return thread.noretry();
  
  // Otherwise retry this opcode
  return thread.retry();
}
