#include "Nuke_Rate.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Nuke_Rate);

bool Nuke_Rate::execute(aithread &thread) const
{
  // Parameters
  BYTE bNukeRate;
  thread.readTuple( std::tie( bNukeRate ) );

  // Assign to controller
  MainController.bNukeRate = bNukeRate;

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
