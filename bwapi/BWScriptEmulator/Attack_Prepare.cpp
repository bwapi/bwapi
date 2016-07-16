#include "Attack_Prepare.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

Attack_Prepare attack_prepare_impl(Enum::ATTACK_PREPARE);
Attack_Prepare quick_attack_impl(Enum::QUICK_ATTACK);

bool Attack_Prepare::execute(aithread &thread) const
{
  // Execute
  // AI_AttackManager(thisLocation, 1, 0);

  if ( this->getOpcode() == Enum::QUICK_ATTACK )
    MainController.dwAttackTime = Broodwar->elapsedTime() - 175;

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
