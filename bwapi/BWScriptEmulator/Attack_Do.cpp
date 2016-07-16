#include "Attack_Do.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Attack_Do);

bool Attack_Do::execute(aithread &thread) const
{
  //if ( !AI_AttackManager(thisLocation, 1, 0) )
  //  return thread.retry();


  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return thread.noretry();
}
