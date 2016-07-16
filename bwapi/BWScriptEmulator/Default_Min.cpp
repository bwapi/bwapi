#include "Default_Min.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Default_Min);

bool Default_Min::execute(aithread &thread) const
{
  // Parameters
  BYTE bDefaultMin;
  thread.readTuple( std::tie( bDefaultMin ) );

  // Assign to controller
  MainController.bDefaultMin = bDefaultMin;

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
