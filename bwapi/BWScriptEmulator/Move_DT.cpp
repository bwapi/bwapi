#include "Move_DT.h"
#include <tuple>
#include <algorithm>

using namespace AISCRIPT;

IMPLEMENT(Move_DT);

bool Move_DT::execute(aithread &thread) const
{
  // Execute
  Unitset myUnits( Broodwar->self()->getUnits() );

  // NOTE: Not actual behaviour. Needs special AI Control & Captain assignment.
  // However this command is not important and serves little purpose.
  myUnits.erase_if(!((GetType == UnitTypes::Protoss_Dark_Templar || GetType == UnitTypes::Hero_Dark_Templar) && Exists && IsCompleted));
  myUnits.move( thread.getLocation().center() );

  // Debug and return
  thread.saveDebug(Text::Yellow, this->getOpcode());
  return true;
}
