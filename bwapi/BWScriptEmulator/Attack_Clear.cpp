#include "Attack_Clear.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Attack_Clear);

bool Attack_Clear::execute(aithread &thread) const
{
  memset(MainController.attackGroups, 0, sizeof(MainController.attackGroups));
  MainController.wAttackGroup = 0;
  MainController.dwAttackTime = 0;

  // Save debug info and return
  thread.saveDebug(Text::Yellow, this->getOpcode());
  return true;
}
