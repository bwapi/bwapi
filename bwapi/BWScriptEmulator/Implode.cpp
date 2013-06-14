#include "Implode.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Implode);

bool Implode::execute(aithread &thread) const
{
  // Implode();

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
