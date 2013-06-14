#include "Check_Transports.h"
#include "Controller.h"

using namespace AISCRIPT;

Check_Transports check_transports_impl(AISCRIPT::Enum::CHECK_TRANSPORTS);
Check_Transports transports_off_impl(AISCRIPT::Enum::TRANSPORTS_OFF);

bool Check_Transports::execute(aithread &thread) const
{
  if ( this->getOpcode() == AISCRIPT::Enum::TRANSPORTS_OFF )  // off
    MainController.setFlags(CONTROLLER_TRANSPORTS_OFF);
  else                                                          // on
    MainController.clearFlags(CONTROLLER_TRANSPORTS_OFF);
  
  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
