#include "Switch_Rescue.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Switch_Rescue);

bool Switch_Rescue::execute(aithread &thread) const
{
  // Become neutral, can't become rescuable
  Broodwar->enemies().setAlliance(true);

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
