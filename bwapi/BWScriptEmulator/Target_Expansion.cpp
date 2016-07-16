#include "Target_Expansion.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Target_Expansion);

bool Target_Expansion::execute(aithread &thread) const
{
  // Set campaign flag
  MainController.setFlags(CONTROLLER_TARGET_EXPANSION);

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
