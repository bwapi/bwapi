#include "Give_Money.h"
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Give_Money);

bool Give_Money::execute(aithread &thread) const
{
  // Minerals
  if ( Broodwar->self()->minerals() < 500 )
    for ( int i = 0; i < 2000; i += 500 )
      Broodwar->sendText("whats mine is mine");

  // Gas
  if ( Broodwar->self()->gas() < 500 )
    for ( int i = 0; i < 2000; i += 500 )
      Broodwar->sendText("breathe deep");

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
