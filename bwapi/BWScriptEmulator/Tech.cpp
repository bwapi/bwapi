#include "Tech.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Tech);

bool Tech::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bPriority;
  WORD wTechType;
  thread.readTuple( std::tie(wTechType, bPriority) );

        //this->pTown->MacroManagerSet(MACRO_TECH, 1, wTechType, bPriority, false);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%s %3u", TechType(wTechType).c_str(), bPriority);
  return true;
}
