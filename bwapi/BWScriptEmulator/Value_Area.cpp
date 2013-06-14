#include "Value_Area.h"

using namespace AISCRIPT;

IMPLEMENT(Value_Area);

bool Value_Area::execute(aithread &thread) const
{
  // ValueArea(thread.getLocation());

  // Debug and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
