#include "Capt_Expand.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Capt_Expand);

bool Capt_Expand::execute(aithread &thread) const
{

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
