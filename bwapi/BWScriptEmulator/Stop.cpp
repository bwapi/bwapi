#include "Stop.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Stop);

bool Stop::execute(aithread &thread) const
{
  // Stop script execution on the current thread
  thread.killThread();

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  thread.noretry();
  return false;
}
