#include "Kill_Thread.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Kill_Thread);

bool Kill_Thread::execute(aithread &thread) const
{
  for ( auto i = aiThreadList.begin(); i != aiThreadList.end(); ++i )
  {
    if ( i->getFlags() & AI_THREAD_KILLABLE )
      i->killThread();
  }

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
