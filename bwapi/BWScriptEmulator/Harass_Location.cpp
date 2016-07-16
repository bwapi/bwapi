#include "Harass_Location.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Harass_Location);

bool Harass_Location::execute(aithread &thread) const
{
  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
