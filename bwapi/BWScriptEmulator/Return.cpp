#include "Return.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Return);

bool Return::execute(aithread &thread) const
{
  thread.saveDebug(Text::Green, this->getOpcode());
  bool result = thread.ret();
  
  // Error handling, kill the script if the callstack is empty
  if ( !result )
  {
    thread.killThread();
    thread.noretry();
    return false;
  }

  // Success
  return true;
}
