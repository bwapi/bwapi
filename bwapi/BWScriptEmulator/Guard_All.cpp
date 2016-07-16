#include "Guard_All.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Guard_All);

bool Guard_All::execute(aithread &thread) const
{
  //for each ( Unit u in self->getUnits() ) {}
    // MakeGuard(u);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
