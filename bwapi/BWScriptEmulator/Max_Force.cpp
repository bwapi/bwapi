#include "Max_Force.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Max_Force);

bool Max_Force::execute(aithread &thread) const
{
  // Parameters
  WORD wMaxForce;
  thread.readTuple( std::tie( wMaxForce ) );

  // Assign to controller
  MainController.wMaxForce = wMaxForce;

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u", wMaxForce);
  return true;
}
