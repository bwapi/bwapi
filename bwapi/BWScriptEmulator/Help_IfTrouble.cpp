#include "Help_IfTrouble.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Help_IfTrouble);

bool Help_IfTrouble::execute(aithread &thread) const
{
  // HelpIfTrouble();
  
  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
