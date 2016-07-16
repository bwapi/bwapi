#include "Panic.h"
#include <tuple>
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Panic);

bool Panic::execute(aithread &thread) const
{
  // Retrieve parameters
  WORD wOffset;
  thread.readTuple( std::tie(wOffset) );

  // Set panic block
  MainController.wPanicBlock = wOffset;

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "p_%04X", wOffset);
  return true;
}
