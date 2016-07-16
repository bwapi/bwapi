#include "If_Dif.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(If_Dif);

bool If_Dif::execute(aithread &thread) const
{
  // Parameters
  BYTE bModifier, bValue;
  WORD wBlock;
  thread.readTuple( std::tie( bModifier, bValue, wBlock ) );
  
  // Compare diff value
  BYTE bDiff = MainController.bIfDif;
  if ( bModifier ? (bDiff > bValue) : (bDiff < bValue) )
    thread.setScriptOffset( wBlock );

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %u %04X", bModifier, bValue, wBlock);
  return true;
}
