#include "Prep_Down.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Prep_Down);

bool Prep_Down::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bSaveCount, bMinCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bSaveCount, bMinCount, wUnitType) );

  // Get count
  int count = GetStandardUnitCount(wUnitType, false) - bSaveCount;
  if ( count < bMinCount )
    count = bMinCount;
  
  // Add to attack group
  // AI_AttackAdd(count, wUnitType);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u %u %s", bSaveCount, bMinCount, AISCRIPT::getUnitName(wUnitType));
  return true;
}
