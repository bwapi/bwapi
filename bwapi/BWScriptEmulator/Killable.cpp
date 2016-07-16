#include "Killable.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Killable);

bool Killable::execute(aithread &thread) const
{
  thread.setFlags(AI_THREAD_KILLABLE);

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
