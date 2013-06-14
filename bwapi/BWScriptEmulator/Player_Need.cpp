#include "Player_Need.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Player_Need);

bool Player_Need::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bBuildCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bBuildCount, wUnitType) );

  //if ( bBuildCount <= 30 && wBuildType < UnitTypes::None )
  //  this->pTown->MacroManagerSet(MACRO_BUILD, bBuildCount, wBuildType, 80, true);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%3u %s %3u", bBuildCount, AISCRIPT::getUnitName(wUnitType));
  return true;
}
