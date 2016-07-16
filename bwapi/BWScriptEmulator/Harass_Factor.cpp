#include "Harass_Factor.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Harass_Factor);

bool Harass_Factor::execute(aithread &thread) const
{
  // Parameters
  WORD wUnknown;
  thread.readTuple( std::tie( wUnknown ) );

  // HarassFactor(wUnk);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u", wUnknown);
  return true;
}
