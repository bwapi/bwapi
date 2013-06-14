#include "Eval_Harass.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Eval_Harass);

bool Eval_Harass::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wOffset;
  thread.readTuple( std::tie(wOffset) );

  //if ( !EvalHarass(this->location.center()) )
  //  thread.setScriptOffset( wOffset );

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "p_%04X", wOffset);
  return true;
}
