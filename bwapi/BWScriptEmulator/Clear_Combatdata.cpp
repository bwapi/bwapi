#include "Clear_Combatdata.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Clear_Combatdata);

bool Clear_Combatdata::execute(aithread &thread) const
{
  // ClearCombatData(this->location);

  // Debug and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
