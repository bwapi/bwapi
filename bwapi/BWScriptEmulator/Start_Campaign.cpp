#include "Start_Campaign.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Start_Campaign);

bool Start_Campaign::execute(aithread &thread) const
{
  // Set campaign flag
  MainController.setFlags(CONTROLLER_IS_CAMPAIGN);

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
