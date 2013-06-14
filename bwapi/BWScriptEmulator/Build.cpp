#include "Build.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Build);

bool Build::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bBuildCount, bPriority;
  WORD wUnitType;
  thread.readTuple( std::tie(bBuildCount, wUnitType, bPriority) );

  //if ( bBuildCount <= 30 && wBuildType < UnitTypes::None )
  //  this->pTown->MacroManagerSet(MACRO_BUILD, bBuildCount, wBuildType, bPriority, false);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u %s %u", bBuildCount, AISCRIPT::getUnitName(wUnitType), bPriority);
  return true;
}
