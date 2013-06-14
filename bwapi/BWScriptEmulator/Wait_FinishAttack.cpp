#include "Wait_FinishAttack.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Wait_FinishAttack);

bool Wait_FinishAttack::execute(aithread &thread) const
{
  // Debug
  thread.saveDebug(Text::Red, this->getOpcode());

  // Check if AI is attacking
  // if ( AIIsAttacking )
  {
    return thread.noretry();
  }
  return thread.retry();
}
