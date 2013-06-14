#include "Define_Max.h"
#include <tuple>
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Define_Max);

bool Define_Max::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bCount;
  WORD wType;
  thread.readTuple( std::tie(bCount, wType) );

  if ( wType < UnitTypes::None )
    MainController.bDefineMax[wType] = bCount;

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wType) );
  return true;
}
