#include "Set_Gencmd.h"
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Set_Gencmd);

bool Set_Gencmd::execute(aithread &thread) const
{
  // Set generic command target
  MainController.genCmdTarget = thread.getLocation();

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
