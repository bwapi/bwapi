#include "Build_Bunkers.h"
#include "Controller.h"

using namespace AISCRIPT;

Build_Bunkers build_bunkers_impl(AISCRIPT::Enum::BUILD_BUNKERS);
Build_Bunkers build_turrets_impl(AISCRIPT::Enum::BUILD_TURRETS);

bool Build_Bunkers::execute(aithread &thread) const
{
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
