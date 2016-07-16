#include "DefenseClear.h"
#include "Controller.h"

using namespace AISCRIPT;

DefenseClear defenseclear_gg_impl(AISCRIPT::Enum::DEFENSECLEAR_GG);
DefenseClear defenseclear_ag_impl(AISCRIPT::Enum::DEFENSECLEAR_AG);
DefenseClear defenseclear_ga_impl(AISCRIPT::Enum::DEFENSECLEAR_GA);
DefenseClear defenseclear_aa_impl(AISCRIPT::Enum::DEFENSECLEAR_AA);

bool DefenseClear::execute(aithread &thread) const
{
  // Execute
  MainController.DefenseClear(this->getOpcode() - AISCRIPT::Enum::DEFENSECLEAR_GG);

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
