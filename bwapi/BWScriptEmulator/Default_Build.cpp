#include "Default_Build.h"
#include "Controller.h"

using namespace AISCRIPT;

Default_Build default_build_impl(AISCRIPT::Enum::DEFAULT_BUILD);
Default_Build defaultbuild_off_impl(AISCRIPT::Enum::DEFAULTBUILD_OFF);

bool Default_Build::execute(aithread &thread) const
{
  if ( this->getOpcode() == AISCRIPT::Enum::DEFAULTBUILD_OFF )  // off
    MainController.setFlags(CONTROLLER_DEFAULT_BUILD_OFF);
  else                                                          // on
    MainController.clearFlags(CONTROLLER_DEFAULT_BUILD_OFF);
  
  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
