#include "Wait_Bunkers.h"
#include "Controller.h"

using namespace AISCRIPT;

Wait_Bunkers wait_bunkers_impl(AISCRIPT::Enum::WAIT_BUNKERS);
Wait_Bunkers wait_turrets_impl(AISCRIPT::Enum::WAIT_TURRETS);

bool Wait_Bunkers::execute(aithread &thread) const
{
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
