#include "Place_Guard.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Place_Guard);

bool Place_Guard::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wUnitType;
  BYTE bGuardType;
  thread.readTuple( std::tie(wUnitType, bGuardType) );

  // Execution
  // this->pTown->PlaceGuard(bGuardType, wUnitType);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%s %u", AISCRIPT::getUnitName(wUnitType), bGuardType);
  return true;
}
