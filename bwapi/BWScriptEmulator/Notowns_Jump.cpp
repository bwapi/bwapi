#include "Notowns_Jump.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Notowns_Jump);

bool Notowns_Jump::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wUnitType, wOffset;
  thread.readTuple( std::tie(wUnitType, wOffset) );

  if ( Broodwar->self()->allUnitCount(wUnitType) == 0 )
    thread.setScriptOffset( wOffset );

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%s p_%04X", AISCRIPT::getUnitName(wUnitType), wOffset);
  return true;
}
