#include "Farms_Timing.h"
#include "Controller.h"

using namespace AISCRIPT;

Farms_Timing farms_notiming_impl(AISCRIPT::Enum::FARMS_NOTIMING);
Farms_Timing farms_timing_impl(AISCRIPT::Enum::FARMS_TIMING);

bool Farms_Timing::execute(aithread &thread) const
{
  if ( this->getOpcode() == AISCRIPT::Enum::FARMS_NOTIMING )  // off
    MainController.setFlags(CONTROLLER_FARMS_TIMING_OFF);
  else                                                          // on
    MainController.clearFlags(CONTROLLER_FARMS_TIMING_OFF);
  
  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
